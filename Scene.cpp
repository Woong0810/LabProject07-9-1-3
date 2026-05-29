//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

static const wchar_t *BGM_ALIAS = L"GameBgm";
static const wchar_t *TITLE_SCREEN_BGM_PATH = L"Sound\\title_screen_bgm.mp3";
static const wchar_t *STAGE_1_BGM_PATH = L"Sound\\stage_1_bgm.mp3";
static const wchar_t *STAGE_2_BGM_PATH = L"Sound\\stage_2_bgm.mp3";
static const wchar_t *SHOOT_SOUND_EFFECT_PATH = L"Sound\\shoot_sound_effect.wav";
static const wchar_t *HIT_SOUND_EFFECT_PATH = L"Sound\\hit_sound_effect.wav";

static void StopBgm()
{
	wchar_t pstrCommand[128];
	swprintf_s(pstrCommand, L"stop %s", BGM_ALIAS);
	mciSendString(pstrCommand, NULL, 0, NULL);
	swprintf_s(pstrCommand, L"close %s", BGM_ALIAS);
	mciSendString(pstrCommand, NULL, 0, NULL);
}

static void PlayBgm(const wchar_t *pstrFilePath)
{
	StopBgm();

	wchar_t pstrCommand[512];
	swprintf_s(pstrCommand, L"open \"%s\" type mpegvideo alias %s", pstrFilePath, BGM_ALIAS);
	if (mciSendString(pstrCommand, NULL, 0, NULL) != 0) return;

	swprintf_s(pstrCommand, L"play %s repeat", BGM_ALIAS);
	mciSendString(pstrCommand, NULL, 0, NULL);
}

static void PlaySoundEffect(const wchar_t *pstrFilePath)
{
	PlaySound(pstrFilePath, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}

CScene::CScene()
{
}

CScene::~CScene()
{
	StopBgm();
}

void CScene::StartTitleBgm()
{
	PlayBgm(TITLE_SCREEN_BGM_PATH);
}

void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 14;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.05f, 0.05f, 0.055f, 1.0f);

	m_pLights[0].m_bEnable = true;
	m_pLights[0].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.015f, 0.015f, 0.018f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.10f, 0.10f, 0.11f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.05f, 0.05f, 0.05f, 8.0f);
	m_pLights[0].m_xmf3Direction = XMFLOAT3(0.35f, -1.0f, 0.25f);

	m_pLights[1].m_bEnable = true;
	m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights[1].m_fRange = 260.0f;
	m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.02f, 0.02f, 0.02f, 1.0f);
	m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.80f, 0.82f, 0.74f, 1.0f);
	m_pLights[1].m_xmf4Specular = XMFLOAT4(0.25f, 0.25f, 0.22f, 16.0f);
	m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.004f, 0.00008f);
	m_pLights[1].m_fFalloff = 10.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(36.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(18.0f));

	const XMFLOAT3 pxmf3CeilingLightPositions[] =
	{
		XMFLOAT3(-210.0f, 43.0f, -150.0f), XMFLOAT3(-30.0f, 43.0f, -150.0f), XMFLOAT3(180.0f, 43.0f, -120.0f),
		XMFLOAT3(-210.0f, 43.0f, 60.0f), XMFLOAT3(0.0f, 43.0f, 30.0f), XMFLOAT3(180.0f, 43.0f, 120.0f),
		XMFLOAT3(-210.0f, 93.0f, -120.0f), XMFLOAT3(0.0f, 93.0f, -120.0f), XMFLOAT3(180.0f, 93.0f, -120.0f),
		XMFLOAT3(-210.0f, 93.0f, 90.0f), XMFLOAT3(0.0f, 93.0f, 90.0f), XMFLOAT3(180.0f, 93.0f, 90.0f)
	};

	for (int i = 0; i < _countof(pxmf3CeilingLightPositions); i++)
	{
		LIGHT& light = m_pLights[i + 2];
		light.m_bEnable = true;
		light.m_nType = SPOT_LIGHT;
		light.m_fRange = 170.0f;
		light.m_xmf4Ambient = XMFLOAT4(0.015f, 0.015f, 0.014f, 1.0f);
		light.m_xmf4Diffuse = XMFLOAT4(0.70f, 0.68f, 0.56f, 1.0f);
		light.m_xmf4Specular = XMFLOAT4(0.12f, 0.12f, 0.10f, 12.0f);
		light.m_xmf3Position = pxmf3CeilingLightPositions[i];
		light.m_xmf3Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
		light.m_xmf3Attenuation = XMFLOAT3(1.0f, 0.0032f, 0.00016f);
		light.m_fFalloff = 8.0f;
		light.m_fPhi = (float)cos(XMConvertToRadians(68.0f));
		light.m_fTheta = (float)cos(XMConvertToRadians(34.0f));
	}
}

struct MAZE_MAP_DESC
{
	const char *m_pFloorTiles[2];
	int m_nWidth;
	int m_nHeight;
	XMFLOAT4 m_xmf4FloorColor;
	XMFLOAT4 m_xmf4RaisedFloorColor;
	XMFLOAT4 m_xmf4CeilingColor;
	XMFLOAT4 m_xmf4WallColor;
	XMFLOAT4 m_xmf4StairColor;
	XMFLOAT4 m_xmf4DoorColor;
};

static const int MAZE_WIDTH = 23;
static const int MAZE_HEIGHT = 17;
static const int MAZE_FLOOR_COUNT = 2;
static const float MAZE_CELL_SIZE = 30.0f;
static const float MAZE_SECOND_FLOOR_HEIGHT = 50.0f;
static const float MAZE_WALL_HEIGHT = 50.0f;
static const float MAZE_DOOR_HEIGHT = 42.0f;
static const int MAZE_STAIR_STEP_COUNT = 10;
static const float PLAYER_HEIGHT_OFFSET = 8.0f;
static const float DOOR_TRIGGER_DISTANCE = 34.0f;
static const float DOOR_OPEN_SPEED = 2.8f;
static const float ENEMY_STOP_DISTANCE = 10.0f;
static const float ENEMY_RADIUS = 5.0f;
static const float ENEMY_DETECTION_DISTANCE = 210.0f;
static const float ENEMY_VIEW_HALF_ANGLE = 55.0f;
static const float ENEMY_SEARCH_DURATION = 4.0f;
static const float ENEMY_PATROL_DISTANCE = 60.0f;
static const float ENEMY_PATROL_REACH_DISTANCE = 4.0f;
static const float ENEMY_FIRE_INTERVAL = 1.6f;
static const int ENEMY_FIRE_DAMAGE = 10;
static const int PLAYER_MAX_HEALTH = 100;
static const float PLAYER_HIT_KNOCKBACK_DISTANCE = 8.0f;
static const float PLAYER_HIT_EFFECT_DURATION = 0.16f;
static const float PLAYER_RAY_SHOT_RANGE = 500.0f;
static const float PLAYER_RAY_SHOT_RADIUS = 10.0f;
static const float PLAYER_RAY_SHOT_HEIGHT = 18.0f;
static const float PLAYER_SHOT_EFFECT_DURATION = 0.05f;
static const float MAZE_FLOOR_THICKNESS = 2.0f;
static const float MAZE_FLOOR_SURFACE_GAP = 0.08f;
static const float MAZE_CEILING_THICKNESS = 2.0f;
static const int CEILING_LIGHT_START_INDEX = 2;
static const int CEILING_LIGHT_COUNT = 12;

static const char g_pStage1Floor0Map[] =
	"#######################"
	"#.....#.....D.........#"
	"#.....#.....#.........#"
	"#..S..D.....#.........#"
	"#.....#.....#####D#####"
	"###D#########.........#"
	"#.........#...........#"
	"#.........#...........#"
	"#.........####D########"
	"#####D#####.....#.....#"
	"#.........#.....#.....#"
	"#.........#.....#.....#"
	"###########.....#.....#"
	"#.......D.......D.....#"
	"#.......#.......###^###"
	"#.......#.......#######"
	"#######################";

static const char g_pStage1Floor1Map[] =
	"#######################"
	"#.............D.......#"
	"#.............#.......#"
	"###############.......#"
	"#.............#.......#"
	"#.............#.......#"
	"#.............###D#####"
	"#.............#.......#"
	"#.............D.......#"
	"#.............###D#####"
	"#.............#.......#"
	"#.............#.......#"
	"#.............#.......#"
	"#####D#########.... ..#"
	"#.................. ..#"
	"#.....................#"
	"#######################";

