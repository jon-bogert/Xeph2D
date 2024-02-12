#pragma once

#include "Xeph2D/Interfaces/Referenceable.h"
#include "Xeph2D/Utility.h"

#include <Xeph2D/Structs.h>
#include <yaml-cpp/yaml.h>

namespace Xeph2D::CustomSerialTypes
{
	template<typename NODE>
	Transform TransformFromYAML(const NODE& node)
	{
		Transform result;
		result.position.x = node["position"]["x"].as<float>();
		result.position.y = node["position"]["y"].as<float>();
		result.rotation.SetDeg(node["rotation"].as<float>());
		result.scale.x = node["scale"]["x"].as<float>();
		result.scale.y = node["scale"]["y"].as<float>();
		return result;
	}

	template<typename NODE>
	Vector2 Vector2FromYAML(const NODE& node)
	{
		Vector2 result;
		result.x = node["x"].as<float>();
		result.y = node["y"].as<float>();
		return result;
	}

	template<typename NODE>
	Color ColorFromYAML(const NODE& node)
	{
		Color result;
		result.r = node["r"].as<float>();
		result.g = node["g"].as<float>();
		result.b = node["b"].as<float>();
		result.a = node["a"].as<float>();
		return result;
	}

	 // ===== SAVE =====

	template<typename NODE>
	void TransformToYAML(NODE& node, const Transform& transform)
	{
		node["position"]["x"] = transform.position.x;
		node["position"]["y"] = transform.position.y;
		node["rotation"] = transform.rotation.GetDeg();
		node["scale"]["x"] = transform.scale.x;
		node["scale"]["y"] = transform.scale.y;
	}

	template<typename NODE>
	void Vector2ToYAML(NODE& node, const Vector2& vector2)
	{
		node["x"] = vector2.x;
		node["y"] = vector2.y;
	}

	template<typename NODE>
	void ColorToYAML(NODE& node, const Color& color)
	{
		node["r"] = color.r;
		node["g"] = color.g;
		node["b"] = color.b;
		node["a"] = color.a;
	}

	template<typename NODE>
	void RefToYAML(NODE& node, const Ref<Referenceable>& ref)
	{
		if (ref.IsNull())
		{
			node = Utility::ToHex32String(NULL);
			return;
		}

		node = Utility::ToHex32String(ref->InstID());
	}
}