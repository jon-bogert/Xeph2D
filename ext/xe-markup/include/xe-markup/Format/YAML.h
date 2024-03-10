#pragma once

#include "../IOFormatter.h"

namespace xe::Markup
{
	class YAMLFormatter : public IOFormatterText
	{
	public:
		void SaveToFile(const Node& node, const std::string& path) const override;
		std::string Dump(const Node& node) const override;

		Node LoadFromFile(const std::string& path) const override;
		Node Read(const std::string& contents) const override;
	};
}

