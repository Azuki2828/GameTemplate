/*!
 *@brief	CGameObjectのマネージャ
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"

CGameObjectManager* CGameObjectManager::m_instance = nullptr;

CGameObjectManager::CGameObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_instance = this;
}
CGameObjectManager::~CGameObjectManager()
{
	m_instance = nullptr;
}
void CGameObjectManager::ExecuteUpdate()
{	
	//死亡フラグがついているゲームオブジェクトを破棄する。
	for (auto& goList : m_gameObjectListArray) {
		goList.remove_if([&](CIGameObject* go) {	
			if (go->IsDead()) {
				delete go;
				return true;
			}
			return false;	 
		});
	}

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->StartWrapper();
		}
	}


	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->UpdateWrapper();
		}
	}
	//物理エンジンのアップデートを呼び出す。
	CPhysicsWorld::GetInstance()->Update(1.0f / 60.0f);
}
void CGameObjectManager::ExecuteRender(CRenderContext& rc)
{
	//レンダラーを変更するならここを改造していくと良い。
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc);
		}
	}
}

void CGameObjectManager::ExecuteForwardRender(CRenderContext& rc)
{
	//レンダラーを変更するならここを改造していくと良い。
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->ForwardRenderWrapper(rc);
		}
	}
}

void CGameObjectManager::Execute2DRender(CRenderContext& rc)
{
	//レンダラーを変更するならここを改造していくと良い。
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->Render2DWrapper(rc);
		}
	}
}