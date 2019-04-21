#include "Tracert.h"
#include <qdebug.h>
#include <qsystemtrayicon.h>
#include <QElapsedTimer>
//IP���ݱ��ײ�
typedef struct
{
	unsigned char hdr_len : 4;  // �ײ�����
	unsigned char version : 4;  // �汾��
	unsigned char tos;   // ��������
	unsigned short total_len;  // �ܳ���
	unsigned short identifier;  // ��ʶ
	unsigned short frag_and_flags; //��־��ƫ�� 
	unsigned char ttl;   // ����ʱ��
	unsigned char protocol;  // Э��
	unsigned short checksum;  // �ײ�У���
	unsigned long sourceIP;  // ԴIP
	unsigned long destIP;   // Ŀ��IP
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


//ICMP�����ֶ�
const BYTE ICMP_ECHO_REQUEST = 8; //�������
const BYTE ICMP_ECHO_REPLY = 0; //����Ӧ��
const BYTE ICMP_TIMEOUT = 11; //���䳬ʱ
const DWORD DEF_ICMP_TIMEOUT = 3000; //Ĭ�ϳ�ʱʱ�䣬��λms
const int DEF_ICMP_DATA_SIZE = 32; //Ĭ��ICMP���ݲ��ֳ���
const int MAX_ICMP_PACKET_SIZE = 1024; //���ICMP���ݱ��Ĵ�С
const int DEF_MAX_HOP = 30;    //�����վ��

const int SEND_ICMP_TIMES = 4;		//����icmp���Ĵ���������ͳ�ƾ�ֵ�ͷ���

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
	char * ar[] = { "΢�Ź��ںţ���֮ͤ","" };
	ui.lineEdit->setText("gitee.com");

	QIcon icon = QIcon("icon.png"); //png,ico,svg,bmp���ָ�ʽ���ߴ������ƣ��Ƽ�60x60��������ͼ���������ԡ����ԡ���Դ·��
	QSystemTrayIcon trayIcon = new QSystemTrayIcon(this);
	trayIcon.setIcon(icon);
	trayIcon.setToolTip("a trayicon example"); //��ʾ����
	QString title = "tip title";
	QString text = "this is a tray tip";
	trayIcon.show();
	trayIcon.showMessage(title, text, QSystemTrayIcon::Information, 5000); //���һ������Ϊ��ʾʱ����Ĭ��10000����10s


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
	//��ʼ��winsock2����
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cerr << "init WinSock2.DLL Failed" << endl;
		return -1;
	}
	//�������в���ת��ΪIP��ַ
	u_long ulDestIP = inet_addr(argv[1]); \
		//��������ΪIP��ַ��������������
		if (ulDestIP == INADDR_NONE)
		{
			//ת�����ɹ�ʱ����������
			hostent* pHostent = gethostbyname(argv[1]);
			if (pHostent)
			{
				//��ȡIP��ַ
				ulDestIP = (*(in_addr*)pHostent->h_addr).s_addr;
				//�����Ļ��Ϣ
				qDebug() << "The IP address of ultimate destination node:" << inet_ntoa(*(in_addr*)(&ulDestIP)) << endl;
				ui.RIP->setText(inet_ntoa(*(in_addr*)(&ulDestIP)));
			}
			else //����������ʧ��
			{
				cerr << "Failed to resolve the host name!" << argv[1] << endl;
				ui.RIP->setText(argv[1]);
				WSACleanup();
				return -1;
			}
		}
		else//Ϊֱ�ӵ�IP��ַ
		{
			//�����Ļ��Ϣ
			qDebug() << "The IP address of ultimate destination node:" << argv[1] << endl;
		}

	//���Ŀ��Socket��ַ
	sockaddr_in destSockAddr;
	//��ȡ����IP��ַ
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

	//ʹ��ICMPЭ�鴴��Raw Socket SOCK_RAW��ʾʹ��ԭʼ�׽��ֵķ�ʽ
	SOCKET sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sockRaw == INVALID_SOCKET)
	{
		cerr << "ICMP failed to create scocked" << endl;
		WSACleanup();
		return -1;
	}

	//���ö˿�����
	int iTimeout = DEF_ICMP_TIMEOUT;
	if (setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeout, sizeof(iTimeout)) == SOCKET_ERROR)
	{
		cerr << "Timeout!" << endl;
		closesocket(sockRaw);
		WSACleanup();
		return -1;
	}
	//����ICMP�����ͻ������ͽ��ջ�����
	char IcmpSendBuf[sizeof(ICMP_HEADER) + DEF_ICMP_DATA_SIZE];
	memset(IcmpSendBuf, 0, sizeof(IcmpSendBuf));
	char IcmpRecvBuf[MAX_ICMP_PACKET_SIZE];
	memset(IcmpRecvBuf, 0, sizeof(IcmpRecvBuf));

	//�������͵�ICMP��
	ICMP_HEADER* pIcmpHeader = (ICMP_HEADER*)IcmpSendBuf;
	pIcmpHeader->type = ICMP_ECHO_REQUEST;
	pIcmpHeader->code = 0;
	pIcmpHeader->id = (USHORT)GetCurrentProcessId();
	memset(IcmpSendBuf + sizeof(ICMP_HEADER), 'E', DEF_ICMP_DATA_SIZE);

	//��ʼ̽��·��
	DECODE_RESULT stDecodeResult;  //����������ṹ�ṹ�壬���ں������·����Ϣ
	BOOL bReachDestHost = FALSE;
	USHORT usSeqNo = 0;
	int iTTL = 1;
	int iMaxHop = DEF_MAX_HOP;
	u_long tmp_dest_addr = 0;
	while (!bReachDestHost && iMaxHop--)
	{
		//QCoreApplication::processEvents();
		showData mydata;

		//����IP���ݱ�ͷ��ttl�ֶ�
		setsockopt(sockRaw, IPPROTO_IP, IP_TTL, (char*)&iTTL, sizeof(iTTL));
		//���ICMP���ݱ�ʣ���ֶ�
		((ICMP_HEADER*)IcmpSendBuf)->cksum = 0;
		((ICMP_HEADER*)IcmpSendBuf)->seq = htons(usSeqNo++);
		((ICMP_HEADER*)IcmpSendBuf)->cksum = this->GenerateChecksum((USHORT*)IcmpSendBuf, sizeof(ICMP_HEADER) + DEF_ICMP_DATA_SIZE);

		//��¼���кź͵�ǰʱ��
		stDecodeResult.usSeqNo = ((ICMP_HEADER*)IcmpSendBuf)->seq;
		stDecodeResult.dwRoundTripTime = GetTickCount();

		//����ѭ��ping�Ĳ����Լ��������
		DWORD *icmp_time_counter = new DWORD[SEND_ICMP_TIMES];

		for (int i = 0; i < SEND_ICMP_TIMES; i++) {
			//QCoreApplication::processEvents();
			//���ó�ʼʱ��
			stDecodeResult.startTimeStamp = GetTickCount();//����icmpѭ����

			//����ICMP��EchoRequest���ݱ�
			if (sendto(sockRaw, IcmpSendBuf, sizeof(IcmpSendBuf), 0, (sockaddr*)&destSockAddr, sizeof(destSockAddr)) == SOCKET_ERROR)
			{
				//���Ŀ���������ɴ���ֱ���˳�
				if (WSAGetLastError() == WSAEHOSTUNREACH)
					qDebug() << "host can not reach,route end." << endl;
					mydata.Statu = "host can not reach,route end.";

				closesocket(sockRaw);
				WSACleanup();
				return 0;
			}
			//����ICMP��EchoReply���ݱ�
			//��Ϊ�յ��Ŀ��ܲ��ǳ������ڴ������ݱ���������Ҫѭ������ֱ���յ���Ҫ���ݻ�ʱ
			sockaddr_in from;
			int iFromLen = sizeof(from);
			int iReadDataLen;

			while (1)
			{
				//�ȴ����ݵ���
				iReadDataLen = recvfrom(sockRaw, IcmpRecvBuf, MAX_ICMP_PACKET_SIZE, 0, (sockaddr*)&from, &iFromLen);
				//QCoreApplication::processEvents();
				if (iReadDataLen != SOCKET_ERROR) //�����ݰ�����
				{
					//����õ������ݰ������������ȷ����������ѭ��������һ��EchoRequest��
					if (DecodeIcmpResponse(IcmpRecvBuf, iReadDataLen, stDecodeResult))
					{
						if (stDecodeResult.dwIPaddr.s_addr == destSockAddr.sin_addr.s_addr)
							bReachDestHost = TRUE;
						//����ÿһ�ε�����ʱ�䵽������
						icmp_time_counter[i] = stDecodeResult.rtt;
						break;
					}
				}
				else if (WSAGetLastError() == WSAETIMEDOUT) //���ճ�ʱ����ӡ�Ǻ�
				{
					//�����timeout ��ô������rttʱ��Ϊ-1����ע�ô�ping���ɹ���������ƽ��ֵ��ʱ��ֱ���Թ�
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

		//�����ҳ�time out��ֵ
		for (int j = 0; j < SEND_ICMP_TIMES; j++) {//4294967295
			if (icmp_time_counter[j] == 4294967295)
				icmp_time_counter[j] = -1;
		}
		//����ƽ��ֵ�Լ�����Ҵ�ӡ���
		unsigned long sum = 0;
		int counter = SEND_ICMP_TIMES;
		for (int j = 0; j < SEND_ICMP_TIMES; j++) {//4294967295
			if (icmp_time_counter[j] >= 0 && icmp_time_counter[j] != 4294967295)
				sum += icmp_time_counter[j];
			else
				counter -= 1;
		}
		unsigned long avg = 0;

		//��־�ŵ�ǰĿ��ip��ַ����ping��
		bool is_reachable = true;

		//�����ǰ��վ����Ϊ·����Ϣ���
		qDebug() << "Router " << iTTL << ": ";//(*(in_addr*)pHostent->h_addr).s_addr inet_ntoa(*(in_addr*)(&ulDestIP))
		mydata.router = QString::number(iTTL);

		u_long source_addr = stDecodeResult.hostinfo.S_un.S_addr;
		u_long dest_addr = stDecodeResult.dwIPaddr.S_un.S_addr;
		if (tmp_dest_addr != dest_addr)
			tmp_dest_addr = dest_addr;
		else
			// Ŀ��ip������
			is_reachable = false;

		unsigned char source_ip[4];//������4��unsigned char������IP�����ݷ�Χ0~255
		unsigned char dest_ip[4];//������4��unsigned char������IP�����ݷ�Χ0~255

		//��ȡԭ��ַip
		for (int j = 0; j < 4; ++j)
		{
			unsigned long temp = source_addr << (2 * j) * 4; //ÿ���Ȱ���Ҫip�����Ƶ������
			temp = temp >> 6 * 4; //Ȼ�������ߵ���λ�Ƶ����λ
			source_ip[j] = (unsigned char)temp; //ǿ��ת����unsigned char����
		}
		//��ȡĿ���ַip
		for (int j = 0; j < 4; ++j)
		{
			unsigned long temp = dest_addr << (2 * j) * 4; //ÿ���Ȱ���Ҫip�����Ƶ������
			temp = temp >> 6 * 4; //Ȼ�������ߵ���λ�Ƶ����λ
			dest_ip[j] = (unsigned char)temp; //ǿ��ת����unsigned char����
		}

		qDebug() << "The RRT between ";
		//���Դ��ַ
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
			//���Ŀ���ַ
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
		else //Ŀ��ip���������*��
			mydata.DestinationIP = "*";
			qDebug() << '*';

		qDebug() << endl;

		//���ÿ�η��͵�icmp����rtt
		qDebug() << '\t';
		for (int j = 0; j < SEND_ICMP_TIMES; j++) {
			if (icmp_time_counter[j] >= 0 && icmp_time_counter[j] != 4294967295)//�ų���ʱ
			{
				mydata.TTL[j] = QString::number(icmp_time_counter[j])+" ms";
				qDebug() << qSetFieldWidth(8) << icmp_time_counter[j] << " ms";
			}
			else {
				mydata.TTL[j] = "* ms";
				qDebug() << qSetFieldWidth(8) << "* ms";
			}
		}
		//����
		qDebug() << endl;
		//���ƽ��ֵ�Լ�
		if (counter != 0)
		{

			avg = sum / counter;
			//���㷽��
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

		//TTLֵ��1
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
	//�����Ļ��Ϣ
	closesocket(sockRaw);
	WSACleanup();
	return 0;
}

//��������У���
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


//����õ������ݱ�
BOOL Tracert::DecodeIcmpResponse(char* pBuf, int iPacketSize, DECODE_RESULT& stDecodeResult)
{
	//������ݱ���С�ĺϷ���
	IP_HEADER* pIpHdr = (IP_HEADER*)pBuf;
	int iIpHdrLen = pIpHdr->hdr_len * 4;
	if (iPacketSize < (int)(iIpHdrLen + sizeof(ICMP_HEADER)))
		return FALSE;
	//����ICMP�����ͼ��id�ֶκ����к���ȷ���Ƿ��ǳ���Ӧ���յ�Icmp��
	ICMP_HEADER* pIcmpHdr = (ICMP_HEADER*)(pBuf + iIpHdrLen);
	USHORT usID, usSquNo;
	if (pIcmpHdr->type == ICMP_ECHO_REPLY)
	{
		usID = pIcmpHdr->id;
		usSquNo = pIcmpHdr->seq;
	}
	else if (pIcmpHdr->type == ICMP_TIMEOUT)
	{
		char* pInnerIpHdr = pBuf + iIpHdrLen + sizeof(ICMP_HEADER);  //�غ��е�IPͷ
		int iInnerIPHdrLen = ((IP_HEADER*)pInnerIpHdr)->hdr_len * 4;//�غ��е�IPͷ��
		ICMP_HEADER* pInnerIcmpHdr = (ICMP_HEADER*)(pInnerIpHdr + iInnerIPHdrLen);//�غ��е�ICMPͷ
		usID = pInnerIcmpHdr->id;
		usSquNo = pInnerIcmpHdr->seq;
	}
	else
		return FALSE;
	if (usID != (USHORT)GetCurrentProcessId() || usSquNo != stDecodeResult.usSeqNo)
		return FALSE;
	//������ȷ�յ���ICMP���ݱ�
	if (pIcmpHdr->type == ICMP_ECHO_REPLY ||
		pIcmpHdr->type == ICMP_TIMEOUT)
	{
		//���ؽ�����
		stDecodeResult.dwIPaddr.s_addr = pIpHdr->sourceIP;
		stDecodeResult.hostinfo.s_addr = pIpHdr->destIP;
		stDecodeResult.rtt = GetTickCount() - stDecodeResult.startTimeStamp;
		//qDebug()<<"log: "<<stDecodeResult.rtt<<endl;
		return TRUE;
	}
	return FALSE;
}