#include "stdafx.h"
#include "SpriteRender.h"

namespace nsMyGame {


	bool CSpriteRender::Start() {

		return true;
	}

	void CSpriteRender::Init(const char* filepath, float width, float height, AlphaBlendMode mode) {

		m_initData.m_ddsFilePath[0] = filepath;
		m_initData.m_width = static_cast<UINT>(width);
		m_initData.m_height = static_cast<UINT>(height);
		m_initData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_initData.m_alphaBlendMode = mode;

		m_sprite.Init(m_initData);
	}

	void CSpriteRender::Update() {

		m_sprite.SetMulColor(m_mulColor);
		m_sprite.Update(m_pos, m_rot, m_sca, m_pivot);
	}

	void CSpriteRender::Render2D(CRenderContext& rc) {

		m_sprite.Draw(rc);
	}
}

