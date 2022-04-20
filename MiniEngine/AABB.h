#pragma once

	/// <summary>
	/// AABB
	/// </summary>
	class AABB {
	private:
		CVector3 m_max;					// AABBを構成するボックスの最大座標。
		CVector3 m_min;					// AABBを構成するボックスの最小座標。
		CVector3 m_centerPos;			// 中心座標。
		CVector3 m_vertexPosition[8];	// AABBを構成する8頂点の座標。
	public:
		/// <summary>
		/// AABBの情報を初期化する。
		/// </summary>
		/// <param name="vMax">最大値</param>
		/// <param name="vMin">最小値</param>
		void Init(const CVector3& vMax, const CVector3& vMin);
		/// <summary>
		/// AABBを構成する8頂点のワールド空間での座標を計算する。
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="mWorld"></param>
		void CalcVertexPositions(CVector3* pos, const CMatrix& mWorld);
		/// <summary>
		/// AABBの最大値を取得。
		/// </summary>
		const CVector3& GetMax() const
		{
			return m_max;
		}
		/// <summary>
		/// AABBの最小値を取得。
		/// </summary>
		/// <returns>最小値</returns>
		const CVector3& GetMin() const
		{
			return m_min;
		}
		/// <summary>
		/// 中心座標を取得。
		/// </summary>
		/// <returns>中心座標</returns>
		const CVector3& GetCenter() const
		{
			return m_centerPos;
		}
	};