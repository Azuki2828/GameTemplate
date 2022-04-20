#pragma once
#include "../MiniEngine/postEffect/PostEffect.h"
#include "LightCulling.h"
#include "ShadowMap.h"

namespace nsMyGame {

	//レンダリングエンジンクラス
	class CRenderingEngine
	{
		//IBLのデータ
		struct SIblData {
			Texture texture;
			float luminance = 1.0f;
		};

		//ディファードライティングのデータ
		struct SDeferredLightingCB {

			nsLight::LigData ligData;		//ライトのデータ
			int isIbl = 0;					//IBLを行う？
			float iblLuminance = 1.0f;		//IBLの明るさ
		};
	public:
		/**
		 * @brief レンダリングエンジンを作成する関数。
		*/
		static void CreateRenderingEngine() {

			m_renderingEngine = new CRenderingEngine;

			//レンダリングエンジンの初期化。
			m_renderingEngine->Init();
		}

		/**
		 * @brief レンダリングエンジンを取得する関数。
		 * @return レンダリングエンジン
		*/
		static CRenderingEngine* GetInstance() {

			return m_renderingEngine;
		}

		/**
		 * @brief シャドウマップを取得する関数。
		 * @return シャドウマップ
		*/
		CRenderTarget& GetShadowMap() {

			return m_shadowMap.GetRenderTarget();
		}

		/**
		 * @brief 初期化関数。
		*/
		void Init();

		/**
		 * @brief 描画関数。
		*/
		void Render();

		/**
		 * @brief IBLのための初期化関数。
		*/
		void InitIbl(const wchar_t* iblTexFilePath, float luminance);

		/**
		 * @brief スカイキューブの初期化関数。
		*/
		void InitSkyCube();
	private:
		/**
		 * @brief レンダリングターゲットを作成する関数。
		*/
		void CreateRenderTarget();

		/**
		 * @brief ディファードライティングで使用するスプライトを初期化する関数。
		*/
		void InitDeferredLightingSprite();

		/**
		 * @brief メインレンダリングターゲットのコピーを取るためのレンダーターゲットを作成する関数。
		*/
		void CreateSnapShotMainRT();

		/**
		 * @brief フレームバッファにコピーするスプライトを作成する関数。
		*/
		void InitCopyToMainRenderTargetSprite();

		/**
		 * @brief IBLのデータの初期化関数。
		*/
		void InitIblData(const wchar_t* iblTexFilePath, float luminance);

		/**
		 * @brief シャドウマップを描画する関数。
		 * @param rc レンダーコンテキスト
		*/
		void DrawShadowMap(CRenderContext& rc);

		/**
		 * @brief スプライトを描画する関数。
		 * @param rc レンダーコンテキスト
		*/
		void RenderSprite(CRenderContext& rc);

		/**
		 * @brief フォントを描画する関数。
		 * @param rc レンダーコンテキスト
		*/
		void DrawFont(CRenderContext& rc);

		/**
		 * @brief エフェクトを描画する関数。
		 * @param rc レンダーコンテキスト
		*/
		void DrawEffect(CRenderContext& rc);

		/**
		 * @brief ワイヤーフレームを描画。
		 * @param rc レンダーコンテキスト
		*/
		void DrawWireFrame(CRenderContext& rc);

		/**
		 * @brief G-Bufferを作成する関数。
		 * @param rc レンダーコンテキスト
		*/
		void CreateGBuffer(CRenderContext& rc);

		/**
		 * @brief ディファードライティングを実行する関数。
		 * @param rc レンダーコンテキスト
		*/
		void ExecuteDeferredLighting(CRenderContext& rc);

		/**
		 * @brief フォワードレンダリングを実行する関数。
		 * @param rc レンダーコンテキスト
		*/
		void ExecuteForwardRendering(CRenderContext& rc);

		/**
		 * @brief フレームバッファ用に、メインレンダリングターゲットの内容をコピーする関数。
		 * @param rc レンダーコンテキスト
		*/
		void SnapShotMainRenderTarget(CRenderContext& rc);


		/**
		 * @brief フレームバッファにスプライトをコピーする関数。
		 * @param rc レンダーコンテキスト
		*/
		void CopyToFrameBuffer(CRenderContext& rc);

	private:
		static CRenderingEngine* m_renderingEngine;	//レンダリングエンジンのインスタンス
		CLightCulling m_lightCulling;				//ライトカリング
		CRenderTarget m_snapShotMainRT;				//メインレンダリングターゲットのスナップショットを取るためのレンダリングターゲット
		nsPostEffect::CPostEffect m_postEffect;		//ポストエフェクト
		CShadowMap m_shadowMap;						//シャドウマップ
		CSprite m_copyToMainRenderTargetSprite;		//メインレンダリングターゲットのスプライト
		CSprite m_deferredLightingSprite;			//ディファードライティング用のスプライト
		SIblData m_iblData;							//IBLのデータ
		SDeferredLightingCB m_deferredLightingCB;	//ディファードライティングに使用する定数バッファ
	};
}

