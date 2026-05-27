//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 4;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);

	m_pLights[0].m_bEnable = true;
	m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights[0].m_fRange = 1000.0f;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(30.0f, 30.0f, 30.0f);
	m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);
	m_pLights[1].m_bEnable = true;
	m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights[1].m_fRange = 500.0f;
	m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[1].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[1].m_fFalloff = 8.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	m_pLights[2].m_bEnable = true;
	m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[2].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	m_pLights[2].m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pLights[3].m_bEnable = true;
	m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights[3].m_fRange = 600.0f;
	m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.7f, 0.0f, 1.0f);
	m_pLights[3].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[3].m_xmf3Position = XMFLOAT3(50.0f, 30.0f, 30.0f);
	m_pLights[3].m_xmf3Direction = XMFLOAT3(0.0f, 1.0f, 1.0f);
	m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[3].m_fFalloff = 8.0f;
	m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
}

struct MAZE_MAP_DESC
{
	const char *m_pFloorTiles[2];
	int m_nWidth;
	int m_nHeight;
	XMFLOAT4 m_xmf4FloorColor;
	XMFLOAT4 m_xmf4RaisedFloorColor;
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

static const char g_pStage1Floor0Map[] =
	"#######################"
	"#S....#.....D.........#"
	"#.....#.....#.........#"
	"#.....D.....#.........#"
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

static const MAZE_MAP_DESC g_pMazeMaps[] =
{
	{ { g_pStage1Floor0Map, g_pStage1Floor1Map }, MAZE_WIDTH, MAZE_HEIGHT, XMFLOAT4(0.42f, 0.42f, 0.40f, 1.0f), XMFLOAT4(0.54f, 0.54f, 0.50f, 1.0f), XMFLOAT4(0.58f, 0.58f, 0.56f, 1.0f), XMFLOAT4(0.82f, 0.68f, 0.28f, 1.0f), XMFLOAT4(0.00f, 0.88f, 0.82f, 1.0f) }
};

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

static bool IsDoorOpenAtCell(const std::vector<DOOR_OBJECT>& doors, int floor, int x, int z)
{
	for (size_t i = 0; i < doors.size(); i++)
	{
		if ((doors[i].m_nFloor == floor) && (doors[i].m_nCellX == x) && (doors[i].m_nCellZ == z))
		{
			return(doors[i].m_fOpenAmount > 0.78f);
		}
	}
	return(false);
}

static bool IsBlockedAtWorld(float x, float z, float y, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors)
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
	if ((tile == 'D') && !IsDoorOpenAtCell(doors, nFloor, nCellX, nCellZ)) return(true);
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

static CGameObject *CreateArmedCharacterObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, char *pstrCharacterFileName, const XMFLOAT3& xmf3Position, const XMFLOAT3& xmf3Rotation)
{
	int nCharacterMeshesInHierarchy = 0;
	int pnMaterialsInHierarchy[64] = { 0 };
	CGameObject *pCharacterObject = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pstrCharacterFileName, &nCharacterMeshesInHierarchy, pnMaterialsInHierarchy);
	if (!pCharacterObject) return(NULL);

	pCharacterObject->Rotate(xmf3Rotation.x, xmf3Rotation.y, xmf3Rotation.z);
	pCharacterObject->SetPosition(xmf3Position);
	pCharacterObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nCharacterMeshesInHierarchy, pnMaterialsInHierarchy);
	return(pCharacterObject);
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

static bool IsEnemyBlockedAtWorld(float x, float z, float y, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors)
{
	return(IsBlockedAtWorld(x - ENEMY_RADIUS, z - ENEMY_RADIUS, y, map, doors) ||
		IsBlockedAtWorld(x + ENEMY_RADIUS, z - ENEMY_RADIUS, y, map, doors) ||
		IsBlockedAtWorld(x - ENEMY_RADIUS, z + ENEMY_RADIUS, y, map, doors) ||
		IsBlockedAtWorld(x + ENEMY_RADIUS, z + ENEMY_RADIUS, y, map, doors));
}

static bool HasLineOfSightToPlayer(const XMFLOAT3& xmf3EnemyPosition, const XMFLOAT3& xmf3PlayerPosition, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors)
{
	float fDistance = DistanceXZ(xmf3EnemyPosition, xmf3PlayerPosition);
	int nSteps = (int)(fDistance / (MAZE_CELL_SIZE * 0.35f));
	if (nSteps < 2) return(true);

	for (int i = 1; i < nSteps; i++)
	{
		float t = (float)i / (float)nSteps;
		float x = xmf3EnemyPosition.x + ((xmf3PlayerPosition.x - xmf3EnemyPosition.x) * t);
		float z = xmf3EnemyPosition.z + ((xmf3PlayerPosition.z - xmf3EnemyPosition.z) * t);
		if (IsBlockedAtWorld(x, z, xmf3EnemyPosition.y, map, doors)) return(false);
	}
	return(true);
}

