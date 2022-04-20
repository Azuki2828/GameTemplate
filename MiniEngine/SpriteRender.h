#pragma once

namespace nsMyGame {

	//スプライト描画クラス
	class CSpriteRender : public CIGameObject
	{
	private:
		/**
		 * @brief 初期化関数。
		 * @return 成功した？
		*/
		bool Start()override final;

		/**
		 * @brief 更新関数。
		*/
		void Update()override final;

	public:
		/**
		 * @brief 初期化関数。
		 * @param filepath ファイルパス
		 * @param width 幅
		 * @param height 高さ
		 * @param mode アルファブレンドのモード
		*/
		void Init(const char* filepath, const float width, const float height, const AlphaBlendMode mode = AlphaBlendMode::AlphaBlendMode_Trans);
		
		/**
		 * @brief 描画関数。
		 * @param rc レンダーコンテキスト
		*/
		void Render2D(CRenderContext& rc)override;

		/**
		 * @brief 座標を設定する関数。
		 * @param pos 座標
		*/
		void SetPosition(const CVector3& pos) { m_pos = pos; }

		/**
		 * @brief 回転を設定する関数。
		 * @param rot 回転
		*/
		void SetRotation(const CQuaternion& rot) { m_rot = rot; }

		/**
		 * @brief 拡大率を設定する関数。
		 * @param sca 拡大率
		*/
		void SetScale(const CVector3& sca) { m_sca = sca; }

		/**
		 * @brief 基点を設定する関数。
		 * @param pivot 基点
		*/
		void SetPivot(const CVector2& pivot) { m_pivot = pivot; }

		/**
		 * @brief 拡大率を取得する関数。
		 * @return 拡大率
		*/
		CVector3& GetScale() { return m_sca; }

		/**
		 * @brief カラーを乗算する関数。
		 * @param mulCol 乗算カラー
		*/
		void SetMulColor(const CVector4& mulCol) {

			m_mulColor = mulCol;
		}
	private:
		CSprite m_sprite;							//スプライト
		SpriteInitData m_initData;					//スプライトのデータ
		CVector3 m_pos = CVector3::Zero;			//座標
		CQuaternion m_rot = CQuaternion::Identity;	//回転
		CVector3 m_sca = CVector3::One;				//拡大
		CVector4 m_mulColor = CVector4::White;		//乗算カラー
		CVector2 m_pivot = CVector2::Half;			//ピボット
	};
}

