#include "Motor.hpp"
#include "Encoder.hpp"
#include "mbed.h"

// コンストラクタ
Motor::Motor(PinName pwm_pin, PinName dir_pin, EncoderController* encoder_controller, PinName encA, PinName encB, int pulse) 
    : pwm(pwm_pin), dir(dir_pin), encoder_controller(encoder_controller), _pulse(pulse),
      Kp(0.1), Ki(0.0), Kd(0.0), integral(0), previousError(0), targetVelocity(0.0f) {
    encoder = encoder_controller->addEncoder(encA, encB);
    controlTicker.attach(callback(this, &Motor::updateControl), 100ms); // 0.1秒ごとに制御を更新
}

// デストラクタ
Motor::~Motor() {
    controlTicker.detach();
}

// 目標角速度を設定
void Motor::setTargetVelocity(float target_velocity) {
    this->targetVelocity = target_velocity;
}

// モーター制御を更新（周期的に呼び出す）
void Motor::updateControl() {
    currentVelocity = encoder->get_angular_velocity(); // 現在の角速度を取得
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

MotorController::MotorController(int pulse) : _pulse(pulse), encoder_controller(pulse) {}

Motor* MotorController::addMovingWeel(PinName pwm_pin, PinName dir_pin, PinName encA, PinName encB) {
    Motor* motor = new Motor(pwm_pin, dir_pin, &encoder_controller, encA, encB, _pulse);
    MovingWeel.push_back(motor);
    return motor;
}

Encoder* MotorController::addMeasureWeel(PinName encA, PinName encB) {
    Encoder* encoder = encoder_controller.addEncoder(encA, encB);
    MeasureWeel.push_back(encoder);
    return encoder;
}

float EncoderController::get_angular_velocity(Encoder* encoder) {
    return encoder -> get_angular_velocity();
}
