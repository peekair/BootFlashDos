// BootFlashDos.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

int        GetDiskType();
LPTSTR        GetDriverName(int nEnum);
LPTSTR  GetDiskID(int nEnum);

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

void CharToTchar (const char * _char, TCHAR * tchar)  
{  
	int iLength ;  

	iLength = MultiByteToWideChar (CP_ACP, 0, _char, strlen (_char) + 1, NULL, 0) ;  
	MultiByteToWideChar (CP_ACP, 0, _char, strlen (_char) + 1, tchar, iLength) ;  
} 

int        GetDiskType()
{
	HKEY        uKey;
	DWORD        dwRet;

	dwRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\CurrentControlSet\Services\Disk\Enum"), NULL, KEY_READ, &uKey);
	if(dwRet != ERROR_SUCCESS)
		return FALSE;

	DWORD        nType;
	BYTE        btData;
	DWORD        dwSize = 255;

	dwRet = RegQueryValueEx(uKey,_T("Count"), NULL, &nType,&btData,&dwSize);
	if(dwRet != ERROR_SUCCESS)
		return FALSE;

	RegCloseKey(uKey);

	return (int)btData;
}

LPTSTR        GetDriverName(int nEnum)
{
	HKEY        uKey;
	DWORD        dwRet;
	TCHAR        ValueName[10];
	char		tmp[10];
	if(nEnum < 0)
		return NULL;

	dwRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\CurrentControlSet\Services\Disk\Enum"), NULL, KEY_READ, &uKey);
	if(dwRet != ERROR_SUCCESS)
		return FALSE;

	_itoa(nEnum, tmp, 10);
	CharToTchar(tmp,ValueName);

	DWORD        nType;
	BYTE*        lpData = new BYTE[MAX_PATH];
	DWORD        dwSize = 255;

	dwRet = RegQueryValueEx(uKey,ValueName,NULL, &nType,lpData,&dwSize);
	RegCloseKey(uKey);

	if(dwRet != ERROR_SUCCESS)
		return FALSE;
	else
		return (LPTSTR)lpData;
}

LPTSTR  GetDiskID(int nEnum)
{
	DWORD dwRet;
	HKEY  uKey;
	HKEY  vKey;
	if(nEnum < 0)
		return FALSE;

	LPCTSTR szlDriverName = GetDriverName(nEnum);
	if(szlDriverName == NULL)
		return FALSE;

	dwRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\CurrentControlSet\Enum"), NULL, KEY_READ, &uKey);
	if(dwRet != ERROR_SUCCESS)
		return FALSE;

	dwRet = RegOpenKeyEx(uKey, szlDriverName, NULL, KEY_READ, &vKey);
	RegCloseKey(uKey);
	if(dwRet != ERROR_SUCCESS)
		return FALSE;

	DWORD        nType;
	BYTE*        lpData = new BYTE[MAX_PATH];
	DWORD        dwSize = 255;

	dwRet = RegQueryValueEx(vKey,_T("FriendlyName"),NULL, &nType,lpData,&dwSize);
	RegCloseKey(vKey);

	if(dwRet != ERROR_SUCCESS)
		return FALSE;
	else
		return (LPTSTR)lpData;
}