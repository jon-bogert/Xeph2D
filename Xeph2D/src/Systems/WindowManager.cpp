#include "Xeph2D/Systems/WindowManager.h"
#include "Xeph2D/Systems/Debug.h"

#include "Xeph2D/GameObject.h"

#include <SFML/Window/WindowStyle.hpp>

#define WINDOW_INFO_FILE "settings/WindowInfo.yaml"

using namespace Xeph2D;

#ifdef _EDITOR
#include "Editor/Editor.h"
#define __CAMERA Edit::Editor::GetViewportTransform()
#else
#define __CAMERA Get().m_camera
#define __POSITION LocalPosition()// TODO - Change to Global Position once implemented
#endif //_EDITOR

namespace
{
	int SFStyle(Xeph2D::WindowStyle style)
	{
		switch (style)
		{
		case WindowStyle::Windowed: return sf::Style::Close;
		case WindowStyle::Borderless: return sf::Style::None;
		case WindowStyle::Fullscreen: return sf::Style::Fullscreen;
		case WindowStyle::WindowedWithResize: return sf::Style::Default;
		case WindowStyle::WindowNoClose: return sf::Style::Titlebar;
		default:
			Debug::LogErr("WindowManager.cpp ::SFStyle -> Unimplemented WindowStyle type");
			return sf::Style::Close;
		}
	}
}

WindowManager& WindowManager::Get()
{
	static WindowManager instance;
	return instance;
}

#ifdef _EDITOR
void Xeph2D::WindowManager::__UpdateViewportSize(Vector2 size)
{
	float windRatio = size.x / size.y;
	if (windRatio > Get().m_aspect)
	{
		Get().m_width = size.y * Get().m_aspect;
		Get().m_height = size.y;
	}
	else
	{
		Get().m_width = size.x;
		Get().m_height = size.x / Get().m_aspect;
	}
	Get().m_resScale = Get().m_height / static_cast<float>(Get().m_refHeight);
	Get().m_viewport->create(Get().m_width, Get().m_height);
}
#endif //_EDITOR

void Xeph2D::WindowManager::Initialize()
{
	YAML::Node windowInfo = YAML::LoadFile(WINDOW_INFO_FILE);
	std::string title = windowInfo["title"].as<std::string>();
#ifdef _DEBUG
	Get().m_width = windowInfo["debug-resolution"]["width"].as<uint32_t>();
	Get().m_height = windowInfo["debug-resolution"]["height"].as<uint32_t>();
	WindowStyle style = static_cast<WindowStyle>(windowInfo["debug-style"].as<int>());
#else
	Get().m_width = windowInfo["resolution"]["width"].as<uint32_t>();
	Get().m_height = windowInfo["resolution"]["height"].as<uint32_t>();
	WindowStyle style = static_cast<WindowStyle>(windowInfo["style"].as<int>());
#endif //_DEBUG

	if (Get().m_width == 0 || Get().m_height == 0)
	{
		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
		Get().m_width = desktop.width;
		Get().m_height = desktop.height;
	}

	Get().m_resScale = Get().m_height / static_cast<float>(Get().m_refHeight);

#ifdef _EDITOR
	Get().m_viewport = std::make_unique<sf::RenderTexture>();
	Get().m_viewport->create(width, height);
#else
	Get().m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(Get().m_width, Get().m_height), title, SFStyle(style));
	Get().m_handle = FindWindowA(NULL, title.c_str());

	uint32_t fps = windowInfo["lock-framerate"].as<uint32_t>();
	if (fps > 0)
		Get().m_window->setFramerateLimit(fps);

#endif //_EDITOR
}

void Xeph2D::WindowManager::Draw(sf::Drawable* item)
{
#ifdef _EDITOR
	Get().m_viewport->draw(*item);
#else
	Get().m_window->draw(*item);
#endif // _EDITOR
}

void Xeph2D::WindowManager::PrepareTransformable(const Ref<GameObject>& gameObject, sf::Transformable* transformable)
{
	Transform finalTransform = gameObject->m_transform;
	//if (gameObject->GetParent())
	//{
	//	GameObject* currObj = gameObject->GetParent();
	//	while (currObj)
	//	{
	//		finalTransform.position.x += currObj->transform.position.x;
	//		finalTransform.position.y += currObj->transform.position.y;
	//
	//		finalTransform.rotation.AddDeg(currObj->transform.rotation.GetDeg());
	//
	//		finalTransform.scale.x *= currObj->transform.scale.x;
	//		finalTransform.scale.y *= currObj->transform.scale.y;
	//
	//		currObj = currObj->GetParent();
	//	}
	//}
	
	transformable->setPosition({ // TODO -> Change LocalPosition() to GlobalPosition()
	(finalTransform.position.x * Get().m_ppu * Get().m_resScale) - (__CAMERA->LocalPosition().x * Get().m_ppu * Get().m_resScale),
	Get().m_height - ((finalTransform.position.y * Get().m_ppu * Get().m_resScale) - (__CAMERA->LocalPosition().y * Get().m_ppu * Get().m_resScale)) });
	
	transformable->setScale(finalTransform.scale.x * Get().m_resScale, finalTransform.scale.y * Get().m_resScale);
	transformable->setRotation(finalTransform.rotation.GetDeg());
}

