#include "stdafx.h"
#include "Material.h"

//���[�g�V�O�l�`���ƃp�C�v���C���X�e�[�g����̓J���J���J��
enum {
	enDescriptorHeap_CB,
	enDescriptorHeap_SRV,
	enNumDescriptorHeap
};
	
void Material::InitTexture(const CTkmFile::SMaterial& tkmMat)
{
	const auto& nullTextureMaps = g_graphicsEngine->GetNullTextureMaps();

	const char* filePath = nullptr;
	char* map = nullptr;
	unsigned int mapSize;

	//�A���x�h�}�b�v�B
	{
		if (tkmMat.albedoMap != nullptr)
		{
			filePath = tkmMat.albedoMapFilePath.c_str();
			map = tkmMat.albedoMap.get();
			mapSize = tkmMat.albedoMapSize;
		}
		else
		{
			filePath = nullTextureMaps.GetAlbedoMapFilePath();
			map = nullTextureMaps.GetAlbedoMap().get();
			mapSize = nullTextureMaps.GetAlbedoMapSize();
		}
		auto albedoMap = g_engine->GetTextureFromBank(filePath);

		if (albedoMap == nullptr)
		{
			albedoMap = new Texture();
			albedoMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, albedoMap);
		}
		m_albedoMap = albedoMap;
	}


	//�@���}�b�v�B
	{
		if (tkmMat.normalMap != nullptr)
		{
			filePath = tkmMat.normalMapFilePath.c_str();
			map = tkmMat.normalMap.get();
			mapSize = tkmMat.normalMapSize;
		}
		else
		{
			filePath = nullTextureMaps.GetNormalMapFilePath();
			map = nullTextureMaps.GetNormalMap().get();
			mapSize = nullTextureMaps.GetNormalMapSize();
		}
		auto normalMap = g_engine->GetTextureFromBank(filePath);

		if (normalMap == nullptr)
		{
			normalMap = new Texture();
			normalMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, normalMap);
		}
		m_normalMap = normalMap;
	}



	//�X�y�L�����}�b�v�B
	{
		if (tkmMat.specularMap != nullptr)
		{
			filePath = tkmMat.specularMapFilePath.c_str();
			map = tkmMat.specularMap.get();
			mapSize = tkmMat.specularMapSize;
		}
		else
		{
			filePath = nullTextureMaps.GetSpecularMapFilePath();
			map = nullTextureMaps.GetSpecularMap().get();
			mapSize = nullTextureMaps.GetSpecularMapSize();
		}
		auto specularMap = g_engine->GetTextureFromBank(filePath);

		if (specularMap == nullptr)
		{
			specularMap = new Texture();
			specularMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, specularMap);
		}
		m_specularMap = specularMap;
	}
}
void Material::InitFromTkmMaterila(
	const CTkmFile::SMaterial& tkmMat,
	const wchar_t* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntryPointFunc,
	const char* psEntryPointFunc)
{
	//�e�N�X�`�������[�h�B
	InitTexture(tkmMat);
	
	//�萔�o�b�t�@���쐬�B
	SMaterialParam matParam;
	matParam.hasNormalMap = m_normalMap->IsValid() ? 1 : 0;
	matParam.hasSpecMap = m_specularMap->IsValid() ? 1 : 0;
	m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);

	//���[�g�V�O�l�`�����������B
	m_rootSignature.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	if (wcslen(fxFilePath) > 0) {
		//�V�F�[�_�[���������B
		InitShaders(fxFilePath, vsEntryPointFunc, vsSkinEntryPointFunc, psEntryPointFunc);
		//�p�C�v���C���X�e�[�g���������B
		InitPipelineState();
	}
}
void Material::InitPipelineState()
{
	// ���_���C�A�E�g���`����B
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//�p�C�v���C���X�e�[�g���쐬�B
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel->GetCompiledBlob());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel->GetCompiledBlob());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = TRUE;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 3;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//�A���x�h�J���[�o�͗p�B
#ifdef SAMPE_16_02
	psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;	//�@���o�͗p�B	
	psoDesc.RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;						//Z�l�B
#else
	psoDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;			//�@���o�͗p�B	
	psoDesc.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;	//Z�l�B
#endif
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	m_skinModelPipelineState.Init(psoDesc);

	//�����ăX�L���Ȃ����f���p���쐬�B
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob());
	m_nonSkinModelPipelineState.Init(psoDesc);

	//�����Ĕ������}�e���A���p�B
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel->GetCompiledBlob());
	psoDesc.BlendState.IndependentBlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

	
	m_transSkinModelPipelineState.Init(psoDesc);

	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob());
	m_transNonSkinModelPipelineState.Init(psoDesc);

}
void Material::InitShaders(
	const wchar_t* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntriyPointFunc,
	const char* psEntryPointFunc
)
{
	char objNameBase[256];
	wcstombs(objNameBase, fxFilePath, 256);

	//�X�L���Ȃ����f���p�̃V�F�[�_�[�����[�h����B
	m_vsNonSkinModel = g_engine->GetShaderFromBank(objNameBase, vsEntryPointFunc);
	if (m_vsNonSkinModel == nullptr) {
		m_vsNonSkinModel = new Shader;
		m_vsNonSkinModel->LoadVS(fxFilePath, vsEntryPointFunc);
		g_engine->RegistShaderToBank(objNameBase, vsEntryPointFunc, m_vsNonSkinModel);
	}
	//�X�L�����胂�f���p�̃V�F�[�_�[�����[�h����B
	m_vsSkinModel = g_engine->GetShaderFromBank(objNameBase, vsSkinEntriyPointFunc);
	if (m_vsSkinModel == nullptr) {
		m_vsSkinModel = new Shader;
		m_vsSkinModel->LoadVS(fxFilePath, vsSkinEntriyPointFunc);
		g_engine->RegistShaderToBank(objNameBase, vsSkinEntriyPointFunc, m_vsSkinModel);
	}

	m_psModel = g_engine->GetShaderFromBank(objNameBase, psEntryPointFunc);
	if (m_psModel == nullptr) {
		m_psModel = new Shader;
		m_psModel->LoadPS(fxFilePath, psEntryPointFunc);
		g_engine->RegistShaderToBank(objNameBase, psEntryPointFunc, m_psModel);
	}
}
void Material::BeginRender(CRenderContext& rc, int hasSkin)
{
	rc.SetRootSignature(m_rootSignature);
	
	if (hasSkin) {
		rc.SetPipelineState(m_skinModelPipelineState);
		//rc.SetPipelineState(m_transSkinModelPipelineState);
	}
	else {
		rc.SetPipelineState(m_nonSkinModelPipelineState);
	//	rc.SetPipelineState(m_transNonSkinModelPipelineState);
	}
}