#include "Encoder.hpp"
#include "mbed.h"
#include <vector>
using namespace std::chrono;

// コンストラクタの実装
Encoder::Encoder(PinName pinA, PinName pinB, int pulse) 
    : _encoderA(pinA), _encoderB(pinB), _pulse(pulse),_position(0), _last_position(0) {
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
    if(currentA ^ currentB) {
        _position--;
    } else {
        _position++;
    }
}


// 角速度計算
float Encoder::get_angular_velocity() {
    return _calculate_angular_velocity();
    //return 0.0f;
}


// 内部で角速度を計算するメソッド
float Encoder::_calculate_angular_velocity() {
    int position_diff = _position - _last_position;
    float time_diff = duration<float>{_timer.elapsed_time()}.count();  // 秒単位
    time_diff = round(time_diff * 1000.0f) / 1000.0f;

    if(time_diff < 0.0f) time_diff = 0.001f; //0で割るのを回避
    // 角速度を計算 (仮にエンコーダーが1回転で2048*4回出力)
    int test = (position_diff * 2 * 3141) / (int)(_pulse * 4);
    float angular_velocity = (float)test / (time_diff * 1000.0f);
    

    // 変数の更新
    _last_position = _position;
    _timer.reset();

    return angular_velocity;
}

int Encoder::_get_count() {
    return _position;
}

EncoderController::EncoderController(int pulse) : _pulse(pulse) {}

Encoder* EncoderController::addEncoder(PinName pinA, PinName pinB) {
    Encoder* encoder = new Encoder(pinA, pinB, _pulse);
    encoders.push_back(encoder);
    return encoder;
}

vector<float> EncoderController::get_all_angular_velocity() {
    const int encoder_num = encoders.size();
    vector<float> velocity(encoder_num);
    for (int i = 0; i < encoder_num; i++) {
        velocity[i] = encoders[i]->get_angular_velocity();
    }
    return velocity;
}

vector<float> EncoderController::get_inrange_angular_velocity(int start, int end) {
    //const int encoder_num = encoders.size();
    //if (end < start || encoder_num < end || start < 0) return vector<float> null;
    vector<float> velocity((end-start)+1);
    for (int i = start; i <= end; i++) {
        velocity[i] = encoders[i]->get_angular_velocity();
    }
    return velocity;
}

float EncoderController::get_angular_velocity(Encoder* encoder) {
    return encoder -> get_angular_velocity();
}
