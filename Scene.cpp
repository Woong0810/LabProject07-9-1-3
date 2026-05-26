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
	const char *m_pTiles;
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
static const float MAZE_CELL_SIZE = 20.0f;

static const char g_pMazeMap0[] =
	"#######################"
	"#S....#.....D.........#"
	"#.....#.....#.........#"
	"#.....D.....#.........#"
	"#.....#.....#####D#####"
	"###D#########.........#"
	"#.........#...........#"
	"#.........#.....D.....#"
	"#.........#####.#######"
	"#####D#####.....#.....#"
	"#.........#.....#.....#"
	"#.........#..^..#.....#"
	"###########..H..#.....#"
	"#.......D....2..D..2..#"
	"#............2..H..2..#"
	"#............222H222..#"
	"#######################";

static const char g_pMazeMap1[] =
	"#######################"
	"#S........#...........#"
	"#.........#...........#"
	"#.........D...........#"
	"#####D###########D#####"
	"#.....#...............#"
	"#.....#...............#"
	"#.....D.......#####.###"
	"#.....#.......#.....D.#"
	"###.###########.......#"
	"#...#.........#.......#"
	"#...#....^....#####D###"
	"#...#....H..........2.#"
	"#...D....2..........2.#"
	"#...#....2222HH2222222#"
	"#...#.................#"
	"#######################";

static const MAZE_MAP_DESC g_pMazeMaps[] =
{
	{ g_pMazeMap0, MAZE_WIDTH, MAZE_HEIGHT, XMFLOAT4(0.42f, 0.42f, 0.40f, 1.0f), XMFLOAT4(0.54f, 0.54f, 0.50f, 1.0f), XMFLOAT4(0.58f, 0.58f, 0.56f, 1.0f), XMFLOAT4(0.82f, 0.68f, 0.28f, 1.0f), XMFLOAT4(0.00f, 0.88f, 0.82f, 1.0f) },
	{ g_pMazeMap1, MAZE_WIDTH, MAZE_HEIGHT, XMFLOAT4(0.38f, 0.38f, 0.39f, 1.0f), XMFLOAT4(0.46f, 0.46f, 0.48f, 1.0f), XMFLOAT4(0.08f, 0.12f, 0.58f, 1.0f), XMFLOAT4(0.52f, 0.60f, 0.30f, 1.0f), XMFLOAT4(0.00f, 0.92f, 0.86f, 1.0f) }
};

static XMFLOAT3 GetMazeCellPosition(int x, int z, int width, int height, float y)
{
	return(XMFLOAT3((x - (width - 1) * 0.5f) * MAZE_CELL_SIZE, y, (z - (height - 1) * 0.5f) * MAZE_CELL_SIZE));
}

static bool IsSecondFloorTile(char tile)
{
	return((tile == '2') || (tile == 'H'));
}

