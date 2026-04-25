// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/02-final-and-override-2.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/02-final-and-override-2.cpp
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
    // play() 是 final, 子类不能再次 override; 通过实现两个纯虚函数完成
    void init_audio_params() override {
        std::cout << "OGGPlayer: Initializing audio parameters..." << std::endl;
    }

    void play_audio() override {
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

    return 0;
}
