#pragma once
#include "constPostEffect.h"
#include "PostEffectComponentBase.h"
#include "GaussianBlur.h"


namespace nsMyGame {

	namespace nsPostEffect {

		//ブルームクラス

		class CBloom : public CPostEffectComponentBase
		{
		public:
			/**
			 * @brief 初期化関数。
			*/
			void OnInit()override final;

			/**
			 * @brief 描画関数。
			 * @param rc レンダーコンテキスト
			*/
			void OnRender(CRenderContext& rc)override final;

			/**
			 * @brief 輝度テクスチャを取得する関数。
			 * @return 輝度テクスチャ
			*/
			Texture& GetResultTexture()override final {

				return m_luminanceRT.GetRenderTargetTexture();
			}

		private:
			/**
			 * @brief 輝度抽出に使用するスプライトを初期化する関数。
			*/
			void InitLuminanceSprite();

			/**
			 * @brief ガウシアンブラーに使用するスプライトを初期化する関数。
			*/
			void InitGaussianBlurSprite();

			/**
			 * @brief 輝度抽出を実行する関数。
			 * @param rc レンダーコンテキスト
			*/
			void ExecuteDrawLuminanceSprite(CRenderContext& rc);

			/**
			 * @brief ガウシアンブラーを実行する関数。
			 * @param rc レンダーコンテキスト
			*/
			void ExecuteGaussianBlur(CRenderContext& rc);

		private:
			CRenderTarget m_luminanceRT;							//輝度抽出用のレンダリングターゲット
			CGaussianBlur m_gaussianBlur[c_gaussianBlurNum];		//ガウシアンブラークラスのインスタンス
			CSprite m_luminanceSprite;							//輝度抽出用のスプライト
			CSprite m_bloomFinalSprite;							//ブルームの最終スプライト
		};
	}
}