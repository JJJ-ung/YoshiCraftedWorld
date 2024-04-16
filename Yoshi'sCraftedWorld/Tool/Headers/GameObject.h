#pragma once

class CComponent;
class CGameObject abstract
{
public:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CGameObject() = default;

public:
	_matrix Get_Transform(_D3DTRANSFORMSTATETYPE eType);

public:
	virtual HRESULT Ready_GameObject();

public:
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

protected:
	map<const _tchar*, CComponent*>			m_Components;
	typedef map<const _tchar*, CComponent*>	COMPONENTS;

protected:
	HRESULT Add_Component(const _tchar * pComponentTag, CComponent * pComponent);
	HRESULT Delete_Component(const _tchar * pComponentTag);

private:
	CComponent* Find_Component(const _tchar* pComponentTag);

public:
	_uint								m_iShaderPass = 0;

public:
	virtual void Free();
};