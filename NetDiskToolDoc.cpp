// NetDiskToolDoc.cpp : implementation of the CNetDiskToolDoc class
//

#include "stdafx.h"
#include "NetDiskTool.h"

#include "NetDiskToolDoc.h"
#include "NetDiskToolView.h"

#include "LoginDlg.h"
#include "TypeDlg.h"
#include "ClientSocket.h"

#include <winioctl.h>
#include "matthewdebug.h"

#define IOCTL_VHDISK_INSTALL_OK	CTL_CODE(IOCTL_DISK_BASE, 0x885, METHOD_BUFFERED, FILE_ANY_ACCESS)


USING_MATTHEW_DEBUG;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




int New_nt_ReadDisk_NT_old(HANDLE drv, DWORD sec_start, DWORD secs_number,char *lpBuf)
{
	DWORD	dwRet = 0;
	LARGE_INTEGER	pos;

	pos.QuadPart = sec_start;
	pos.QuadPart *= 0x200ul;	//每扇区大小为0x200个字节
	

	if ((((DWORD)0xFFFFFFFF) == SetFilePointer(drv, pos.u.LowPart, &pos.u.HighPart, FILE_BEGIN)) 
		&& (GetLastError() != NO_ERROR))
	{
		return NDT_ERROR_SETFILEPOINTER;
	}

	if ( 0 == ReadFile( drv, lpBuf, secs_number*0x200ul, &dwRet, NULL))
	{
		return NDT_ERROR_READ;
	}

	if ( secs_number*0x200ul < dwRet )
	{
		return NDT_ERROR_READ;
	}
	
	return NDT_ERROR_SUCCESSFUL;
}

void IO_Error_Report(char *	err_fmt, HANDLE drv, DWORD sec_start, DWORD secs_number, DWORD dwErrCode)
{
	CString		str;
	
	if (NO_ERROR != dwErrCode)
		str.Format("%s. \nDiskID:0x%08X, LBA:0x%08X, Sector Count:0x%08X\nError Code: 0x%08X\n请重启系统后再试，如果问题仍然存在，请尝试升级与硬盘相关的驱动程序！",err_fmt,drv,sec_start,secs_number, dwErrCode);
	else
		str.Format("%s. \nDiskID:0x%08X, LBA:0x%08X, Sector Count:0x%08X\n请重启系统后再试，如果问题仍然存在，请尝试升级与硬盘相关的驱动程序！",err_fmt,drv,sec_start,secs_number);
	
	AfxMessageBox(str);
	return ;
}


extern "C" int WINAPI New_nt_ReadDisk_NT(HANDLE drv, DWORD sec_start, DWORD secs_number,char *lpBuf)
{
	DWORD	dwReadWriteSpeed = 1;
	
	int		nRetry = 5;
	DWORD	dwCount;
	DWORD	dwTemp;
	DWORD	dwRet, dwErrCode;
	LPSTR	lpTempBuf = lpBuf;
	LARGE_INTEGER	pos, qwTemp;
	pos.QuadPart = sec_start;
	pos.QuadPart *= 0x200;
	
	if (NULL == lpTempBuf)
		return ERROR_INVALID_PARAMETER;
	
	dwRet = 0;
	dwTemp = 0;
	dwCount = secs_number;
	dwErrCode = NO_ERROR;
	
	if( (((DWORD)0xffffffff) == SetFilePointer((HANDLE)drv, pos.LowPart, &pos.HighPart, FILE_BEGIN))
		&& (NO_ERROR != (dwErrCode = GetLastError())))
	{
		IO_Error_Report("SetFilePointer Failed:", drv, sec_start, secs_number, dwErrCode);
		return ERROR_NEGATIVE_SEEK;
	}
	
	dwReadWriteSpeed = dwCount;
	
	for (DWORD dw = 0; dw < dwCount; dw += dwReadWriteSpeed, dwRet += dwTemp)
	{
		dwReadWriteSpeed = dwCount - dw;
		
		if (!ReadFile((HANDLE)drv, (lpTempBuf + dwRet), (dwReadWriteSpeed * 0x200ul), &dwTemp, NULL))
		{
			dwErrCode = GetLastError();
			while (1)
			{
				qwTemp.QuadPart = pos.QuadPart + dw;
				if ( (((DWORD)0xffffffff) == SetFilePointer((HANDLE)drv, qwTemp.LowPart, &qwTemp.HighPart, FILE_BEGIN))
					&& (NO_ERROR != (dwErrCode = GetLastError())))
				{
					// DebugOutput(__LINE__, "SetFilePointer Failed!");
					IO_Error_Report("SetFilePointer Failed:", drv, sec_start+dw, secs_number, dwErrCode);
					return ERROR_NEGATIVE_SEEK;
				}
				
				//DebugOutput(__LINE__, "ReadFile Failed! Try again! Error Code : 0x%08X, Speed = 0x%08X, dwTemp = 0x%08X", dwErrCode, dwReadWriteSpeed, dwTemp);
				dwReadWriteSpeed = dwReadWriteSpeed / 2 +1;
				if (ReadFile((HANDLE)drv, (lpTempBuf + dwRet), (dwReadWriteSpeed * 0x200ul), &dwTemp, NULL))
				{
					dwErrCode = NO_ERROR;
					break;
				}
				else
				{
					dwErrCode = GetLastError();
					if (1 == dwReadWriteSpeed)
					{
						if (2 > nRetry--)
						{
							break;
						}
					}
				}
			}
			
			if (NO_ERROR != dwErrCode)
			{
				//DebugOutput(__LINE__, "读数据失败！\nError Code : 0x%08X, Speed = %u", dwErrCode, dwReadWriteSpeed);
				IO_Error_Report("读数据失败！", drv, sec_start+dw, dwCount, dwErrCode);
				return ERROR_READ_FAULT;
			}
		}
		dwErrCode = NO_ERROR;
	}
	
	if (dwRet != (secs_number * 0x200ul))
	{
		//DebugOutput(__LINE__, "ReadFile Failed!\nError Code : 0x%08X, dwRet = 0x%08X, Secs_num = 0x%08X", dwErrCode, dwRet, secs_number);
		IO_Error_Report("ReadFile Failed(No more data):", drv, sec_start + dw, secs_number, dwErrCode);
		return ERROR_HANDLE_EOF;
	}
	
	return NO_ERROR;
}


