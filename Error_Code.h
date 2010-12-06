
#ifndef NET_DISK_TOOL_ERROR_CODE_H
#define NET_DISK_TOOL_ERROR_CODE_H

#pragma pack(1)


typedef struct _CONVECTION_MSG
{
//0x0
	DWORD	dwCmdType;						//��������
	DWORD	dwSysErrCode;					//ϵͳ�������
	DWORD	dwConnectType;					//��������
	DWORD	dwDiskID;						//����ID
	DWORD	dwDiskCount;					//������Ŀ
	DWORD	dwStartSector;					//����ƫ��ֵ
	DWORD	dwSectorCount;					//������
	char	strUserName[16];				//�����û���
	char	strDestName[16];				//Ŀ������û���
	char	strPassWord[16];				//Ŀ���������
	UCHAR	szSectorData[0x200];			//�������������ݻ�����
	UCHAR	dwReserve[0xB4];				//����
//0x300
}CONVECTION_MSG, *LPCONVECTION_MSG;


typedef struct _CONNCET_INFO
{
//0x0
	CSocket			*lpstService;			//�����Socketָ��
	char			strServiceName[16];		//������û���
	char			strPassWord[16];		//��������
	CSocket			*lpstControl;			//���ƶ�Socketָ��
	char			strControlName[16];		//���ƶ��û���
	CONVECTION_MSG	ConvectionMsg;			//���������
	DWORD			dwReserve[0x32];		//����
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
#define NDT_ERROR_SUCCESSFUL		NO_ERROR	//�ɹ�
#define NDT_ERROR_FAILDED			0x0001ul	//ʧ��
#define NDT_ERROR_READ				0x0002ul	//������
#define NDT_ERROR_WRITE				0x0003ul	//д����
#define NDT_ERROR_SETFILEPOINTER	0x0004ul	//����SetFilePointer����

// Net Connection Type
#define NDT_CT_SERVER				0x2001ul	//�����
#define NDT_CT_CONTROL				0x2002ul	//���ƶ�

// Net Command Type
#define NDT_CMD_LOGIN				0x1001ul	//��¼
#define NDT_CMD_LOGOUT				0x1002ul	//�ǳ�
#define NDT_CMD_WRITE_START			0x1003ul	//��ʼд����
#define NDT_CMD_WRITE_DONE			0X1004ul	//д���
#define NDT_CMD_READ_START			0x1005ul	//��ʼ������
#define NDT_CMD_READ_DONE			0X1006ul	//�����
#define NDT_CMD_CLOSE				0x1007ul	//�Ͽ�����
#define NDT_CMD_DISKINFO			0x1008ul	//��ʼ��������Ϣ

// Net Error Codes
#define NDT_CMD_ERR_DONE			NO_ERROR	//���
#define NDT_CMD_ERR_SEND			0x1101ul	//�������ݴ���
#define	NDT_CMD_ERR_READ			0x1102ul	//��ȡ���ݴ���
#define NDT_CMD_ERR_PWD				0x1103ul	//�������
#define NDT_CMD_ERR_UNAME			0x1104ul	//û�ҵ�ָ�����û���
#define NDT_CMD_ERR_ALRYEXIST		0x1105ul	//�û����Ѿ�����

#pragma pack()

#endif //NET_DISK_TOOL_ERROR_CODE_H