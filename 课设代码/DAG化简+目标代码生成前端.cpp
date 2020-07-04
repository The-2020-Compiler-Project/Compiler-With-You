#include <iostream>
#include<vector>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

struct token {
	string name;
	int type;// 1������2����ʱ������3����ʱ����
	bool active;//��Ծ��Ϣ
	token() { name = ""; type = 0; active = false; }//�����ʼ���Ĺ��캯��
	token(string namet, int typet, bool activet) { name = namet; type = typet; active = activet; }//�����ʼ���Ĺ��캯��
	void clear() { name = ""; type = 0; active = false; }//������պ���

};

struct QT
{
	string Operat;//������
	token opA;//Ŀ�������
	token opB;//Դ������
	token res;//����ݴ浥Ԫ
	int block;//������
	QT() { Operat = ""; opA.clear(); opB.clear(); res.clear(); }//������յĹ��캯��
	QT(string Operatt, token opAt, token opBt, token rest) { Operat = Operatt; opA = opAt; opB = opBt; res = rest; }//������յĹ��캯��
	void clear() { Operat = ""; opA.clear(); opB.clear(); res.clear(); }//������յĺ���
};

struct node
{
	int num; //����
	int left; //����
	int right; //�ҽ��
	string Operat; //������
	token Mark[20]; //��ǣ�0��Ϊ�����
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

//vector<QT>qt; //ԭ����һ���������ڵ���Ԫʽ��
vector<QT>groupQT; //����Ż����һ���������ڵ���Ԫʽ
vector<QT>resQT; //��������Ż����������Ԫʽ
vector<node>DAG;
vector<token>SYMBL;
vector<token>SYMBL_ALL;
QT specialQT;
token AX; //���Ĵ���
token BX; //���Ĵ���
token CX; //���Ĵ���
token DX; //���Ĵ���

int searchNode(string idtemp); //���ݱ�ʶ��Ѱ�ҵ������
int searchNode(string tempOper, string tempB, string tempC); //���ݲ������Ͳ�����B��C���ҽ���ţ�˫Ŀ�������
int searchNode(string tempOper, string tempB); //���ݲ������Ͳ�����B���ҽ���ţ���Ŀ�������
int searchMark(string marktemp, int nodeNum);//��ȷ��NODE�ڱ�������ҳ��Ƿ���ڴ˱��
bool searchActive(string temp); //��SYMBL���и��ݱ�ʶ�����ػ�Ծ��Ϣ
void deleteMark(string nameM, int n); //ɾ�����˵�N�Ž������ĸ����
int judgeOperate(string t); //���ز�����������
string calculate(string tempOper, string tempB, string tempC); //���������������ֵ
void attachNode(int number, token tempA); //�ѱ��A���ŵ������
void swapMark(node& temp); //��ǻ���
QT createQT(string tempOper, token opA, token opB, token res); //����˫Ŀ�������Ԫʽ
QT createQT(string tempOper, token opA, token res); //������Ŀ�������Ԫʽ
int createNode(); //�����½�㣬�������һ�����ı��
void createDAG(vector<QT>block_qt); //���ݸ�������Ԫʽ��block_qt�����Ż���DAG��ԭ����block_qt���ı䣬������һ��DAG
void createQT(); //�����Ż���DAG��������ԪʽC
void createSYMBL(); //�����Ż����QT�飬���ɻ�Ծ��Ϣ��
void createRes();//����groupQT������Ԫʽ�����resQT
int searchSYMBL(string temp); //��SYMBL���������ñ�ʶ���Ƿ����
void changeActive(); //�������ɻ�Ծ��Ϣ
void addSYMBL(); //����һ��ȫ�ֵķ��ű�ΪDSEG����׼��
void printQT(vector<QT>tempQT); //��ӡ��Ԫʽ��
void initQT(vector<QT>& tempQT); //�����ù����ʼ��Ԫʽ��
void code(QT temp, string lastOper, int i);

int searchNode(string idtemp) //���ݱ�ʶ��Ѱ�ҵ������
{
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++)
	{
		if (searchMark(idtemp, it->num) != -1) //�ҵ���
		{
			return it->num;
		}
	}
	return -1;//û�ҵ�
}

