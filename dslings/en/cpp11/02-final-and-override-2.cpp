// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/02-final-and-override-2.cpp
//
// Exercise: cpp11 | 02 - final and override
//
// Tips: Implement the OGGPlayer class, correct the usage errors of final and override in the code
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/final
//   - https://en.cppreference.com/w/cpp/language/override
//
// Auto-Checker command:
//
//   d2x checker final-and-override-2
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

struct AudioPlayer { // Do not directly modify the AudioPlayer class
    virtual void play() final {
        init_audio_params();
        play_audio();
    }
private:
    virtual void init_audio_params() = 0;
    virtual void play_audio() = 0;
};

struct WAVPlayer : AudioPlayer {
    void init_audio_params() override {
        std::cout << "WAVPlayer: Initializing audio parameters..." << std::endl;
    }

    void play_audio() override {
        std::cout << "WAVPlayer: Playing WAV audio..." << std::endl;
    }
};

struct  MP3Player : AudioPlayer {
    void init_audio_params() override {
        std::cout << "MP3Player: Initializing audio parameters..." << std::endl;
    }

    void play_audio() override {
        std::cout << "MP3Player: Playing MP3 audio..." << std::endl;
    }
};

struct OGGPlayer : AudioPlayer {
    // Correctly implement OGGPlayer

    void play() override {
        // init_audio_params();
        std::cout << "OGGPlayer: Initializing audio parameters..." << std::endl;
        // play_audio();
        std::cout << "OGGPlayer: Playing OGG audio..." << std::endl;
    }

};


int main() { // Do not directly modify the code in the main function

    AudioPlayer *player1 = new WAVPlayer();
    AudioPlayer *player2 = new MP3Player();
    AudioPlayer *player3 = new OGGPlayer();

    player1->play();
    player2->play();
    player3->play();

    delete player1;
    delete player2;
    delete player3;

    D2X_WAIT

    return 0;
}