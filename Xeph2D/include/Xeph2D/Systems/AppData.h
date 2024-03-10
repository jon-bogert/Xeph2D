#ifndef __X2D_SYSTEMS_APPDATA_H__
#define __X2D_SYSTEMS_APPDATA_H__

#include <xe-markup/Node.h>

namespace Xeph2D
{
    namespace Markup = xe::Markup;

    class AppData final
    {
    public:
        ~AppData() = default;
        AppData(const AppData& other) = delete;
        AppData(const AppData&& other) = delete;
        AppData operator=(const AppData& other) = delete;
        AppData operator=(const AppData&& other) = delete;

        enum class DataFile : uint8_t
        {
            AssetManifest,
            BuildInfo,
            WindowProperties,
#ifdef _EDITOR
            ComponentManifest,
            Editor,
#endif //_EDITOR
            Count
        };

        static Markup::Node Load(const DataFile file);
#ifdef _EDITOR
        static void Save(const DataFile file, const Markup::Node& node);
        static void BuildForRelease();
#endif _EDITOR

#ifdef NDEBUG
        static void Initialize();
        static void ClearData();
#endif //NDEBUG

    private:
        AppData() {}
        static AppData& Get() { static AppData instance; return instance; }

#ifdef NDEBUG
        Markup::Node m_data;
#endif //NDEBUG

    };

}

#endif //!__X2D_SYSTEMS_APPDATA_H__