int searchNode(string tempOper, string tempB, string tempC) //���ݲ������Ͳ�����B��C���ҽ���ţ�˫Ŀ�������
{
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++)
	{
		if (it->Operat == tempOper)
		{
			int i = it->left;
			int j = it->right;
			if (searchMark(tempB, i) != -1 && searchMark(tempC, j) != -1) //�������Ͳ���������Ӧ��ȷ
			{
				return it->num;
			}
		}
	}
	return -1; //û�ҵ�
}

int searchNode(string tempOper, string tempB) //���ݲ������Ͳ�����B���ҽ���ţ���Ŀ�������
{
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++)
	{
		if (it->Operat == tempOper && it->right == -1) //��������ͬ��Ϊ��Ŀ������
		{
			int i = it->left;
			if (searchMark(tempB, i) != -1) //�������Ͳ���������Ӧ��ȷ
			{
				return it->num;
			}
		}
	}
	return -1;
}

int searchMark(string marktemp, int nodeNum) //��ȷ��NODE�ڱ�������ҳ��Ƿ���ڴ˱��
{
	if (marktemp != "")
	{
		for (int i = 0; i < 20; i++)
		{
			if (DAG[nodeNum].Mark[i].name == marktemp)
				return i;
		}
	}
	return -1;//û�ҵ�
}

bool searchActive(string temp) //��SYMBL���и��ݱ�ʶ�����ػ�Ծ��Ϣ
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

void deleteMark(string nameM, int n) //ɾ�����˵�N�Ž������ĸ����
{
	int j;
	vector<node>::iterator delMark;
	for (delMark = DAG.begin(); delMark != DAG.end(); delMark++)
	{
		if (delMark->num != n) //����N�Ž��
		{
			j = searchMark(nameM, delMark->num);
			if (j != -1 && j != 0) //�������ɾ
			{
				DAG[delMark->num].Mark[j].name = "";
				DAG[delMark->num].Mark[j].type = 0;
			}
		}
	}
}

int judgeOperate(string t) //���ز�����������
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
	else return 3; //������Ԫʽ
}

string calculate(string tempOper, string tempB, string tempC)   //���������������ֵ
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

void attachNode(int number, token tempA) //�ѱ��A���ŵ������
{
	int i; //�洢���ŵ���MARK�����
	for (i = 0; i < 20; i++) //��A���ӵ�B��
	{
		if (DAG[number].Mark[i].name == "")
		{
			DAG[number].Mark[i].name = tempA.name;
			DAG[number].Mark[i].type = tempA.type;
			break;
		}
	}
}

void swapMark(node& temp) //��ǻ���
{
	token swap;
	for (int i = 1; i < 20; i++)
	{
		if (temp.Mark[i].type != 0 && temp.Mark[0].type > temp.Mark[i].type) 	//����
		{
			swap = temp.Mark[0];
			temp.Mark[0] = temp.Mark[i];
			temp.Mark[i] = swap;
		}
	}
}

QT createQT(string tempOper, token opA, token opB, token res) //����˫Ŀ�������Ԫʽ
{
	QT temp;
	temp.Operat = tempOper;
	temp.opA.active = opA.active; temp.opA.name = opA.name; temp.opA.type = opA.type;
	temp.opB.active = opB.active; temp.opB.name = opB.name; temp.opB.type = opB.type;
	temp.res.active = res.active; temp.res.name = res.name; temp.res.type = res.type;
	return temp;
}

QT createQT(string tempOper, token opA, token res) //������Ŀ�������Ԫʽ
{
	QT temp;
	temp.Operat = tempOper;
	temp.opB.clear();
	temp.opA.active = opA.active; temp.opA.name = opA.name; temp.opA.type = opA.type;
	temp.res.active = res.active; temp.res.name = res.name; temp.res.type = res.type;
	return temp;
}

