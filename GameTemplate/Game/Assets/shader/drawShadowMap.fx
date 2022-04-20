/*!
 * @brief	シンプルなモデルシェーダー。
 */

static const int MAX_DIRECTION_LIGHT = 4;
static const int MAX_POINT_LIGHT = 100;
static const float PI = 3.1415926f;         // π

struct DirectionLight {
	float3 dir;
	float4 color;
};

struct PointLight {
	float3 pos;
	float4 color;
	float2 attn;
};

cbuffer LightCb : register(b1) {
	DirectionLight directionLight[MAX_DIRECTION_LIGHT];
	PointLight pointLight[MAX_POINT_LIGHT];
	float3 eyePos;
	int pointLightNum;
	float4x4 mLVP;
}
////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0) {
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn {
	int4  Indices  	: BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn {
	float4 pos : POSITION;  // モデルの頂点座標
	float3 normal : NORMAL; // 法線
	float3 tangent  : TANGENT;
	float3 biNormal : BINORMAL;
	float2 uv : TEXCOORD0;  // UV座標
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn {
	float4 pos : SV_POSITION;       // スクリーン空間でのピクセルの座標
	float3 normal : NORMAL;         // 法線
	float3 tangent : TANGENT;
	float3 biNormal : BINORMAL;
	float2 uv : TEXCOORD0;          // uv座標
	float3 worldPos : TEXCOORD1;    // ワールド空間でのピクセルの座標
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);   // 法線マップ
Texture2D<float4> g_specularMap : register(t2); // スペキュラマップ。rgbにスペキュラカラー、aに金属度
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
	if (hasSkin) {
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else {
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.normal = normalize(mul(mWorld, vsIn.normal));
	psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
	psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
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
SPSIn VSSkinMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	float depth = psIn.pos.z;
	//xに深度値。yに深度値の２乗。
	return float4(depth, depth * depth, 0.0f, 1.0f);
}
