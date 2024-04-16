#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

int CFileInfo::DirFileCount(const wstring & wstrFullPath)
{
	CFileFind	find;

	wstring wstrFindPath = wstrFullPath + L"\\*.*";
	BOOL bContinue = find.FindFile(wstrFindPath.c_str());

	int iCount = 0;

	while (bContinue)
	{
		bContinue = find.FindNextFile();

		if (find.IsDots()) 
			continue;
		else
		{
			if (find.IsSystem())
				continue;

			iCount++;
		}
	}

	return iCount;
}

CString CFileInfo::GetFolderPath(const wstring & strFolderName)
{
	TCHAR szCurrentPath[256] = L"";
	GetCurrentDirectory(256, szCurrentPath);
	for (int i = 0; i < 2; ++i)
		PathRemoveFileSpec(szCurrentPath);

	CString temp(szCurrentPath);

	temp += strFolderName.c_str();

	return temp;
}

void CFileInfo::DirRootExtraction(const wstring & strFolderPath, CTreeCtrl & pTreeCtrl, const wstring& strFileType)
{
	HTREEITEM hItem = nullptr;

	CFileFind finder;
	CString temp = strFolderPath.c_str() + CString(L"\\*.*");
	BOOL bWorking = finder.FindFile(temp.GetString());

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;
		else if (finder.IsDirectory())
			DirTreeExtraction(wstring(finder.GetFilePath()), pTreeCtrl, hItem, strFileType);
		else
		{
			if(finder.IsSystem())
				continue;

			CString strFileName = finder.GetFileName();
			int i = strFileName.ReverseFind('.');
			strFileName = strFileName.Right(strFileName.GetLength() - i);

			if (strFileName.GetString() == strFileType)
				pTreeCtrl.InsertItem(finder.GetFileName(), hItem);
		}
	}

	pTreeCtrl.EnsureVisible(hItem);
}

void CFileInfo::DirTreeExtraction(const wstring & strFolderPath, CTreeCtrl & pTreeCtrl, HTREEITEM hItem, const wstring& strFileType)
{
	HTREEITEM hItem2 = nullptr;
	hItem2 = pTreeCtrl.InsertItem(FindFolderName(strFolderPath), hItem);

	CFileFind finder;
	CString temp = strFolderPath.c_str() + CString(L"\\*.*");
	BOOL bWorking = finder.FindFile(temp.GetString());

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;
		else if (finder.IsDirectory())
			DirTreeExtraction(wstring(finder.GetFilePath()), pTreeCtrl, hItem2, strFileType);
		else
		{
			if (finder.IsSystem())
				continue;

			CString strFileName = finder.GetFileName();
			int i = strFileName.ReverseFind('.');
			strFileName = strFileName.Right(strFileName.GetLength() - i);

			if (strFileName.GetString() == strFileType)
				pTreeCtrl.InsertItem(finder.GetFileName(), hItem2);
		}
	}
}

CString CFileInfo::FindFolderName(const wstring & strFolderPath)
{
	CString strPath = strFolderPath.c_str();
	int i = strPath.ReverseFind('\\');
	strPath = strPath.Right(strPath.GetLength() - i - 1);

	return strPath;
}

CString CFileInfo::FindFileName(const wstring & strFolderPath)
{
	return CString();
}

CString CFileInfo::ConvertRelativePath(CString & strFullPath)
{
	TCHAR szRelativePath[256] = L"";
	TCHAR szCurrentPath[256] = L"";

	GetCurrentDirectory(256, szCurrentPath);

	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

CString CFileInfo::ChangeStringPart(CString& strOriginal, CString& strOld, CString& strNew)
{
	CString strOutput = strOriginal;

	strOutput.Replace(strOld, strNew);

	return strOutput;
}

void CFileInfo::DirMeshInfoExtraction(CString & strFullPath, CString & strPath, CString & strFileName, CListBox& PathList)
{
	CString strRelativePath = L"";

	CFileFind finder;
	CString temp = strFullPath + CString(L"\\*.*");
	BOOL bWorking = finder.FindFile(temp.GetString());

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;
		else if (finder.IsDirectory())
			DirMeshInfoExtraction(finder.GetFilePath(), strPath, strFileName, PathList);
		else
		{
			if (finder.IsSystem())
				continue;

			TCHAR		szPath[256] = L"";
			lstrcpy(szPath, finder.GetFilePath().GetString());
			PathRemoveFileSpec(szPath);
			strPath = ConvertRelativePath(CString(szPath));
			strPath.Replace(L"\\", L"/");
			strPath += L"/";

			CString strFileType = finder.GetFileName();
			int i = strFileType.ReverseFind('.');
			strFileType = strFileType.Right(strFileType.GetLength() - i);

			if (strFileType == L".X")
				strFileName = finder.GetFileName();
			else
				continue;

			CString strCombined = strPath + L" " + strFileName;
			PathList.AddString(strCombined);
		}
	}
}
