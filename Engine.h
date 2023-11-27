#pragma once

DWORD GetGame()
{
	DWORD GameAddress;
	GameAddress = read<DWORD>(pGetGame_Adr + BaseGameAddress());
	GameAddress ^= pGetGame_Xor;
	return GameAddress;
}

DWORD GetGameWorld()
{
	DWORD GameWorldAddress;
	GameWorldAddress = read<DWORD>(pGameWorld_Adr + BaseGameAddress());
	GameWorldAddress ^= pGameWorld_Xor;
	return GameWorldAddress;
}