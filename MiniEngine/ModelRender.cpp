#include "stdafx.h"
#include "ModelRender.h"

namespace nsMyGame {

	void CModelRender::Init() {

		//tkmファイルとtksファイルのパスを設定。
		SetFilePathTkmAndTks();

		//頂点シェーダーのエントリーポイントを指定。
		m_modelInitData.m_vsEntryPointFunc = c_entryPointVSMain;
		m_modelInitData.m_vsSkinEntryPointFunc = c_entryPointVSSkinMain;

		//fxファイルパスを指定。
		m_modelInitData.m_fxFilePath = c_fxFilePath_Model;

		//コンスタントバッファにモデルデータを入れる。
		m_modelInitData.m_expandConstantBuffer = &m_modelData;
		m_modelInitData.m_expandConstantBufferSize = sizeof(m_modelData);

		//レジスタのt10にシャドウマップを設定。
		m_modelInitData.m_expandShaderResoruceView = &CRenderingEngine::GetInstance()->GetShadowMap().GetRenderTargetTexture();

		//スケルトンを設定。
		if (m_skeleton.IsInited()) {
			m_modelInitData.m_skeleton = &m_skeleton;
		}

		//シャドウキャスターフラグがtrueならシャドウモデルを作成する。
		if (m_shadowCasterFlag) { CreateShadowModel(); }

		//モデルデータを元にモデルを初期化。
		m_model.Init(m_modelInitData);

		//アニメーションを初期化。
		m_animation.Init(m_skeleton, m_animationClip, m_animNum);
	}

	void CModelRender::Init(ModelInitData& modelInitData) {

		//tkmファイルのロード。
		if (modelInitData.m_tkmFilePath == nullptr) {

			//ロードエラー。
			MessageBox(nullptr, L"tkmファイルが読み込まれていません。", L"警告", MB_OK);
		}

		//tksファイルのロード。
		if (modelInitData.m_tksFilePath != nullptr) {
			m_skeleton.Init(modelInitData.m_tksFilePath);
		}

		//スケルトンを設定。
		if (m_skeleton.IsInited()) {
			m_modelInitData.m_skeleton = &m_skeleton;
		}

		//シャドウキャスターフラグがtrueならシャドウモデルを作成する。
		if (m_shadowCasterFlag) { CreateShadowModel(); }

		//モデルデータを元にモデルを初期化。
		m_model.Init(modelInitData);

		//アニメーションを初期化。
		m_animation.Init(m_skeleton, m_animationClip, m_animNum);
	}

	void CModelRender::InitForwardRenderingModel() {

		//tkmファイルとtksファイルのパスを設定。
		SetFilePathTkmAndTks();

		//頂点シェーダーのエントリーポイントを指定。
		m_modelInitData.m_vsEntryPointFunc = c_entryPointVSMain;
		m_modelInitData.m_vsSkinEntryPointFunc = c_entryPointVSSkinMain;

		//fxファイルパスを指定。
		m_modelInitData.m_fxFilePath = m_filePathFx;

		//コンスタントバッファにモデルデータを入れる。
		m_modelInitData.m_expandConstantBuffer = &m_modelData;
		m_modelInitData.m_expandConstantBufferSize = sizeof(m_modelData);

		//レジスタのt10にシャドウマップを設定。
		m_modelInitData.m_expandShaderResoruceView = &CRenderingEngine::GetInstance()->GetShadowMap().GetRenderTargetTexture();

		//スケルトンを設定。
		if (m_skeleton.IsInited()) {
			m_modelInitData.m_skeleton = &m_skeleton;
		}

		//シャドウキャスターフラグがtrueならシャドウモデルを作成する。
		if (m_shadowCasterFlag) { CreateShadowModel(); }

		//モデルデータを元にモデルを初期化。
		m_forwardRenderModel.Init(m_modelInitData);

		//アニメーションを初期化。
		m_animation.Init(m_skeleton, m_animationClip, m_animNum);
	}