static const char g_pStage2Floor0Map[] =
	"#######################"
	"#.......D........D....#"
	"#...#####..#..####....#"
	"#########..#.....######"
	"#.......D.##........###"
	"#.......##............#"
	"##D######....###......#"
	"#..........##...###D###"
	"#.........#.....#.....#"
	"#..##..####.....D.....#"
	"#..##..#......###.....#"
	"#..###D###......#.....#"
	"#..#.....#......##....#"
	"#..#.....###.....###..#"
	"#..D...........^.#....#"
	"#..#.............#....#"
	"#######################";

static const char g_pStage2Floor1Map[] =
	"#######################"
	"#.....########.......##"
	"#..S..D..............##"
	"#.....#######.......###"
	"###D#############...###"
	"##.....D......##....###"
	"##..#..########.....###"
	"##..#..####.........###"
	"##....................#"
	"#####D#########D#######"
	"#.........#.......#...#"
	"#.........#.......#...#"
	"##D########...........#"
	"#...#.....D.#...#.....#"
	"#...#...#####.. #.....#"
	"#.......##......D.....#"
	"#######################";

static const MAZE_MAP_DESC g_pMazeMaps[] =
{
	{ { g_pStage1Floor0Map, g_pStage1Floor1Map }, MAZE_WIDTH, MAZE_HEIGHT, XMFLOAT4(0.78f, 0.78f, 0.76f, 1.0f), XMFLOAT4(0.78f, 0.78f, 0.76f, 1.0f), XMFLOAT4(0.64f, 0.64f, 0.62f, 1.0f), XMFLOAT4(0.05f, 0.18f, 0.82f, 1.0f), XMFLOAT4(0.95f, 0.78f, 0.08f, 1.0f), XMFLOAT4(0.00f, 0.88f, 0.82f, 1.0f) },
	{ { g_pStage2Floor0Map, g_pStage2Floor1Map }, MAZE_WIDTH, MAZE_HEIGHT, XMFLOAT4(0.94f, 0.94f, 0.90f, 1.0f), XMFLOAT4(0.94f, 0.94f, 0.90f, 1.0f), XMFLOAT4(0.78f, 0.75f, 0.68f, 1.0f), XMFLOAT4(0.70f, 0.48f, 0.30f, 1.0f), XMFLOAT4(0.96f, 0.62f, 0.10f, 1.0f), XMFLOAT4(0.10f, 0.76f, 0.72f, 1.0f) }
};

static int GetStageIndexFromStageNumber(int nStage)
{
	int nStageIndex = nStage - 1;
	if (nStageIndex < 0) nStageIndex = 0;
	if (nStageIndex >= _countof(g_pMazeMaps)) nStageIndex = _countof(g_pMazeMaps) - 1;
	return(nStageIndex);
}

static void ApplyStageCeilingSpotLights(LIGHT *pLights, int nStageIndex)
{
	if (!pLights) return;

	static const XMFLOAT3 pxmf3StageCeilingLightPositions[][CEILING_LIGHT_COUNT] =
	{
		{
			XMFLOAT3(-210.0f, 43.0f, -150.0f), XMFLOAT3(-30.0f, 43.0f, -150.0f), XMFLOAT3(180.0f, 43.0f, -120.0f),
			XMFLOAT3(-210.0f, 43.0f, 60.0f), XMFLOAT3(0.0f, 43.0f, 30.0f), XMFLOAT3(180.0f, 43.0f, 120.0f),
			XMFLOAT3(-210.0f, 93.0f, -120.0f), XMFLOAT3(0.0f, 93.0f, -120.0f), XMFLOAT3(180.0f, 93.0f, -120.0f),
			XMFLOAT3(-210.0f, 93.0f, 90.0f), XMFLOAT3(0.0f, 93.0f, 90.0f), XMFLOAT3(180.0f, 93.0f, 90.0f)
		},
		{
			XMFLOAT3(-210.0f, 43.0f, -150.0f), XMFLOAT3(0.0f, 43.0f, -150.0f), XMFLOAT3(210.0f, 43.0f, -150.0f),
			XMFLOAT3(-180.0f, 43.0f, 0.0f), XMFLOAT3(30.0f, 43.0f, 30.0f), XMFLOAT3(210.0f, 43.0f, 120.0f),
			XMFLOAT3(-210.0f, 93.0f, -180.0f), XMFLOAT3(30.0f, 93.0f, -150.0f), XMFLOAT3(210.0f, 93.0f, -120.0f),
			XMFLOAT3(-210.0f, 93.0f, 60.0f), XMFLOAT3(0.0f, 93.0f, 120.0f), XMFLOAT3(210.0f, 93.0f, 180.0f)
		}
	};

	if (nStageIndex < 0) nStageIndex = 0;
	if (nStageIndex >= _countof(pxmf3StageCeilingLightPositions)) nStageIndex = _countof(pxmf3StageCeilingLightPositions) - 1;

	for (int i = 0; i < CEILING_LIGHT_COUNT; i++)
	{
		LIGHT& light = pLights[CEILING_LIGHT_START_INDEX + i];
		light.m_bEnable = true;
		light.m_nType = SPOT_LIGHT;
		light.m_fRange = 170.0f;
		light.m_xmf4Ambient = XMFLOAT4(0.015f, 0.015f, 0.014f, 1.0f);
		light.m_xmf4Diffuse = XMFLOAT4(0.70f, 0.68f, 0.56f, 1.0f);
		light.m_xmf4Specular = XMFLOAT4(0.12f, 0.12f, 0.10f, 12.0f);
		light.m_xmf3Position = pxmf3StageCeilingLightPositions[nStageIndex][i];
		light.m_xmf3Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
		light.m_xmf3Attenuation = XMFLOAT3(1.0f, 0.0032f, 0.00016f);
		light.m_fFalloff = 8.0f;
		light.m_fPhi = (float)cos(XMConvertToRadians(68.0f));
		light.m_fTheta = (float)cos(XMConvertToRadians(34.0f));
	}
}

static XMFLOAT3 GetMazeCellPosition(int x, int z, int width, int height, float y)
{
	return(XMFLOAT3((x - (width - 1) * 0.5f) * MAZE_CELL_SIZE, y, (z - (height - 1) * 0.5f) * MAZE_CELL_SIZE));
}

static int WorldToMazeCellX(float x, const MAZE_MAP_DESC& map)
{
	return((int)floorf((x / MAZE_CELL_SIZE) + ((map.m_nWidth - 1) * 0.5f) + 0.5f));
}

static int WorldToMazeCellZ(float z, const MAZE_MAP_DESC& map)
{
	return((int)floorf((z / MAZE_CELL_SIZE) + ((map.m_nHeight - 1) * 0.5f) + 0.5f));
}

static bool IsInsideMazeCell(int x, int z, const MAZE_MAP_DESC& map)
{
	return((x >= 0) && (x < map.m_nWidth) && (z >= 0) && (z < map.m_nHeight));
}

static char GetMazeTileAtCell(const MAZE_MAP_DESC& map, int floor, int x, int z)
{
	if ((floor < 0) || (floor >= MAZE_FLOOR_COUNT)) return('#');
	if (!IsInsideMazeCell(x, z, map)) return('#');
	return(map.m_pFloorTiles[floor][z * map.m_nWidth + x]);
}

static char GetMazeTileAtWorld(float x, float z, int floor, const MAZE_MAP_DESC& map)
{
	int nCellX = WorldToMazeCellX(x, map);
	int nCellZ = WorldToMazeCellZ(z, map);
	return(GetMazeTileAtCell(map, floor, nCellX, nCellZ));
}

static bool IsBlockingTile(char tile)
{
	return(tile == '#');
}

static bool HasSecondFloorTile(char tile)
{
	return(tile != ' ');
}

static int GetMazeFloorIndexFromWorldY(float y)
{
	return(y >= (MAZE_SECOND_FLOOR_HEIGHT * 0.75f)) ? 1 : 0;
}

static bool IsHorizontalDoor(const MAZE_MAP_DESC& map, int floor, int x, int z)
{
	bool bLeftRightWall = (GetMazeTileAtCell(map, floor, x - 1, z) == '#') && (GetMazeTileAtCell(map, floor, x + 1, z) == '#');
	bool bFrontBackWall = (GetMazeTileAtCell(map, floor, x, z - 1) == '#') && (GetMazeTileAtCell(map, floor, x, z + 1) == '#');
	return(bLeftRightWall || !bFrontBackWall);
}

static bool IsDoorOpenAtCell(const std::vector<DOOR_OBJECT>& doors, int stage, int floor, int x, int z)
{
	for (size_t i = 0; i < doors.size(); i++)
	{
		if ((doors[i].m_nStage == stage) && (doors[i].m_nFloor == floor) && (doors[i].m_nCellX == x) && (doors[i].m_nCellZ == z))
		{
			return(doors[i].m_fOpenAmount > 0.78f);
		}
	}
	return(false);
}

