#pragma once

#include <unordered_map>
#include<vector>
#include <cmath>
#include <unordered_set>

#include"../../Object/Common/Collider/LineCollider.h"
#include"../../Object/Common/Collider/SphereCollider.h"
#include"../../Object/Common/Collider/CapsuleCollider.h"
#include"../../Object/Common/Collider/BoxCollider.h"
#include"../../Object/Common/Collider/ModelCollider.h"
#include"../../Object/Common/Collider/XZCircleCollider.h"

class CollisionManager
{

#pragma region 定義

private:

	// チャンクサイズ
	static constexpr float CHUNK_SIZE = 500.0f;

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

#pragma region チャンク管理構造体定義

	// チャンク座標
	struct ChunkIndex {
		int x = 0, y = 0, z = 0;

		bool operator==(const ChunkIndex& other) const {
			return x == other.x && y == other.y && z == other.z;
		}
	};

	// unordered_map 用ハッシュ
	struct ChunkIndexHash {
		std::size_t operator()(const ChunkIndex& index) const {
			std::size_t h1 = std::hash<int>()(index.x);
			std::size_t h2 = std::hash<int>()(index.y);
			std::size_t h3 = std::hash<int>()(index.z);

			return h1 ^ (h2 << 1) ^ (h3 << 2);
		}
	};

	// 1チャンクに所属するデータ
	struct ChunkData {
		std::vector<ColliderBase*> colliders;
	};

	// チャンク管理マップ
	using ChunkMap = std::unordered_map<ChunkIndex, ChunkData, ChunkIndexHash>;

	// 座標からチャンク番号に変換する関数
	static int ToChunkIndex(float pos, float chunkSize) {
		return static_cast<int>(std::floor(pos / chunkSize));
	}

#pragma endregion

	// 1グループが持つデータ
	struct ColliderGroupData {
		// コライダー本体
		std::vector<ColliderBase*> colliders;

		// 静的コライダーのチャンク分け参照
		ChunkMap staticChunks;

		// 動的コライダーのチャンク分け参照
		ChunkMap dynamicChunks;
	};

	struct ColliderPairKey {
		ColliderBase* a = nullptr;
		ColliderBase* b = nullptr;

		ColliderPairKey(ColliderBase* lhs, ColliderBase* rhs) {
			if (lhs < rhs) { a = lhs; b = rhs; }
			else { a = rhs; b = lhs; }
		}

		bool operator==(const ColliderPairKey& other) const {
			return a == other.a && b == other.b;
		}
	};

	struct ColliderPairKeyHash {
		std::size_t operator()(const ColliderPairKey& key) const {
			return std::hash<ColliderBase*>()(key.a) ^ (std::hash<ColliderBase*>()(key.b) << 1);
		}
	};

#pragma endregion

public:
	CollisionManager() : groupColliders() {}
	~CollisionManager() = default;

	// オブジェクト追加
	void Add(ColliderBase* collider) {

		// 安全処理
		if (!collider) { return; }

		// グループを一時変数として定義（「その他」で初期化）
		COLLIDER_GROUP group = COLLIDER_GROUP::Other;

		// グループリストに記述が存在すれば、取得
		if (TAG_TO_GROUP_LIST.contains(collider->GetTag())) {
			group = TAG_TO_GROUP_LIST.at(collider->GetTag());
		}

		// 最終的なグループで格納
		groupColliders[(int)group].colliders.emplace_back(collider);
	}
	// オブジェクト追加
	void Add(std::vector<ColliderBase*> collider) { for (ColliderBase*& c : collider) { Add(c); } }

	// 判定実行
	void Check(void);

	// 解放
	void Clear(void) {
		for (ColliderGroupData& colliders : groupColliders) {
			colliders.dynamicChunks.clear();
			colliders.staticChunks.clear();
			colliders.colliders.clear();
		}
	}

private:

	// グループ別の格納配列
	ColliderGroupData groupColliders[(int)COLLIDER_GROUP::Max];

	std::unordered_set<ColliderPairKey, ColliderPairKeyHash> checkedPairs;

#pragma region チャンク分け用

	// 重なるチャンクインデックス(ChunkIndex)一覧を取得する
	void GetOverlappedChunks(const ColliderBase::AABB& aabb, std::vector<ChunkIndex>& out) const {
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

	// 指定のチャンクマップにコライダーを1つ登録
	void RegisterToChunks(ChunkMap& chunks, ColliderBase* collider) {

		// 安全処理
		if (!collider) { return; }

		// 重なるチャンクインデックスを取得
		std::vector<ChunkIndex> indexes;
		GetOverlappedChunks(collider->GetAABB(), indexes);

		// 取得したチャンクインデックスすべてに登録
		for (const ChunkIndex& index : indexes) {
			chunks[index].colliders.emplace_back(collider);
		}
	}

	// チャンク分け
	void BuildChunks(void) {
		for (ColliderGroupData& group : groupColliders) {

			group.staticChunks.clear();
			group.dynamicChunks.clear();

			for (ColliderBase* collider : group.colliders) {
				// 安全処理
				if (!collider) { continue; }

				// 動的
				if (collider->GetDynamicFlg()) { RegisterToChunks(group.dynamicChunks, collider); }
				// 静的
				else { RegisterToChunks(group.staticChunks, collider); }
			}
		}
	}

#pragma endregion


#pragma region 当たり判定用
	void Matching(COLLIDER_GROUP groupA, COLLIDER_GROUP groupB);
	void Matching(COLLIDER_GROUP group);

	void MatchingChunks(ChunkMap& aChunks, ChunkMap& bChunks);
	void MatchingChunks(ChunkMap& chunks);

	void CheckPairOnce(ColliderBase* a, ColliderBase* b);

	bool IsHit(ColliderBase* a, ColliderBase* b);

	bool LineToLine(LineCollider* a, LineCollider* b);
	bool SphereToSphere(SphereCollider* a, SphereCollider* b);
	bool CapsuleToCapsule(CapsuleCollider* a, CapsuleCollider* b);
	bool BoxToBox(BoxCollider* a, BoxCollider* b);
	bool ModelToModel(ModelCollider* a, ModelCollider* b);
	bool XZCircleToXZCircle(XZCircleCollider* a, XZCircleCollider* b);

	bool LineToSphere(LineCollider* line, SphereCollider* sphere);
	bool LineToCapsule(LineCollider* line, CapsuleCollider* capsule);
	bool LineToBox(LineCollider* line, BoxCollider* box);
	bool LineToModel(LineCollider* line, ModelCollider* model);
	bool SphereToCapsule(SphereCollider* sphere, CapsuleCollider* capsule);
	bool SphereToBox(SphereCollider* sphere, BoxCollider* box);
	bool SphereToModel(SphereCollider* sphere, ModelCollider* model);
	bool SphereToXZCircle(SphereCollider* sphere, XZCircleCollider* xzcircle);
	bool CapsuleToBox(CapsuleCollider* capsule, BoxCollider* box);
	bool CapsuleToModel(CapsuleCollider* capsule, ModelCollider* model);
	bool CapsuleToXZCircle(CapsuleCollider* capsule, XZCircleCollider* xzcircle);
	bool BoxToModel(BoxCollider* box, ModelCollider* model);
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