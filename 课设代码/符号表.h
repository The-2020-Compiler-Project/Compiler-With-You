#pragma once
#ifndef _HEAD4_H
#define _HEAD4_H
#include<string>
#include<vector>
#include<iostream>
#include<queue>
#include<fstream>

#include"语法分析.h"

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
	int clen;		//每个成员占多长
	TypeSheet* type;
};

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

struct ParaSheet {	//形参表
	string content;						//标识符名
	TypeSheet*  type;					//指向类型表
	LevelAndOffsetAndValue*  addr;		//指向LevelAndOffset表
};

struct FuncSheet
{
	int level;									//层次号
	int fnum;									//形参个数
	vector<ParaSheet*>* para;					//函数中的形参表
	string entry;								//入口地址，该函数目标程序首地址(运行时填写)；
	string value;
	//Vall* vall; //vall表
	//int off;				//区距离，该过函自身数据区起始单元相对该过函值区区头位置
};

struct LevelAndOffsetAndValue {
	int level;				//层次号
	int offset;				//偏移量
	string value;			//值
};


struct SynbollistToFunc {		//一个函数对应一张符号表
	vector<MainSheet*> synbollist;
	string func;
	int offset_max;				//当前offset最大值
};

extern vector<SynbollistToFunc*> synbollist_main_g;        //符号总表 
extern SynbollistToFunc* synbollistToFunc_g;			   //函数所对应的符号表的指针
extern SynbollistToFunc* globalsynbollistToFunc_g;		   //全局变量所要填入的地方	
extern MainSheet* mainSheet_g;					   //符号表项指针
extern TypeSheet* typeSheet_g;					   //类型表指针
extern FuncSheet* funcSheet_g;					   //函数表指针
extern vector<ParaSheet*>* paras_g;				   //参数表指针
extern ParaSheet* para_g;						   //参数表项指针
extern LevelAndOffsetAndValue* levelAndOffsetAndValue_g;		   //LevelAndOffset表指针


extern string type_g;	//用于暂时保存类型
extern string id_g;		//用于暂时保存标识符的名字

extern int fnum_g;		//记录形参个数
extern int offset_g;	//记录offset值

extern int global_offset_g;	//记录全局offset值

extern queue<MainSheet*> tempMainSheetpointers;	 //用于暂时保存MainSheet对象的指针


bool checkGlobalNum(string str);
bool checkSynbollistToFuncNum(string str);


void checkSynbollist_main(string str);		//查重符号总表
void checkSynbollistToFunc(string funcId, string str);		//查重函数符号表
void checkParas(string str);				//查重形参表
void checkGlobal(string str);				//查重全局变量


//为了实现这6个函数，定义一些模块化函数......
void writeTypeSheet(string type);							//写一张类型表 参数:类型
void writeLevelAndOffsetAndValue(int level, int offset);		//写一张LevelAndOffsetAndValue表，value暂时填不了
void showErrowWhenCreateASheet(string str);


/*高级输出函数*/
void printGlobal();											//输出全局变量
void printSynbollistMain_g();								//输出符号总表的所有内容
/*高级输出函数*/



/*高级清理函数*/
void clearLists();			//释放所用存入表中的指针
void clearAll_g();			//释放所有全局指针
void clearGlobalVariable();	//释放全局变量（由globalsynbollistToFunc_g指向）
/*高级清理函数*/



/*底层输出函数*/
void printMainSheet(MainSheet* mainSheet);					//输出MainSheet表	
void printTypeSheet(TypeSheet* typeSheet);					//输出TypeSheet表
void printLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue);	//输出LevelAndOffset表
void printfFuncSheet(FuncSheet* funcsheet);					//函数表输出
void printParaSheet(ParaSheet* paraSheet);					//输出形参表
/*底层输出函数*/



/*底层清理函数*/
void deleteMainSheet(MainSheet* mainSheet_p);	//完全销毁一个MainSheet对象
void deleteTypeSheet(TypeSheet* typeSheet_p);	//完全销毁一个TypeSheet对象
void deleteLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue_p);	//完全销毁一个levelAndOffset对象
void deleteFuncSheet(FuncSheet* funcSheet_p);   //完全销毁一个FuncSheet对象
void deletePara(ParaSheet* para_p);				//完全销毁一个ParaSheet对象
/*底层清理函数*/

