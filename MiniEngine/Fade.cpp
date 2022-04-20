#include "stdafx.h"
#include "Fade.h"

namespace nsMyGame {

	bool CFade::Start()
	{
		//フェード用のスプライトを初期化。
		m_fadeSprite = NewGO<CSpriteRender>(enPriority_Fifth);
		m_fadeSprite->Init(c_filePathFadeSprite, FRAME_BUFFER_W, FRAME_BUFFER_H);

		return true;
	}
	void CFade::Update()
	{
		//状態を調べる。
		switch (m_state) {
		case enState_FadeIn:

			//徐々にフェードイン。
			m_currentAlpha -= 0.5f * CGameTime().GameTimeFunc().GetFrameDeltaTime();

			//終わったら待機状態に設定。
			if (m_currentAlpha <= 0.0f) {
				m_currentAlpha = 0.0f;
				m_state = enState_Idle;
			}
			break;
		case enState_FadeOut:

			//徐々にフェードアウト。
			m_currentAlpha += 0.5f * CGameTime().GameTimeFunc().GetFrameDeltaTime();

			//終わったら待機状態に設定。
			if (m_currentAlpha >= 1.0f) {
				m_currentAlpha = 1.0f;
				m_state = enState_Idle;
			}
			break;
		case enState_Idle:

			//待機状態ならそのまま。
			break;
		}

		//透明度を設定。
		m_fadeSprite->SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
	}
}