/*!
 * @brief	シンプルなモデルシェーダー。
 */

static const int MAX_DIRECTION_LIGHT = 4;	//ディレクションライトの最大数
static const int MAX_POINT_LIGHT = 1000;	//ポイントライトの最大数
static const float PI = 3.1415926f;         //π
static const int TILE_WIDTH = 16;			// タイルの幅
static const int TILE_HEIGHT = 16;			// タイルの高さ
static const int FRAME_BUFFER_W = 1280;		//フレームバッファの幅
static const int FRAME_BUFFER_H = 720;		//フレームバッファの高さ
static const int NUM_TILE = (FRAME_BUFFER_W / TILE_WIDTH) * (FRAME_BUFFER_H / TILE_HEIGHT); // タイルの数

// 一度に実行されるスレッド数
#define TILE_WIDTH 16
#define TILE_HEIGHT 16

//ディレクションライト
struct DirectionLight {
	float3 dir;				//方向
	float4 color;			//カラー
};

struct PointLight {
	float3 pos;				//座標
	float3 positionInView;	//カメラ空間での座標
	float4 color;			//カラー
	float2 attn;			//減衰パラメータ。xに影響範囲、yに影響率に累乗するパラメータ
};

struct Light {
	DirectionLight directionLight[MAX_DIRECTION_LIGHT];		//ディレクションライトの配列
	PointLight pointLight[MAX_POINT_LIGHT];					//ポイントライトの配列
	float3 eyePos;											//視点
	int pointLightNum;										//ポイントライトの数
	float4x4 mLVP;											//プロジェクション行列
};

cbuffer DefferdLightingCb : register(b1) {
	Light light;				//ライト情報
	int isIbl;					//IBLを行う？
	float iblLuminance;			//IBLの明るさ
};

cbuffer cb : register(b0) {
	float4x4 mvp;
	float4 mulColor;
	float4 screenParam;
};
struct VSInput {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> albedoAndShadowReceiverTexture : register(t0);		//rgbにテクスチャカラー。aにシャドウレシーバーフラグ
Texture2D<float4> normalTexture : register(t1);							//rgbに法線
Texture2D<float4> worldPosTexture : register(t2);						//rgbにワールド座標
Texture2D<float4> depthTexture : register(t3);							//深度値。
Texture2D<float4> occlusionAndSmoothAndMetaricTexture : register(t4);	//rにオクルージョン。bに滑らかさ。bに金属度
TextureCube<float4> g_skyCubeMap : register(t5);
Texture2D<float4> g_shadowMap : register(t10);							//シャドウマップ
StructuredBuffer<uint> pointLightListInTile : register(t11);			// タイルごとのポイントライトのインデックスのリスト

sampler g_sampler : register(s0);

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

/// ランバート拡散反射光を計算する。
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	//ピクセルの法線とライトの方向の内積を計算する。
	float t = dot(normal, lightDirection) * -1.0f;
	//内積の値を0以上の値にする。
	t = max(0.0f, t);
	//拡散反射光を計算する。
	return lightColor * t;
}

/// フォン鏡面反射光を計算する。
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
	//反射ベクトルを求める。
	float3 refVec = reflect(lightDirection, normal);
	//光が当たったサーフェイスから視点に伸びるベクトルを求める。
	float3 toEye = light.eyePos - worldPos;
	toEye = normalize(toEye);
	//鏡面反射の強さを求める。
	float t = dot(refVec, toEye);
	//鏡面反射の強さを0以上の数値にする。
	t = max(0.0f, t);
	//鏡面反射の強さを絞る。
	t = pow(t, 5.0f);
	//鏡面反射光を求める。
	return lightColor * t;
}

// ベックマン分布を計算する
float Beckmann(float m, float t)
{
	float t2 = t * t;
	float t4 = t * t * t * t;
	float m2 = m * m;
	float D = 1.0f / (4.0f * m2 * t4);
	D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
	return D;
}

