#pragma once

#include "ChunkStDefine.h"

#include"../../Object/Common/Collider/LineCollider.h"
#include"../../Object/Common/Collider/SphereCollider.h"
#include"../../Object/Common/Collider/CapsuleCollider.h"
#include"../../Object/Common/Collider/BoxCollider.h"
#include"../../Object/Common/Collider/ModelCollider.h"
#include"../../Object/Common/Collider/XZCircleCollider.h"

class CollisionManager
{
private:

#pragma region コライダーグループ定義

	// 列挙型定義定義
	enum class COLLIDER_GROUP {

		// 基本種別～～～～～～～～～～～～

		// プレイヤー系
		Player,
		// エネミー系
		Enemy,
		// ステージ系
		Stage,

		// ～～～～～～～～～～～～基本種別


		// 特殊種別～～～～～～～～～～～～

		// プレイヤー系にだけ当たるコライダー
		PlayerOnly,
		// エネミー系にだけ当たるコライダー
		EnemyOnly,
		// ステージ系にだけ当たるコライダー
		StageOnly,

		// ～～～～～～～～～～～～特殊種別

		// その他(例外)
		Other,

		// 種類数
		Max
	};

	/*以下の組み合わせの判定が実行される（順番も以下の通り）
	*
	* ①プレイヤー系 × ステージ系
	* ②エネミー系	 × ステージ系
	* 
	* ③プレイヤー系 × エネミー系
	* 
	* ④プレイヤー系 × プレイヤー系にだけ当たるコライダー
	* ⑤エネミー系	 × エネミー系にだけ当たるコライダー
	* ⑥ステージ系	 × ステージ系にだけ当たるコライダー
	*
	* ⑦その他 × ステージ系
	* ⑧その他 × プレイヤー系
	* ⑨その他 × エネミー系
	* ⑩その他 × その他
	*/

	// 各タグのグループ分け
	const std::map<COLLIDER_TAG, COLLIDER_GROUP>TAG_TO_GROUP_LIST = {

		// プレイヤー系
		{ COLLIDER_TAG::PLAYER,			COLLIDER_GROUP::Player },
		{ COLLIDER_TAG::PLAYER_ATTACK,	COLLIDER_GROUP::Player },
		{ COLLIDER_TAG::PLAYER_COMMON,	COLLIDER_GROUP::Player },

		// エネミー系
		{ COLLIDER_TAG::BOSS,	COLLIDER_GROUP::Enemy },
		{ COLLIDER_TAG::ENEMY,	COLLIDER_GROUP::Enemy },

		// ステージ系
		{ COLLIDER_TAG::STAGE,	COLLIDER_GROUP::Stage },

		// プレイヤー系にだけ当たるコライダー
		{ COLLIDER_TAG::TOMATO_BOSS_DISTANCE,	COLLIDER_GROUP::PlayerOnly },
		{ COLLIDER_TAG::BOSS_ATTACK,			COLLIDER_GROUP::PlayerOnly },

		// エネミー系にだけ当たるコライダー


		// ステージ系にだけ当たるコライダー
		{ COLLIDER_TAG::BOSS_ATTACK_AREA,	COLLIDER_GROUP::StageOnly },
	};


#pragma endregion

public:
	CollisionManager() : groupColliders() {}
	~CollisionManager() = default;

	// オブジェクト追加
	void Add(ColliderBase* collider);
	// オブジェクト追加
	void Add(std::vector<ColliderBase*> collider);

	// チャンク分け(初期化時)
	void InitBuildChunks(void);

	// 判定実行
	void Check(void);

	// チャンク描画
	void DrawChunkGrid(void) const;

	// 解放
	void Clear(void) {
		for (ColliderGroupData& colliders : groupColliders) {
			colliders.dynamicChunksXZ.clear();
			colliders.staticChunksXZ.clear();
			colliders.dynamicChunks3D.clear();
			colliders.staticChunks3D.clear();
			colliders.colliders.clear();
		}
	}

private:

	// グループ別の格納配列
	ColliderGroupData groupColliders[(int)COLLIDER_GROUP::Max];