int New_nt_WriteDisk_NT_old(HANDLE drv, DWORD sec_start, DWORD secs_number, const char *lpBuf)
{
	DWORD	dwRet = 0;
	LARGE_INTEGER	pos;
	
	//	char	ooo[111];
	//	sprintf(ooo,"Write:%08lx\n",sec_start);
	//	OutputDebugString(ooo);
	
	pos.QuadPart = sec_start;
	pos.QuadPart *= 0x200ul;		//每扇区大小为0x200个字节


	if( (((DWORD)0xFFFFFFFF) == SetFilePointer(drv, pos.u.LowPart, &pos.u.HighPart, FILE_BEGIN))
		&& (GetLastError() != NO_ERROR))
	{
		return NDT_ERROR_SETFILEPOINTER;
	}


	if( 0 == WriteFile(drv, lpBuf, secs_number*0x200ul, &dwRet, NULL) )
	{
		return NDT_ERROR_WRITE;
	}


	if ( secs_number*0x200ul < dwRet )
	{
		return NDT_ERROR_WRITE;
	}

	return NDT_ERROR_SUCCESSFUL;
}

extern "C"	int WINAPI New_nt_WriteDisk_NT(HANDLE drv, DWORD sec_start, DWORD secs_number, const char *lpBuf)
{	
	int		nRetry = 5;
	DWORD	dwReadWriteSpeed = 1;
	DWORD	dwCount;
	DWORD	dwTemp;
	DWORD	dwRet, dwErrCode;
	LPSTR	lpTempBuf = (LPTSTR)lpBuf;
	LARGE_INTEGER	pos, qwTemp;
	pos.QuadPart = sec_start;
	pos.QuadPart *= 0x200;
	
	if (NULL == lpTempBuf)
		return ERROR_INVALID_PARAMETER;
	
	dwRet = 0;
	dwTemp = 0;
	dwCount = secs_number;
	dwErrCode = NO_ERROR;
	
	if( (((DWORD)0xffffffff) == SetFilePointer((HANDLE)drv, pos.LowPart, &pos.HighPart, FILE_BEGIN))
		&& (NO_ERROR != (dwErrCode = GetLastError())))
	{
		DbgOutput(_T("SetFilePointer Failed!"));
		IO_Error_Report("SetFilePointer Failed:", drv, sec_start, secs_number, dwErrCode);
		return ERROR_NEGATIVE_SEEK;
	}
	
	dwReadWriteSpeed = dwCount;
	
	for (DWORD dw = 0; dw < dwCount; dw += dwReadWriteSpeed, dwRet += dwTemp)
	{
		dwReadWriteSpeed = dwCount - dw;
		
		if (!WriteFile((HANDLE)drv, (lpTempBuf + dwRet), (dwReadWriteSpeed * 0x200ul), &dwTemp, NULL))
		{
			dwErrCode = GetLastError();
			while (1)
			{
				qwTemp.QuadPart = pos.QuadPart + dw;
				if ( (((DWORD)0xffffffff) == SetFilePointer((HANDLE)drv, qwTemp.LowPart, &qwTemp.HighPart, FILE_BEGIN))
					&& (NO_ERROR != (dwErrCode = GetLastError())))
				{
					DbgOutput(_T("SetFilePointer Failed!"));
					IO_Error_Report("SetFilePointer Failed:", drv, sec_start+dw, secs_number, dwErrCode);
					return ERROR_NEGATIVE_SEEK;
				}
				
				//DebugOutput(__LINE__, "WriteFile Failed! Try again! Error Code : 0x%08X, Speed = 0x%08X, dwTemp = 0x%08X", dwErrCode, dwReadWriteSpeed, dwTemp);
				dwReadWriteSpeed = dwReadWriteSpeed / 2 +1;
				if (WriteFile((HANDLE)drv, (lpTempBuf + dwRet), (dwReadWriteSpeed * 0x200ul), &dwTemp, NULL))
				{
					dwErrCode = NO_ERROR;
					break;
				}
				else
				{
					dwErrCode = GetLastError();
					if (1 == dwReadWriteSpeed)
					{
						if (2 > nRetry--)
						{
							break;
						}
					}
				}
			}
			
			if (NO_ERROR != dwErrCode)
			{
				DbgOutput(_T("写数据失败！\nError Code : 0x%08X, Speed = %u"), dwErrCode, dwReadWriteSpeed);
				IO_Error_Report("写数据失败！", drv, sec_start + dw, secs_number, dwErrCode);
				return ERROR_WRITE_FAULT;
			}
		}
		dwErrCode = NO_ERROR;
	}
	
	if (dwRet != (secs_number * 0x200ul))
	{
		DbgOutput(_T("WriteFile Failed!\nError Code : 0x%08X, dwRet = 0x%08X, Secs_num = 0x%08X"), dwErrCode, dwRet, secs_number);
		IO_Error_Report("WriteFile Failed(No more data):", drv, sec_start + dw, secs_number, dwErrCode);
		return ERROR_HANDLE_EOF;
	}
	
	return NO_ERROR;
}




