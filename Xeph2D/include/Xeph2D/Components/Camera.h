#ifndef __XEPH2D_COMPONENTS_CAMERA_H__
#define __XEPH2D_COMPONENTS_CAMERA_H__

#include "Xeph2D/Component.h"
#include "Xeph2D/Systems/SceneManager.h"

namespace Xeph2D
{
	class Camera : public Component
	{
		friend class WindowManager;
		XEPH2D_REG_COMP(0x00000001);

		void OnEditorStart() override;

		void Awake() override;
		void OnDestroy() override;
	};
}

#endif //!__XEPH2D_COMPONENTS_CAMERA_H__
