#pragma once
#pragma once

#include "Texture.h"
#include "../GameTemplate/Game/ConstValue.h"

class CGraphicsEngine;


//レンダリングターゲットのリスト
enum EnRenderTargetList {
	enMainRT,

	enRenderTargetNum
};


//GBufferのリスト
enum EnGBufferList {
	enAlbedoAndShadowReceiverFlgMap,
	enNormalMap,
	enWorldPosMap,
	enDepthMap,
	enocclusionAndSmoothAndMetaricMap,

	enGBufferNum
};


/// レンダリングターゲットクラス。

class CRenderTarget {
public:
	
	/**
	 * @brief デストラクタ。
	*/
	~CRenderTarget();

	/**
	 * @brief レンダリングターゲットの作成。
	 * @param w レンダリングターゲットの幅
	 * @param h レンダリングターゲットの高さ
	 * @param mipLevel ミップマップレベル。0を指定した場合はミップマップがサポートされているGPUでは1*1ピクセルまでのミップマップが作成される。
	 * @param arraySize テクスチャ配列のサイズ
	 * @param colorFormat カラーバッファのフォーマット。
	 * @param depthStencilFormat 深度ステンシルバッファのフォーマット。
	 * @param clearColor 初期カラー
	 * @return 作成に成功した？
	*/
	bool Create(
		int w,
		int h,
		int mipLevel,
		int arraySize,
		DXGI_FORMAT colorFormat,
		DXGI_FORMAT depthStencilFormat,
		float clearColor[4] = nullptr
	);

