#ifndef __X2DIS_EDITORIS_EDITORSCENEDATA_H__
#define __X2DIS_EDITORIS_EDITORSCENEDATA_H__

#ifdef IS_EDITOR
#include <variant>
#include "Xeph2D/Structs.h"
#include "Xeph2D/Ref.h"

namespace Xeph2D::Edit
{
	struct Field
	{
		using Value = std::variant<
			int,
			float,
			bool,
			char,
			std::string,
			Xeph2D::Vector2,
			Xeph2D::Color,
			Xeph2D::Transform,
			uint32_t>; //uint32_t is the instID of the GameObject

		std::string name;
		SerializableType type;
		void* ptr;
		template<typename T>
		T& As()
		{
			return *static_cast<T*>(ptr);
		}
		template<typename T>
		const T& As() const
		{
			return *static_cast<const T*>(ptr);
		}
	};

	struct EditorComponent
	{
		uint32_t typeID;
		Field enabled;
		std::vector<Field> fields;
		Field* GetField(const std::string& name)
		{
			auto result = std::find_if(
				fields.begin(),
				fields.end(),
				[=](Field& f) { return f.name == name; });

			if (result == fields.end())
				return nullptr;
			return &(*result);
		}
	};

	struct EditorGameObject
	{
		uint32_t instID;
		Field isActive;
		Field transform;
		Field name;
		std::vector<EditorComponent> components;

		EditorComponent* GetComponent(uint32_t typeID)
		{
			auto result = std::find_if(
				components.begin(),
				components.end(),
				[=](EditorComponent& c) { return c.typeID == typeID; });

			if (result == components.end())
				return nullptr;
			return &(*result);
		}
	};

	class EditorSceneData
	{
	public:

		std::vector<EditorGameObject> gameObjects;
		EditorGameObject* GetGameObject(uint32_t instID)
		{
			auto result = std::find_if(
				gameObjects.begin(),
				gameObjects.end(),
				[=](EditorGameObject& go) { return go.instID == instID; });

			if (result == gameObjects.end())
				return nullptr;
			return &(*result);
		}
	};
}
#endif //IS_EDITOR

#endif //!__X2DIS_EDITORIS_EDITORSCENEDATA_H__
