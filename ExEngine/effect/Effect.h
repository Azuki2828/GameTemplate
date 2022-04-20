/*!
* @brief	エフェクト
*/

#pragma once
#include "EffectEngine.h"
/*!
* @brief	エフェクト。
*/
class Effect : public CIGameObject{
public:
	Effect();
	~Effect();
	
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <remark>
	/// エフェクト再生のための準備を行います。
	/// </remark>
	/// <param name="filePath">ファイルパス</param>
	void Init(const char16_t* filePath);
	/// <summary>
	/// 再生開始
	/// </summary>
	/// <remark>
	/// この関数を呼び出すとエフェクトの再生が開始されます。
	/// </remark>
	void Play();

	void Stop();
	/*!
	*@brief	座標を設定。
	*/
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <returns></returns>
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/*!
	*@brief	回転クォータニオンを設定。
	*/
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// 回転クォータニオンを取得。
	/// </summary>
	/// <returns></returns>
	const CQuaternion& GetRotation() const
	{
		return m_rotation;
	}
	/*!
		*@brief	拡大率を設定。
		*/
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 拡大率を取得。
	/// </summary>
	/// <returns></returns>
	const CVector3& GetScale() const
	{
		return m_scale;
	}
	/// <summary>
	/// エフェクトが再生中か判定。
	/// </summary>
	bool IsPlay() const
	{
		return EffectEngine::GetInstance()->IsPlay(m_handle);
	}
	/*!
	 *@brief	更新。
	 */
	void Update() override;
private:
	Effekseer::EffectRef m_effect;	//エフェクト
	int	m_handle = -1;				//再生中のエフェクトのハンドル。
	CVector3 m_position;				//座標。
	CQuaternion m_rotation;;			//回転。
	CVector3 m_scale = g_vec3One;	//拡大率。
};
