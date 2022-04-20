#pragma once

namespace nsMyGame {

	//ライトカリングクラス
	class CLightCulling
	{
	private:
		// ライトカリングで使用するカメラ情報
		struct alignas(16) LightCullingCameraData
		{
			CMatrix mProj;           // プロジェクション行列
			CMatrix mProjInv;        // プロジェクション行列の逆行列
			CMatrix mCameraRot;      // カメラの回転行列
			CVector4 screenParam;    // スクリーン情報
		};
	public:
		/**
		 * @brief 初期化関数。
		*/
		void Init();

		/**
		 * @brief ライトカリング実行関数。
		*/
		void Execute(CRenderContext& rc);

		/**
		 * @brief タイルごとのポイントライトの番号のリストを出力するUAVを取得する関数。
		 * @return タイルごとのポイントライトの番号のリストを出力するUAV
		*/
		RWStructuredBuffer& GetPointLightNoListInTileUAV() {

			return m_pointLightNoListInTileUAV;
		}
	private:
		/**
		 * @brief パイプラインステートを初期化する関数。
		*/
		void InitPipelineState(RootSignature& rs, PipelineState& pipelineState, Shader& cs);

		/**
		 * @brief ライトカリングに使用するカメラのデータを初期化する関数。
		*/
		void UpdateLightCullingCameraData();

		/**
		 * @brief ライトカリング用のディスクリプタヒープを初期化する関数。
		*/
		void InitLightCullingDescriptorHeap();
	private:
		Shader m_csLightCulling;							//ライトカリング用のコンピュートシェーダー
		PipelineState m_lightCullingPipelineState;			//パイプラインステート
		RootSignature m_rootSignature;						//ルートシグネチャ
		RWStructuredBuffer m_pointLightNoListInTileUAV;		//タイルごとのポイントライトの番号のリストを出力するUAV
		LightCullingCameraData m_lightCullingCameraData;	//ライトカリングに使用するカメラのデータ
		ConstantBuffer m_cameraParamCB;						//ポイントライトの情報を送るための定数バッファ
		ConstantBuffer m_lightCB;							//ポイントライトの情報を送るための定数バッファ
		DescriptorHeap m_lightCullingDescriptorHeap;		//ライトカリング用のディスクリプタヒープ
	};
}

