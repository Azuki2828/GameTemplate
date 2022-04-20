#include "stdafx.h"
#include "PostEffect.h"


namespace nsMyGame {

	namespace nsPostEffect {

		void CPostEffect::Init() {

			//ブルームの初期化。
			m_bloom.Init();

			//FXAAの初期化。
			m_fxaa.Init();
		}

		void CPostEffect::Render(CRenderContext& rc) {
			
			//ブルーム。
			m_bloom.Render(rc);

			//FXAA。
			m_fxaa.Render(rc);
		}
	}
}