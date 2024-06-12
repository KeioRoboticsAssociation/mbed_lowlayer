# mbed用低レイヤーライブラリ
PlatformIO上のmbedで動作するライブラリです。
PID制御に困った新人の救済措置になれば幸いです。

ライブラリのやり方わからないんでファイルそのまま上げます。
変数や関数のネーミングセンスないのは許して。


使用方法
main.cppの最初のほうに

```bash
#include "Motor.hpp"
#include "Encoder.hpp"
```

の文章を追加すればOK

# How to use
MotorController編
```bash
MotorController name(int pulse);
```
  引数のパルスを書けばいい。nameには好きな名前。
  分解能はパルスの4倍となるので注意。もし、AB相でないならばライブラリを書き換えて。

```bash
addMovingWeel(PinName pwm_pin, PinName dir_pin, PinName encA, PinName encB);
```
  引数にそれぞれのピンを書くことでモーターとそれに付随するエンコーダーを登録することができる。戻り値としてMotor型が返ってくる。
  駆動輪でつかうこと。

```bash
addMeasureWeel(PinName encA, PinName encB);
```
  引数に測定輪のエンコーダーピンを登録することができる。戻り値としてEncoder型が返ってくる。
  
```bash
Motor* -> setTargetVelocity(float target_velocity);
```
  float型で値を入れると指定したモーターの角速度がtarget_velocityになるようにPID制御してくれる。

```bash
Encoder* -> get_angular_velocity();
```
  Encoderの角速度を返してくれる。しかし、前回の速度出力からの平均速度なので注意。戻り値はfloat。

使用例
'''bash
MotorController motorcontroller(2048);
Motor* movingweel = motorcontroller.addMovingWeel(PA_10,PB_7,PA_12,PC_1);
Encoder* measureweel = motorcontroller.addMeasureWeel(PB_1,PC_0);
movingweel -> setTargetVelocity(3.0f);
float speed = measureweel -> get_angular_velocity();
'''

環境構築方法は横のurlより
https://kra.kibe.la/notes/5555

# 事前準備
srcと同じ層にあるplatformio.iniの中に以下の文章を追加する。

```bash
lib_extra_dirs=lib
```

よくわかんない人はとりあえずplatformio.iniの一番最後の行に入れておけばOK

次にEncoderとMotorControllerのフォルダをlib内部に入れる。
ダウンロードしてドラックアンドドロップでいいよ。

どんどんアップデートしていくよー。
動かなかったらごめん。連絡してね。