void Xeph2D::WindowManager::CheckWindowEvents()
{
	sf::Event winEvent;
	while (Get().m_window->pollEvent(winEvent))
	{
		if (winEvent.type == sf::Event::Closed)
			Close();
		if (winEvent.type == sf::Event::Resized)
		{
			sf::FloatRect visibleArea(0, 0, winEvent.size.width, winEvent.size.height);
			Get().m_window->setView(sf::View(visibleArea));
			Get().m_width = winEvent.size.width;
			Get().m_height = winEvent.size.height;
			Get().m_resScale = Get().m_height / static_cast<float>(Get().m_refHeight);
		}
	}
}

void Xeph2D::WindowManager::Begin()
{
#ifdef _EDITOR
	Get().m_viewport->clear(Color(0.1f, 0.1f, 0.1f, 1.f));
#else
	Get().m_window->clear(sf::Color::Black);
#endif //_EDITOR
}

void Xeph2D::WindowManager::End()
{
#ifdef _EDITOR
	Get().m_viewport->display();
#else
	Get().m_window->display();
#endif //_EDITOR
}

bool Xeph2D::WindowManager::IsOpen()
{
	return Get().m_window->isOpen();
}

void Xeph2D::WindowManager::Close()
{
	Get().m_window->close();
}

HWND& WindowManager::GetHandle()
{
	return Get().m_handle;
}

uint32_t Xeph2D::WindowManager::GetWidthPixels()
{
	return Get().m_width;
}

uint32_t Xeph2D::WindowManager::GetHeightPixels()
{
	return Get().m_height;
}

float Xeph2D::WindowManager::GetWidthUnits()
{
	return PixelToUnit(Get().m_width);
}

float Xeph2D::WindowManager::GetHeightUnits()
{
	return PixelToUnit(Get().m_height);
}

Vector2 Xeph2D::WindowManager::WorldWindowMinimum()
{
	return Vector2(__CAMERA->__POSITION.x, __CAMERA->__POSITION.y);
}

Vector2 Xeph2D::WindowManager::WorldWindowMaximum()
{
	return Vector2(__CAMERA->__POSITION.x + (float)GetWidthUnits(), __CAMERA->__POSITION.y + (float)GetHeightUnits());
}

Vector2 Xeph2D::WindowManager::PixelToScreen(const Vector2 point)
{
	return Vector2(point.x / static_cast<float>(Get().m_ppu) / Get().m_resScale, (Get().m_height - point.y) / static_cast<float>(Get().m_ppu) / Get().m_resScale);
}

Vector2 Xeph2D::WindowManager::PixelToWorld(const Vector2 point)
{
	return PixelToScreen(point) - __CAMERA->__POSITION;
}

Vector2 Xeph2D::WindowManager::ScreenToPixel(const Vector2 point)
{
	return Vector2(point.x * Get().m_ppu * Get().m_resScale, Get().m_height - point.y * Get().m_ppu * Get().m_resScale);
}

Vector2 Xeph2D::WindowManager::WorldToPixel(const Vector2 point)
{
	return Vector2((point.x - __CAMERA->__POSITION.x) * Get().m_ppu * Get().m_resScale, Get().m_height - (point.y - __CAMERA->__POSITION.y) * Get().m_ppu * Get().m_resScale);
}

float Xeph2D::WindowManager::PixelToUnit(const float val)
{
	return val / Get().m_ppu / Get().m_resScale;
}

Vector2 Xeph2D::WindowManager::PixelToUnit(const Vector2 val)
{
	return val / Get().m_ppu / Get().m_resScale;
}

float Xeph2D::WindowManager::UnitToPixel(const float val)
{
	return val * Get().m_ppu * Get().m_resScale;
}

Vector2 Xeph2D::WindowManager::UnitToPixel(const Vector2 val)
{
	return val * Get().m_ppu * Get().m_resScale;
}

void Xeph2D::WindowManager::SetTargetFramerate(uint32_t framerate)
{
#ifndef _EDITOR
	Get().m_window->setFramerateLimit(framerate);
#endif //!_EDITOR
}

sf::RenderWindow* Xeph2D::WindowManager::UnWrap()
{
	return Get().m_window.get();
}

void Xeph2D::WindowManager::SetCamera(Camera* camera)
{
	if (Get().m_camera)
		Debug::LogWarn("WindowManager::SetCamera -> Camera was not cleared before setting");

	Get().m_camera = camera;
}

void Xeph2D::WindowManager::ClearCamera(Camera* camera)
{
	if (!camera)
	{
		Get().m_camera = nullptr;
		return;
	}

	if (camera == Get().m_camera)
	{
		Get().m_camera = nullptr;
	}

	Debug::LogWarn("WindowManager::ClearCamera -> Camera to be cleared was not the same as provided camera");
}
