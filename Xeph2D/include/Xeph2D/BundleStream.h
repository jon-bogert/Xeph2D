#ifndef __XEPH2D_EDITOR_BUNDLESTREAM_H__
#define __XEPH2D_EDITOR_BUNDLESTREAM_H__

#include "SFML/Audio.hpp"
#include <fstream>

namespace Xeph2D
{
	//NOTE: Formatted to fit the SFML-style for override
	class BundleStream : public sf::InputStream
	{
    public:
        BundleStream() = default;
        BundleStream(const std::string& filename, size_t offset, size_t size);

        sf::Int64 read(void* data, sf::Int64 size) override;
        sf::Int64 seek(sf::Int64 position) override;
        sf::Int64 tell() override;
        sf::Int64 getSize() override;
        sf::Int64 getOffset();

        void open(const std::string& filename, size_t offset, size_t size);
        bool isOpen() const;
        void close();

    private:
        std::ifstream m_file;
        std::streamoff m_offset;
        std::streamsize m_size;
    };
}

#endif //!__XEPH2D_EDITOR_BUNDLESTREAM_H__