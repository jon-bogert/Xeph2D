#ifndef __X2D_SYSTEMS_APPDATA_H__
#define __X2D_SYSTEMS_APPDATA_H__

#include <xe-markup/Node.h>
#include "../Structs.h"
#include "../BundleReader.h"

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
        static bool GetAssetData(const AssetType type, const std::string& key, std::unique_ptr<std::vector<unsigned char>>& dataPtr);
#endif //NDEBUG

        static Markup::Node GetSceneData(const std::string& scene);

    private:
        AppData() {}
        static AppData& Get() { static AppData instance; return instance; }

#ifdef NDEBUG
        Markup::Node m_data;
        BundleReader m_bundleReader;
#endif //NDEBUG

    };

}

#endif //!__X2D_SYSTEMS_APPDATA_H__
