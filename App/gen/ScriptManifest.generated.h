#pragma once

//ScriptManifest.generated.h is Auto-Generated and managed by the Xeph2D Editor.
//Use the Editor to add or remove scripts

#include <Xeph2D.h>

#include <memory>
#include <unordered_map>
#include <cstdint>

#include "TestScript.h"

#define __X2D_REGISTER_COMP_NAMES &Xeph2D::__RegisterComponentNames
#define __X2D_POPULATE_COMP_PTR &Xeph2D::__PopulateComponentPtr

namespace Xeph2D
{
    std::unordered_map<uint32_t, std::string> __RegisterComponentNames()
    {
        return{
        {0x00000001, "Camera"},
        {0x00000002, "SpriteRenderer"},
        {0xa45ff78b, "TestScript"}};
    };

    void __PopulateComponentPtr(std::shared_ptr<Component>& ptr, uint32_t compID)
    {
        switch (compID)
        {
        case 0x00000001: ptr = std::make_shared<Camera>(); break;
        case 0x00000002: ptr = std::make_shared<SpriteRenderer>(); break;
        case 0xa45ff78b: ptr = std::make_shared<TestScript>(); break;
        }
    }
}
