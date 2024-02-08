#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>

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

	inline std::string ToShortString(const std::wstring& wstr)
	{
		const wchar_t* cwstr = wstr.c_str();
		const size_t size = wstr.size() + 1;
		char* buffer = new char[size];
		wcstombs(buffer, cwstr, size);
		std::string result(buffer);
		delete[] buffer;
		return result;
	}

	inline std::wstring ToWideString(const std::string& str)
	{
		const char* cstr = str.c_str();
		const size_t size = str.size() + 1;
		wchar_t* buffer = new wchar_t[size];
		mbstowcs(buffer, cstr, size);
		std::wstring result(buffer);
		delete[] buffer;
		return result;
	}

	
}