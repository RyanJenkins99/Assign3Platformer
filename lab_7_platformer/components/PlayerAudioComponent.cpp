#include "PlayerAudioComponent.h"

bool PlayerAudioComponent::initSoundBuffer(const std::string& filePath)
{
    return m_soundBuffer.loadFromFile(filePath);
}

void PlayerAudioComponent::play(const std::string& filePath)
{
    if (!initSoundBuffer(filePath))
    {
        // Throw an exception if the file could not be loaded
        throw std::runtime_error("Failed to load sound effect: " + filePath);
    }

    // Set up the sound effects volume here
    m_sound.setBuffer(m_soundBuffer);

    m_sound.setVolume(100.0f);

    m_sound.play();
}

sf::SoundBuffer PlayerAudioComponent::m_soundBuffer;

sf::Sound PlayerAudioComponent::m_sound;