#include "Tracert.h"
#include <qdebug.h>
#include <qsystemtrayicon.h>
#include <QElapsedTimer>
//IP数据报首部
typedef struct
{
	unsigned char hdr_len : 4;  // 首部长度
	unsigned char version : 4;  // 版本号
	unsigned char tos;   // 服务类型
	unsigned short total_len;  // 总长度
	unsigned short identifier;  // 标识
	unsigned short frag_and_flags; //标志和偏移 
	unsigned char ttl;   // 生存时间
	unsigned char protocol;  // 协议
	unsigned short checksum;  // 首部校验和
	unsigned long sourceIP;  // 源IP
	unsigned long destIP;   // 目的IP
} IP_HEADER;

LPCSTR ip;

struct showData
{
	QString router = "";
	QString SourceIP = "";
	QString DestinationIP = "";
	QString TTL[5];
	QString Average = "";
	QString Variance = "";
	QString Statu = "";
};


//ICMP类型字段
const BYTE ICMP_ECHO_REQUEST = 8; //请求回显
const BYTE ICMP_ECHO_REPLY = 0; //回显应答
const BYTE ICMP_TIMEOUT = 11; //传输超时
const DWORD DEF_ICMP_TIMEOUT = 3000; //默认超时时间，单位ms
const int DEF_ICMP_DATA_SIZE = 32; //默认ICMP数据部分长度
const int MAX_ICMP_PACKET_SIZE = 1024; //最大ICMP数据报的大小
const int DEF_MAX_HOP = 30;    //最大跳站数

const int SEND_ICMP_TIMES = 4;		//发送icmp报的次数，用于统计均值和方差

using namespace std;

Tracert::Tracert(QWidget *parent)
	: QMainWindow(parent)
{
	//QSystemTrayIcon m_systemTray = new QSystemTrayIcon(this);
	//m_systemTray.setIcon(QIcon(": /Tracert/network."));
	//m_systemTray.setToolTip("SystemTray Program");
	//m_systemTray.show();
	//m_systemTray.showMessage("Tips", "The program is running behind!");
	ui.setupUi(this);
	char * ar[] = { "微信公众号：叙之亭","" };
	ui.lineEdit->setText("gitee.com");

	QIcon icon = QIcon("icon.png"); //png,ico,svg,bmp各种格式，尺寸无限制（推荐60x60），加载图标可以用相对、绝对、资源路径
	QSystemTrayIcon trayIcon = new QSystemTrayIcon(this);
	trayIcon.setIcon(icon);
	trayIcon.setToolTip("a trayicon example"); //提示文字
	QString title = "tip title";
	QString text = "this is a tray tip";
	trayIcon.show();
	trayIcon.showMessage(title, text, QSystemTrayIcon::Information, 5000); //最后一个参数为提示时长，默认10000，即10s


	//this->Run(2,ar);

}
void Tracert::tracerting() {
	string des = ui.lineEdit->text().toStdString();
	char * ar[] = { "marx.cbr.tracert","" };
	ar[1] = (char*)des.data();
	ui.listWidget->clear();

	this->Run(2,ar);
	
}
//192.168.1.1
QString Tracert::color(QString stime) {

	QString jud = stime.replace(" ms", "");
	if (jud=="*")
	{
		return "background-color: rgb(233, 0, 6);";
	}

	int time = stime.replace(" ms", "").toInt();
	
	switch (time/10)
	{
	case  0: {
		return "background-color: rgb(170, 255, 127);";
	}
	case  1: {
		return "background-color: rgb(104, 255, 153);";
	}
	case  2: {
		return "background-color: rgb(104, 255, 153);";
	}
	case  3: {
		return "background-color: rgb(252, 162, 79);";
	}
	case  4: {
		return "background-color: rgb(252, 162, 79);";
	}
	case  5: {
		return "background-color: rgb(237, 66, 10);";
	}
	case  6: {
		return "background-color: rgb(237, 66, 10);";
	}
	default:
	{
		return "background-color: rgb(233, 0, 6);";
		break;
	}
	}
}

