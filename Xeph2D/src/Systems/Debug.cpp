#include "Xeph2D/Systems/Debug.h"
#ifdef _DEBUG
#include "Xeph2D/Systems/WindowManager.h"
#include "Xeph2D/Systems/Time.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <filesystem>

#include "../../res/JetBrainsMono_ttf.h"

#pragma warning (disable : 4996)
#endif // _DEBUG

using namespace Xeph2D;

#define FONT_SIZE 12
#define LINE_OFFSET 14
#define INSTR_OFFSET 80.f

Xeph2D::Debug::Debug()
{
#ifdef _DEBUG
    res::JetBrainsMono_ttf(m_fontData, m_fontDataLength);

    m_font = std::make_unique<sf::Font>();
    m_font->loadFromMemory((void*)m_fontData.get(), m_fontDataLength);

    m_textTemplate.setFont(*m_font);
    m_textTemplate.setCharacterSize(FONT_SIZE);
    m_textTemplate.setFillColor(m_defaultLogColor);

    InputActionMap* map = InputSystem::CreateInputActionMap("Xeph2D::Debug");
    InputAction* toggleShowGraphics = map->CreateAction("ToggleShowGraphics");
    toggleShowGraphics->AddButton(m_drawGraphicsKey);
    toggleShowGraphics->performed.Subscribe(XEInputActionCallback(Debug::ToggleShowGraphics));
    InputAction* toggleShowOutput = map->CreateAction("ToggleShowOutput");
    toggleShowOutput->AddButton(m_drawOutputKey);
    toggleShowOutput->performed.Subscribe(XEInputActionCallback(Debug::ToggleShowOutput));

    m_instructions = m_textTemplate;
    m_instructions.setString("Press: [F1] to hide Debug Text, [F2] to hide Debug Graphics");
    m_instructions.setPosition(INSTR_OFFSET, 0.f);

    if (m_logToFile)
    {
        if (!std::filesystem::exists(L"log/"))
            std::filesystem::create_directories(L"log/");

        time_t now = time(0);
        struct tm* localtm = localtime(&now);
        char timebuffer[80];
        strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d_%H-%M-%S", localtm);

        m_logFile.open("log/" + std::string(timebuffer) + ".log");
    }
#endif // _DEBUG
}

Xeph2D::Debug::~Debug()
{
#ifdef _DEBUG
    if (m_logFile.is_open())
        m_logFile.close();
#endif // _DEBUG
}

Debug& Debug::Get()
{
    static Debug instance;
    return instance;
}

void Xeph2D::Debug::Update()
{
#ifdef _DEBUG
#ifndef _EDITOR
    if (Get().m_closeOnEscape && InputSystem::GetKeyDown(Key::Esc))
        WindowManager::Close();
#endif //!_EDITOR

    for (size_t i = 0; i < Get().m_logBuffer.size(); ++i)
    {
        Get().m_logBuffer[i].timer += Time::UnscaledDeltaTime();
        if (Get().m_logBuffer[i].timer >= Get().m_logTime)
        {
            Get().m_logBuffer.resize(i);
            return;
        }
    }
#endif //_DEBUG
}

void Xeph2D::Debug::Log(const char* format, ...)
{
#ifdef _DEBUG
    time_t now = time(0);
    struct tm* localtm = localtime(&now);

    char timebuffer[80];

    strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d %H:%M:%S", localtm);

    LogEntry entry;
    entry.dateTime = "{" + std::string(timebuffer) + "} ";

    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    entry.contents = " [LOG] " + std::string(buffer);
    entry.color = Get().m_logColor;
    Get().m_logColor = Get().m_defaultLogColor;

    Get().AddToLogBuffer(entry);
#ifdef _CONSOLE
    std::cout << entry.dateTime + entry.contents << std::endl;
#else
    OutputDebugStringA((entry.dateTime + entry.contents + "\n").c_str());
#endif // _CONSOLE
    Get().LogToFile(entry.dateTime + entry.contents);
#endif //_DEBUG
}

void Xeph2D::Debug::LogWarn(const char* format, ...)
{
#ifdef _DEBUG
    time_t now = time(0);
    struct tm* localtm = localtime(&now);

    char timebuffer[80];

    strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d %H:%M:%S", localtm);

    LogEntry entry;
    entry.dateTime = "{" + std::string(timebuffer) + "} ";

    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    entry.contents = "[WARN] " + std::string(buffer);
    entry.color = Get().m_warnColor;
    Get().m_warnColor = Get().m_defaultWarnColor;

    Get().AddToLogBuffer(entry);
#ifdef _CONSOLE
    std::cout << entry.dateTime + entry.contents << std::endl;
#else
    OutputDebugStringA((entry.dateTime + entry.contents + "\n").c_str());
#endif // _CONSOLE
    Get().LogToFile(entry.dateTime + entry.contents);
#endif //_DEBUG
}

