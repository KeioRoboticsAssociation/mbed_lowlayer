#pragma once
#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "mbed.h"
#include "Encoder.hpp"

class MotorController {
public:
    // コンストラクタ
    MotorController(PinName pwm_pin, PinName dir_pin, PinName encA, PinName encB, float pulse);

    // デストラクタ
    ~MotorController();

    // 目標角速度を設定
    void setTargetVelocity(float target_velocity);

    // モーター制御を更新（周期的に呼び出す）
    void updateControl();

private:
    PwmOut pwm;           // PWM出力
    DigitalOut dir;       // モーター回転方向
    Encoder encoder;      // エンコーダー
    float Kp; // 比例ゲイン
    float Ki; // 積分ゲイン
    float Kd; // 微分ゲイン
    float integral;        // 積分項
    float previousError;   // 前回の誤差
    Ticker controlTicker; // 制御周期タイマー

    float targetVelocity; // 目標角速度
    float currentVelocity; // 現在の角速度
    float error;           // 目標との誤差
    float derivative;      // 微分項

    // モーターのPWM値を計算して出力
    void computePWM();
};

#endif
