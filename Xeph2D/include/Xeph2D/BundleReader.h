#ifndef __X2D_EDITOR_BUNDLEREADER_H__
#define __X2D_EDITOR_BUNDLEREADER_H__

#include <fstream>
#include <vector>
#include <memory>

namespace Xeph2D
{
    class BundleReader
    {
    public:
        BundleReader() = default;
        BundleReader(const std::filesystem::path& path)
        {
            Open(path);
        }

        void Open(const std::filesystem::path& path)
        {
            if (m_file.is_open())
                Close();

            m_file.open(path, std::ios::binary);

            if (!m_file)
                throw std::runtime_error("Unable to open file");

            size_t numFiles;
            m_file.read(reinterpret_cast<char*>(&numFiles), sizeof(numFiles));

            m_fileData.resize(numFiles);
            for (FileData& data : m_fileData)
            {
                //Name
                size_t nameLength;
                m_file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                data.name.resize(nameLength);
                m_file.read(data.name.data(), nameLength);

                //Position
                m_file.read(reinterpret_cast<char*>(&data.position), sizeof(data.position));

                //Length
                m_file.read(reinterpret_cast<char*>(&data.length), sizeof(data.length));
            }

            //Body Offset
            m_dataOffset = m_file.tellg();
        }

        void Close()
        {
            if (!m_file.is_open())
                return;

            m_file.close();
            m_dataOffset = 0;
            m_fileData.clear();
        }

        bool ReadData(const std::string& key, std::unique_ptr<std::vector<unsigned char>>& dataPtr)
        {
            if (!m_file)
                throw std::runtime_error("file is not open");

            for (const FileData& data : m_fileData)
            {
                if (data.name == key)
                {
                    dataPtr = std::make_unique<std::vector<unsigned char>>(data.length);
                    m_file.seekg(data.position + m_dataOffset, std::ios::beg);
                    m_file.read(reinterpret_cast<char*>(dataPtr->data()), data.length);
                    return true;
                }
            }
            return false;
        }

        bool ReadData(const std::string& key, std::unique_ptr<unsigned char[]>& dataPtr, size_t& dataLength)
        {
            if (!m_file)
                throw std::runtime_error("file is not open");

            for (const FileData& data : m_fileData)
            {
                if (data.name == key)
                {
                    dataLength = data.length;
                    dataPtr = std::make_unique<unsigned char[]>(data.length);
                    m_file.seekg(data.position + m_dataOffset, std::ios::beg);
                    m_file.read(reinterpret_cast<char*>(dataPtr.get()), data.length);
                    return true;
                }
            }
            return false;
        }

        bool ReadDataParallel(const std::string& key, std::unique_ptr<std::vector<unsigned char>>& dataPtr)
        {
            std::ifstream file(m_path, std::ios::binary);

            if (!file)
                throw std::runtime_error("Unable to open file");

            for (const FileData& data : m_fileData)
            {
                if (data.name == key)
                {
                    dataPtr = std::make_unique<std::vector<unsigned char>>(data.length);
                    file.seekg(data.position + m_dataOffset, std::ios::beg);
                    file.read(reinterpret_cast<char*>(dataPtr->data()), data.length);
                    return true;
                }
            }
            return false;
        }

        bool ReadDataParallel(const std::string& key, std::unique_ptr<unsigned char[]>& dataPtr, size_t& dataLength)
        {
            std::ifstream file(m_path, std::ios::binary);

            if (!file)
                throw std::runtime_error("Unable to open file");

            for (const FileData& data : m_fileData)
            {
                if (data.name == key)
                {
                    dataLength = data.length;
                    dataPtr = std::make_unique<unsigned char[]>(data.length);
                    m_file.seekg(data.position + m_dataOffset, std::ios::beg);
                    m_file.read(reinterpret_cast<char*>(dataPtr.get()), data.length);
                    return true;
                }
            }
            return false;
        }

    private:
        struct FileData
        {
            std::string name;
            size_t position;
            size_t length;
        };

        std::ifstream m_file;
        std::string m_path;
        std::vector<FileData> m_fileData;
        size_t m_dataOffset;
    };
}

#endif //!__X2D_EDITOR_BUNDLEREADER_H__