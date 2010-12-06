
#ifndef NET_DISK_TOOL_ERROR_CODE_H
#define NET_DISK_TOOL_ERROR_CODE_H

#pragma pack(1)


typedef struct _CONVECTION_MSG
{
//0x0
	DWORD	dwCmdType;						//命令类型
	DWORD	dwSysErrCode;					//系统错误代码
	DWORD	dwConnectType;					//连接类型
	DWORD	dwDiskID;						//磁盘ID
	DWORD	dwDiskCount;					//磁盘数目
	DWORD	dwStartSector;					//扇区偏移值
	DWORD	dwSectorCount;					//扇区数
	char	strUserName[16];				//本地用户名
	char	strDestName[16];				//目标机器用户名
	char	strPassWord[16];				//目标机器密码
	UCHAR	szSectorData[0x200];			//扇区或其他数据缓冲区
	UCHAR	dwReserve[0xB4];				//保留
//0x300
}CONVECTION_MSG, *LPCONVECTION_MSG;


typedef struct _CONNCET_INFO
{
//0x0
	CSocket			*lpstService;			//服务端Socket指针
	char			strServiceName[16];		//服务端用户名
	char			strPassWord[16];		//控制密码
	CSocket			*lpstControl;			//控制端Socket指针
	char			strControlName[16];		//控制端用户名
	CONVECTION_MSG	ConvectionMsg;			//服务端数据
	DWORD			dwReserve[0x32];		//保留
//0x400
}CONNECT_INFO, *LPCONNECT_INFO;


typedef struct _DISK_INFO
{
	HANDLE	hDisk;
	DWORD	dwMaxSector;
}DISK_INFO, *LPDISK_INFO;


#define MEDIA_INFO_SIZE sizeof(GET_MEDIA_TYPES) + 15 * sizeof(DEVICE_MEDIA_INFO)

// ListCtrl Define
#define NDT_LH_SERVER_SOCKET				0
#define NDT_LH_SERVER_USERNAME				1
#define NDT_LH_SERVER_PASSWROD				2
#define NDT_LH_CONTROL_SOCKET				3
#define NDT_LH_CONTROL_USERNAME				4


// Error Codes
#define NDT_ERROR_SUCCESSFUL		NO_ERROR	//成功
#define NDT_ERROR_FAILDED			0x0001ul	//失败
#define NDT_ERROR_READ				0x0002ul	//读错误
#define NDT_ERROR_WRITE				0x0003ul	//写错误
#define NDT_ERROR_SETFILEPOINTER	0x0004ul	//运行SetFilePointer出错

// Net Connection Type
#define NDT_CT_SERVER				0x2001ul	//服务端
#define NDT_CT_CONTROL				0x2002ul	//控制端

// Net Command Type
#define NDT_CMD_LOGIN				0x1001ul	//登录
#define NDT_CMD_LOGOUT				0x1002ul	//登出
#define NDT_CMD_WRITE_START			0x1003ul	//开始写数据
#define NDT_CMD_WRITE_DONE			0X1004ul	//写完成
#define NDT_CMD_READ_START			0x1005ul	//开始读数据
#define NDT_CMD_READ_DONE			0X1006ul	//读完成
#define NDT_CMD_CLOSE				0x1007ul	//断开连接
#define NDT_CMD_DISKINFO			0x1008ul	//初始化磁盘信息

// Net Error Codes
#define NDT_CMD_ERR_DONE			NO_ERROR	//完成
#define NDT_CMD_ERR_SEND			0x1101ul	//发送数据错误
#define	NDT_CMD_ERR_READ			0x1102ul	//读取数据错误
#define NDT_CMD_ERR_PWD				0x1103ul	//密码错误
#define NDT_CMD_ERR_UNAME			0x1104ul	//没找到指定的用户名
#define NDT_CMD_ERR_ALRYEXIST		0x1105ul	//用户名已经存在

#pragma pack()

#endif //NET_DISK_TOOL_ERROR_CODE_H