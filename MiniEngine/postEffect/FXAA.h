#pragma once
#include "constPostEffect.h"
#include "PostEffectComponentBase.h"

namespace nsMyGame {

	namespace nsPostEffect {

		//FXAAクラス

		class CFXAA : public CPostEffectComponentBase
		{
		private:
			//FXAA用Buffer
			struct SFXAABuffer {
				float bufferW;
				float bufferH;
			};
		public:
			/**
			 * @brief 初期化関数。
			*/
			void OnInit();

			/**
			 * @brief 描画関数。
			 * @param rc レンダーコンテキスト
			*/
			void OnRender(CRenderContext& rc);

			/**
			 * @brief FXAA後のテクスチャを取得する関数。
			 * @return FXAA後のテクスチャ
			*/
			Texture& GetResultTexture()override final {

				return m_fxaaRT.GetRenderTargetTexture();
			};

			/**
			 * @brief ポストの結果をメインレンダリングターゲットにコピーする？
			 * @return コピーする？
			*/
			bool IsCopyResultTextureToMainRenderTarget() const override final
			{
				return true;
			}

		private:
			CRenderTarget m_fxaaRT;		//FXAA用のレンダリングターゲット
			SFXAABuffer m_buffer;		//FXAA用の構造体バッファ
			CSprite m_finalSprite;		//最終スプライト
		};
	}
}

