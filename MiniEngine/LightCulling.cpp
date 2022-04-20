#include "stdafx.h"
#include "LightCulling.h"

namespace nsMyGame {

	void CLightCulling::Init() {

		// ルートシグネチャの初期化。
		m_rootSignature.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP
		);

		//ライトカリング用のコンピュートシェーダーをロード。
		m_csLightCulling.LoadCS(L"Assets/shader/lightCulling.fx", "CSMain");

		//パイプラインステートを初期化。
		InitPipelineState(m_rootSignature, m_lightCullingPipelineState, m_csLightCulling);

		//タイルごとのポイントライトの番号のリストを出力するUAVを初期化。
		m_pointLightNoListInTileUAV.Init(
			sizeof(int),
			nsLight::c_maxPointLightNum * nsLight::c_tileNum,
			nullptr
		);

		//ポイントライトの情報を送るための定数バッファを作成。
		UpdateLightCullingCameraData();
		m_cameraParamCB.Init(sizeof(m_lightCullingCameraData), &m_lightCullingCameraData);
		m_lightCB.Init(sizeof(*nsLight::CLightManager::GetInstance()->GetLigData()), nsLight::CLightManager::GetInstance()->GetLigData());

		//ライトカリング用のディスクリプタヒープを初期化。
		InitLightCullingDescriptorHeap();
	}

	void CLightCulling::Execute(CRenderContext& rc) {

		//カメラの情報を更新。
		UpdateLightCullingCameraData();
		
		//ライトカリングのコンピュートシェーダーをディスパッチ
		rc.SetComputeRootSignature(m_rootSignature);
		auto ligData = *nsLight::CLightManager::GetInstance()->GetLigData();
		m_lightCB.CopyToVRAM(*nsLight::CLightManager::GetInstance()->GetLigData());
		rc.SetComputeDescriptorHeap(m_lightCullingDescriptorHeap);
		rc.SetPipelineState(m_lightCullingPipelineState);

		rc.Dispatch(
			c_frameBufferWidth / c_tileWidth,
			c_frameBufferHeight / c_tileHeight,
			1
		);

		// リソースバリア
		rc.TransitionResourceState(
			m_pointLightNoListInTileUAV.GetD3DResoruce(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}

	void CLightCulling::InitPipelineState(RootSignature& rs, PipelineState& pipelineState, Shader& cs)
	{
		// パイプラインステートを作成。
		D3D12_COMPUTE_PIPELINE_STATE_DESC  psoDesc = { 0 };
		psoDesc.pRootSignature = rs.Get();
		psoDesc.CS = CD3DX12_SHADER_BYTECODE(cs.GetCompiledBlob());
		psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		psoDesc.NodeMask = 0;

		pipelineState.Init(psoDesc);
	}

	void CLightCulling::UpdateLightCullingCameraData() {

		//カメラの情報を初期化。
		m_lightCullingCameraData.mProj = g_camera3D->GetProjectionMatrix();
		m_lightCullingCameraData.mProjInv.Inverse(g_camera3D->GetProjectionMatrix());
		m_lightCullingCameraData.mCameraRot = g_camera3D->GetCameraRotation();
		m_lightCullingCameraData.screenParam.x = g_camera3D->GetNear();
		m_lightCullingCameraData.screenParam.y = g_camera3D->GetFar();
		m_lightCullingCameraData.screenParam.z = c_frameBufferWidth;
		m_lightCullingCameraData.screenParam.w = c_frameBufferHeight;

		m_cameraParamCB.CopyToVRAM(&m_lightCullingCameraData);
	}

	void CLightCulling::InitLightCullingDescriptorHeap() {

		//各情報をディスクリプタヒープに登録。
		m_lightCullingDescriptorHeap.RegistShaderResource(
			0,																	//t0
			CRenderTarget::GetGBufferRT(enDepthMap)->GetRenderTargetTexture()	//深度値テクスチャ
		);
		m_lightCullingDescriptorHeap.RegistUnorderAccessResource(
			0,																	//u0
			m_pointLightNoListInTileUAV											//タイルごとのポイントライトの番号のリストを出力するUAV
		);
		m_lightCullingDescriptorHeap.RegistConstantBuffer(
			0,																	//b0
			m_cameraParamCB														//カメラの情報
		);
		m_lightCullingDescriptorHeap.RegistConstantBuffer(
			1,																	//b1
			m_lightCB															//ライトの情報
		);

		//登録。
		m_lightCullingDescriptorHeap.Commit();
	}
}