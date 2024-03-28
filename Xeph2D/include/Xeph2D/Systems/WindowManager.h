#pragma once

#include "Xeph2D/GameObject.h"
#include "Xeph2D/Components/Camera.h"

#include <SFML.hpp>

#include <memory>
#include <Windows.h>

namespace Xeph2D
{
	enum class WindowStyle
	{
		Windowed,
		Borderless,
		Fullscreen,
		WindowedWithResize,
		WindowNoClose
	};

	class WindowManager final
	{
	public:
#ifdef IS_EDITOR
		static sf::RenderTexture* __Viewport() { return Get().m_viewport.get(); }
		static void __UpdateViewportSize(Vector2 size);
#endif //IS_EDITOR

		~WindowManager() = default;
		WindowManager(const WindowManager& other) = delete;
		WindowManager(const WindowManager&& other) = delete;
		WindowManager operator=(const WindowManager& other) = delete;
		WindowManager operator=(const WindowManager&& other) = delete;

		static void Initialize();

		static void Draw(sf::Drawable* item);
		static void PrepareTransformable(const Ref<GameObject>& gameObject, sf::Transformable* transformable);
		static void PrepareTransformableUI(const Ref<GameObject>& gameObject, sf::Transformable* transformable);

		static void CheckWindowEvents();
		static void Begin();
		static void End();

		static bool IsOpen();
		static void Close();
		static HWND& GetHandle();

		static uint32_t GetWidthPixels();
		static uint32_t GetHeightPixels();
		static float GetWidthUnits();
		static float GetHeightUnits();

		static Vector2 WorldWindowMinimum();
		static Vector2 WorldWindowMaximum();

		static Vector2 PixelToScreen(const Vector2 point);
		static Vector2 PixelToWorld(const Vector2 point);
		static Vector2 ScreenToPixel(const Vector2 point);
		static Vector2 WorldToPixel(const Vector2 point);
		static float PixelToUnit(const float val);
		static Vector2 PixelToUnit(const Vector2 val);
		static float UnitToPixel(const float val);
		static Vector2 UnitToPixel(const Vector2 val);

		static void SetTargetFramerate(uint32_t framerate);


	private:
#ifdef IS_EDITOR
		std::unique_ptr<sf::RenderTexture> m_viewport = nullptr;
#endif //IS_EDITOR

		WindowManager() {}
		static WindowManager& Get();

		friend class Debug;
		static sf::RenderWindow* UnWrap();

		friend class Camera;
		static void SetCamera(Camera* camera);
		static void ClearCamera(Camera* camera = nullptr);

		std::unique_ptr<sf::RenderWindow> m_window = nullptr;

		uint32_t m_width = 1280;
		uint32_t m_height = 720;
		uint32_t m_refWidth = 1920;
		uint32_t m_refHeight = 1080;
		float m_aspect = m_refWidth / (float)m_refHeight;
		uint32_t m_ppu = 32;

		float m_resScale = 1.f;

		Camera* m_camera = nullptr;

		HWND m_handle = nullptr;
	};
}