int createNode()//�����½�㣬�������һ�����ı��
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

void createDAG(vector<QT>block_qt) //���ݸ�������Ԫʽ��block_qt�����Ż���DAG��ԭ����block_qt���ı䣬������һ��DAG
{
	DAG.clear();
	vector<QT>::iterator QTit;
	QT QTtemp;
	specialQT.clear();
	for (QTit = block_qt.begin(); QTit != block_qt.end(); QTit++)//����һ����������
	{
		QTtemp = *QTit;
		int numB, numC;
		numB = searchNode(QTtemp.opA.name);
		numC = searchNode(QTtemp.opB.name);
		if (numB == -1) //û��B�������򴴽�
		{
			numB = createNode();
			DAG[numB].Mark[0].name = QTtemp.opA.name;
			DAG[numB].Mark[0].type = QTtemp.opA.type;
		}
		if (numC == -1) //û��C�������򴴽�
		{
			numC = createNode();
			DAG[numC].Mark[0].name = QTtemp.opB.name;
			DAG[numC].Mark[0].type = QTtemp.opB.type;
		}
		int judgetemp = judgeOperate(QTtemp.Operat);
		if (judgetemp == 0 && QTtemp.opB.name == "") //����Ǹ�ֵ��ԪʽA=B��B�����ǳ�ֵ
		{
			if (QTtemp.opA.type == 1) //����ǳ�ֵ���ʽA=C
			{
				int i;
				i = stringToNum<int>(QTtemp.opA.name);
				QTtemp.opA.name = to_string(i);
				string tempC = QTtemp.opA.name;
				int k = searchNode(tempC); //Ѱ��C�Ƿ��Ѿ������,���ڴ��C���ڵĽ����
				int l = searchNode(QTtemp.res.name); //Ѱ��A�Ƿ��Ѿ������
				if (k != -1) //C�Ѿ������
				{
					attachNode(k, QTtemp.res); //��A������C��
				}
				else //Cû�����
				{
					int i = createNode();
					DAG[i].Mark[0].name = tempC;
					DAG[i].Mark[0].type = 1;
					DAG[i].Mark[1].name = QTtemp.res.name;
					DAG[i].Mark[1].type = QTtemp.res.type;
					DAG[i].num = i; //�����½�㲢��ʼ��
					k = i;
				}
				if (l != -1)//A��N2�Ѿ������
				{
					deleteMark(QTtemp.res.name, k);
				}
			}
			else //�Ǹ�ֵ��ԪʽA=B
			{
				int l = searchNode(QTtemp.res.name); //Ѱ��A�Ƿ��Ѿ������
				attachNode(numB, QTtemp.res); //��A������B��
				if (l != -1)//A��N2�Ѿ������
				{
					deleteMark(QTtemp.res.name, numB);
				}
			}
		}
		else if (judgetemp != 3 && judgetemp != 0) //��1��2���͵ı��ʽ
		{
			if (QTtemp.opA.type == 1 && QTtemp.opB.type == 1)//�ǳ�ֵ���ʽA=C1��C2
			{
				int i;
				i = stringToNum<int>(QTtemp.opA.name);
				QTtemp.opA.name = to_string(i);
				i = stringToNum<int>(QTtemp.opB.name);
				QTtemp.opB.name = to_string(i);
				string tempC = calculate(QTtemp.Operat, QTtemp.opA.name, QTtemp.opB.name); //���㳣ֵC=C1��C2
				int k = searchNode(tempC); //Ѱ��C�Ƿ��Ѿ������
				int l = searchNode(QTtemp.res.name); //Ѱ��A�Ƿ��Ѿ������
				if (k != -1) //C�����
				{
					attachNode(k, QTtemp.res);
				}
				else //Cδ������������½��,����A������C��
				{
					int i = createNode();
					DAG[i].Mark[0].name = tempC;
					DAG[i].Mark[0].type = 1;
					DAG[i].Operat = "";
					DAG[i].Mark[1].name = QTtemp.res.name;
					DAG[i].Mark[1].type = QTtemp.res.type;
					DAG[i].num = i; //�����½�㲢��ʼ��
					k = i;
				}
				if (l != -1)
				{
					deleteMark(QTtemp.res.name, k);
				}
			}
			else if (QTtemp.opB.name == "") //A=��B
			{
				int i = searchNode(QTtemp.Operat, QTtemp.opA.name);
				if (i != -1) //�Ѿ����ڹ������ʽ��B
				{
					attachNode(i, QTtemp.res);
				}
				else //�����ڹ������ʽ
				{
					int j = createNode();
					DAG[j].left = numB;
					DAG[j].Mark[0].name = QTtemp.res.name;
					DAG[j].Mark[0].type = QTtemp.res.type;
					DAG[j].Operat = QTtemp.Operat;
					DAG[j].num = j; //�����½�㲢��ʼ��
					i = j;
				}
				deleteMark(QTtemp.res.name, i);
			}
			else //A=B��C
			{
				int i = searchNode(QTtemp.Operat, QTtemp.opA.name, QTtemp.opB.name);
				if (i != -1) //�Ѿ����ڹ������ʽB��C
				{
					attachNode(i, QTtemp.res);
				}
				else //�����ڹ������ʽ
				{
					int j = createNode();
					DAG[j].left = numB;
					DAG[j].right = numC;
					DAG[j].Mark[0].name = QTtemp.res.name;
					DAG[j].Mark[0].type = QTtemp.res.type;
					DAG[j].Operat = QTtemp.Operat;
					DAG[j].num = j; //�����½�㲢��ʼ��
					i = j;
				}
				deleteMark(QTtemp.res.name, i);
			}
		}
		else if (judgetemp == 3) //������Ԫʽ
		{
			specialQT = QTtemp;
		}
	}
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++) //����DAG��㽻�����
	{
		swapMark(*it);
	}
}

