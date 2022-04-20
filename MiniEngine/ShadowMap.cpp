#include "stdafx.h"
#include "ShadowMap.h"

namespace nsMyGame{

	void CShadowMap::Init() {

		//レンダリングターゲットを作成。
		m_shadowMap = new CRenderTarget;

		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		m_shadowMap->Create(
			c_shadowMapWH.x,
			c_shadowMapWH.y,
			c_mipLevel1,
			c_renderArraySize1,
			DXGI_FORMAT_R32G32_FLOAT,	//VSMのため、x,yに深度値、深度値の２乗を書き込む。
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		//シャドウマップにかけるブラーを初期化。
		m_shadowBlur.Init(&m_shadowMap->GetRenderTargetTexture());
	}

	void CShadowMap::ExecuteBlur(CRenderContext& rc) {

		//シャドウマップにブラーをかける。
		m_shadowBlur.ExecuteOnGPU(rc, 5.0f);
	}
}