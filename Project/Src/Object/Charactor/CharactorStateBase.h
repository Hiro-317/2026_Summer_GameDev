#pragma once

#include <vector>
#include <functional>

class CharactorStateBase
{
public:
	CharactorStateBase(
		const std::function<void(void)>& ownChangeState,
		std::function<bool(void)> isOwnState
	) :
		ownChangeState(ownChangeState),
		isOwnState(isOwnState)
	{
	}
	virtual ~CharactorStateBase() = default;

	// 自分の状態に遷移する条件関数
	virtual void OwnStateConditionUpdate(void) {}

	// 他の状態に遷移する条件関数を追加する関数
	void AddOtherStateCondition(const std::function<void(void)>& func) {
		otherStateConditions.push_back(func);
		if (!isOwnState()) { Exit(); }
	}

	// 他の状態に遷移する条件関数をすべて呼び出す関数
	void OtherStateConditionsUpdate(void) {
		for (std::function<void(void)>& func : otherStateConditions) { func(); }
	}

	virtual void Init(void) {}
	virtual void Update(void) {}
	virtual void Exit(void) {}

protected:
	// 自分の状態に遷移させる関数のポインタ
	const std::function<void(void)> ownChangeState;

private:
	// 現在自分の状態かどうかを返す関数のポインタ
	std::function<bool(void)> isOwnState;

	// 他の状態に遷移する条件関数のポインタを格納するベクター
	std::vector<std::function<void(void)>> otherStateConditions;
};