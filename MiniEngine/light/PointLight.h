#pragma once
#include "constLight.h"
#include "LightBase.h"

namespace nsMyGame {

	namespace nsLight {

		//ポイントライトのデータ
		struct PointLigData {
			CVector3 pos = CVector3::Zero;		//座標
			float pad1;							//パディング
			CVector3 positionInView;			// カメラ空間での座標
			float pad2;							//パディング
			CVector4 color = CVector4::White;	//カラー
			CVector2 attn;						//減衰パラメータ。xに影響範囲、yには影響率に累乗するパラメータ
			CVector2 pad3;						//パディング
		};

		//点光源クラス
		class CPointLight : public CLightBase
		{
			/**
			 * @brief 開始処理関数。
			*/
			bool StartSub();
			~CPointLight();
		public:

			/**
			 * @brief 座標を設定する関数。
			 * @param pos 座標
			*/
			void SetPosition(const CVector3& pos) {
				m_poiLigData.pos = pos;
			}

			/**
			 * @brief 座標を取得する関数。
			 * @return 座標
			*/
			const CVector3 GetPosition()const {

				return m_poiLigData.pos;
			}

			/**
			 * @brief カラーを設定する関数。
			 * @param color カラー
			*/
			void SetColor(const CVector3& color) {

				m_poiLigData.color.x = color.x;
				m_poiLigData.color.y = color.y;
				m_poiLigData.color.z = color.z;
			}

			/**
			 * @brief カラーを設定する関数。
			 * @param color カラー
			*/
			void SetColor(const float red, const float green, const float blue) {

				m_poiLigData.color.x = red;
				m_poiLigData.color.y = green;
				m_poiLigData.color.z = blue;
			}

			/**
			 * @brief カラーを取得する関数。
			 * @return カラー
			*/
			const CVector4& GetColor()const {

				return m_poiLigData.color;
			}

			/**
			 * @brief 影響範囲を設定する関数。
			 * @param range 影響範囲
			*/
			void SetRange(const float range) {

				m_poiLigData.attn.x = range;
			}

			/**
			 * @brief 影響範囲を取得する関数。
			 * @return 影響範囲
			*/
			const float GetRange()const {

				return m_poiLigData.attn.x;
			}

			/**
			 * @brief 影響率の累乗数を設定する関数。
			 * @param powParam 影響率の累乗数
			*/
			void SetAffectPowParam(const float powParam) {

				m_poiLigData.attn.y = powParam;
			}

			/**
			 * @brief 影響率の累乗数を取得する関数。
			 * @return 影響率の累乗数
			*/
			const float GetAffectpowParam()const {

				return m_poiLigData.attn.y;
			}

			/**
			 * @brief ライトのデータを取得。
			 * @return ライトのデータ
			*/
			void* GetLigData() { return &m_poiLigData; }

		private:
			PointLigData m_poiLigData;		//ポイントライトのデータ
		};
	}
}