//void deleteLenSheet(LenSheet* lenSheet_p);			//完全销毁一个LenSheet对象
//void deleteArrSheet(ArrSheet* arrSheet_p);			//完全销毁一个ArrSheet对象



/*最终要实现的7个函数*/
//没查到返回no
string searchType(string funcname, string name);//查非全局用户定义和临时变量类型
string searchType(string name);//查全局用户定义变量类型
string searchValue(string funcname, string name);//查非全局变量和临时变量值
string searchValue(string name);//查全局用户定义变量值
void saveTemp(string funcname, string name, string type, string value);//保存临时变量类型和值
void saveAdmin(string funcname, string name, string value);//保存用户的非全局变量的类型和值
void saveGlobal(string name, string value);			//保存用户定义的全局变量
void saveGlobal(string name, string type, string value); //保存临时全局变量
/*最重要实现的7个函数*/


void writeGlobalToFile(string fileName);
void writeSynbollistMain_gToFile(string fileName);

void writeMainSheetToFile(string funcName, MainSheet* mainSheet);
void writeTypeSheetToFile(string funcName, TypeSheet* typeSheet);
void writeLevelAndOffsetAndValueToFile(string funcName, LevelAndOffsetAndValue* levelAndOffsetAndValue);	//输出LevelAndOffset表
void writeFuncSheetToFile(string funcName, FuncSheet* funcsheet);					//函数表输出
void writeParaSheetToFile(string funcName, ParaSheet* paraSheet);

string TypeOfPara(string funcName, int pos);//返回函数对应位置形参类型
int NumOfPara(string fucName);//返回函数形参个数


void checkSynbollist_main(string str) {		//查重符号总表（即查是否函数重定义）
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (str == synbollist_main_g.at(i)->func) {
			string s = str + "重定义";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			ErrorAndShow(token_g.row, token_g.content, s);
		}
	}
}
void checkSynbollistToFunc(string funcId, string str) {	//查重函数符号表（查局部变量是否重定义）
	for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
		if (str == synbollistToFunc_g->synbollist.at(i)->content) {
			string s = str + "重定义";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			ErrorAndShow(token_g.row, token_g.content, s);
		}
	}
}

void checkParas(string str) {				//查重形参表(查形参是否重定义)
	for (unsigned i = 0; i < paras_g->size(); i++) {
		if (str == paras_g->at(i)->content) {
			string s = str + "重定义";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			ErrorAndShow(token_g.row, token_g.content, s);
		}
	}
}

void checkGlobal(string str) {				//查重全局变量
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (str == globalsynbollistToFunc_g->synbollist.at(i)->content) {
			string s = "全局变量" + str + "重定义";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			ErrorAndShow(token_g.row, token_g.content, s);
		}
	}
}


bool checkGlobalNum(string str) {
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (str == globalsynbollistToFunc_g->synbollist.at(i)->content) {
			return true;
		}
	}
	return false;
}

bool checkSynbollistToFuncNum(string str) {
	for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
		if (str == synbollistToFunc_g->synbollist.at(i)->content) {
			return true;
		}
	}
	return false;
}




//为了实现这6个函数，定义一些模块化函数......

void writeTypeSheet(string type) {		//写一张类型表
	if (typeSheet_g == NULL) {
		typeSheet_g = new TypeSheet;
		typeSheet_g->typevalue = type;
		typeSheet_g->tpoint = NULL;
	}
	else {
		showErrowWhenCreateASheet("TypeSheet");
	}
}

void writeLevelAndOffsetAndValue(int level, int offset) {	//写一张表，这张表存的是 level offset vlaue
	if (levelAndOffsetAndValue_g == NULL) {
		levelAndOffsetAndValue_g = new LevelAndOffsetAndValue;
		levelAndOffsetAndValue_g->level = level;
		levelAndOffsetAndValue_g->offset = offset;
		levelAndOffsetAndValue_g->value = "I don't know till now";
	}
	else {
		showErrowWhenCreateASheet("LevelAndOffsetAndValue");
	}
}


void showErrowWhenCreateASheet(string str) {
	string s = "在创建一个" + str + "对象时出现冲突";
	clearAll_g();
	clearLists();
	clearGlobalVariable();
	ErrorAndShow(token_g.row, token_g.content, s);
}