/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolDoc

IMPLEMENT_DYNCREATE(CNetDiskToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CNetDiskToolDoc, CDocument)
	//{{AFX_MSG_MAP(CNetDiskToolDoc)
	ON_COMMAND(ID_FILE_TYPE, OnFileType)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolDoc construction/destruction

CNetDiskToolDoc::CNetDiskToolDoc()
{
	// TODO: add one-time construction code here
	static char szBuffer[SECTOR_SIZE + 10];
	pcBuf = szBuffer;
	m_lpCM = NULL;
	m_pSocket = NULL;
	m_pComboBar = NULL;
	m_bLocalData = TRUE;

	m_dwCurSector = -1;
//	AfxMessageBox("CNetDiskToolDoc()");
}

CNetDiskToolDoc::~CNetDiskToolDoc()
{
	if (NULL != m_pSocket)
	{
		m_pSocket->ShutDown();
		m_pSocket->Close();
		delete m_pSocket;
		m_pSocket = NULL;
	}
	delete[] m_lpDI;
}


/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolDoc diagnostics

#ifdef _DEBUG
void CNetDiskToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNetDiskToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolDoc commands

BOOL CNetDiskToolDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!IsModified())
		return TRUE;        // ok to continue

	switch (AfxMessageBox("数据已被更改，是否保存？", MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		OnFileSave();
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	
	return TRUE;
}

BOOL CNetDiskToolDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	OnFileOpen();
	return TRUE;
}

BOOL CNetDiskToolDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	OnFileSave();
	return TRUE;
}

//BOOL CNetDiskToolDoc::OpenDriver(int nDriver)
//{
//	char szDriver[100];
//	sprintf(szDriver, "\\\\.\\PhysicalDrive%d", nDriver);
//
//	if (IsModified())
//	{
//		if(!SaveModified())
//		{
//			return FALSE;
//		}
//	}
//
//	if (INVALID_HANDLE_VALUE == m_hDisk
//		||m_nDriver != nDriver)
//	{
//		CloseHandle(m_hDisk);
//		HANDLE hDisk = CreateFile(szDriver,
//							GENERIC_READ|GENERIC_WRITE,
//							FILE_SHARE_READ|FILE_SHARE_WRITE,
//							NULL,
//							OPEN_EXISTING,
//							FILE_ATTRIBUTE_NORMAL,
//							NULL);
//		
//
//		if (INVALID_HANDLE_VALUE == hDisk)
//		{
//			((CNetDiskToolView *)m_viewList.GetHead())->SetText(NULL, 0);
//			return FALSE;
//		}
//
//		DWORD dwHigh, dwLow;
// 
//		dwLow = GetFileSize(hDisk, &dwHigh);
//		
//		m_hDisk = hDisk;
//		m_nDriver = nDriver;
//		m_dwLowSector = 0;
//
//		if (!OpenSector(m_dwLowSector))
//		{
//			AfxMessageBox("读取扇区失败！");
//			return FALSE;
//		}
//	}
//	
//	sprintf(szDriver, "Physical Drive %d", m_nDriver);
//	SetTitle(szDriver);
//
//	return TRUE;
//}


