#pragma once

namespace nsMyGame {

	//サウンドの種類
	enum class SoundType {
		Type_BGM,
		Type_SE,
		Type_Effect,

		Type_Num
	};

	//サウンド管理クラス
	class CSoundManager
	{
		struct SoundData {
			int soundNum = 0;							//登録番号
			bool roopFlg = false;						//ループフラグ
			const wchar_t* soundName = nullptr;			//ファイルパス
			float soundVolume = 1.0f;					//音量
			CSoundSource* sound = nullptr;				//サウンドソース(音源)
			SoundType soundType = SoundType::Type_BGM;	//サウンドの種類
		};
	public:

		/**
		 * @brief 初期化関数。
		 * @param filePath ファイルパス
		 * @param num サウンド番号
		 * @param flg ループ再生する？
		 * @param sType サウンドの種類
		 * @param vol 音量
		*/
		void Init(const wchar_t* filePath, const int num, const bool flg = false, SoundType sType = SoundType::Type_BGM, const float vol = 1.0f);

		/**
		 * @brief インスタンスを生成する関数。
		*/
		static void CreateInstance() {

			m_soundManager = new CSoundManager;
		}

		/**
		 * @brief インスタンスを取得する関数。
		 * @return インスタンス
		*/
		static CSoundManager* GetInstance() {

			return m_soundManager;
		}

		/**
		 * @brief インスタンスを削除する関数。
		*/
		static void DeleteInstance() {

			//サウンドデータを全削除。
			m_soundManager->m_soundData.clear();

			//インスタンスを削除。
			delete m_soundManager;
		}

		/**
		 * @brief 更新関数。
		*/
		void Update();

		/**
		 * @brief サウンドを削除する関数。
		 * @param num サウンド番号
		*/
		void Release(const int num);

		/**
		 * @brief サウンドを再生する関数。
		 * @param num サウンド番号
		*/
		void Play(const int num);

		/**
		 * @brief サウンドを停止する関数。
		 * @param num サウンド番号
		*/
		void Stop(const int num);

		/**
		 * @brief 再生中？
		 * @param num サウンド番号
		 * @return 再生中かどうかの判定
		*/
		bool IsPlaying(const int num);
		
		/**
		 * @brief 音量を設定する関数。
		 * @param num  サウンド番号
		 * @param vol サウンドボリューム (0.0f～1.0fを設定)
		*/
		void SetSoundSourceVolume(const int num, const float vol);

		/**
		 * @brief ループ再生するかどうか設定する関数。
		 * @param num サウンド番号
		 * @param flg ループ再生するかどうかのフラグ
		*/
		void SetRoopFlg(const int num, const bool flg);

		/**
		 * @brief 音量を設定する関数。
		 * @param vol サウンドボリューム
		 * @param type サウンドの種類
		*/
		void SetVolume(const float vol, SoundType type = SoundType::Type_BGM) {
			switch (type) {
			case SoundType::Type_BGM:
				m_bgmVolume = vol;
				break;
			case SoundType::Type_SE:
				m_seVolume = vol;
				break;
			case SoundType::Type_Effect:
				m_effectVolume = vol;
				break;
			}
		}

	private:
		static CSoundManager* m_soundManager;		//インスタンス
		float m_bgmVolume = 1.0f;					//BGM音量
		float m_seVolume = 1.0f;					//SE音量
		float m_effectVolume = 1.0f;				//エフェクト音量
		std::vector<SoundData> m_soundData;			//サウンドデータ
	};
}