/*高级输出函数*/
void printGlobal() {											//输出全局变量
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		printMainSheet(globalsynbollistToFunc_g->synbollist.at(i));
	}
}
void printSynbollistMain_g() {								//输出符号总表的所有内容
	for (unsigned int i = 0; i < synbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
			printMainSheet(synbollist_main_g.at(i)->synbollist.at(j));
		}
	}
}
/*底层输出函数*/



/*高级清理函数*/
void clearLists() {			//释放所用存入表中的指针
	//synbollist_main_g
	for (unsigned int i = 0; i < synbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
			deleteMainSheet(synbollist_main_g.at(i)->synbollist.at(j));
		}
		delete synbollist_main_g.at(i);
	}
}

void clearAll_g() {			//释放所有全局指针和队列中的指针,只有在出错时会调用
	delete synbollistToFunc_g;			   //函数所对应的符号表的指针
	delete mainSheet_g;					   //符号表项指针
	delete typeSheet_g;					   //类型表指针
	delete funcSheet_g;					   //函数表指针
	delete paras_g;						   //参数表指针
	delete para_g;						   //参数表项指针
	delete levelAndOffsetAndValue_g;	   //LevelAndOffset表指针
	//释放队列指针
	while (!tempMainSheetpointers.empty()) {
		delete tempMainSheetpointers.front();
		tempMainSheetpointers.pop();
	}
}

void clearGlobalVariable() {	//释放全局变量（由globalsynbollistToFunc_g指向）
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		deleteMainSheet(globalsynbollistToFunc_g->synbollist.at(i));
	}
}
/*高级清理函数*/



/*底层输出函数*/
void printMainSheet(MainSheet* mainSheet) {				//输出MainSheet表	
	cout << "MainSheet表内容: " << endl;
	cout << "标识符名字: " << mainSheet->content << endl;
	cout << "标识符类型: " << endl; printTypeSheet(mainSheet->type);
	cout << "标识符种类: " << mainSheet->category << endl;
	if (mainSheet->flag == 0) {
		printfFuncSheet((FuncSheet*)mainSheet->addr);
	}
	else if (mainSheet->flag == 1) {
		printLevelAndOffsetAndValue((LevelAndOffsetAndValue*)mainSheet->addr);
	}
}
void printTypeSheet(TypeSheet* typeSheet) {					//输出TypeSheet表
	cout << "----" << "TypeSheet表内容: " << endl;
	cout << "    " << "typeValue: " << typeSheet->typevalue << endl;
	//因为目前没有数组和结构体所以typeSheet的指针项为空
	cout << endl;
}
void printLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue) {	//输出LevelAndOffset表
	cout << "----" << "LevelAndOffset表内容: " << endl;
	cout << "    " << "level:  " << levelAndOffsetAndValue->level << endl;
	cout << "    " << "offset: " << levelAndOffsetAndValue->offset << endl;
	cout << "    " << "vlaue:  " << levelAndOffsetAndValue->value << endl;
}
void printfFuncSheet(FuncSheet* funcsheet) {				//函数表输出
	cout << "----" << "函数表内容: " << endl;
	cout << "    " << "函数层次: " << funcsheet->level << endl;
	cout << "    " << "形参个数: " << funcsheet->fnum << endl;
	cout << "    " << "入口地址: " << funcsheet->entry << endl;
	cout << "    " << "参数表: " << endl;
	for (unsigned i = 0; i < funcsheet->para->size(); i++) {
		printParaSheet(funcsheet->para->at(i));
	}
	cout << endl;
}
void printParaSheet(ParaSheet* paraSheet) {					//输出形参表
	cout << "----" << "Para表内容: " << endl;
	cout << "    " << "形参名: " << paraSheet->content << endl;
	cout << "    " << "形参类型: "; printTypeSheet(paraSheet->type);
	cout << "    " << "形参表的LevelAndOffsetAndValue表: "; printLevelAndOffsetAndValue(paraSheet->addr);
	cout << endl;
}
/*底层输出函数*/



