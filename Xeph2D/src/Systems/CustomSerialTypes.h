#pragma once

#include <Xeph2D/Structs.h>
#include "Xeph2D/Systems/AppData.h"

namespace Xeph2D::CustomSerialTypes
{
	template<typename NODE>
	Transform TransformFromYAML(NODE& node)
	{
		Transform result;
		result.position.x = node["position"]["x"].As<float>();
		result.position.y = node["position"]["y"].As<float>();
		result.rotation.SetDeg(node["rotation"].As<float>());
		result.scale.x = node["scale"]["x"].As<float>();
		result.scale.y = node["scale"]["y"].As<float>();
		return result;
	}

	template<typename NODE>
	Vector2 Vector2FromYAML(NODE& node)
	{
		Vector2 result;
		result.x = node["x"].As<float>();
		result.y = node["y"].As<float>();
		return result;
	}

	template<typename NODE>
	Color ColorFromYAML(NODE& node)
	{
		Color result;
		result.r = node["r"].As<float>();
		result.g = node["g"].As<float>();
		result.b = node["b"].As<float>();
		result.a = node["a"].As<float>();
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
}