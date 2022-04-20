#include "stdafx.h"
#include "Bloom.h"

namespace nsMyGame {

	namespace nsPostEffect {

		void CBloom::OnInit() {

			//輝度抽出用のレンダリングターゲットを作成。
			m_luminanceRT.Create(
				c_renderTargetW1280H720.x,       // 解像度はメインレンダリングターゲットと同じ
				c_renderTargetW1280H720.y,        // 解像度はメインレンダリングターゲットと同じ
				c_mipLevel1,
				c_renderArraySize1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_D32_FLOAT
			);

			//輝度抽出用のスプライトを初期化。
			InitLuminanceSprite();

			//ガウシアンブラー用のスプライトを初期化。
			InitGaussianBlurSprite();
		}

		void CBloom::OnRender(CRenderContext& rc) {

			//輝度抽出
			ExecuteDrawLuminanceSprite(rc);

			//ガウシアンブラー
			ExecuteGaussianBlur(rc);
		}

		void CBloom::InitLuminanceSprite() {

			//輝度抽出用のスプライトを初期化
			//初期化情報を作成する
			SpriteInitData luminanceSpriteInitData;

			//輝度抽出用のシェーダーのファイルパスを指定する。
			luminanceSpriteInitData.m_fxFilePath = c_fxFilePathBloom;

			//頂点シェーダーのエントリーポイントを指定する。
			luminanceSpriteInitData.m_vsEntryPointFunc = c_vsEntryPointFuncBloom;

			//ピクセルシェーダーのエントリーポイントを指定する。
			luminanceSpriteInitData.m_psEntryPoinFunc = c_psEntryPointFuncSamplingLuminance;

			//スプライトの幅と高さは輝度抽出用のレンダリングターゲットと同じ。
			luminanceSpriteInitData.m_width = m_luminanceRT.GetWidth();
			luminanceSpriteInitData.m_height = m_luminanceRT.GetHeight();

			//テクスチャはメインレンダリングターゲットのカラーバッファー。
			luminanceSpriteInitData.m_textures[0] = &CRenderTarget::GetRenderTarget(enMainRT)->GetRenderTargetTexture();

			//描き込むレンダリングターゲットのフォーマットを指定する。
			luminanceSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

			//作成した初期化情報をもとにスプライトを初期化する。
			m_luminanceSprite.Init(luminanceSpriteInitData);
		}

		void CBloom::InitGaussianBlurSprite() {

			//ガウシアンブラークラスのインスタンスを初期化。
			m_gaussianBlur[0].Init(&m_luminanceRT.GetRenderTargetTexture());
			m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
			m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
			m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

			//ボケ画像を合成して書き込むためのスプライトを初期化
			SpriteInitData finalSpriteInitData;

			//ガウシアンブラー用のテクスチャを設定。
			finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
			finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
			finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
			finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();

			//スプライトの幅と高さはメインレンダリングターゲットと同じ
			finalSpriteInitData.m_width = CRenderTarget::GetRenderTarget(enMainRT)->GetWidth();
			finalSpriteInitData.m_height = CRenderTarget::GetRenderTarget(enMainRT)->GetHeight();

			//ガウシアンブラー用のシェーダーのファイルパスを指定する
			finalSpriteInitData.m_fxFilePath = c_fxFilePathBloom;

			//ピクセルシェーダーのエントリーポイントを指定する
			finalSpriteInitData.m_psEntryPoinFunc = c_psEntryPointFuncBloomFinal;

			//アルファブレンディングモードは加算合成を選択する。
			finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;

			//カラーバッファのフォーマットはメインレンダリングターゲットと同じにする。
			finalSpriteInitData.m_colorBufferFormat = CRenderTarget::GetRenderTarget(enMainRT)->GetColorBufferFormat();

			//スプライトを初期化。
			m_bloomFinalSprite.Init(finalSpriteInitData);
		}

		void CBloom::ExecuteDrawLuminanceSprite(CRenderContext& rc) {

			// 輝度抽出用のレンダリングターゲットに変更
			rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRT);

			// レンダリングターゲットを設定
			rc.SetRenderTargetAndViewport(m_luminanceRT);

			// レンダリングターゲットをクリア
			rc.ClearRenderTargetView(m_luminanceRT);

			// 輝度抽出を行う
			m_luminanceSprite.Draw(rc);

			// レンダリングターゲットへの書き込み終了待ち
			rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRT);
		}

		void CBloom::ExecuteGaussianBlur(CRenderContext& rc) {

			//ガウシアンブラーを4回実行する
			for (int i = 0; i < c_gaussianBlurNum; i++) {

				m_gaussianBlur[i].ExecuteOnGPU(rc, c_blurPower);
			}

			//4枚のボケ画像を合成してメインレンダリングターゲットに加算合成
			rc.WaitUntilToPossibleSetRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));

			//レンダリングターゲットとビューポートを設定。
			rc.SetRenderTargetAndViewport(*CRenderTarget::GetRenderTarget(enMainRT));

			//最終スプライトを描画。
			m_bloomFinalSprite.Draw(rc);

			//描き込み終了待ち。
			rc.WaitUntilFinishDrawingToRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));
		}
	}
}