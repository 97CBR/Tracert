#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Tracert.h"
#include <ws2tcpip.h>

#include <winsock2.h>
#include <math.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32")
#include <iostream>
#include <iomanip>

//ICMP数据首部
typedef struct
{
	BYTE type;  //8位类型
	BYTE code;  //8位代码
	USHORT cksum;  //16位校验和
	USHORT id;   //16位标识符
	USHORT seq;  //16位序列号
} ICMP_HEADER;

//解码结果
typedef struct
{
	USHORT usSeqNo;   //包序列号
	DWORD dwRoundTripTime; //往返时间
	DWORD startTimeStamp;	//起始时间
	in_addr dwIPaddr, hostinfo;  //对端IP地址,本机IP
	DWORD rtt;   		//往返时间

} DECODE_RESULT;

class Tracert : public QMainWindow
{
	Q_OBJECT
		
public:

	Tracert(QWidget *parent = Q_NULLPTR);

public slots:
	void  tracerting();

	

private:
	Ui::TracertClass ui;
	QString color(QString time);
	int Run(int argc, char * argv[]);
	USHORT GenerateChecksum(USHORT * pBuf, int iSize);
	BOOL DecodeIcmpResponse(char * pBuf, int iPacketSize, DECODE_RESULT & stDecodeResult);
};

