#ifndef __X2D_EDITOR_BUNDLEFACTORY_H__
#define __X2D_EDITOR_BUNDLEFACTORY_H__

#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

namespace Xeph2D::Editor
{
	class BundleFactory
	{
		struct FileData
		{
			std::string name;
			std::vector<unsigned char> data;
		};

	public:
		void WriteToFile(const std::filesystem::path& path) const
		{
			if (!path.empty() && !std::filesystem::exists(path.parent_path()))
				std::filesystem::create_directories(path.parent_path());

			std::ofstream file(path, std::ios::binary);

			size_t numFiles = m_fileData.size();
			file.write(reinterpret_cast<const char*>(&numFiles), sizeof(numFiles));

			size_t position = 0;
			for (const FileData& data : m_fileData)
			{
				//Name
				size_t nameLength = data.name.size();
				file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
				file.write(data.name.c_str(), nameLength);

				//Position
				file.write(reinterpret_cast<const char*>(&position), sizeof(position));

				//Length
				size_t length = data.data.size();
				file.write(reinterpret_cast<const char*>(&length), sizeof(length));
				position += length;
			}

			//Body
			for (const FileData& data : m_fileData)
			{
				file.write(reinterpret_cast<const char*>(data.data.data()), data.data.size());
			}

			file.close();
		}

		void AddFile(const std::filesystem::path& path, std::string key = "")
		{
			if (!std::filesystem::exists(path))
				return;

			if (key.empty())
			{
				key = path.filename().u8string();
			}

			std::ifstream file(path, std::ios::binary | std::ios::ate);
			std::size_t size = file.tellg();
			file.seekg(0, std::ios::beg);

			FileData& dataEntry = m_fileData.emplace_back();
			dataEntry.name = key;
			dataEntry.data.resize(size);

			file.read((char*)dataEntry.data.data(), size);
		}

		void AddData(const std::vector<unsigned char>& data, const std::string& key)
		{
			FileData& dataEntry = m_fileData.emplace_back();
			dataEntry.name = key;
			dataEntry.data.resize(data.size());

			std::copy(data.begin(), data.end(), dataEntry.data.begin());
		}

	private:
		std::vector<FileData> m_fileData;
	};
}

#endif //!__X2D_EDITOR_BUNDLEFACTORY_H__

