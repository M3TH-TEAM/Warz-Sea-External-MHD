#pragma once

DWORD r3drender = NULL;

#define OBJTYPE_HUMAN 0x20
#define OBJTYPE_HUMAN1 0x21

enum E_Bones // 62
{
	Bip01 = 0,
	Bip01_Pelvis = 1,
	Bip01_Spine = 2,
	Bip01_Spine1 = 3,
	Bip01_Spine2 = 4,
	Bip01_Neck = 5,
	Bip01_Head = 6,
	Bip01_L_Clavicle = 7,
	Bip01_L_UpperArm = 8,
	Bip01_L_Forearm = 9,
	Bip01_L_Hand = 10,
	Bip01_L_Finger0 = 11,
	Bip01_L_Finger01 = 12,
	Bip01_L_Finger02 = 13,
	Bip01_L_Finger1 = 14,
	Bip01_L_Finger11 = 15,
	Bip01_L_Finger12 = 16,
	Bip01_L_Finger2 = 17,
	Bip01_L_Finger21 = 18,
	Bip01_L_Finger22 = 19,
	Bip01_L_Finger3 = 20,
	Bip01_L_Finger31 = 21,
	Bip01_L_Finger32 = 22,
	Bip01_L_Finger4 = 23,
	Bip01_L_Finger41 = 24,
	Bip01_L_Finger42 = 25,
	Bip01_L_ForeTwist = 26,
	Bip01_L_ForeTwist1 = 27,
	Bip01_R_Clavicle = 28,
	Bip01_R_UpperArm = 29,
	Bip01_R_Forearm = 30,
	Bip01_R_Hand = 31,
	Bip01_R_Finger0 = 32,
	Bip01_R_Finger01 = 33,
	Bip01_R_Finger02 = 34,
	Bip01_R_Finger1 = 35,
	Bip01_R_Finger11 = 36,
	Bip01_R_Finger12 = 37,
	Bip01_R_Finger2 = 38,
	Bip01_R_Finger21 = 39,
	Bip01_R_Finger22 = 40,
	Bip01_R_Finger3 = 41,
	Bip01_R_Finger31 = 42,
	Bip01_R_Finger32 = 43,
	Bip01_R_Finger4 = 44,
	Bip01_R_Finger41 = 45,
	Bip01_R_Finger42 = 46,
	PrimaryWeaponBone = 47,
	Bip01_R_ForeTwist = 48,
	Bip01_R_ForeTwist1 = 49,
	Weapon_BackLeft = 50,
	Weapon_BackRPG = 51,
	Weapon_BackRight = 52,
	Bip01_L_Thigh = 53,
	Bip01_L_Calf = 54,
	Bip01_L_Foot = 55,
	Bip01_L_Toe0 = 56,
	Weapon_Side = 57,
	Bip01_R_Thigh = 58,
	Bip01_R_Calf = 59,
	Bip01_R_Foot = 60,
	Bip01_R_Toe0 = 61
};

D3DXMATRIX matProj, matView, matWorld;

void MatrixToScreen()
{
	r3drender = read<DWORD>(BaseGameAddress() + dwRenderAddress);
	D3DXMATRIX matTmp = read<D3DXMATRIX>(r3drender + matTmp_off);

	if (matTmp.m[3][3] != 1.f)
	{
		matProj = matTmp;
		matView = read<D3DXMATRIX>(r3drender + matView_off);
	}
}

bool ProjectToScreen(const D3DXVECTOR3& pvPos, D3DXVECTOR3* pvOut, LPDIRECT3DDEVICE9 pDevice)
{
	D3DVIEWPORT9 ViewPort;
	D3DXMatrixIdentity(&matWorld);
	pDevice->GetViewport(&ViewPort);
	D3DXVec3Project(pvOut, &pvPos, &ViewPort, &matProj, &matView, &matWorld);

	if (pvOut->z >= 1 || pvOut->z < 0)
	{
		return false;

	}

	return true;
}


float getDistance(D3DXVECTOR3  me, D3DXVECTOR3 you) {

	float tempx, tempy, tempz, tempA;

	tempx = (me.x - you.x) * (me.x - you.x);
	tempy = (me.y - you.y) * (me.y - you.y);
	tempz = (me.z - you.z) * (me.z - you.z);

	tempA = tempx + tempy + tempz;

	if (tempA < 0)
		tempA = (tempA * (-1));

	return sqrt(tempA);
}



class CBone {

public:
	char* m_BoneName;       //0x00  
	char        pad_04[136];      //0x04
	D3DXMATRIX  BoneMatrix;       //0x8C  
	D3DXMATRIX  SomeMatrix;       //0xCC  
};

class CBoneInfo {

public:
	char       pad_00[20]; //new 0x20 v2-v1 0x24
	DWORD      oneNum;            //0x14 
	CBone* pBones;            //0x18
};

class CSkeleton {

public:
	char       pad_00[CSkeleton_off];
	CBoneInfo* BonePtr;           //0x28 
};

class CGameWorld
{
public:
	char _0x0000[8];
	__int32 m_Counter;
	char _0x000C[32];
	DWORD m_MaxObjects;
	char _0x0030[20];
	DWORD m_ObjectList;

	DWORD GetMaxObjects()
	{
		return m_MaxObjects ^ XOR_OBJLIST_MAX;
	}

	DWORD GetObject(int id)
	{
		if (/* m_MaxObjects && m_ObjectList*/ GetMaxObjects() > id && id > 0)
		{
			return 	(m_ObjectList ^ XOR_OBJLIST_PTR) + (id * 4);
		}
	}
};