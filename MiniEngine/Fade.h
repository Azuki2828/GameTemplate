#pragma once

namespace nsMyGame {

	//フェードクラス
	class CFade : public CIGameObject
	{
	public:
		/**
		 * @brief Update()関数の前に一度だけ呼ばれる関数。
		 * @return 成功した？
		*/
		bool Start() override final;

		/**
		 * @brief 更新関数。
		*/
		void Update() override final;

		/**
		 * @brief フェードインを始める関数。
		*/
		void StartFadeIn()
		{
			m_state = enState_FadeIn;
		}

		/**
		 * @brief フェードアウトを始める関数。
		*/
		void StartFadeOut()
		{
			m_state = enState_FadeOut;
		}

		/**
		 * @brief フェード中？
		 * @return フェード中かどうかの判定結果
		*/
		bool IsFade() const
		{
			return m_state != enState_Idle;
		}

		/**
		 * @brief フェード画像の透明度を取得する関数。
		 * @return フェード画像の透明度
		*/
		float GetCurrentAlpha() const
		{
			return m_currentAlpha;
		}

	private:
		//フェードのステート
		enum EnState {
			enState_FadeIn,		//フェードイン中
			enState_FadeOut,	//フェードアウト中
			enState_Idle,		//アイドル中
		};
		CSpriteRender* m_fadeSprite = nullptr;	//フェードのスプライト
		EnState m_state = enState_Idle;			//状態
		float m_currentAlpha = 1.0f;			//現在の透明度
	};
}