void Xeph2D::Debug::LogErr(const char* format, ...)
{
#ifdef _DEBUG
    time_t now = time(0);
    struct tm* localtm = localtime(&now);

    char timebuffer[80];

    strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d %H:%M:%S", localtm);

    LogEntry entry;
    entry.dateTime = "{" + std::string(timebuffer) + "} ";

    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    entry.contents = " [ERR] " + std::string(buffer);
    entry.color = Get().m_errColor;
    Get().m_errColor = Get().m_defaultErrColor;

    Get().AddToLogBuffer(entry);
#ifdef _CONSOLE
    std::cout << entry.dateTime + entry.contents << std::endl;
#else
    OutputDebugStringA((entry.dateTime + entry.contents + "\n").c_str());
#endif // _CONSOLE
    Get().LogToFile(entry.dateTime + entry.contents);
#endif //_DEBUG
}

void Xeph2D::Debug::LogColor(Color color, LogType type)
{
#ifdef _DEBUG
    switch (type)
    {
    case LogType::Log:
        Get().m_logColor = color;
        break;
    case LogType::Warn:
        Get().m_warnColor = color;
        break;
    case LogType::Err:
        Get().m_errColor = color;
        break;
    }
#endif // _DEBUG
}

void Xeph2D::Debug::Monitor(const std::string& key, const std::string& valueStr)
{
#ifdef _DEBUG
    Get().m_monitorBuffer[key] = valueStr;
#endif // _DEBUG
}

void Xeph2D::Debug::ClearMonitorBuffer()
{
#ifdef _DEBUG
    Get().m_monitorBuffer.clear();
#endif // _DEBUG
}

void Debug::DrawLine(Vector2 start, Vector2 end, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::VertexArray line = sf::VertexArray(sf::LinesStrip, 2);
    line[0].position = (isWorldSpace) ? WindowManager::WorldToPixel(start) : WindowManager::ScreenToPixel(start);
    line[1].position = (isWorldSpace) ? WindowManager::WorldToPixel(end) : WindowManager::ScreenToPixel(end);
    line[0].color = line[1].color = color;
    Get().m_lineBuffer.push_back(line);
#endif // _DEBUG
}

void Debug::DrawChainLine(const VertexChain& vertBuffer, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::VertexArray chain = sf::VertexArray(sf::LinesStrip, vertBuffer.size());
    for (int i = 0; i < vertBuffer.size(); ++i)
    {
        chain[i].position = (isWorldSpace) ? WindowManager::WorldToPixel(vertBuffer[i]) : WindowManager::ScreenToPixel(vertBuffer[i]);
        chain[i].color = color;
    }
    Get().m_lineBuffer.push_back(chain);
#endif // _DEBUG
}

void Debug::DrawBoxOutline(Vector2 center, Vector2 span, float rotation, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::RectangleShape shape;
    shape.setFillColor(Color::Transparent);
    shape.setOutlineColor(color);
    shape.setOutlineThickness(-1.f);
    Vector2 size = WindowManager::UnitToPixel(span);
    shape.setSize(size);
    shape.setOrigin(size * 0.5f);
    shape.setPosition((isWorldSpace) ? WindowManager::WorldToPixel(center) : WindowManager::ScreenToPixel(center));
    shape.setRotation(rotation);
    Get().m_rectBuffer.push_back(shape);
#endif // _DEBUG
}

void Debug::DrawBoxFilled(Vector2 center, Vector2 span, float rotation, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::RectangleShape shape;
    shape.setFillColor(color);
    Vector2 size = WindowManager::UnitToPixel(span);
    shape.setSize(size);
    shape.setOrigin(size * 0.5f);
    shape.setPosition((isWorldSpace) ? WindowManager::WorldToPixel(center) : WindowManager::ScreenToPixel(center));
    shape.setRotation(rotation);
    Get().m_rectBuffer.insert(Get().m_rectBuffer.begin(), shape);
#endif // _DEBUG
}