static bool IsBlockedAtWorld(float x, float z, float y, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors, int stage)
{
	int nFloor = GetMazeFloorIndexFromWorldY(y);
	int nCellX = WorldToMazeCellX(x, map);
	int nCellZ = WorldToMazeCellZ(z, map);
	if (!IsInsideMazeCell(nCellX, nCellZ, map)) return(true);

	char baseTile = GetMazeTileAtCell(map, 0, nCellX, nCellZ);
	if (baseTile == '^') return(false);

	char tile = GetMazeTileAtCell(map, nFloor, nCellX, nCellZ);
	if ((nFloor == 1) && !HasSecondFloorTile(tile)) return(true);
	if (IsBlockingTile(tile)) return(true);
	if ((tile == 'D') && !IsDoorOpenAtCell(doors, stage, nFloor, nCellX, nCellZ)) return(true);
	return(false);
}

static float GetStairFloorHeight(float z, int nCellX, int nCellZ, const MAZE_MAP_DESC& map)
{
	XMFLOAT3 xmf3CellCenter = GetMazeCellPosition(nCellX, nCellZ, map.m_nWidth, map.m_nHeight, 0.0f);
	float fLocalZ = (z - (xmf3CellCenter.z - (MAZE_CELL_SIZE * 0.5f))) / MAZE_CELL_SIZE;
	if (fLocalZ < 0.0f) fLocalZ = 0.0f;
	if (fLocalZ > 1.0f) fLocalZ = 1.0f;
	return(fLocalZ * MAZE_SECOND_FLOOR_HEIGHT);
}

static float GetMazeFloorHeight(float x, float z, float y, const MAZE_MAP_DESC& map)
{
	int nCellX = WorldToMazeCellX(x, map);
	int nCellZ = WorldToMazeCellZ(z, map);
	if (!IsInsideMazeCell(nCellX, nCellZ, map)) return(0.0f);

	char baseTile = GetMazeTileAtCell(map, 0, nCellX, nCellZ);
	if (baseTile == '^') return(GetStairFloorHeight(z, nCellX, nCellZ, map));

	int nFloor = GetMazeFloorIndexFromWorldY(y);
	if ((nFloor == 1) && HasSecondFloorTile(GetMazeTileAtCell(map, 1, nCellX, nCellZ))) return(MAZE_SECOND_FLOOR_HEIGHT);

	return(0.0f);
}

static XMFLOAT3 FindMazeSpawnPosition(const MAZE_MAP_DESC& map)
{
	for (int floor = 0; floor < MAZE_FLOOR_COUNT; floor++)
	{
		for (int z = 0; z < map.m_nHeight; z++)
		{
			for (int x = 0; x < map.m_nWidth; x++)
			{
				if (GetMazeTileAtCell(map, floor, x, z) == 'S')
				{
					float fFloorHeight = (floor == 1) ? MAZE_SECOND_FLOOR_HEIGHT : 0.0f;
					XMFLOAT3 xmf3SpawnPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, fFloorHeight);
					xmf3SpawnPosition.y = fFloorHeight + PLAYER_HEIGHT_OFFSET;
					return(xmf3SpawnPosition);
				}
			}
		}
	}
	return(XMFLOAT3(-200.0f, PLAYER_HEIGHT_OFFSET, -140.0f));
}
static CGameObject *CreateColoredBoxObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT3& xmf3Position, const XMFLOAT3& xmf3Scale, const XMFLOAT4& xmf4Color)
{
	CGameObject *pObject = new CGameObject();
	pObject->m_nMeshInHierarchy = 0;
	pObject->SetMesh(new CBoxMesh(pd3dDevice, pd3dCommandList));
	pObject->m_nMaterials = 1;
	pObject->m_ppMaterials = new CMaterial*[1];
	pObject->m_ppMaterials[0] = NULL;

	CMaterial *pMaterial = new CMaterial();
	CMaterialColors *pMaterialColors = new CMaterialColors();
	pMaterialColors->m_xmf4Ambient = XMFLOAT4(xmf4Color.x * 0.45f, xmf4Color.y * 0.45f, xmf4Color.z * 0.45f, 1.0f);
	pMaterialColors->m_xmf4Diffuse = xmf4Color;
	pMaterialColors->m_xmf4Specular = XMFLOAT4(0.12f, 0.12f, 0.12f, 8.0f);
	pMaterialColors->m_xmf4Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pMaterial->SetMaterialColors(pMaterialColors);
	pMaterial->SetIlluminatedShader();
	pObject->SetMaterial(0, pMaterial);

	pObject->SetScale(xmf3Scale.x, xmf3Scale.y, xmf3Scale.z);
	pObject->SetPosition(xmf3Position);

	int pnMaterialsInHierarchy[1] = { 1 };
	pObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, 1, pnMaterialsInHierarchy);

	return(pObject);
}
static CGameObject *CreateLoadedModelObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, char *pstrFileName, const XMFLOAT3& xmf3Position, const XMFLOAT3& xmf3Rotation)
{
	int nMeshesInHierarchy = 0;
	int pnMaterialsInHierarchy[64] = { 0 };
	CGameObject *pObject = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pstrFileName, &nMeshesInHierarchy, pnMaterialsInHierarchy);
	if (!pObject) return(NULL);
	pObject->Rotate(xmf3Rotation.x, xmf3Rotation.y, xmf3Rotation.z);
	pObject->SetPosition(xmf3Position);
	pObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	return(pObject);
}

static void SetObjectMaterialColorRecursive(CGameObject *pObject, const XMFLOAT4& xmf4Color)
{
	if (!pObject) return;

	for (int i = 0; i < pObject->m_nMaterials; i++)
	{
		if (pObject->m_ppMaterials && pObject->m_ppMaterials[i] && pObject->m_ppMaterials[i]->m_pMaterialColors)
		{
			CMaterialColors *pMaterialColors = pObject->m_ppMaterials[i]->m_pMaterialColors;
			pMaterialColors->m_xmf4Ambient = XMFLOAT4(xmf4Color.x * 0.35f, xmf4Color.y * 0.35f, xmf4Color.z * 0.35f, 1.0f);
			pMaterialColors->m_xmf4Diffuse = xmf4Color;
			pMaterialColors->m_xmf4Specular = XMFLOAT4(0.14f, 0.14f, 0.14f, 16.0f);
			pMaterialColors->m_xmf4Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	SetObjectMaterialColorRecursive(pObject->m_pChild, xmf4Color);
	SetObjectMaterialColorRecursive(pObject->m_pSibling, xmf4Color);
}

static CGameObject *CreateArmedCharacterObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, char *pstrCharacterFileName, const XMFLOAT3& xmf3Position, const XMFLOAT3& xmf3Rotation)
{
	int nCharacterMeshesInHierarchy = 0;
	int pnMaterialsInHierarchy[64] = { 0 };
	CGameObject *pCharacterObject = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pstrCharacterFileName, &nCharacterMeshesInHierarchy, pnMaterialsInHierarchy);
	if (!pCharacterObject) return(NULL);

	SetObjectMaterialColorRecursive(pCharacterObject, XMFLOAT4(0.86f, 0.05f, 0.04f, 1.0f));
	pCharacterObject->Rotate(xmf3Rotation.x, xmf3Rotation.y, xmf3Rotation.z);
	pCharacterObject->SetPosition(xmf3Position);
	pCharacterObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nCharacterMeshesInHierarchy, pnMaterialsInHierarchy);
	return(pCharacterObject);
}

static void SetMenuTextTransform(CGameObject *pTextObject, const XMFLOAT3& xmf3Position, float fScale, const XMFLOAT3& xmf3Rotation)
{
	if (!pTextObject) return;

	XMMATRIX xmmtxText = XMMatrixScaling(fScale, fScale, fScale) *
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(xmf3Rotation.x), XMConvertToRadians(xmf3Rotation.y), XMConvertToRadians(xmf3Rotation.z)) *
		XMMatrixTranslation(xmf3Position.x, xmf3Position.y, xmf3Position.z);
	XMStoreFloat4x4(&pTextObject->m_xmf4x4Transform, xmmtxText);
	pTextObject->UpdateTransform(NULL);
}

static const XMFLOAT3 MENU_NAME_POSITION = XMFLOAT3(0.0f, -40.0f, 0.0f);
static const XMFLOAT3 MENU_NAME_ROTATION = XMFLOAT3(-90.0f, 170.0f, -8.0f);
static const float MENU_NAME_SCALE = 300.0f;

