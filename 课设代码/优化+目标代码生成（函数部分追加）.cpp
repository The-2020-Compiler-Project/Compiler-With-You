#include <iostream>
#include<vector>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <stack>
#include <queue>

using namespace std;

struct token {
	string name;
	int type;// 1常量，2非临时变量，3是临时变量
	bool active;//活跃信息
	token() { name = ""; type = 0; active = false; }//负责初始化的构造函数
	token(string namet, int typet, bool activet) { name = namet; type = typet; active = activet; }//负责初始化的构造函数
	void clear() { name = ""; type = 0; active = false; }//负责清空函数

};

struct QT
{
	string Operat;//操作符
	token opA;//目标操作数
	token opB;//源操作数
	token res;//结果暂存单元
	int block;//基本块
	QT() { Operat = ""; opA.clear(); opB.clear(); res.clear(); }//负责清空的构造函数
	QT(string Operatt, token opAt, token opBt, token rest) { Operat = Operatt; opA = opAt; opB = opBt; res = rest; }//负责清空的构造函数
	void clear() { Operat = ""; opA.clear(); opB.clear(); res.clear(); }//负责清空的函数
};

struct node
{
	int num; //结点号
	int left; //左结点
	int right; //右结点
	string Operat; //操作符
	token Mark[20]; //标记，0号为主标记
	node() { num = -1; left = -1; right = -1; Operat = ""; }
};

template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

//vector<QT>qt; //原本的一个基本块内的四元式组
vector<QT>groupQT; //存放优化后的一个基本块内的四元式
vector<QT>resQT; //存放最终优化后的所有四元式
vector<QT>block_qt;
vector<QT>ceshi;
vector<node>DAG;
vector<token>SYMBL;
vector<token>SYMBL_ALL;
queue<token>param;
vector<token>RETm;
stack<token*>whstack; //while语义栈
stack<int>whstacknum; //while反填栈
QT specialQT;
token AX; //单寄存器
token BX; //单寄存器
token CX; //单寄存器
token DX; //单寄存器
string namem;//存储函数名字
int ppp;//函数参数个数

int searchNode(string idtemp); //根据标识符寻找到结点标号
int searchNode(string tempOper, string tempB, string tempC); //根据操作符和操作数B，C查找结点标号（双目运算符）
int searchNode(string tempOper, string tempB); //根据操作符和操作数B查找结点标号（单目运算符）
int searchMark(string marktemp, int nodeNum);//在确定NODE内遍历标记找出是否存在此标记
bool searchActive(string temp); //在SYMBL表中根据标识符返回活跃信息
void deleteMark(string nameM, int n); //删除除了第N号结点以外的附标记
int judgeOperate(string t); //返回操作符的类型
string calculate(string tempOper, string tempB, string tempC); //计算两个数运算的值
void attachNode(int number, token tempA); //把标记A附着到结点上
void swapMark(node& temp); //标记互换
QT createQT(string tempOper, token opA, token opB, token res); //创建双目运算符四元式
QT createQT(string tempOper, token opA, token res); //创建单目运算符四元式
int createNode(); //建立新结点，返回最后一个结点的标号
void createDAG(vector<QT>block_qt); //根据给定的四元式组block_qt构造优化的DAG，原本的block_qt不改变，而生成一个DAG
void createQT(); //根据优化的DAG，重组四元式C
void createSYMBL(); //遍历优化后的QT组，生成活跃信息表
void createRes();//遍历groupQT，把四元式加入进resQT
int searchSYMBL(string temp); //在SYMBL表中搜索该标识符是否存在
void changeActive(); //逆序生成活跃信息
void addSYMBL(); //填入一个全局的符号表为DSEG段做准备
void printQT(vector<QT>tempQT); //打印四元式组
void initQT(vector<QT>& tempQT); //测试用构造初始四元式组
void code(QT temp, string lastOper, int i);

void divide()//基本块划分
{
	int num = 1;
	vector<QT>::iterator QTit;
	QT QTtemp;
	for (QTit = ceshi.begin(); QTit != ceshi.end(); QTit++)//遍历一整个基本块
	{
		QTtemp = *QTit;
		QTit->block = num;
		if (QTtemp.Operat == "if" || QTtemp.Operat == "el" || QTtemp.Operat == "ie" || QTtemp.Operat == "do" || QTtemp.Operat == "we" || QTtemp.Operat == "wh" || QTtemp.Operat == "for" || QTtemp.Operat == "fordo" || QTtemp.Operat == "jumpdo" || QTtemp.Operat == "dos" || QTtemp.Operat == "doe" || QTtemp.Operat == "fore" || QTtemp.Operat == "call" || QTtemp.Operat == "RET"|| QTtemp.Operat == "para"|| QTtemp.Operat == "MAIN"|| QTtemp.Operat == "FUNC"|| QTtemp.Operat == "para_n")
		{
			 num++;
		}
	}
	QT abc;
	abc.block = 0;
	ceshi.push_back(abc);
}

/*void divide()//基本块划分
{
	int num = 1;
	vector<QT>::iterator QTit;
	QT QTtemp;
	for (QTit = qt.begin(); QTit != qt.end(); QTit++)//遍历一整个基本块
	{
		QTtemp = *QTit;
		QTit->block = num;
		if (QTtemp.Operat == "if" || QTtemp.Operat == "el" || QTtemp.Operat == "ie" || QTtemp.Operat == "do" || QTtemp.Operat == "we" || QTtemp.Operat == "wh" || QTtemp.Operat == "for" || QTtemp.Operat == "fordo" || QTtemp.Operat == "jumpdo" || QTtemp.Operat == "dos" || QTtemp.Operat == "doe" || QTtemp.Operat == "fore" || QTtemp.Operat == "call" || QTtemp.Operat == "RET")
		{
			 num++;
		}
	}
	QT abc;
	abc.block = 0;
	qt.push_back(abc);
}*/
int searchNode(string idtemp) //根据标识符寻找到结点标号
{
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++)
	{
		if (searchMark(idtemp, it->num) != -1) //找到了
		{
			return it->num;
		}
	}
	return -1;//没找到
}

int searchNode(string tempOper, string tempB, string tempC) //根据操作符和操作数B，C查找结点标号（双目运算符）
{
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++)
	{
		if (it->Operat == tempOper)
		{
			int i = it->left;
			int j = it->right;
			if (searchMark(tempB, i) != -1 && searchMark(tempC, j) != -1) //操作符和操作数都对应正确
			{
				return it->num;
			}
		}
	}
	return -1; //没找到
}

int searchNode(string tempOper, string tempB) //根据操作符和操作数B查找结点标号（单目运算符）
{
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++)
	{
		if (it->Operat == tempOper && it->right == -1) //操作符相同且为单目操作符
		{
			int i = it->left;
			if (searchMark(tempB, i) != -1) //操作符和操作数都对应正确
			{
				return it->num;
			}
		}
	}
	return -1;
}

int searchMark(string marktemp, int nodeNum) //在确定NODE内遍历标记找出是否存在此标记
{
	if (marktemp != "")
	{
		for (int i = 0; i < 20; i++)
		{
			if (DAG[nodeNum].Mark[i].name == marktemp)
				return i;
		}
	}
	return -1;//没找到
}

bool searchActive(string temp) //在SYMBL表中根据标识符返回活跃信息
{
	vector<token>::iterator tokent;
	if (temp == "")
		return false;
	for (tokent = SYMBL.begin(); tokent != SYMBL.end(); tokent++)
	{
		if (tokent->name == temp)
		{
			return tokent->active;
		}
	}
	return false;
}

void deleteMark(string nameM, int n) //删除除了第N号结点以外的附标记
{
	int j;
	vector<node>::iterator delMark;
	for (delMark = DAG.begin(); delMark != DAG.end(); delMark++)
	{
		if (delMark->num != n) //不是N号结点
		{
			j = searchMark(nameM, delMark->num);
			if (j != -1 && j != 0) //主标记免删
			{
				DAG[delMark->num].Mark[j].name = "";
				DAG[delMark->num].Mark[j].type = 0;
			}
		}
	}
}

int judgeOperate(string t) //返回操作符的类型
{
	if (t[0] == '=')
	{
		return 0;
	}
	else if (t[0] == '+' || t[0] == '-' || t[0] == '*' || t[0] == '/')
	{
		return 1;
	}
	else if (t[0] == '!' || t[0] == '<' || t[0] == '>' || t == "!=" || t == "==" || t == ">=" || t == "<=")
	{
		return 2;
	}
	else return 3; //特殊四元式
}