static bool CanEnemySeePlayer(const ENEMY_OBJECT& enemy, const XMFLOAT3& xmf3EnemyPosition, const XMFLOAT3& xmf3PlayerPosition, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors)
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

	return(HasLineOfSightToPlayer(xmf3EnemyPosition, xmf3PlayerPosition, map, doors));
}

static bool MoveEnemyToward(ENEMY_OBJECT& enemy, const XMFLOAT3& xmf3TargetPosition, float fTimeElapsed, const MAZE_MAP_DESC& map, const std::vector<DOOR_OBJECT>& doors, float fStopDistance)
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
	if (IsEnemyBlockedAtWorld(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3EnemyPosition.y, map, doors)) xmf3ResolvedPosition.x = xmf3EnemyPosition.x;

	xmf3ResolvedPosition.z += xmf3Direction.z * fMoveDistance;
	if (IsEnemyBlockedAtWorld(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3EnemyPosition.y, map, doors)) xmf3ResolvedPosition.z = xmf3EnemyPosition.z;

	xmf3ResolvedPosition.y = GetMazeFloorHeight(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3EnemyPosition.y, map);
	SetObjectLookDirectionXZ(enemy.m_pObject, xmf3ResolvedPosition, xmf3Direction);
	return(DistanceXZ(xmf3ResolvedPosition, xmf3TargetPosition) <= fStopDistance);
}

