#pragma once

#include "Node.h"

#include <string>
#include <fstream>

namespace xe::Markup
{
	class IOFormatter
	{
	public:
		virtual void SaveToFile(const Node& node, const std::string& path) const = 0;
		virtual Node LoadFromFile(const std::string& path) const = 0;
	};

	class IOFormatterText : public IOFormatter
	{
		virtual std::string Dump(const Node& node) const = 0;
		virtual Node Read(const std::string& contents) const = 0;
	};

	class IOFormatterBinary : public IOFormatter
	{
		virtual std::vector<uint8_t> Dump(const Node& node) const = 0;
		virtual Node Read(const std::vector<uint8_t>& contents) const = 0;
	};
}