string calculate(string tempOper, string tempB, string tempC)   //计算两个数运算的值
{
	char a;
	char s[10];
	float v = 0;
	a = tempOper[0];
	switch (a)
	{
	case '+':
		v = (float)atof(tempB.c_str()) + (float)atof(tempC.c_str());
		break;
	case '-':
		v = (float)atof(tempB.c_str()) - (float)atof(tempC.c_str());
		break;
	case '*':
		v = (float)atof(tempB.c_str()) * (float)atof(tempC.c_str());
		break;
	case '/':
		v = (float)atof(tempB.c_str()) / (float)atof(tempC.c_str());
		break;
	}
	sprintf_s(s, "%f", v);
	string p(s);
	return p;
}

void attachNode(int number, token tempA) //把标记A附着到结点上
{
	int i; //存储附着到的MARK的序号
	for (i = 0; i < 20; i++) //把A附加到B上
	{
		if (DAG[number].Mark[i].name == "")
		{
			DAG[number].Mark[i].name = tempA.name;
			DAG[number].Mark[i].type = tempA.type;
			break;
		}
	}
}

void swapMark(node& temp) //标记互换
{
	token swap;
	for (int i = 1; i < 20; i++)
	{
		if (temp.Mark[i].type != 0 && temp.Mark[0].type > temp.Mark[i].type) 	//互换
		{
			swap = temp.Mark[0];
			temp.Mark[0] = temp.Mark[i];
			temp.Mark[i] = swap;
		}
	}
}

QT createQT(string tempOper, token opA, token opB, token res) //创建双目运算符四元式
{
	QT temp;
	temp.Operat = tempOper;
	temp.opA.active = opA.active; temp.opA.name = opA.name; temp.opA.type = opA.type;
	temp.opB.active = opB.active; temp.opB.name = opB.name; temp.opB.type = opB.type;
	temp.res.active = res.active; temp.res.name = res.name; temp.res.type = res.type;
	return temp;
}

QT createQT(string tempOper, token opA, token res) //创建单目运算符四元式
{
	QT temp;
	temp.Operat = tempOper;
	temp.opB.clear();
	temp.opA.active = opA.active; temp.opA.name = opA.name; temp.opA.type = opA.type;
	temp.res.active = res.active; temp.res.name = res.name; temp.res.type = res.type;
	return temp;
}

int createNode()//建立新结点，返回最后一个结点的标号
{
	node temp;
	temp.left = -1;
	temp.right = -1;
	temp.Operat = "";
	DAG.push_back(temp);
	int number = (int)DAG.size() - 1;
	DAG[number].num = number;
	return number;
}

void createDAG(vector<QT>block_qt) //根据给定的四元式组block_qt构造优化的DAG，原本的block_qt不改变，而生成一个DAG
{
	DAG.clear();
	vector<QT>::iterator QTit;
	QT QTtemp;
	specialQT.clear();
	for (QTit = block_qt.begin(); QTit != block_qt.end(); QTit++)//遍历一整个基本块
	{
		QTtemp = *QTit;
		int numB, numC;
		numB = searchNode(QTtemp.opA.name);
		numC = searchNode(QTtemp.opB.name);
		if (numB == -1) //没有B这个结点则创建
		{
			numB = createNode();
			DAG[numB].Mark[0].name = QTtemp.opA.name;
			DAG[numB].Mark[0].type = QTtemp.opA.type;
		}
		if (numC == -1) //没有C这个结点则创建
		{
			numC = createNode();
			DAG[numC].Mark[0].name = QTtemp.opB.name;
			DAG[numC].Mark[0].type = QTtemp.opB.type;
		}
		int judgetemp = judgeOperate(QTtemp.Operat);
		if (judgetemp == 0 && QTtemp.opB.name == "") //如果是赋值四元式A=B，B可能是常值
		{
			if (QTtemp.opA.type == 1) //如果是常值表达式A=C
			{
				int i;
				i = stringToNum<int>(QTtemp.opA.name);
				QTtemp.opA.name = to_string(i);
				string tempC = QTtemp.opA.name;
				int k = searchNode(tempC); //寻找C是否已经定义过,用于存放C所在的结点标号
				int l = searchNode(QTtemp.res.name); //寻找A是否已经定义过
				if (k != -1) //C已经定义过
				{
					attachNode(k, QTtemp.res); //把A附着于C上
				}
				else //C没定义过
				{
					int i = createNode();
					DAG[i].Mark[0].name = tempC;
					DAG[i].Mark[0].type = 1;
					DAG[i].Mark[1].name = QTtemp.res.name;
					DAG[i].Mark[1].type = QTtemp.res.type;
					DAG[i].num = i; //创建新结点并初始化
					k = i;
				}
				if (l != -1)//A在N2已经定义过
				{
					deleteMark(QTtemp.res.name, k);
				}
			}
			else //是赋值四元式A=B
			{
				int l = searchNode(QTtemp.res.name); //寻找A是否已经定义过
				attachNode(numB, QTtemp.res); //把A附加在B上
				if (l != -1)//A在N2已经定义过
				{
					deleteMark(QTtemp.res.name, numB);
				}
			}
		}
		else if (judgetemp != 3 && judgetemp != 0) //是1，2类型的表达式
		{
			if (QTtemp.opA.type == 1 && QTtemp.opB.type == 1)//是常值表达式A=C1ωC2
			{
				int i;
				i = stringToNum<int>(QTtemp.opA.name);
				QTtemp.opA.name = to_string(i);
				i = stringToNum<int>(QTtemp.opB.name);
				QTtemp.opB.name = to_string(i);
				string tempC = calculate(QTtemp.Operat, QTtemp.opA.name, QTtemp.opB.name); //计算常值C=C1ωC2
				int k = searchNode(tempC); //寻找C是否已经定义过
				int l = searchNode(QTtemp.res.name); //寻找A是否已经定义过
				if (k != -1) //C定义过
				{
					attachNode(k, QTtemp.res);
				}
				else //C未定义过，申请新结点,并把A附着在C上
				{
					int i = createNode();
					DAG[i].Mark[0].name = tempC;
					DAG[i].Mark[0].type = 1;
					DAG[i].Operat = "";
					DAG[i].Mark[1].name = QTtemp.res.name;
					DAG[i].Mark[1].type = QTtemp.res.type;
					DAG[i].num = i; //创建新结点并初始化
					k = i;
				}
				if (l != -1)
				{
					deleteMark(QTtemp.res.name, k);
				}
			}
			else if (QTtemp.opB.name == "") //A=ωB
			{
				int i = searchNode(QTtemp.Operat, QTtemp.opA.name);
				if (i != -1) //已经存在公共表达式ωB
				{
					attachNode(i, QTtemp.res);
				}
				else //不存在公共表达式
				{
					int j = createNode();
					DAG[j].left = numB;
					DAG[j].Mark[0].name = QTtemp.res.name;
					DAG[j].Mark[0].type = QTtemp.res.type;
					DAG[j].Operat = QTtemp.Operat;
					DAG[j].num = j; //创建新结点并初始化
					i = j;
				}
				deleteMark(QTtemp.res.name, i);
			}
			else //A=BωC
			{
				int i = searchNode(QTtemp.Operat, QTtemp.opA.name, QTtemp.opB.name);
				if (i != -1) //已经存在公共表达式BωC
				{
					attachNode(i, QTtemp.res);
				}
				else //不存在公共表达式
				{
					int j = createNode();
					DAG[j].left = numB;
					DAG[j].right = numC;
					DAG[j].Mark[0].name = QTtemp.res.name;
					DAG[j].Mark[0].type = QTtemp.res.type;
					DAG[j].Operat = QTtemp.Operat;
					DAG[j].num = j; //创建新结点并初始化
					i = j;
				}
				deleteMark(QTtemp.res.name, i);
			}
		}
		else if (judgetemp == 3) //特殊四元式
		{
			specialQT = QTtemp;
		}
	}
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++) //遍历DAG结点交换标记
	{
		swapMark(*it);
	}
}

