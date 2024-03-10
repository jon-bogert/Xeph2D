#pragma once

#include "../IOFormatter.h"

namespace xe::Markup
{
	class JSONFormatter : public IOFormatterText
	{
	public:
		void SaveToFile(const Node& node, const std::string& path) const override;
		std::string Dump(const Node& node) const override;

		bool GetFloatsOnly() const { return m_floatsOnly; }
		bool GetFloatsOnly(const bool floatsOnly) { m_floatsOnly = floatsOnly; }

		Node LoadFromFile(const std::string& path) const override;
		Node Read(const std::string& contents) const override;

	protected:
		bool m_floatsOnly = false;
	};

	class JSONFormatterPretty : public JSONFormatter
	{
	public:
		std::string Dump(const Node& node) const override;

		
	};
}