#pragma once

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
}