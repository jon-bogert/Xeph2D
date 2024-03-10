#pragma once

#include "../IOFormatter.h"

namespace xe::Markup
{
	class BSONFormatter : public IOFormatterBinary
	{
	public:
		void SaveToFile(const Node& node, const std::string& path) const override;
		std::vector<uint8_t> Dump(const Node& node) const override;

		bool GetFloatsOnly() const { return m_floatsOnly; }
		bool GetFloatsOnly(const bool floatsOnly) { m_floatsOnly = floatsOnly; }

		Node LoadFromFile(const std::string& path) const override;
		Node Read(const std::vector<uint8_t>& contents) const override;

	protected:
		bool m_floatsOnly = false;
	};
}