BOOL CNetDiskToolDoc::WriteSector(int nDisk,
								  DWORD dwSector,
								  DWORD dwSectorCount,
								  const char *szBuffer)
{
	DISK_INFO di;
	CString szSector;

	di = m_lpDI[nDisk];

	ASSERT(m_dwCurSector < di.dwMaxSector);	//写入的扇区偏值应该小于最大扇区数
	ASSERT(m_dwCurSector == dwSector);		//本次写入的扇区偏值应该为上次读取的相同

//	if (dwSector > di.dwMaxSector) 不控制
//	{
//		return FALSE;
//	}

//	if (CB_ERR == m_pComboBar->m_cbSector.FindString(-1, szSector))
//	{
//		m_pComboBar->m_cbSector.AddString(szSector);
//	}

	if (NO_ERROR != New_nt_WriteDisk_NT(di.hDisk, dwSector, dwSectorCount, szBuffer))
	{
		((CNetDiskToolView *)m_viewList.GetHead())->SetText(NULL, 0);
		return FALSE;
	}
	
	((CNetDiskToolView *)m_viewList.GetHead())->SetText((char*)szBuffer, SECTOR_SIZE);
	
	return TRUE;
}


BOOL CNetDiskToolDoc::WriteSector()
{
	DISK_INFO di;
	int nDisk;

	nDisk = m_pComboBar->m_cbDisk.GetCurSel();
	di = m_lpDI[nDisk];

	ASSERT(m_dwCurSector < di.dwMaxSector);

	if (m_bLocalData)
	{
		if (!WriteSector(nDisk, m_dwCurSector, 1, pcBuf))
		{
			CString szMsg;
			szMsg.Format("写取扇区失败！0x%08X", GetLastError());
			AfxMessageBox(szMsg);
			m_pComboBar->m_cbSector.DeleteString(m_pComboBar->m_cbSector.GetCurSel());
			return FALSE;
		}
		GetView()->SetFocus();
	}
	else
	{
		if (NULL != m_pSocket)
		{
			m_lpCM->dwCmdType = NDT_CMD_WRITE_START;
			m_lpCM->dwDiskID = nDisk;
			m_lpCM->dwStartSector = m_dwCurSector;
			m_lpCM->dwConnectType = m_dwConnectType;
			m_lpCM->dwSectorCount = 1;
			
			if (SOCKET_ERROR == m_pSocket->Send(m_lpCM, sizeof(CONVECTION_MSG)))
			{
				AfxMessageBox("发送远程写指令失败！");
			}
		}
	}

	SetModifiedFlag(FALSE);
	return TRUE;
}



BOOL CNetDiskToolDoc::OpenSector(int nDisk, DWORD &dwSector,
								 DWORD dwSectorCount, char *szBuffer)
{
	DISK_INFO di;
//	CString szSector;

	di = m_lpDI[nDisk];
	m_pComboBar->m_cbDisk.SetCurSel(nDisk);	

//	if (dwSector > di.dwMaxSector)
//	{
//		dwSector = di.dwMaxSector;
//	}
	
//	szSector.Format("%X", dwSector);
//	if (!szSector.IsEmpty())
//	{
//		if (CB_ERR == m_pComboBar->m_cbSector.FindString(0, szSector))
//		{
//			m_pComboBar->m_cbSector.AddString(szSector);
//		}
//		m_pComboBar->m_cbSector.SelectString(0, szSector);
//	}
	m_dwCurSector = dwSector;

	if (NDT_ERROR_SUCCESSFUL != New_nt_ReadDisk_NT(di.hDisk, dwSector, dwSectorCount, szBuffer))
	{
		((CNetDiskToolView *)m_viewList.GetHead())->SetText(NULL, 0);
		return FALSE;
	}

	GetView()->SetText(szBuffer, SECTOR_SIZE);

	return TRUE;
}

