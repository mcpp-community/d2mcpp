// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/02-final-and-override-2.cpp
//
// Exercise/练习: cpp11 | 02 - final and override
//
// Tips/提示: 实现OGGPlayer类，修正代码中final和override的使用错误
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/final
//   - https://en.cppreference.com/w/cpp/language/override
//
// Auto-Checker/自动检测命令:
//
//   d2x checker final-and-override-2
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

struct AudioPlayer { // 不要直接修改AudioPlayer类
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
    // 正确实现OGGPlayer

    void play() override {
        // init_audio_params();
        std::cout << "OGGPlayer: Initializing audio parameters..." << std::endl;
        // play_audio();
        std::cout << "OGGPlayer: Playing OGG audio..." << std::endl;
    }

};


int main() { // 不要直接修改main函数中的代码

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