void createQT() //根据优化的DAG，重组四元式C
{
	//需要增加功能，将类似于A与a这样的变量要改变其标识符用于在生成目标代码时区分变量
	groupQT.clear();
	vector<node>::iterator itdag;
	QT temp;
	for (itdag = DAG.begin(); itdag != DAG.end(); itdag++)
	{
		if (itdag->Operat == "") //是叶结点
		{
			for (int i = 1; i < 20; i++) //Ai为非临时变量则生成Ai=B
			{
				if (itdag->Mark[i].type == 2) //为非临时变量
				{
					temp.Operat = "=";
					temp.opA = itdag->Mark[0];
					temp.res = itdag->Mark[i];
					groupQT.push_back(temp);
					temp.clear();
				}
			}
		}
		else //非叶结点
		{
			if (DAG[itdag->right].Mark[0].name == "")//单目运算符
			{
				temp.Operat = itdag->Operat;
				temp.opA = DAG[itdag->left].Mark[0]; //以主标记参与运算
				temp.res = itdag->Mark[0];
				groupQT.push_back(temp);
				temp.clear();
				for (int i = 1; i < 20; i++) //Ai为非临时变量则生成Ai=A
				{
					if (itdag->Mark[i].type == 2) //为非临时变量
					{
						temp.Operat = "=";
						temp.opA = itdag->Mark[0];
						temp.res = itdag->Mark[i];
						groupQT.push_back(temp);
						temp.clear();
					}
				}
			}
			else //双目运算符
			{
				temp.Operat = itdag->Operat;
				temp.opA = DAG[itdag->left].Mark[0]; //以主标记参与运算
				temp.opB = DAG[itdag->right].Mark[0];
				temp.res = itdag->Mark[0];
				groupQT.push_back(temp);
				temp.clear();
				for (int i = 1; i < 20; i++) //Ai为非临时变量则生成Ai=A
				{
					if (itdag->Mark[i].type == 2) //为非临时变量
					{
						temp.Operat = "=";
						temp.opA = itdag->Mark[0];
						temp.res = itdag->Mark[i];
						groupQT.push_back(temp);
						temp.clear();
					}
				}
			}
		}
	}
	if (specialQT.Operat != "")
	{
		groupQT.push_back(specialQT);
		int i = specialQT.block;
		vector<QT>::iterator it;
		for (it = groupQT.begin(); it != groupQT.end(); it++)
		{
			it->block = i;
		}
	}
}

//void cleanDAG() //害clean啥啊，老出问题不clean了
//{
//	vector<node>::iterator DAGit;
//	for (DAGit = DAG.begin(); DAGit != DAG.end(); )
//	{
//		if (DAGit->Mark[0].name == "" && DAGit->Operat == "" && DAGit->left == -1 && DAGit->right == -1)
//		{
//			DAGit = DAG.erase(DAGit);
//			if (DAGit == DAG.end()) break;
//			DAGit->num--;
//		}
//		else
//		{
//			DAGit++;
//		}
//	}
//}

void printQT(vector<QT>tempQT) //打印四元式组
{
	vector<QT>::iterator it;
	for (it = tempQT.begin(); it != tempQT.end(); it++)
	{
		if (it->opA.name == "")
		{
			cout << "( " << it->Operat << " , " << " _ " << " , " << " _ " << " , " << " _ " << " )" << endl;
		}
		else if (it->opB.name == "")
		{
			if (it->Operat == "")
			{
				cout << "( " << " = " << " , " << it->opA.name << " , " << " _ " << " , " << it->res.name << " )"  << endl;

			}
			else
			{
				if (it->Operat == "if")
				{
					cout << "( " << it->Operat << " , " << it->opA.name << " , " << " _ " << " , " << " _ " << ')' << endl;
				}
				else
					cout << "( " << it->Operat << " , " << it->opA.name  << " , " << " _ " << " , " << it->res.name  << ')' << endl;
			}
		}
		else
		{
			cout << "( " << it->Operat << " , " << it->opA.name << " , " << it->opB.name << " , " << it->res.name << " )"  << endl;
		}
	}
}

void printQT1(vector<QT>tempQT) //打印四元式组
{
	vector<QT>::iterator it;
	for (it = tempQT.begin(); it != tempQT.end(); it++)
	{
		if (it->opA.name == "")
		{
			cout << "( " << it->Operat << " , " << " _ " << " , " << " _ " << " , " << " _ " << " )" << endl;
		}
		else if (it->opB.name == "")
		{
			if (it->Operat == "")
			{
				cout << "( " << " = " << " , " << it->opA.name << '(' << it->opA.active << ')' << " , " << " _ " << " , " << it->res.name << '(' << it->res.active << ')' << ')' << endl;

			}
			else
			{
				if (it->Operat == "if")
				{
					cout << "( " << it->Operat << " , " << it->opA.name << '(' << it->opA.active << ')' << " , " << " _ " << " , " << " _ "  << ')' << endl;
				}
				else
				cout << "( " << it->Operat << " , " << it->opA.name << '(' << it->opA.active << ')' << " , " << " _ " << " , " << it->res.name << '(' << it->res.active << ')' << ')' << endl;
			}
		}
		else
		{
			cout << "( " << it->Operat << " , " << it->opA.name << '(' << it->opA.active << ')' << " , " << it->opB.name << '(' << it->opB.active << ')' << " , " << it->res.name << '(' << it->res.active << ')' << " )" << endl;
		}
	}
}

