#include "Engine_Defines.h"
#include "Base.h"
#include "Defines.h"

BEGIN(Client)
class CMouse_Manager : public CBase
{
	DECLARE_SINGLETON(CMouse_Manager)

private:
	explicit CMouse_Manager();
	virtual ~CMouse_Manager() = default;

public:
	_vec3& Get_RayPos() { return m_vRayPos; }
	_vec3& Get_RayDir() { return m_vRayDir; }

public:
	HRESULT Ready_MouseManager(LPDIRECT3DDEVICE9 pGraphic_Device);
	void Update_Ray();

public:
	virtual void	Free(void);

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device;

private:
	_vec3			m_vRayPos;
	_vec3			m_vRayDir;
};
END