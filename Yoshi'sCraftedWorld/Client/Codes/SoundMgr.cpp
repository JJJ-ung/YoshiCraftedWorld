#include "stdafx.h"
#include "SoundMgr.h"

USING(Client)
IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
	:m_pSystem(nullptr)
{
	Initialize();
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
}

void CSoundMgr::Update()
{
}

void CSoundMgr::Free()
{
	for (auto& MyPair : m_mapSound)
		FMOD_Sound_Release(MyPair.second);

	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

_bool CSoundMgr::PlaySound(const wstring & strSoundKey, CHANNELID eID)
{
	auto iter_find = m_mapSound.find(strSoundKey);

	if (m_mapSound.end() == iter_find)
		return false;

	FMOD_BOOL isPlay;
	_bool temp = true;
	if (FMOD_Channel_IsPlaying(m_pChannel[eID], &isPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second, FALSE, &m_pChannel[eID]);
		temp = false;
	}
	
	FMOD_System_Update(m_pSystem);

	return temp;
}

void CSoundMgr::PlayBGM(const wstring & strSoundKey)
{
	auto iter_find = m_mapSound.find(strSoundKey);

	if (m_mapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE,
		iter_find->second, FALSE, &(m_pChannel[BGM]));

	FMOD_Channel_SetMode(m_pChannel[BGM], FMOD_LOOP_NORMAL);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannel[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
	{
		FMOD_Channel_Stop(m_pChannel[i]);
	}
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd;

	long Handle = _findfirst("../Bin/Sound/*.*", &fd);

	if (0 == Handle)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Bin/Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);
		if (eRes == FMOD_OK)
		{
			int iLen = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLen];
			ZeroMemory(pSoundKey, iLen);

			MultiByteToWideChar(CP_ACP, 0, fd.name, iLen, pSoundKey, iLen);

			m_mapSound.emplace(pSoundKey, pSound);

			Safe_Delete_Array(pSoundKey);
		}
		iResult = _findnext(Handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
}

