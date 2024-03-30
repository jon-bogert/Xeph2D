#pragma once

//ScriptManifest.generated.h is Auto-Generated and managed by the Xeph2D Editor.
//Use the Editor to add or remove scripts

#include <Xeph2D.h>

#include <memory>
#include <cstdint>

#include "CoolScript.h"
#include "TestScript.h"

#define __X2D_POPULATE_COMP_PTR &Xeph2D::__PopulateComponentPtr

namespace Xeph2D
{
    void __PopulateComponentPtr(std::shared_ptr<Component>& ptr, uint32_t compID)
    {
        switch (compID)
        {
        case 0x00000001: ptr = std::make_shared<Camera>(); break;
        case 0x00000002: ptr = std::make_shared<SpriteRenderer>(); break;
        case 0x00000003: ptr = std::make_shared<SpritesheetRenderer>(); break;
        case 0x00000004: ptr = std::make_shared<TextRenderer>(); break;
        case 0x0867932b: ptr = std::make_shared<CoolScript>(); break;
        case 0xa45ff78b: ptr = std::make_shared<TestScript>(); break;
        }
    }
}