BOOL CNetDiskToolDoc::OpenSector(LPCSTR strSector, BOOL bUpUI)
{
	CString szSector, strDisk;
	DWORD dwSector = 0;
	DISK_INFO di;
	int nCurSel, nDisk;

	if (IsModified())
	{
		if(!SaveModified())
			return FALSE;
	}

	if (NULL == strSector || strlen(strSector) <= 0)
	{
		m_pComboBar->m_cbSector.GetLBText(m_pComboBar->m_cbSector.GetCurSel(), szSector);
		if (szSector.IsEmpty())
		{
			return FALSE;
		}
	}
	else
	{
		szSector = strSector;		
	}

	nDisk = m_pComboBar->m_cbDisk.GetCurSel();
	di = m_lpDI[nDisk];

	sscanf(szSector, "%X", &dwSector);


//	if (dwSector > di.dwMaxSector)
//	{
//		dwSector = di.dwMaxSector;
//		szSector.Format("%X", dwSector);
//	}

	if (bUpUI)
	{
		UpdateSectorUI(szSector);
		nCurSel = m_pComboBar->m_cbSector.SelectString(-1, szSector);
		ASSERT(CB_ERR != nCurSel);
	}

	if (m_bLocalData)
	{

		if (!OpenSector(nDisk, dwSector, 1, pcBuf))
		{
			AfxMessageBox("读取扇区失败！");
			m_pComboBar->m_cbSector.DeleteString(m_pComboBar->m_cbSector.GetCurSel());
			return FALSE;
		}

		m_pComboBar->m_cbDisk.GetLBText(nDisk, strDisk);
		SetTitle(strDisk);
		GetView()->SetFocus();
	}
	else
	{

		if (NULL != m_pSocket)
		{
			memset(m_lpCM, 0, sizeof(CONVECTION_MSG));
			m_lpCM->dwCmdType = NDT_CMD_READ_START;
			m_lpCM->dwDiskID = nDisk;
			m_lpCM->dwStartSector = dwSector;
			m_lpCM->dwConnectType = m_dwConnectType;
			m_lpCM->dwSectorCount = 1;
			if (SOCKET_ERROR == m_pSocket->Send(m_lpCM, sizeof(CONVECTION_MSG)))
			{
				AfxMessageBox("发送远程访问指令失败！");
				return FALSE;
			}
		}
	}

	SetModifiedFlag(FALSE);

	return TRUE;
}



CNetDiskToolView* CNetDiskToolDoc::GetView()
{
	return (CNetDiskToolView *) m_viewList.GetHead();
}

BOOL CNetDiskToolDoc::ConnectSocket(LPCTSTR strAddress,
									UINT	nPort)
{
	DWORD dwErrCode = GetLastError();

//	AfxSocketInit();
	if (NULL == m_pSocket)
	{
		m_pSocket = new CClientSocket(this);
		if (NULL == m_pSocket)
		{
			AfxMessageBox("内存不足，请关闭一些程序再试！");
			return FALSE;
		}
	}

	if (!m_pSocket->Create())
	{
		dwErrCode = GetLastError();
		delete m_pSocket;
		m_pSocket = NULL;
		//AfxMessageBox(IDS_CREATEFAILED);
		return FALSE;
	}

	while (!m_pSocket->Connect(strAddress, nPort))
	{
		if (IDNO == AfxMessageBox(IDS_RETRYCONNECT, MB_YESNO))
		{
			delete m_pSocket;
			m_pSocket = NULL;
			return FALSE;
		}
	}

	return TRUE;
}

void CNetDiskToolDoc::GetDiskList(CComboBar *pComboBar)
{
	char szDriver[100];

	int		nDriver = 0;
	BOOL	bResult;
	DWORD	dwOutBytes;

	PGET_MEDIA_TYPES pmt = NULL; 
	DISK_GEOMETRY	dg;
	DISK_INFO		df;
	CList<DISK_INFO, DISK_INFO>	DiskInfoList;		

	ASSERT(NULL != m_pComboBar || NULL != pComboBar);

	if (NULL == m_pComboBar && NULL != pComboBar)
		m_pComboBar = pComboBar;

	m_pComboBar->m_cbDisk.ResetContent();
	sprintf(szDriver, "\\\\.\\PhysicalDrive%d", nDriver);

	
	HANDLE hDisk = CreateFile(szDriver,
							GENERIC_READ|GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);

	while (INVALID_HANDLE_VALUE != hDisk)
	{
		
		// 用IOCTL_DISK_GET_DRIVE_GEOMETRY取磁盘参数
		bResult = DeviceIoControl(hDisk,       // 设备句柄
			IOCTL_DISK_GET_DRIVE_GEOMETRY,         // 取磁盘参数
			NULL, 0,                               // 不需要输入数据
			&dg, sizeof(DISK_GEOMETRY),            // 输出数据缓冲区
			&dwOutBytes,                           // 输出数据长度
			(LPOVERLAPPED)NULL);                   // 用同步I/O
		
		// 如果失败，再用IOCTL_STORAGE_GET_MEDIA_TYPES_EX取介质类型参数
		if (!bResult)
		{
			pmt = (GET_MEDIA_TYPES *)new BYTE[MEDIA_INFO_SIZE];
			
			bResult = DeviceIoControl(hDisk,    // 设备句柄
				IOCTL_STORAGE_GET_MEDIA_TYPES_EX,   // 取介质类型参数
				NULL, 0,                            // 不需要输入数据
				pmt, MEDIA_INFO_SIZE,               // 输出数据缓冲区
				&dwOutBytes,                        // 输出数据长度
				(LPOVERLAPPED)NULL);                // 用同步I/O
			
			if (bResult)
			{
				// 注意到结构DEVICE_MEDIA_INFO是在结构DISK_GEOMETRY的基础上扩充的
				// 为简化程序，用memcpy代替如下多条赋值语句：
				// pdg->MediaType = (MEDIA_TYPE)pmt->MediaInfo[0].DeviceSpecific.DiskInfo.MediaType;
				// pdg->Cylinders = pmt->MediaInfo[0].DeviceSpecific.DiskInfo.Cylinders;
				// pdg->TracksPerCylinder = pmt->MediaInfo[0].DeviceSpecific.DiskInfo.TracksPerCylinder;
				// ... ...
				memcpy(&dg, pmt->MediaInfo, sizeof(DISK_GEOMETRY));
			}
			
			delete pmt;
		}		
		
		df.hDisk = hDisk;

		ULARGE_INTEGER qwSector;

		qwSector.QuadPart = dg.Cylinders.QuadPart * dg.TracksPerCylinder * dg.SectorsPerTrack * dg.BytesPerSector / 0x200;
		df.dwMaxSector = qwSector.LowPart;

		DiskInfoList.AddTail(df);

		sprintf(szDriver, "Physical Drive %d", nDriver++);
		m_pComboBar->m_cbDisk.AddString(szDriver);

		sprintf(szDriver, "\\\\.\\PhysicalDrive%d", nDriver);
		hDisk = CreateFile(szDriver,
							GENERIC_READ|GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL, OPEN_EXISTING,
							0, NULL);
	}

	ASSERT(DiskInfoList.GetCount() > 0);

	m_dwDiskCount = DiskInfoList.GetCount();
	m_lpDI = new DISK_INFO[m_dwDiskCount];
	for (UINT i=0; i < m_dwDiskCount; i++)
	{
		m_lpDI[i] = DiskInfoList.GetAt(DiskInfoList.FindIndex(i));
	}

	m_pComboBar->m_cbDisk.SetCurSel(0);
	m_pComboBar->m_cbSector.SetCurSel(0);

}




