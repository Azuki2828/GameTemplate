#include "stdafx.h"
#include "system/system.h"
using namespace nsMyGame;

	/**
	 * @brief 様々なインスタンスを作成する関数。
	*/
	void CreateInstance();

	/**
	 * @brief BGMとSEを一括ロードする関数。
	*/
	void InitBGMAndSE();

	///////////////////////////////////////////////////////////////////
	// ウィンドウプログラムのメイン関数。
	///////////////////////////////////////////////////////////////////
	int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
	{
		//ゲームの初期化。
		InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

		//////////////////////////////////////
		// ここから初期化を行うコードを記述する。
		//////////////////////////////////////

		//様々なインスタンスを作成する。
		CreateInstance();

		//BGMとSEを一括ロード。
		InitBGMAndSE();

		//ワイヤーフレーム表示をONにする。
		//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

		//ストップウォッチを作成。
		CStopwatch stopWatch;

		////////////////////////////////////////////////
		// 初期化を行うコードを書くのはここまで
		////////////////////////////////////////////////

		// ここからゲームループ。
		while (DispatchWindowMessage())
		{
			//ストップウォッチの計測開始。
			stopWatch.Start();

			//レンダリング開始。
			g_engine->BeginFrame();

			//ゲームオブジェクトを更新。
			CGameObjectManager::GetInstance()->ExecuteUpdate();

			//エフェクトエンジンを更新。
			EffectEngine::GetInstance()->Update(CGameTime::GetInstance().GetFrameDeltaTime());

			//ライトを更新。
			nsLight::CLightManager::GetInstance()->Update();

			//描画。
			CRenderingEngine::GetInstance()->Render();

			//レンダリング終了。
			g_engine->EndFrame();

			//スピンロックを行う。
			int restTime = c_initializeTime;

			do {
				stopWatch.Stop();
				restTime = c_spinLockCriteria - (int)stopWatch.GetElapsedMillisecond();
			} while (restTime > c_restInitialTime);

			//ストップウォッチの計測終了。
			stopWatch.Stop();

			//デルタタイムをストップウォッチの計測時間から計算する。
			CGameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());
		}

		//ゲームオブジェクトマネージャーを削除。
		CGameObjectManager::DeleteInstance();

		return FALSE;
	}

	void CreateInstance() {

		CGameObjectManager::CreateInstance();				//ゲームオブジェクト管理クラス
		CPhysicsWorld::CreateInstance();					//物理ワールド
		CSoundEngine::CreateInstance();						//サウンドエンジン
		nsLight::CLightManager::CreateInstance();			//ライト管理クラス
		CCamera::CreateLightCamera();						//ライトカメラ
		CRenderingEngine::CreateRenderingEngine();			//レンダリングエンジン
		EffectEngine::CreateInstance();						//エフェクトエンジン
		CSoundManager::CreateInstance();					//サウンドエンジン
		NewGO<CFade>(enPriority_Zeroth, c_classNameFade);	//フェード
	}

	void InitBGMAndSE() {

		//CSoundManager::GetInstance()->Init("ここにファイルパス", "ここにサウンド番号", "ここにループフラグ", "ここサウンドタイプ");
		//CSoundManager::GetInstance()->SetSoundSourceVolume("ここにサウンド番号", "ここにサウンドボリューム");
	}