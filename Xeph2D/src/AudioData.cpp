#include "Xeph2D/AudioData.h"

Xeph2D::AudioData::AudioData(sf::SoundBuffer buffer)
{
    Load(buffer);
}

Xeph2D::AudioData::AudioData(sf::BundleStream stream)
{
    Load(stream);
}

Xeph2D::AudioData::AudioData(const AudioData& other)
{
    m_isStreamed = other.m_isStreamed;
    if (m_isStreamed)
    {
        m_data = std::get<sf::BundleStream>(other.m_data);
    }
    else
    {
        m_data = std::get<sf::SoundBuffer>(other.m_data);
    }
    sf::SoundBuffer buff;
}

Xeph2D::AudioData Xeph2D::AudioData::operator=(AudioData& other)
{
    m_isStreamed = other.m_isStreamed;
    if (m_isStreamed)
    {
        std::get<sf::BundleStream>(m_data) = std::get<sf::BundleStream>(other.m_data);
    }
    else
    {
        std::get<sf::SoundBuffer>(m_data) = std::get<sf::SoundBuffer>(other.m_data);
    }

    return *this;
}

sf::SoundBuffer* Xeph2D::AudioData::GetAsBuffer()
{
    if (m_data.valueless_by_exception() || m_isStreamed)
        return nullptr;

    return &std::get<sf::SoundBuffer>(m_data);
}

sf::BundleStream* Xeph2D::AudioData::GetAsStream()
{
    if (m_data.valueless_by_exception() || !m_isStreamed)
        return nullptr;

    return &std::get<sf::BundleStream>(m_data);
}

void Xeph2D::AudioData::Load(sf::SoundBuffer buffer)
{
    m_isStreamed = false;
    std::get<sf::SoundBuffer>(m_data) = buffer;
}

void Xeph2D::AudioData::Load(sf::BundleStream stream)
{
    m_isStreamed = true;
    std::get<sf::BundleStream>(m_data) = stream;
}