static const XMFLOAT3 MENU_START_POSITION = XMFLOAT3(0.0f, 40.0f, 0.0f);
static const XMFLOAT3 MENU_START_ROTATION = XMFLOAT3(-90.0f, 150.0f, 15.0f);
static const float MENU_START_SCALE = 700.0f;

static const XMFLOAT3 MENU_STAGE1_POSITION = XMFLOAT3(0.0f, 40.0f, 0.0f);
static const XMFLOAT3 MENU_STAGE1_ROTATION = XMFLOAT3(-90.0f, 160.0f, 0.0f);
static const float MENU_STAGE_SCALE = 500.0f;

static const XMFLOAT3 MENU_STAGE2_POSITION = XMFLOAT3(0.0f, -40.0f, 0.0f);
static const XMFLOAT3 MENU_STAGE2_ROTATION = XMFLOAT3(0.0f, 160.0f, 0.0f);

static const RECT MENU_START_HIT_BOX = { 350, 60, 930, 300 };
static const RECT MENU_STAGE1_HIT_BOX = { 350, 150, 930, 300 };
static const RECT MENU_STAGE2_HIT_BOX = { 350, 400, 930, 600 };

static bool IsPointInScreenBox(int x, int y, const RECT& rect)
{
	return((x >= rect.left) && (x <= rect.right) && (y >= rect.top) && (y <= rect.bottom));
}

static void UpdateMenuTextMotion(CGameObject *pTextObject, const XMFLOAT3& xmf3BasePosition, float fScale, const XMFLOAT3& xmf3BaseRotation, bool bHovered, float fElapsedTime, float fPhase)
{
	XMFLOAT3 xmf3Position = xmf3BasePosition;
	xmf3Position.y += sinf((fElapsedTime * 2.2f) + fPhase) * 4.0f;

	XMFLOAT3 xmf3Rotation = xmf3BaseRotation;
	if (bHovered) xmf3Rotation.y += fmodf(fElapsedTime * 180.0f, 360.0f);

	SetMenuTextTransform(pTextObject, xmf3Position, fScale, xmf3Rotation);
}

static CGameObject *CreateMenuTextObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, char *pstrFileName, const XMFLOAT3& xmf3Position, float fScale, const XMFLOAT3& xmf3Rotation, const XMFLOAT4& xmf4Color)
{
	int nMeshesInHierarchy = 0;
	int pnMaterialsInHierarchy[64] = { 0 };
	CGameObject *pTextObject = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pstrFileName, &nMeshesInHierarchy, pnMaterialsInHierarchy);
	if (!pTextObject) return(NULL);

	SetObjectMaterialColorRecursive(pTextObject, xmf4Color);
	SetMenuTextTransform(pTextObject, xmf3Position, fScale, xmf3Rotation);
	pTextObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	return(pTextObject);
}

static float DistanceXZ(const XMFLOAT3& a, const XMFLOAT3& b)
{
	float dx = a.x - b.x;
	float dz = a.z - b.z;
	return(sqrtf((dx * dx) + (dz * dz)));
}

static void SetObjectLookDirectionXZ(CGameObject *pObject, const XMFLOAT3& xmf3Position, const XMFLOAT3& xmf3Look)
{
	if (!pObject) return;

	XMFLOAT3 xmf3FlatLook = Vector3::Normalize(XMFLOAT3(xmf3Look.x, 0.0f, xmf3Look.z));
	XMFLOAT3 xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 xmf3Right = Vector3::CrossProduct(xmf3Up, xmf3FlatLook, true);

	pObject->m_xmf4x4Transform._11 = xmf3Right.x; pObject->m_xmf4x4Transform._12 = xmf3Right.y; pObject->m_xmf4x4Transform._13 = xmf3Right.z;
	pObject->m_xmf4x4Transform._21 = xmf3Up.x; pObject->m_xmf4x4Transform._22 = xmf3Up.y; pObject->m_xmf4x4Transform._23 = xmf3Up.z;
	pObject->m_xmf4x4Transform._31 = xmf3FlatLook.x; pObject->m_xmf4x4Transform._32 = xmf3FlatLook.y; pObject->m_xmf4x4Transform._33 = xmf3FlatLook.z;
	pObject->m_xmf4x4Transform._41 = xmf3Position.x; pObject->m_xmf4x4Transform._42 = xmf3Position.y; pObject->m_xmf4x4Transform._43 = xmf3Position.z;
	pObject->UpdateTransform(NULL);
}

static bool IsEnemyBlockedAtWorld(float x, float z, float y, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors, int stage)
{
	return(IsBlockedAtWorld(x - ENEMY_RADIUS, z - ENEMY_RADIUS, y, map, doors, stage) ||
		IsBlockedAtWorld(x + ENEMY_RADIUS, z - ENEMY_RADIUS, y, map, doors, stage) ||
		IsBlockedAtWorld(x - ENEMY_RADIUS, z + ENEMY_RADIUS, y, map, doors, stage) ||
		IsBlockedAtWorld(x + ENEMY_RADIUS, z + ENEMY_RADIUS, y, map, doors, stage));
}

static bool HasLineOfSightToPlayer(const XMFLOAT3& xmf3EnemyPosition, const XMFLOAT3& xmf3PlayerPosition, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors, int stage)
{
	float fDistance = DistanceXZ(xmf3EnemyPosition, xmf3PlayerPosition);
	int nSteps = (int)(fDistance / (MAZE_CELL_SIZE * 0.35f));
	if (nSteps < 2) return(true);

	for (int i = 1; i < nSteps; i++)
	{
		float t = (float)i / (float)nSteps;
		float x = xmf3EnemyPosition.x + ((xmf3PlayerPosition.x - xmf3EnemyPosition.x) * t);
		float z = xmf3EnemyPosition.z + ((xmf3PlayerPosition.z - xmf3EnemyPosition.z) * t);
		if (IsBlockedAtWorld(x, z, xmf3EnemyPosition.y, map, doors, stage)) return(false);
	}
	return(true);
}

static bool RayIntersectsSphere(const XMFLOAT3& xmf3RayOrigin, const XMFLOAT3& xmf3RayDirection, const XMFLOAT3& xmf3SphereCenter, float fSphereRadius, float fMaxDistance, float& fHitDistance)
{
	XMVECTOR xmvRayOrigin = XMLoadFloat3(&xmf3RayOrigin);
	XMVECTOR xmvRayDirection = XMVector3Normalize(XMLoadFloat3(&xmf3RayDirection));
	XMVECTOR xmvSphereCenter = XMLoadFloat3(&xmf3SphereCenter);
	XMVECTOR xmvOriginToCenter = xmvSphereCenter - xmvRayOrigin;

	float fProjectedDistance = XMVectorGetX(XMVector3Dot(xmvOriginToCenter, xmvRayDirection));
	if (fProjectedDistance < 0.0f) return(false);

	float fDistanceSqToRay = XMVectorGetX(XMVector3LengthSq(xmvOriginToCenter)) - (fProjectedDistance * fProjectedDistance);
	float fRadiusSq = fSphereRadius * fSphereRadius;
	if (fDistanceSqToRay > fRadiusSq) return(false);

	float fHalfChord = sqrtf(fRadiusSq - fDistanceSqToRay);
	fHitDistance = fProjectedDistance - fHalfChord;
	if (fHitDistance < 0.0f) fHitDistance = fProjectedDistance + fHalfChord;

	return((fHitDistance >= 0.0f) && (fHitDistance <= fMaxDistance));
}

static bool IsRayBlockedBeforeDistance(const XMFLOAT3& xmf3RayOrigin, const XMFLOAT3& xmf3RayDirection, float fHitDistance, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors, int stage)
{
	int nSteps = (int)(fHitDistance / (MAZE_CELL_SIZE * 0.25f));
	if (nSteps < 2) return(false);

	for (int i = 1; i < nSteps; i++)
	{
		float t = (float)i / (float)nSteps;
		float x = xmf3RayOrigin.x + (xmf3RayDirection.x * fHitDistance * t);
		float y = xmf3RayOrigin.y + (xmf3RayDirection.y * fHitDistance * t);
		float z = xmf3RayOrigin.z + (xmf3RayDirection.z * fHitDistance * t);
		if (IsBlockedAtWorld(x, z, y, map, doors, stage)) return(true);
	}
	return(false);
}

