#ifndef _DEF_H_
#define _DEF_H_


#define SAFE_RELEASE(a) if (a){delete[] (a); (a)=NULL;}


#define AppLog(str) if (AfxGetApp()->GetMainWnd()){ ((CMainFrame *)AfxGetApp()->GetMainWnd())->m_dlgLog.AppendLog(str, TRUE, FALSE); }


#define COLORBARWIDTH	(16)

#define WM_DEVICE_DISCONNECTED		(WM_APP+1001)
#define WM_DEVICE_ENUMREFRESH		(WM_APP+1002)
#define WM_DHCPSERVER_NOTIFY		(WM_APP+1003)

#define WM_UPDATE_DATA				(WM_APP+1010)
#define WM_UPDATE_LOG				(WM_APP+1011)


#endif