# mbed用低レイヤーライブラリ
PlatformIO上のmbedで動作するライブラリです。
PID制御に困った新人の救済措置になれば幸いです。

ライブラリのやり方わからないんでファイルそのまま上げます。
変数や関数のネーミングセンスないのは許して。


使用方法
main.cppの最初のほうに

```bash
#include "MotorController.hpp"
#include "Encoder.hpp"
```

の文章を追加すればOK
基本的にオドメトリ(自己位置推定)用のエンコーダーを使う理由がないならEncoderのほうは書かなくてもいい。

# How to use
MotorController編
```bash
MotorController name(PinName pwm_pin, PinName dir_pin, PinName encA, PinName encB, float pulse);
```
  引数のそれぞれを(モーターの出力pwm, モーターのdir, エンコーダーA, エンコーダーB, パルス)のピンを書けばいい。nameには好きな名前
  includeの下あたりに書けばモーターとエンコーダーを紐づけてくれる。
  分解能はパルスの4倍となるので注意。もし、AB相でないならばライブラリを書き換えて。

```bash
name.setTargetVelocity(float target_velocity);
```
  float型で値を入れるとnameのモーターの角速度がtarget_velocityになるようにPID制御してくれる。


Encoder編

```bash
Encoder name(PinName pinA, PinName pinB, PinName dir_pin, float pulse);
```
  引数のそれぞれを(エンコーダーA, エンコーダーB, パルス)のピンを書けばいい。nameには好きな名前
  分解能はパルスの4倍となるので注意。もし、AB相でないならばライブラリを書き換えて。

```bash
name.get_angular_velocity();
```
  nameのエンコーダーの角速度を返してくれる関数。

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
