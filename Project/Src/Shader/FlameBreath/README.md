# 直線状の炎ブレス

固有エフェクトは個別のアクターとして必要な分だけ保持します。共通ヒットエフェクトなどの管理は、その機能を追加するときに別途設計します。

`FlameBreathActor` は `EffectBase` を継承する描画用のエフェクトです。
開始点・向き・最大到達距離・太さ・明るさ・色・寿命を指定できます。
根元から終端円へ広がる立体の円錐を描画します。側面と終端面を持ち、見る角度で一枚の板に潰れません。

## 最初に準備して、必要なときだけ再生する

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
settings.stopSeconds = 0.24f;   // Stop後に根元から先端へ消える時間

auto* breath = new FlameBreathActor(
    Vector3(0.0f, 150.0f, 0.0f), // 開始点（ワールド座標）
    Vector3(0.0f, 0.0f, 1.0f),   // 向き（この例では+Z方向）
    settings);
AddActor(breath); // 準備だけ。ここではまだ表示しない。

// 攻撃開始など、表示したいタイミングで呼ぶ。
breath->Restart();
```

シーンの `SubPostLoad` や `SubPostUpdate` などから追加できます。
向きのベクトルは内部で正規化するため、長さにかかわらず距離は `distance` で決まります。
ゼロ方向、正でない距離・半径、負の寿命・強度・速度、有限でない数値は例外になります。
寿命の `0.0f` は無期限を意味します。

初期化が済んだ後は、同じインスタンスに `Restart()` を何度でも呼んで使い回せます。
再生中に呼ぶとその再生を最初からやり直します。同時に独立した再生が必要な分は、個別のアクターを用意します。

## 無期限再生と根元からの停止

```cpp
FlameBreathSettings settings;
settings.durationSeconds = 0.0f; // 無期限。勝手にフェードアウトしない。
auto* breath = new FlameBreathActor(startPoint, direction, settings);
AddActor(breath); // 準備だけ。ここではまだ表示しない。

// 攻撃開始など、表示したいタイミングで呼ぶ。
breath->Restart();

// 炎の供給を止め、根元から先端へ順に消す。完全停止まで更新を続ける。
breath->Stop();
```

`Stop()` は先端の位置を保ったまま、炎の後端を根元から先端へ進めます。
`settings.stopSeconds`（標準0.24秒）後に完全停止します。0を指定すると即時停止します。
消えている間は `IsFinished()` がfalseです。完全停止まで、シーンや親からUpdate/Drawを呼び続けてください。
毎フレームStopを呼んでも停止演出は最初に戻りません。ポーズ中は進行も止まります。
その場で全て消す場合は `StopImmediately()` を呼びます。初期化前のStopも即時キャンセルします。
有限寿命の自動終了はこれまでどおり寿命に達した時点で止まります。
オブジェクトのdeleteはせず、所有するシーン・親アクターの解放時に破棄します。
新しく根元から再生するときは `Restart()`、無期限かどうかの確認は `IsInfinite()` を使います。
無期限のブレスはRestartのたびに0.24秒で設定距離へ伸び、以後は炎の模様を流し続けます。
停止演出中にRestartした場合も、経過時間・伸び・停止状態を初期化します。
Playの共通処理は変更していません。新しい発生として伸び直させる呼び出しはRestartを使ってください。

## 再生中の変更と再利用

```cpp
// ボスの口元などに追従させる場合は、所有者の更新処理から呼ぶ。
breath->SetStartPoint(mouthPosition);
breath->SetDirection(targetPosition - mouthPosition);
breath->SetStrength(1000.0f, 120.0f, 3.0f); // 距離・半径・明るさ
breath->SetColor(0x4090ff);               // 青い炎
breath->SetFlowSpeed(4.0f);

