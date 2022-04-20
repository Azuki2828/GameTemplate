/*!
 * @brief	シンプルなモデルシェーダー。
 */

static const int MAX_DIRECTION_LIGHT = 4;
static const int MAX_POINT_LIGHT = 100;
static const float PI = 3.1415926f;         // π

cbuffer LightCb : register(b1) {
	int shadowReceiverFlg;
}
////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos : POSITION;  // モデルの頂点座標
	float3 normal : NORMAL; // 法線
	float3 tangent  : TANGENT;
	float3 biNormal : BINORMAL;
	float2 uv : TEXCOORD0;  // UV座標
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos : SV_POSITION;       // スクリーン空間でのピクセルの座標
	float3 normal : NORMAL;         // 法線
	float3 tangent : TANGENT;
	float3 biNormal : BINORMAL;
	float2 uv : TEXCOORD0;          // uv座標
	float3 worldPos : TEXCOORD1;    // ワールド空間でのピクセルの座標
};

struct SPSOut {
	float4 albedoAndShadowReceiver : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 worldPos : SV_TARGET2;
	float depth : SV_TARGET3;
	float4 occlusionAndSmoothAndMetaric : SV_TARGET4;
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);   // 法線マップ
Texture2D<float4> g_occlusionAndSmoothAndMetaricMap : register(t2);
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.normal = normalize(mul(m, vsIn.normal));
	psIn.tangent = normalize(mul(m, vsIn.tangent));
	psIn.biNormal = normalize(mul(m, vsIn.biNormal));
	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
SPSOut PSMain(SPSIn psIn)
{
	SPSOut psOut;

	//rgbにテクスチャカラーを取得。
	psOut.albedoAndShadowReceiver.xyz = g_albedo.Sample(g_sampler, psIn.uv).xyz;
	//aにシャドウレシーバーフラグを取得。
	psOut.albedoAndShadowReceiver.w = shadowReceiverFlg;

	//0.0f～1.0fの範囲で法線を取得。
	psOut.normal.xyz = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
	//-1.0f～1.0fの範囲に変換。
	psOut.normal.xyz = (psOut.normal.xyz - 0.5f) * 2.0f;

	//法線を計算。
	psOut.normal.xyz = psIn.tangent * psOut.normal.x + psIn.biNormal * psOut.normal.y + psIn.normal * psOut.normal.z;

	//ワールド座標を取得。
	psOut.worldPos.xyz = psIn.worldPos;

	//深度値を取得。
	psOut.depth = psIn.pos.z;

	//オクルージョン、滑らかさ、金属度を取得。（オクルージョンはまだ未入力）
	psOut.occlusionAndSmoothAndMetaric.xyz = g_occlusionAndSmoothAndMetaricMap.Sample(g_sampler, psIn.uv).xyz;

	//結果を出力。
	return psOut;
}