void CNetDiskToolDoc::OnFileType()
{
	if (!GetDataType())
	{
		if (-1 == m_dwCurSector)
			PostQuitMessage(0);
	}

}

void CNetDiskToolDoc::ProcessPendingRead()
{
	m_pSocket->Receive(m_lpCM, sizeof(CONVECTION_MSG));

//	if (NDT_CT_CONTROL == m_dwConnectType)
//		ASSERT(NDT_CT_SERVER == m_lpCM->dwConnectType);
//
//	else if (NDT_CT_SERVER == m_dwConnectType)
//		ASSERT(NDT_CT_CONTROL == m_lpCM->dwConnectType);

	switch(m_lpCM->dwCmdType)
	{
		case NDT_CMD_ERR_SEND:
		{
			AfxMessageBox("传送数据出错！");
			break;
		}

		case NDT_CMD_CLOSE:
		{
			CloseConncet();
			break;
		}

		case NDT_CMD_DISKINFO:
		{
			CString str;
			SetTitle("已经连接到远程计算机");
			m_dwDiskCount = m_lpCM->dwDiskCount;
			memcpy(m_lpDI, m_lpCM->szSectorData, sizeof(DISK_INFO) * m_dwDiskCount);
			m_pComboBar->m_cbDisk.ResetContent();
			for (DWORD i = 0; i < m_dwDiskCount; i++)
			{
				str.Format("Physical Drive %d", i);
				m_pComboBar->m_cbDisk.AddString(str);
			}
			m_pComboBar->m_cbDisk.SetCurSel(0);
			OpenSector();
			break;
		}

		case NDT_CMD_READ_START:
		{
			NetReadStart();
			break;
		}

		case NDT_CMD_READ_DONE:
		{
			NetReadDone();
			break;
		}

		case NDT_CMD_WRITE_START:
		{
			NetWriteStart();
			break;
		}

		case NDT_CMD_WRITE_DONE:
		{
			NetWriteDone();
			break;
		}

		case NDT_CMD_ERR_PWD:
		{
			AfxMessageBox("密码错误！");
			OnFileType();
			break;
		}

		case NDT_CMD_ERR_UNAME:
		{
			AfxMessageBox("目标用户名不存在！");
			OnFileType();
			break;
		}

		case NDT_CMD_ERR_ALRYEXIST:
		{
			CloseConncet();
			AfxMessageBox("用户名已经存在！");
			OnFileType();
			break;
		}

		case NDT_CMD_LOGIN:
		{
			if (m_bLocalData)
			{
				SetTitle("允许远程计算机连接");
			}
			break;
		}


	default:
		NULL;
	}


}

BOOL CNetDiskToolDoc::NetReadStart()
{
	if (NDT_CT_SERVER == m_dwConnectType)
	{
		CString szSector;
		OpenSector(m_lpCM->dwDiskID,
			m_lpCM->dwStartSector,
			m_lpCM->dwSectorCount,
			(LPSTR)m_lpCM->szSectorData);

		m_lpCM->dwCmdType = NDT_CMD_READ_DONE;
		m_lpCM->dwConnectType = m_dwConnectType;
		m_lpCM->dwStartSector = m_dwCurSector;
		m_pSocket->Send(m_lpCM, sizeof(CONVECTION_MSG));
		return TRUE;
	}
	return FALSE;
}