static bool CanEnemySeePlayer(const ENEMY_OBJECT& enemy, const XMFLOAT3& xmf3EnemyPosition, const XMFLOAT3& xmf3PlayerPosition, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors, int stage)
{
	if (!enemy.m_pObject) return(false);
	if (GetMazeFloorIndexFromWorldY(xmf3EnemyPosition.y) != GetMazeFloorIndexFromWorldY(xmf3PlayerPosition.y)) return(false);

	float fDistance = DistanceXZ(xmf3EnemyPosition, xmf3PlayerPosition);
	if (fDistance > ENEMY_DETECTION_DISTANCE) return(false);

	XMFLOAT3 xmf3DirectionToPlayer = XMFLOAT3(xmf3PlayerPosition.x - xmf3EnemyPosition.x, 0.0f, xmf3PlayerPosition.z - xmf3EnemyPosition.z);
	if (Vector3::Length(xmf3DirectionToPlayer) < 0.001f) return(true);
	xmf3DirectionToPlayer = Vector3::Normalize(xmf3DirectionToPlayer);

	XMFLOAT3 xmf3EnemyLook = enemy.m_pObject->GetLook();
	xmf3EnemyLook = Vector3::Normalize(XMFLOAT3(xmf3EnemyLook.x, 0.0f, xmf3EnemyLook.z));
	float fDot = Vector3::DotProduct(xmf3EnemyLook, xmf3DirectionToPlayer);
	float fMinDot = cosf(XMConvertToRadians(ENEMY_VIEW_HALF_ANGLE));
	if (fDot < fMinDot) return(false);

	return(HasLineOfSightToPlayer(xmf3EnemyPosition, xmf3PlayerPosition, map, doors, stage));
}

static bool MoveEnemyToward(ENEMY_OBJECT& enemy, const XMFLOAT3& xmf3TargetPosition, float fTimeElapsed, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors, int stage, float fStopDistance)
{
	if (!enemy.m_pObject) return(false);

	XMFLOAT3 xmf3EnemyPosition = enemy.m_pObject->GetPosition();
	XMFLOAT3 xmf3Direction = XMFLOAT3(xmf3TargetPosition.x - xmf3EnemyPosition.x, 0.0f, xmf3TargetPosition.z - xmf3EnemyPosition.z);
	float fDistance = Vector3::Length(xmf3Direction);
	if (fDistance <= fStopDistance) return(true);

	xmf3Direction = Vector3::Normalize(xmf3Direction);
	float fMoveDistance = enemy.m_fMoveSpeed * fTimeElapsed;
	if (fMoveDistance > (fDistance - fStopDistance)) fMoveDistance = fDistance - fStopDistance;

	XMFLOAT3 xmf3ResolvedPosition = xmf3EnemyPosition;
	xmf3ResolvedPosition.x += xmf3Direction.x * fMoveDistance;
	if (IsEnemyBlockedAtWorld(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3EnemyPosition.y, map, doors, stage)) xmf3ResolvedPosition.x = xmf3EnemyPosition.x;

	xmf3ResolvedPosition.z += xmf3Direction.z * fMoveDistance;
	if (IsEnemyBlockedAtWorld(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3EnemyPosition.y, map, doors, stage)) xmf3ResolvedPosition.z = xmf3EnemyPosition.z;

	xmf3ResolvedPosition.y = GetMazeFloorHeight(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3EnemyPosition.y, map);
	SetObjectLookDirectionXZ(enemy.m_pObject, xmf3ResolvedPosition, xmf3Direction);
	return(DistanceXZ(xmf3ResolvedPosition, xmf3TargetPosition) <= fStopDistance);
}

bool CScene::IsPlayerBlockedAtWorld(float x, float z, float y)
{
	const MAZE_MAP_DESC& map = g_pMazeMaps[GetStageIndexFromStageNumber(m_nSelectedStage)];
	const float fPlayerRadius = 6.0f;
	const float px[4] = { x - fPlayerRadius, x + fPlayerRadius, x - fPlayerRadius, x + fPlayerRadius };
	const float pz[4] = { z - fPlayerRadius, z - fPlayerRadius, z + fPlayerRadius, z + fPlayerRadius };

	for (int i = 0; i < 4; i++)
	{
		if (IsBlockedAtWorld(px[i], pz[i], y, map, m_vDoors, GetStageIndexFromStageNumber(m_nSelectedStage))) return(true);
	}
	return(false);
}

void CScene::BeginStage(int nStage)
{
	m_nSelectedStage = nStage;
	m_nPlayerHealth = PLAYER_MAX_HEALTH;
	m_bGameOver = false;
	m_fHitEffectTime = 0.0f;
	m_nScreenMode = SCENE_SCREEN_PLAYING;
	PlayBgm((m_nSelectedStage == 2) ? STAGE_2_BGM_PATH : STAGE_1_BGM_PATH);
	if (m_pPlayer)
	{
		const MAZE_MAP_DESC& map = g_pMazeMaps[GetStageIndexFromStageNumber(m_nSelectedStage)];
		m_pPlayer->SetPosition(FindMazeSpawnPosition(map));
	}
}

void CScene::DamagePlayer(int nDamage, const XMFLOAT3& xmf3HitDirection)
{
	if ((m_nScreenMode != SCENE_SCREEN_PLAYING) || m_bGameOver) return;
	if (m_bDebugNoDamage) return;

	PlaySoundEffect(HIT_SOUND_EFFECT_PATH);
	m_fHitEffectTime = PLAYER_HIT_EFFECT_DURATION;

	if (m_pPlayer)
	{
		XMFLOAT3 xmf3KnockbackDirection = XMFLOAT3(xmf3HitDirection.x, 0.0f, xmf3HitDirection.z);
		if (Vector3::Length(xmf3KnockbackDirection) > 0.001f)
		{
			xmf3KnockbackDirection = Vector3::Normalize(xmf3KnockbackDirection);
			XMFLOAT3 xmf3OldPosition = m_pPlayer->GetPosition();
			m_pPlayer->Move(Vector3::ScalarProduct(xmf3KnockbackDirection, PLAYER_HIT_KNOCKBACK_DISTANCE, false), false);
			ResolvePlayerCollision(m_pPlayer, xmf3OldPosition, false);
		}
	}

	m_nPlayerHealth -= nDamage;
	if (m_nPlayerHealth <= 0)
	{
		m_nPlayerHealth = 0;
		m_bGameOver = true;
	}
}

void CScene::HealPlayer()
{
	if (m_nScreenMode != SCENE_SCREEN_PLAYING) return;

	m_nPlayerHealth = PLAYER_MAX_HEALTH;
	m_fHitEffectTime = 0.0f;
}

bool CScene::AreAllEnemiesKilledInStage() const
{
	int nStageIndex = GetStageIndexFromStageNumber(m_nSelectedStage);
	bool bHasEnemyInStage = false;

	for (size_t i = 0; i < m_vEnemies.size(); i++)
	{
		const ENEMY_OBJECT& enemy = m_vEnemies[i];
		if (enemy.m_nStage != nStageIndex) continue;

		bHasEnemyInStage = true;
		if (enemy.m_bAlive) return(false);
	}
	return(bHasEnemyInStage);
}