/*底层清理函数*/
void deleteMainSheet(MainSheet* mainSheet_p) {	//完全销毁一个MainSheet对象
	if (mainSheet_p->flag == 0) {
		deleteFuncSheet((FuncSheet*)mainSheet_p->addr);
	}
	else if (mainSheet_p->flag == 1) {
		deleteLevelAndOffsetAndValue((LevelAndOffsetAndValue*)mainSheet_p->addr);
	}
	deleteTypeSheet(mainSheet_p->type);
	delete mainSheet_p;
	cout << "成功释放了一个MainSheet对象" << endl;
}
void deleteTypeSheet(TypeSheet* typeSheet_p) {	//完全销毁一个TypeSheet对象
	if (typeSheet_p == NULL)return;
	delete typeSheet_p;
	cout << "成功释放了一个TypeSheet对象" << endl;
}
void deleteLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue_p) {	//完全销毁一个levelAndOffset对象
	if (levelAndOffsetAndValue_p == NULL)return;
	delete levelAndOffsetAndValue_p;
	cout << "成功释放了一个LevelAndOffsetAndValue对象" << endl;
}
void deleteFuncSheet(FuncSheet* funcSheet_p) {   //完全销毁一个FuncSheet对象
	if (funcSheet_p == NULL) {
		return;
	}
	for (unsigned i = 0; i < funcSheet_p->para->size(); i++) {
		deletePara(funcSheet_p->para->at(i));
	}
	delete funcSheet_p;
	cout << "成功释放了一个FuncSheet对象" << endl;
}
void deletePara(ParaSheet* para_p) {				//完全销毁一个ParaSheet对象
	deleteLevelAndOffsetAndValue(para_p->addr);
	deleteTypeSheet(para_p->type);
	delete para_p;
	cout << "成功释放了一个Para对象" << endl;
}
/*底层清理函数*/


/*最终要实现的7个函数*/
//没查到返回no
string searchType(string funcname, string name) {//查非全局用户定义和临时变量类型
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (synbollist_main_g.at(i)->func == funcname) {
			for (unsigned j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
				if (synbollist_main_g.at(i)->synbollist.at(j)->content == name) {
					return synbollist_main_g.at(i)->synbollist.at(j)->type->typevalue;
				}
			}
		}
	}
	if (synbollistToFunc_g != NULL)
	{
		for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
			if (synbollistToFunc_g->synbollist.at(i)->content == name) {
				return synbollistToFunc_g->synbollist.at(i)->type->typevalue;
			}
		}
	}
	return "no";
}

