/*!
 *@brief	波形データバンク。
 */
#pragma once

#include <map>
	class CWaveFile;
	typedef std::shared_ptr<CWaveFile>	CWaveFilePtr;
	typedef std::map<unsigned int , CWaveFilePtr>	CWaveFilePtrMap;
	/*!
	 *@brief	波形データバンク。
	 *@details
	 * 一度ロードされた波形データをバンクに登録することが出来ます。
	 * 登録された波形データは再読み込みを行う必要がなく、バンクから再利用することが出来ます。
	 */
	class CWaveFileBank{
	public:
		CWaveFileBank();
		~CWaveFileBank();
		/*!
		 *@brief	波形データを登録。
		 *@param[in]	groupID		グループＩＤ。指定できるＩＤの最大値はMAX_GROUP-1。
		 *@param[in]	waveFile	波形データ。
		 */
		void RegistWaveFile(int groupID, CWaveFilePtr waveFile);
		/*!
		*@brief	引数で指定したファイルパスの波形データがバンクに登録されているか検索する。
		*@param[in]	groupID		グループＩＤ。指定できるＩＤの最大値はMAX_GROUP-1。
		*@param[in]	filePath	ファイルパス。
		*@return 波形データ。登録されていない場合はNULLが返る。
		*/
		CWaveFilePtr FindWaveFile(int groupID, const wchar_t* filePath);
		/*!
		*@brief	引数で指定したファイルパスの波形データがバンクに登録されているか検索する。
		*@details 
		*  こっちのほうが少しだけ早い。
		*@param[in]	groupID		グループＩＤ。指定できるＩＤの最大値はMAX_GROUP-1。
		*@param[in]	nameyKey	名前キー。。
		*@return 波形データ。登録されていない場合はNULLが返る。
		*/
		//CWaveFilePtr FindWaveFile(int groupID, const WNameKey& nameKey);
		/*!
		*@brief	波形データをバンクから登録解除。
		*@param[in]	groupID		グループＩＤ。指定できるＩＤの最大値はMAX_GROUP-1。
		*@param[in]	waveFile	波形データ。
		*/
		void UnregistWaveFile(int groupID, CWaveFilePtr waveFile);
		/*!
		*@brief	グループ単位で解放。
		*/
		void Release(int groupID);
		/*!
		*@brief	全て解放。
		*/
		void ReleaseAll()
		{
			for (int i = 0; i < MAX_GROUP; i++) {
				Release(i);
			}
		}
	private:
		static const int MAX_GROUP = 256;
		CWaveFilePtrMap	m_waveFileMap[MAX_GROUP];		//!<waveファイルのリスト。
	};