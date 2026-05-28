//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once

#include <vector>

#include "Shader.h"
#include "Player.h"

#define MAX_LIGHTS			16 

#define POINT_LIGHT			1
#define SPOT_LIGHT			2
#define DIRECTIONAL_LIGHT	3

struct LIGHT
{
	XMFLOAT4				m_xmf4Ambient;
	XMFLOAT4				m_xmf4Diffuse;
	XMFLOAT4				m_xmf4Specular;
	XMFLOAT3				m_xmf3Position;
	float 					m_fFalloff;
	XMFLOAT3				m_xmf3Direction;
	float 					m_fTheta; //cos(m_fTheta)
	XMFLOAT3				m_xmf3Attenuation;
	float					m_fPhi; //cos(m_fPhi)
	bool					m_bEnable;
	int						m_nType;
	float					m_fRange;
	float					padding;
};

struct LIGHTS
{
	LIGHT					m_pLights[MAX_LIGHTS];
	XMFLOAT4				m_xmf4GlobalAmbient;
	int						m_nLights;
};

struct DOOR_OBJECT
{
	CGameObject				*m_pObject = NULL;
	int						m_nStage = 0;
	int						m_nCellX = 0;
	int						m_nCellZ = 0;
	int						m_nFloor = 0;
	bool					m_bHorizontal = false;
	float					m_fFloorHeight = 0.0f;
	float					m_fOpenAmount = 0.0f;
	XMFLOAT3				m_xmf3ClosedPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
};

enum ENEMY_AI_STATE
{
	ENEMY_AI_PATROL = 0,
	ENEMY_AI_CHASE,
	ENEMY_AI_SEARCH
};

struct ENEMY_OBJECT
{
	CGameObject				*m_pObject = NULL;
	float					m_fMoveSpeed = 18.0f;
	float					m_fFireCooldown = 0.0f;
	ENEMY_AI_STATE			m_nState = ENEMY_AI_PATROL;
	int						m_nStage = 0;
	int						m_nFloor = 0;
	bool					m_bAlive = true;
	int						m_nHealth = 1;
	bool					m_bMovingToPatrolTarget = true;
	float					m_fSearchTime = 0.0f;
	XMFLOAT3				m_xmf3SpawnPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3				m_xmf3PatrolTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3				m_xmf3LastKnownPlayerPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
};

enum SCENE_SCREEN_MODE
{
	SCENE_SCREEN_TITLE = 0,
	SCENE_SCREEN_STAGE_SELECT,
	SCENE_SCREEN_PLAYING
};

class CScene
{
public:
    CScene();
    ~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	void BuildDefaultLightsAndMaterials();
	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseObjects();

	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }

	bool ProcessInput(UCHAR *pKeysBuffer);
    void AnimateObjects(float fTimeElapsed);
    void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera=NULL);
	bool FireRayShot();
	bool IsShotEffectVisible() const { return(m_fShotEffectTime > 0.0f); }
	bool IsHitEffectVisible() const { return(m_fHitEffectTime > 0.0f); }
	bool IsPlaying() const { return(m_nScreenMode == SCENE_SCREEN_PLAYING); }
	bool IsGameOver() const { return(m_bGameOver); }
	int GetPlayerHealth() const { return(m_nPlayerHealth); }
	SCENE_SCREEN_MODE GetScreenMode() const { return(m_nScreenMode); }
	void BeginStage(int nStage);
	void DamagePlayer(int nDamage, const XMFLOAT3& xmf3HitDirection);
	void HealPlayer();
	void KillAllEnemiesInStage();
	bool AreAllEnemiesKilledInStage() const;

	void ReleaseUploadBuffers();
	void ResolvePlayerCollision(CPlayer *pPlayer, const XMFLOAT3& xmf3OldPosition, bool bFreeFlyMode);
	bool IsPlayerBlockedAtWorld(float x, float z, float y);

	CPlayer						*m_pPlayer = NULL;

public:
	ID3D12RootSignature			*m_pd3dGraphicsRootSignature = NULL;

	CGameObject					**m_ppGameObjects = NULL;
	int							m_nGameObjects = 0;
	int							m_nWorldObjects = 0;
	int							m_pnStageWorldStart[2] = { 0, 0 };
	int							m_pnStageWorldCount[2] = { 0, 0 };
	CGameObject					*m_pNameText = NULL;
	CGameObject					*m_pStartText = NULL;
	CGameObject					*m_pStage1Text = NULL;
	CGameObject					*m_pStage2Text = NULL;
	SCENE_SCREEN_MODE			m_nScreenMode = SCENE_SCREEN_TITLE;
	int							m_nSelectedStage = 1;
	int							m_nMenuCursorX = -1;
	int							m_nMenuCursorY = -1;

	std::vector<DOOR_OBJECT>	m_vDoors;
	std::vector<ENEMY_OBJECT>	m_vEnemies;
	float						m_fShotEffectTime = 0.0f;
	float						m_fHitEffectTime = 0.0f;
	int							m_nPlayerHealth = 100;
	bool						m_bGameOver = false;
	bool						m_bDebugNoDamage = false;

	LIGHT						*m_pLights = NULL;
	int							m_nLights = 0;

	XMFLOAT4					m_xmf4GlobalAmbient;

	ID3D12Resource				*m_pd3dcbLights = NULL;
	LIGHTS						*m_pcbMappedLights = NULL;

	float						m_fElapsedTime = 0.0f;
	float						m_fMenuElapsedTime = 0.0f;
};
