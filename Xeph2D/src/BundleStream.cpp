#include "Xeph2D/BundleStream.h"

Xeph2D::BundleStream::BundleStream(const std::string& filename, size_t offset, size_t size)
{
    open(filename, offset, size);
}

sf::Int64 Xeph2D::BundleStream::read(void* data, sf::Int64 size)
{
    m_file.read(static_cast<char*>(data), size);
    return m_file.gcount();
}

sf::Int64 Xeph2D::BundleStream::seek(sf::Int64 position)
{
    m_file.seekg(m_offset + position);
    return tell();
}

sf::Int64 Xeph2D::BundleStream::tell()
{
    return m_file.tellg() - m_offset;
}

sf::Int64 Xeph2D::BundleStream::getSize()
{
    return m_size;
}

sf::Int64 Xeph2D::BundleStream::getOffset()
{
    return m_offset;
}

void Xeph2D::BundleStream::open(const std::string& filename, size_t offset, size_t size)
{
    m_file.open(filename, std::ios::binary);
    if (!m_file.is_open())
        return;

    m_file.seekg(offset, std::ios::beg);
    m_offset = offset;
    m_size = size;
}

bool Xeph2D::BundleStream::isOpen() const
{
    return m_file.is_open();
}

void Xeph2D::BundleStream::close()
{
    m_file.close();
}