void Debug::DrawCircleOutline(Vector2 center, float radius, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::CircleShape shape;
    shape.setFillColor(Color::Transparent);
    shape.setOutlineColor(color);
    shape.setOutlineThickness(-1.f);
    float rad = WindowManager::UnitToPixel(radius);
    shape.setRadius(rad);
    shape.setOrigin(rad, rad);
    shape.setPosition((isWorldSpace) ? WindowManager::WorldToPixel(center) : WindowManager::ScreenToPixel(center));
    Get().m_circleBuffer.push_back(shape);
#endif // _DEBUG
}

void Debug::DrawCircleFilled(Vector2 center, float radius, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::CircleShape shape;
    shape.setFillColor(color);
    float rad = WindowManager::UnitToPixel(radius);
    shape.setRadius(rad);
    shape.setOrigin(rad, rad);
    shape.setPosition((isWorldSpace) ? WindowManager::WorldToPixel(center) : WindowManager::ScreenToPixel(center));
    Get().m_circleBuffer.insert(Get().m_circleBuffer.begin(), shape);
#endif // _DEBUG
}

void Debug::DrawToWindow()
{
#ifdef _DEBUG

#ifdef _EDITOR
    if (Get().m_drawGraphics)
    {
        for (sf::RectangleShape& r : Get().m_rectBuffer)
            WindowManager::__Viewport()->draw(r);

        for (sf::CircleShape& c : Get().m_circleBuffer)
            WindowManager::__Viewport()->draw(c);

        for (sf::VertexArray& va : Get().m_lineBuffer)
        {
            if (va.getVertexCount() <= 1)
                return;

            WindowManager::__Viewport()->draw(va);
        }
    }

    if (Get().m_drawOutput)
    {
        WindowManager::__Viewport()->draw(Get().m_instructions);

        Get().UpdateTextBuffer();

        for (sf::Text& t : Get().m_textBuffer)
        {
            WindowManager::__Viewport()->draw(t);
        }
    }
#else
    if (Get().m_drawGraphics)
    {
        for (sf::RectangleShape& r : Get().m_rectBuffer)
            WindowManager::UnWrap()->draw(r);

        for (sf::CircleShape& c : Get().m_circleBuffer)
            WindowManager::UnWrap()->draw(c);

        for (sf::VertexArray& va : Get().m_lineBuffer)
        {
            if (va.getVertexCount() <= 1)
                return;

            WindowManager::UnWrap()->draw(va);
        }
    }

    if (Get().m_drawOutput)
    {
        WindowManager::UnWrap()->draw(Get().m_instructions);

        Get().UpdateTextBuffer();

        for (sf::Text& t : Get().m_textBuffer)
        {
            WindowManager::UnWrap()->draw(t);
        }
    }
#endif //_EDITOR

    Get().m_rectBuffer.clear();
    Get().m_circleBuffer.clear();
    Get().m_lineBuffer.clear();
#endif // _DEBUG
}

#ifdef _DEBUG
void Xeph2D::Debug::UpdateTextBuffer()
{
    m_textBuffer.resize(1 + m_monitorBuffer.size() + m_logBuffer.size(), m_textTemplate);
    m_textBuffer[0].setString("FPS: " + std::to_string(Time::FPS()));
    size_t index = 1;
    for (auto& keyval : m_monitorBuffer)
    {
        sf::Text& text = m_textBuffer[index];
        text.setString(keyval.first + ": " + keyval.second);
        text.setPosition(0, LINE_OFFSET * (index++));
    }
    for (LogEntry& entry : m_logBuffer)
    {
        sf::Text& text = m_textBuffer[index];
        Color color = entry.color;
        color.a = (m_logTime - entry.timer) / m_logTime;
        text.setColor(color);
        text.setString(entry.contents);
        text.setPosition(0, LINE_OFFSET * (index++));
    }
}

void Xeph2D::Debug::AddToLogBuffer(const LogEntry& entry)
{
    m_logBuffer.push_front(entry);
}

void Xeph2D::Debug::ToggleShowOutput(InputAction* ctx)
{
    m_drawOutput = !m_drawOutput;
}

void Xeph2D::Debug::ToggleShowGraphics(InputAction* ctx)
{
    m_drawGraphics = !m_drawGraphics;
}

void Xeph2D::Debug::LogToFile(const std::string& msg)
{
    if (!m_logToFile || !m_logFile.is_open())
        return;

    m_logFile << msg << std::endl;
    m_logFile.flush();
}
#endif // _DEBUG
