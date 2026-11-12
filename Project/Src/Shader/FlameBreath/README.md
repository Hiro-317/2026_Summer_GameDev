# 直線状の炎ブレス

`FlameBreathActor` は `EffectBase` を継承する描画用のエフェクトです。
開始点・向き・最大到達距離・太さ・明るさ・色・寿命を指定できます。
根元は細く、先へ行くほど太くなり、白く明るい芯と揺れる炎を描画します。

## 任意のタイミングで生成

```cpp
// GameSceneなど、シーンから呼ぶ場合の例。
#include "../../Object/Common/Effect/FlameBreathActor.h"

FlameBreathSettings settings;
settings.distance = 800.0f;      // 到達距離
settings.radius = 100.0f;        // 最大半径（最大の太さはおよそ200）
settings.durationSeconds = 2.0f; // 寿命（秒）
settings.intensity = 2.5f;       // 発光の強さ
settings.flowSpeed = 3.0f;       // 模様が流れる速さ
settings.color = 0xff8020;      // 0xRRGGBB形式
settings.seed = 1.0f;           // 炎の模様

auto* breath = new FlameBreathActor(
    Vector3(0.0f, 150.0f, 0.0f), // 開始点（ワールド座標）
    Vector3(0.0f, 0.0f, 1.0f),   // 向き（この例では+Z方向）
    settings);
AddActor(breath);
```

シーンの `SubPostLoad` や `SubPostUpdate` などから追加できます。
向きのベクトルは内部で正規化するため、長さにかかわらず距離は `distance` で決まります。
ゼロ方向、正でない距離・半径、負の寿命・強度・速度、有限でない数値は例外になります。
寿命の `0.0f` は無期限を意味します。

## 無期限再生と即時停止

```cpp
FlameBreathSettings settings;
settings.durationSeconds = 0.0f; // 無期限。勝手にフェードアウトしない。
auto* breath = new FlameBreathActor(startPoint, direction, settings);
AddActor(breath);

// 終わらせたいタイミングで呼ぶ。有限寿命の途中でも同じように即座に消せる。
breath->Stop();
```

`Stop()` は残り時間やフェードを待たず、その呼び出し後の更新・描画を止めます。
オブジェクトのdeleteはせず、所有するシーン・親アクターの解放時に破棄します。
再開するときは `Restart()`、無期限かどうかの確認は `IsInfinite()` を使います。
無期限のブレスは0.24秒で設定距離へ伸び、以後は炎の模様を流し続けます。

## 再生中の変更と再利用

```cpp
// ボスの口元などに追従させる場合は、所有者の更新処理から呼ぶ。
breath->SetStartPoint(mouthPosition);
breath->SetDirection(targetPosition - mouthPosition);
breath->SetStrength(1000.0f, 120.0f, 3.0f); // 距離・半径・明るさ
breath->SetColor(0x4090ff);               // 青い炎
breath->SetFlowSpeed(4.0f);

breath->Stop();    // 更新・描画を止める
breath->Restart(); // 設定を保ったまま、経過時間を0に戻して再生
bool finished = breath->IsFinished();
```

終了したアクターは自動削除せず、シーンまたは親アクターの解放時に破棄します。
頻繁に発生させる場合は、事前生成したものを `Restart()` で再利用できます。
シーンが所有するため、呼び出し側で `delete` しないでください。
子アクターとして使う場合は、親の読み込み時に `AddChildActor` で登録します。

## 描画・ビルド

- HLSLは `Project/Src/Shader/FlameBreath/` に配置しています。
- `FlameBreathShader` は `ShaderBase` を継承し、`ShaderResourceManager` の共有VS/PSを使用します。
- 色・時間などを持つ定数バッファはアクターごとに作成し、PSのb4へ32バイトで渡します。
- C++とVS/PSは `Project.vcxproj` に登録済みです。HLSLは `main` / Shader Model 5.0でコンパイルします。
- 出力は `Project/Data/Shader/FlameBreathVS.cso` / `FlameBreathPS.cso` です。
- C++はUTF-8 BOM付き、HLSLはUTF-8 BOMなしで保存してください。
- 形状は軸固定ビルボードです。正面・真後ろから薄くなる場合は先端の補助炎を表示します。
- 加算合成と深度判定を使い、描画中は深度を書き込みません。壁との交差を滑らかにするソフトパーティクル処理は含みません。
- 当たり判定やダメージ、発生タイミングは持ちません。攻撃ロジックから必要なタイミングで生成してください。
- 時間は既存の `EffectBase` と同じく、60FPSと `TimeScale` に従います。

Windows / Visual Studio / DxLibでのビルド・表示確認は別途必要です。
