#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_VERTEX3DSHADER
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// 型を直接宣言し、VS_INPUTの条件付き別名に依存しないようにする。
// DxLibの固定入力レイアウトと同じ9要素・同じ順番を必ず宣言する。
// 未使用項目を省くと、固定レイアウトのUV入力位置とシェーダー側のレジスターがずれる。
// POSITION0=v0、TEXCOORD0=v7、TEXCOORD1=v8。使わない値も削除しないこと。
struct FlameBreathVertexInput
{
    float3 positionWS : POSITION0;
    float4 sparePosition : POSITION1;
    float3 normal : NORMAL0; // ブレスの方向×現在の距離（ワールド空間）
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
    float4 diffuse : COLOR0;
    float4 specular : COLOR1;
    float2 uv : TEXCOORD0;
    float2 drawParameters : TEXCOORD1; // x: 描画モード、y: 本体の符号付き幅/補助炎の半径
};

struct FlameBreathOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    // x: 本体/正面用の区別、y: 表示量。三角形内で補間する必要はない。
    nointerpolation float2 drawParameters : TEXCOORD1;
};

FlameBreathOutput main(FlameBreathVertexInput input)
{
    FlameBreathOutput output;
    // 中心軸上の位置と方向を、描画で使用するカメラ座標へ変換する。
    float3 viewPosition = mul(float4(input.positionWS, 1.0f), g_base.viewMatrix);
    float3 axisVS = mul(float4(input.normal, 0.0f), g_base.viewMatrix);
    float visibility = 1.0f;
    if (input.drawParameters.x < 0.5f) {
        // 透視投影後の軸の向き。画面端でも奥行きによる傾きを含めて求める。
        // 同じ軸上なら始点と終点で同じ方向になる。
        float2 projectedAxis = axisVS.xy * viewPosition.z - viewPosition.xy * axisVS.z;
        float axisLengthSquared = dot(projectedAxis, projectedAxis);
        float2 side = float2(1.0f, 0.0f);
        if (axisLengthSquared > 0.000001f) {
            side = float2(-projectedAxis.y, projectedAxis.x) * rsqrt(axisLengthSquared);
        }
        // 深度を変えずに画面の横方向へ広げるので、カメラ角度で幅が潰れない。
        viewPosition.xy += side * input.drawParameters.y;
    } else {
        // 軸方向から見る場合の補助炎は、爆発と同様に画面へ完全に正対させる。
        float2 corner = float2(input.uv.x * 2.0f - 1.0f, 1.0f - input.uv.y * 2.0f);
        float alignment = abs(dot(axisVS, -viewPosition)) *
            rsqrt(max(dot(axisVS, axisVS) * dot(viewPosition, viewPosition), 0.000001f));
        visibility = smoothstep(0.75f, 0.98f, saturate(alignment));
        viewPosition.xy += corner * input.drawParameters.y;
    }
    output.position = mul(float4(viewPosition, 1.0f), g_base.projectionMatrix);
    output.uv = input.uv;
    // 本体の幅は頂点ごとに符号が違うため、PSへ渡す表示量とは分ける。
    output.drawParameters = float2(input.drawParameters.x, visibility);
    return output;
}
