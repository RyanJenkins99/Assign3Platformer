//
// Created by ryanj on 24/12/2022.
//

#ifndef GAMES_ENGINEERING_AUDIO_H
#define GAMES_ENGINEERING_AUDIO_H


#include <iostream>
#include "SFML/Audio/Music.hpp"

class audio {
public:
    static void playAudio(std::string audioFile) {


        if (!music.openFromFile(audioFile)){
            std::cout << "Music didn't load ..." << std::endl;
        }
        else{
            music.setLoop(true);

            music.setVolume(20);

            music.play();
        }
    }
private:
    static sf::Music music;
};
sf::Music audio::music;



#endif //GAMES_ENGINEERING_AUDIO_H
