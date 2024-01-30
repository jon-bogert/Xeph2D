#pragma once

//ScriptManifest.h is Auto-Generated and managed by the Xeph2D Editor.
//Use the Editor to add or remove scripts

#include <Xeph2D.h>

#include <memory>
#include <unordered_map>
#include <cstdint>

#define __X2D_REGISTER_COMP_NAMES &Xeph2D::__RegisterComponentNames
#define __X2D_POPULATE_COMP_PTR &Xeph2D::__PopulateComponentPtr

namespace Xeph2D
{
    std::unordered_map<uint32_t, std::string> __RegisterComponentNames()
    {
        return{
        {0x00000001,"Camera"},
        {0x00000002,"SpriteRenderer"}};
    };

    void __PopulateComponentPtr(std::shared_ptr<Component>& ptr, uint32_t compID)
    {
        switch (compID)
        {
        case 0x00000001: ptr = std::make_unique<Camera>(); break;
        case 0x00000002: ptr = std::make_unique<SpriteRenderer>(); break;
        }
    }
}
