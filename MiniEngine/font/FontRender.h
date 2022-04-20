#pragma once
#include "constFont.h"
#include "stdafx.h"

namespace nsMyGame {

	namespace nsFont {

		class CFontRender : public CIGameObject
		{
		public:
			/**
			 * @brief フォントのデータ
			*/
			struct SFontValue {
				wchar_t text[c_maxTextSize];	//テキスト
				CVector2 position = CVector2::Zero;	//座標
				CVector4 color = CVector4::White;		//色
				float rotation = 0.0f;		//回転
				float scale = 1.0f;		//拡大率
				CVector2 pivot = CVector2::Half;		//ピボット
			};

			/**
			 * @brief テキストを設定する関数
			 * @param text テキスト
			*/
			void SetText(const wchar_t* text)
			{
				swprintf_s(m_fontValue.text, text);
			}

			/**
			 * @brief 座標を設定する関数
			 * @param pos 座標
			*/
			void SetPosition(const CVector2& pos) {
				m_fontValue.position = pos;
			}

			/**
			 * @brief 拡大率を設定する関数
			 * @param sca 拡大率
			*/
			void SetScale(const float sca) {
				m_fontValue.scale = sca;
			}

			/**
			 * @brief フォントの色を設定する関数
			 * @param col 色
			*/
			void SetColor(const CVector4& col) {
				m_fontValue.color = col;
			}

			/**
			 * @brief ピボットを設定する関数
			 * @param piv ピボット
			*/
			void SetPivot(const CVector2& piv) {
				m_fontValue.pivot = piv;
			}

			/**
			 * @brief 初期化関数
			 * @param text テキスト
			 * @param position 座標
			 * @param color 色
			 * @param rotation 回転
			 * @param scale 拡大率
			 * @param pivot ピボット
			*/
			void Init(
				const wchar_t* text,
				const CVector2& position = CVector2::Zero,
				const CVector4& color = CVector4::Black,
				const float rotation = 0.0f,
				const float scale = 1.0f,
				const CVector2& pivot = { 0.5f,0.5f }
			);

			/**
			 * @brief 枠の影を付ける関数
			 * @param isDrawShadow 枠の影を付けるかどうかのフラグ
			 * @param shadowOffset 枠の影の太さ
			 * @param shadowColor 枠の影の色
			*/
			void SetShadowParam(bool isDrawShadow, float shadowOffset, const CVector4& shadowColor)
			{
				m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
			}

			/**
			 * @brief 描画関数
			 * @param rc レンダーコンテキスト
			*/
			void Render(CRenderContext& rc)override final;

		private:
			CFont m_font;			//フォント
			SFontValue m_fontValue;	//フォントのデータ
		};
	}
}

