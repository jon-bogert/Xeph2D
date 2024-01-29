#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

namespace Xeph2D::Utility
{
	inline std::vector<const char*> CStrVect(const std::vector<std::string>& input)
	{
		std::vector<const char*> inputCStr;
		for (const std::string& item : input)
		{
			inputCStr.push_back(item.c_str());
		}
		return inputCStr;
	}

	inline std::string ToHex32String(const uint32_t val)
	{
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0') << std::setw(8)
			<< std::hex << val;

		std::string hexString = stream.str();
		return hexString;
	}

	inline uint32_t FromHex32String(const std::string& hexStr)
	{
		uint32_t val = std::stoul(hexStr, nullptr, 16);
		return val;
	}

	
}