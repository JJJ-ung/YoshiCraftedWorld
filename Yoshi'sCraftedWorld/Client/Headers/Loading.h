#pragma once

#include "Defines.h"
#include "Base.h"

BEGIN(Engine)
class CManagement;
END

BEGIN(Client)

class CLoading final : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading() = default;

public:
	SCENEID			Get_LoadingID(void) const { return m_eLoadingID; }
	CRITICAL_SECTION*	Get_Crt(void) { return &m_Crt; }
	_bool				Get_Finish(void) const { return m_bFinish; }

public:
	HRESULT		Ready_Loading(SCENEID eLoadingID);

	// Load For Scenes
public:
	_uint				Loading_ForTitle();
	_uint				Loading_ForWorldMap();
	_uint				Loading_ForTownTrain();

public:
	HRESULT		Load_MeshFromFile(SCENEID eLoadingID, const _tchar* pFilePath);
	HRESULT		Load_DynamicMeshFromFile(SCENEID eLoadingID);

public:
	static unsigned int CALLBACK Thread_Main(void* pArg);

private:
	HANDLE							m_hThread;
	CRITICAL_SECTION		m_Crt;
	SCENEID							m_eLoadingID;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool									m_bFinish;

public:
	static CLoading*	Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eLoadingID);
	virtual	void		Free(void);
};

END