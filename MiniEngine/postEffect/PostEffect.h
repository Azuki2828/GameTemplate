#pragma once
#include "constPostEffect.h"
#include "Bloom.h"
#include "FXAA.h"

namespace nsMyGame {

	namespace nsPostEffect {

		//ポストエフェクトクラス

		class CPostEffect
		{
		public:
			/**
			 * @brief 初期化関数。
			*/
			void Init();

			/**
			 * @brief 描画関数。
			 * @param rc レンダーコンテキスト
			*/
			void Render(CRenderContext& rc);
		private:
			CBloom m_bloom;		//ブルーム
			CFXAA m_fxaa;		//FXAA
		};
	}
}

