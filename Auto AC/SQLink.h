#include "Htmlparse.h" 

_ConnectionPtr pMyConnect;
string strqlk;
extern string StateAns;
extern string StateSapce;
extern string StateTime;
extern string CodeHtml;
extern char s[200];
extern vector <string> blogUrl;

void SQLinkInit()   
{
	CoInitialize(NULL);//初始化Com库
	pMyConnect = NULL;//这是个对象指针，关于对象指针的内容可以百度一下，不过不理解也就算了
	HRESULT hr = pMyConnect.CreateInstance(__uuidof(Connection));
	//将对象指针实例化
	if (FAILED(hr))
	{
		cout << "_ConnectionPtr对象指针实例化失败！" << endl;
		return ;
	}
	_bstr_t strConnect = "Driver={sql server};server=Tach-PC\\SQLEXPRESS;uid=tach1;pwd=123456;database=ProblemSolved";  //SQLSERVER
	//这是连接到SQL SERVER数据库的连接字符串，其中的参数要自己改
	try{
		pMyConnect->Open(strConnect, "", "", NULL);
	}//连接到数据库，要捕捉异常
	catch (_com_error &e){
		cout << "连接数据库异常！" << endl;
		cout << e.ErrorMessage() << endl;
	}

	_RecordsetPtr m_pRecordset;//记录集对象指针，用来执行SQL语句并记录查询结果
	if (FAILED(m_pRecordset.CreateInstance(__uuidof(Recordset))))
	{
		cout << "记录集对象指针实例化失败！" << endl;
		return ;
	}

	char sSql[] = "SELECT* FROM Problem";//这里可以用_variant_t的类型（一种COM变量类型，如下_bstr_t也是）

	try{
			m_pRecordset->Open((_variant_t)sSql,
			pMyConnect.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic, adCmdText);//打开数据库，执行SQL语句
	}
	catch (_com_error &e)
	{

		cout << "执行语句错误" << endl;
		cout << e.ErrorMessage() << endl;
	}

	//SQL语句执行完毕，读取查询结果
	/*try
	{
		m_pRecordset->MoveFirst(); //记录集指针移动到查询结果集的前面
		while (m_pRecordset->EndOfFile == VARIANT_FALSE)
		{
			//这里就读取emp表中的雇员的名字
			char *sName = (char*)(_bstr_t)(m_pRecordset->Fields->GetItem(_variant_t("probnum"))->Value);
			cout << (char*)(_bstr_t)(m_pRecordset->Fields->GetItem(_variant_t("probnum"))->Value) << endl;
			m_pRecordset->MoveNext();
		}
	}
	catch (_com_error &e)
	{
		cout << "读取结果错误" << endl;
		cout << e.ErrorMessage() << endl;
	}*/

}

void InsertProb(int i)
{
	//插入数据
	try{
	//pMyConnect.CreateInstance(__uuidof(Connection));

		char strsql[300] = "insert into Problem values(";
		strcat(strsql, s);
		strcat(strsql, ",'");
		strcat(strsql, blogUrl[i].c_str());
		strcat(strsql, "','");
		strcat(strsql, "hdu");
		strcat(strsql, "')");
		cout << strsql << endl;
		//char strsql[] = "insert into Problem values(1002,'34','hdu')";
		_variant_t RecordsAffected;
		pMyConnect->Execute(_bstr_t(strsql), &RecordsAffected, adCmdText);
	}
	catch (_com_error &e)
	{
		cout << "插入数据错误" << endl;
		cout << e.ErrorMessage() << endl;
	}
	
}


void InsertState(int i)
{
	//插入数据
	try{

		char kk[10];
		_itoa(i+1, kk, 10);
	   strqlk = "";
	   strqlk = strqlk + "insert into CodeState values('" + blogUrl[i] + "',"+(string)kk+",'" + StateAns + "','" + CodeHtml + "','" + StateSapce + "','" + StateTime + "')";

		cout << strqlk << endl;
		//char strsql[] = "insert into Problem values(1002,'34','hdu')";
		_variant_t RecordsAffected;
		pMyConnect->Execute(_bstr_t(strqlk.c_str()), &RecordsAffected, adCmdText);
		//pMyConnect->Execute(_bstr_t(strsql), NULL, adCmdText);
		}
	catch (_com_error &e)
	{
		cout << "插入数据错误" << endl;
		cout << e.ErrorMessage() << endl;
	}
	
}

