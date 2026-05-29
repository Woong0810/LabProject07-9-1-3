//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Player.h"
#include "Shader.h"

static const float FIRST_PERSON_CAMERA_HEIGHT = 20.0f;
static const float THIRD_PERSON_LOOK_AT_HEIGHT = 30.0f;
static const XMFLOAT3 THIRD_PERSON_CAMERA_OFFSET = XMFLOAT3(8.0f, 37.0f, -40.0f);

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

static void BuildCameraOrientationFromPlayer(const XMFLOAT3& xmf3PlayerRight, const XMFLOAT3& xmf3PlayerUp, const XMFLOAT3& xmf3PlayerLook, float fPitch, XMFLOAT3& xmf3CameraRight, XMFLOAT3& xmf3CameraUp, XMFLOAT3& xmf3CameraLook)
{
	xmf3CameraRight = xmf3PlayerRight;
	xmf3CameraUp = xmf3PlayerUp;
	xmf3CameraLook = xmf3PlayerLook;

	if (fPitch != 0.0f)
	{
		XMMATRIX xmmtxPitch = XMMatrixRotationAxis(XMLoadFloat3(&xmf3CameraRight), XMConvertToRadians(fPitch));
		xmf3CameraLook = Vector3::TransformNormal(xmf3CameraLook, xmmtxPitch);
		xmf3CameraUp = Vector3::TransformNormal(xmf3CameraUp, xmmtxPitch);
	}

	xmf3CameraLook = Vector3::Normalize(xmf3CameraLook);
	xmf3CameraRight = Vector3::Normalize(xmf3CameraRight);
	xmf3CameraUp = Vector3::CrossProduct(xmf3CameraLook, xmf3CameraRight, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPlayer

CPlayer::CPlayer()
{
	m_pCamera = NULL;

	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

}

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

void CPlayer::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nMeshesInHierarchy, int* pnMaterialsInHierarchy)
{
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CGameObject::CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
}

void CPlayer::ReleaseShaderVariables()
{
	if (m_pCamera) m_pCamera->ReleaseShaderVariables();

/***/	CGameObject::ReleaseShaderVariables();
}

void CPlayer::Move(DWORD dwDirection, float fDistance)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);

		Move(xmf3Shift);
	}
}

void CPlayer::Move(const XMFLOAT3& xmf3Shift)
{
	m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
	if (m_pCamera) m_pCamera->Move(xmf3Shift);
}

void CPlayer::Rotate(float x, float y, float z)
{
	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) && (x != 0.0f))
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
			if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		else if (nCurrentCameraMode == THIRD_PERSON_CAMERA)
		{
			x = 0.0f;
		}
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		m_pCamera->Rotate(x, y, z);
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}
	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);
}

void CPlayer::Update(float fTimeElapsed)
{
	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	XMFLOAT3 xmf3CameraLookAt = m_xmf3Position;
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA)
	{
		xmf3CameraLookAt.y += THIRD_PERSON_LOOK_AT_HEIGHT;
		m_pCamera->Update(xmf3CameraLookAt, fTimeElapsed);
	}
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(xmf3CameraLookAt);
	m_pCamera->RegenerateViewMatrix();
}

CCamera *CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	CCamera *pNewCamera = NULL;
	switch (nNewCameraMode)
	{
		case FIRST_PERSON_CAMERA:
			pNewCamera = new CFirstPersonCamera(m_pCamera);
			break;
		case THIRD_PERSON_CAMERA:
			pNewCamera = new CThirdPersonCamera(m_pCamera);
			break;
	}

	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}

void CPlayer::OnPrepareRender()
{
	m_xmf4x4Transform._11 = m_xmf3Right.x; m_xmf4x4Transform._12 = m_xmf3Right.y; m_xmf4x4Transform._13 = m_xmf3Right.z;
	m_xmf4x4Transform._21 = m_xmf3Up.x; m_xmf4x4Transform._22 = m_xmf3Up.y; m_xmf4x4Transform._23 = m_xmf3Up.z;
	m_xmf4x4Transform._31 = m_xmf3Look.x; m_xmf4x4Transform._32 = m_xmf3Look.y; m_xmf4x4Transform._33 = m_xmf3Look.z;
	m_xmf4x4Transform._41 = m_xmf3Position.x; m_xmf4x4Transform._42 = m_xmf3Position.y; m_xmf4x4Transform._43 = m_xmf3Position.z;

	UpdateTransform(NULL);
}

void CPlayer::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
	if (nCameraMode == THIRD_PERSON_CAMERA) CGameObject::Render(pd3dCommandList, pCamera, m_ppd3dcbInstancingGameObjects, m_ppcbMappedInstancingGameObjects);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSoldierPlayer

CSoldierPlayer::CSoldierPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);

	int nMeshesInHierarchy = 0;
	int pnMaterialsInHierarchy[64] = { 0 };
	CGameObject *pModelObject = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Soldier_Aiming_Baked.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	SetObjectMaterialColorRecursive(pModelObject, XMFLOAT4(0.05f, 0.78f, 0.18f, 1.0f));
	pModelObject->SetPosition(0.0f, -8.0f, 0.0f);

	SetChild(pModelObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
}

CSoldierPlayer::~CSoldierPlayer()
{
}

CCamera *CSoldierPlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
		case FIRST_PERSON_CAMERA:
			m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.0f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, FIRST_PERSON_CAMERA_HEIGHT, 0.0f));
			if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_fPitch = 0.0f;
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		case THIRD_PERSON_CAMERA:
			m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.25f);
			m_pCamera->SetOffset(THIRD_PERSON_CAMERA_OFFSET);
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		default:
			break;
	}

	m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
	if (nNewCameraMode == FIRST_PERSON_CAMERA)
	{
		XMFLOAT3 xmf3CameraRight, xmf3CameraUp, xmf3CameraLook;
		BuildCameraOrientationFromPlayer(m_xmf3Right, m_xmf3Up, m_xmf3Look, m_fPitch, xmf3CameraRight, xmf3CameraUp, xmf3CameraLook);
		m_pCamera->SetOrientation(xmf3CameraRight, xmf3CameraUp, xmf3CameraLook);
	}
	Update(fTimeElapsed);

	return(m_pCamera);
}

