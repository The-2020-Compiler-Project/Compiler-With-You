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
vector<QT>block_qt;
vector<QT>ceshi;
vector<node>DAG;
vector<token>SYMBL;
vector<token>SYMBL_ALL;
queue<token>param;
vector<token>RETm;
stack<token*>whstack; //while����ջ
stack<int>whstacknum; //while����ջ
QT specialQT;
token AX; //���Ĵ���
token BX; //���Ĵ���
token CX; //���Ĵ���
token DX; //���Ĵ���
string namem;//�洢��������
int ppp;//������������

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

void divide()//�����黮��
{
	int num = 1;
	vector<QT>::iterator QTit;
	QT QTtemp;
	for (QTit = ceshi.begin(); QTit != ceshi.end(); QTit++)//����һ����������
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

/*void divide()//�����黮��
{
	int num = 1;
	vector<QT>::iterator QTit;
	QT QTtemp;
	for (QTit = qt.begin(); QTit != qt.end(); QTit++)//����һ����������
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

//void cleanDAG() //��cleanɶ�����ϳ����ⲻclean��
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

void printQT3(vector<QT>tempQT) //��ӡ��Ԫʽ��
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
void initQT(vector<QT>& tempQT) //�����ù����ʼ��Ԫʽ��
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

/*void changeActive() //�������ɻ�Ծ��Ϣ
{
	int i;
	for (int j = int(groupQT.size())- 1; j >= 0; j--)
	{  if(groupQT[j].Operat != ""&&groupQT[j].Operat!="wh"&& groupQT[j].Operat != "do"&& groupQT[j].Operat != "we"&& groupQT[j].Operat != "if"&& groupQT[j].Operat != "ie"&& groupQT[j].Operat != "el")
		if (groupQT[j].res.name != "") //������res����,��ȻҲ����Ϊ����
		{
			i = searchSYMBL(groupQT[j].res.name); //������res���ڣ������ܲ�������SYMBL���У��ʲ����ж�
			if (i != -1)
			{
				groupQT[j].res.active = SYMBL[i].active;
				SYMBL[i].active = false;
			}
			
		}
		if (groupQT[j].opB.type != 1 && groupQT[j].opB.name != "") //������opB����,�Ҳ�Ϊ����
		{
			i = searchSYMBL(groupQT[j].opB.name);
			if (i != -1)
			{
				groupQT[j].opB.active = SYMBL[i].active;
				SYMBL[i].active = true;
			}
			
		}
		if (groupQT[j].opA.type != 1 && groupQT[j].opA.name != "") //������opA����,�Ҳ�Ϊ����
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
void changeActive() //�������ɻ�Ծ��Ϣ
{
	vector<QT>::reverse_iterator it;
	int i = 0;
	for (it = groupQT.rbegin(); it != groupQT.rend(); it++)
	{
		if (it->res.name != "") //������res����,��ȻҲ����Ϊ����
		{
			i = searchSYMBL(it->res.name); //������res���ڣ������ܲ�������SYMBL���У��ʲ����ж�
			if (i != -1&&i<SYMBL.size())
			{
				it->res.active = SYMBL[i].active;
				SYMBL[i].active = false;
			}
		}
		if (it->opB.type != 1 && it->opB.name != "") //������opB����,�Ҳ�Ϊ����
		{
			i = searchSYMBL(it->opB.name);
			if (i != -1 && i < SYMBL.size())
			{
				it->opB.active = SYMBL[i].active;
				SYMBL[i].active = true;
			}
		}
		if (it->opA.type != 1 && it->opA.name != "") //������opA����,�Ҳ�Ϊ����
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

void addSYMBL() //����һ��ȫ�ֵķ��ű�ΪDSEG����׼��
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

void createCode(vector<QT>resQT) //����һ����Ԫʽ������һ�δ���
{
	ofstream file;
	file.open("D:\\byresult\\code.txt", ios::app);
	vector<QT>::iterator it;
	string lastOper = "";
	ppp = 1;
	int i = 1; //��Ԫʽ���
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

void code(QT temp, string lastOper, int i) //���ݵ�����Ԫʽ����CODE
{
	ofstream addfile;
	addfile.open("D:\\byresult\\code.txt", ios::app);
	if (temp.Operat == "=")	//��ֵ���
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
	else if (temp.Operat == "+") //�ӷ�
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
	else if (temp.Operat == "-"&&(temp.opB.name!="")) //����������Ҫ��һ�����ţ���ûд
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
	else if (temp.Operat == "*") //�˷�
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
	else if (temp.Operat == "/") //����
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
	else if (temp.Operat == "&&") //�߼���
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
	else if (temp.Operat == "||") //�߼���
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
	else if (temp.Operat == "!=" || temp.Operat == "==" || temp.Operat == "<" || temp.Operat == "<=" || temp.Operat == ">" || temp.Operat == ">=") //�����ڣ��ж���ȣ����ڵ��ڣ����ڣ�С�ڵ��ڣ�С��
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
	else if (temp.Operat == "!") //��
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
	else if (temp.Operat == "if") //if���
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
	else if (temp.Operat == "el") //else���
	{
	    addfile << "	JMP	" << "QT" << temp.res.name << endl;
	    addfile << "QT" << i + 1 << ":";
	}
	else if (temp.Operat == "ie") //ie�ṹβ��
	{
	    addfile << "QT" << i << ":";
	}
	else if (temp.Operat == "wh") //while���
	{
	addfile << "QT" << i << ":";
	}
	else if (temp.Operat == "do") //do��䣬ѭ������
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
	else if (temp.Operat == "we") //while�ṹβ��
	{
	   addfile << "	JMP	" << "QT" << temp.res.name << endl;
	   addfile << "QT" << i << ":";
	}
	else if (temp.Operat == "FUNC")//���������һ��������Ϊ������
	{
	   addfile << temp.opA.name<<"      PROC" << endl;
	   namem = temp.opA.name;
    }
	else if (temp.Operat == "para_n")//����
	{
	   param.push(temp.opA);
	   addfile << "	" << temp.opA.name << "	DB	4 DUP(0)" << endl;
	}
	else if (temp.Operat == "RET")//����
	{
	   addfile << "            POP  " << "DX" << endl;
	   addfile << "            POP  " << "CX" << endl;
	   addfile << "            POP  " << "BX" << endl;
	   addfile << "    RET  " << endl;
	   addfile << namem << "    ENDP  " << endl;
	   RETm.push_back(temp.opA);
	}
	else if (temp.Operat == "para")//����
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
	else if (temp.Operat == "call")//��������
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
	/*else if (temp.Operat == "RET")//��������
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
	/*else if (temp.Operat == "END")//��������
	{
	    addfile << temp.opA.name <<"    ENDP  " << endl;
    }*/
}

