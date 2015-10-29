#include "Htmlparse.h"
#include "Socket.h"
#include "SQLink.h"
vector <string> blogUrl;
string allHtml;
string CodeHtml;
string ResCode;
SOCKET sock;
char host[200];
char othPath[300];
char buf[1024];
int ProblemID;
char s[200];
string StateAns;
string StateSapce;
string StateTime;

int main()
{
	
	SQLinkInit();  //连接数据库

	for (ProblemID = 1000; ProblemID < 5508; ProblemID++) //按题号开始刷题
	{
		strcpy(host, "www.haosou.com");
		strcpy(othPath, "/s?ie=utf-8&shb=1&src=360sou_newhome&q=hdu+");
		_itoa(ProblemID, s, 10);
		strcat(othPath, s);
		strcat(othPath, "+csdn");

		//cout << othPath << endl;
		
		SendRequst(host, othPath);
		int n;
		allHtml = "";
		while ((n = recv(sock, buf, sizeof(buf)-1, 0)) > 0)
		{
			buf[n] = '\0';
			allHtml += buf;
		}
		closesocket(sock);
		regexGetcom(allHtml);  //提取博客网址

		for (int i = 0; i < min(blogUrl.size(),3); i++)
		{
			InsertProb(i);    //信息插入数据库
			cout << blogUrl[i] << endl;

			char tmp[400];
			strcpy(tmp, blogUrl[i].c_str());  ///////这里不能改成i？？？？？
			if (analyUrl(tmp) == false)
			{
				cout << "analyUrl函数错误！" << endl;
				return 0;
			}
			SendRequst(host, othPath);
			//freopen("out.txt", "w", stdout);
			allHtml = "";
			while ((n = recv(sock, buf, sizeof(buf)-1, 0)) > 0)
			{
				buf[n] = '\0';
				allHtml += buf;
			}
			closesocket(sock);
			GetCode(allHtml);

			CodeHtml = ReplaceDiv(CodeHtml);
			char  *p=U2G(CodeHtml.c_str()); //处理汉字，UTF-8编码转为GB

			CodeHtml = string(p);
		   // cout << CodeHtml;
	
			strcpy(tmp, "http://acm.hdu.edu.cn/submit.php?action=submit");
			if (analyUrl(tmp) == false)
			{
				cout << "analyUrl函数错误！" << endl;
				return 0;
			}

			ResCode = GetRescode((string)p);
			
			cout << ResCode;
			SendCode(host, othPath, ResCode);

			//freopen("out.txt", "w", stdout);

			while ((n = recv(sock, buf, sizeof(buf)-1, 0)) > 0)
			{
				buf[n] = '\0';
				cout << buf;
			}

            //新增内容
			//Sleep(4000);
			char temp[400];
			strcpy(temp,"http://acm.hdu.edu.cn/status.php");  ///////这里不能改成i？？？？？
			if (analyUrl(temp) == false)
			{
				cout << "analyUrl函数错误！" << endl;
				return 0;
			}
			SendRequst(host, othPath);
			//freopen("out.txt", "w", stdout);
			allHtml = "";
			while ((n = recv(sock, buf, sizeof(buf)-1, 0)) > 0)
			{
				buf[n] = '\0';
				allHtml += buf;
			}

			GetResult(allHtml, ProblemID);

		    InsertState(i);

			closesocket(sock);

			Sleep(3000);
			//if (StateAns=="Accepted")
			//break;    //暂时性先提交一次
			
		}
	}
	return 0;
}


