# 紫色の爆発エフェクト

`EffectBase` は `ActorBase` を継承するエフェクト用の中間基底クラスです。
`PurpleExplosionActor` はその派生で、指定したワールド座標に紫色の爆発を表示します。
攻撃やキー入力など、固定の発生タイミングは設定していません。

## 任意のタイミングで生成する

シーンの `SubPostLoad` や `SubPostUpdate` など、`AddActor` を呼べる場所で追加します。
`SceneBase` が読み込み・初期化・更新・描画・解放を管理します。

```cpp
#include "../../Object/Common/Effect/PurpleExplosionActor.h"

// シーンの更新処理など、出したいタイミングで1回呼ぶ。
AddActor(new PurpleExplosionActor(
    Vector3(0.0f, 150.0f, 0.0f), // ワールド座標
    150.0f,                     // 最大半径（ワールド単位）
    1.2f,                       // 持続時間（秒）
    2.0f,                       // 明るさ
    1.0f                        // 模様のシード
));
```

親アクターから子として使用する場合は、その親の読み込み時に `AddChildActor` で追加してください。
生成時の所有権はシーンまたは親アクターに渡すため、利用側で `delete` しません。

## 再利用する

終了時は更新と描画を止めます。自動削除はせず、シーン・親アクターの解放時に破棄します。
頻繁に発生させる場合は事前に用意したインスタンスを再利用できます。

```cpp
// シーンが所有するインスタンスへの参照を保持しておく。
auto* explosion = new PurpleExplosionActor(Vector3(0.0f, 150.0f, 0.0f));
AddActor(explosion);

// 初期化後、別のタイミング・座標で再生する。
explosion->SetPos(Vector3(300.0f, 150.0f, 0.0f));
explosion->Restart();

// 明示停止と終了確認。
explosion->Stop();
bool finished = explosion->IsFinished();
```

経過時間は既存ゲームに合わせて `1 / 60秒 × TimeScale::Get()` で進めます。
ポーズ中など、所有者が更新しない間は再生も進みません。

## 色を変更する

色は `0xRRGGBB` 形式で指定します。指定を省略すると紫色になります。

```cpp
// コンストラクターの最後の引数で、生成時の色を指定する。
auto* explosion = new PurpleExplosionActor(
    Vector3(0.0f, 150.0f, 0.0f), 150.0f, 1.2f, 2.0f, 1.0f, 0xff4000);
AddActor(explosion);

// 再生中にも変更可能。次の描画から青色に変わる。
explosion->SetColor(0x00aaff);

// 初期化前やRestartの前にも設定できる。色の変更だけでは時間をリセットしない。
explosion->SetColor(0x00ff80);
explosion->Restart();
```

火球・衝撃波・火花が指定色に合わせて変わり、中心と火花は明るい色になります。
黒（`0x000000`）は発光しません。明るさは引き続き `intensity` で指定します。
各アクターが色を保持するため、同時に別々の色で再生できます。

## シェーダーとビルド

- `PurpleExplosionShader` は `ShaderBase` を継承し、定数バッファの更新・適用・解除を担当します。
- 頂点・ピクセルシェーダーは `ShaderResourceManager` が所有し、複数の爆発で共有します。
- 各爆発が所有するのは個別の定数バッファです。PSの `b4` へ時間・持続時間・明るさ・シード・RGBを渡します（32バイト）。
- `PurpleExplosionVS.hlsl` / `PurpleExplosionPS.hlsl` を `main`、Shader Model 5.0でビルドします。
- `Project.vcxproj` に登録済みで、出力先は `Project/Data/Shader/PurpleExplosionVS.cso` と `PurpleExplosionPS.cso` です。
- `PurpleExplosion.hlsl` は共通の評価関数で、単体のコンパイル対象ではありません。
- 描画は半透明パスで行い、乗算済みαの加算ブレンドを使います。深度判定は有効、描画中の深度書き込みは無効です。
- 描画後はブレンド・カリングを元に戻し、深度書き込みは既存3Dシーンの通常設定（有効）へ戻します。

HLSLファイルはUTF-8の署名なし（BOMなし）で保存してください。C++ファイルはUTF-8 BOM付きです。

Windows版Visual Studio / DxLibでの実行確認は別途必要です。