	void CModelRender::InitForwardRenderingModel(ModelInitData& modelInitData) {

		//tkmファイルのロード。
		if (modelInitData.m_tkmFilePath == nullptr) {

			//ロードエラー。
			MessageBox(nullptr, L"tkmファイルが読み込まれていません。", L"警告", MB_OK);
		}

		//tksファイルのロード。
		if (modelInitData.m_tksFilePath != nullptr) {
			m_skeleton.Init(modelInitData.m_tksFilePath);
		}

		//スケルトンを設定。
		if (m_skeleton.IsInited()) {
			modelInitData.m_skeleton = &m_skeleton;
		}

		//シャドウキャスターフラグがtrueならシャドウモデルを作成する。
		if (m_shadowCasterFlag) { CreateShadowModel(); }

		//モデルデータを元にモデルを初期化。
		m_forwardRenderModel.Init(modelInitData);

		//アニメーションを初期化。
		m_animation.Init(m_skeleton, m_animationClip, m_animNum);
	}

	void CModelRender::CreateShadowModel() {

		//シャドウモデルのデータを初期化。
		ModelInitData ShadowModelInitData;
		ShadowModelInitData.m_fxFilePath = c_fxFilePath_ShadowMap;
		ShadowModelInitData.m_tkmFilePath = m_filePathTkm;
		ShadowModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;
		ShadowModelInitData.m_vsEntryPointFunc = c_entryPointVSMain;
		ShadowModelInitData.m_vsSkinEntryPointFunc = c_entryPointVSSkinMain;

		//スケルトンを設定。
		if (m_skeleton.IsInited()) {
			ShadowModelInitData.m_skeleton = &m_skeleton;
		}
		//シャドウモデルを初期化。
		m_shadowModel.Init(ShadowModelInitData);

		//シャドウモデルの座標、回転率、拡大率を更新。
		m_shadowModel.UpdateWorldMatrix(
			m_pos,
			m_rot,
			m_sca
		);
	}

	void CModelRender::SetFilePathTkmAndTks() {

		//tkmファイルのロード。
		if (m_filePathTkm != nullptr) {
			m_modelInitData.m_tkmFilePath = m_filePathTkm;
		}
		else {
			//ロードエラー。
			MessageBox(nullptr, L"tkmファイルが読み込まれていません。", L"警告", MB_OK);
		}

		//tksファイルのロード。
		if (m_filePathTks != nullptr) {
			m_skeleton.Init(m_filePathTks);
		}
	}

	void CModelRender::Update() {

		//スケルトンを更新。
		m_skeleton.Update(m_model.GetWorldMatrix());

		//アニメーションを進める。
		if (m_animFlg) {
			m_animation.Progress(m_animationSpeed / 30.0f);
		}

		//モデルの座標、回転率、拡大率を更新。
		m_model.UpdateWorldMatrix(
			m_pos,
			m_rot,
			m_sca
		);

		//モデルの座標、回転率、拡大率を更新。
		m_forwardRenderModel.UpdateWorldMatrix(
			m_pos,
			m_rot,
			m_sca
		);

		//シャドウモデルの座標、回転率、拡大率を更新。
		m_shadowModel.UpdateWorldMatrix(
			m_pos,
			m_rot,
			m_sca
		);
	}

	void CModelRender::Render(CRenderContext& rc) {

		//描画モードに応じて描画するモデルを変える。
		switch (rc.GetRenderMode()) {
		case CRenderContext::EnRender_Mode::enRenderMode_Normal:
			//モデルを描画。
			if (!IsCollisionModel()) {
				m_model.Draw(rc);
			}
			break;
		case CRenderContext::EnRender_Mode::enRenderMode_Shadow:
			//シャドウモデルを描画。
			m_shadowModel.Draw(rc, *CCamera::GetLightCamera());
			break;
		}
	}

	void CModelRender::ForwardRender(CRenderContext& rc) {

		//モデルを描画。
		if (!IsCollisionModel()) {
			m_forwardRenderModel.Draw(rc);
		}
	}
}
