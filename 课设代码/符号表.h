#pragma once
#pragma once
#ifndef _HEAD4_H
#define _HEAD4_H
#include<string>
#include<vector>
#include<iostream>
#include<queue>
#include"�﷨����.h"
using namespace std;

struct TypeSheet;
struct MainSheet;
struct ArrSheet;
struct FuncSheet;
struct ParaSheet;
struct LevelAndOffsetAndValue;


struct ArrSheet {
	int low;
	int up;
	int clen;		//ÿ����Առ�೤
	TypeSheet* type;
};

struct TypeSheet //���ͱ�
{
	string typevalue;		//���ͱ��룺int,float ,char ,double
	ArrSheet* tpoint;		//�������ͱ�����ת����ͬ����Ϣ����Ŀ
};

struct MainSheet //�����ܱ�
{
	string content;			//��ʶ����
	TypeSheet*  type;		//ָ�����ͱ�
	string category;		//������룬/*category      v:������ vn:�βΣ� f:������c:����*/
	void*  addr;			//��־��0,addrָ������; 1,addrָ��LevelAndOffsetAndValue��
	int flag;				//�жϵ�ַָ�������
};

struct ParaSheet {	//�βα�
	string content;						//��ʶ����
	TypeSheet*  type;					//ָ�����ͱ�
	LevelAndOffsetAndValue*  addr;		//ָ��LevelAndOffset��
};

struct FuncSheet
{
	int level;									//��κ�
	int fnum;									//�βθ���
	vector<ParaSheet*>* para;					//�����е��βα�
	string entry;								//��ڵ�ַ���ú���Ŀ������׵�ַ(����ʱ��д)��
	string value;
	//Vall* vall; //vall��
	//int off;				//�����룬�ù���������������ʼ��Ԫ��Ըù���ֵ����ͷλ��
};

struct LevelAndOffsetAndValue {
	int level;				//��κ�
	int offset;				//ƫ����
	string value;			//ֵ
};


struct SynbollistToFunc {		//һ��������Ӧһ�ŷ��ű�
	vector<MainSheet*> synbollist;
	string func;
	int offset_max;				//��ǰoffset���ֵ
};


extern vector<SynbollistToFunc*> synbollist_main_g;        //�����ܱ� 
extern SynbollistToFunc* synbollistToFunc_g;			   //��������Ӧ�ķ��ű��ָ��
extern SynbollistToFunc* globalsynbollistToFunc_g;		   //ȫ�ֱ�����Ҫ����ĵط�	
extern MainSheet* mainSheet_g;					   //���ű���ָ��
extern TypeSheet* typeSheet_g;					   //���ͱ�ָ��
extern FuncSheet* funcSheet_g;					   //������ָ��
extern vector<ParaSheet*>* paras_g;				   //������ָ��
extern ParaSheet* para_g;						   //��������ָ��
extern LevelAndOffsetAndValue* levelAndOffsetAndValue_g;		   //LevelAndOffset��ָ��


extern string type_g;	//������ʱ��������
extern string id_g;		//������ʱ�����ʶ��������
extern int fnum_g;		//��¼�βθ���
extern int offset_g;	//��¼offsetֵ
extern int global_offset_g;	//��¼ȫ��offsetֵ
extern queue<MainSheet*> tempMainSheetpointers;	 //������ʱ����MainSheet�����ָ��


bool checkGlobalNum(string str);
bool checkSynbollistToFuncNum(string str);
void checkSynbollist_main(string str);		//���ط����ܱ�
void checkSynbollistToFunc(string funcId, string str);		//���غ������ű�
void checkParas(string str);				//�����βα�
void checkGlobal(string str);				//����ȫ�ֱ���



//Ϊ��ʵ����6������������һЩģ�黯����......
void writeTypeSheet(string type);							//дһ�����ͱ� ����:����
void writeLevelAndOffsetAndValue(int level, int offset);		//дһ��LevelAndOffsetAndValue��value��ʱ���
void showErrowWhenCreateASheet(string str);




/*�߼��������*/
void printGlobal();											//���ȫ�ֱ���
void printSynbollistMain_g();								//��������ܱ����������
/*�߼��������*/



/*�߼�������*/
void clearLists();			//�ͷ����ô�����е�ָ��
void clearAll_g();			//�ͷ�����ȫ��ָ��
void clearGlobalVariable();	//�ͷ�ȫ�ֱ�������globalsynbollistToFunc_gָ��
/*�߼�������*/



/*�ײ��������*/
void printMainSheet(MainSheet* mainSheet);					//���MainSheet��	
void printTypeSheet(TypeSheet* typeSheet);					//���TypeSheet��
void printLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue);	//���LevelAndOffset��
void printfFuncSheet(FuncSheet* funcsheet);					//���������
void printParaSheet(ParaSheet* paraSheet);					//����βα�
/*�ײ��������*/



/*�ײ�������*/
void deleteMainSheet(MainSheet* mainSheet_p);	//��ȫ����һ��MainSheet����
void deleteTypeSheet(TypeSheet* typeSheet_p);	//��ȫ����һ��TypeSheet����
void deleteLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue_p);	//��ȫ����һ��levelAndOffset����
void deleteFuncSheet(FuncSheet* funcSheet_p);   //��ȫ����һ��FuncSheet����
void deletePara(ParaSheet* para_p);				//��ȫ����һ��ParaSheet����
/*�ײ�������*/


/*�ӿں���*/
//û�鵽����no
string searchType(string funcname, string name);//���ȫ���û��������ʱ��������
string searchType(string name);//��ȫ���û������������
string searchValue(string funcname, string name);//���ȫ�ֱ�������ʱ����ֵ
string searchValue(string name);//��ȫ���û��������ֵ
void saveTemp(string funcname, string name, string type, string value);//������ʱ�������ͺ�ֵ
void saveAdmin(string funcname, string name, string value);//�����û��ķ�ȫ�ֱ��������ͺ�ֵ
void saveGlobal(string name, string value);			//�����û������ȫ�ֱ���
void saveGlobal(string name, string type, string value); //������ʱȫ�ֱ���
/*�ӿں���*/


void writeGlobalToFile(string fileName);
void writeSynbollistMain_gToFile(string fileName);

void writeMainSheetToFile(string funcName, MainSheet* mainSheet);
void writeTypeSheetToFile(string funcName, TypeSheet* typeSheet);
void writeLevelAndOffsetAndValueToFile(string funcName, LevelAndOffsetAndValue* levelAndOffsetAndValue);	//���LevelAndOffset��
void writeFuncSheetToFile(string funcName, FuncSheet* funcsheet);					//���������
void writeParaSheetToFile(string funcName, ParaSheet* paraSheet);

string TypeOfPara(string funcName, int pos);//���غ�����Ӧλ���β�����
int NumOfPara(string fucName);//���غ����βθ���

#endif
