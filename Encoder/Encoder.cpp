#include "Encoder.hpp"
#include "mbed.h"

// コンストラクタの実装
Encoder::Encoder(PinName pinA, PinName pinB, float pulse) : _encoderA(pinA,PullUp), _encoderB(pinB,PullUp), _position(0), _last_position(0) {
    // 割り込みハンドラを設定
    _encoderA.rise(callback(this, &Encoder::_encode_isr));
    _encoderA.fall(callback(this, &Encoder::_encode_isr));
    _encoderB.rise(callback(this, &Encoder::_encode_isr));
    _encoderB.fall(callback(this, &Encoder::_encode_isr));

    // タイマーの開始
    _timer.start();
}

// 割り込みハンドラ
void Encoder::_encode_isr() {
    int currentA = _encoderA.read();
    int currentB = _encoderB.read();
    if(currentA ^ currentB == 0) {
        _position++;
    } else {
        _position--;
    }
}

// 角速度計算
float Encoder::get_angular_velocity() {
    return _calculate_angular_velocity();
}

// 内部で角速度を計算するメソッド
float Encoder::_calculate_angular_velocity() {
    int position_diff = _position - _last_position;
    float time_diff = _timer.elapsed_time().count() / 1000000.0f;  // 秒単位に変換

    // 角速度を計算 (仮にエンコーダーが1回転で2048*4回出力)
    float angular_velocity = position_diff * 2.0f * 3.14159f / (_pulse * 4.0f * time_diff);

    // 変数の更新
    _last_position = _position;
    _timer.reset();

    return angular_velocity;
}

int Encoder::_get_count() {
    return _position;
}
