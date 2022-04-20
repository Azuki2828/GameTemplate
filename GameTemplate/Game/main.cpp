#include "stdafx.h"
#include "system/system.h"
using namespace nsMyGame;

	/**
	 * @brief �l�X�ȃC���X�^���X���쐬����֐��B
	*/
	void CreateInstance();

	/**
	 * @brief BGM��SE���ꊇ���[�h����֐��B
	*/
	void InitBGMAndSE();

	///////////////////////////////////////////////////////////////////
	// �E�B���h�E�v���O�����̃��C���֐��B
	///////////////////////////////////////////////////////////////////
	int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
	{
		//�Q�[���̏������B
		InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

		//////////////////////////////////////
		// �������珉�������s���R�[�h���L�q����B
		//////////////////////////////////////

		//�l�X�ȃC���X�^���X���쐬����B
		CreateInstance();

		//BGM��SE���ꊇ���[�h�B
		InitBGMAndSE();

		//���C���[�t���[���\����ON�ɂ���B
		//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

		//�X�g�b�v�E�H�b�`���쐬�B
		CStopwatch stopWatch;

		////////////////////////////////////////////////
		// ���������s���R�[�h�������̂͂����܂�
		////////////////////////////////////////////////

		// ��������Q�[�����[�v�B
		while (DispatchWindowMessage())
		{
			//�X�g�b�v�E�H�b�`�̌v���J�n�B
			stopWatch.Start();

			//�����_�����O�J�n�B
			g_engine->BeginFrame();

			//�Q�[���I�u�W�F�N�g���X�V�B
			CGameObjectManager::GetInstance()->ExecuteUpdate();

			//�G�t�F�N�g�G���W�����X�V�B
			EffectEngine::GetInstance()->Update(CGameTime::GetInstance().GetFrameDeltaTime());

			//���C�g���X�V�B
			nsLight::CLightManager::GetInstance()->Update();

			//�`��B
			CRenderingEngine::GetInstance()->Render();

			//�����_�����O�I���B
			g_engine->EndFrame();

			//�X�s�����b�N���s���B
			int restTime = c_initializeTime;

			do {
				stopWatch.Stop();
				restTime = c_spinLockCriteria - (int)stopWatch.GetElapsedMillisecond();
			} while (restTime > c_restInitialTime);

			//�X�g�b�v�E�H�b�`�̌v���I���B
			stopWatch.Stop();

			//�f���^�^�C�����X�g�b�v�E�H�b�`�̌v�����Ԃ���v�Z����B
			CGameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());
		}

		//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
		CGameObjectManager::DeleteInstance();

		return FALSE;
	}

	void CreateInstance() {

		CGameObjectManager::CreateInstance();				//�Q�[���I�u�W�F�N�g�Ǘ��N���X
		CPhysicsWorld::CreateInstance();					//�������[���h
		CSoundEngine::CreateInstance();						//�T�E���h�G���W��
		nsLight::CLightManager::CreateInstance();			//���C�g�Ǘ��N���X
		CCamera::CreateLightCamera();						//���C�g�J����
		CRenderingEngine::CreateRenderingEngine();			//�����_�����O�G���W��
		EffectEngine::CreateInstance();						//�G�t�F�N�g�G���W��
		CSoundManager::CreateInstance();					//�T�E���h�G���W��
		NewGO<CFade>(enPriority_Zeroth, c_classNameFade);	//�t�F�[�h
	}

	void InitBGMAndSE() {

		//CSoundManager::GetInstance()->Init("�����Ƀt�@�C���p�X", "�����ɃT�E���h�ԍ�", "�����Ƀ��[�v�t���O", "�����T�E���h�^�C�v");
		//CSoundManager::GetInstance()->SetSoundSourceVolume("�����ɃT�E���h�ԍ�", "�����ɃT�E���h�{�����[��");
	}