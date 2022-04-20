#pragma once

#include "physics/PhysicsStaticObject.h"


namespace nsMyGame {

	struct LevelObjectData;

	class Material;

	class MapChip
	{
	public:
		MapChip(const LevelObjectData& objData);
		~MapChip();
		void Draw(CRenderContext& rc);
		void Draw(CRenderContext& rc,
			const CMatrix& mWorld,
			const CMatrix& mView,
			const CMatrix& mProj);

	private:

		struct MConstantBuffer {
			CMatrix mWorld;		//ワールド行列。
			CMatrix mView;		//ビュー行列。
			CMatrix mProj;		//プロジェクション行列。
		};
		ConstantBuffer m_constantBuffer;					//定数バッファ。
		//PhysicsStaticObject m_physicsStaticObject;

		std::vector< SMesh* > m_meshs;							//メッシュ。
		std::vector< DescriptorHeap > m_descriptorHeap;		//ディスクリプタヒープ。

		CModelRender* m_modelRender = nullptr;
		CModelRender* m_collisionModelRender = nullptr;
		PhysicsStaticObject m_physicsStaticObject;
	};
}

