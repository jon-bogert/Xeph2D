#pragma once
#include "Xeph2D/Structs.h"
#include "InputSystem/InputSystem.h"
#include <iostream>
#include <deque>
#include <fstream>

//namespace Xeph2D
//{
//	class Debug final
//	{
//	public:
//		using VertexChain = std::vector<Vector2>;
//		enum class LogType {Log, Warn, Err};
//
//		~Debug();
//		Debug(const Debug& other) = delete;
//		Debug(const Debug&& other) = delete;
//		Debug operator=(const Debug& other) = delete;
//		Debug operator=(const Debug&& other) = delete;
//
//		static void Update();
//
//		static void Log(const char* format, ...);
//		static void LogWarn(const char* format, ...);
//		static void LogErr(const char* format, ...);
//		static void LogColor(Color color, LogType type = LogType::Log);
//		static void Monitor(const std::string& key, const std::string& valueStr);
//		static void ClearMonitorBuffer();
//
//		static void DrawLine(Vector2 start, Vector2 end, Color color = Color::White, bool isWorldSpace = true);
//		static void DrawChainLine(const VertexChain& vertBuffer, Color color = Color::White, bool isWorldSpace = true);
//		static void DrawBoxOutline(Vector2 center, Vector2 span, float rotation = 0.f, Color color = Color::White, bool isWorldSpace = true);
//		static void DrawBoxFilled(Vector2 center, Vector2 span, float rotation = 0.f, Color color = Color::White, bool isWorldSpace = true);
//		static void DrawCircleOutline(Vector2 center, float radius, Color color = Color::White, bool isWorldSpace = true);
//		static void DrawCircleFilled(Vector2 center, float radius, Color color = Color::White, bool isWorldSpace = true);
//
//		static void DrawToWindow();
//
//	private:
//		struct LogEntry
//		{
//			Color color = Color::LightGrey;
//			float timer = 0.f;
//			std::string dateTime = "";
//			std::string contents = "";
//		};
//
//		Debug();
//		static Debug& Get();
//
//#ifdef _DEBUG
//		void UpdateTextBuffer();
//		void AddToLogBuffer(const LogEntry& entry);
//		void ToggleShowOutput(InputAction* ctx);
//		void ToggleShowGraphics(InputAction* ctx);
//		void LogToFile(const std::string& msg);
//
//		bool m_closeOnEscape = true;
//		bool m_logToFile = false;
//		std::string m_logPath = "log/";
//		std::ofstream m_logFile;
//
//		bool m_drawOutput = true;
//		bool m_drawGraphics = true;
//		Key m_drawOutputKey = Key::F1;
//		Key m_drawGraphicsKey = Key::F2;
//
//		std::vector<sf::RectangleShape> m_rectBuffer;
//		std::vector<sf::CircleShape> m_circleBuffer;
//		std::vector<sf::VertexArray> m_lineBuffer;
//
//		std::unique_ptr<sf::Font> m_font = nullptr;
//		std::unique_ptr<unsigned char[]> m_fontData = nullptr;
//		size_t m_fontDataLength = 0;
//		std::deque<sf::Text> m_textBuffer;
//		sf::Text m_instructions;
//		sf::Text m_textTemplate{};
//
//		std::map<std::string, std::string> m_monitorBuffer;
//		std::deque<LogEntry> m_logBuffer;
//		Color m_defaultLogColor = Color::LightGrey;
//		Color m_defaultWarnColor = Color::Yellow;
//		Color m_defaultErrColor = Color::Red;
//		Color m_logColor = m_defaultLogColor;
//		Color m_warnColor = m_defaultWarnColor;
//		Color m_errColor = m_defaultErrColor;
//		float m_logTime = 5.f;
//#endif // _DEBUG
//	};
//}

//vvv--- TEMPORARY ---vvv

namespace Xeph2D::Debug
{
    inline void Log(const char* format, ...)
    {
        printf("[LOG] ");
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
    }

    inline void LogWarn(const char* format, ...)
    {
        printf("[WARN] ");
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
    }

    inline void LogErr(const char* format, ...)
    {
        printf("[ERR] ");
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
    }
}