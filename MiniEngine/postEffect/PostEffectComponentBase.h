#pragma once
#include "constPostEffect.h"

namespace nsMyGame {

	namespace nsPostEffect {

		class CPostEffectComponentBase
		{
		public:
			/**
			 * @brief 基底クラスの初期化関数。派生クラスの初期化と、メインレンダリングターゲットに描画するためのスプライトを初期化する。
			*/
			void Init();

			/**
			 * @brief 描画関数。派生クラスのスプライトを描画し、メインレンダリングターゲットに反映させる。
			 * @param rc
			*/
			void Render(CRenderContext& rc);
		private:
			/**
			 * @brief 派生クラスで定義すべき初期化関数。
			*/
			virtual void OnInit() = 0;

			/**
			 * @brief 派生クラスで定義すべき描画関数。
			 * @param rc レンダーコンテキスト
			*/
			virtual void OnRender(CRenderContext& rc) = 0;

			/**
			 * @brief 派生クラスで定義すべき、テクスチャを取得する関数。
			 * @return テクスチャ
			*/
			virtual Texture& GetResultTexture() = 0;

			/**
			 * @brief ポストの結果をメインレンダリングターゲットにコピーする？
			 * @return コピーする？
			*/
			virtual bool IsCopyResultTextureToMainRenderTarget() const
			{
				return false;
			}

		private:
			CSprite m_copyMainRtSprite;		//メインレンダリングターゲットに描画するためのスプライト。
		};
	}
}

