#include "stdafx.h"
#include "LightBase.h"

namespace nsMyGame {

	namespace nsLight {

		bool CLightBase::Start() {

			//ライトマネージャーにライトを追加する。
			CLightManager::GetInstance()->AddLight(this);
			return StartSub();
		}

		void CLightBase::OnDestroy() {

			//ライトマネージャーからライトを削除する。
			CLightManager::GetInstance()->RemoveLight(this);
		}
	}
}