int Tracert::Run(int argc, char* argv[]) {

	if (argc != 2)
	{
		cerr << "please enter true arg" << endl;
		return -1;
	}
	//初始化winsock2环境
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cerr << "init WinSock2.DLL Failed" << endl;
		return -1;
	}
	//将命令行参数转换为IP地址
	u_long ulDestIP = inet_addr(argv[1]); \
		//主机名不为IP地址，进行域名解析
		if (ulDestIP == INADDR_NONE)
		{
			//转换不成功时按域名解析
			hostent* pHostent = gethostbyname(argv[1]);
			if (pHostent)
			{
				//获取IP地址
				ulDestIP = (*(in_addr*)pHostent->h_addr).s_addr;
				//输出屏幕信息
				qDebug() << "The IP address of ultimate destination node:" << inet_ntoa(*(in_addr*)(&ulDestIP)) << endl;
				ui.RIP->setText(inet_ntoa(*(in_addr*)(&ulDestIP)));
			}
			else //解析主机名失败
			{
				cerr << "Failed to resolve the host name!" << argv[1] << endl;
				ui.RIP->setText(argv[1]);
				WSACleanup();
				return -1;
			}
		}
		else//为直接的IP地址
		{
			//输出屏幕信息
			qDebug() << "The IP address of ultimate destination node:" << argv[1] << endl;
		}

	//填充目的Socket地址
	sockaddr_in destSockAddr;
	//获取本机IP地址
	PHOSTENT    hostinfo;
	char        name[255];

	if (gethostname(name, sizeof(name)) == 0)
	{
		if ((hostinfo = gethostbyname(name)) != NULL)
		{
			ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			qDebug() << "The IP address of the source node          :" << ip << endl;
		}
	}

	ZeroMemory(&destSockAddr, sizeof(sockaddr_in));
	destSockAddr.sin_family = AF_INET;
	destSockAddr.sin_addr.s_addr = ulDestIP;

	//使用ICMP协议创建Raw Socket SOCK_RAW表示使用原始套接字的方式
	SOCKET sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sockRaw == INVALID_SOCKET)
	{
		cerr << "ICMP failed to create scocked" << endl;
		WSACleanup();
		return -1;
	}

	//设置端口属性
	int iTimeout = DEF_ICMP_TIMEOUT;
	if (setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeout, sizeof(iTimeout)) == SOCKET_ERROR)
	{
		cerr << "Timeout!" << endl;
		closesocket(sockRaw);
		WSACleanup();
		return -1;
	}
	//创建ICMP包发送缓冲区和接收缓冲区
	char IcmpSendBuf[sizeof(ICMP_HEADER) + DEF_ICMP_DATA_SIZE];
	memset(IcmpSendBuf, 0, sizeof(IcmpSendBuf));
	char IcmpRecvBuf[MAX_ICMP_PACKET_SIZE];
	memset(IcmpRecvBuf, 0, sizeof(IcmpRecvBuf));

	//填充待发送的ICMP包
	ICMP_HEADER* pIcmpHeader = (ICMP_HEADER*)IcmpSendBuf;
	pIcmpHeader->type = ICMP_ECHO_REQUEST;
	pIcmpHeader->code = 0;
	pIcmpHeader->id = (USHORT)GetCurrentProcessId();
	memset(IcmpSendBuf + sizeof(ICMP_HEADER), 'E', DEF_ICMP_DATA_SIZE);

	//开始探测路由
	DECODE_RESULT stDecodeResult;  //声明明解码结构结构体，便于后续输出路由信息
	BOOL bReachDestHost = FALSE;
	USHORT usSeqNo = 0;
	int iTTL = 1;
	int iMaxHop = DEF_MAX_HOP;
	u_long tmp_dest_addr = 0;
	while (!bReachDestHost && iMaxHop--)
	{
		//QCoreApplication::processEvents();
		showData mydata;

		//设置IP数据报头的ttl字段
		setsockopt(sockRaw, IPPROTO_IP, IP_TTL, (char*)&iTTL, sizeof(iTTL));
		//填充ICMP数据报剩余字段
		((ICMP_HEADER*)IcmpSendBuf)->cksum = 0;
		((ICMP_HEADER*)IcmpSendBuf)->seq = htons(usSeqNo++);
		((ICMP_HEADER*)IcmpSendBuf)->cksum = this->GenerateChecksum((USHORT*)IcmpSendBuf, sizeof(ICMP_HEADER) + DEF_ICMP_DATA_SIZE);

		//记录序列号和当前时间
		stDecodeResult.usSeqNo = ((ICMP_HEADER*)IcmpSendBuf)->seq;
		stDecodeResult.dwRoundTripTime = GetTickCount();

		//设置循环ping的参数以及结果保存
		DWORD *icmp_time_counter = new DWORD[SEND_ICMP_TIMES];

		for (int i = 0; i < SEND_ICMP_TIMES; i++) {
			//QCoreApplication::processEvents();
			//设置初始时间
			stDecodeResult.startTimeStamp = GetTickCount();//放入icmp循环体

			//发送ICMP的EchoRequest数据报
			if (sendto(sockRaw, IcmpSendBuf, sizeof(IcmpSendBuf), 0, (sockaddr*)&destSockAddr, sizeof(destSockAddr)) == SOCKET_ERROR)
			{
				//如果目的主机不可达则直接退出
				if (WSAGetLastError() == WSAEHOSTUNREACH)
					qDebug() << "host can not reach,route end." << endl;
					mydata.Statu = "host can not reach,route end.";

				closesocket(sockRaw);
				WSACleanup();
				return 0;
			}
			//接收ICMP的EchoReply数据报
			//因为收到的可能并非程序所期待的数据报，所以需要循环接收直到收到所要数据或超时
			sockaddr_in from;
			int iFromLen = sizeof(from);
			int iReadDataLen;

			while (1)
			{
				//等待数据到达
				iReadDataLen = recvfrom(sockRaw, IcmpRecvBuf, MAX_ICMP_PACKET_SIZE, 0, (sockaddr*)&from, &iFromLen);
				//QCoreApplication::processEvents();
				if (iReadDataLen != SOCKET_ERROR) //有数据包到达
				{
					//解码得到的数据包，如果解码正确则跳出接收循环发送下一个EchoRequest包
					if (DecodeIcmpResponse(IcmpRecvBuf, iReadDataLen, stDecodeResult))
					{
						if (stDecodeResult.dwIPaddr.s_addr == destSockAddr.sin_addr.s_addr)
							bReachDestHost = TRUE;
						//设置每一次的往返时间到数组中
						icmp_time_counter[i] = stDecodeResult.rtt;
						break;
					}
				}
				else if (WSAGetLastError() == WSAETIMEDOUT) //接收超时，打印星号
				{
					//如果是timeout 那么设置其rtt时间为-1，标注该次ping不成功，最后计算平均值的时候直接略过
					icmp_time_counter[i] = -1;
					break;
				}
				else
				{
					cerr << "Accepting packet error" << endl;
					closesocket(sockRaw);
					WSACleanup();
					return -1;
				}
			}// end while
		}// end send icmp iteration

		//遍历找出time out的值
		for (int j = 0; j < SEND_ICMP_TIMES; j++) {//4294967295
			if (icmp_time_counter[j] == 4294967295)
				icmp_time_counter[j] = -1;
		}
		//计算平均值以及方差并且打印输出
		unsigned long sum = 0;
		int counter = SEND_ICMP_TIMES;
		for (int j = 0; j < SEND_ICMP_TIMES; j++) {//4294967295
			if (icmp_time_counter[j] >= 0 && icmp_time_counter[j] != 4294967295)
				sum += icmp_time_counter[j];
			else
				counter -= 1;
		}
		unsigned long avg = 0;

		//标志着当前目标ip地址不可ping到
		bool is_reachable = true;

		//输出当前跳站数作为路由信息序号
		qDebug() << "Router " << iTTL << ": ";//(*(in_addr*)pHostent->h_addr).s_addr inet_ntoa(*(in_addr*)(&ulDestIP))
		mydata.router = QString::number(iTTL);

		u_long source_addr = stDecodeResult.hostinfo.S_un.S_addr;
		u_long dest_addr = stDecodeResult.dwIPaddr.S_un.S_addr;
		if (tmp_dest_addr != dest_addr)
			tmp_dest_addr = dest_addr;
		else
			// 目标ip不存在
			is_reachable = false;

		unsigned char source_ip[4];//可以用4个unsigned char来保存IP，数据范围0~255
		unsigned char dest_ip[4];//可以用4个unsigned char来保存IP，数据范围0~255

		//获取原地址ip
		for (int j = 0; j < 4; ++j)
		{
			unsigned long temp = source_addr << (2 * j) * 4; //每次先把需要ip数据移到最左边
			temp = temp >> 6 * 4; //然后把最左边的两位移到最低位
			source_ip[j] = (unsigned char)temp; //强制转换成unsigned char保存
		}
		//获取目标地址ip
		for (int j = 0; j < 4; ++j)
		{
			unsigned long temp = dest_addr << (2 * j) * 4; //每次先把需要ip数据移到最左边
			temp = temp >> 6 * 4; //然后把最左边的两位移到最低位
			dest_ip[j] = (unsigned char)temp; //强制转换成unsigned char保存
		}

		qDebug() << "The RRT between ";
		//输出源地址
		QString temp = "";
		for (int j = 3; j >= 0; j--) {
			if (j != 0)
			{
				temp = temp + QString::number(int(source_ip[j])) + '.';
				qDebug() << int(source_ip[j]) << '.';
			}
			else {
				temp = temp + QString::number(int(source_ip[j]));
				qDebug() << int(source_ip[j]);
			}
		}
		mydata.SourceIP = temp;
		qDebug() << " and ";
		if (is_reachable) {
			temp = "";
			//输出目标地址
			for (int j = 3; j >= 0; j--) {
				if (j != 0) {
					temp = temp + QString::number(int(dest_ip[j])) + '.';
					qDebug() << QString::number( int(dest_ip[j])) << '.';
				}
				else {
					temp = temp + QString::number(int(dest_ip[j]));
					qDebug() << int(dest_ip[j]);
				}
			}
			mydata.DestinationIP = temp;
		}
		else //目标ip不存在输出*号
			mydata.DestinationIP = "*";
			qDebug() << '*';

		qDebug() << endl;

		//输出每次发送的icmp报的rtt
		qDebug() << '\t';
		for (int j = 0; j < SEND_ICMP_TIMES; j++) {
			if (icmp_time_counter[j] >= 0 && icmp_time_counter[j] != 4294967295)//排除超时
			{
				mydata.TTL[j] = QString::number(icmp_time_counter[j])+" ms";
				qDebug() << qSetFieldWidth(8) << icmp_time_counter[j] << " ms";
			}
			else {
				mydata.TTL[j] = "* ms";
				qDebug() << qSetFieldWidth(8) << "* ms";
			}
		}
		//换行
		qDebug() << endl;
		//输出平均值以及
		if (counter != 0)
		{

			avg = sum / counter;
			//计算方差
			sum = 0;
			for (int j = 0; j < SEND_ICMP_TIMES; j++)
				if (icmp_time_counter[j] >= 0 && icmp_time_counter[j] != 4294967295)
					sum += (icmp_time_counter[j] - avg) * (icmp_time_counter[j] - avg);
			unsigned long std = sqrt(sum / counter);
			mydata.Average = QString::number(avg) + " ms";
			mydata.Variance = QString::number(std) + " ms";

			qDebug() << "\t Avg :" << qSetFieldWidth(4) << avg << " ms, Std : " << qSetFieldWidth(4) << std << " ms." << endl;
		}
		else {
			qDebug() << "\t Time out, avg and std of RTT is not exist." << endl;
		}

		//TTL值加1
		iTTL++;
		qDebug() << mydata.SourceIP<<mydata.DestinationIP<<mydata.TTL[0] << mydata.TTL[1] << mydata.TTL[2] << mydata.TTL[3] <<mydata.Average<<mydata.Variance<<mydata.Statu;

		QWidget *content;
		QHBoxLayout *horizontalLayout_2;
		QLabel *A;
		QLabel *B;
		QLabel *C;
		QLabel *D;
		QLabel *E;
		QLabel *F;
		QLabel *G;
		QLabel *H;
		QLabel *I;

		content = new QWidget(this);
		content->setObjectName(QStringLiteral("content"));
		content->setGeometry(QRect(0, 0, 900, 30));
		horizontalLayout_2 = new QHBoxLayout(content);
		horizontalLayout_2->setSpacing(0);
		//horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
		horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
		horizontalLayout_2->setContentsMargins(0, 0, 0, 0);

		A = new QLabel(content);
		A->setObjectName(QStringLiteral("A"));

		horizontalLayout_2->addWidget(A);

		B = new QLabel(content);
		B->setObjectName(QStringLiteral("B"));

		horizontalLayout_2->addWidget(B);

		C = new QLabel(content);
		C->setObjectName(QStringLiteral("C"));

		horizontalLayout_2->addWidget(C);

		D = new QLabel(content);
		D->setObjectName(QStringLiteral("D"));

		horizontalLayout_2->addWidget(D);

		E = new QLabel(content);
		E->setObjectName(QStringLiteral("E"));

		horizontalLayout_2->addWidget(E);

		G = new QLabel(content);
		G->setObjectName(QStringLiteral("G"));

		horizontalLayout_2->addWidget(G);

		F = new QLabel(content);
		F->setObjectName(QStringLiteral("F"));

		horizontalLayout_2->addWidget(F);

		H = new QLabel(content);
		H->setObjectName(QStringLiteral("H"));

		horizontalLayout_2->addWidget(H);

		I = new QLabel(content);
		I->setObjectName(QStringLiteral("I"));

		horizontalLayout_2->addWidget(I);

		horizontalLayout_2->setStretch(0, 1);
		horizontalLayout_2->setStretch(1, 2);
		horizontalLayout_2->setStretch(2, 2);
		horizontalLayout_2->setStretch(3, 1);
		horizontalLayout_2->setStretch(4, 1);
		horizontalLayout_2->setStretch(5, 1);
		horizontalLayout_2->setStretch(6, 1);
		horizontalLayout_2->setStretch(7, 1);
		horizontalLayout_2->setStretch(8, 1);

		A->setText(mydata.router);
		B->setText(mydata.SourceIP);
		C->setText(mydata.DestinationIP);

		D->setText(mydata.TTL[0]);
		D->setStyleSheet(this->color(mydata.TTL[0]));
		E->setText(mydata.TTL[1]);
		E->setStyleSheet(this->color(mydata.TTL[1]));
		F->setText(mydata.TTL[2]);
		F->setStyleSheet(this->color(mydata.TTL[2]));
		G->setText(mydata.TTL[3]);
		G->setStyleSheet(this->color(mydata.TTL[3]));

		H->setText(mydata.Average);
		H->setStyleSheet(this->color(mydata.Average));

		I->setText(mydata.Variance);
		I->setStyleSheet(this->color(mydata.Variance));
		C->setStyleSheet(this->color(mydata.Average));
		QListWidgetItem *items = new QListWidgetItem();

		//192.168.1.1
		//QWidget *itemwidget = ui.centralWidget->findChild<QWidget *>((QString)"dataline");

		////ui.disklist->setDragEnabled(false);
		//ui.disklist->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		items->setSizeHint(QSize(900, 32));
		ui.listWidget->addItem(items);
		ui.listWidget->setItemWidget(items, content);
		ui.listWidget->setCurrentRow(iTTL-2);
		this->ui.listWidget->repaint();
		//this->adjustSize();
		
		//this->ui.listWidget ->update();
		//this->repaint();
		
	}
	//输出屏幕信息
	closesocket(sockRaw);
	WSACleanup();
	return 0;
}

