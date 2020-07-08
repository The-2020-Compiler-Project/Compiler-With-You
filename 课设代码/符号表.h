#pragma once
#ifndef _HEAD4_H
#define _HEAD4_H
#include<vector>
#include<iostream>
#include<string.h>

using namespace std;

//表的结构定义*****************************
struct TypeSheet;
struct MainSheet;
struct ArrSheet;
struct FuncSheet;
struct ParaSheet;
struct LevelAndOffsetAndValue;

struct TypeSheet //类型表
{
	string typevalue;		//类型编码：int,float ,char ,double
	ArrSheet* tpoint;		//根据类型编码跳转到不同的信息表项目（目前只有数组表)
};

struct MainSheet //符号总表
{
	string content;			//标识符名
	TypeSheet*  type;		//指向类型表
	string category;		//种类编码，/*category      v:变量， vn:形参， f:函数，c:常量*/
	void*  addr;			//标志，0,addr指向函数表; 1,addr指向LevelAndOffsetAndValue表
	int flag;				//判断地址指向表类型
};

struct ArrSheet {//数组表
	int low;
	int up;
	int clen;		//每个成员占多长
	TypeSheet* type;
};

struct FuncSheet //函数表
{
	int level;									//层次号
	int fnum;									//形参个数
	vector<ParaSheet*>* para;					//函数中的形参表
	string entry;								//入口地址，该函数目标程序首地址(运行时填写)；
	string value;
	//Vall* vall; //vall表
	//int off;				//区距离，该过函自身数据区起始单元相对该过函值区区头位置
};

struct ParaSheet {	//形参表
	string content;						//标识符名
	TypeSheet*  type;					//指向类型表
	LevelAndOffsetAndValue*  addr;		//指向LevelAndOffset表
};

struct LevelAndOffsetAndValue {
	int level;				//层次号
	int offset;				//偏移量
	string value;			//值
};

struct symbollistToFunc {		//一个函数对应一张符号表
	vector<MainSheet*> symbollist;
	string func;
	int offset_max;				//当前offset最大值
};
//表的结构定义*****************************

//表的存储*********************************
extern vector<symbollistToFunc*> symbollist_main_g;        //符号总表 
extern symbollistToFunc* symbollistToFunc_g;			   //函数所对应的符号表的指针
extern symbollistToFunc* globalsymbollistToFunc_g;		   //全局变量所要填入的地方	
extern MainSheet* mainSheet_g;					   //符号表项指针
extern TypeSheet* typeSheet_g;					   //类型表指针
extern FuncSheet* funcSheet_g;					   //函数表指针
extern vector<ParaSheet*>* paras_g;				   //参数表指针
extern ParaSheet* para_g;						   //参数表项指针
extern LevelAndOffsetAndValue* levelAndOffsetAndValue_g;		   //LevelAndOffset表指针
//表的存储*********************************


void ErrorAndShow(int col, string tmp, string tmp2);//报错函数


//表的删除***********************************
/*高级清理函数*/
void clearLists();			//释放所用存入表中的指针
void clearAll_g();			//释放所有全局指针
void clearGlobalVariable();	//释放全局变量（由globalsymbollistToFunc_g指向）
/*高级清理函数*/

/*底层清理函数*/
void deleteMainSheet(MainSheet* mainSheet_p);	//完全销毁一个MainSheet对象
void deleteTypeSheet(TypeSheet* typeSheet_p);	//完全销毁一个TypeSheet对象
void deleteLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue_p);	//完全销毁一个levelAndOffset对象
void deleteFuncSheet(FuncSheet* funcSheet_p);   //完全销毁一个FuncSheet对象
void deletePara(ParaSheet* para_p);				//完全销毁一个ParaSheet对象
/*底层清理函数*/
//表的删除***********************************

//检查重定义（常量表）*********************************
bool checkGlobalNum(string str);
bool checksymbollistToFuncNum(string str);

void checksymbollist_main(string str);		//查重符号总表
void checksymbollistToFunc(string funcId, string str);		//查重函数符号表
void checkParas(string str);				//查重形参表
void checkGlobal(string str);				//查重全局变量
//检查重定义（常量表）*********************************

//单张表的填写****************************************
void writeTypeSheet(string type);							//写一张类型表 参数:类型（新建）
void writeLevelAndOffsetAndValue(int level,int offset);//写一张LevelAndOffsetAndValue表，
void showErrowWhenCreateASheet(string str);					//在填写以上两表时报错
//单张表的填写****************************************

//表的输出********************************************
void printMainSheet(MainSheet* mainSheet);					//输出MainSheet表	
void printTypeSheet(TypeSheet* typeSheet);					//输出TypeSheet表
void printArrSheet(ArrSheet* arrsheet);						//输出数组表
void printLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue);	//输出LevelAndOffset表
void printfFuncSheet(FuncSheet* funcsheet);					//函数表输出
void printParaSheet(ParaSheet* paraSheet);					//输出形参表
//表的输出********************************************

//查表与填表******************************************
string searchType(string funcname, string name);//查非全局用户定义和临时变量类型
string searchType(string name);//查全局用户定义变量类型
string searchValue(string funcname, string name);//查非全局变量和临时变量值
string searchValue(string name);//查全局用户定义变量值
void saveTemp(string funcname, string name, string type, string value);//保存临时变量类型和值
void saveAdmin(string funcname, string name, string value);//保存用户的非全局变量的类型和值
void saveGlobal(string name, string value);			//保存用户定义的全局变量
void saveGlobal(string name, string type, string value); //保存临时全局变量
//查表与填表******************************************
#endif
