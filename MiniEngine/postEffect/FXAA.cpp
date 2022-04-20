#include "stdafx.h"
#include "FXAA.h"

namespace nsMyGame {

    namespace nsPostEffect {

        void CFXAA::OnInit() {

            //FXAA用のレンダリングターゲットを作成する。
            m_fxaaRT.Create(
                CRenderTarget::GetRenderTarget(enMainRT)->GetWidth(),
                CRenderTarget::GetRenderTarget(enMainRT)->GetHeight(),
                c_mipLevel1,
                c_renderArraySize1,
                DXGI_FORMAT_R8G8B8A8_UNORM,
                DXGI_FORMAT_UNKNOWN
            );

            //最終合成用のスプライトを初期化する。
            SpriteInitData spriteInitData;
            spriteInitData.m_textures[0] = &CRenderTarget::GetRenderTarget(enMainRT)->GetRenderTargetTexture();

            //解像度はmainRenderTargetの幅と高さと同じにする。
            spriteInitData.m_width = CRenderTarget::GetRenderTarget(enMainRT)->GetWidth();
            spriteInitData.m_height = CRenderTarget::GetRenderTarget(enMainRT)->GetHeight();

            //FXAA用のシェーダーを使用する。
            spriteInitData.m_fxFilePath = c_fxFilePathFXAA;
            spriteInitData.m_vsEntryPointFunc = c_vsEntryPointFuncFXAA;
            spriteInitData.m_psEntryPoinFunc = c_psEntryPointFuncFXAA;

            //アルファブレンディングモードは指定しない。
            spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;

            //解像度をGPUに送るための定数バッファを設定する。
            spriteInitData.m_expandConstantBuffer = (void*)&m_buffer;
            spriteInitData.m_expandConstantBufferSize = sizeof(SFXAABuffer) +
                (16 - (sizeof(SFXAABuffer) % 16));

            //スプライトを初期化。
            m_finalSprite.Init(spriteInitData);
        }

        void CFXAA::OnRender(CRenderContext& rc) {

            // レンダリングターゲットとして利用できるまで待つ
            rc.WaitUntilToPossibleSetRenderTarget(m_fxaaRT);
            // レンダリングターゲットを設定
            rc.SetRenderTargetAndViewport(m_fxaaRT);
            m_buffer.bufferW = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
            m_buffer.bufferH = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
            //描画。
            m_finalSprite.Draw(rc);
            // レンダリングターゲットへの書き込み終了待ち
            rc.WaitUntilFinishDrawingToRenderTarget(m_fxaaRT);
        }
    }
}