void printQT3(vector<QT>tempQT) //打印四元式组
{
	vector<QT>::iterator it;
	for (it = tempQT.begin(); it != tempQT.end(); it++)
	{
		if (it->opA.name == "")
		{
			cout << "( " << it->Operat << " , " << " _ " << " , " << " _ " << " , " << " _ " << " )" << it->block << endl;
		}
		else if (it->opB.name == "")
		{
			if (it->Operat == "")
			{
				cout << "( " << " = " << " , " << it->opA.name << " , " << " _ " << " , " << it->res.name << " )" << it->block << endl;

			}
			else
			{
				if (it->Operat == "if")
				{
					cout << "( " << it->Operat << " , " << it->opA.name  << " , " << " _ " << " , " << " _ " << ')' << it->block << endl;
				}
				else
					cout << "( " << it->Operat << " , " << it->opA.name << " , " << " _ " << " , " << it->res.name  << ')' << it->block << endl;
			}
		}
		else
		{
			cout << "( " << it->Operat << " , " << it->opA.name << " , " << it->opB.name << " , " << it->res.name << " )" << it->block << endl;
		}
	}
}
void initQT(vector<QT>& tempQT) //测试用构造初始四元式组
{
	tempQT.clear();
	tempQT.push_back({ "FUNC",{"add",0,false},{"",0,false},{"",0,false} }); //x+y=t1
	tempQT.push_back({ "para_n",{"aadd",0,false},{"",0,false},{"",0,false} }); //x+y=t1
	tempQT.push_back({ "para_n",{"badd",0,false},{"",0,false},{"",0,false} }); //x+y=t1
	tempQT.push_back({ "+",{"a",2,false},{"b",2,false},{"t2",3,false} }); //z*z=t2
	tempQT.push_back({ "/",{"t2",3,false},{"c",2,false},{"x",2,false} }); //t=t2
	tempQT.push_back({ "RET",{"dadd",0,false},{"",0,false},{"",0,false} }); //m-n=t3
	tempQT.push_back({ "para",{"amain",0,false},{"",0,false},{"",0,false} }); //x+y=t1
	tempQT.push_back({ "para",{"bmain",0,false},{"",0,false},{"",0,false} }); //x+y=t1
	tempQT.push_back({ "call",{"add",0,false},{"",0,false},{"",0,false} }); //x+y=t1
	//tempQT.push_back({ "*",{"a",2,false},{"b",2,false},{"t3",3,false} }); //z*z=t2
	//tempQT.push_back({ "-",{"5",1,false},{"t3",3,false},{"x",2,false} }); //t=t2
	//tempQT.push_back({ "wh",{"",0,false},{"",0,false},{"",0,false} }); //x+y=t1
	//tempQT.push_back({ "<",{"a",2,false},{"b",2,false},{"t1",3,false} }); //x+y=t1
	//tempQT.push_back({ "do",{"",0,false},{"",0,false},{"",0,false} }); //z=t1
	//tempQT.push_back({ "+",{"a",2,false},{"b",2,false},{"t2",3,false} }); //z*z=t2
	//tempQT.push_back({ "/",{"t2",3,false},{"c",2,false},{"x",2,false} }); //t=t2
	//tempQT.push_back({ "we",{"",0,false},{"",0,false},{"",0,false} }); //m-n=t3
	//tempQT.push_back({ "*",{"a",2,false},{"b",2,false},{"t3",3,false} }); //z*z=t2
	//tempQT.push_back({ "-",{"5",1,false},{"t3",3,false},{"x",2,false} }); //t=t2
	//tempQT.push_back({ "ie",{"",0,false},{"",0,false},{"",0,false} }); //m-n=t3
	//tempQT.push_back({ "+",{"x",2,false},{"y",2,false},{"t1",3,false} }); //x+y=t1
	//tempQT.push_back({ "=",{"t3",3,false},{"",0,false},{"z",2,false} }); //z=t1
	//tempQT.push_back({ "*",{"z",2,false},{"z",2,false},{"t2",3,false} }); //z*z=t2
	//tempQT.push_back({ "=",{"t2",3,false},{"",0,false},{"t",2,false} }); //t=t2
	//tempQT.push_back({ "<",{"z",2,false},{"t",2,false},{"",0,false} }); //m-n=t3
	//tempQT.push_back({ "=",{"t3",3,false},{"",0,false},{"f",2,false} }); //f=t3
	//tempQT.push_back({ "*",{"f",2,false},{"z",2,false},{"t4",3,false} }); //f*z=t4
	//tempQT.push_back({ "=",{"t4",3,false},{"",0,false},{"p",2,false} }); //p=t4
	//tempQT.push_back({ "+",{"w",2,false},{"v",2,false},{"t5",3,false} }); //w+v=t3
	//tempQT.push_back({ "=",{"t5",3,false},{"",0,false},{"h",2,false} }); //h=t5
    //tempQT.push_back({ "/",{"s",2,false},{"d",2,false},{"t6",3,false} }); //s/d=t6
	//tempQT.push_back({ "=",{"t6",3,false},{"",0,false},{"l",2,false} }); //l=t6
	//tempQT.push_back({ "=",{"1",1,false},{"",0,false},{"A",2,false} }); //A=1
	//tempQT.push_back({ "=",{"2",1,false},{"",0,false},{"B",2,false} }); //B=2
	//tempQT.push_back({ "=",{"5",1,false},{"",0,false},{"B",2,false} }); //B=5
	//tempQT.push_back({ "*",{"2",1,false},{"3.14",1,false},{"t1",3,false} }); //t1=2*3
	//tempQT.push_back({ "+",{"R",2,false},{"r",2,false},{"t2",3,false} }); //t2=R+r
	//tempQT.push_back({ "*",{"t1",3,false},{"t2",3,false},{"t3",3,false} }); //t3=t1*t2
	//tempQT.push_back({ "=",{"t3",3,false},{"",0,false},{"A",2,false} }); //A=t3
	//tempQT.push_back({ "*",{"2",1,false},{"3.14",1,false},{"t4",3,false} }); //t4=2*3.14
	//tempQT.push_back({ "+",{"R",2,false},{"r",2,false},{"t5",3,false} }); //t5=R+r
	//tempQT.push_back({ "*",{"t4",3,false},{"t5",3,false},{"t6",3,false} }); //t6=t4*t5
	//tempQT.push_back({ "-",{"R",2,false},{"r",2,false},{"t7",3,false} }); //t7=R-r
	//tempQT.push_back({ "/",{"t6",3,false},{"t7",3,false},{"t8",3,false} }); //t8=t6/t7
	//tempQT.push_back({ "=",{"t8",3,false},{"",0,false},{"B",2,false} }); //B=t8
}

//int main()
//{
//	//initQT(qt);
//	//QTit = qt.begin();
//	//createDAG();
//	//createQT();
//	//printQT(groupQT);
//	return 0;
//}

void createSYMBL() //遍历优化后的QT组，生成活跃信息表
{
	vector<QT>::iterator it;
	SYMBL.clear();
	for (it = groupQT.begin(); it != groupQT.end(); it++)
	{
		int i = -1;
		if ((i = searchSYMBL(it->opA.name)) == -1) //未填入A操作符则填，已经填入则不改动
		{
			if (it->opA.type == 2) //为非临时变量
			{
				SYMBL.push_back({ it->opA.name,it->opA.type,true });
				it->opA.active = true;
			}
			else if (it->opA.type == 3) //为临时变量
				SYMBL.push_back(it->opA);
		}
		if ((i = searchSYMBL(it->opB.name)) == -1) //未填入B操作符则填，已经填入则不改动
		{
			if (it->opB.type == 2) //为非临时变量
			{
				SYMBL.push_back({ it->opB.name,it->opB.type,true });
				it->opB.active = true;
			}
			else if (it->opB.type == 3) //为临时变量
				SYMBL.push_back(it->opB);
		}
		if ((i = searchSYMBL(it->res.name)) == -1) //未填入B操作符则填，已经填入则不改动
		{
			if (it->res.type == 2) //为非临时变量
			{
				SYMBL.push_back({ it->res.name,it->res.type,true });
				it->res.active = true;
			}
			else if (it->res.type == 3) //为临时变量
				SYMBL.push_back(it->res);
		}
	}
}

void createRes() //遍历groupQT，把四元式加入进resQT
{
	vector<QT>::iterator it;
	for (it = groupQT.begin(); it != groupQT.end(); it++)
	{
		resQT.push_back(*it);
	}
}

int searchSYMBL(string temp) //在SYMBL表中搜索该标识符是否存在
{
	vector<token>::iterator tokent;
	int i = 0;
	if (temp == "")
	{
		return -1;
	}
	for (tokent = SYMBL.begin(); tokent != SYMBL.end(); tokent++, i++)
	{
		if (tokent->name == temp)
		{
			return i;
		}
	}
	return -1;
}

/*void changeActive() //逆序生成活跃信息
{
	int i;
	for (int j = int(groupQT.size())- 1; j >= 0; j--)
	{  if(groupQT[j].Operat != ""&&groupQT[j].Operat!="wh"&& groupQT[j].Operat != "do"&& groupQT[j].Operat != "we"&& groupQT[j].Operat != "if"&& groupQT[j].Operat != "ie"&& groupQT[j].Operat != "el")
		if (groupQT[j].res.name != "") //操作数res存在,当然也不会为常数
		{
			i = searchSYMBL(groupQT[j].res.name); //操作数res存在，不可能不存在于SYMBL表中，故不加判断
			if (i != -1)
			{
				groupQT[j].res.active = SYMBL[i].active;
				SYMBL[i].active = false;
			}
			
		}
		if (groupQT[j].opB.type != 1 && groupQT[j].opB.name != "") //操作数opB存在,且不为常数
		{
			i = searchSYMBL(groupQT[j].opB.name);
			if (i != -1)
			{
				groupQT[j].opB.active = SYMBL[i].active;
				SYMBL[i].active = true;
			}
			
		}
		if (groupQT[j].opA.type != 1 && groupQT[j].opA.name != "") //操作数opA存在,且不为常数
		{
			i = searchSYMBL(groupQT[j].opA.name);
			if (i != -1)
			{
				groupQT[j].opA.active = SYMBL[i].active;
				SYMBL[i].active = true;
			}
			
		}
	}
}*/
void changeActive() //逆序生成活跃信息
{
	vector<QT>::reverse_iterator it;
	int i = 0;
	for (it = groupQT.rbegin(); it != groupQT.rend(); it++)
	{
		if (it->res.name != "") //操作数res存在,当然也不会为常数
		{
			i = searchSYMBL(it->res.name); //操作数res存在，不可能不存在于SYMBL表中，故不加判断
			if (i != -1&&i<SYMBL.size())
			{
				it->res.active = SYMBL[i].active;
				SYMBL[i].active = false;
			}
		}
		if (it->opB.type != 1 && it->opB.name != "") //操作数opB存在,且不为常数
		{
			i = searchSYMBL(it->opB.name);
			if (i != -1 && i < SYMBL.size())
			{
				it->opB.active = SYMBL[i].active;
				SYMBL[i].active = true;
			}
		}
		if (it->opA.type != 1 && it->opA.name != "") //操作数opA存在,且不为常数
		{
			i = searchSYMBL(it->opA.name);
			if (i != -1 && i < SYMBL.size())
			{
				it->opA.active = SYMBL[i].active;
				SYMBL[i].active = true;
			}
		}
	}

}

