#ifndef __X2D_SYSTEMS_RUNTIME_H__
#define __X2D_SYSTEMS_RUNTIME_H__

#include <memory>
#include <functional>
#include <string>
#include <unordered_map>

namespace Xeph2D
{
    class Component;
    class Runtime final
    {
    public:
        ~Runtime() = default;
        Runtime(const Runtime& other) = delete;
        Runtime(const Runtime&& other) = delete;
        Runtime operator=(const Runtime& other) = delete;
        Runtime operator=(const Runtime&& other) = delete;

        static void Initialize(
            std::function<std::unordered_map<uint32_t, std::string>(void)> namingCallback,
            std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback
        );
        static void Update();
        static void Terminate();
        static bool IsOpen() { return Get().m_isOpen; }

    private:
        Runtime() {}
        static Runtime& Get() { static Runtime instance; return instance; }

        bool m_isOpen = true;
    };
}

#endif //__X2D_SYSTEMS_RUNTIME_H__
