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
	// ���� ���ҽ� ���� ���� �ִ� ��ü 
	map<wstring, FMOD_SOUND*> m_mapSound;
	// ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannel[MAXCHANNEL];
	//���� ä�� ��ü�� ��ġ�� ������ ��ü 
	FMOD_SYSTEM* m_pSystem;

public:
	virtual void Free();
};
END
#endif // !__SOUNDMGR__H__