void addSYMBL() //填入一个全局的符号表为DSEG段做准备
{
	vector<token>::iterator it;
	vector<token>::iterator itS;
	bool exist = false;
	for (it = SYMBL.begin(); it != SYMBL.end(); it++)
	{
		for (itS = SYMBL_ALL.begin(); itS != SYMBL_ALL.end(); itS++)
		{
			if (it->name != "" && it->name == itS->name)
			{
				exist = true;
			}
		}
		if (!exist)
			SYMBL_ALL.push_back(*it);
	}
}

void createCode(vector<QT>resQT) //根据一个四元式组生成一段代码
{
	ofstream file;
	file.open("D:\\byresult\\code.txt", ios::app);
	vector<QT>::iterator it;
	string lastOper = "";
	ppp = 1;
	int i = 1; //四元式序号
	int j = 1;
	for (it = resQT.begin(); it != resQT.end(); it++, i++)
	{
		code(*it, lastOper, i);
		lastOper = it->Operat;
	}
	if (AX.active)
	{
		file << "	MOV	" << AX.name << ",AX" << endl;
	}
	if (BX.active)
	{
		file << "	MOV	" << BX.name << ",BX" << endl;
	}
	if (CX.active)
	{
		file << "	MOV	" << CX.name << ",CX" << endl;
	}
	if (DX.active)
	{
		file << "	MOV	" << DX.name << ",DX" << endl;
	}
}