void CScene::KillAllEnemiesInStage()
{
	if (m_nScreenMode != SCENE_SCREEN_PLAYING) return;

	int nStageIndex = GetStageIndexFromStageNumber(m_nSelectedStage);
	for (size_t i = 0; i < m_vEnemies.size(); i++)
	{
		ENEMY_OBJECT& enemy = m_vEnemies[i];
		if ((enemy.m_nStage != nStageIndex) || !enemy.m_bAlive || !enemy.m_pObject) continue;

		enemy.m_bAlive = false;
		enemy.m_nHealth = 0;
		SetObjectMaterialColorRecursive(enemy.m_pObject, XMFLOAT4(0.08f, 0.08f, 0.08f, 1.0f));

		XMFLOAT3 xmf3EnemyPosition = enemy.m_pObject->GetPosition();
		xmf3EnemyPosition.y -= 1000.0f;
		enemy.m_pObject->SetPosition(xmf3EnemyPosition);
	}
	m_bGameOver = true;
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	BuildDefaultLightsAndMaterials();

	m_vDoors.clear();
	m_vEnemies.clear();

	std::vector<CGameObject*> ppObjects;
	const int pnStageEnemyCounts[] = { 13, 12 };
	const int ppnStageEnemyCells[][13][3] =
	{
		{
			{ 16, 2, 0 }, { 7, 7, 0 }, { 18, 10, 1 }, { 19, 2, 0 }, { 18, 6, 0 },
			{ 3, 14, 0 }, { 4, 8, 1 }, { 18, 3, 1 }, { 8, 12, 1 }, { 6, 14, 1 },
			{ 4, 4, 1 }, { 3, 9, 1 }, { 11, 11, 1 }
		},
		{
			{ 4, 1, 0 }, { 14, 1, 0 }, { 5, 4, 0 }, { 15, 5, 0 }, { 6, 8, 0 },
			{ 13, 11, 0 }, { 17, 2, 1 }, { 4, 5, 1 }, { 11, 5, 1 }, { 15, 8, 1 },
			{ 6, 11, 1 }, { 19, 15, 1 }, { 0, 0, 0 }
		}
	};

	for (int stage = 0; stage < _countof(g_pMazeMaps); stage++)
	{
		const MAZE_MAP_DESC& map = g_pMazeMaps[stage];
		m_pnStageWorldStart[stage] = (int)ppObjects.size();

		for (int z = 0; z < map.m_nHeight; z++)
		{
			for (int x = 0; x < map.m_nWidth; x++)
			{
				XMFLOAT3 xmf3BaseFloorPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, -(MAZE_FLOOR_THICKNESS * 0.5f) - MAZE_FLOOR_SURFACE_GAP);
				ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3BaseFloorPosition, XMFLOAT3(MAZE_CELL_SIZE, MAZE_FLOOR_THICKNESS, MAZE_CELL_SIZE), map.m_xmf4FloorColor));

				for (int floor = 0; floor < MAZE_FLOOR_COUNT; floor++)
				{
					char tile = GetMazeTileAtCell(map, floor, x, z);
					if ((floor == 1) && !HasSecondFloorTile(tile)) continue;

					float fFloorHeight = (floor == 1) ? MAZE_SECOND_FLOOR_HEIGHT : 0.0f;
					if (floor == 1)
					{
						XMFLOAT3 xmf3RaisedFloorPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, fFloorHeight - (MAZE_FLOOR_THICKNESS * 0.5f) - MAZE_FLOOR_SURFACE_GAP);
						ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3RaisedFloorPosition, XMFLOAT3(MAZE_CELL_SIZE, MAZE_FLOOR_THICKNESS, MAZE_CELL_SIZE), map.m_xmf4RaisedFloorColor));
					}

					if (tile == '#')
					{
						XMFLOAT3 xmf3WallPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, fFloorHeight + (MAZE_WALL_HEIGHT * 0.5f));
						ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3WallPosition, XMFLOAT3(MAZE_CELL_SIZE, MAZE_WALL_HEIGHT, MAZE_CELL_SIZE), map.m_xmf4WallColor));
					}
					else if (tile == 'D')
					{
						bool bHorizontalDoor = IsHorizontalDoor(map, floor, x, z);
						XMFLOAT3 xmf3DoorPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, fFloorHeight + (MAZE_DOOR_HEIGHT * 0.5f));
						XMFLOAT3 xmf3DoorScale = bHorizontalDoor ? XMFLOAT3(MAZE_CELL_SIZE * 0.90f, MAZE_DOOR_HEIGHT, 4.0f) : XMFLOAT3(4.0f, MAZE_DOOR_HEIGHT, MAZE_CELL_SIZE * 0.90f);
						CGameObject *pDoorObject = CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3DoorPosition, xmf3DoorScale, map.m_xmf4DoorColor);
						ppObjects.push_back(pDoorObject);

						DOOR_OBJECT door;
						door.m_pObject = pDoorObject;
						door.m_nStage = stage;
						door.m_nCellX = x;
						door.m_nCellZ = z;
						door.m_nFloor = floor;
						door.m_bHorizontal = bHorizontalDoor;
						door.m_fFloorHeight = fFloorHeight;
						door.m_xmf3ClosedPosition = xmf3DoorPosition;
						m_vDoors.push_back(door);
					}

					if (floor == 1)
					{
						XMFLOAT3 xmf3CeilingPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, fFloorHeight + MAZE_WALL_HEIGHT + (MAZE_CEILING_THICKNESS * 0.5f));
						ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3CeilingPosition, XMFLOAT3(MAZE_CELL_SIZE, MAZE_CEILING_THICKNESS, MAZE_CELL_SIZE), map.m_xmf4CeilingColor));
					}
				}

				char baseTile = GetMazeTileAtCell(map, 0, x, z);
				if (baseTile == '^')
				{
					XMFLOAT3 xmf3CellCenter = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, 0.0f);
					for (int i = 0; i < MAZE_STAIR_STEP_COUNT; i++)
					{
						float fStepHeight = ((float)(i + 1) / (float)MAZE_STAIR_STEP_COUNT) * MAZE_SECOND_FLOOR_HEIGHT;
						float fStepDepth = MAZE_CELL_SIZE / (float)MAZE_STAIR_STEP_COUNT;
						XMFLOAT3 xmf3StepPosition = XMFLOAT3(xmf3CellCenter.x, fStepHeight * 0.5f, xmf3CellCenter.z - (MAZE_CELL_SIZE * 0.5f) + (fStepDepth * 0.5f) + (fStepDepth * i));
						ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3StepPosition, XMFLOAT3(MAZE_CELL_SIZE * 0.85f, fStepHeight, fStepDepth), map.m_xmf4StairColor));
					}
				}
			}
		}

		for (int i = 0; i < pnStageEnemyCounts[stage]; i++)
		{
			const int *pnEnemyCell = ppnStageEnemyCells[stage][i];
			XMFLOAT3 xmf3EnemyCellPosition = GetMazeCellPosition(pnEnemyCell[0], pnEnemyCell[1], map.m_nWidth, map.m_nHeight, 0.0f);
			xmf3EnemyCellPosition.y = (pnEnemyCell[2] == 1) ? MAZE_SECOND_FLOOR_HEIGHT : GetMazeFloorHeight(xmf3EnemyCellPosition.x, xmf3EnemyCellPosition.z, 0.0f, map);
			CGameObject *pEnemyObject = CreateArmedCharacterObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Soldier_Aiming_Baked.bin", xmf3EnemyCellPosition, XMFLOAT3(0.0f, 0.0f, 0.0f));
			if (pEnemyObject)
			{
				ppObjects.push_back(pEnemyObject);
				ENEMY_OBJECT enemy;
				enemy.m_pObject = pEnemyObject;
				enemy.m_nStage = stage;
				enemy.m_nFloor = pnEnemyCell[2];
				enemy.m_fMoveSpeed = 18.0f;
				enemy.m_fFireCooldown = ENEMY_FIRE_INTERVAL * (0.35f + (0.25f * (float)(i % 4)));
				enemy.m_xmf3SpawnPosition = xmf3EnemyCellPosition;
				enemy.m_xmf3PatrolTarget = XMFLOAT3(xmf3EnemyCellPosition.x + ((i % 2 == 0) ? ENEMY_PATROL_DISTANCE : -ENEMY_PATROL_DISTANCE), xmf3EnemyCellPosition.y, xmf3EnemyCellPosition.z);
				enemy.m_xmf3LastKnownPlayerPosition = xmf3EnemyCellPosition;
				m_vEnemies.push_back(enemy);
			}
		}

		m_pnStageWorldCount[stage] = (int)ppObjects.size() - m_pnStageWorldStart[stage];
	}

	m_nWorldObjects = (int)ppObjects.size();

	m_pNameText = CreateMenuTextObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Name_Text.bin", MENU_NAME_POSITION, MENU_NAME_SCALE, MENU_NAME_ROTATION, XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));
	if (m_pNameText) ppObjects.push_back(m_pNameText);
	m_pStartText = CreateMenuTextObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Start_Text.bin", MENU_START_POSITION, MENU_START_SCALE, MENU_START_ROTATION, XMFLOAT4(1.f, 0.f, 0.f, 1.0f));
	if (m_pStartText) ppObjects.push_back(m_pStartText);
	m_pStage1Text = CreateMenuTextObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Stage_1_Text.bin", MENU_STAGE1_POSITION, MENU_STAGE_SCALE, MENU_STAGE1_ROTATION, XMFLOAT4(1.f, 1.0f, 0.f, 1.0f));
	if (m_pStage1Text) ppObjects.push_back(m_pStage1Text);
	m_pStage2Text = CreateMenuTextObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Stage_2_Text.bin", MENU_STAGE2_POSITION, MENU_STAGE_SCALE, MENU_STAGE2_ROTATION, XMFLOAT4(0.5f, 0.5f, 0.f, 1.0f));
	if (m_pStage2Text) ppObjects.push_back(m_pStage2Text);

	m_nGameObjects = (int)ppObjects.size();
	m_ppGameObjects = new CGameObject*[m_nGameObjects];
	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i] = ppObjects[i];

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

