#ifndef Engine_Function_h__
#define Engine_Function_h__

template <typename T>
void Safe_Delete(T& pointer)
{
	if (NULL != pointer)
	{
		delete pointer;
		pointer = NULL;
	}
}

template <typename T>
void Safe_Release(T& pointer)
{
	if (NULL != pointer)
	{
		pointer->Release();
		pointer = NULL;
	}
}

template <typename T>
void Safe_Delete_Array(T& pointer)
{
	if (NULL != pointer)
	{
		delete[] pointer;
		pointer = NULL;
	}
}
//////////////////////////////////////////////////////////////////
/////////////////////////////////Functor//////////////////////////

class CTag_Finder
{
public:
	explicit CTag_Finder(const _tchar* pTag)
		: m_pTargetTag(pTag) {		}
	~CTag_Finder() {		}
public:
	template<typename T>
	bool operator()(const T& pair)
	{
		if (0 == lstrcmpW(m_pTargetTag, pair.first))
		{
			return true;
		}

		return false;
	}

private:
	const _tchar*		m_pTargetTag = nullptr;
};

#endif // Engine_Function_h__