BOOL CNetDiskToolDoc::NetReadDone()
{
	
	if (NDT_CT_CONTROL == m_dwConnectType)
	{
//		memcpy(pcBuf, m_lpCM->szSectorData, SECTOR_SIZE);
		m_dwCurSector = m_lpCM->dwStartSector;
		GetView()->SetText((LPSTR)m_lpCM->szSectorData, SECTOR_SIZE);
		SetModifiedFlag(FALSE);
		return TRUE;
	}

	return FALSE;
}

BOOL CNetDiskToolDoc::NetWriteStart()
{
	if (NDT_CT_SERVER == m_dwConnectType)
	{
		if(WriteSector(m_lpCM->dwDiskID,
					m_lpCM->dwStartSector,
					m_lpCM->dwSectorCount,
					(LPSTR)m_lpCM->szSectorData))
		{
			m_lpCM->dwCmdType = NDT_CMD_WRITE_DONE;
			m_lpCM->dwConnectType = m_dwConnectType;
			m_pSocket->Send(m_lpCM, sizeof(CONVECTION_MSG));
			return TRUE;
		}

	}
	return FALSE;
}

BOOL CNetDiskToolDoc::NetWriteDone()
{
	if (NDT_CT_CONTROL == m_dwConnectType)
	{
		SetModifiedFlag(FALSE);
		AfxMessageBox("保存数据成功！");
		GetView()->SetText((LPSTR)m_lpCM->szSectorData, SECTOR_SIZE);
		return TRUE;
	}
	return FALSE;
}

BOOL CNetDiskToolDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class

	CloseConncet();
	return CDocument::CanCloseFrame(pFrame);
}

void CNetDiskToolDoc::CloseConncet()
{
	if (m_pSocket != NULL)
	{
		m_lpCM->dwCmdType = NDT_CMD_CLOSE;
		m_pSocket->Send(m_lpCM, sizeof(CONVECTION_MSG));
		m_pSocket->ShutDown();
		m_pSocket->Close();
		delete m_pSocket;
		m_pSocket = NULL;
		SetTitle("连接已断开");
	}
}

void CNetDiskToolDoc::OnFileSave() 
{
	WriteSector();
}

void CNetDiskToolDoc::OnFileSaveAs() 
{
	CString  FilePathName;
	CFileDialog dlg(FALSE);
	if ( IDOK == dlg.DoModal())
		FilePathName = dlg.GetPathName();

	FILE *fl = fopen(FilePathName, "w+b");
	if (NULL != fl)
	{
		if (m_bLocalData)
		{
			fwrite(pcBuf, SECTOR_SIZE, 1, fl);
			GetView()->SetText(pcBuf, SECTOR_SIZE);
		}
		else
		{
			fwrite(m_lpCM->szSectorData, SECTOR_SIZE, 1, fl);
//			fread(m_lpCM->szSectorData, SECTOR_SIZE, 1, fl);
			GetView()->SetText((LPSTR)m_lpCM->szSectorData, SECTOR_SIZE);
		}
//		fwrite(pcBuf, SECTOR_SIZE, 1, fl);
		fclose(fl);
	}
}

void CNetDiskToolDoc::OnFileOpen() 
{
	CString FilePathName;

	if (IsModified())
	{
		if(!SaveModified())
			return;
	}

	CFileDialog dlg(TRUE);
	if ( IDOK == dlg.DoModal())
		FilePathName = dlg.GetPathName();

	FILE *fl = fopen(FilePathName, "rb");
	if (NULL != fl)
	{
		if (m_bLocalData)
		{
			fread(pcBuf, SECTOR_SIZE, 1, fl);
			GetView()->SetText(pcBuf, SECTOR_SIZE);
		}
		else
		{
			fread(m_lpCM->szSectorData, SECTOR_SIZE, 1, fl);
			GetView()->SetText((LPSTR)m_lpCM->szSectorData, SECTOR_SIZE);
		}
		fclose(fl);
//		SetTitle(dlg.GetFileTitle());
	}
}

BOOL CNetDiskToolDoc::OnNewDocument() 
{
	m_strPathName.Empty();
	m_strTitle = "未选择数据源";
	
	return PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(ID_FILE_TYPE, CN_COMMAND), 0);
}

