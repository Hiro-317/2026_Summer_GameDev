# 1回のAddActorから複数のエフェクトを再生する

`EffectPlayer` は `EffectBase` を継承する管理アクターです。
`AddActor` では準備だけを行い、`PlayExplosion` / `PlayFlameBreath` を呼んだときに表示します。
同じ関数を何度呼んでも前の再生をリセットせず、複数の場所へ同時に表示できます。

## 最初に準備する

シーンのメンバーなどに `EffectPlayer* effects` を持たせ、`SubPostLoad` などで一度だけ追加します。

```cpp
#include "../../Object/Common/Effect/EffectPlayer.h"

// このシーンで使う爆発だけを事前ロードする。ブレスのシェーダーはまだ読み込まない。
effects = new EffectPlayer({EffectPlayer::Type::Explosion});
AddActor(effects);
```

両方を事前ロードする場合は `{EffectPlayer::Type::Explosion, EffectPlayer::Type::FlameBreath}`、
事前ロードせず実際に使う種類だけを読み込む場合は `new EffectPlayer()` を指定します。
未指定の種類を初めてPlayした場合は、そのときに必要なシェーダーだけを遅延ロードします。

## 欲しいタイミングで再生する

```cpp
// 1つのeffectsから、異なる場所・色へ同時に爆発を出す。
auto first = effects->PlayExplosion(Vector3(0.0f, 150.0f, 0.0f));
auto second = effects->PlayExplosion(
    Vector3(400.0f, 150.0f, 0.0f), 200.0f, 1.5f, 2.0f, 2.0f, 0xff8020);

FlameBreathSettings settings;
settings.distance = 800.0f;
settings.radius = 100.0f;
settings.color = 0x00aaff;
settings.durationSeconds = 0.0f; // この再生だけ、止めるまで無期限。
auto breath = effects->PlayFlameBreath(
    Vector3(0.0f, 200.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), settings);
```

Playは、今回の再生だけを識別するIDを返します。IDは再利用しません。
シーンのInit前にPlayを呼んだ場合は予約され、Initで準備が完了してから再生が進みます。

## 個別変更・停止

```cpp
effects->SetPosition(breath, mouthPosition);
effects->SetDirection(breath, targetPosition - mouthPosition);
effects->SetColor(breath, 0xff4020);
effects->SetBreathStrength(breath, 1000.0f, 120.0f, 3.0f);

effects->Stop(breath); // このIDだけを即座に消す。寿命0の再生も同じ。
effects->StopAll();   // 現在の再生と開始待ちの予約をすべて消す。次のPlayも使用可能。
bool playing = effects->IsPlaying(first);
auto count = effects->GetPlayingCount(); // 初期化前の予約も含む。
```

終了済み・無効なIDへのStopや変更はfalseを返します。
方向・強さの変更を爆発のIDへ指定した場合もfalseです。
`Stop()` を引数なしで呼ぶと、全再生を回収して管理アクター自体の更新・描画も止めます。
その後もPlayを呼べば管理アクターは再開します。`Release`後の再生はできません。

## メモリと所有権

- 未使用の種類はロードせず、指定種類のVS/PSだけを `ShaderResourceManager` から共有します。
- AddActorだけでは、内部の再生アクターや個別定数バッファを事前生成しません。
- 再生中・開始待ちの分だけ内部アクターを確保し、定数バッファは再生を初期化するときに確保します。
- 有限寿命が終了した再生は更新時に、Stopした再生はその場で、アクターと定数バッファを回収します。
- 無期限再生はStopされるまで保持します。大量に追加し続ける場合は必要なIDを停止してください。
- 共有VS/PSの解放時期は `ShaderResourceManager` に従います。他のアクターが使う共有ハンドルを勝手に解放しません。
- 内部再生は管理アクターが更新・描画・解放するので、個別にAddActorしたりdeleteしたりする必要はありません。
- `effects` 自体はシーンが所有します。シーン解放時にすべての再生も回収します。
- 描画専用です。当たり判定・ダメージ・通信同期は攻撃側で扱ってください。

Windows / Visual Studio / DxLibでの実機表示は別途確認してください。