//产生网际校验和
USHORT Tracert::GenerateChecksum(USHORT* pBuf, int iSize)
{
	unsigned long cksum = 0;
	while (iSize > 1)
	{
		cksum += *pBuf++;
		iSize -= sizeof(USHORT);
	}
	if (iSize)
		cksum += *(UCHAR*)pBuf;
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum);
}


//解码得到的数据报
BOOL Tracert::DecodeIcmpResponse(char* pBuf, int iPacketSize, DECODE_RESULT& stDecodeResult)
{
	//检查数据报大小的合法性
	IP_HEADER* pIpHdr = (IP_HEADER*)pBuf;
	int iIpHdrLen = pIpHdr->hdr_len * 4;
	if (iPacketSize < (int)(iIpHdrLen + sizeof(ICMP_HEADER)))
		return FALSE;
	//按照ICMP包类型检查id字段和序列号以确定是否是程序应接收的Icmp包
	ICMP_HEADER* pIcmpHdr = (ICMP_HEADER*)(pBuf + iIpHdrLen);
	USHORT usID, usSquNo;
	if (pIcmpHdr->type == ICMP_ECHO_REPLY)
	{
		usID = pIcmpHdr->id;
		usSquNo = pIcmpHdr->seq;
	}
	else if (pIcmpHdr->type == ICMP_TIMEOUT)
	{
		char* pInnerIpHdr = pBuf + iIpHdrLen + sizeof(ICMP_HEADER);  //载荷中的IP头
		int iInnerIPHdrLen = ((IP_HEADER*)pInnerIpHdr)->hdr_len * 4;//载荷中的IP头长
		ICMP_HEADER* pInnerIcmpHdr = (ICMP_HEADER*)(pInnerIpHdr + iInnerIPHdrLen);//载荷中的ICMP头
		usID = pInnerIcmpHdr->id;
		usSquNo = pInnerIcmpHdr->seq;
	}
	else
		return FALSE;
	if (usID != (USHORT)GetCurrentProcessId() || usSquNo != stDecodeResult.usSeqNo)
		return FALSE;
	//处理正确收到的ICMP数据报
	if (pIcmpHdr->type == ICMP_ECHO_REPLY ||
		pIcmpHdr->type == ICMP_TIMEOUT)
	{
		//返回解码结果
		stDecodeResult.dwIPaddr.s_addr = pIpHdr->sourceIP;
		stDecodeResult.hostinfo.s_addr = pIpHdr->destIP;
		stDecodeResult.rtt = GetTickCount() - stDecodeResult.startTimeStamp;
		//qDebug()<<"log: "<<stDecodeResult.rtt<<endl;
		return TRUE;
	}
	return FALSE;
}