BOOL CNetDiskToolDoc::GetDataType()
{
	CString szConnectType;
	CTypeDlg tdlg;
	tdlg.m_bLocal = m_bLocalData;
	if (NDT_CT_SERVER == m_dwConnectType)
	{
		tdlg.m_bControl = TRUE;
	}
	if (IDOK == tdlg.DoModal())
	{
		m_bLocalData = tdlg.m_bLocal;
		CloseConncet();
		
		if (tdlg.m_bLocal && tdlg.m_bControl)
		{
			m_dwConnectType = NDT_CT_SERVER;
			EnableAdminPrivilege();
			OpenSector();
		}
		else if (!tdlg.m_bLocal)
		{
			m_dwConnectType = NDT_CT_CONTROL;
			memset(pcBuf, 0, SECTOR_SIZE);
		}
		else
		{
			EnableAdminPrivilege();
			GetDiskList();
			OpenSector();
			return TRUE;
		}
		
		CLoginDlg ldlg;
		ldlg.m_bLocalData = m_bLocalData;
		ldlg.m_nPort = 6666;
		ldlg.m_szAddress = "127.0.0.1";
		ldlg.m_szUserName = "matthew";
		ldlg.m_szPassword = "123456";
		
		while (TRUE)
		{
			if (IDOK != ldlg.DoModal())
			{
				return FALSE;
			}
			
			if (ConnectSocket(ldlg.m_szAddress,
				ldlg.m_nPort))
			{
				if (NULL == m_lpCM)
				{
					m_lpCM = new CONVECTION_MSG;
				}
				memset(m_lpCM, 0, sizeof(CONVECTION_MSG));
				m_lpCM->dwCmdType = NDT_CMD_LOGIN;
				m_lpCM->dwConnectType = m_dwConnectType;
				m_lpCM->dwDiskCount = m_dwDiskCount;
				memcpy(m_lpCM->szSectorData, m_lpDI, sizeof(DISK_INFO) * m_dwDiskCount);
				strncpy(m_lpCM->strDestName, ldlg.m_szDestName, 16);
				strncpy(m_lpCM->strUserName, ldlg.m_szUserName, 16);
				strncpy(m_lpCM->strPassWord, ldlg.m_szPassword, 16);
				
				ASSERT(NULL != m_pSocket);
				m_pSocket->Send((char*)m_lpCM, sizeof(CONVECTION_MSG));
				return TRUE;
			}

			if (IDYES != AfxMessageBox(IDS_CONNECTOTHER, MB_YESNO))
			{
				return FALSE;
			}
		}
	}

	return FALSE;
}

void CNetDiskToolDoc::ShortcutOpenSector(BOOL bNext)
{
	char szSector[50];
	if ((!bNext && m_dwCurSector > 0) || (bNext && m_dwCurSector < 0xFFFFFFFF))
	{
		sprintf(szSector, "%X", bNext ? ++m_dwCurSector : --m_dwCurSector);
		m_pComboBar->m_cbSector.SetWindowText(szSector);
		OpenSector(szSector, FALSE);
	}
}

void CNetDiskToolDoc::UpdateSectorUI(LPCTSTR strSector)
{
	int nCurSel;
	CString szTemp;
	CString szSector;

	if (NULL == strSector)
		m_pComboBar->m_cbSector.GetWindowText(szSector);
	else
		szSector = strSector;

	int nCount = m_pComboBar->m_cbSector.GetCount();
	for( nCurSel = 0; nCurSel < nCount; nCurSel++ )
	{
		m_pComboBar->m_cbSector.GetLBText(nCurSel, szTemp);
		if (0 == strcmp(szTemp, szSector))
		{
			break;
		}
	}
	if (nCurSel >= nCount)
	{
		m_pComboBar->m_cbSector.AddString(szSector);
	}

	nCurSel = m_pComboBar->m_cbSector.SelectString(-1, szSector);
	ASSERT(CB_ERR != nCurSel);
}


void CNetDiskToolDoc::EnableAdminPrivilege(BOOL bEnable /* = TRUE */)
{
	HANDLE	hDisk;
	BOOL	bRet;
	DWORD	dwRet;
	hDisk = CreateFile( "\\\\.\\PhysicalDrive0",
		0, 
		FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, 0 , NULL);
	
	if ( INVALID_HANDLE_VALUE == hDisk ) 
	{
		DbgOutput(_T("CreateFile() failed! %s"), FmtMsg(GetLastError()));
		return;
	}
	
	bRet = DeviceIoControl(
		(HANDLE) hDisk,            // handle to volume
		IOCTL_VHDISK_INSTALL_OK,       // dwIoControlCode
		NULL,               // lpInBuffer
		0,                    // nInBufferSize
		NULL,                        // lpOutBuffer
		0,							// nOutBufferSize
		(LPDWORD)&dwRet,				// number of bytes returned
		NULL							// OVERLAPPED structure
		);

	if (!bRet)
	{
		DbgOutput(_T("DeviceIoControl(IOCTL_VHDISK_INSTALL_OK) failed! %s"), FmtMsg(GetLastError()));
	}
	else
	{
		DbgOutput(_T("DeviceIoControl(IOCTL_VHDISK_INSTALL_OK) OK!"));
	}
}