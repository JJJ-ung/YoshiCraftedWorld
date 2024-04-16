#pragma once


#define BEGIN(NAMESPACE) namespace NAMESPACE { 
#define END }
#define USING(NAMESPACE) using namespace NAMESPACE;

#define MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK)

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define NO_COPY(CLASSNAME)								\
		private:										\
		CLASSNAME(const CLASSNAME&);					\
		CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)					\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static unsigned long DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)							\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;				\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
			if(NULL == m_pInstance) {							\
				m_pInstance = new CLASSNAME;					\
			}													\
			return m_pInstance;									\
		}														\
		unsigned long CLASSNAME::DestroyInstance( void ) {		\
			unsigned long dwRefCnt = 0;							\
			if(nullptr != m_pInstance) {						\
				dwRefCnt = m_pInstance->Release();				\
				if(0 == dwRefCnt)								\
					m_pInstance = nullptr;						\
				}												\
			return dwRefCnt;									\
		}

#define NO_COPY_STATE(STATENAME)                        \
private:                                          \
   STATENAME() = default;                              \
   STATENAME& operator=(const STATENAME&) = default;         \
   STATENAME(const STATENAME&) = default;                  

#define DECLARE_STATE(STATENAME, OWNER)                     \
NO_COPY_STATE(STATENAME)                              \
public:                                             \
   static   STATENAME* Instance();                        \
   void   Enter(OWNER* owner);                               \
   void   Update(OWNER* owner);                              \
   void   Exit(OWNER* owner);                               \

#define IMPLEMENT_STATE(STATENAME)                        \
STATENAME* STATENAME::Instance()                        \
{                                                \
   static STATENAME instance;                           \
   return &instance;                                 \
}

#define ENTER_STATE(STATENAME, OWNER)                        \
void STATENAME::Enter(OWNER* owner)		

#define UPDATE_STATE(STATENAME, OWNER)                        \
void STATENAME::Update(OWNER* owner)		

#define EXIT_STATE(STATENAME, OWNER)                        \
void STATENAME::Exit(OWNER* owner)		

#define  CHANGE_STATE(STATENAME)			\
owner->GetFSM()->ChangeState(STATENAME::Instance())