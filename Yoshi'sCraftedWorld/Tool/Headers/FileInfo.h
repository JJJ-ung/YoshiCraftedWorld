#pragma once
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static int DirFileCount(const wstring& wstrFullPath);
	static CString GetFolderPath(const wstring& strFolderName);
	static void DirRootExtraction(const wstring& strFolderPath, CTreeCtrl& pTreeCtrl, const wstring& strFileType);
	static void DirTreeExtraction(const wstring& strFolderPath, CTreeCtrl& pTreeCtrl, HTREEITEM hItem, const wstring& strFileType);
	static CString FindFolderName(const wstring& strFolderPath);
	static CString FindFileName(const wstring& strFolderPath);
	static CString ConvertRelativePath(CString& strFullPath);
	static CString ChangeStringPart(CString& strOriginal, CString& strOld, CString& strNew);
	static void DirMeshInfoExtraction(CString& strFullPath, CString& strPath, CString& strFileName, CListBox& PathList);

public:
	vector<HTREEITEM> m_vecTreeItem;
};

