#include "MotorController.hpp"
#include "mbed.h"

// コンストラクタ
MotorController::MotorController(PinName pwm_pin, PinName dir_pin, PinName encA, PinName encB, float pulse) 
    : pwm(pwm_pin), dir(dir_pin), encoder(encA, encB, pulse),
      Kp(0.1), Ki(0.01), Kd(0.01), integral(0), previousError(0) {
    controlTicker.attach(callback(this, &MotorController::updateControl), 100ms); // 0.1秒ごとに制御を更新
}

// デストラクタ
MotorController::~MotorController() {
    controlTicker.detach();
}

// 目標角速度を設定
void MotorController::setTargetVelocity(float target_velocity) {
    this->targetVelocity = target_velocity;
}

// モーター制御を更新（周期的に呼び出す）
void MotorController::updateControl() {
    currentVelocity = encoder.get_angular_velocity(); // 現在の角速度を取得
    error = targetVelocity - currentVelocity;          // 誤差を計算
    integral += error * 0.1;                          // 積分項を更新 (積分時間 = 制御周期)
    derivative = (error - previousError) / 0.1;       // 微分項を更新 (微分時間 = 制御周期)

    // PID制御値を計算
    float control = Kp * error + Ki * integral + Kd * derivative;

    // モーターの速度制御（PWM値と方向の設定）
    if (control > 0) {
        dir = 1;  // 正の方向
    } else {
        dir = 0;  // 負の方向
        control = -control; // PWM値は正でなければならない
    }

    // PWM信号を設定 (0.0から1.0の範囲)
    pwm = (control > 1.0 ? 1.0 : control);

    // 前回の誤差を更新
    previousError = error;
}

// モーターのPWM値を計算して出力
void MotorController::computePWM() {
    // このメソッドはupdateControlで直接呼び出されるので、特に内容は不要
}
