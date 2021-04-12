#pragma once

namespace Category
{
	enum Type
	{
		None				= 0,
		Scene				= 1 << 0,
		PlayerCharacter		= 1 << 1,
		PlayerProjectile	= 1 << 2,
	};
}