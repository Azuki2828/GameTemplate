#pragma once
#include "stdafx.h"

namespace nsMyGame {

	//優先度。enPriority_Zerothが最も先に描画される。
	enum PriorityNum {
		enPriority_Zeroth,
		enPriority_First,
		enPriority_Second,
		enPriority_Third,
		enPriority_Fourth,
		enPriority_Fifth
	};

	/*------クラスの名前------*/
	constexpr const char* c_classNameFade = "fade";										//フェードクラスの名前
	constexpr const char* c_classNameSkyCube = "skycube";								//空クラスの名前
	/*------------------------*/

	/*--------サウンドリスト--------*/
	enum EnSoundList {

		enSound_Num
	};
	/*-----------------------------*/

	/*------サウンドのファイルパス------*/


	/*----------------------------------*/

	/*----------シェーダー関係----------*/
	constexpr const char* c_entryPointVSMain = "VSMain";						//頂点シェーダーのエントリーポイント
	constexpr const char* c_entryPointVSSkinMain = "VSSkinMain";				//スキンありの頂点シェーダーのエントリーポイント
	constexpr const char* c_fxFilePath_Model = "Assets/shader/model.fx";		//G-Buffer作成に使うシェーダーのファイルパス
	constexpr UINT c_frameBufferWidth = 1280;									//フレームバッファの幅
	constexpr UINT c_frameBufferHeight = 720;									//フレームバッファの高さ
	constexpr int c_tileWidth = 16;												//TBRで分割するタイルの横の数
	constexpr int c_tileHeight = 16;											//TBRで分割するタイルの縦の数
	/*----------------------------------*/

	/*---------ストップウォッチ---------*/

	const int c_initializeTime = 0;			//初期化するための変数
	constexpr short c_restInitialTime = 0;	//スピンロック時の待ちタイムの初期時間
	const int c_spinLockCriteria = 16;		//スピンロック時の基準となる時間
	/*----------------------------------*/

	constexpr float c_skyCubeLumminance = 0.2f;		//空の輝度

	constexpr const char* c_filePathFadeSprite = "Assets/image/fade.dds";			//フェード画像のファイルパス
}





/**
 * @brief HUDに登録されているスプライト
*/
enum HUDList {

	enSprite_HUDNum
};

namespace {

	const int c_pad0 = 0;											//0番目のパッド
	const int c_nameSize = 256;										//名前の最大のサイズ
	const int c_textSize = 64;										//テキストの標準サイズ
	const float c_textBitSize = 16.0f;								//テキストのビットサイズ

	const CVector3 c_gravityScale = { 0.0f,-300.0f,0.0f };			//標準の重力

	const CVector3 c_cameraUp = { 0.0f,1.0f,0.0f };					//カメラの標準の上方向

	const CVector2 c_renderTargetW1280H720 = { 1280.0f,720.0f };						//レンダリングターゲットで使用する幅と高さ
	const CVector2 c_shadowMapWH = { 4096.0f,4096.0f };									//シャドウマップの幅と高さ
	const int c_mipLevel1 = 1;															//レンダリングターゲットで使用するミップマップレベル
	const int c_renderArraySize1 = 1;													//レンダリングターゲットで使用するテクスチャ配列のサイズ
	const int c_gaussianBlurNum = 4;													//ガウシアンブラーをかける回数
	const int c_blurPower = 10;															//ブラーの強さ
	const char* c_fxFilePath_DeferredLighting = "Assets/shader/deferredLighting.fx";	//ディファードライティングに使うシェーダーのファイルパス
	const char* c_fxFilePath_Sprite = "Assets/shader/sprite.fx";						//スプライトシェーダーのファイルパス
	const char* c_fxFilePath_ShadowMap = "Assets/shader/drawShadowMap.fx";				//シャドウマップ描画に使うシェーダーのファイルパス

	const CVector4 c_translucentValue_Half = { 1.0f,1.0f,1.0f,0.5f };		//透明率50%
	const CVector4 c_translucentValue_OneThird = { 1.0f,1.0f,1.0f,0.3f };	//透明率70%
	const CVector4 c_translucentValue_Zero = { 1.0f,1.0f,1.0f,0.0f };		//透明率100%
	const CVector4 c_translucentValue_Max = { 1.0f,1.0f,1.0f,1.0f };		//透明率0%

	const float c_textShadowThickness = 1.0f;								//テキストの影（枠線）の厚さ

	const CVector3 c_scaleValue_Half = { 0.5f,0.5f,0.5f };					//拡大率50%
	const CVector3 c_scaleValue_ATenth = { 0.1f,0.1f,0.1f };				//拡大率10%
	const CVector3 c_scaleValue_AFifth = { 0.2f,0.2f,0.2f };				//拡大率20%
}

/**
 * @brief 数字の桁数を取得する関数
 * @tparam Num テンプレート
 * @param num 桁数を確かめたい数字
 * @return 桁数
*/
template<typename Num>
int GetDigit(Num num) {

	int digit = 0;
	while (num >= 1) {
		num /= 10;
		digit++;
	}

	return digit;
}