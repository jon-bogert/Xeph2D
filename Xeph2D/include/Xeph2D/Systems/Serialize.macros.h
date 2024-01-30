#pragma once

#define SERIALIZE_INT(var)		SceneManager::__Deserialize(SerializableType::Int, &var, #var)
#define SERIALIZE_FLOAT(var)	SceneManager::__Deserialize(SerializableType::Float, &var, #var)
#define SERIALIZE_BOOL(var)		SceneManager::__Deserialize(SerializableType::Bool, &var, #var)
#define SERIALIZE_CHAR(var)		SceneManager::__Deserialize(SerializableType::Char, &var, #var)
#define SERIALIZE_STRING(var)	SceneManager::__Deserialize(SerializableType::String, &var, #var)
#define SERIALIZE_VEC2(var)		SceneManager::__Deserialize(SerializableType::Vector2, &var, #var)
#define SERIALIZE_COLOR(var)	SceneManager::__Deserialize(SerializableType::Color, &var, #var)
#define SERIALIZE_TRANSF(var)	SceneManager::__Deserialize(SerializableType::Transform, var, #var)
#define SERIALIZE_REF(var)		SceneManager::__Deserialize(SerializableType::Reference, var, #var)