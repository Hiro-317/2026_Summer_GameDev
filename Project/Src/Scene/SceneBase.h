#pragma once

#include "../Object/ActorBase.h"

class SceneBase
{
public:
	// コンストラクタ
	SceneBase(void) : objects() {}
	// デストラクタ
	virtual ~SceneBase(void) = default;

	// 読み込み
	virtual void Load(void) = 0;
	// 初期化
	virtual void Init(void) { for (ActorBase* obj : objects) { obj->Init(); } }
	// 更新
	virtual void Update(void) { for (ActorBase* obj : objects) { obj->Update(); } }
	// 描画
	virtual void Draw(void) { for (ActorBase* obj : objects) { obj->Draw(); } }
	// 解放
	virtual void Release(void) {
		for (ActorBase*& obj : objects) {
			obj->Release();
			delete obj;
			obj = nullptr;
		}
		objects.clear();
	}

protected:

	// オブジェクト格納用の配列
	std::vector<ActorBase*>objects;

	// 配列の中から特定のオブジェクトを探す
	template<typename T>
	T* ObjSerch(void) {
		for (auto* obj : objects) {
			if (T* cast = dynamic_cast<T*>(obj)) { return cast; }
		}
		return nullptr;
	}
	template<typename T>
	std::vector<T*> ObjArraySerch(void) {
		std::vector<T*> objArray;
		for (auto* obj : objects) {
			if (T* cast = dynamic_cast<T*>(obj)) { objArray.emplace_back(cast); }
		}
		return objArray;
	}
};
