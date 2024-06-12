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
