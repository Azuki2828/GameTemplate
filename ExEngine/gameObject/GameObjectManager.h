/*!
 *@brief	CGameObjectのマネージャー。
 */

#pragma once

#include "gameObject/IGameObject.h"
#include <functional>
#include <vector>


/// <summary>
/// GameObjectManagerクラス
/// <summary>
/// <remark>
/// このクラスはシングルトンパターンで設計されています。
/// インスタンスの生成はCreateInstance()、
/// インスタンスの破棄はDeleteInstance()、
/// インスタンスの取得はCreateInstance()を呼び出してください。
/// シングルトンパターンは下記の二つの機能を提供するパターンです。
/// 1.　グローバルなアクセスポイント
///		->グローバル変数の機能
/// 2　インスタンスの数を一つに制限する機能。
/// </reramk>
/// 
class CGameObjectManager {
private:
	CGameObjectManager();
	~CGameObjectManager();
public: //静的メンバ関数
	/// <summary>
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new CGameObjectManager;
	}
	/// <summary>
	/// インスタンスの破棄。
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}
	/// <summary>
	/// インスタンスを取得。
	/// </summary>
	/// <returns></returns>
	static CGameObjectManager* GetInstance()
	{
		return m_instance;
	}
public:
	/// <summary>
	/// 更新処理を実行
	/// </summary>
	void ExecuteUpdate();
	/// <summary>
	/// 描画処理を実行。
	/// </summary>
	/// <param name="rc"></param>
	void ExecuteRender(CRenderContext& rc);

	/// <summary>
	/// 描画処理を実行。
	/// </summary>
	/// <param name="rc"></param>
	void ExecuteForwardRender(CRenderContext& rc);

	/// <summary>
	/// 描画処理を実行。
	/// </summary>
	/// <param name="rc"></param>
	void Execute2DRender(CRenderContext& rc);
	
	
	/*!
	*@brief	ゲームオブジェクトのnew
	*@details
	* この関数を使用してnewしたオブジェクトは必ずDeleteGameObjectを実行することでdeleteされます。
	*@param[in]	prio		実行優先順位。
	*@param[in]	objectName	オブジェクト名。
	*/
	template<class T>
	T* NewGameObject(int prio, const char* objectName)
	{
		T* newObject = new T();
		newObject->SetName(objectName);
		m_gameObjectListArray.at(prio).push_back(newObject);
		return newObject;
	}
	/*!
	*@brief	ゲームオブジェクトの削除。
	*/
	void DeleteGameObject( CIGameObject* gameObject )
	{
		if (gameObject != nullptr) {
			gameObject->Dead();
			gameObject->OnDestroyWrapper();
		}
	}
	/*!
	*@brief	ゲームオブジェクトの検索。
	*@details
	* 重いよ！
	*@param[in]	objectName		オブジェクト名。
	*/
	template<class T>
	T* FindGameObject(const char* objectName)
	{
		
		for (auto goList : m_gameObjectListArray) {
			for (auto go : goList) {
				if (go->IsMatchName(objectName)) {
					//見つけた。
					T* p = dynamic_cast<T*>(go);
					return p;
				}
			}
		}
		
		//見つからなかった。
		return nullptr;
	}
	template<class T>
	void QueryGameObjects(const char* objectName, std::function<bool(T* go)> func)
	{
		for (auto goList : m_gameObjectListArray) {
			for (auto go : goList) {
				if (go->IsMatchName(objectName)) {
					//見つけた。
					T* p = dynamic_cast<T*>(go);
					if (func(p) == false) {
						//クエリ中断。
						return;
					}
				}
			}
		}
	}
	template<class T>
	const std::vector<T*>& FindGameObjects(const char* objectName)
	{
		static std::vector<T*> objectVector;
		objectVector.clear();

		for (auto goList : m_gameObjectListArray) {
			for (auto go : goList) {
				if (go->IsMatchName(objectName)) {
					//見つけた。
					T* p = dynamic_cast<T*>(go);
					objectVector.push_back(p);
				}
			}
		}
		return objectVector;
	}
	
private:
	enum { GAME_OBJECT_PRIO_MAX = 255 };		//!<ゲームオブジェクトの優先度の最大値。
	typedef std::list<CIGameObject*>	 GameObjectList;
	std::array<GameObjectList, GAME_OBJECT_PRIO_MAX>	m_gameObjectListArray;							//!<ゲームオブジェクトの優先度付きリスト。
	static CGameObjectManager* m_instance;		//唯一のインスタンスのアドレスを記録する変数。
};


/*!
*@brief	ゲームオブジェクトの検索のヘルパー関数。
*@details
* 名前の検索が入るため遅いです。
*@param[in]	objectName	ゲームオブジェクトの名前。
*@return 見つかったインスタンスのアドレス。見つからなかった場合はnullptrを返す。
*/
template<class T>
static inline T* FindGO(const char* objectName)
{
	return CGameObjectManager::GetInstance()->FindGameObject<T>(objectName);
}
/*!
	*@brief	ゲームオブジェクトの検索のヘルパー関数(複数対応)。
	*@details
	* 名前の検索が入るため遅いです。
	*@param[in]	objectName	ゲームオブジェクトの名前。
	*@return 見つかったインスタンスのアドレスの配列。
	*/
template<class T>
static inline  const std::vector<T*>& FindGOs(const char* objectName)
{
	return CGameObjectManager::GetInstance()->FindGameObjects<T>(objectName);
}
/*!
*@brief	ゲームオブジェクトの検索のヘルパー関数。
*@details
* 同名のゲームオブジェクトに全てに対して、何らかの処理を行いたい場合に使用してください。
*@param[in]	objectName	ゲームオブジェクトの名前。
*@param[in]	func		ゲームオブジェクトが見つかったときに呼ばれるコールバック関数。
*/
template<class T>
static inline void QueryGOs(const char* objectName, std::function<bool(T* go)> func)
{
	return CGameObjectManager::GetInstance()->QueryGameObjects<T>(objectName, func);
}
/*!
	*@brief	ゲームオブジェクト生成のヘルパー関数。
	*@param[in]	priority	プライオリティ。
	*@param[in]	objectName	オブジェクト名。(NULLの指定可）
	*/
template<class T>
static inline T* NewGO( int priority, const char* objectName = nullptr)
{
	return CGameObjectManager::GetInstance()->NewGameObject<T>( priority, objectName);
}
	
/*!
	*@brief	ゲームオブジェクト削除のヘルパー関数。
	* NewGOを使用して作成したオブジェクトは必ずDeleteGOを実行するように。
	*@param[in]	go		削除するゲームオブジェクト。
	*/
static inline void DeleteGO(CIGameObject* go)
{
	CGameObjectManager::GetInstance()->DeleteGameObject(go);
}



	

 