	// 重複判定防止の判定記録
	std::unordered_set<ColliderPairKey, ColliderPairKeyHash> checkedPairs;

#pragma region チャンク分け用

	// 重なるチャンクインデックス(ChunkIndex)一覧を取得する
	void GetOverlappedChunks3D(const ColliderBase::AABB& aabb, std::vector<ChunkIndex>& out) const {
		out.clear();

		ChunkIndex minIndex = {
			ToChunkIndex(aabb.min.x, CHUNK_SIZE),
			ToChunkIndex(aabb.min.y, CHUNK_SIZE),
			ToChunkIndex(aabb.min.z, CHUNK_SIZE)
		};

		ChunkIndex maxIndex = {
			ToChunkIndex(aabb.max.x, CHUNK_SIZE),
			ToChunkIndex(aabb.max.y, CHUNK_SIZE),
			ToChunkIndex(aabb.max.z, CHUNK_SIZE)
		};

		for (int z = minIndex.z; z <= maxIndex.z; z++) {
			for (int y = minIndex.y; y <= maxIndex.y; y++) {
				for (int x = minIndex.x; x <= maxIndex.x; x++) {
					out.push_back(ChunkIndex(x, y, z));
				}
			}
		}
	}
	// 重なるチャンクインデックス(ChunkIndex)一覧を取得する（XZチャンク用）
	void GetOverlappedChunksXZ(const ColliderBase::AABB& aabb, std::vector<ChunkIndex>& out) const {
		out.clear();

		ChunkIndex minIndex = {
			ToChunkIndex(aabb.min.x, CHUNK_SIZE),
			0,
			ToChunkIndex(aabb.min.z, CHUNK_SIZE)
		};

		ChunkIndex maxIndex = {
			ToChunkIndex(aabb.max.x, CHUNK_SIZE),
			0,
			ToChunkIndex(aabb.max.z, CHUNK_SIZE)
		};

		for (int z = minIndex.z; z <= maxIndex.z; z++) {
			for (int x = minIndex.x; x <= maxIndex.x; x++) {
				out.push_back(ChunkIndex(x, 0, z));
			}
		}
	}

	// 指定のチャンクマップにコライダーを1つ登録
	void RegisterToChunks3D(ChunkMap& chunks, ColliderBase* collider) {

		// 安全処理
		if (!collider) { return; }

		// 重なるチャンクインデックスを取得
		std::vector<ChunkIndex> indexes;
		GetOverlappedChunks3D(collider->GetAABB(), indexes);

		// 取得したチャンクインデックスすべてに登録
		for (const ChunkIndex& index : indexes) {
			chunks[index].colliders.emplace_back(collider);
		}
	}
	void RegisterToChunksXZ(ChunkMap& chunks, ColliderBase* collider) {

		// 安全処理
		if (!collider) { return; }

		// 重なるチャンクインデックスを取得
		std::vector<ChunkIndex> indexes;
		GetOverlappedChunksXZ(collider->GetAABB(), indexes);

		// 取得したチャンクインデックスすべてに登録
		for (const ChunkIndex& index : indexes) {
			chunks[index].colliders.emplace_back(collider);
		}
	}

	// チャンク分け(更新時)
	void BuildChunks(void) {
		for (ColliderGroupData& group : groupColliders) {

			// 動的コライダーチャンク分け配列をリセット
			group.dynamicChunks3D.clear();
			group.dynamicChunksXZ.clear();

			for (ColliderBase* collider : group.colliders) {
				// 安全処理
				if (!collider) { continue; }

				// 静的コライダーは変更なし
				if (!collider->GetDynamicFlg()) { continue; }

				// 動的コライダーチャンク分け配列に割り当てなおす
				if (collider->GetChunkSpace() == ColliderBase::CHUNK_SPACE::XYZ) {
					RegisterToChunks3D(group.dynamicChunks3D, collider);
					RegisterToChunksXZ(group.dynamicChunksXZ, collider);
				}
				else if (collider->GetChunkSpace() == ColliderBase::CHUNK_SPACE::XZ) {
					RegisterToChunksXZ(group.dynamicChunksXZ, collider);
				}
			}
		}
	}

#pragma endregion

#pragma region 判定の振り分け

