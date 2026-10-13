#pragma once

// 形状列挙型定義
enum class COLLIDER_SHAPE
{
	None = -1,

	Line,
	Sphere,
	Capsule,
	Box,
	Mesh,
	XzCircle,
};

// コライダータグ列挙型定義
enum class COLLIDER_TAG
{
	NON = -1,

	PLAYER,

	PLAYER_COMMON,

	PLAYER_ATTACK,

	PLAYER_HEAL,
	PLAYER_BUFF,
	PLAYER_DEBUFF,

	BOSS,

	BOSS_ATTACK_AREA,

	BOSS_ATTACK,

	ENEMY,

	BOSS_DISTANCE,

	STAGE,

	DEBUG_OBJECT,
};