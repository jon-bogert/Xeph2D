#ifndef __XEPH2D_AUDIODATA_H__
#define __XEPH2D_AUDIODATA_H__

#include "BundleStream.h"
#include <variant>

namespace Xeph2D
{
	class AudioData
	{
	public:
		AudioData() = default;
		~AudioData() = default;
		AudioData(sf::SoundBuffer buffer);
		AudioData(sf::BundleStream stream);

		AudioData(const AudioData& other);
		AudioData operator=(AudioData& other);

		sf::SoundBuffer* GetAsBuffer();
		sf::BundleStream* GetAsStream();

		void Load(sf::SoundBuffer buffer);
		void Load(sf::BundleStream stream);

		bool IsStreamed() const { return m_isStreamed; }

	private:
		bool m_isStreamed = false;

		using Data = std::variant<sf::SoundBuffer, sf::BundleStream>;
		Data m_data;
	};
}

#endif //!__XEPH2D_AUDIODATA_H__
