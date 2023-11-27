#pragma once

D3DXVECTOR3 GetBoneOrigin(DWORD pPlayer, int iBoneID)
{
	D3DXMATRIX v6;
	D3DXMATRIX v7;
	D3DXMATRIX BoneMatrix;
	D3DXVECTOR3 Pos;

	D3DXMatrixRotationYawPitchRoll(&v6, 0.0, 1.570796251296997, 0.0);

	DWORD CSkeletonAddress = read<DWORD>(pPlayer + m_pSkeleton_off);

	CSkeleton _pSkeleton = read<CSkeleton>(CSkeletonAddress);
	CBoneInfo _pBoneInfo = read<CBoneInfo>((DWORD)_pSkeleton.BonePtr);

	DWORD CBoneArrayAddress = (DWORD)_pBoneInfo.pBones;

	CBone Bone = read<CBone>((DWORD)CBoneArrayAddress + (iBoneID * sizeof(CBone)));

	BoneMatrix = Bone.BoneMatrix;

	D3DXMATRIX BoneCoeff = read<D3DXMATRIX>(pPlayer + BoneCoeff_off);

	D3DXMatrixMultiply(&BoneMatrix, &BoneMatrix, &BoneCoeff);
	D3DXMatrixMultiply(&v7, &v6, &BoneMatrix);

	Pos.x = v7._41;
	Pos.y = v7._42;
	Pos.z = v7._43;

	return Pos;
}

void DrawSkeleton(DWORD Player)
{
	int aSkeleton[][2] = {
		{ 1, 2 }, { 2, 3 }, { 3, 4 }, { 4, 5 }, { 5, 6 },
		{ 5, 7 }, { 7, 8 }, { 8, 9 }, { 9, 10 },
		{ 5, 28 }, { 28, 29 }, { 29, 30 }, { 30, 31 },
		{ 53, 54 }, { 54, 55 }, { 55, 56 },
		{ 58, 59 }, { 59, 60 }, { 60, 61 },
		{ 53, 58 },
	};



	for (int x = 0; x < 20; x++)
	{
		D3DXVECTOR3 Pos1 = GetBoneOrigin(Player, aSkeleton[x][0]);
		D3DXVECTOR3 Pos1s;

		D3DXVECTOR3 Pos12 = GetBoneOrigin(Player, aSkeleton[x][0]);
		D3DXVECTOR3 Pos1s2;

		D3DXVECTOR3 Pos122 = GetBoneOrigin(Player, aSkeleton[x][0]);
		D3DXVECTOR3 Pos1s22;

		D3DXVECTOR3 Pos1222 = GetBoneOrigin(Player, aSkeleton[x][0]);
		D3DXVECTOR3 Pos1s222;

		ProjectToScreen(Pos1, &Pos1s, Overlay->d3dDevice);
		ProjectToScreen(Pos12, &Pos1s2, Overlay->d3dDevice);
		ProjectToScreen(Pos122, &Pos1s22, Overlay->d3dDevice);
		ProjectToScreen(Pos1222, &Pos1s222, Overlay->d3dDevice);

		if (Pos1s2 != Pos1s)
			return;
		if (Pos1s != Pos1s22)
			return;
		if (Pos1s.x != Pos1s222.x)
			return;

		D3DXVECTOR3 Pos2 = GetBoneOrigin(Player, aSkeleton[x][1]);
		D3DXVECTOR3 Pos2s;

		D3DXVECTOR3 Pos22 = GetBoneOrigin(Player, aSkeleton[x][1]);
		D3DXVECTOR3 Pos2s2;

		D3DXVECTOR3 Pos222 = GetBoneOrigin(Player, aSkeleton[x][1]);
		D3DXVECTOR3 Pos2s22;

		D3DXVECTOR3 Pos2222 = GetBoneOrigin(Player, aSkeleton[x][1]);
		D3DXVECTOR3 Pos2s222;

		ProjectToScreen(Pos2, &Pos2s, Overlay->d3dDevice);
		ProjectToScreen(Pos22, &Pos2s2, Overlay->d3dDevice);
		ProjectToScreen(Pos222, &Pos2s22, Overlay->d3dDevice);
		ProjectToScreen(Pos2222, &Pos2s222, Overlay->d3dDevice);

		if (Pos2s2 != Pos2s)
			return;
		if (Pos2s22 != Pos2s)
			return;
		if (Pos2s.x != Pos2s222.x)
			return;

		Overlay->DrawLine(Pos1s.x, Pos1s.y, Pos2s.x, Pos2s.y, 0.9, 255, 0, 0);
	}
}

DWORD GetAimKey()
{
	if (MyMenu[5].value == 0)
		return 0x02;
	else if (MyMenu[5].value == 1)
		return 0x01;
}

static DWORD TickInvite;

int FOVSettint;
DWORD Player[999];
int PlayerCount;

int Close_Screen(LPDIRECT3DDEVICE9 pDevice)
{
	if (PlayerCount > 0)
	{
		CGameWorld gw = read<CGameWorld>(GetGameWorld());

		float smallestDistance;
		int index = -1;

		for (int i = 0; i < PlayerCount; i++)
		{
			DWORD object = read<int>(gw.GetObject(Player[i]));

			D3DXVECTOR3 pRootPos, RootPos, headVec, screen, box, screenHead, vTargetDistance;

			D3DVIEWPORT9 ViewP;
			pDevice->GetViewport(&ViewP);

			D3DXVECTOR3 Center((float)(ViewP.Width / 2), (float)(ViewP.Height / 2), 0);

			headVec = GetBoneOrigin((DWORD)object, Bip01_Head);

			int CrosshairX = (ViewP.Width / 2);
			int CrosshairY = (ViewP.Height / 2);

			if (ProjectToScreen(headVec, &screenHead, Overlay->d3dDevice))
			{
				if (screenHead.z > 1.0f)
					continue;

				float fDistance = getDistance(screenHead, Center);

				if (index == -1 || fDistance < smallestDistance)
				{
					smallestDistance = fDistance;
					index = i;
				}

			}
		}
		if (index == -1)
			return 99999;
		else
			return index;
	}
}

