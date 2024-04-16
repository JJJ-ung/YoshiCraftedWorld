#pragma once

template<class T>
class CState
{
public:
	virtual void Enter(T*) = 0;
	virtual void Update(T*) = 0;
	virtual void Exit(T*) = 0;
	virtual ~CState() {};
};
