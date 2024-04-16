#ifndef __SOUNDMGR__H__
#define __SOUNDMGR__H__

#include "Defines.h"
#include "Base.h"

BEGIN(Client)
class CSoundMgr final : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)
private:
	explicit CSoundMgr();
	virtual ~CSoundMgr() = default;
public:
	enum CHANNELID { BGM, PLAYER, PLAYERMOVE, PLAYER2, PLAYER3, THROW, JUMP, JUMP2, HOVERING, EGG, COIN, COIN2, 
		MONSTER, FLOWER, FLOWER2, EFFECT, EFFECT2, GIMMICKS, UI, TRAIN, MAXCHANNEL };
public:
	void Initialize();
	void Update();

public:
	_bool PlaySound(const wstring& strSoundKey, CHANNELID eID);
	void PlayBGM(const wstring& strSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
private:
	void LoadSoundFile();
private:
	// 사운드 리소스 정보 갖고 있는 객체 
	map<wstring, FMOD_SOUND*> m_mapSound;
	// 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannel[MAXCHANNEL];
	//사운드 채널 객체및 장치를 관리할 객체 
	FMOD_SYSTEM* m_pSystem;

public:
	virtual void Free();
};
END
#endif // !__SOUNDMGR__H__

