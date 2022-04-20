#pragma once
#include "stdafx.h"

class LightCamera : CCamera
{
	//ライトカメラのインスタンスを作成する関数。
	static void CreateLightCamera() {

		m_lightCamera = new LightCamera;
	}
	//ライトカメラのインスタンスを取得する関数。
	static LightCamera* GetLightCamera() {

		return m_lightCamera;
	}
private:
	static LightCamera* m_lightCamera;		//ライトカメラのインスタンス
	CVector3 m_pos = CVector3::Zero;			//座標
	CVector3 m_tar = CVector3::One;			//ターゲット
};

