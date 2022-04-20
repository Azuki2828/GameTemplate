#pragma once
#include "constLight.h"

namespace nsMyGame {

	namespace nsLight {

		//ライトのデータ
		struct LigData {
			DirectionLigData directionLigData[c_maxDirectionLightNum];		//ディレクションライトのデータ
			PointLigData pointLigData[c_maxPointLightNum];					//ポイントライトのデータ
			CVector3 eyePos = CVector3::Zero;								//視点の座標
			int pointLightNum = 0;											//ポイントライトの総数
			CMatrix m_viewProj;												//プロジェクション行列
		};

		//ライト管理クラス
		class CLightManager
		{
		public:
			/**
			 * @brief ライトマネージャーのインスタンスを作成する関数。
			*/
			static void CreateInstance() {

				m_lightManager = new CLightManager;
			}

			/**
			 * @brief ライトマネージャーのインスタンスを取得する関数。
			 * @return インスタンス
			*/
			static CLightManager* GetInstance() {

				return m_lightManager;
			}

			/**
			 * @brief ライトを追加する関数。
			 * @param light ライトのインスタンス
			*/
			void AddLight(CLightBase* light);

			/**
			 * @brief ライトを削除する関数。
			 * @param light ライトのインスタンス
			*/
			void RemoveLight(CLightBase* light);

			/**
			 * @brief 登録されているライトを全て削除する関数。
			*/
			void RemoveLightAll();
			/**
			 * @brief ライトのデータを取得する関数。
			 * @return ライトのデータ
			*/
			LigData* GetLigData() { return &m_ligData; }

			/**
			 * @brief 更新関数。
			*/
			void Update();
		private:
			LigData m_ligData;									//ライトのデータ
			static CLightManager* m_lightManager;				//ライトマネージャーのインスタンス
			std::list<DirectionLigData*> m_directionLights;		//ディレクションライトのデータ
			std::list<PointLigData*> m_pointLights;				//ポイントライトのデータ
		};
	}
}