	/**
	 * @brief メインレンダリングターゲットを作成する関数。
	*/
	static void CreateMainRenderTarget() {

		m_renderTarget[enMainRT] = new CRenderTarget;

		m_renderTarget[enMainRT]->Create(
			c_renderTargetW1280H720.x,
			c_renderTargetW1280H720.y,
			c_mipLevel1,
			c_renderArraySize1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}


	/**
	 * @brief アルベドカラーとシャドウレシーバーフラグを保管するGBufferを作成する関数。
	*/
	static void CreateAlbedoAndShadowReceiverFlagRenderTarget() {

		m_GBuffer[enAlbedoAndShadowReceiverFlgMap] = new CRenderTarget;

		m_GBuffer[enAlbedoAndShadowReceiverFlgMap]->Create(
			c_renderTargetW1280H720.x,
			c_renderTargetW1280H720.y,
			c_mipLevel1,
			c_renderArraySize1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	/**
	 * @brief 法線と深度値を保管するGBufferを作成する関数。
	*/
	static void CreateNormalMapRenderTarget() {

		m_GBuffer[enNormalMap] = new CRenderTarget;

		m_GBuffer[enNormalMap]->Create(
			c_renderTargetW1280H720.x,
			c_renderTargetW1280H720.y,
			c_mipLevel1,
			c_renderArraySize1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
	}

	/**
	 * @brief ワールド座標を保管するGBufferを作成する関数。
	*/
	static void CreateWorldPosRenderTarget() {

		m_GBuffer[enWorldPosMap] = new CRenderTarget;

		m_GBuffer[enWorldPosMap]->Create(
			c_renderTargetW1280H720.x,
			c_renderTargetW1280H720.y,
			c_mipLevel1,
			c_renderArraySize1,
			DXGI_FORMAT_R32G32B32A32_FLOAT, // ワールド座標を記録するので、32ビット浮動小数点バッファを利用する
			DXGI_FORMAT_UNKNOWN
		);
	}

	/**
	 * @brief 深度値を保管するGBufferを作成する関数。
	*/
	static void CreateDepthRenderTarget() {

		m_GBuffer[enDepthMap] = new CRenderTarget;

		m_GBuffer[enDepthMap]->Create(
			c_renderTargetW1280H720.x,
			c_renderTargetW1280H720.y,
			c_mipLevel1,
			c_renderArraySize1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
	}

	static void CreateSpecularRenderTarget() {

		m_GBuffer[enocclusionAndSmoothAndMetaricMap] = new CRenderTarget;

		m_GBuffer[enocclusionAndSmoothAndMetaricMap]->Create(
			c_renderTargetW1280H720.x,
			c_renderTargetW1280H720.y,
			c_mipLevel1,
			c_renderArraySize1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	/**
	 * @brief レンダリングターゲットを取得する関数。
	 * @param renderTarget 指定するレンダリングターゲット
	 * @return 指定したレンダリングターゲット
	*/
	static CRenderTarget* GetRenderTarget(EnRenderTargetList renderTarget) {

		return m_renderTarget[renderTarget];
	}

	/**
	 * @brief GBuffer用のレンダリングターゲットを取得する関数。
	 * @param gBufferRT 指定するレンダリングターゲット
	 * @return 指定したレンダリングターゲット
	*/
	static CRenderTarget* GetGBufferRT(EnGBufferList gBufferRT) {

		return m_GBuffer[gBufferRT];
	}

	/**
	 * @brief CPU側のレンダリングターゲットのディスクリプタハンドルを取得。
	 * @return CPU側のレンダリングターゲットのディスクリプタハンドル
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCpuDescriptorHandle() const
	{
		return m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/**
	 * @brief CPU側のデプスステンシルバッファのディスクリプタハンドルを取得。
	 * @return CPU側のデプスステンシルバッファのディスクリプタハンドル
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuDescriptorHandle() const
	{
		return m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/**
	 * @brief レンダリングターゲットとなるテクスチャを取得。
	 * @return レンダリングターゲットとなるテクスチャ
	*/
	Texture& GetRenderTargetTexture()
	{
		return m_renderTargetTexture;
	}

	/**
	 * @brief デプスステンシルバッファが存在しているか判定
	 * @return デプスステンシルバッファが存在している?
	*/
	bool IsExsitDepthStencilBuffer() const
	{
		return m_depthStencilTexture;
	}

	/**
	 * @brief レンダリングターゲットの幅を取得。
	 * @return レンダリングターゲットの幅
	*/
	int GetWidth() const
	{
		return m_width;
	}

	/**
	 * @brief レンダリングターゲットの高さを取得。
	 * @return レンダリングターゲットの高さ
	*/
	int GetHeight() const
	{
		return m_height;
	}

	/**
	 * @brief カラーバッファのフォーマットを取得。
	 * @return カラーバッファのフォーマット
	*/
	DXGI_FORMAT GetColorBufferFormat() const
	{
		return m_renderTargetTexture.GetFormat();
	}

	/**
	 * @brief レンダリングターゲットビューのクリアカラーを取得する関数。
	 * @return レンダリングターゲットビューのクリアカラー
	*/
	const float* GetRTVClearColor() const
	{
		return m_rtvClearColor;
	}

	/**
	 * @brief DSVのクリアカラーを取得する関数。
	 * @return DSVのクリアカラー
	*/
	float GetDSVClearValue() const
	{
		return m_dsvClearValue;
	}
private:

	/**
	 * @brief ディスクリプタヒープを作成。
	 * @param ge グラフィックエンジン
	 * @param d3dDevice D3Dデバイス
	 * @return 作成に成功した？
	*/
	bool CreateDescriptorHeap(CGraphicsEngine& ge, ID3D12Device5*& d3dDevice);

	/**
	 * @brief レンダリングターゲットとなるテクスチャを作成。
	 * @param ge グラフィックエンジン
	 * @param d3dDevice D3Dデバイス
	 * @param w テクスチャの幅
	 * @param h テクスチャの高さ
	 * @param mipLevel ミップマップレベル
	 * @param arraySize テクスチャ配列のサイズ
	 * @param format テクスチャのフォーマット
	 * @param clearColor 初期カラー
	 * @return 作成に成功した？
	*/
	bool CreateRenderTargetTexture(
		CGraphicsEngine& ge,
		ID3D12Device5*& d3dDevice,
		int w, 
		int h,
		int mipLevel,
		int arraySize,
		DXGI_FORMAT format,
		float clearColor[4]
	);

	/**
	 * @brief 深度ステンシルバッファとなるテクスチャを作成。
	 * @param ge グラフィックエンジン
	 * @param d3dDevice D3Dデバイス
	 * @param w テクスチャの幅
	 * @param h テクスチャの高さ
	 * @param format テクスチャのフォーマット
	 * @return 作成に成功した？
	*/
	bool CreateDepthStencilTexture(
		CGraphicsEngine& ge,
		ID3D12Device5*& d3dDevice,
		int w,
		int h,
		DXGI_FORMAT format);

	/**
	 * @brief ディスクリプタの作成。
	 * @param d3dDevice D3Dデバイス
	*/
	void CreateDescriptor(ID3D12Device5*& d3dDevice);
private:
	static CRenderTarget* m_renderTarget[enRenderTargetNum];		//レンダリングターゲット
	static CRenderTarget* m_GBuffer[enGBufferNum];				//GBuffer用のレンダリングターゲット

	Texture m_renderTargetTexture;				//レンダリングターゲットとなるテクスチャ。
	ID3D12Resource* m_renderTargetTextureDx12;	//レンダリングターゲットとなるテクスチャ。
	ID3D12Resource* m_depthStencilTexture;		//深度ステンシルバッファとなるテクスチャ。
	ID3D12DescriptorHeap*		m_rtvHeap;		//RTV用のディスクリプタヒープ。
	ID3D12DescriptorHeap*		m_dsvHeap;		//深度ステンシルバッファビューのディスクリプタヒープ。
	UINT m_rtvDescriptorSize = 0;				//フレームバッファのディスクリプタのサイズ。
	UINT m_dsvDescriptorSize = 0;				//深度ステンシルバッファのディスクリプタのサイズ。
	int m_width = 0;							//レンダリングターゲットの幅。
	int m_height = 0;							//レンダリングターゲットの高さ。
	float m_rtvClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	//レンダリングターゲットビューのクリアカラー。
	float m_dsvClearValue = 1.0f;							//DSVのクリアカラー。
};


