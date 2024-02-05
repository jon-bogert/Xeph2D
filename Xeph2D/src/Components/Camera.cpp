#include "Xeph2D/Components/Camera.h"

#include "Xeph2D/Systems/WindowManager.h"

void Xeph2D::Camera::OnEditorStart()
{
	Awake();
}

void Xeph2D::Camera::Awake()
{
	WindowManager::SetCamera(this);
}

void Xeph2D::Camera::OnDestroy()
{
	if (!(WindowManager::Get().m_camera == this))
	{
		Debug::LogErr("Camera -> Tried to clear camera when not active in WindowManager");
		return;
	}
	WindowManager::ClearCamera();
}