void createQT() //�����Ż���DAG��������ԪʽC
{
	//��Ҫ���ӹ��ܣ���������A��a�����ı���Ҫ�ı����ʶ������������Ŀ�����ʱ���ֱ���
	groupQT.clear();
	vector<node>::iterator itdag;
	QT temp;
	for (itdag = DAG.begin(); itdag != DAG.end(); itdag++)
	{
		if (itdag->Operat == "") //��Ҷ���
		{
			for (int i = 1; i < 20; i++) //AiΪ����ʱ����������Ai=B
			{
				if (itdag->Mark[i].type == 2) //Ϊ����ʱ����
				{
					temp.Operat = "=";
					temp.opA = itdag->Mark[0];
					temp.res = itdag->Mark[i];
					groupQT.push_back(temp);
					temp.clear();
				}
			}
		}
		else //��Ҷ���
		{
			if (DAG[itdag->right].Mark[0].name == "")//��Ŀ�����
			{
				temp.Operat = itdag->Operat;
				temp.opA = DAG[itdag->left].Mark[0]; //������ǲ�������
				temp.res = itdag->Mark[0];
				groupQT.push_back(temp);
				temp.clear();
				for (int i = 1; i < 20; i++) //AiΪ����ʱ����������Ai=A
				{
					if (itdag->Mark[i].type == 2) //Ϊ����ʱ����
					{
						temp.Operat = "=";
						temp.opA = itdag->Mark[0];
						temp.res = itdag->Mark[i];
						groupQT.push_back(temp);
						temp.clear();
					}
				}
			}
			else //˫Ŀ�����
			{
				temp.Operat = itdag->Operat;
				temp.opA = DAG[itdag->left].Mark[0]; //������ǲ�������
				temp.opB = DAG[itdag->right].Mark[0];
				temp.res = itdag->Mark[0];
				groupQT.push_back(temp);
				temp.clear();
				for (int i = 1; i < 20; i++) //AiΪ����ʱ����������Ai=A
				{
					if (itdag->Mark[i].type == 2) //Ϊ����ʱ����
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

void printQT(vector<QT>tempQT) //��ӡ��Ԫʽ��
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
				cout << "( " << " = " << " , " << it->opA.name << " , " << " _ " << " , " << it->res.name << " )" << endl;

			}
			else
			{
				cout << "( " << it->Operat << " , " << it->opA.name << " , " << " _ " << " , " << it->res.name << " )" << endl;
			}
		}
		else
		{
			cout << "( " << it->Operat << " , " << it->opA.name << " , " << it->opB.name << " , " << it->res.name << " )" << endl;
		}
	}
}

void printQT1(vector<QT>tempQT) //��ӡ��Ԫʽ��
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
				cout << "( " << " = " << " , " << it->opA.name<<'('<< it->opA.active<<')'<< " , " << " _ " << " , " << it->res.name << '(' << it->res.active << ')'  <<')'<< endl;

			}
			else
			{
				cout << "( " << it->Operat << " , " << it->opA.name << '(' << it->opA.active << ')' << " , " << " _ " << " , " << it->res.name << '(' << it->res.active << ')'<<')' << endl;
			}
		}
		else
		{
			cout << "( " << it->Operat << " , " << it->opA.name << '(' << it->opA.active << ')' << " , " << it->opB.name << '(' << it->opB.active << ')' << " , " << it->res.name << '(' << it->res.active << ')' << " )" << endl;
		}
	}
}

