#pragma once
#include "Base.h"
#include "State.h"

template <class T>
class CStateMachine : public CBase
{
public:
	CStateMachine(T * owner)
		: m_pOwner(owner)
		, m_pCurrentState(NULL)
	{}

	void Update()
	{
		m_pCurrentState->Update(m_pOwner);
	}
	void ChangeState(CState<T>*pState)
	{
		m_pCurrentState->Exit(m_pOwner);
		m_pCurrentState = pState;
		m_pCurrentState->Enter(m_pOwner);
	}
	virtual void Free() {};

public:
	CState<T>*	GetCurrentState() { return this->m_pCurrentState; }
	void		SetCurrentState(CState<T>*pState) { m_pCurrentState = pState; }

private:
	CState<T>* m_pCurrentState;
	T*	m_pOwner;
};
