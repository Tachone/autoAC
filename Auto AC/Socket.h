#include "Htmlparse.h"

extern SOCKET sock;
extern char host[200];
extern char othPath[300];
extern string ResCode;
extern int ProblemID;
extern char s[200];

void SendCode(char *host, char *othPath, string &Code)  //发送最终的Code至服务器
{
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cout << "建立socket失败！ 错误码： " << WSAGetLastError() << endl;
		return;
	}
	sockaddr_in sa = { AF_INET };
	int n = bind(sock, (sockaddr*)&sa, sizeof(sa));
	if (n == SOCKET_ERROR)
	{
		cout << "bind函数失败！ 错误码： " << WSAGetLastError() << endl;
		return;
	}

	struct hostent  *p = gethostbyname(host);
	if (p == NULL)
	{
		cout << "主机解析错误! 错误码： " << WSAGetLastError() << endl;
		return;
	}
	sa.sin_port = htons(80);
	memcpy(&sa.sin_addr, p->h_addr, 4); //

	n = connect(sock, (sockaddr*)&sa, sizeof(sa));
	if (n == SOCKET_ERROR)
	{
		cout << "connect函数失败！ 错误码： " << WSAGetLastError() << endl;
		return;
	}

	string  Typee = "\r\nContent-Type: application/x-www-form-urlencoded";
	string ConLen = "\r\nContent-Length: ";
	_itoa(ProblemID, s, 10);

	//string ElseInfo = "\r\nCache-Control: max-age=0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8O\r\nOrigin: http://acm.hdu.edu.cn\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36\r\nReferer: http://acm.hdu.edu.cn/submit.php?pid=1003\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: zh-CN,zh;q=0.8";
	string ElseInfo = "\r\nCache-Control: max-age=0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8O\r\nOrigin: http://acm.hdu.edu.cn\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36\r\nReferer: http://acm.hdu.edu.cn/submit.php?pid=";
	ElseInfo = ElseInfo + (string)s + "\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: zh-CN,zh;q=0.8";
	//向服务器发送POST请求 

	string HeaderP = "check=0&problemid=" + (string)s;
	HeaderP += "&language=2&usercode=";

	ResCode = HeaderP + ResCode;
	char s[300];
	_itoa(ResCode.length(), s, 10);  /////??????
	string Cookie = "exesubmitlang=2; PHPSESSID=btv0707556tk01jk49f2jn1b97; CNZZDATA1254072405=385429082-1445151305-http%253A%252F%252Facm.hdu.edu.cn%252F%7C1446104055";
	string  reqInfo = "POST " + (string)othPath + " HTTP/1.1\r\nHost: " + (string)host + ElseInfo + Typee + ConLen + (string)s + "\r\nCookie: " + Cookie + "\r\nConnection:Close\r\n\r\n" + ResCode;

	if (SOCKET_ERROR == send(sock, reqInfo.c_str(), reqInfo.size(), 0))
	{
		cout << "send error! 错误码： " << WSAGetLastError() << endl;
		closesocket(sock);
	}

	cout << reqInfo;
}


void SendRequst(char *host, char *othPath)  //请求建立连接
{
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cout << "建立socket失败！ 错误码： " << WSAGetLastError() << endl;
		return;
	}
	sockaddr_in sa = { AF_INET };
	int n = bind(sock, (sockaddr*)&sa, sizeof(sa));
	if (n == SOCKET_ERROR)
	{
		cout << "bind函数失败！ 错误码： " << WSAGetLastError() << endl;
		return;
	}

	struct hostent  *p = gethostbyname(host);
	if (p == NULL)
	{
		cout << "主机解析错误! 错误码： " << WSAGetLastError() << endl;
		return;
	}
	sa.sin_port = htons(80);
	memcpy(&sa.sin_addr, p->h_addr, 4); //

	n = connect(sock, (sockaddr*)&sa, sizeof(sa));
	if (n == SOCKET_ERROR)
	{
		cout << "connect函数失败！ 错误码： " << WSAGetLastError() << endl;
		return;
	}

	//向服务器发送GET请求 
	string  reqInfo = "GET " + (string)othPath + " HTTP/1.1\r\nHost: " + (string)host + "\r\nConnection:Close\r\n\r\n";
	if (SOCKET_ERROR == send(sock, reqInfo.c_str(), reqInfo.size(), 0))
	{
		cout << "send error! 错误码： " << WSAGetLastError() << endl;
		closesocket(sock);
	}

}










