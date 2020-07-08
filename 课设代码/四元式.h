#pragma once
#pragma once
#ifndef _HEAD3_H
#define _HEAD3_H
#define _HEAD_QT

#include<string>
#include<vector>
#include<iostream>
#include<sstream>
using namespace std;


extern int rr;//��ʱ��������
extern int sem;

extern string type_1;//����������
extern string type_2;

extern string value_1; //������ֵ
extern string value_2;

extern int func_num;
struct token
{
	string name;
	int type;// 1������2����ʱ������3����ʱ����
	bool active;//��Ծ��Ϣ
	token() { name = ""; type = 0; active = false; }
	token(string namet, int typet, bool activet) { name = namet; type = typet; active = activet; }
	void clear() { name = ""; type = 0; active = false; }
};

extern token left_n;
extern token right_n;
extern token res_n;

struct QT
{
	string Operat;//������
	token opA;
	token opB;
	token res;
	int block;//������
	QT() { Operat = ""; opA.clear(); opB.clear(); res.clear(); }
	QT(string Operatt, token opAt, token opBt, token rest) { Operat = Operatt; opA = opAt; opB = opBt; res = rest; }
	void clear() { Operat = ""; opA.clear(); opB.clear(); res.clear(); }

};

extern vector<QT> qt;

template<class T>
class mystack
{
	int top;
	T* mys;
	int maxsize;
public:
	mystack() :top(-1), maxsize(111)
	{
		mys = new T[maxsize];
		if (mys == NULL)
		{
			cerr << "��̬�洢����ʧ�ܣ�" << endl;
			exit(1);
		}
	}
	mystack(int size) :top(-1), maxsize(size)
	{
		mys = new T[maxsize];
		if (mys == NULL)
		{
			cerr << "��̬�洢����ʧ�ܣ�" << endl;
			exit(1);
		}
	}
	//~mystack()
	//{
	//	delete[] mys;
	//}
	bool Empty();//�Ƿ�Ϊ��
	void PUSH(T tp); //ѹջ
	T TOP();//����ջ��Ԫ��
	T SEC();//���ش�ջ��Ԫ��
	void POP();//��ջ
	int SIZE();//ջ��С
};

//ģ�庯������string���ͱ���ת��Ϊ���õ���ֵ���ͣ��˷��������ձ������ԣ�
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

extern mystack<token> SEM;
bool CompareType();
string resvalue(string opeart);
void ASSI();
void WriteinFileL(string name, string content);
void PUSHSEM(token operand);//д���ļ�
void GEQ(string operat);
void ASSI();
void IF();
void EL();
void IE();
void WH();
void DO();
void WE();
void RET();//RETURN��䣺��һ������
void WriteQt(string name, string content);//��Ԫʽд���ļ�

#endif