#pragma once

namespace nsMyGame {

	namespace nsPostEffect {


		/*------PostEffectComponentBase------*/
		constexpr const char* c_fxFilePathCopySprite = "Assets/shader/sprite.fx";			//メインレンダリングターゲットにコピーする時に使用するシェーダーのファイルパス
		/*-----------------------------------*/


		/*------GaussianBlur------*/
		constexpr const char* c_fxFilePathGaussianBlur = "Assets/shader/gaussianBlur.fx";	//ガウシアンブラーで使用するシェーダーのファイルパス
		constexpr const char* c_vsEntryPointFuncXBlur = "VSXBlur";							//横ブラー用のスプライトの頂点シェーダーのエントリーポイント
		constexpr const char* c_vsEntryPointFuncYBlur = "VSYBlur";							//縦ブラー用のスプライトの頂点シェーダーのエントリーポイント
		constexpr const char* c_psEntryPointFuncBlur = "PSBlur";							//ブラー用のスプライトのピクセルシェーダーのエントリーポイント
		constexpr const int c_gaussianBlurNumWeight = 8;									//ガウシアンブラーの重みの数
		/*------------------------*/


		/*------Bloom------*/
		constexpr const int c_gaussianBlurNum = 4;												//ガウシアンブラーの数
		constexpr const char* c_fxFilePathBloom = "Assets/shader/bloom.fx";						//ブルーム用のシェーダーのファイルパス
		constexpr const char* c_vsEntryPointFuncBloom = "VSMain";								//ブルーム用の頂点シェーダーのエントリーポイント
		constexpr const char* c_psEntryPointFuncSamplingLuminance = "PSSamplingLuminance";		//輝度抽出用のピクセルシェーダーのエントリーポイント
		constexpr const char* c_psEntryPointFuncBloomFinal = "PSBloomFinal";					//ブルーム最終のピクセルシェーダーのエントリーポイント
		/*-----------------*/


		/*------FXAA------*/
		constexpr const char* c_fxFilePathFXAA = "Assets/shader/fxaa.fx";	//FXAAで使用するシェーダーのファイルパス
		constexpr const char* c_vsEntryPointFuncFXAA = "VSMain";			//FXAA用のスプライトの頂点シェーダーのエントリーポイント
		constexpr const char* c_psEntryPointFuncFXAA = "PSMain";			//FXAA用のスプライトのピクセルシェーダーのエントリーポイント
		/*----------------*/
	}
}