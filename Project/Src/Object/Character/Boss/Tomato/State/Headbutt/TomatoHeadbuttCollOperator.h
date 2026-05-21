#pragma once

#include "../../../../../ActorBase.h"

class TomatoHeadbuttCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	TomatoHeadbuttCollOperator(
		const CharacterStats stats,
		const float CAPSULE_COLLIDER_RADIUS,
		const Vector3 CAPSULE_COLLIDER_START_POS_X,
		const Vector3 CAPSULE_COLLIDER_END_POS_X,
		const Vector3 CAPSULE_COLLIDER_START_POS_XZ,
		const Vector3 CAPSULE_COLLIDER_END_POS_XZ,
		const Vector3 CAPSULE_COLLIDER_START_POS_Z,
		const Vector3 CAPSULE_COLLIDER_END_POS_Z,
		const Vector3 CAPSULE_COLLIDER_START_POS_ZX,
		const Vector3 CAPSULE_COLLIDER_END_POS_ZX,
		const float CAPSULE_COLLIDER_ENOUGH_DISTANCE,
		const Vector3& pos, const Vector3& angle
	);
	~TomatoHeadbuttCollOperator()override = default;

	void Load(void)override;

	void OnCollision(const ColliderBase& other)override;

	void CollSet(bool flg) { SetJudge(flg); }

private:

	// カプセルコライダーの半径
	const float CAPSULE_COLLIDER_RADIUS;

	// カプセルコライダーのローカルX始点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_START_POS_X;
	// カプセルコライダーのローカルX終点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_END_POS_X;

	// カプセルコライダーのローカルXZ始点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_START_POS_XZ;
	// カプセルコライダーのローカルXZ終点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_END_POS_XZ;

	// カプセルコライダーのローカルZ始点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_START_POS_Z;
	// カプセルコライダーのローカルZ終点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_END_POS_Z;

	// カプセルコライダーのローカルZX始点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_START_POS_ZX;
	// カプセルコライダーのローカルZX終点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_END_POS_ZX;

	// カプセルコライダーの絶対に当たらないおおよその距離
	const float CAPSULE_COLLIDER_ENOUGH_DISTANCE;


#pragma region 受け取る参照変数

	const CharacterStats stats;

	const Vector3& pos;

	const Vector3& angle;

#pragma endregion

	// 更新処理
	void SubUpdate(void)override;

};