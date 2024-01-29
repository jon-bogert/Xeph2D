#pragma once

#define SERIALIZE_INT(var)		SceneLoader::__Deserialize(SerializableType::Int, &var, #var)
#define SERIALIZE_FLOAT(var)	SceneLoader::__Deserialize(SerializableType::Float, &var, #var)
#define SERIALIZE_BOOL(var)		SceneLoader::__Deserialize(SerializableType::Bool, &var, #var)
#define SERIALIZE_CHAR(var)		SceneLoader::__Deserialize(SerializableType::Char, &var, #var)
#define SERIALIZE_STRING(var)	SceneLoader::__Deserialize(SerializableType::String, &var, #var)
#define SERIALIZE_VEC2(var)		SceneLoader::__Deserialize(SerializableType::Vector2, &var, #var)
#define SERIALIZE_COLOR(var)	SceneLoader::__Deserialize(SerializableType::Color, &var, #var)
#define SERIALIZE_TRANSF(var)	SceneLoader::__Deserialize(SerializableType::Transform, var, #var)
#define SERIALIZE_REF(var)		SceneLoader::__Deserialize(SerializableType::Reference, var, #var)