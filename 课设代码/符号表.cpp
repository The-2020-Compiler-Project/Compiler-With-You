#include "符号表.h"
#include<fstream>


void ErrorAndShow(int col, string tmp, string tmp2)//错误提示
{
	ofstream myfile;
	myfile.open("D:\\byresult\\error.txt");
	myfile << "*************   ERROR!  ****************" << endl;
	myfile << tmp2 << endl;
	myfile << "行数 " << col << "  内容 " << tmp << "有问题" << endl;
	myfile.close();
	exit(1);
}

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

void writeLevelAndOffsetAndValue(int level, int offset) {	//写一张表，存 level offset vlaue
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

void showErrowWhenCreateASheet(string str) {//填表时报错
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

/*底层输出函数*/
void printMainSheet(MainSheet* mainSheet) {				//输出MainSheet表	
	cout << "MainSheet表内容: " << endl;
	cout << "标识符名字: " << mainSheet->content << endl;
	cout << "标识符类型: " << endl; printTypeSheet(mainSheet->type);
	cout << "标识符种类: " << mainSheet->category << endl;
	if (mainSheet->flag == 0) {
		printfFuncSheet((FuncSheet*)mainSheet->addr);
	}
	else if(mainSheet->flag == 1) {
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