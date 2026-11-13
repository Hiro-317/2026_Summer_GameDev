# 中心から周囲へ広がる土埃

`DustBurstActor : EffectBase` が、地面を広がる太い輪と、その内側・周囲を舞う20個の細かな砂煙を描きます。
地面の輪は64分割の円形メッシュで、カメラに合わせて回転しません。
砂煙は穴のあいたノイズの破片として描き、丸い塊を塗りつぶす形を使いません。
煙はsettings.heightに従って輪から立ち上がり、時間とともに上昇します。輪の幅とは独立して高さを調整できます。
基本色は参考画像に合わせた橙色です。角度に応じて赤寄り・黄寄りの明るさを付けます。
`SetColor()`で白、砂色、別の色へ変えることもできます。
輪は内外の端を柔らかくぼかし、砂煙は画面へ正対させて幅を保ちます。

```cpp
#include "../../Object/Common/Effect/DustBurstActor.h"

// シーン/親アクターの読み込み時に、必要な分だけ準備する。
DustBurstSettings settings;
settings.radius = 180.0f;         // 中心から外へ広がる距離（ワールド単位）
settings.height = 100.0f;         // 巻き上がる高さの基準
settings.durationSeconds = 1.0f;  // 消滅演出を含む寿命。0なら無期限
settings.stopSeconds = 0.3f;      // 中心から外周へ消える時間
settings.opacity = 0.9f;          // 不透明度（0～1）
settings.color = 0xffa04a;        // 参考画像に近い橙。砂色なら0xbca078など
settings.seed = 1.0f;             // 形・配置のシード

auto* dust = new DustBurstActor(groundPosition, settings);
AddActor(dust);                  // 初期化のみ。親で所有するならAddChildActor

// 着地・衝撃などの発生時に一度だけ呼ぶ。
dust->SetPosition(groundPosition);
dust->Restart();                // 毎回中心から伸びる。再生中でも最初からやり直す
```

同時に異なる場所で出す場合は、必要数のアクターを事前に準備します。
アクターの所有権はシーンまたは親へ渡し、利用側でdeleteしません。
一般エフェクト用の管理アクターは追加していません。

## 中心から外周への消滅

有限寿命では、最後の`min(stopSeconds, durationSeconds * 0.5)`秒を消滅演出に使います。
短い寿命でも前半に出現時間を残します。寿命に達するころには煙が消え切ります。
不透明度を一斉に下げるのではなく、中心からの地面上の距離に沿って消す範囲を広げます。

```cpp
dust->Stop();             // 寿命を待たず、中心から外周へ消す
dust->StopImmediately();  // 消滅演出を待たず、その場で全て消す
dust->Restart();          // 出現・消滅状態と時間を戻して再利用
```

`Stop()`を毎フレーム呼んでも演出は最初へ戻りません。
演出中は`IsFinished()`がfalseです。完全停止するまでUpdate/Drawを続けてください。
`TimeScale`が0の間は出現・消滅も進みません。
`stopSeconds = 0.0f`なら、寿命切れ・Stopとも即時に消します。
初期化前のStopも即時キャンセルします。

寿命`0.0f`は無期限です。1秒で展開して残り、Stopを呼ぶと同じ中心からの消滅演出を行います。
土埃のPlayは、待機・終了後・消滅途中から呼ぶと新しい発生として0秒から始まります。
通常の再生中にPlayを呼んでも時間は戻りません。常に最初からやり直す場合はRestartを使ってください。
共通のEffectBase::Playは経過時間を戻さず再生状態へ戻します。
寿命に達したときの共通経過時間は0へ戻さず寿命の値で止め、Restartで0へ戻します。

## リソース・ビルド

- シェーダーは`Project/Src/Shader/DustBurst/`に配置しています。
- `DustBurstShader : ShaderBase`は`ShaderResourceManager`の共有VS/PSを初回Init時だけロードします。
- 既存のDustBurstActorとVS/PSを変更し、DustBurstShaderの共有ハンドルと定数バッファをそのまま利用します。別のエフェクトや管理クラスは追加していません。
- 各アクターは既存と同じ48バイトの個別定数バッファを持ちます。テクスチャは使いません。
- VS/PSへ渡す描画モードが変わるため、C++とDustBurstVS/PS.hlslを一緒にリビルドしてください。
- `Project.vcxproj`とfiltersへC++/HLSLを登録済みです。
- VS/PSは`main` / Shader Model 5.0、出力先は`Data/Shader/DustBurstVS.cso`と`DustBurstPS.cso`です。
- プル後はVisual Studioでリビルドしてください。C++はUTF-8 BOM付き、HLSLはBOMなしです。
- 深度判定は有効、深度書き込みは無効です。床とのちらつきを避けて輪を少し上に置き、輪→奥から並べた砂煙の順に描画します。
- 壁と交差する部分のソフトパーティクル処理、当たり判定、ダメージ処理は含みません。

Windows / Visual Studio / DxLibでのゲーム内表示確認は別途必要です。