// フレネルを計算。Schlick近似を使用
float SpcFresnel(float f0, float u)
{
	// from Schlick
	return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// クックトランスモデルの鏡面反射を計算
/// </summary>
/// <param name="L">光源に向かうベクトル</param>
/// <param name="V">視点に向かうベクトル</param>
/// <param name="N">法線ベクトル</param>
/// <param name="metaric">金属度</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metaric, float microfacet)
{
	// 金属度を垂直入射の時のフレネル反射率として扱う
	// 金属度が高いほどフレネル反射は大きくなる
	float f0 = 1.0f - microfacet;

	// ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
	float3 H = normalize(L + V);

	// 各種ベクトルがどれくらい似ているかを内積を利用して求める
	float NdotH = saturate(dot(N, H));
	float VdotH = saturate(dot(V, H));
	float NdotL = saturate(dot(N, L));
	float NdotV = saturate(dot(N, V));

	// D項をベックマン分布を用いて計算する
	float D = Beckmann(microfacet, NdotH);

	// F項をSchlick近似を用いて計算する
	float F = SpcFresnel(f0, VdotH);

	// G項を求める
	float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

	// m項を求める
	float m = PI * NdotV * NdotH;
	if (m == 0.0f)
		//0割り防止
		return 0.0f;

	// ここまで求めた、値を利用して、クックトランスモデルの鏡面反射を求める
	return max(F * D * G / m, 0.0);
}

/// <summary>
/// フレネル反射を考慮した拡散反射を計算
/// </summary>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V, float roughness)
{
	// step-1 ディズニーベースのフレネル反射による拡散反射を真面目に実装する。
   // 光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
	float3 H = normalize(L + V);

	float energyBias = lerp(0.0f, 0.5f, roughness);
	float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

	// 光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める
	float dotLH = saturate(dot(L, H));

	// 光源に向かうベクトルとハーフベクトル、
	// 光が平行に入射したときの拡散反射量を求めている
	float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

	// 法線と光源に向かうベクトルwを利用して拡散反射率を求める
	float dotNL = saturate(dot(N, L));
	float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

	// 法線と視点に向かうベクトルを利用して拡散反射率を求める
	float dotNV = saturate(dot(N, V));
	float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

	// 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
	// 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
	return (FL * FV * energyFactor);
}
// ハッシュ関数
float hash(float n)
{
	return frac(sin(n) * 43758.5453);
}

// 3次元ベクトルからシンプレックスノイズを生成する関数
float SimplexNoise(float3 x)
{
	// The noise function returns a value in the range -1.0f -> 1.0f
	float3 p = floor(x);
	float3 f = frac(x);

	f = f * f * (3.0 - 2.0 * f);
	float n = p.x + p.y * 57.0 + 113.0 * p.z;

	return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
		lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
		lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
			lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = mul(mvp, In.pos);
	psIn.uv = In.uv;
	return psIn;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(PSInput psIn) : SV_Target0
{
	//このピクセルが含まれているタイルの番号を計算する。
	uint numCellX = (screenParam.z + TILE_WIDTH - 1) / TILE_WIDTH;
	uint tileIndex = floor(psIn.pos.x / TILE_WIDTH) + floor(psIn.pos.y / TILE_WIDTH) * numCellX;

	//含まれるタイルの影響の開始位置と終了位置を計算する。
	uint lightStart = tileIndex * light.pointLightNum;
	uint lightEnd = lightStart + light.pointLightNum;

	//アルベドカラー。
	float3 albedoColor = albedoAndShadowReceiverTexture.Sample(g_sampler, psIn.uv).xyz;

	//シャドウレシーバーフラグ。
	int shadowReceiverFlg = albedoAndShadowReceiverTexture.Sample(g_sampler, psIn.uv).w;

	// スペキュラカラー(鏡面反射光)はアルベドカラーを使用。
	float3 specColor = albedoColor;

	//ワールド座標。
	float3 worldPos = worldPosTexture.Sample(g_sampler, psIn.uv).xyz;

	//法線。
	float3 normal = normalTexture.Sample(g_sampler, psIn.uv).xyz;
	
	//金属度。
	float metaric = occlusionAndSmoothAndMetaricTexture.Sample(g_sampler, psIn.uv).z;

	//滑らかさ。
	float smooth = occlusionAndSmoothAndMetaricTexture.Sample(g_sampler, psIn.uv).y;

	//視点。
	float3 toEye = normalize(light.eyePos - worldPos);

	//ライト
	float3 lig = float3(0.0f, 0.0f, 0.0f);
	
	// 影が落ちている？
	float ligFactor[MAX_DIRECTION_LIGHT] = {
		1.0f,
		1.0f,
		1.0f,
		1.0f
	};

	//シャドウレシーバー？
	if (shadowReceiverFlg) {

		//ライトビュースクリーン空間での座標系に変換。
		float4 posInLVP = mul(light.mLVP, float4(worldPos, 1.0f));

		//ライトビューの座標系を.wで割ることで正規化スクリーン座標系に変換できる。(重要)
		float2 shadowMapUV = posInLVP.xy / posInLVP.w;

		//ライトビュースクリーン空間からUV空間に座標変換。
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;

		// ライトビュースクリーン空間でのZ値を計算する。
		float zInLVP = posInLVP.z / posInLVP.w;

		//範囲内？
		if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
			&& shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f
			&& zInLVP > 0.0f && zInLVP < 1.0f
			) {
			//シャドウマップに描き込まれているZ値と比較する。
			float2 zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).xy;
			if (zInLVP > zInShadowMap.x + 0.0001f) {

				// 遮蔽されているなら、チェビシェフの不等式を利用して光が当たる確率を求める 
				float depth_sq = zInShadowMap.x * zInShadowMap.x;
				// このグループの分散具合を求める
				// 分散が大きいほど、varianceの数値は大きくなる
				float variance = min(max(zInShadowMap.y - depth_sq, 0.0001f), 1.0f);
				// このピクセルのライトから見た深度値とシャドウマップの平均の深度値の差を求める   
				float md = zInLVP - zInShadowMap.x;
				// 光が届く確率を求める
				float lit_factor = variance / (variance + md * md);
				//確率を元に、太陽光の影響率を求める。
				ligFactor[0] *= lit_factor;
			}
		}
	}

	//return float4(ligFactor[0], 0.0f, 0.0f, 1.0f);

	//ディレクションライトの計算。
	for (int dirLigNo = 0; dirLigNo < MAX_DIRECTION_LIGHT; dirLigNo++) {
	
		//フレネル反射を計算。
		float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -light.directionLight[dirLigNo].dir, toEye, 1.0f - smooth);

		//内積を計算。
		float NdotL = saturate(dot(normal, -light.directionLight[dirLigNo].dir));

		//ランバート拡散反射を計算。
		float3 lambertDiffuse = light.directionLight[dirLigNo].color * NdotL / PI;
		
		//拡散反射を計算。
		float3 dirDiffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;

		//鏡面反射を計算。
		float3 dirSpec = CookTorranceSpecular(-light.directionLight[dirLigNo].dir,
			toEye, normal, metaric, 1.0f - smooth) * light.directionLight[dirLigNo].color;

		//スペキュラ反射を計算。
		dirSpec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, smooth);

		//ディレクションライトの影響を加算する。
		lig += (dirDiffuse * (1.0f - smooth) + dirSpec * smooth) * ligFactor[dirLigNo];
		
	}
	//ポイントライトの計算。
	for (
		uint lightListIndex = lightStart;
		lightListIndex < lightEnd;
		lightListIndex++
		)
	{
		//リストから番号を取得する。
		uint ligNo = pointLightListInTile[lightListIndex];

		//無かったら終了。
		if (ligNo == 0xffffffff) {
			break;
		}

		//方向を求める。
		float3 ligDir = worldPos - light.pointLight[ligNo].pos;
		ligDir = normalize(ligDir);

		//拡散反射を計算。
		float3 poiDiffuse = CalcLambertDiffuse(ligDir, light.pointLight[ligNo].color, normal);

		//鏡面反射を計算。
		float3 poiSpec = CalcPhongSpecular(ligDir, light.pointLight[ligNo].color, worldPos, normal);

		//距離を計算。
		float3 distance = length(worldPos - light.pointLight[ligNo].pos);

		//影響率を計算。
		float affect = max(0.0f, 1.0f - 1.0f / light.pointLight[ligNo].attn.x * distance);
		affect = pow(affect, light.pointLight[ligNo].attn.y);

		//拡散反射、鏡面反射に影響率を乗算。
		poiDiffuse *= affect;
		poiSpec *= affect;

		//ポイントライトの影響を加算する。
		lig += poiDiffuse * (1.0f - smooth) + poiSpec * smooth;
	}


	if (isIbl == 1) {
		// 視線からの反射ベクトルを求める。
		float3 v = reflect(toEye * -1.0f, normal);
		int level = lerp(0, 12, 1 - smooth);
		lig += albedoColor * g_skyCubeMap.SampleLevel(g_sampler, v, level) * iblLuminance;
		
	}
	else {

		//環境光を加算。
		lig += float3(0.001f, 0.001f, 0.001f);
	}

	
	
	//最終カラーを初期化。
	float4 finalColor = float4(albedoColor, 1.0f);

	//今までのライト計算を乗算。
	finalColor.xyz *= lig;

	//最終カラーを出力。
	return finalColor;
}