	// 指定のグループ同士を判定(2グループ指定)
	void Matching(COLLIDER_GROUP groupA, COLLIDER_GROUP groupB);
	// 指定のグループを総当たりで判定(1グループ指定)
	void Matching(COLLIDER_GROUP group);

	// チャンクを振り分けて判定実行
	void MatchingChunks(ChunkMap& aChunks, ChunkMap& bChunks);
	// チャンクを振り分けて判定実行
	void MatchingChunks(ChunkMap& chunks);

	// 重複判定チェック
	void CheckPairOnce(ColliderBase* a, ColliderBase* b);

	// 形状の振り分け
	bool IsHit(ColliderBase* a, ColliderBase* b);

#pragma endregion

#pragma region 各形状の実判定

	// 同形状同士～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// 線分×線分
	bool LineToLine(LineCollider* a, LineCollider* b);

	// 球体×球体
	bool SphereToSphere(SphereCollider* a, SphereCollider* b);

	// カプセル×カプセル
	bool CapsuleToCapsule(CapsuleCollider* a, CapsuleCollider* b);

	// ボックス×ボックス
	bool BoxToBox(BoxCollider* a, BoxCollider* b);

	// モデル×モデル
	bool ModelToModel(ModelCollider* a, ModelCollider* b);

	// XZ平面上の円形×XZ平面上の円形
	bool XZCircleToXZCircle(XZCircleCollider* a, XZCircleCollider* b);

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～同形状同士


	// 別形状同士～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// 線分×球体
	bool LineToSphere(LineCollider* line, SphereCollider* sphere);

	// 線分×カプセル
	bool LineToCapsule(LineCollider* line, CapsuleCollider* capsule);

	// 線分×ボックス
	bool LineToBox(LineCollider* line, BoxCollider* box);

	// 線分×モデル
	bool LineToModel(LineCollider* line, ModelCollider* model);

	// 球体×カプセル
	bool SphereToCapsule(SphereCollider* sphere, CapsuleCollider* capsule);

	// 球体×ボックス
	bool SphereToBox(SphereCollider* sphere, BoxCollider* box);

	// 球体×モデル
	bool SphereToModel(SphereCollider* sphere, ModelCollider* model);

	// 球体×XZ平面上の円形
	bool SphereToXZCircle(SphereCollider* sphere, XZCircleCollider* xzcircle);

	// カプセル×ボックス
	bool CapsuleToBox(CapsuleCollider* capsule, BoxCollider* box);

	// カプセル×モデル
	bool CapsuleToModel(CapsuleCollider* capsule, ModelCollider* model);

	// カプセル×XZ平面上の円形
	bool CapsuleToXZCircle(CapsuleCollider* capsule, XZCircleCollider* xzcircle);

	// ボックス×モデル
	bool BoxToModel(BoxCollider* box, ModelCollider* model);

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～別形状同士

#pragma endregion

#pragma region ユーティリティ
	/// <summary>
	/// 指定した2つのコライダー同士に押し出し処理が必要かどうか
	/// </summary>
	/// <param name="a">コライダー１</param>
	/// <param name="b">コライダー２</param>
	/// <returns></returns>
	bool NeedPush(ColliderBase* a, ColliderBase* b)const {
		if (a->GetPushFlg() && b->GetPushFlg()) { return true; }
		return false;
	}

	/// <summary>
	/// 重みづけの割合を計算
	/// </summary>
	/// <param name="aWeight">（in） 重み</param>
	/// <param name="bWeight">（in） 重み</param>
	/// <param name="aWeightRatio">（out） 重みの割合</param>
	/// <param name="bWeightRatio">（out） 重みの割合</param>
	void WeightRatioCalculation(unsigned char aWeight, unsigned char bWeight, float& aWeightRatio, float& bWeightRatio) {
		// お互いの重みにおける割合を計算（相手の重み ÷ 自分と相手の重みの合計）

		// 自分と相手の重みの合計
		float abWeightSum = (float)(aWeight + bWeight);

		if (abWeightSum != 0.0f) {
			aWeightRatio = (float)bWeight / abWeightSum;
			bWeightRatio = (float)aWeight / abWeightSum;
		}
		else {
			// 両方重み０だったら半分ずつで返す（例外処理）
			aWeightRatio = bWeightRatio = 0.5f;
		}
	}

