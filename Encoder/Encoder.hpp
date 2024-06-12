#pragma once

#include "mbed.h"
#include <vector>
using namespace std;

class Encoder {
public:
    // コンストラクタ
    Encoder(PinName pinA, PinName pinB, int pulse);

    // 角速度を計算するメソッド
    float get_angular_velocity();
    int _get_count();

private:
    // 割り込みピン
    InterruptIn _encoderA;
    InterruptIn _encoderB;
    int _pulse;

    // エンコーダーの位置と計時用タイマー
    volatile int _position;
    volatile int _last_position;
    Timer _timer;

    // 割り込みで呼び出されるハンドラ
    void _encode_isr();

    // 角速度を計算するための内部メソッド
    float _calculate_angular_velocity();
};

class EncoderController {
public:
    //コンストラクタ
    EncoderController(int pulse);

    Encoder* addEncoder(PinName pinA, PinName pinB);

    //すべてのモーターの角速度を入手するメソッド
    vector<float> get_all_angular_velocity();

    vector<float> get_inrange_angular_velocity(int start, int end);

    float get_angular_velocity(Encoder* encoder);

private:
    int _pulse;
    vector<Encoder*> encoders;
};
