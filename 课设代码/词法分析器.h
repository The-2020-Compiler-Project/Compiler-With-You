#pragma once
#pragma once
#ifndef HEAD_2_
#define HEAD_2_
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;
extern int row;
class Token {		//Token
public:
	Token(char t, string c, int r);
	Token();
	void set(char t, string c, int r);

	char type;		//���ʵ����ͣ���ؼ��֣���ʶ���������
	/*
	int token_location;		//����Ǳ�ʶ������ָ����ű�
							//����ǹؼ��֣���ָ��ؼ��ֱ�
							//����ǽ����  ��ָ������
							//����ǳ�����  ��ָ������
							//�������Ϊ�����ͣ������ͣ��ַ����ַ���
	*/
	string content;			//���token����ĵ���
	int row;
};

class Recognizer {		//ɨ����
public:
	string  fname;				//ɨ����ļ���
	ifstream ifs;				//������
	char ch;					//��ǰ�������ַ�
	int state;					//�Զ�����״̬
	int type;					//ɨ����ĵ��ʵ�����
	string keyOrIdentifier;
	string number;
	string String;
	string c;

	int numState;
	double n, num;
	int p, m, t, e;

	void init(string f);		//ȷ��ɨ����Ҫɨ����ļ�
	string getAWord();			//��Դ�ļ���ȡһ�����ʣ����ظ��ʷ�������

private:
	void getAChar();			//��Դ�ļ��ж���һ���ַ�
	void pushbackAChar();		//����һ���ַ�������
	bool isALetter();			//�ж�һ���ַ��Ƿ����ַ�
	bool isANumber();			//�ж�һ���ַ��Ƿ�������

};

class Lexer {		//�ʷ���������O(��_��)O
public:
	void init(string name);		//��ʼ��ɨ����
	Token next_Token();		//����һ��Token,����Ǳ�ʶ��ָ����ű��ؼ���ָ��ؼ��ֱ����ָ����������ָ������
private:
	Recognizer rec;				//ɨ����
	string currentWord;			//��ǰ�����ĵ���
};

#endif 
