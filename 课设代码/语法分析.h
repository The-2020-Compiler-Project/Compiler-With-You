#pragma once
#pragma once
#pragma once
#pragma once
#ifndef _HEAD5_H
#define _HEAD5_H
#include"�ʷ�������.h"
#include"���ű�.h"
#include"��Ԫʽ.h"
#include<string>
using namespace std;

extern int bool_jubu; //�ж��ǲ����ڶ���ֲ�����

extern string id_qt;//��Ԫʽ�����ʶ��������λ��
extern string num_qt;
extern int arr_num_qt;//����Ԫ��

extern string func_name;//��¼��ǰ��������

extern Lexer scan;			//ɨ����
extern Token token_g;			//��������token��

extern string changtype_1;//��������
extern string changtype_2;

extern vector<string> T_num;//���汻�û��������ʱ����tnum

extern string func_call;//���ú���������
extern int func_call_num;
extern int parapos;//��������λ��

void Error(int col, string tmp, string tmp2);//�������
int Recursive();//�ӳ���������
int Variable_Type();//��������
int ID();//��ʶ��
int NumCharID();//��ʼ������
int ZeroOrMoreDeclation();//�����б�
int Declaration();//������ǰ�ַ�������
int Const_Declaration();//��������const
int Variable_Declaration();//�������壨���顢������
int Array_Declaration();//����������������ʼ�����������塢��������
int EqualStringOrNumOrNull();//�����ʼ��
int StringOrNum();
int Statement();		//���ĺ���
int Block();			//�����ڲ�����
int ExpressionStatement();//�����ı��ʽ���
int Expression();//���ʽ
int Assignment();//��ֵ���
int Assignment_1();
int Logical_Expression();//
int Other_Logical();//
int Equality();
int Equality_2();
int Arithmetic_Expression();
int Arithmetic_Expression_2();
int MD_Expression();
int Arithmetic_Expression_1();
int MD_Expression_1();
int Factor();
int Primary();//�ս��

int Paraters();			//�β�
int Realparaters();
int Realparaters_2();
int Realparaters_3();
int Other_Type_ID();
int ArrFunc();//��������

#endif











