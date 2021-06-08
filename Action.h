#pragma once
#include "CharState.h"


class Action : public CharState
{
public:
	virtual void									update(Character& character) override;
	virtual void									startup(Character& character);

protected:
	int												startup_;
	int												active_;
	int												recovery_;
};