/***/	ReleaseShaderVariables();

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Release();
		delete[] m_ppGameObjects;
	}
	m_vDoors.clear();
	m_vEnemies.clear();
	m_fShotEffectTime = 0.0f;
	m_fMenuElapsedTime = 0.0f;
	m_pNameText = NULL;
	m_pStartText = NULL;
	m_pStage1Text = NULL;
	m_pStage2Text = NULL;

	if (m_pLights) delete[] m_pLights;
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[4];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[1].Descriptor.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[3].Constants.Num32BitValues = 1;
	pd3dRootParameters[3].Constants.ShaderRegister = 3; //Framework Constants
	pd3dRootParameters[3].Constants.RegisterSpace = 0;
	pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256?? ???
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	ApplyStageCeilingSpotLights(m_pLights, GetStageIndexFromStageNumber(m_nSelectedStage));
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}

	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->ReleaseShaderVariables();
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->ReleaseUploadBuffers();
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	m_nMenuCursorX = LOWORD(lParam);
	m_nMenuCursorY = HIWORD(lParam);

	if (nMessageID == WM_LBUTTONDOWN)
	{
		if (m_nScreenMode == SCENE_SCREEN_TITLE)
		{
			if (IsPointInScreenBox(m_nMenuCursorX, m_nMenuCursorY, MENU_START_HIT_BOX))
			{
				m_nScreenMode = SCENE_SCREEN_STAGE_SELECT;
				return(true);
			}
		}
		else if (m_nScreenMode == SCENE_SCREEN_STAGE_SELECT)
		{
			if (IsPointInScreenBox(m_nMenuCursorX, m_nMenuCursorY, MENU_STAGE1_HIT_BOX))
			{
				BeginStage(1);
				return(true);
			}
			if (IsPointInScreenBox(m_nMenuCursorX, m_nMenuCursorY, MENU_STAGE2_HIT_BOX))
			{
				BeginStage(2);
				return(true);
			}
		}
	}
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (nMessageID == WM_KEYDOWN)
	{
		if (m_nScreenMode == SCENE_SCREEN_TITLE)
		{
			if ((wParam == VK_SPACE) || (wParam == VK_RETURN))
			{
				m_nScreenMode = SCENE_SCREEN_STAGE_SELECT;
				return(true);
			}
		}
		else if (m_nScreenMode == SCENE_SCREEN_STAGE_SELECT)
		{
			if (wParam == '1')
			{
				BeginStage(1);
				return(true);
			}
			if (wParam == '2')
			{
				BeginStage(2);
				return(true);
			}
		}
	}
	if (m_nScreenMode != SCENE_SCREEN_PLAYING) return(false);

	if ((nMessageID == WM_KEYDOWN) && ((lParam & 0x40000000) == 0))
	{
		if (wParam == VK_SPACE)
		{
			FireRayShot();
			return(true);
		}
		if (wParam == 'K')
		{
			KillAllEnemiesInStage();
			return(true);
		}
		if (wParam == 'H')
		{
			HealPlayer();
			return(true);
		}
		if (wParam == VK_F5)
		{
			m_bDebugNoDamage = !m_bDebugNoDamage;
			return(false);
		}
	}
	return(false);
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	if (m_nScreenMode != SCENE_SCREEN_PLAYING) return(true);
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
	m_fMenuElapsedTime += fTimeElapsed;

	if (m_nScreenMode == SCENE_SCREEN_PLAYING)
	{
		int nStageIndex = GetStageIndexFromStageNumber(m_nSelectedStage);
		for (int i = m_pnStageWorldStart[nStageIndex]; i < (m_pnStageWorldStart[nStageIndex] + m_pnStageWorldCount[nStageIndex]); i++) m_ppGameObjects[i]->Animate(fTimeElapsed, NULL);
	}
	else
	{
		for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->Animate(fTimeElapsed, NULL);
	}

	if (m_nScreenMode == SCENE_SCREEN_TITLE)
	{
		bool bStartHovered = IsPointInScreenBox(m_nMenuCursorX, m_nMenuCursorY, MENU_START_HIT_BOX);
		UpdateMenuTextMotion(m_pNameText, MENU_NAME_POSITION, MENU_NAME_SCALE, MENU_NAME_ROTATION, false, m_fMenuElapsedTime, 0.0f);
		UpdateMenuTextMotion(m_pStartText, MENU_START_POSITION, MENU_START_SCALE, MENU_START_ROTATION, bStartHovered, m_fMenuElapsedTime, 0.8f);
	}
	else if (m_nScreenMode == SCENE_SCREEN_STAGE_SELECT)
	{
		bool bStage1Hovered = IsPointInScreenBox(m_nMenuCursorX, m_nMenuCursorY, MENU_STAGE1_HIT_BOX);
		bool bStage2Hovered = IsPointInScreenBox(m_nMenuCursorX, m_nMenuCursorY, MENU_STAGE2_HIT_BOX);
		UpdateMenuTextMotion(m_pStage1Text, MENU_STAGE1_POSITION, MENU_STAGE_SCALE, MENU_STAGE1_ROTATION, bStage1Hovered, m_fMenuElapsedTime, 0.3f);
		UpdateMenuTextMotion(m_pStage2Text, MENU_STAGE2_POSITION, MENU_STAGE_SCALE, MENU_STAGE2_ROTATION, bStage2Hovered, m_fMenuElapsedTime, 1.1f);
	}

	if (m_fShotEffectTime > 0.0f)
	{
		m_fShotEffectTime -= fTimeElapsed;
		if (m_fShotEffectTime <= 0.0f) m_fShotEffectTime = 0.0f;
	}
	if (m_fHitEffectTime > 0.0f)
	{
		m_fHitEffectTime -= fTimeElapsed;
		if (m_fHitEffectTime <= 0.0f) m_fHitEffectTime = 0.0f;
	}

	if (m_nScreenMode != SCENE_SCREEN_PLAYING) return;

	int nStageIndex = GetStageIndexFromStageNumber(m_nSelectedStage);
	const MAZE_MAP_DESC& map = g_pMazeMaps[nStageIndex];
	XMFLOAT3 xmf3PlayerPosition = (m_pPlayer) ? m_pPlayer->GetPosition() : XMFLOAT3(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < m_vDoors.size(); i++)
	{
		DOOR_OBJECT& door = m_vDoors[i];
		if (door.m_nStage != nStageIndex) continue;
		XMFLOAT3 xmf3DoorCell = GetMazeCellPosition(door.m_nCellX, door.m_nCellZ, map.m_nWidth, map.m_nHeight, door.m_fFloorHeight);
		bool bSameFloor = (GetMazeFloorIndexFromWorldY(xmf3PlayerPosition.y) == door.m_nFloor);
		float fTargetOpen = (bSameFloor && (DistanceXZ(xmf3DoorCell, xmf3PlayerPosition) < DOOR_TRIGGER_DISTANCE)) ? 1.0f : 0.0f;
		float fDelta = DOOR_OPEN_SPEED * fTimeElapsed;

		if (door.m_fOpenAmount < fTargetOpen)
		{
			door.m_fOpenAmount += fDelta;
			if (door.m_fOpenAmount > fTargetOpen) door.m_fOpenAmount = fTargetOpen;
		}
		else if (door.m_fOpenAmount > fTargetOpen)
		{
			door.m_fOpenAmount -= fDelta;
			if (door.m_fOpenAmount < fTargetOpen) door.m_fOpenAmount = fTargetOpen;
		}

		XMFLOAT3 xmf3DoorPosition = door.m_xmf3ClosedPosition;
		float fSlide = MAZE_CELL_SIZE * 0.85f * door.m_fOpenAmount;
		if (door.m_bHorizontal) xmf3DoorPosition.x += fSlide;
		else xmf3DoorPosition.z += fSlide;
		if (door.m_pObject) door.m_pObject->SetPosition(xmf3DoorPosition);
	}

	for (size_t i = 0; i < m_vEnemies.size(); i++)
	{
		ENEMY_OBJECT& enemy = m_vEnemies[i];
		if (enemy.m_nStage != nStageIndex) continue;
		if (!enemy.m_bAlive || !enemy.m_pObject || !m_pPlayer) continue;
		if (enemy.m_fFireCooldown > 0.0f) enemy.m_fFireCooldown -= fTimeElapsed;

		XMFLOAT3 xmf3EnemyPosition = enemy.m_pObject->GetPosition();
		bool bCanSeePlayer = CanEnemySeePlayer(enemy, xmf3EnemyPosition, xmf3PlayerPosition, map, m_vDoors, nStageIndex);
		if (bCanSeePlayer)
		{
			enemy.m_nState = ENEMY_AI_CHASE;
			enemy.m_fSearchTime = ENEMY_SEARCH_DURATION;
			enemy.m_xmf3LastKnownPlayerPosition = xmf3PlayerPosition;
		}
		else if (enemy.m_nState == ENEMY_AI_CHASE)
		{
			enemy.m_nState = ENEMY_AI_SEARCH;
			enemy.m_fSearchTime = ENEMY_SEARCH_DURATION;
		}

		if (enemy.m_nState == ENEMY_AI_CHASE)
		{
			MoveEnemyToward(enemy, xmf3PlayerPosition, fTimeElapsed, map, m_vDoors, nStageIndex, ENEMY_STOP_DISTANCE);
			if (bCanSeePlayer && (enemy.m_fFireCooldown <= 0.0f))
			{
				xmf3EnemyPosition = enemy.m_pObject->GetPosition();
				XMFLOAT3 xmf3FireDirection = XMFLOAT3(xmf3PlayerPosition.x - xmf3EnemyPosition.x, 0.0f, xmf3PlayerPosition.z - xmf3EnemyPosition.z);
				if (Vector3::Length(xmf3FireDirection) > 0.001f) SetObjectLookDirectionXZ(enemy.m_pObject, xmf3EnemyPosition, Vector3::Normalize(xmf3FireDirection));
				PlaySoundEffect(SHOOT_SOUND_EFFECT_PATH);
				DamagePlayer(ENEMY_FIRE_DAMAGE, xmf3FireDirection);
				enemy.m_fFireCooldown = ENEMY_FIRE_INTERVAL;
			}
		}
		else if (enemy.m_nState == ENEMY_AI_SEARCH)
		{
			enemy.m_fSearchTime -= fTimeElapsed;
			bool bReachedLastKnownPosition = MoveEnemyToward(enemy, enemy.m_xmf3LastKnownPlayerPosition, fTimeElapsed, map, m_vDoors, nStageIndex, ENEMY_PATROL_REACH_DISTANCE);
			if (bReachedLastKnownPosition || (enemy.m_fSearchTime <= 0.0f))
			{
				enemy.m_nState = ENEMY_AI_PATROL;
				enemy.m_bMovingToPatrolTarget = true;
			}
		}
		else
		{
			XMFLOAT3 xmf3PatrolDestination = enemy.m_bMovingToPatrolTarget ? enemy.m_xmf3PatrolTarget : enemy.m_xmf3SpawnPosition;
			bool bReachedPatrolPoint = MoveEnemyToward(enemy, xmf3PatrolDestination, fTimeElapsed, map, m_vDoors, nStageIndex, ENEMY_PATROL_REACH_DISTANCE);
			if (bReachedPatrolPoint) enemy.m_bMovingToPatrolTarget = !enemy.m_bMovingToPatrolTarget;
		}
	}

	if (m_pLights)
	{
		m_pLights[1].m_xmf3Position = m_pPlayer->GetPosition();
		m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
	}
}

