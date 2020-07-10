#pragma once
#ifndef _HEAD_DAG
#define _HEAD_DAG
#ifndef _HEAD_QT
#include "��Ԫʽ.h"
#endif // !_HEAD_QT

#include <iostream>
#include <string>
#include <vector>


struct node
{
	int num; //����
	int left; //����
	int right; //�ҽ��
	string Operat; //������
	token Mark[20]; //��ǣ�0��Ϊ�����
	node() { num = -1; left = -1; right = -1; Operat = ""; }
};

extern vector<QT>resQT; //��������Ż����������Ԫʽ
extern vector<QT>groupQT; //�Ż����ŵ���Ԫʽ��
extern vector<node>DAG;
extern vector<token>SYMBL;
extern vector<token>SYMBL_ALL;

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

#endif