static bool IsSecondFloorDoor(const MAZE_MAP_DESC& map, int x, int z)
{
	if (map.m_pTiles[z * map.m_nWidth + x] != 'D') return(false);
	if ((x > 0) && IsSecondFloorTile(map.m_pTiles[z * map.m_nWidth + (x - 1)])) return(true);
	if ((x < (map.m_nWidth - 1)) && IsSecondFloorTile(map.m_pTiles[z * map.m_nWidth + (x + 1)])) return(true);
	if ((z > 0) && IsSecondFloorTile(map.m_pTiles[(z - 1) * map.m_nWidth + x])) return(true);
	if ((z < (map.m_nHeight - 1)) && IsSecondFloorTile(map.m_pTiles[(z + 1) * map.m_nWidth + x])) return(true);
	return(false);
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

static char GetMazeTileAtWorld(float x, float z, const MAZE_MAP_DESC& map)
{
	int nCellX = WorldToMazeCellX(x, map);
	int nCellZ = WorldToMazeCellZ(z, map);
	if (!IsInsideMazeCell(nCellX, nCellZ, map)) return('#');
	return(map.m_pTiles[nCellZ * map.m_nWidth + nCellX]);
}

static bool IsBlockingTile(char tile)
{
	return((tile == '#') || (tile == 'D') || (tile == 'H'));
}

static bool IsBlockedAtWorld(float x, float z, const MAZE_MAP_DESC& map)
{
	return(IsBlockingTile(GetMazeTileAtWorld(x, z, map)));
}

static bool IsPlayerBlockedAtWorld(float x, float z, const MAZE_MAP_DESC& map)
{
	const float fPlayerRadius = 6.0f;
	return(IsBlockedAtWorld(x - fPlayerRadius, z - fPlayerRadius, map) ||
		IsBlockedAtWorld(x + fPlayerRadius, z - fPlayerRadius, map) ||
		IsBlockedAtWorld(x - fPlayerRadius, z + fPlayerRadius, map) ||
		IsBlockedAtWorld(x + fPlayerRadius, z + fPlayerRadius, map));
}

static float GetMazeFloorHeight(float x, float z, const MAZE_MAP_DESC& map)
{
	int nCellX = WorldToMazeCellX(x, map);
	int nCellZ = WorldToMazeCellZ(z, map);
	if (!IsInsideMazeCell(nCellX, nCellZ, map)) return(0.0f);

	char tile = map.m_pTiles[nCellZ * map.m_nWidth + nCellX];
	if (IsSecondFloorTile(tile) || IsSecondFloorDoor(map, nCellX, nCellZ)) return(30.0f);
	if (tile == '^')
	{
		XMFLOAT3 xmf3CellCenter = GetMazeCellPosition(nCellX, nCellZ, map.m_nWidth, map.m_nHeight, 0.0f);
		float fLocalZ = (z - (xmf3CellCenter.z - (MAZE_CELL_SIZE * 0.5f))) / MAZE_CELL_SIZE;
		if (fLocalZ < 0.0f) fLocalZ = 0.0f;
		if (fLocalZ > 1.0f) fLocalZ = 1.0f;
		return(fLocalZ * 30.0f);
	}
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
void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	BuildDefaultLightsAndMaterials();

	std::vector<CGameObject*> ppObjects;
	const MAZE_MAP_DESC& map = g_pMazeMaps[0];

	for (int z = 0; z < map.m_nHeight; z++)
	{
		for (int x = 0; x < map.m_nWidth; x++)
		{
			char tile = map.m_pTiles[z * map.m_nWidth + x];
			XMFLOAT3 xmf3BaseFloorPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, -1.0f);
			ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3BaseFloorPosition, XMFLOAT3(MAZE_CELL_SIZE, 2.0f, MAZE_CELL_SIZE), map.m_xmf4FloorColor));

			bool bSecondFloor = IsSecondFloorTile(tile) || IsSecondFloorDoor(map, x, z);
			float fFloorHeight = bSecondFloor ? 30.0f : 0.0f;
			if (bSecondFloor)
			{
				XMFLOAT3 xmf3RaisedFloorPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, fFloorHeight - 1.0f);
				ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3RaisedFloorPosition, XMFLOAT3(MAZE_CELL_SIZE, 2.0f, MAZE_CELL_SIZE), map.m_xmf4RaisedFloorColor));
			}

			if ((tile == '#') || (tile == 'H'))
			{
				XMFLOAT3 xmf3WallPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, fFloorHeight + 15.0f);
				ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3WallPosition, XMFLOAT3(MAZE_CELL_SIZE, 30.0f, MAZE_CELL_SIZE), map.m_xmf4WallColor));
			}
			else if (tile == 'D')
			{
				bool bHorizontalDoor = (x > 0 && x < (map.m_nWidth - 1) && map.m_pTiles[z * map.m_nWidth + (x - 1)] == '#' && map.m_pTiles[z * map.m_nWidth + (x + 1)] == '#');
				XMFLOAT3 xmf3DoorPosition = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, fFloorHeight + 11.0f);
				XMFLOAT3 xmf3DoorScale = bHorizontalDoor ? XMFLOAT3(MAZE_CELL_SIZE * 0.90f, 22.0f, 4.0f) : XMFLOAT3(4.0f, 22.0f, MAZE_CELL_SIZE * 0.90f);
				ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3DoorPosition, xmf3DoorScale, map.m_xmf4DoorColor));
			}
			else if (tile == '^')
			{
				XMFLOAT3 xmf3CellCenter = GetMazeCellPosition(x, z, map.m_nWidth, map.m_nHeight, 0.0f);
				for (int i = 0; i < 6; i++)
				{
					float fStepHeight = (i + 1) * 5.0f;
					float fStepDepth = MAZE_CELL_SIZE / 6.0f;
					XMFLOAT3 xmf3StepPosition = XMFLOAT3(xmf3CellCenter.x, fStepHeight * 0.5f, xmf3CellCenter.z - (MAZE_CELL_SIZE * 0.5f) + (fStepDepth * 0.5f) + (fStepDepth * i));
					ppObjects.push_back(CreateColoredBoxObject(pd3dDevice, pd3dCommandList, xmf3StepPosition, XMFLOAT3(MAZE_CELL_SIZE * 0.85f, fStepHeight, fStepDepth), map.m_xmf4StairColor));
				}
			}
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
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256ÀÇ ¹è¼ö
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
	if (IsPlayerBlockedAtWorld(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, map)) xmf3ResolvedPosition.x = xmf3OldPosition.x;

	xmf3ResolvedPosition.z = xmf3Position.z;
	if (IsPlayerBlockedAtWorld(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, map)) xmf3ResolvedPosition.z = xmf3OldPosition.z;

	xmf3ResolvedPosition.y = GetMazeFloorHeight(xmf3ResolvedPosition.x, xmf3ResolvedPosition.z, map) + 8.0f;
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