breath->Stop();    // 根元から先端へ消す
breath->StopImmediately(); // 必要なら演出を待たず即時に消す
breath->Restart(); // 設定を保ったまま、経過時間を0に戻して再生
bool finished = breath->IsFinished();
```

終了したアクターは自動削除せず、シーンまたは親アクターの解放時に破棄します。
頻繁に発生させる場合は、事前生成したものを `Restart()` で再利用できます。
シーンが所有するため、呼び出し側で `delete` しないでください。
子アクターとして使う場合は、親の読み込み時に `AddChildActor` で登録します。

## 予告円の円周に合わせる・終端面を傾ける

```cpp
// 開始点は口元。circleCenterは予告円のワールド中心、circleRadiusはその半径。
breath->SetStartPoint(mouthPosition);
breath->SetEndCircle(circleCenter, circleRadius); // 標準は水平な終端円。
breath->Restart();

// 再生中に口元が動いても、終端円は指定した位置に残る。
breath->SetStartPoint(mouthPosition);

// 終端面の向きを変える場合。上向きなら水平な床と同じ向き。
breath->SetEndCircle(circleCenter, circleRadius, Vector3(0.0f, 1.0f, 0.0f));
// 例: 水平面から30度傾けた終端面（長さは内部で正規化する）。
breath->SetEndCircle(circleCenter, circleRadius, Vector3(0.0f, 0.8660254f, 0.5f));
```

`SetEndCircle`の第3引数は角度の度数ではなく、終端面に垂直な方向ベクトルです。
ブレスの軸は口元から終端円の中心へ向きます。終端面は軸と独立して傾けられるため、斜めのブレスでも水平な床の円周へ合わせられます。
口元が終端面上にある指定は、円錐が潰れるため例外になります。

既存の方向・距離指定も使えます。その場合は軸に垂直な終端円を作り、`settings.radius`が展開後の円の半径になります。
`SetDirection`または`SetStrength`を呼ぶと終端円の位置固定を解除するため、固定したい場合は最後に`SetEndCircle`を呼んでください。
発生直後は設定した時間で先端まで伸び、展開完了後に終端円へ一致します。
先端付近を途中で消す処理は廃止し、円周まで炎を残しています。

## 描画・ビルド

- HLSLは `Project/Src/Shader/FlameBreath/` に配置しています。
- `FlameBreathShader` は `ShaderBase` を継承し、`ShaderResourceManager` の共有VS/PSを使用します。
- 色・時間などを持つ定数バッファはアクターごとに作成し、PSのb4へ32バイトで渡します。
- C++とVS/PSは `Project.vcxproj` に登録済みです。HLSLは `main` / Shader Model 5.0でコンパイルします。
- 出力は `Project/Data/Shader/FlameBreathVS.cso` / `FlameBreathPS.cso` です。
- C++はUTF-8 BOM付き、HLSLはUTF-8 BOMなしで保存してください。
- 円錐は周方向32分割・長さ方向8分割の薄い側面と、円形の終端面を持ちます。カメラに合わせて形状を回しません。
- 内部に3枚の半透明の炎を重ね、初期のブレスと同じ流れるノイズ、白い芯、ぼかした横端を描きます。外側の円錐は薄い発光に抑えています。
- 根元からの停止では円錐の先端を固定し、後端を先へ進めて残った炎だけを描画します。
- 半径はワールド単位です。展開後の終端円周の頂点が、指定した半径と終端面上に配置されます。
- VSは通常のワールド→ビュー→射影変換を行い、PSは側面と終端円へ炎の模様を付けます。
- `FlameBreathActor.cpp/.h`と`FlameBreathVS/PS.hlsl`を一緒に更新してリビルドしてください。
- 加算合成と深度判定を使い、描画中は深度を書き込みません。壁との交差を滑らかにするソフトパーティクル処理は含みません。
- 当たり判定やダメージ、発生タイミングは持ちません。攻撃ロジックから必要なタイミングで生成してください。
- 時間は既存の `EffectBase` と同じく、60FPSと `TimeScale` に従います。

Windows / Visual Studio / DxLibでのビルド・表示確認は別途必要です。
