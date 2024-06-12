#pragma once

#include "mbed.h"
#include "Encoder.hpp"
#include <vector>

class Motor {
public:
    // コンストラクタ
    Motor(PinName pwm_pin, PinName dir_pin, EncoderController* encoder_controller, PinName encA, PinName encB, int pulse);

    // デストラクタ
    ~Motor();

    // 目標角速度を設定
    void setTargetVelocity(float target_velocity);

    // モーター制御を更新（周期的に呼び出す）
    void updateControl();

private:
    PwmOut pwm;
    DigitalOut dir;
    Encoder* encoder;
    EncoderController* encoder_controller;
    Ticker controlTicker;
    int _pulse;
    float Kp, Ki, Kd;
    float integral, previousError, targetVelocity;
    float currentVelocity, error, derivative;
};

class MotorController {
public:
    MotorController(int pulse);
    Motor* addMovingWeel(PinName pwm_pin, PinName dir_pin, PinName encA, PinName encB);
    Encoder* addMeasureWeel(PinName encA, PinName encB);
    float get_angular_velocity(Encoder* encoder);

private:
    int _pulse;
    std::vector<Motor*> MovingWeel;
    std::vector<Encoder*> MeasureWeel;
    EncoderController encoder_controller;
};
