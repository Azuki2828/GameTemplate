#pragma once

#pragma comment( lib, "xinput.lib")
#pragma comment(lib, "dxcompiler.lib")

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <map>
#include <functional>
#include <algorithm>
#include <DirectXMath.h>
#include <Xinput.h>
#include <array>
#include <dxcapi.h>
#include <atlbase.h>
#include <dxgi.h>
#include <dxgitype.h>
#include <dxgiformat.h>
#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>

#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteBatch.h"
#include "DirectXTK/Inc/SpriteFont.h"

#include "d3dx12.h"
#include "dbg/MyAssert.h"

#include "math/Math.h"
#include "math/Vector.h"
#include "math/Matrix.h"

#include "tkEngine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "IShaderResource.h"
#include "IUnorderAccessResource.h"
#include "GPUBuffer.h"
#include "Texture.h"
#include "RWStructuredBuffer.h"
#include "StructuredBuffer.h"
#include "Shader.h"
#include "PipelineState.h"
#include "ConstantBuffer.h"
#include "RootSignature.h"
#include "raytracing/Raytracing.h"
#include "GraphicsEngine.h"
#include "DescriptorHeap.h"
#include "RenderTarget.h"
#include "Animation.h"
#include "GameTime.h"
#include "Stopwatch.h"

#include "RenderContext_inline.h"
#include "DescriptorHeap_inline.h"

#include "Sprite.h"
#include "Model.h"
#include "HID/GamePad.h"



#include "light/DirectionLight.h"
#include "light/PointLight.h"
#include "light/LightManager.h"
#include "light/LightBase.h"

#include "font/Font.h"
#include "font/FontRender.h"

#include "LightCamera.h"


#include "SpriteRender.h"
#include "ModelRender.h"
#include "RenderingEngine.h"

#include "nature/SkyCube.h"

#include "Fade.h"

const UINT FRAME_BUFFER_W = 1280;				//フレームバッファの幅。
const UINT FRAME_BUFFER_H = 720;				//フレームバッファの高さ。

const UINT FULL_HD_W = 1920;
const UINT FULL_HD_H = 1080;

static const int MAX_BONE = 512;	//!<ボーンの最大数。