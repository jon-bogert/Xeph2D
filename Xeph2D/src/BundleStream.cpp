#include "Xeph2D/BundleStream.h"

sf::BundleStream::BundleStream(const std::string& filename, size_t offset, size_t size)
{
    open(filename, offset, size);
}

sf::BundleStream::BundleStream(const BundleStream& other)
{
    if (other.isOpen())
        m_file.open(other.m_filename);
        
    m_filename = other.m_filename;
    m_offset = other.m_offset;
    m_size = other.m_size;
}

sf::BundleStream sf::BundleStream::operator=(const BundleStream& other)
{
    if (other.isOpen())
    {
        close();
        m_file.open(other.m_filename);
    }

    m_filename = other.m_filename;
    m_offset = other.m_offset;
    m_size = other.m_size;

    return *this;
}

sf::Int64 sf::BundleStream::read(void* data, sf::Int64 size)
{
    m_file.read(static_cast<char*>(data), size);
    return m_file.gcount();
}

sf::Int64 sf::BundleStream::seek(sf::Int64 position)
{
    m_file.seekg(m_offset + position);
    return tell();
}

sf::Int64 sf::BundleStream::tell()
{
    return m_file.tellg() - m_offset;
}

sf::Int64 sf::BundleStream::getSize()
{
    return m_size;
}

sf::Int64 sf::BundleStream::getOffset()
{
    return m_offset;
}

void sf::BundleStream::open(const std::string& filename, size_t offset, size_t size)
{
    m_file.open(filename, std::ios::binary);
    if (!m_file.is_open())
        return;

    m_filename = filename;
    m_file.seekg(offset, std::ios::beg);
    m_offset = offset;
    m_size = size;
}

bool sf::BundleStream::isOpen() const
{
    return m_file.is_open();
}

void sf::BundleStream::close()
{
    m_file.close();
}
