#include "stdafx.h"
#include "Sprite.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

	namespace {
		struct SSimpleVertex {
			CVector4 pos;
			CVector2 tex;
		};
	}
	const CVector2	CSprite::DEFAULT_PIVOT = { 0.5f, 0.5f };
	CSprite::~CSprite()
	{
	}
	void CSprite::InitTextures(const SpriteInitData& initData)
	{
		//�X�v���C�g�Ŏg�p����e�N�X�`������������B
		if (initData.m_ddsFilePath[0] != nullptr) {
			//dds�t�@�C���̃p�X���w�肳��Ă�̂Ȃ�Adds�t�@�C������e�N�X�`�����쐬����B
			int texNo = 0;
			while (initData.m_ddsFilePath[texNo] && texNo < MAX_TEXTURE) {
				wchar_t wddsFilePath[1024];
				mbstowcs(wddsFilePath, initData.m_ddsFilePath[texNo], 1023);
				m_textures[texNo].InitFromDDSFile(wddsFilePath);
				texNo++;
			}
			m_numTexture = texNo;
		}
		else if (initData.m_textures[0] != nullptr) {
			//�O���e�N�X�`�����w�肳��Ă���B
			int texNo = 0;
			while (initData.m_textures[texNo] != nullptr) {
				m_textureExternal[texNo] = initData.m_textures[texNo];
				texNo++;
			}
			m_numTexture = texNo;
		}
		else {
			//�e�N�X�`�����w�肳��ĂȂ��B
			MessageBoxA(nullptr, "initData.m_ddsFilePath��m_textures�̂ǂ��炩�Ɏg�p����e�N�X�`���̏���ݒ肵�Ă��������B", "�G���[", MB_OK);
			std::abort();
		}
	}
	void CSprite::InitShader(const SpriteInitData& initData)
	{
		if (initData.m_fxFilePath == nullptr) {
			MessageBoxA(nullptr, "fx�t�@�C�����w�肳��Ă��܂���B", "�G���[", MB_OK);
			std::abort();
		}
		wchar_t fxFilePath[1024];
		mbstowcs(fxFilePath, initData.m_fxFilePath, 1023);
		//�V�F�[�_�[�����[�h����B
		m_vs.LoadVS(fxFilePath, initData.m_vsEntryPointFunc);
		m_ps.LoadPS(fxFilePath, initData.m_psEntryPoinFunc);
	}
	void CSprite::InitDescriptorHeap(const SpriteInitData& initData)
	{
		if (m_textureExternal[0] != nullptr) {
			//�O���̃e�N�X�`�����w�肳��Ă���B
			for (int texNo = 0; texNo < m_numTexture; texNo++) {
				m_descriptorHeap.RegistShaderResource(texNo, *m_textureExternal[texNo]);
			}
		}
		else {
			for (int texNo = 0; texNo < m_numTexture; texNo++) {
				m_descriptorHeap.RegistShaderResource(texNo, m_textures[texNo]);
			}
		}
		if (initData.m_expandShaderResoruceView != nullptr) {
			//�g���V�F�[�_�[���\�[�X�r���[�B
			m_descriptorHeap.RegistShaderResource(
				EXPAND_SRV_REG__START_NO,
				*initData.m_expandShaderResoruceView
			);
		}
		if (initData.m_expandShaderResoruceView2 != nullptr) {
			//�g���V�F�[�_�[���\�[�X�r���[�B
			m_descriptorHeap.RegistShaderResource(
				EXPAND_SRV_REG__START_NO2,
				*initData.m_expandShaderResoruceView2
			);
		}
		m_descriptorHeap.RegistConstantBuffer(0, m_constantBufferGPU);
		if (m_userExpandConstantBufferCPU != nullptr) {
			//���[�U�[�g���̒萔�o�b�t�@��b1�Ɋ֘A�t������B
			m_descriptorHeap.RegistConstantBuffer(1, m_userExpandConstantBufferGPU);
		}
		m_descriptorHeap.Commit();
	}
	void CSprite::InitVertexBufferAndIndexBuffer(const SpriteInitData& initData)
	{
		float halfW = m_size.x * 0.5f;
		float halfH = m_size.y * 0.5f;
		//���_�o�b�t�@�̃\�[�X�f�[�^�B
		SSimpleVertex vertices[] =
		{
			{
				CVector4(-halfW, -halfH, 0.0f, 1.0f),
				CVector2(0.0f, 1.0f),
			},
			{
				CVector4(halfW, -halfH, 0.0f, 1.0f),
				CVector2(1.0f, 1.0f),
			},
			{
				CVector4(-halfW, halfH, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(halfW, halfH, 0.0f, 1.0f),
				CVector2(1.0f, 0.0f)
			}

		};
		unsigned short indices[] = { 0,1,2,3 };

		m_vertexBuffer.Init(sizeof(vertices), sizeof(vertices[0]));
		m_vertexBuffer.Copy(vertices);

		m_indexBuffer.Init(sizeof(indices), sizeof(indices[0]));
		m_indexBuffer.Copy(indices);
	}
	void CSprite::InitPipelineState(const SpriteInitData& initData)
	{
		// ���_���C�A�E�g���`����B
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		//�p�C�v���C���X�e�[�g���쐬�B
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		if (initData.m_alphaBlendMode == AlphaBlendMode_Trans) {
			//�����������̃u�����h�X�e�[�g���쐬����B
			psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
			psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		}
		else if (initData.m_alphaBlendMode == AlphaBlendMode_Add) {
			//���Z�����B
			psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
			psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
			psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		}

		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_pipelineState.Init(psoDesc);
	}
	void CSprite::InitConstantBuffer(const SpriteInitData& initData)
	{
		//�萔�o�b�t�@�̏������B
		m_constantBufferGPU.Init(sizeof(m_constantBufferCPU), nullptr);
		//���[�U�[�g���̒萔�o�b�t�@���w�肳��Ă���B
		if (initData.m_expandConstantBuffer != nullptr){
			m_userExpandConstantBufferCPU = initData.m_expandConstantBuffer;
			m_userExpandConstantBufferGPU.Init(
				initData.m_expandConstantBufferSize, 
				initData.m_expandConstantBuffer
			);
		}
	}
	void CSprite::Init(const SpriteInitData& initData)
	{
		m_size.x = static_cast<float>(initData.m_width);
		m_size.y = static_cast<float>(initData.m_height);

		//�e�N�X�`�����������B
		InitTextures(initData);
		//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���������B
		InitVertexBufferAndIndexBuffer(initData);
		//�萔�o�b�t�@���������B
		InitConstantBuffer(initData);
		
		//���[�g�V�O�l�`���̏������B
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

		//�V�F�[�_�[���������B
		InitShader(initData);
		//�p�C�v���C���X�e�[�g�̏������B
		InitPipelineState(initData);
		//�f�B�X�N���v�^�q�[�v���������B
		InitDescriptorHeap(initData);

		m_isInited = true;
	}
	void CSprite::Update(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot)
	{
		if (m_isInited == false) {
			//��������
			return;
		}
		//�s�{�b�g���l���ɓ��ꂽ���s�ړ��s����쐬�B
		//�s�{�b�g�͐^�񒆂�0.0, 0.0�A���オ-1.0f, -1.0�A�E����1.0�A1.0�ɂȂ�悤�ɂ���B
		CVector2 localPivot = pivot;
		localPivot.x -= 0.5f;
		localPivot.y -= 0.5f;
		localPivot.x *= -2.0f;
		localPivot.y *= -2.0f;
		//�摜�̃n�[�t�T�C�Y�����߂�B
		CVector2 halfSize = m_size;
		halfSize.x *= 0.5f;
		halfSize.y *= 0.5f;
		CMatrix mPivotTrans;

		mPivotTrans.MakeTranslation(
			{ halfSize.x * localPivot.x, halfSize.y * localPivot.y, 0.0f }
		);
		CMatrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		m_world = mPivotTrans * mScale;
		m_world = m_world * mRot;
		m_world = m_world * mTrans;
	}
	void CSprite::Draw(CRenderContext& renderContext)
	{
		if (m_isInited == false) {
			//���������B
			return;
		}
		//���݂̃r���[�|�[�g���畽�s���e�s����v�Z����B
        D3D12_VIEWPORT viewport = renderContext.GetViewport();
        //todo �J�����s��͒萔�Ɏg�p�B�ǂ����ς��Ȃ����E�E�E�B
        CMatrix viewMatrix = g_camera2D->GetViewMatrix();
        CMatrix projMatrix;
        projMatrix.MakeOrthoProjectionMatrix(viewport.Width, viewport.Height, 0.1f, 1.0f);

		m_constantBufferCPU.mvp = m_world * viewMatrix * projMatrix;
		m_constantBufferCPU.mulColor = m_mulColor;
		m_constantBufferCPU.screenParam.x = g_camera3D->GetNear();
		m_constantBufferCPU.screenParam.y = g_camera3D->GetFar();
		m_constantBufferCPU.screenParam.z = FRAME_BUFFER_W;
		m_constantBufferCPU.screenParam.w = FRAME_BUFFER_H;

		//�萔�o�b�t�@���X�V�B
		m_constantBufferGPU.CopyToVRAM(&m_constantBufferCPU);
		if (m_userExpandConstantBufferCPU != nullptr) {
			m_userExpandConstantBufferGPU.CopyToVRAM(m_userExpandConstantBufferCPU);
		}
		//���[�g�V�O�l�`����ݒ�B
		renderContext.SetRootSignature(m_rootSignature);
		//�p�C�v���C���X�e�[�g��ݒ�B
		renderContext.SetPipelineState(m_pipelineState);
		//���_�o�b�t�@��ݒ�B
		renderContext.SetVertexBuffer(m_vertexBuffer);
		//�C���f�b�N�X�o�b�t�@��ݒ�B
		renderContext.SetIndexBuffer(m_indexBuffer);
		//�v���~�e�B�u�g�|���W�[��ݒ肷��B
		renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//�f�B�X�N���v�^�q�[�v��ݒ肷��B
		renderContext.SetDescriptorHeap(m_descriptorHeap);
		//�`��
		renderContext.DrawIndexed(m_indexBuffer.GetCount());
	}