void CScene::ResolvePlayerCollision(CPlayer *pPlayer, const XMFLOAT3& xmf3OldPosition, bool bFreeFlyMode)
{
	if (!pPlayer || bFreeFlyMode) return;

	int nStageIndex = GetStageIndexFromStageNumber(m_nSelectedStage);
	const MAZE_MAP_DESC& map = g_pMazeMaps[nStageIndex];
	XMFLOAT3 xmf3Position = pPlayer->GetPosition();
	XMFLOAT3 xmf3ResolvedPosition = xmf3OldPosition;

	xmf3ResolvedPosition.x = xmf3Position.x;
	if (IsPlayerBlockedAtWorld(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3OldPosition.y)) xmf3ResolvedPosition.x = xmf3OldPosition.x;

	xmf3ResolvedPosition.z = xmf3Position.z;
	if (IsPlayerBlockedAtWorld(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3OldPosition.y)) xmf3ResolvedPosition.z = xmf3OldPosition.z;

	xmf3ResolvedPosition.y = GetMazeFloorHeight(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3OldPosition.y, map) + PLAYER_HEIGHT_OFFSET;
	pPlayer->SetPosition(xmf3ResolvedPosition);
}

bool CScene::FireRayShot()
{
	if (!m_pPlayer || !m_pPlayer->GetCamera()) return(false);

	PlaySoundEffect(SHOOT_SOUND_EFFECT_PATH);

	CCamera *pCamera = m_pPlayer->GetCamera();
	XMFLOAT3 xmf3RayOrigin = pCamera->GetPosition();
	XMFLOAT3 xmf3RayDirection = pCamera->GetLookVector();
	xmf3RayDirection = Vector3::Normalize(xmf3RayDirection);

	int nStageIndex = GetStageIndexFromStageNumber(m_nSelectedStage);
	const MAZE_MAP_DESC& map = g_pMazeMaps[nStageIndex];
	int nBestEnemy = -1;
	float fBestHitDistance = PLAYER_RAY_SHOT_RANGE;

	for (size_t i = 0; i < m_vEnemies.size(); i++)
	{
		ENEMY_OBJECT& enemy = m_vEnemies[i];
		if (enemy.m_nStage != nStageIndex) continue;
		if (!enemy.m_bAlive || !enemy.m_pObject) continue;

		XMFLOAT3 xmf3EnemyPosition = enemy.m_pObject->GetPosition();
		XMFLOAT3 xmf3EnemyCenter = XMFLOAT3(xmf3EnemyPosition.x, xmf3EnemyPosition.y + PLAYER_RAY_SHOT_HEIGHT, xmf3EnemyPosition.z);
		float fHitDistance = 0.0f;
		if (!RayIntersectsSphere(xmf3RayOrigin, xmf3RayDirection, xmf3EnemyCenter, PLAYER_RAY_SHOT_RADIUS, PLAYER_RAY_SHOT_RANGE, fHitDistance)) continue;
		if (fHitDistance >= fBestHitDistance) continue;
		if (IsRayBlockedBeforeDistance(xmf3RayOrigin, xmf3RayDirection, fHitDistance, map, m_vDoors, nStageIndex)) continue;

		fBestHitDistance = fHitDistance;
		nBestEnemy = (int)i;
	}

	m_fShotEffectTime = PLAYER_SHOT_EFFECT_DURATION;

	if (nBestEnemy < 0) return(false);

	ENEMY_OBJECT& enemy = m_vEnemies[nBestEnemy];
	enemy.m_nHealth--;
	if (enemy.m_nHealth <= 0)
	{
		enemy.m_bAlive = false;
		SetObjectMaterialColorRecursive(enemy.m_pObject, XMFLOAT4(0.08f, 0.08f, 0.08f, 1.0f));

		XMFLOAT3 xmf3EnemyPosition = enemy.m_pObject->GetPosition();
		xmf3EnemyPosition.y -= 1000.0f;
		enemy.m_pObject->SetPosition(xmf3EnemyPosition);
		if (AreAllEnemiesKilledInStage()) m_bGameOver = true;
	}
	else
	{
		SetObjectMaterialColorRecursive(enemy.m_pObject, XMFLOAT4(1.0f, 0.85f, 0.12f, 1.0f));
	}

	return(true);
}
void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights

	if (m_nScreenMode != SCENE_SCREEN_PLAYING)
	{
		CGameObject *pMenuObjects[2] = { NULL, NULL };
		if (m_nScreenMode == SCENE_SCREEN_TITLE)
		{
			pMenuObjects[0] = m_pNameText;
			pMenuObjects[1] = m_pStartText;
		}
		else
		{
			pMenuObjects[0] = m_pStage1Text;
			pMenuObjects[1] = m_pStage2Text;
		}

		for (int i = 0; i < _countof(pMenuObjects); i++)
		{
			if (pMenuObjects[i])
			{
				pMenuObjects[i]->Animate(m_fElapsedTime, NULL);
				pMenuObjects[i]->UpdateTransform(NULL);
				pMenuObjects[i]->Render(pd3dCommandList, pCamera, pMenuObjects[i]->m_ppd3dcbInstancingGameObjects, pMenuObjects[i]->m_ppcbMappedInstancingGameObjects);
			}
		}
		return;
	}

	int nStageIndex = GetStageIndexFromStageNumber(m_nSelectedStage);
	for (int i = m_pnStageWorldStart[nStageIndex]; i < (m_pnStageWorldStart[nStageIndex] + m_pnStageWorldCount[nStageIndex]); i++)
	{
		if (m_ppGameObjects[i])
		{
			m_ppGameObjects[i]->Animate(m_fElapsedTime, NULL);
			m_ppGameObjects[i]->UpdateTransform(NULL);
			m_ppGameObjects[i]->Render(pd3dCommandList, pCamera, m_ppGameObjects[i]->m_ppd3dcbInstancingGameObjects, m_ppGameObjects[i]->m_ppcbMappedInstancingGameObjects);
		}
	}
}