string searchType(string name) {//查全局用户定义变量类型
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (globalsynbollistToFunc_g->synbollist.at(i)->content == name) {
			return globalsynbollistToFunc_g->synbollist.at(i)->type->typevalue;
		}
	}
	return "no";
}
string searchValue(string funcname, string name) {//查非全局变量和临时变量值
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (synbollist_main_g.at(i)->func == funcname) {
			for (unsigned j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
				if (synbollist_main_g.at(i)->synbollist.at(j)->content == name) {
					if (synbollist_main_g.at(i)->synbollist.at(j)->category == "f") {
						return ((FuncSheet*)synbollist_main_g.at(i)->synbollist.at(j)->addr)->value;
					}
					else {
						if (((LevelAndOffsetAndValue*)(synbollist_main_g.at(i)->synbollist.at(j)->addr))->value != "I don't know till now") {
							return ((LevelAndOffsetAndValue*)(synbollist_main_g.at(i)->synbollist.at(j)->addr))->value;
						}
						else {
							return "no";
						}
					}
				}
			}
		}
	}
	if (synbollistToFunc_g != NULL) {
		for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
			if (name == synbollistToFunc_g->synbollist.at(i)->content) {
				if (((LevelAndOffsetAndValue*)(synbollistToFunc_g->synbollist.at(i))->addr)->value != "I don't know till now") {
					return ((LevelAndOffsetAndValue*)(synbollistToFunc_g->synbollist.at(i))->addr)->value;
				}
				else {
					return "no";
				}
			}
		}
	}

	return "no";
}
string searchValue(string name) {//查全局用户定义变量值
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (globalsynbollistToFunc_g->synbollist.at(i)->content == name) {
			if (((LevelAndOffsetAndValue*)(globalsynbollistToFunc_g->synbollist.at(i)->addr))->value != "I don't know till now") {
				return ((LevelAndOffsetAndValue*)(globalsynbollistToFunc_g->synbollist.at(i)->addr))->value;
			}
			else {
				return "no";
			}
		}
	}
	return "no";
}
void saveTemp(string funcname, string name, string type, string value) {//保存临时变量类型和值
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (funcname == synbollist_main_g.at(i)->func) {
			MainSheet* tempMainSheet;

			tempMainSheet = new MainSheet;
			tempMainSheet->content = name;
			tempMainSheet->category = "vt";
			tempMainSheet->flag = 1;
			writeTypeSheet(type);
			tempMainSheet->type = typeSheet_g;
			typeSheet_g = NULL;
			writeLevelAndOffsetAndValue(((LevelAndOffsetAndValue*)(synbollist_main_g.at(i)->synbollist.at(0)->addr))->level, synbollist_main_g.at(i)->offset_max);
			if (type == "char") {
				synbollist_main_g.at(i)->offset_max += 1;
			}
			else if (type == "int") {
				synbollist_main_g.at(i)->offset_max += 4;
			}
			else if (type == "double") {
				synbollist_main_g.at(i)->offset_max += 8;
			}
			else if (type == "float") {
				synbollist_main_g.at(i)->offset_max += 4;
			}
			levelAndOffsetAndValue_g->value = value;
			tempMainSheet->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			synbollist_main_g.at(i)->synbollist.push_back(tempMainSheet);
			tempMainSheet = NULL;
			return;

		}
	}

	if (synbollistToFunc_g != NULL && synbollistToFunc_g->func == funcname) {
		MainSheet* tempMainSheet;

		tempMainSheet = new MainSheet;
		tempMainSheet->content = name;
		tempMainSheet->category = "vt";
		tempMainSheet->flag = 1;
		writeTypeSheet(type);
		tempMainSheet->type = typeSheet_g;
		typeSheet_g = NULL;
		writeLevelAndOffsetAndValue(((LevelAndOffsetAndValue*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, synbollistToFunc_g->offset_max);
		if (type == "char") {
			synbollistToFunc_g->offset_max += 1;
		}
		else if (type == "int") {
			synbollistToFunc_g->offset_max += 4;
		}
		else if (type == "double") {
			synbollistToFunc_g->offset_max += 8;
		}
		else if (type == "float") {
			synbollistToFunc_g->offset_max += 4;
		}
		levelAndOffsetAndValue_g->value = value;
		tempMainSheet->addr = levelAndOffsetAndValue_g;
		levelAndOffsetAndValue_g = NULL;
		synbollistToFunc_g->synbollist.push_back(tempMainSheet);
		tempMainSheet = NULL;

	}

}
void saveAdmin(string funcname, string name, string value) {//保存用户的非全局变量的类型和值
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (funcname == synbollist_main_g.at(i)->func) {
			for (unsigned j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
				if (name == synbollist_main_g.at(i)->synbollist.at(j)->content) {
					((LevelAndOffsetAndValue*)synbollist_main_g.at(i)->synbollist.at(j)->addr)->value = value;
					return;
				}
			}
		}
	}
	for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
		if (name == synbollistToFunc_g->synbollist.at(i)->content) {
			((LevelAndOffsetAndValue*)(synbollistToFunc_g->synbollist.at(i))->addr)->value = value;
		}
	}
}
void saveGlobal(string name, string value) {			//保存用户定义的全局变量
	unsigned i = 0;
	for (i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (name == globalsynbollistToFunc_g->synbollist.at(i)->content) {
			((LevelAndOffsetAndValue*)(globalsynbollistToFunc_g->synbollist.at(i)->addr))->value = value;
			break;
		}
	}
	if (i == globalsynbollistToFunc_g->synbollist.size()) {
		cout << "没查到要存的变量" << endl;
	}
}

void saveGlobal(string name, string type, string value) {			//保存全局临时变量

	if (mainSheet_g == NULL) {
		mainSheet_g = new MainSheet;
		mainSheet_g->content = name;
		mainSheet_g->category = "vt";
		mainSheet_g->flag = 1;
		writeTypeSheet(type);
		mainSheet_g->type = typeSheet_g;
		typeSheet_g = NULL;
		writeLevelAndOffsetAndValue(0, global_offset_g);
		if (type == "char") {
			global_offset_g += 1;
		}
		else if (type == "int") {
			global_offset_g += 4;
		}
		else if (type == "double") {
			global_offset_g += 8;
		}
		else if (type == "float") {
			global_offset_g += 4;
		}
		levelAndOffsetAndValue_g->value = value;
		mainSheet_g->addr = levelAndOffsetAndValue_g;
		levelAndOffsetAndValue_g = NULL;
		globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
		mainSheet_g = NULL;
	}
	else {
		showErrowWhenCreateASheet("MainSheet");
	}

}

