# iraira_bo_gear_mod

## 概要

- スレーブモジュールの共通部分だけ書いたコード
- …を修正した、歯車モジュール専用のプログラム  

## ピン関係

|ピン番号|役割|
|-|-|
|10|モジュール通過判定|
|A1|コース接触判定|
|11|DIPスイッチbit0|
|12|DIPスイッチbit1|
|13|DIPスイッチbit2|
|A0|DIPスイッチbit3|
|SDA|D-sub2番ピン(SDA)|
|SCL|D-sub3番ピン(SCL)|
|GND|D-sub1番ピン|
|2,3,4,5|ステッピングモータX制御|
|6,7,8,9|ステッピングモータY制御|

## ギミック
- モジュール侵入中
  - ステッピングモータを一定方向に稼働する
- モジュール終端到達時
  - ステッピングモータを停止する

## モジュール共通部分の処理について

- D-sub関係の処理は`DsubSlaveCommunicator`クラス内の処理で完結する
- `setup()`内で`DsubSlaveCommunicator`クラスのインスタンス`*dsubSlaveCommunicator`を生成する
- DsubSlaveCommunicatorクラスは、マスタから通信開始通知を受け取ると、アクティブ状態になる
- アクティブ状態でなければ、モジュールは何もする必要がない
  - `loop()`内の以下のコードでアクティブ状態かどうか確認している
    ```c++
    bool now_active = DsubSlaveCommunicator::is_active()
    if(now_active){
      ...
    ```
- D-sub関係処理は以下の部分で処理している
  ```c++
  dsubSlaveCommunicator->handle_dsub_event();
  ```
  - コース接触判定、モジュール通過判定もここで行っている
  - この関数は定期的に呼ぶ必要がある

## 本プログラムのマニュアルについて
- 本プログラムは[Doxygen](http://www.doxygen.nl/index.html)を使ってドキュメント化しています
- マニュアルを見るにはプログラムダウンロード後、html/index.htmlをブラウザで開いてください
