#pragma once
#include "GameObject.h"

class CTerrain;
class CCalculator;
class CToolMenu;
class CInput_Device;
class CMouse : public CGameObject
{
public:
	explicit CMouse(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMouse() = default;

public:
	virtual HRESULT Ready_GameObject();

public:
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT Add_Component();

public:
	HRESULT Add_MapObject(CString strFilePath, _vec3 vPos, COLLISION_TYPE eType);
	HRESULT Modify_MapObject(_vec3 vPos);

public:
	HRESULT Add_Gimmick(CString strFilePath, CString strFileName, _vec3 vPos, _uint eType, _uint iOption);
	HRESULT Modify_Gimmick(_vec3 vPos);
	HRESULT Modify_Trigger(_vec3 vPos);

public:
	HRESULT	Modify_EffectEmitter(_vec3 vPos);

public:
	CToolMenu*			m_pToolMenu;
	CCalculator*		m_pCalculatorCom;
	CTerrain*				m_pTerrain;
	CInput_Device*	m_pInput_Device;

public:
	bool						m_bClick = true;

public:
	static CMouse*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
};