void ESPPlayer()
{
	CGameWorld gw = read<CGameWorld>(GetGameWorld());

	int Target = Close_Screen(Overlay->d3dDevice);

	if ((GetTickCount() > TickInvite + 20000))
	{
		TickInvite = GetTickCount();

		PlayerCount = 0;
		for (int i = 0; i < gw.GetMaxObjects(); i++)
		{
			DWORD object = read<int>(gw.GetObject(i));

			if (object == NULL || object == LocalPlayerAddress)
				continue;

			if (object && read<int>((DWORD)object + Objecttype_off) == OBJTYPE_HUMAN || object && read<int>((DWORD)object + Objecttype_off) == OBJTYPE_HUMAN1)
			{
				Player[PlayerCount] = i;
				PlayerCount++;
			}
		}
	}

	if (PlayerCount > 0)
	{
		for (int i = 0; i < PlayerCount; i++)
		{
			DWORD object = read<int>(gw.GetObject(Player[i]));

			D3DXVECTOR3 AimTarget, NeckPos, SpinePos, screenSpine, screenNeck, pRootPos, RootPos, headVec, bodyVec, bodyscreen, screen, box, screenHead, vTargetDistance;

			if (object == NULL || object == LocalPlayerAddress)
				continue;

			if (object && read<int>((DWORD)object + Objecttype_off) == OBJTYPE_HUMAN || object && read<int>((DWORD)object + Objecttype_off) == OBJTYPE_HUMAN1)
			{
				pRootPos = read<D3DXVECTOR3>((DWORD)LocalPlayerAddress + Getposition_off);
				headVec = GetBoneOrigin((DWORD)object, Bip01_Head);
				NeckPos = GetBoneOrigin((DWORD)object, Bip01_Neck);
				RootPos = read<D3DXVECTOR3>((DWORD)object + Getposition_off);

				float flDistance = getDistance(pRootPos, RootPos);
				
				float health = read<float>((DWORD)object + GetHealth_off);

				if (ProjectToScreen(RootPos, &screen, Overlay->d3dDevice))
				{
					if (ProjectToScreen(NeckPos, &screenNeck, Overlay->d3dDevice))
					{
						if (ProjectToScreen(headVec, &screenHead, Overlay->d3dDevice))
						{
							if (MyMenu[4].value == 1)
							{
								if (MyMenu[6].value == 0)
								{
									AimTarget = screenHead;
								}

								if (MyMenu[6].value == 1)
								{
									AimTarget = screenNeck;
								}

								if (Target == i)
								{
									D3DVIEWPORT9 viewPort;

									int curx = AimTarget.x;
									int cury = AimTarget.y;
									Overlay->d3dDevice->GetViewport(&viewPort);
									double DistX = (double)curx - viewPort.Width / 2.0f;
									double DistY = (double)cury - viewPort.Height / 2.0f;

									DistX /= (float)5;
									DistY /= (float)5;

									if (GetAsyncKeyState(GetAimKey()))
									{
										mouse_event(MOUSEEVENTF_MOVE, (int)DistX, (int)DistY, NULL, NULL);
									}
								}
							}

							if (MyMenu[0].value == 1)
							{
								D3DXVECTOR3 box;
								box.y = ((screen.y - screenHead.y) > 1.f) ? screen.y - screenHead.y : 1.f;
								box.x = box.y / 2.75f;

								Overlay->DrawLine(screen.x - (box.x / 2), screen.y - box.y, screen.x - (box.x / 2) + box.x, screen.y - box.y, 0.9, 255, 0, 0);
								Overlay->DrawLine(screen.x - (box.x / 2) + box.x, screen.y - box.y, screen.x - (box.x / 2) + box.x, screen.y - box.y + box.y, 0.9, 255, 0, 0);
								Overlay->DrawLine(screen.x - (box.x / 2) + box.x, screen.y - box.y + box.y, screen.x - (box.x / 2), screen.y - box.y + box.y, 0.9, 255, 0, 0);
								Overlay->DrawLine(screen.x - (box.x / 2), screen.y - box.y + box.y, screen.x - (box.x / 2), screen.y - box.y, 0.9, 255, 0, 0);
							}
						}
					}

					if (MyMenu[1].value == 1)
					{
						DrawSkeleton(object);
					}
					
					if (MyMenu[2].value == 1)
					{
						swprintf(txt, L"%0.1f", health);
						Overlay->DrawString(txt, 9, screen.x - 5, screen.y + 3, true, Col.red[1], Col.red[0], Col.red[0], Col.red[1]);
					}

					if (MyMenu[3].value == 1)
					{
						swprintf(txt, L"%0.0fm", flDistance);
						Overlay->DrawString(txt, 9, screen.x - 5, screen.y + 13, true, Col.bule[0], Col.bule[1], Col.bule[2], Col.bule[3]);
					}
				}
			}
		}
	}
}