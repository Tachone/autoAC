#include "Htmlparse.h"

extern vector <string> blogUrl;
extern string CodeHtml;
extern string ResCode;

extern char host[200];
extern char othPath[300];

extern int ProblemID;

extern string StateAns;
extern string StateSapce;
extern string StateTime;

void regexGetcom(string &allHtml) //提取网页中的csdn博客的url  
{
	blogUrl.clear();
	smatch mat;
	regex pattern("href=\"(http://blog.csdn[^\\s\"]+)\"");

	string::const_iterator start = allHtml.begin();
	string::const_iterator end = allHtml.end();
	while (regex_search(start, end, mat, pattern))
	{
		string msg(mat[1].first, mat[1].second);
		blogUrl.push_back(msg);
		start = mat[0].second;
	}
}


bool analyUrl(char *url) //仅支持http协议,解析出主机和IP地址  
{
	char *pos = strstr(url, "http://");
	if (pos == NULL)
		return false;
	else
		pos += 7;
	sscanf(pos, "%[^/]%s", host, othPath);   //http:// 后一直到/之前的是主机名  
	cout << "host: " << host << "   repath:" << othPath << endl;
	return true;
}

void GetCode(string &allHtml)  //提取代码部分
{
	CodeHtml = "";
	int pos = allHtml.find("#include");
	if (pos != string::npos)
	{
		for (int i = pos; i < allHtml.length(); i++)
		{

			if ((allHtml[i] == '<'&&allHtml[i + 1] == '/'&&allHtml[i + 2] == 't'&&allHtml[i + 3] == 'e'&&allHtml[i + 4] == 'x'&&allHtml[i + 5] == 't') || (allHtml[i] == '<'&&allHtml[i + 1] == '/'&&allHtml[i + 2] == 'p'&&allHtml[i + 3] == 'r'&&allHtml[i + 4] == 'e'&&allHtml[i + 5] == '>'))
			{
				return;
			}
			CodeHtml += allHtml[i];
		}
	}
	else
	{
		cout << "未找到合适的代码！" << endl;
		return;
	}

}

string ReplaceDiv(string &CodeHtml)  //去除html中的一些符号
{
	string ans;
	for (int i = 0; i < CodeHtml.length(); i++)
	{
		if (CodeHtml[i] == '&'&&CodeHtml[i + 1] == 'l'&&CodeHtml[i + 2] == 't'&&CodeHtml[i + 3] == ';')
		{
			ans += '<';
			i += 3;
		}
		else if (CodeHtml[i] == '&'&&CodeHtml[i + 1] == 'g'&&CodeHtml[i + 2] == 't'&&CodeHtml[i + 3] == ';')
		{
			ans += '>';
			i += 3;
		}
		else if (CodeHtml[i] == '/'&&CodeHtml[i + 1] == 'n')
		{
			ans += "\\n";
			i += 1;
		}
		else if (CodeHtml[i] == '&'&&CodeHtml[i + 1] == 'a'&&CodeHtml[i + 2] == 'm'&&CodeHtml[i + 3] == 'p'&&CodeHtml[i + 4] == ';')
		{
			ans += '&';
			i += 4;
		}
		else if (CodeHtml[i] == '&'&&CodeHtml[i + 1] == 'q'&&CodeHtml[i + 2] == 'u'&&CodeHtml[i + 3] == 'o'&&CodeHtml[i + 4] == 't'&&CodeHtml[i + 5] == ';')
		{
			ans += '\"';
			i += 5;
		}
		else if (CodeHtml[i] == '&'&&CodeHtml[i + 1] == 'n'&&CodeHtml[i + 2] == 'b'&&CodeHtml[i + 3] == 's'&&CodeHtml[i + 4] == 'p'&&CodeHtml[i + 5] == ';')
		{
			ans += ' ';
			i += 5;
		}
		else if (CodeHtml[i] == '&'&&CodeHtml[i + 1] == '#'&&CodeHtml[i + 2] == '4'&&CodeHtml[i + 3] == '3'&&CodeHtml[i + 4] == ';')
		{
			ans += '+';
			i += 4;
		}
		else if (CodeHtml[i] == '&'&&CodeHtml[i + 1] == '#'&&CodeHtml[i + 2] == '3'&&CodeHtml[i + 3] == '9'&&CodeHtml[i + 4] == ';')
		{
			ans += '\'';
			i += 4;
		}
		else
			ans += CodeHtml[i];
	}
	return ans;
}

