#include <windows.h>
#include <iostream>
#include <string>
//#include <WinSock2.h>
#include <stdio.h>
//#include "unistd.h"
#include <sstream>
#pragma warning(disable: 4005)
using namespace std;
#pragma comment(lib, "ws2_32.lib") /*链接ws2_32.lib动态链接库*/
struct Base64Date6
{
    unsigned int d4 : 6;
    unsigned int d3 : 6;
    unsigned int d2 : 6;
    unsigned int d1 : 6;
};
void color(int a)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
}
char ConvertToBase64(char uc);
void EncodeBase64(char* dbuf, char* buf128, int len);
void sendmail();
void recvmail();
char buff[5000];
string mail = "2414635846";
string pass = "";
int color1 = 12;
int color2 = 10;
int color3 = 14;
int color4 = 30;
void sendthing(SOCKET &sockObj, string payload) {
    color(color2);
    cout << " send:  " << payload;
    send(sockObj, payload.c_str(), (int)payload.length(), 0);
    buff[recv(sockObj, buff, 1000, 0)] = 0;
    color(color1);
    cout << " recv:  " << buff << endl;
}
int main() {
    cout << "--陈驰 2021303090--" << endl;
    cout << "请输入发信人邮箱（需为QQ邮箱，不带‘@qq.com’）:";
    cin >> mail;
    cout << "请输入邮箱授权码:";
    cin >> pass;

    //int i = 0;
    //for (; i < 20; i++){
    //    color(i);
    //    cout << i << endl;
    //}
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 1);
	int err = WSAStartup(wVersionRequested, &wsaData);//进行相应的socket库绑定
	int choice;
	while (1) {
        color(color3);
        cout << "请输入你的选择：1.发  2.收" << endl;
		cin >> choice;
        color(color4);
        cout << "                                                             " << endl;
        color(0);
		if (choice == 1)
			 sendmail();
		else if (choice == 2)
			 recvmail();
		else
			continue;
        color(color4);
        cout << "                                                             " ;
        color(color3);
        cout << endl;
	}
}
void sendmail() {

	string payload;
	SOCKET sockObj;
	sockObj = socket(AF_INET, SOCK_STREAM, 0);//建立socket
	hostent* pHostent;
	pHostent = (hostent*)gethostbyname("smtp.qq.com");
	SOCKADDR_IN ServerAddr;
	memcpy(&ServerAddr.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
	ServerAddr.sin_family = AF_INET;//TCP/IP-IPv4
	ServerAddr.sin_port = htons(25); //25号端口

	cout << "IP of smtp.qq.com:" << (int)ServerAddr.sin_addr.S_un.S_un_b.s_b1 << '.' << (int)ServerAddr.sin_addr.S_un.S_un_b.s_b2 << '.' << (int)ServerAddr.sin_addr.S_un.S_un_b.s_b3 << '.' << (int)ServerAddr.sin_addr.S_un.S_un_b.s_b4 << endl;
	connect(sockObj, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
    color(color1);
	buff[ recv(sockObj, buff, 1000, 0)]=0;
	cout << buff << endl;

	payload = "ehlo qq.com\r\n";
    sendthing(sockObj,payload);

	payload = "AUTH LOGIN\r\n";
    sendthing(sockObj, payload);
    //回应VXNlcm5hbWU6说明等待输入用户名

    char temp1[50] = {0};
    char temp2[50] = { 0 };
    memcpy(temp1, mail.c_str(), mail.length());
    //cout << temp1 << endl;
    EncodeBase64(temp2, temp1, strlen(temp1));
    //cout << temp2 << endl;
    payload = temp2;
    payload+="\r\n";
    sendthing(sockObj, payload);//回应UGFzc3dvcmQ6说明等待输入授权码

    memset(temp1, 0,50);
    memset(temp2, 0,50);
    memcpy(temp1, pass.c_str(), pass.length());
    //cout << temp1 << endl;
    EncodeBase64(temp2, temp1, strlen(temp1));
    //cout << temp2 << endl;
    payload = temp2;
    payload += "\r\n";//写上base64加密后的授权码
    sendthing(sockObj, payload);//回应Authentication successful说明认证成功
    color(color2);
    string succ = "235 Authentication successful\r\n";
    if (buff == succ)
        cout << "授权成功" << endl;
    else { cout << "授权失败，请检查授权码与QQ号是否正确输入" << endl; return; }
    string destaddr,content,topic;
    color(color3);
    cout << "请输入收件人邮箱:" << endl;
    color(15);
	cin >> destaddr;
    color(color3);
    cout << "请输入邮件主题:" << endl;
    color(15);
    cin >> topic;
    color(color3);
    cout << "请输入邮件内容:" << endl;
    color(15);
    cin >> content;

    payload = "MAIL FROM:<"+mail+"@qq.com> \r\nRCPT TO:<" + destaddr + "> \r\n";
    sendthing(sockObj, payload);


    payload= "DATA\r\n";
    sendthing(sockObj, payload);


    payload= "From: " + mail + "@qq.com\r\nTo: " + mail + "\r\nsubject:"+topic+"\r\n\r\n"+content+ "\r\n.\r\n";
    sendthing(sockObj, payload);

    payload = "QUIT\r\n";
    sendthing(sockObj, payload);

    cout << "发送完成" << endl;
}
void recvmail() {
    string payload;
    SOCKET sockObj;
    sockObj = socket(AF_INET, SOCK_STREAM, 0);//建立socket
    hostent* pHostent;
    pHostent = (hostent*)gethostbyname("pop.qq.com");
    SOCKADDR_IN ServerAddr;
    memcpy(&ServerAddr.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
    ServerAddr.sin_family = AF_INET;//TCP/IP-IPv4
    ServerAddr.sin_port = htons(110); //25号端口

    cout << "IP of smtp.qq.com:" << (int)ServerAddr.sin_addr.S_un.S_un_b.s_b1 << '.' << (int)ServerAddr.sin_addr.S_un.S_un_b.s_b2 << '.' << (int)ServerAddr.sin_addr.S_un.S_un_b.s_b3 << '.' << (int)ServerAddr.sin_addr.S_un.S_un_b.s_b4 << endl;
    connect(sockObj, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
    color(color1);
    buff[recv(sockObj, buff, 1000, 0)] = 0;
    cout << buff << endl;

    payload = "user "+mail+"@qq.com\r\n";
    sendthing(sockObj, payload);

    payload = "pass "+pass+"\r\n";
    sendthing(sockObj, payload);

    string succ = "+OK\r\n";
    color(color2);
    if (buff == succ)
        cout << "授权成功" << endl;
    else { cout << "授权失败，请检查授权码与QQ号是否正确输入" << endl; return; }

    payload = "stat\r\n";
    sendthing(sockObj, payload);
    Sleep(10);

    payload = "list\r\n";
    color(color2);
    cout << " send:  " << payload;
    send(sockObj, payload.c_str(), (int)payload.length(), 0);
    Sleep(10);
    buff[recv(sockObj, buff, 1000, 0)] = 0;
    color(15);
    cout << "找到以下选择："<<endl;
    color(color3);
    cout << " recv:  " << buff << endl;
    color(15);
    cout << "请选择查阅邮件的编号：" << endl;
    int num;
    cin >> num;
    payload = "retr "+to_string(num)+"\r\n";
    send(sockObj, payload.c_str(), (int)payload.length(), 0);
    Sleep(10);
    color(color1);
    buff[recv(sockObj, buff, 1000, 0)] = 0;
    cout << buff<<endl;

    payload = "quit\r\n";
    sendthing(sockObj, payload);

    cout << "查询完成" << endl;

}

char ConvertToBase64(char uc)
{
    if (uc < 26)
    {
        return 'A' + uc;
    }
    if (uc < 52)
    {
        return 'a' + (uc - 26);
    }
    if (uc < 62)
    {
        return '0' + (uc - 52);
    }
    if (uc == 62)
    {
        return '+';
    }
    return '/';
}

// base64的实现
void EncodeBase64(char* dbuf, char* buf128, int len)
{
    struct Base64Date6* ddd = NULL;
    int           i = 0;
    char          buf[256] = { 0 };
    char* tmp = NULL;
    char          cc = '\0';

    memset(buf, 0, 256);
    for (int i = 0; i < 256; i++)
    {
        buf[i] = buf128[i];
    }
    for (i = 1; i <= len / 3; i++)
    {
        tmp = buf + (i - 1) * 3;
        cc = tmp[2];
        tmp[2] = tmp[0];
        tmp[0] = cc;
        ddd = (struct Base64Date6*)tmp;
        dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
        dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
        dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
        dbuf[(i - 1) * 4 + 3] = ConvertToBase64((unsigned int)ddd->d4);
    }
    if (len % 3 == 1)
    {
        tmp = buf + (i - 1) * 3;
        cc = tmp[2];
        tmp[2] = tmp[0];
        tmp[0] = cc;
        ddd = (struct Base64Date6*)tmp;
        dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
        dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
        dbuf[(i - 1) * 4 + 2] = '=';
        dbuf[(i - 1) * 4 + 3] = '=';
    }
    if (len % 3 == 2)
    {
        tmp = buf + (i - 1) * 3;
        cc = tmp[2];
        tmp[2] = tmp[0];
        tmp[0] = cc;
        ddd = (struct Base64Date6*)tmp;
        dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
        dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
        dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
        dbuf[(i - 1) * 4 + 3] = '=';
    }
    return;
}