void writeGlobalToFile(string fileName) {
	ofstream ofs(fileName, ios::app);
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		writeMainSheetToFile(fileName, globalsynbollistToFunc_g->synbollist.at(i));
	}
	ofs.close();
}
void writeSynbollistMain_gToFile(string fileName) {
	ofstream ofs(fileName, ios::app);
	for (unsigned int i = 0; i < synbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
			writeMainSheetToFile(fileName, synbollist_main_g.at(i)->synbollist.at(j));
		}
	}
	ofs.close();
}


void writeMainSheetToFile(string funcName, MainSheet* mainSheet) {
	ofstream ofs(funcName, ios::app);
	ofs << "主表内容: " << endl;
	ofs << "标识符: " << mainSheet->content << endl;
	ofs << "类型: " << endl; printTypeSheet(mainSheet->type);
	ofs << "种类: " << mainSheet->category << endl;
	if (mainSheet->flag == 0) {
		writeFuncSheetToFile(funcName, (FuncSheet*)mainSheet->addr);
	}
	else if (mainSheet->flag == 1) {
		writeLevelAndOffsetAndValueToFile(funcName, (LevelAndOffsetAndValue*)mainSheet->addr);
	}
	ofs << endl;
	ofs.close();
}
void writeTypeSheetToFile(string funcName, TypeSheet* typeSheet) {
	ofstream ofs(funcName, ios::app);
	ofs << "----" << "TypeSheet表内容: " << endl;
	ofs << "    " << "typeValue: " << typeSheet->typevalue << endl;
	//因为目前没有数组和结构体所以typeSheet的指针项为空
	ofs << endl;
	ofs.close();
}
void writeLevelAndOffsetAndValueToFile(string funcName, LevelAndOffsetAndValue* levelAndOffsetAndValue) {
	ofstream ofs(funcName, ios::app);
	ofs << "    " << "层次:  " << levelAndOffsetAndValue->level << endl;
	ofs << "    " << "区距: " << levelAndOffsetAndValue->offset << endl;
	ofs << "    " << "值:  " << levelAndOffsetAndValue->value << endl;
	ofs << endl;
	ofs.close();
}
void writeFuncSheetToFile(string funcName, FuncSheet* funcsheet) {
	ofstream ofs(funcName, ios::app);
	ofs << "----" << "函数表内容: " << endl;
	ofs << "    " << "层次: " << funcsheet->level << endl;
	ofs << "    " << "形参个数: " << funcsheet->fnum << endl;
	ofs << "    " << "入口地址: " << funcsheet->entry << endl;
	for (unsigned i = 0; i < funcsheet->para->size(); i++) {
		writeParaSheetToFile(funcName, funcsheet->para->at(i));
	}
	ofs << endl;
	ofs.close();
}
void writeParaSheetToFile(string funcName, ParaSheet* paraSheet) {
	ofstream ofs(funcName, ios::app);
	ofs << "----" << "参数表内容: " << endl;
	ofs << "    " << "形参名: " << paraSheet->content << endl;
	ofs << "    " << "形参类型: "; writeTypeSheetToFile(funcName, paraSheet->type);
	ofs << "    " << "形参表的LevelAndOffsetAndValue表: "; writeLevelAndOffsetAndValueToFile(funcName, paraSheet->addr);
	ofs << endl;
	ofs.close();
}

/*最重要实现的7个函数*/
string TypeOfPara(string funcName, int pos) {
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (synbollist_main_g.at(i)->func == funcName) {
			return synbollist_main_g.at(i)->synbollist.at(pos)->type->typevalue;
		}
	}
	return "no";
}

//通过函数名，返回形参个数
int NumOfPara(string funcName) {
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (synbollist_main_g.at(i)->func == funcName) {
			return ((FuncSheet*)(synbollist_main_g.at(i)->synbollist.at(0)->addr))->fnum;
		}
	}
	return -1;
}

#endif
