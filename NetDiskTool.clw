; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CComboBar
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "netdisktool.h"
LastPage=0

ClassCount=10
Class1=CClientSocket
Class2=CComboBar
Class3=CLoginDlg
Class4=CMainFrame
Class5=CNetDiskToolApp
Class6=CAboutDlg
Class7=CNetDiskToolDoc
Class8=CNetDiskToolView
Class9=CTypeDlg
Class10=CReEdit

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_COMBOBAR
Resource3=IDR_MAINFRAME
Resource4=IDD_DIALOG_LOGIN
Resource5=IDD_DIALOG_TYPE

[CLS:CClientSocket]
Type=0
BaseClass=CAsyncSocket
HeaderFile=ClientSocket.h
ImplementationFile=ClientSocket.cpp
Filter=N
VirtualFilter=q
LastObject=CClientSocket

[CLS:CComboBar]
Type=0
BaseClass=CDialogBar
HeaderFile=ComboBar.h
ImplementationFile=ComboBar.cpp
LastObject=IDC_COMBO_SECTOR
Filter=D
VirtualFilter=dWC

[CLS:CLoginDlg]
Type=0
BaseClass=CDialog
HeaderFile=LoginDlg.h
ImplementationFile=LoginDlg.cpp
LastObject=CLoginDlg
Filter=D
VirtualFilter=dWC

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame
Filter=T
VirtualFilter=fWC

[CLS:CNetDiskToolApp]
Type=0
BaseClass=CWinApp
HeaderFile=NetDiskTool.h
ImplementationFile=NetDiskTool.cpp
LastObject=ID_EDIT_COPY
Filter=N
VirtualFilter=AC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=NetDiskTool.cpp
ImplementationFile=NetDiskTool.cpp
LastObject=CAboutDlg

[CLS:CNetDiskToolDoc]
Type=0
BaseClass=CDocument
HeaderFile=NetDiskToolDoc.h
ImplementationFile=NetDiskToolDoc.cpp
LastObject=ID_FILE_SAVE_AS
Filter=N
VirtualFilter=DC

[CLS:CNetDiskToolView]
Type=0
BaseClass=CView
HeaderFile=NetDiskToolView.h
ImplementationFile=NetDiskToolView.cpp
Filter=W
VirtualFilter=VWC

[CLS:CReEdit]
Type=0
BaseClass=CEdit
HeaderFile=ReEdit.h
ImplementationFile=ReEdit.cpp

[CLS:CTypeDlg]
Type=0
BaseClass=CDialog
HeaderFile=TypeDlg.h
ImplementationFile=TypeDlg.cpp

[DLG:IDD_DIALOG_LOGIN]
Type=1
Class=CLoginDlg
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_SERVER_PORT,edit,1350631552
Control6=IDC_EDIT_SERVER_ADDRESS,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_USER_NAME,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_CTRL_DESTNAME,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT_CTRL_PASSWORD,edit,1350631552

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DIALOG_TYPE]
Type=1
Class=CTypeDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_RADIO_LOCAL,button,1342177289
Control3=IDC_RADIO_REMOTE,button,1342177289
Control4=IDC_CHECK_CONTROL,button,1342242819
Control5=IDC_STATIC,static,1342177298

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_TYPE
Command2=ID_FILE_SAVE
Command3=ID_FILE_OPEN
Command4=ID_FILE_SAVE_AS
Command5=ID_APP_EXIT
Command6=ID_EDIT_UNDO
Command7=ID_EDIT_CUT
Command8=ID_EDIT_COPY
Command9=ID_EDIT_PASTE
Command10=ID_VIEW_STATUS_BAR
Command11=ID_APP_ABOUT
CommandCount=11

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE_AS
Command3=ID_FILE_PRINT
Command4=ID_FILE_SAVE
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_NEXT_PANE
Command8=ID_PREV_PANE
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_EDIT_CUT
Command12=ID_EDIT_UNDO
CommandCount=12

[DLG:IDR_COMBOBAR]
Type=1
Class=CComboBar
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_DISK,combobox,1344340227
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMBO_SECTOR,combobox,1344348418

