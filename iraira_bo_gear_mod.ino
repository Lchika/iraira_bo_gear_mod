/**
 * @file iraira_gear_mod.ino
 * @brief 歯車モジュールプログラム
 * @date 2019/07/13
 */

#include <Unistep2.h>
#include "debug.h"
#include "dsub_slave_communicator.hpp"

#define PIN_GOAL_SENSOR     10   //  通過/ゴールしたことを検知するセンサのピン
#define PIN_HIT_SENSOR      A1   //  当たったことを検知するセンサのピン
#define PIN_DIP_0           11   //  DIPスイッチbit0
#define PIN_DIP_1           12   //  DIPスイッチbit1
#define PIN_DIP_2           13   //  DIPスイッチbit2
#define PIN_DIP_3           A0   //  DIPスイッチbit3

/* 変数宣言 */
unsigned char slv_address;      //  スレーブアドレス(0はゴールモジュール固定、それ以外は1~)
DsubSlaveCommunicator *dsubSlaveCommunicator = NULL;
Unistep2 stepperX(2, 3, 4, 5, 4096, 4000);
Unistep2 stepperY(6, 7, 8, 9, 4096, 4000);

/**
 * @brief セットアップ関数
 * @param None
 * @return None
 */
void setup(void) {
  /* ここから各スレーブ共通コード */
  BeginDebugPrint();                //  デバッグ用
  /* ディップスイッチを入力として設定 */
  pinMode(PIN_DIP_0, INPUT);
  pinMode(PIN_DIP_1, INPUT);
  pinMode(PIN_DIP_2, INPUT);
  pinMode(PIN_DIP_3, INPUT);

  slv_address = ReadDipSwitch();    //  SLVアドレスを設定
  /* D-sub通信管理用インスタンスの生成 */
  dsubSlaveCommunicator = new DsubSlaveCommunicator(PIN_GOAL_SENSOR,
                              PIN_HIT_SENSOR, slv_address, true, true);
  /* ここまで各スレーブ共通コード */

  /* ここから各モジュール独自コード */
  /* ここまで各モジュール独自コード */
}

/**
 * @brief ループ関数
 * @param None
 * @return None
 */
void loop(void) {
  /* ここから各スレーブ共通コード */
  static bool pre_active = false;
//  デバッグ設定が有効の場合、シリアル入力からnow_stateを変更できるようにする
#ifdef DEBUG_NOW_STATE
  static bool now_active = false;
  if (Serial.available() > 0){
    Serial.read();
    now_active = !now_active;
  }
#else
  bool now_active = DsubSlaveCommunicator::is_active();
#endif

  //  プレイヤーがこのモジュール上で遊んでいるとき
  if(now_active){
    //  D-sub関係イベント処理
    dsubSlaveCommunicator->handle_dsub_event();

    // We need to call run() frequently during loop()
    stepperX.run();
    stepperY.run();
    if ( stepperX.stepsToGo() == 0 ){ // If stepsToGo returns 0 the stepper is not moving
        stepperX.move(-4000);
    }
    if ( stepperY.stepsToGo() == 0 ){
        stepperY.move(4000);
    }
  //  プレイヤーがこのモジュール上で遊んでいないとき
  }else{
    //  前回活性状態だった場合
    //  (このモジュールを終了したタイミング)
    if(pre_active){
      DebugPrint("stop stepper");
      stepperX.stop();
      stepperY.stop();
    }
  }
  pre_active = now_active;
}

/**
 * @brief DIPスイッチ読み取り関数
 * @param None
 * @return unsigned char  DIPスイッチの値
 */
unsigned char ReadDipSwitch(void) {
  unsigned char value = 0;

  if(digitalRead(PIN_DIP_0) == HIGH) {
    value += 1;
  }
  if(digitalRead(PIN_DIP_1) == HIGH) {
    value += 2;
  }
  if(digitalRead(PIN_DIP_2) == HIGH) {
    value += 4;
  }
  if(digitalRead(PIN_DIP_3) == HIGH) {
    value += 8;
  }

  return value;
}