void initQT(vector<QT>& tempQT) //�����ù����ʼ��Ԫʽ��
{
	tempQT.clear();
	tempQT.push_back({ "+",{"x",2,false},{"y",2,false},{"t1",3,false} }); //x+y=t1
	tempQT.push_back({ "=",{"t1",3,false},{"",0,false},{"z",2,false} }); //z=t1
	tempQT.push_back({ "*",{"t1",3,false},{"z",2,false},{"t2",3,false} }); //z*z=t2
	tempQT.push_back({ "=",{"t2",3,false},{"",0,false},{"t",2,false} }); //t=t2
	tempQT.push_back({ "-",{"m",2,false},{"n",2,false},{"t3",3,false} }); //m-n=t3
	tempQT.push_back({ "=",{"t3",3,false},{"",0,false},{"f",2,false} }); //f=t3
	tempQT.push_back({ "*",{"f",2,false},{"z",2,false},{"t4",3,false} }); //f*z=t4
	tempQT.push_back({ "=",{"t4",3,false},{"",0,false},{"p",2,false} }); //p=t4
	tempQT.push_back({ "+",{"w",2,false},{"v",2,false},{"t5",3,false} }); //w+v=t3
	tempQT.push_back({ "=",{"t5",3,false},{"",0,false},{"h",2,false} }); //h=t5
	tempQT.push_back({ "/",{"s",2,false},{"d",2,false},{"t6",3,false} }); //s/d=t6
	tempQT.push_back({ "=",{"t6",3,false},{"",0,false},{"l",2,false} }); //l=t6
	tempQT.push_back({ "=",{"1",1,false},{"",0,false},{"A",2,false} }); //A=1
	tempQT.push_back({ "=",{"2",1,false},{"",0,false},{"B",2,false} }); //B=2
	tempQT.push_back({ "=",{"5",1,false},{"",0,false},{"B",2,false} }); //B=5
	tempQT.push_back({ "*",{"2",1,false},{"3.14",1,false},{"t1",3,false} }); //t1=2*3
	tempQT.push_back({ "+",{"R",2,false},{"r",2,false},{"t2",3,false} }); //t2=R+r
	tempQT.push_back({ "*",{"t1",3,false},{"t2",3,false},{"t3",3,false} }); //t3=t1*t2
	tempQT.push_back({ "=",{"t3",3,false},{"",0,false},{"A",2,false} }); //A=t3
	tempQT.push_back({ "*",{"2",1,false},{"3.14",1,false},{"t4",3,false} }); //t4=2*3.14
	tempQT.push_back({ "+",{"R",2,false},{"r",2,false},{"t5",3,false} }); //t5=R+r
	tempQT.push_back({ "*",{"t4",3,false},{"t5",3,false},{"t6",3,false} }); //t6=t4*t5
	tempQT.push_back({ "-",{"R",2,false},{"r",2,false},{"t7",3,false} }); //t7=R-r
	tempQT.push_back({ "/",{"t6",3,false},{"t7",3,false},{"t8",3,false} }); //t8=t6/t7
	tempQT.push_back({ "=",{"t8",3,false},{"",0,false},{"B",2,false} }); //B=t8
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

void createSYMBL() //�����Ż����QT�飬���ɻ�Ծ��Ϣ��
{
	vector<QT>::iterator it;
	SYMBL.clear();
	for (it = groupQT.begin(); it != groupQT.end(); it++)
	{
		int i = -1;
		if ((i = searchSYMBL(it->opA.name)) == -1) //δ����A����������Ѿ������򲻸Ķ�
		{
			if (it->opA.type == 2) //Ϊ����ʱ����
			{
				SYMBL.push_back({ it->opA.name,it->opA.type,true });
				it->opA.active = true;
			}
			else if (it->opA.type == 3) //Ϊ��ʱ����
				SYMBL.push_back(it->opA);
		}
		if ((i = searchSYMBL(it->opB.name)) == -1) //δ����B����������Ѿ������򲻸Ķ�
		{
			if (it->opB.type == 2) //Ϊ����ʱ����
			{
				SYMBL.push_back({ it->opB.name,it->opB.type,true });
				it->opB.active = true;
			}
			else if (it->opB.type == 3) //Ϊ��ʱ����
				SYMBL.push_back(it->opB);
		}
		if ((i = searchSYMBL(it->res.name)) == -1) //δ����B����������Ѿ������򲻸Ķ�
		{
			if (it->res.type == 2) //Ϊ����ʱ����
			{
				SYMBL.push_back({ it->res.name,it->res.type,true });
				it->res.active = true;
			}
			else if (it->res.type == 3) //Ϊ��ʱ����
				SYMBL.push_back(it->res);
		}
	}
}

void createRes() //����groupQT������Ԫʽ�����resQT
{
	vector<QT>::iterator it;
	for (it = groupQT.begin(); it != groupQT.end(); it++)
	{
		resQT.push_back(*it);
	}
}

int searchSYMBL(string temp) //��SYMBL���������ñ�ʶ���Ƿ����
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

void changeActive() //�������ɻ�Ծ��Ϣ
{
	vector<QT>::reverse_iterator it;
	int i = 0;
	for (it = groupQT.rbegin(); it != groupQT.rend(); it++)
	{
		if (it->res.name != "") //������res����,��ȻҲ����Ϊ����
		{
			i = searchSYMBL(it->res.name); //������res���ڣ������ܲ�������SYMBL���У��ʲ����ж�
			it->res.active = SYMBL[i].active;
			SYMBL[i].active = false;
		}
		if (it->opB.type != 1 && it->opB.name != "") //������opB����,�Ҳ�Ϊ����
		{
			i = searchSYMBL(it->opB.name);
			it->opB.active = SYMBL[i].active;
			SYMBL[i].active = true;
		}
		if (it->opA.type != 1 && it->opA.name != "") //������opA����,�Ҳ�Ϊ����
		{
			i = searchSYMBL(it->opA.name);
			it->opA.active = SYMBL[i].active;
			SYMBL[i].active = true;
		}
	}

}

int main()
{
	initQT(groupQT);
	createDAG(groupQT);
	createQT();
	printQT(groupQT);
	return 0;
}