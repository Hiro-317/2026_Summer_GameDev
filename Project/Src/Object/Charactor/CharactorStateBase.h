#pragma once

class CharactorStateBase
{
public:
	CharactorStateBase() = default;
	virtual ~CharactorStateBase() = default;

	virtual void Load(void) {}
	virtual void Init(void) {}
	virtual void Update(void) {}
	virtual void Draw(void) {}
	virtual void Release(void) {}
};