void code(QT temp, string lastOper, int i) //根据单个四元式生成CODE
{
	ofstream addfile;
	addfile.open("D:\\byresult\\code.txt", ios::app);
	if (temp.Operat == "=")	//赋值语句
	{
		/*if (lastOper == "para_n")
		{
			addfile << "	ASSUME	CS:" << namem << ",DS:" << namem << endl;
			addfile << "    PUSH  " << "AX" << endl;
			addfile << "    PUSH  " << "BX" << endl;
			addfile << "    PUSH  " << "CX" << endl;
			addfile << "    PUSH  " << "DX" << endl;
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name<< ",AX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",BX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",CX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",DX" << endl;
				param.pop();
			}
		}*/
		if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		AX = temp.res;
	}
	else if (temp.Operat == "+") //加法
	{
		 if (lastOper == "para_n")
		 {
			addfile << "	ASSUME	CS:" << namem << ",DS:" << namem << endl;
			addfile << "                PUSH  " << "AX" << endl;
			addfile << "                PUSH  " << "BX" << endl;
			addfile << "                PUSH  " << "CX" << endl;
			addfile << "                PUSH  " << "DX" << endl;
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",AX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",BX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",CX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",DX" << endl;
				param.pop();
			}
		 }
		if (AX.name == temp.opA.name || AX.name == temp.opB.name || BX.name == temp.opA.name || BX.name == temp.opB.name || CX.name == temp.opA.name || CX.name == temp.opB.name || DX.name == temp.opA.name || DX.name == temp.opB.name)
		{
			if (AX.name == temp.opA.name && !(AX.active))
			{
				addfile << "	ADD	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opA.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	ADD	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opB.name && !(AX.active))
			{
				addfile << "	ADD	" << "AX," << temp.opA.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opB.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	ADD	" << "AX," << temp.opA.name << endl;
				AX = temp.res;
			}
			else if (BX.name == temp.opA.name && !(BX.active))
			{
				addfile << "	ADD	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opA.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	ADD	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opB.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	ADD	" << "BX," << temp.opA.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opB.name && !(BX.active))
			{
				addfile << "	ADD	" << "BX," << temp.opA.name << endl;
				BX = temp.res;
			}
			else if (CX.name == temp.opA.name && !(CX.active))
			{
				addfile << "	ADD	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opA.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	ADD	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opB.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	ADD	" << "CX," << temp.opA.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opB.name && !(CX.active))
			{
				addfile << "	ADD	" << "CX," << temp.opA.name << endl;
				CX = temp.res;
			}
			else if (DX.name == temp.opA.name && !(DX.active))
			{
				addfile << "	ADD	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opA.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	ADD	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opB.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	ADD	" << "DX," << temp.opA.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opB.name && !(DX.active))
			{
				addfile << "	ADD	" << "DX," << temp.opA.name << endl;
				DX = temp.res;
			}
		}
		else if (!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	ADD	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	ADD	" << "BX," << temp.opB.name << endl;
			BX = temp.res;
		}
		else if (!(CX.active))
		{
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			addfile << "	ADD	" << "CX," << temp.opB.name << endl;
			CX = temp.res;
		}
		else if (!(DX.active))
		{
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			addfile << "	ADD	" << "DX," << temp.opB.name << endl;
			DX = temp.res;
		}
		else
		{
			addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	ADD	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		/*if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		addfile << "	ADD	" << "AX," << temp.opB.name << endl;
		AX = temp.res;*/
	}
	else if (temp.Operat == "-"&&(temp.opB.name!="")) //减法，这里要加一个符号，还没写
	{
	 if (lastOper == "para_n")
	  {
		addfile << "	ASSUME	CS:" << namem << ",DS:" << namem << endl;
		addfile << "                PUSH  " << "AX" << endl;
		addfile << "                PUSH  " << "BX" << endl;
		addfile << "                PUSH  " << "CX" << endl;
		addfile << "                PUSH  " << "DX" << endl;
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",AX" << endl;
			param.pop();
		}
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",BX" << endl;
			param.pop();
		}
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",CX" << endl;
			param.pop();
		}
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",DX" << endl;
			param.pop();
		}
	  }
		if (AX.name == temp.opA.name || BX.name == temp.opA.name || CX.name == temp.opA.name || DX.name == temp.opA.name)
		{
			if (AX.name == temp.opA.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	SUB	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opA.name && !(AX.active))
			{
				addfile << "	SUB	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (BX.name == temp.opA.name && !(BX.active))
			{
				addfile << "	SUB	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opA.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	SUB	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (CX.name == temp.opA.name && !(CX.active))
			{
				addfile << "	SUB	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opA.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	SUB	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (DX.name == temp.opA.name && !(DX.active))
			{
				addfile << "	SUB	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opA.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	SUB	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}

		}
		else if (!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	SUB	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	SUB	" << "BX," << temp.opB.name << endl;
			BX = temp.res;
		}
		else if (!(CX.active))
		{
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			addfile << "	SUB	" << "CX," << temp.opB.name << endl;
			CX = temp.res;
		}
		else if (!(DX.active))
		{
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			addfile << "	SUB	" << "DX," << temp.opB.name << endl;
			DX = temp.res;
		}
		else
		{
			addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	SUB	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		/*if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	SUB	" << "AX," << temp.opB.name << endl;
		AX = temp.res;*/
	}
	else if (temp.Operat == "*") //乘法
	{
	  if (lastOper == "para_n")
	  {
		addfile << "	ASSUME	CS:" << namem << ",DS:" << namem << endl;
		addfile << "                PUSH  " << "AX" << endl;
		addfile << "                PUSH  " << "BX" << endl;
		addfile << "                PUSH  " << "CX" << endl;
		addfile << "                PUSH  " << "DX" << endl;
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",AX" << endl;
			param.pop();
		}
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",BX" << endl;
			param.pop();
		}
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",CX" << endl;
			param.pop();
		}
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",DX" << endl;
			param.pop();
		}
	  }
		if (AX.name == temp.opA.name || AX.name == temp.opB.name)
		{
			if (AX.name == temp.opA.name && !(AX.active))
			{
				if(DX.active)
					addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	IMUL	" << temp.opB.name << endl;
				addfile << "	MOV	" << temp.res.name << ",AX" << endl;
				addfile << "	MOV	" << temp.res.name << "+2,DX" << endl;
				AX.clear(); DX.clear();
			}
			else if (AX.name == temp.opA.name && AX.active)
			{
				if (DX.active)
					addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	IMUL	" << temp.opB.name << endl;
				addfile << "	MOV	" << temp.res.name << ",AX" << endl;
				addfile << "	MOV	" << temp.res.name << "+2,DX" << endl;
				AX.clear(); DX.clear();
			}
			else if (AX.name == temp.opB.name && AX.active)
			{
				if (DX.active)
					addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	IMUL	" << temp.opA.name << endl;
				addfile << "	MOV	" << temp.res.name << ",AX" << endl;
				addfile << "	MOV	" << temp.res.name << "+2,DX" << endl;
				AX.clear(); DX.clear();
			}
			else if (AX.name == temp.opB.name && !(AX.active))
			{
				if (DX.active)
					addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	IMUL	" << temp.opA.name << endl;
				addfile << "	MOV	" << temp.res.name << ",AX" << endl;
				addfile << "	MOV	" << temp.res.name << "+2,DX" << endl;
				AX.clear(); DX.clear();
			}
		}
		else
		{
			if (DX.active)
				addfile << "	MOV	" << DX.name << ",DX" << endl;
			if (AX.active)
				addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	IMUL	" << temp.opB.name << endl;
			addfile << "	MOV	" << temp.res.name << ",AX" << endl;
			addfile << "	MOV	" << temp.res.name << "+2,DX" << endl;
			AX.clear(); DX.clear();
		}
		/*if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	MOV	" << "AX," << temp.opB.name << endl;
		addfile << "	MUL	" << temp.opB.name << endl;
		AX = temp.res;*/
	}
	else if (temp.Operat == "/") //除法
	{
	  if (lastOper == "para_n")
	  { 
		addfile << "	ASSUME	CS:" << namem << ",DS:" << namem << endl;
		addfile << "                PUSH  " << "AX" << endl;
		addfile << "                PUSH  " << "BX" << endl;
		addfile << "                PUSH  " << "CX" << endl;
		addfile << "                PUSH  " << "DX" << endl;
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",AX" << endl;
			param.pop();
		}
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",BX" << endl;
			param.pop();
		}
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",CX" << endl;
			param.pop();
		}
		if (!(param.empty()))
		{
			addfile << "	MOV	" << param.front().name << ",DX" << endl;
			param.pop();
		}
	  }
	    if(AX.name == temp.opA.name)
		{
			if (AX.active)
				addfile << "	MOV	" << AX.name << ",AX" << endl;
			if (DX.active)
				addfile << "	MOV	" << DX.name << ",DX" << endl;
			addfile << "	CWD	" << endl;
			addfile << "	IDIV	" << temp.opB.name << endl;
			AX = temp.res;
		}
		/*if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	CWD	"  << endl;
		addfile << "	DIV	" << temp.opB.name << endl;
		AX = temp.res;*/
	}
	else if (temp.Operat == "&&") //逻辑与
	{
	    if(AX.name == temp.opA.name || AX.name == temp.opB.name || BX.name == temp.opA.name || BX.name == temp.opB.name || CX.name == temp.opA.name || CX.name == temp.opB.name || DX.name == temp.opA.name || DX.name == temp.opB.name)
		{
			if (AX.name == temp.opA.name && !(AX.active))
			{
				addfile << "	AND	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opA.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	AND	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opB.name && !(AX.active))
			{
				addfile << "	AND	" << "AX," << temp.opA.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opB.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	AND	" << "AX," << temp.opA.name << endl;
				AX = temp.res;
			}
			else if (BX.name == temp.opA.name && !(BX.active))
			{
				addfile << "	AND	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opA.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	AND	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opB.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	AND	" << "BX," << temp.opA.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opB.name && !(BX.active))
			{
				addfile << "	AND	" << "BX," << temp.opA.name << endl;
				BX = temp.res;
			}
			else if (CX.name == temp.opA.name && !(CX.active))
			{
				addfile << "	AND	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opA.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	AND	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opB.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	AND	" << "CX," << temp.opA.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opB.name && !(CX.active))
			{
				addfile << "	AND	" << "CX," << temp.opA.name << endl;
				CX = temp.res;
			}
			else if (DX.name == temp.opA.name && !(DX.active))
			{
				addfile << "	AND	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opA.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	AND	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opB.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	AND	" << "DX," << temp.opA.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opB.name && !(DX.active))
			{
				addfile << "	AND	" << "DX," << temp.opA.name << endl;
				DX = temp.res;
			}
		}
		else if (!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	AND	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	AND	" << "BX," << temp.opB.name << endl;
			BX = temp.res;
		}
		else if (!(CX.active))
		{
		addfile << "	MOV	" << "CX," << temp.opA.name << endl;
		addfile << "	AND	" << "CX," << temp.opB.name << endl;
		CX = temp.res;
		}
		else if (!(DX.active))
		{
		addfile << "	MOV	" << "DX," << temp.opA.name << endl;
		addfile << "	AND	" << "DX," << temp.opB.name << endl;
		DX = temp.res;
		}
		else
		{
		addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		addfile << "	AND	" << "AX," << temp.opB.name << endl;
		AX = temp.res;
		}
		/*addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		addfile << "	AND	" << "AX," << temp.opB.name << endl;*/

	}
	else if (temp.Operat == "||") //逻辑或
	{
	   if(AX.name == temp.opA.name || AX.name == temp.opB.name || BX.name == temp.opA.name || BX.name == temp.opB.name || CX.name == temp.opA.name || CX.name == temp.opB.name || DX.name == temp.opA.name || DX.name == temp.opB.name)
	   {
		   if (AX.name == temp.opA.name && !(AX.active))
		   {
			   addfile << "	OR	" << "AX," << temp.opB.name << endl;
			   AX = temp.res;
		   }
		   else if (AX.name == temp.opA.name && AX.active)
		   {
			   addfile << "	MOV	" << AX.name << ",AX" << endl;
			   addfile << "	OR	" << "AX," << temp.opB.name << endl;
			   AX = temp.res;
		   }
		   else if (AX.name == temp.opB.name && !(AX.active))
		   {
			   addfile << "	OR	" << "AX," << temp.opA.name << endl;
			   AX = temp.res;
		   }
		   else if (AX.name == temp.opB.name && AX.active)
		   {
			   addfile << "	MOV	" << AX.name << ",AX" << endl;
			   addfile << "	OR	" << "AX," << temp.opA.name << endl;
			   AX = temp.res;
		   }
		   else if (BX.name == temp.opA.name && !(BX.active))
		   {
			   addfile << "	OR	" << "BX," << temp.opB.name << endl;
			   BX = temp.res;
		   }
		   else if (BX.name == temp.opA.name && BX.active)
		   {
			   addfile << "	MOV	" << BX.name << ",BX" << endl;
			   addfile << "	OR	" << "BX," << temp.opB.name << endl;
			   BX = temp.res;
		   }
		   else if (BX.name == temp.opB.name && BX.active)
		   {
			   addfile << "	MOV	" << BX.name << ",BX" << endl;
			   addfile << "	OR	" << "BX," << temp.opA.name << endl;
			   BX = temp.res;
		   }
		   else if (BX.name == temp.opB.name && !(BX.active))
		   {
			   addfile << "	OR	" << "BX," << temp.opA.name << endl;
			   BX = temp.res;
		   }
		   else if (CX.name == temp.opA.name && !(CX.active))
		   {
			   addfile << "	OR	" << "CX," << temp.opB.name << endl;
			   CX = temp.res;
		   }
		   else if (CX.name == temp.opA.name && CX.active)
		   {
			   addfile << "	MOV	" << CX.name << ",CX" << endl;
			   addfile << "	OR	" << "CX," << temp.opB.name << endl;
			   CX = temp.res;
		   }
		   else if (CX.name == temp.opB.name && CX.active)
		   {
			   addfile << "	MOV	" << CX.name << ",CX" << endl;
			   addfile << "	OR	" << "CX," << temp.opA.name << endl;
			   CX = temp.res;
		   }
		   else if (CX.name == temp.opB.name && !(CX.active))
		   {
			   addfile << "	OR	" << "CX," << temp.opA.name << endl;
			   CX = temp.res;
		   }
		   else if (DX.name == temp.opA.name && !(DX.active))
		   {
			   addfile << "	OR	" << "DX," << temp.opB.name << endl;
			   DX = temp.res;
		   }
		   else if (DX.name == temp.opA.name && DX.active)
		   {
			   addfile << "	MOV	" << DX.name << ",DX" << endl;
			   addfile << "	OR	" << "DX," << temp.opB.name << endl;
			   DX = temp.res;
		   }
		   else if (DX.name == temp.opB.name && DX.active)
		   {
			   addfile << "	MOV	" << DX.name << ",DX" << endl;
			   addfile << "	OR	" << "DX," << temp.opA.name << endl;
			   DX = temp.res;
		   }
		   else if (DX.name == temp.opB.name && !(DX.active))
		   {
			   addfile << "	OR	" << "DX," << temp.opA.name << endl;
			   DX = temp.res;
		   }
	   }
	   else if (!(AX.active))
	   {
		   addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		   addfile << "	OR	" << "AX," << temp.opB.name << endl;
		   AX = temp.res;
	   }
	   else if (!(BX.active))
	   {
		   addfile << "	MOV	" << "BX," << temp.opA.name << endl;
		   addfile << "	OR	" << "BX," << temp.opB.name << endl;
		   BX = temp.res;
	   }
	   else if (!(CX.active))
		{
		   addfile << "	MOV	" << "CX," << temp.opA.name << endl;
		   addfile << "	OR	" << "CX," << temp.opB.name << endl;
		   CX = temp.res;
		}
	   else if (!(DX.active))
		{
		   addfile << "	MOV	" << "DX," << temp.opA.name << endl;
		   addfile << "	OR	" << "DX," << temp.opB.name << endl;
		   DX = temp.res;
		}
	   else
	   {
	      addfile << "	MOV	" << AX.name << ",AX" << endl;
	      addfile << "	MOV	" << "AX," << temp.opA.name << endl;
	      addfile << "	OR	" << "AX," << temp.opB.name << endl;
	      AX = temp.res;
	   }
    }
	else if (temp.Operat == "!=" || temp.Operat == "==" || temp.Operat == "<" || temp.Operat == "<=" || temp.Operat == ">" || temp.Operat == ">=") //不等于，判断相等，大于等于，大于，小于等于，小于
	{
	   if(AX.name == temp.opA.name )
	   {
		   addfile << "	CMP	" << "AX," << temp.opB.name << endl;
	   }
	   else if (BX.name == temp.opA.name)
	   {
		   addfile << "	CMP	" << "BX," << temp.opB.name << endl;
	   }
	   else if (CX.name == temp.opA.name)
	   {
		   addfile << "	CMP	" << "CX," << temp.opB.name << endl;
	   }
	   else if (DX.name == temp.opA.name)
	   {
		   addfile << "	CMP	" << "DX," << temp.opB.name << endl;
	   }
	   else if (!(AX.active))
	   {
		   addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		   addfile << "	CMP	" << "AX," << temp.opB.name << endl;
		   AX = temp.opA;
	   }
	   else if (!(BX.active))
	   {
		   addfile << "	MOV	" << "BX," << temp.opA.name << endl;
		   addfile << "	CMP	" << "BX," << temp.opB.name << endl;
		   BX = temp.opA;
	   }
	   else if (!(CX.active))
	   {
		   addfile << "	MOV	" << "CX," << temp.opA.name << endl;
		   addfile << "	CMP	" << "CX," << temp.opB.name << endl;
		   CX = temp.opA;
	   }
	   else if (!(DX.active))
	   {
		   addfile << "	MOV	" << "DX," << temp.opA.name << endl;
		   addfile << "	CMP	" << "DX," << temp.opB.name << endl;
		   DX = temp.opA;
	   }
	   else
	   {
		   addfile << "	MOV	" << AX.name << ",AX" << endl;
		   addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		   addfile << "	CMP	" << "AX," << temp.opB.name << endl;
		   AX = temp.opA;
	   }
		/*if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	CMP	" << "AX," << temp.opB.name << endl;*/
	}
	else if (temp.Operat == "!") //非
	{
	    if (AX.name == temp.opA.name || BX.name == temp.opA.name || CX.name == temp.opA.name || DX.name == temp.opA.name )
	    {
			if (AX.name == temp.opA.name && !(AX.active))
			{
				addfile << "	NOT	" << "AX"  << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opA.name && AX.active)
			{
				addfile << "	MOV	"  << AX.name << ",AX" << endl;
				addfile << "	NOT	" << "AX" << endl;
				AX = temp.res;
			}
			else if (BX.name == temp.opA.name && !(BX.active))
			{
				addfile << "	NOT	" << "BX" << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opA.name && BX.active)
			{
				addfile << "	MOV	"  << BX.name << ",BX" << endl;
				addfile << "	NOT	" << "BX" << endl;
				BX = temp.res;
			}
			else if (CX.name == temp.opA.name && !(CX.active))
			{
				addfile << "	NOT	" << "CX" << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opA.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	NOT	" << "CX" << endl;
				CX = temp.res;
			}
			else if (DX.name == temp.opA.name && !(DX.active))
			{
				addfile << "	NOT	" << "DX" << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opA.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	NOT	" << "DX" << endl;
				DX = temp.res;
			}
	    }
		else if(!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	NOT	" << "AX" << endl;
			AX = temp.res;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	NOT	" << "BX" << endl;
			BX = temp.res;
		}
		else if (!(CX.active))
		{
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			addfile << "	NOT	" << "CX" << endl;
			CX = temp.res;
		}
		else if (!(DX.active))
		{
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			addfile << "	NOT	" << "DX" << endl;
			DX = temp.res;
		}
		else
		{
			addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	NOT	" << "AX" << endl;
			AX = temp.res;
		}
    }
	else if (temp.Operat == "-" && (temp.opB.name == ""))
	{
	  if (AX.name == temp.opA.name || BX.name == temp.opA.name || CX.name == temp.opA.name || DX.name == temp.opA.name)
	  {
		if (AX.name == temp.opA.name && !(AX.active))
		{
			addfile << "	NEG	" << "AX" << endl;
			AX = temp.res;
		}
		else if (AX.name == temp.opA.name && AX.active)
		{
			addfile << "	MOV	" << AX.name <<",AX" << endl;
			addfile << "	NEG	" << "AX" << endl;
			AX = temp.res;
		}
		else if (BX.name == temp.opA.name && !(BX.active))
		{
			addfile << "	NEG	" << "BX" << endl;
			BX = temp.res;
		}
		else if (BX.name == temp.opA.name && BX.active)
		{
			addfile << "	MOV	" << BX.name << ",BX" << endl;
			addfile << "	NEG	" << "BX" << endl;
			BX = temp.res;
		}
		else if (CX.name == temp.opA.name && !(CX.active))
		{
			addfile << "	NEG	" << "CX" << endl;
			CX = temp.res;
		}
		else if (CX.name == temp.opA.name && CX.active)
		{
			addfile << "	MOV	" << CX.name << ",CX" << endl;
			addfile << "	NEG	" << "CX" << endl;
			CX = temp.res;
		}
		else if (DX.name == temp.opA.name && !(DX.active))
		{
			addfile << "	NEG	" << "DX" << endl;
			DX = temp.res;
		}
		else if (DX.name == temp.opA.name && DX.active)
		{
			addfile << "	MOV	" << DX.name << ",DX" << endl;
			addfile << "	NEG	" << "DX" << endl;
			DX = temp.res;
		}
	  }
	  else if (!(AX.active))
	  {
		  addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		  addfile << "	NEG	" << "AX" << endl;
		  AX = temp.res;
	  }
	  else if (!(BX.active))
	  {
		  addfile << "	MOV	" << "BX," << temp.opA.name << endl;
		  addfile << "	NEG	" << "BX" << endl;
		  BX = temp.res;
	  }
	  else if (!(CX.active))
	  {
		  addfile << "	MOV	" << "CX," << temp.opA.name << endl;
		  addfile << "	NEG	" << "CX" << endl;
		  CX = temp.res;
	  }
	  else if (!(DX.active))
	  {
		  addfile << "	MOV	" << "DX," << temp.opA.name << endl;
		  addfile << "	NEG	" << "DX" << endl;
		  DX = temp.res;
	  }
	  else
	  {
		  addfile << "	MOV	" << AX.name << ",AX" << endl;
		  addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		  addfile << "	NEG	" << "AX" << endl;
		  AX = temp.res;
	  }
    }
	else if (temp.Operat == "if") //if语句
	{
	   if (lastOper == "<")
		addfile << "	JGE	" << "QT" << temp.res.name << endl;
	   else if (lastOper == "<=")
		addfile << "	JG	" << "QT" << temp.res.name << endl;
	   else if (lastOper == ">")
		addfile << "	JLE	" << "QT" << temp.res.name << endl;
	   else if (lastOper == ">=")
		addfile << "	JL	" << "QT" << temp.res.name << endl;
	}
	else if (temp.Operat == "el") //else语句
	{
	    addfile << "	JMP	" << "QT" << temp.res.name << endl;
	    addfile << "QT" << i + 1 << ":";
	}
	else if (temp.Operat == "ie") //ie结构尾部
	{
	    addfile << "QT" << i << ":";
	}
	else if (temp.Operat == "wh") //while语句
	{
	addfile << "QT" << i << ":";
	}
	else if (temp.Operat == "do") //do语句，循环部分
	{
	if (lastOper == "<")
		addfile << "	JGE	" << "QT" << temp.res.name << endl;
	else if (lastOper == "<=")
		addfile << "	JG	" << "QT" << temp.res.name << endl;
	else if (lastOper == ">")
		addfile << "	JLE	" << "QT" << temp.res.name << endl;
	else if (lastOper == ">=")
		addfile << "	JL	" << "QT" << temp.res.name << endl;
	}
	else if (temp.Operat == "we") //while结构尾部
	{
	   addfile << "	JMP	" << "QT" << temp.res.name << endl;
	   addfile << "QT" << i << ":";
	}
	else if (temp.Operat == "FUNC")//函数，最后一个操作数为函数名
	{
	   addfile << temp.opA.name<<"      PROC" << endl;
	   namem = temp.opA.name;
    }
	else if (temp.Operat == "para_n")//参数
	{
	   param.push(temp.opA);
	   addfile << "	" << temp.opA.name << "	DB	4 DUP(0)" << endl;
	}
	else if (temp.Operat == "RET")//参数
	{
	   addfile << "            POP  " << "DX" << endl;
	   addfile << "            POP  " << "CX" << endl;
	   addfile << "            POP  " << "BX" << endl;
	   addfile << "    RET  " << endl;
	   addfile << namem << "    ENDP  " << endl;
	   RETm.push_back(temp.opA);
	}
	else if (temp.Operat == "para")//参数
	{
	   /*if (!(param.empty()))
	   {
            if(AX.active)
				addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	MOV	" << param.front().name <<  ",AX" << endl;
			param.pop();
	   }*/
	   if (ppp == 1)
	   {
		   if (AX.active)
			   addfile << "	MOV	" << AX.name << ",AX" << endl;
		       addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		   ppp++;
	   }
	   else if (ppp == 2)
	   {
		   if (BX.active)
			   addfile << "	MOV	" << BX.name << ",BX" << endl;
		       addfile << "	MOV	" << "BX," << temp.opA.name << endl;
		   ppp++;
	   }
	   else if (ppp == 3)
	   {
		   if (BX.active)
			   addfile << "	MOV	" << CX.name << ",CX" << endl;
		       addfile << "	MOV	" << "CX," << temp.opA.name << endl;
		   ppp++;
	   }
	   else if (ppp == 4)
	   {
		   if (BX.active)
			   addfile << "	MOV	" << DX.name << ",DX" << endl;
		   addfile << "	MOV	" << "DX," << temp.opA.name << endl;
		   ppp++;
	   }
	}
	else if (temp.Operat == "call")//函数调用
	{
	     ppp = 0;
			addfile << "    CALL  " << temp.opA.name  << endl;
		/*int bbb = 1;
	    vector<token> ::iterator aaa;
		for (aaa = param.begin(); aaa != param.end(); aaa++,bbb++)
		{
			if (temp.opA.name != ""&&bbb==1)
			{
				addfile << "    MOV  " <<  "AX," << temp.opA.name << endl;
				addfile << "    MOV  " << aaa->name << ",AX"<< endl;
			}
			if (temp.opB.name != "" && bbb == 2)
			{
				addfile << "    MOV  " << "AX," << temp.opB.name << endl;
				addfile << "    MOV  " << aaa->name << ",AX" << endl;
			}
			if (temp.res.name != "" && bbb == 3)
			{
				addfile << "    MOV  " << "AX," << temp.res.name << endl;
				addfile << "    MOV  " << aaa->name << ",AX" << endl;
			}
		}*/
	}
	/*else if (temp.Operat == "RET")//函数返回
	{
	    if(AX.active)
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		if (BX.active)
			addfile << "	MOV	" << BX.name << ",BX" << endl;
		if (CX.active)
			addfile << "	MOV	" << CX.name << ",CX" << endl;
		if (DX.active)
			addfile << "	MOV	" << DX.name << ",DX" << endl;
	    addfile << "    POP  " << "DX" << endl;
	    addfile << "    POP  " << "CX" << endl;
	    addfile << "    POP  " << "BX" << endl;
		addfile << "    RET  "<< endl;
    }*/
	/*else if (temp.Operat == "END")//函数结束
	{
	    addfile << temp.opA.name <<"    ENDP  " << endl;
    }*/
}