void createDSEG() //����DSEG��
{
	//����ű����������ɿռ�
	//�����ʱ���ڻ�಻���ִ�Сд������Ӧ��������
	ofstream file;
	file.open("D:\\byresult\\code.txt");
	file << "SSEG	SEGMENT STACK" << endl;
	file << "SKT	DB	200 DUP(0)" << endl;
	file << "SSEG	ENDS" << endl;
	file << "DSEG	SEGMENT " << endl;
	vector<token>::iterator tokent; //ԭ���Ǳ������ű������ȱ����ұ����SYMBL
	for (tokent = SYMBL_ALL.begin(); tokent != SYMBL_ALL.end(); tokent++)
	{
		file << "	" << tokent->name << "	DB	2 DUP(0)" << endl;
	}
	file << "DSEG	ENDS " << endl;
}

void createCSEGBEGIN() //����CSEG��
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

void createCSEGEND() //����CSEG��
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
	vector<QT>::iterator it;//ָ��qt���ݵĵ�����
	int i = 1;
	//block_qt.clear();
	for (it = ceshi.begin(); it != ceshi.end(); it++) //����qt
	{
		if (i != it->block) //itΪ��һ���������ڵ���Ԫʽ
		{
			createDAG(block_qt);//������Ԫʽ����DAGͼ
			createQT();//����DAGͼ������Ԫʽ
			createSYMBL();//�����Ծ��Ϣ��
			changeActive();//������д��Ծ��Ϣ
			//addSYMBL(); //����һ��ȫ�ֵķ��ű�ΪDSEG����׼��
			createRes(); //�����Ż������Ԫʽ��resQT
			i++;
			block_qt.clear();//������һ�����������
		}
		block_qt.push_back(*it); //ȡ��Ԫʽ
	}
}
void secondview()
{
	vector<QT>::iterator it;
	int i = 1; //��Ԫʽ�����
	stack<token*>ifstack; //if����ջ
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
	initQT(ceshi);//��ʼ��
	divide();//�����黮��
	printQT3(ceshi);//��ӡ���Ż�ǰ����Ԫʽ
	firstview();//��Ԫʽ�Ż��Լ���Ծ��Ϣ����д
	secondview();//��ע��תѭ������λ��
	printQT(resQT);//��ӡ�Ż������Ԫʽ
	printQT1(resQT);//��ӡ��Ԫʽ��д��Ծ��Ϣ������
	//createDSEG(); //����DSEG��
	//createCSEGBEGIN();//����CSEG�εĿ�ͷ
	createCode(resQT);//����CSEG�ε����ִ�в���
	//createCSEGEND();//����CSEG�εĽ�β
	return 0;
}