string ASCtoHex(int num) //十进制转换成十六进制
{
	char str[] = "0123456789ABCDEF";
	int temp = num;
	string ans;
	while (temp)
	{
		ans += str[temp % 16];
		temp /= 16;
	}
	ans += '%';
	reverse(ans.begin(), ans.end());
	return ans;
}


string GetRescode(string &CodeHtml)  //转换为http编码
{
	ResCode = "";
	for (int i = 0; i < CodeHtml.length(); i++)
	{
		//if (!isdigit(unsigned(CodeHtml[i])) && !isalpha(unsigned(CodeHtml[i])))
		if ((CodeHtml[i] >= 0 && CodeHtml[i] < 48) || (CodeHtml[i]>57 && CodeHtml[i]<65) || (CodeHtml[i]>90 && CodeHtml[i]<97) || (CodeHtml[i]>122 & CodeHtml[i] <= 127))
		{
			//if (CodeHtml[i] == '\r' && (i + 1) < CodeHtml.length() && CodeHtml[i + 1] == '\n')
			//if (CodeHtml[i] == '\r')
			//{
			//	ResCode += "++%0D";
			//
			//}
			//else if (CodeHtml[i] == '\n')
			if (CodeHtml[i] == '\n')
			{
				ResCode += "%0D%0A";
			}
			else if (CodeHtml[i] == '.' || CodeHtml[i] == '-' || CodeHtml[i] == '*')
				ResCode += CodeHtml[i];

			/*if (CodeHtml[i] == 10)
			ResCode += "%0D%0A";
			*/
			/*else if (CodeHtml[i] >= 0xB0 && (i + 1)<CodeHtml.length()&&CodeHtml[i + 1] >= 0xA1)//判断汉字
			{
			i++;
			ResCode += CodeHtml[i];
			ResCode += CodeHtml[i + 1];
			}*/
			else
			{
				string cur = ASCtoHex(CodeHtml[i]);
				if (cur == "%9")
					ResCode += "++++";
				else if (cur == "%20")
					ResCode += '+';
				else if (cur == "%D")
					ResCode += "++";
				else
					ResCode += cur;
			}

		}
		else
			ResCode += CodeHtml[i];

	}

	return ResCode;
}


void GetResult(string &allHtml, int Prob)  //解析出state.php中的结果，空间，时间
{
	StateAns = "";
	StateSapce = "";
	StateTime = "";
	char d[200];
	_itoa(ProblemID, d, 10);
	strcat(d, "</a>");
	int pos = allHtml.find((string)d);
	int Mpos = pos;
	int Tpos;
	if (Mpos == string::npos)
		return;
	else
	{
		Mpos += 17;
		while (1)
		{
			if (allHtml[Mpos] == '<')
			{
				Tpos = Mpos;
				break;
			}
			StateSapce += allHtml[Mpos];
			Mpos++;
		}
		cout << "使用的空间大小为：" << StateSapce << endl;

	}
	Tpos += 9;
	while (1)
	{
		if (allHtml[Tpos] == '<')
			break;
		StateTime += allHtml[Tpos];
		Tpos++;
	}
	cout << "使用的时间为：" << StateTime << endl;

	if (pos == string::npos)
		return;
	else
	{
		pos = pos - 52;
		int begin;
		while (1)
		{
			if (allHtml[pos] == '>')
			{
				begin = pos;
				break;
			}
			pos--;
		}

		for (int i = begin + 1; allHtml[i] != '<'; i++)
		{
			StateAns += allHtml[i];
		}
	}
	cout << "最终的state界面的答案是：" << "---------------::::::" << StateAns << endl;


}



//UTF-8到GB2312的转换
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}


//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