void createDSEG() //建立DSEG段
{
	//查符号表，给变量生成空间
	//查变量时由于汇编不区分大小写，所以应该在这里
	ofstream file;
	file.open("D:\\byresult\\code.txt");
	file << "SSEG	SEGMENT STACK" << endl;
	file << "SKT	DB	200 DUP(0)" << endl;
	file << "SSEG	ENDS" << endl;
	file << "DSEG	SEGMENT " << endl;
	vector<token>::iterator tokent; //原本是遍历符号表，这里先遍历我本身的SYMBL
	for (tokent = SYMBL_ALL.begin(); tokent != SYMBL_ALL.end(); tokent++)
	{
		file << "	" << tokent->name << "	DB	2 DUP(0)" << endl;
	}
	file << "DSEG	ENDS " << endl;
}

void createCSEGBEGIN() //建立CSEG段
{
	ofstream file;
	file.open("D:\\byresult\\code.txt", ios::app);
	file << "CSEG	SEGMENT " << "\n"
		<< "	ASSUME	CS:CSEG,DS:DSEG,SS:SSEG" << endl;
	file << "START:	MOV	AX,DSEG" << "\n"
		<< "	MOV	DS,AX" << endl;
	file << "	MOV	AX,SSEG" << "\n"
		<< "	MOV	SS,AX" << "\n"
		<< "	MOV	SP,SIZE SKT" << endl;
	file.close();
}

