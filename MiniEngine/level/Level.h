#pragma once

#include "MapChip.h"
#include "../tkFile/TklFile.h"


namespace nsMyGame {

	struct LevelObjectData {
		CVector3 position;		//<座標。
		CQuaternion rotation;	//!<回転。
		CVector3 scale;			//!<拡大率。
		const wchar_t* name;	//!<名前。
		/*!
		* @brief	引数で渡したオブジェクト名のオブジェクトか調べる。
		*@param[in]	objName		調べる名前。
		*@return	名前が同じ場合にtrueを返します。
		*/
		bool EqualObjectName(const char* objName)
		{
			char objNameBase[256];
			wcstombs(objNameBase, name, 256);

			return strcmp(objName, objNameBase) == 0;
		}
		/*!
		* @brief	名前が前方一致するか調べる。
		*/
		bool ForwardMatchName(const wchar_t* n)
		{
			auto len = wcslen(n);
			auto namelen = wcslen(name);
			if (len > namelen) {
				//名前が長い。不一致。
				return false;
			}
			return wcsncmp(n, name, len) == 0;
		}
	};

	class Level
	{
	public:
		void Init(const char* filePath, std::function<bool(LevelObjectData& objData)> hookFunc);
		void Draw(CRenderContext& rc);
	private:
		CTklFile m_tklFile;
		std::vector<std::unique_ptr<MapChip>> m_mapChipPtrs;			//マップチップの可変長配列。
	};
}

