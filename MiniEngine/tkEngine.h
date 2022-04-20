#pragma once

#include "HID/GamePad.h"
#include "../MiniEngine/GameTime.h"
#include "../MiniEngine/TResourceBank.h"

class CGraphicsEngine;
class Texture;
class CTkmFile;
class Shader;

class TkEngine {
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TkEngine();
	/// <summary>
	/// フレームの開始時に呼ばれる処理。
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// フレームの終了時に呼ばれる処理。
	/// </summary>
	void EndFrame();
	/// <summary>
	/// ゲームエンジンの初期化。
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);

	/// <summary>
		/// tkmファイルをバンクから取得。
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <returns>tkmファイル。nullptrが返ってきたらバンクに登録されていない。</returns>
	CTkmFile* GetTkmFileFromBank(const char* filePath)
	{
		return m_tkmFileBank.Get(filePath);
	}

	/// <summary>
	/// tkmファイルをバンクに登録。
	/// </summary>
	/// <param name="filePath">tkmファイルパス</param>
	/// <param name="tkmFile"></param>
	void RegistTkmFileToBank(const char* filePath, CTkmFile* tkmFile)
	{
		m_tkmFileBank.Regist(filePath, tkmFile);
	}
	/// <summary>
	/// シェーダーファイルバンクからシェーダーを取得。
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return m_shaderBank.Get(programName.c_str());
	}
	/// <summary>
	/// テクスチャをバンクに登録。
	/// </summary>
	/// <param name="filePath">ファイルパス。</param>
	/// <param name="texture">テクスチャ。</param>
	void RegistTextureToBank(const char* filePath, Texture* texture)
	{
		m_textureBank.Regist(filePath, texture);
	}
	/// <summary>
	/// バンクからテクスチャを取得。
	/// </summary>
	/// <param name="filePath">ファイルパス。</param>
	/// <returns>テクスチャ。</returns>
	Texture* GetTextureFromBank(const char* filePath)
	{
		return m_textureBank.Get(filePath);
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="shader"></param>
	void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		m_shaderBank.Regist(programName.c_str(), shader);
	}
private:
	CGraphicsEngine* m_graphicsEngine = nullptr;	//グラフィックエンジン。
	TResourceBank<CTkmFile> m_tkmFileBank;			// tkmファイルバンク。
	TResourceBank<Shader> m_shaderBank;				// シェーダーバンク。
	TResourceBank<Texture>	m_textureBank;			// テクスチャバンク。
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。
	CGameTime m_gameTime;							//ゲームタイム。
};

extern TkEngine* g_engine;	//TKエンジン。
extern CGameTime* g_gameTime;