bool CScene::IsPlayerBlockedAtWorld(float x, float z, float y)
{
	const MAZE_MAP_DESC& map = g_pMazeMaps[0];
	const float fPlayerRadius = 6.0f;
	const float px[4] = { x - fPlayerRadius, x + fPlayerRadius, x - fPlayerRadius, x + fPlayerRadius };
	const float pz[4] = { z - fPlayerRadius, z - fPlayerRadius, z + fPlayerRadius, z + fPlayerRadius };

	for (int i = 0; i < 4; i++)
	{
		if (IsBlockedAtWorld(px[i], pz[i], y, map, m_vDoors)) return(true);
	}
	return(false);
}
void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	BuildDefaultLightsAndMaterials();

	m_vDoors.clear();
	m_vEnemies.clear();

	std::vector<CGameObject*> ppObjects;
	const MAZE_MAP_DESC& map = g_pMazeMaps[0];

	for (int z = 0; z < map.m_nHeight; z++)
	{
		for (int x = 0; x < map.m_nWidth; x++)
		{
			XMFLOAT3 xmf3BaseFloorPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, -1.0f);
			ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3BaseFloorPosition, XMFLOAT3(MAZE_CELL_SIZE, 2.0f, MAZE_CELL_SIZE), map.m_xmf4FloorColor));

			for (int floor = 0; floor < MAZE_FLOOR_COUNT; floor++)
			{
				char tile = GetMazeTileAtCell(map, floor, x, z);
				if ((floor == 1) && !HasSecondFloorTile(tile)) continue;

				float fFloorHeight = (floor == 1) ? MAZE_SECOND_FLOOR_HEIGHT : 0.0f;
				if (floor == 1)
				{
					XMFLOAT3 xmf3RaisedFloorPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, fFloorHeight - 1.0f);
					ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3RaisedFloorPosition, XMFLOAT3(MAZE_CELL_SIZE, 2.0f, MAZE_CELL_SIZE), map.m_xmf4RaisedFloorColor));
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
					door.m_nCellX = x;
					door.m_nCellZ = z;
					door.m_nFloor = floor;
					door.m_bHorizontal = bHorizontalDoor;
					door.m_fFloorHeight = fFloorHeight;
					door.m_xmf3ClosedPosition = xmf3DoorPosition;
					m_vDoors.push_back(door);
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
	const int pnEnemyCells[][3] = { { 16, 2, 0 }, { 7, 7, 0 }, { 18, 13, 1 } };
	for (int i = 0; i < _countof(pnEnemyCells); i++)
	{
		XMFLOAT3 xmf3EnemyCellPosition = GetMazeCellPosition(pnEnemyCells[i][0], pnEnemyCells[i][1], map.m_nWidth, map.m_nHeight, 0.0f);
		xmf3EnemyCellPosition.y = (pnEnemyCells[i][2] == 1) ? MAZE_SECOND_FLOOR_HEIGHT : GetMazeFloorHeight(xmf3EnemyCellPosition.x, xmf3EnemyCellPosition.z, 0.0f, map);
		CGameObject *pEnemyObject = CreateArmedCharacterObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Soldier_Aiming_Baked.bin", xmf3EnemyCellPosition, XMFLOAT3(0.0f, 0.0f, 0.0f));
		if (pEnemyObject)
		{
			ppObjects.push_back(pEnemyObject);
			ENEMY_OBJECT enemy;
			enemy.m_pObject = pEnemyObject;
			enemy.m_nFloor = pnEnemyCells[i][2];
			enemy.m_fMoveSpeed = 18.0f;
			enemy.m_xmf3SpawnPosition = xmf3EnemyCellPosition;
			enemy.m_xmf3PatrolTarget = XMFLOAT3(xmf3EnemyCellPosition.x + ((i % 2 == 0) ? ENEMY_PATROL_DISTANCE : -ENEMY_PATROL_DISTANCE), xmf3EnemyCellPosition.y, xmf3EnemyCellPosition.z);
			enemy.m_xmf3LastKnownPlayerPosition = xmf3EnemyCellPosition;
			m_vEnemies.push_back(enemy);
		}
	}

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
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;

	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->Animate(fTimeElapsed, NULL);

	const MAZE_MAP_DESC& map = g_pMazeMaps[0];
	XMFLOAT3 xmf3PlayerPosition = (m_pPlayer) ? m_pPlayer->GetPosition() : XMFLOAT3(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < m_vDoors.size(); i++)
	{
		DOOR_OBJECT& door = m_vDoors[i];
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
		if (!enemy.m_pObject || !m_pPlayer) continue;

		XMFLOAT3 xmf3EnemyPosition = enemy.m_pObject->GetPosition();
		bool bCanSeePlayer = CanEnemySeePlayer(enemy, xmf3EnemyPosition, xmf3PlayerPosition, map, m_vDoors);
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
			MoveEnemyToward(enemy, xmf3PlayerPosition, fTimeElapsed, map, m_vDoors, ENEMY_STOP_DISTANCE);
		}
		else if (enemy.m_nState == ENEMY_AI_SEARCH)
		{
			enemy.m_fSearchTime -= fTimeElapsed;
			bool bReachedLastKnownPosition = MoveEnemyToward(enemy, enemy.m_xmf3LastKnownPlayerPosition, fTimeElapsed, map, m_vDoors, ENEMY_PATROL_REACH_DISTANCE);
			if (bReachedLastKnownPosition || (enemy.m_fSearchTime <= 0.0f))
			{
				enemy.m_nState = ENEMY_AI_PATROL;
				enemy.m_bMovingToPatrolTarget = true;
			}
		}
		else
		{
			XMFLOAT3 xmf3PatrolDestination = enemy.m_bMovingToPatrolTarget ? enemy.m_xmf3PatrolTarget : enemy.m_xmf3SpawnPosition;
			bool bReachedPatrolPoint = MoveEnemyToward(enemy, xmf3PatrolDestination, fTimeElapsed, map, m_vDoors, ENEMY_PATROL_REACH_DISTANCE);
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

	const MAZE_MAP_DESC& map = g_pMazeMaps[0];
	XMFLOAT3 xmf3Position = pPlayer->GetPosition();
	XMFLOAT3 xmf3ResolvedPosition = xmf3OldPosition;

	xmf3ResolvedPosition.x = xmf3Position.x;
	if (IsPlayerBlockedAtWorld(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3OldPosition.y)) xmf3ResolvedPosition.x = xmf3OldPosition.x;

	xmf3ResolvedPosition.z = xmf3Position.z;
	if (IsPlayerBlockedAtWorld(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3OldPosition.y)) xmf3ResolvedPosition.z = xmf3OldPosition.z;

	xmf3ResolvedPosition.y = GetMazeFloorHeight(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, xmf3OldPosition.y, map) + PLAYER_HEIGHT_OFFSET;
	pPlayer->SetPosition(xmf3ResolvedPosition);
}
void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights

	for (int i = 0; i < m_nGameObjects; i++)
	{
		if (m_ppGameObjects[i])
		{
			m_ppGameObjects[i]->Animate(m_fElapsedTime, NULL);
			m_ppGameObjects[i]->UpdateTransform(NULL);
			m_ppGameObjects[i]->Render(pd3dCommandList, pCamera, m_ppGameObjects[i]->m_ppd3dcbInstancingGameObjects, m_ppGameObjects[i]->m_ppcbMappedInstancingGameObjects);
		}
	}
}