void createCSEGEND() //建立CSEG段
{
	ofstream file;
	file.open("D:\\byresult\\code.txt", ios::app);
	file << "	MOV	AX,4C00H" << "\n"
		<< "	INT	21H" << "\n"
		<< "CSEG	ENDS " << "\n"
		<< "	END	START" << endl;
	file.close();
}

/*int main()
{
	initQT(groupQT);
	createDAG(groupQT);
	createQT();
	printQT(groupQT);
	return 0;
}*/
void firstview()
{
	vector<QT>::iterator it;//指向qt内容的迭代器
	int i = 1;
	//block_qt.clear();
	for (it = ceshi.begin(); it != ceshi.end(); it++) //遍历qt
	{
		if (i != it->block) //it为下一个基本块内的四元式
		{
			createDAG(block_qt);//根据四元式创建DAG图
			createQT();//根据DAG图化简四元式
			createSYMBL();//构造活跃信息表
			changeActive();//逆序填写活跃信息
			//addSYMBL(); //填入一个全局的符号表为DSEG段做准备
			createRes(); //生成优化后的四元式组resQT
			i++;
			block_qt.clear();//处理完一个基本块清空
		}
		block_qt.push_back(*it); //取四元式
	}
}
void secondview()
{
	vector<QT>::iterator it;
	int i = 1; //四元式的序号
	stack<token*>ifstack; //if语义栈
	for (it = resQT.begin(); it != resQT.end(); i++, it++)
	{
		if (it->Operat == "if")
		{
			ifstack.push(&it->res);
		}
		else if (it->Operat == "el")
		{
			ifstack.top()->name = to_string(i + 1);
			ifstack.pop();
			ifstack.push(&it->res);
		}
		else if (it->Operat == "ie")
		{
			ifstack.top()->name = to_string(i);
			ifstack.pop();
		}
		else if (it->Operat == "wh")
		{
			whstacknum.push(i);
		}
		else if (it->Operat == "do")
		{
			whstack.push(&it->res);
		}
		else if (it->Operat == "we")
		{
			it->res.name = to_string(whstacknum.top());
			whstacknum.pop();
			whstack.top()->name = to_string(i);
			whstack.pop();
		}
	}
}

int main()
{
	initQT(ceshi);//初始化
	divide();//基本块划分
	printQT3(ceshi);//打印出优化前的四元式
	firstview();//四元式优化以及活跃信息的填写
	secondview();//标注跳转循环语句的位置
	printQT(resQT);//打印优化后的四元式
	printQT1(resQT);//打印四元式填写活跃信息后的情况
	//createDSEG(); //建立DSEG段
	//createCSEGBEGIN();//建立CSEG段的开头
	createCode(resQT);//建立CSEG段的语句执行部分
	//createCSEGEND();//建立CSEG段的结尾
	return 0;
}