	/// <summary>
	/// 押し出し処理
	/// </summary>
	/// <param name="a">コライダー１</param>
	/// <param name="b">コライダー２</param>
	/// <param name="normal">押し出し方向</param>
	/// <param name="overlap">めり込んだ量</param>
	void ApplyPush(ColliderBase* a, ColliderBase* b, const Vector3& normal, float overlap) {
		// 動的フラグ
		bool aDynamic = a->GetDynamicFlg();
		bool bDynamic = b->GetDynamicFlg();

		// 両方動的オブジェクトの場合
		if (aDynamic && bDynamic)
		{
			float aRatio = 0.0f, bRatio = 0.0f;
			WeightRatioCalculation(a->GetPushWeight(), b->GetPushWeight(), aRatio, bRatio);

			a->SetTransformPos(a->GetTransform().pos + normal * (overlap * aRatio));
			b->SetTransformPos(b->GetTransform().pos - normal * (overlap * bRatio));

			if (normal.y > 0.5f) { a->CallOnGrounded(); }
			else if (normal.y < -0.5f) { b->CallOnGrounded(); }
		}
		// aだけ動的の場合
		else if (aDynamic && !bDynamic)
		{
			a->SetTransformPos(a->GetTransform().pos + normal * overlap);
			if (normal.y > 0.5f) { a->CallOnGrounded(); }
		}
		// bだけ動的の場合
		else if (!aDynamic && bDynamic)
		{
			b->SetTransformPos(b->GetTransform().pos - normal * overlap);
			if (normal.y < -0.5f) { b->CallOnGrounded(); }
		}
		// 両方静的オブジェクトの場合
		else { /*何もしない*/ }
	}

	/// <summary>
	/// 押し出し処理
	/// </summary>
	/// <param name="a">コライダー１</param>
	/// <param name="b">コライダー２</param>
	/// <param name="overlapVec">押し出しベクトル</param>
	void ApplyPush(ColliderBase* a, ColliderBase* b, const Vector3& overlapVec) {
		// 動的フラグ
		bool aDynamic = a->GetDynamicFlg();
		bool bDynamic = b->GetDynamicFlg();

		// 両方動的オブジェクトの場合
		if (aDynamic && bDynamic)
		{
			float aRatio = 0.0f, bRatio = 0.0f;
			WeightRatioCalculation(a->GetPushWeight(), b->GetPushWeight(), aRatio, bRatio);

			a->SetTransformPos(a->GetTransform().pos + overlapVec * aRatio);
			b->SetTransformPos(b->GetTransform().pos - overlapVec * bRatio);

			Vector3 normalOverlapVec = overlapVec.Normalized();

			if (normalOverlapVec.y > 0.5f) { a->CallOnGrounded(); }
			else if (normalOverlapVec.y < -0.5f) { b->CallOnGrounded(); }
		}
		// aだけ動的の場合
		else if (aDynamic && !bDynamic)
		{
			a->SetTransformPos(a->GetTransform().pos + overlapVec);
			if (overlapVec.Normalized().y > 0.5f) { a->CallOnGrounded(); }
		}
		// bだけ動的の場合
		else if (!aDynamic && bDynamic)
		{
			b->SetTransformPos(b->GetTransform().pos - overlapVec);
			if (overlapVec.Normalized().y < -0.5f) { b->CallOnGrounded(); }
		}
		// 両方静的オブジェクトの場合
		else { /*何もしない*/ }
	}

	/// <summary>
	/// 押し出し処理
	/// </summary>
	/// <param name="a">無条件に押し出される方</param>
	/// <param name="b">無条件に動かさず押し出す方</param>
	/// <param name="overlapVec">押し出しベクトル</param>
	void ApplyPushOneSide(ColliderBase* dynamicColl, ColliderBase* staticColl, const Vector3& overlapVec) {
		dynamicColl->SetTransformPos(dynamicColl->GetTransform().pos + overlapVec);
		if (overlapVec.Normalized().y > 0.5f) { dynamicColl->CallOnGrounded(); }
	}
#pragma endregion

};