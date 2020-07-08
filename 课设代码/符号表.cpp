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
	//symbollist_main_g
	for (unsigned int i = 0; i < symbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < symbollist_main_g.at(i)->symbollist.size(); j++) {
			deleteMainSheet(symbollist_main_g.at(i)->symbollist.at(j));
		}
		delete symbollist_main_g.at(i);
	}
}

void clearAll_g() {			//释放所有全局指针和队列中的指针,只有在出错时会调用
	delete symbollistToFunc_g;			   //函数所对应的符号表的指针
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

void clearGlobalVariable() {	//释放全局变量（由globalsymbollistToFunc_g指向）
	for (unsigned i = 0; i < globalsymbollistToFunc_g->symbollist.size(); i++) {
		deleteMainSheet(globalsymbollistToFunc_g->symbollist.at(i));
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


void checksymbollist_main(string str) {		//查重符号总表（即查是否函数重定义）
	for (unsigned i = 0; i < symbollist_main_g.size(); i++) {
		if (str == symbollist_main_g.at(i)->func) {
			string s = str + "重定义";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			ErrorAndShow(token_g.row, token_g.content, s);
		}
	}
}
void checksymbollistToFunc(string funcId, string str) {	//查重函数符号表（查局部变量是否重定义）
	for (unsigned i = 0; i < symbollistToFunc_g->symbollist.size(); i++) {
		if (str == symbollistToFunc_g->symbollist.at(i)->content) {
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
	for (unsigned i = 0; i < globalsymbollistToFunc_g->symbollist.size(); i++) {
		if (str == globalsymbollistToFunc_g->symbollist.at(i)->content) {
			string s = "全局变量" + str + "重定义";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			ErrorAndShow(token_g.row, token_g.content, s);
		}
	}
}


bool checkGlobalNum(string str) {
	for (unsigned i = 0; i < globalsymbollistToFunc_g->symbollist.size(); i++) {
		if (str == globalsymbollistToFunc_g->symbollist.at(i)->content) {
			return true;
		}
	}
	return false;
}

bool checksymbollistToFuncNum(string str) {
	for (unsigned i = 0; i < symbollistToFunc_g->symbollist.size(); i++) {
		if (str == symbollistToFunc_g->symbollist.at(i)->content) {
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
	for (unsigned i = 0; i < globalsymbollistToFunc_g->symbollist.size(); i++) {
		printMainSheet(globalsymbollistToFunc_g->symbollist.at(i));
	}
}
void printsymbollistMain_g() {								//输出符号总表的所有内容
	for (unsigned int i = 0; i < symbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < symbollist_main_g.at(i)->symbollist.size(); j++) {
			printMainSheet(symbollist_main_g.at(i)->symbollist.at(j));
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
	printArrSheet(typeSheet->tpoint);
	cout << endl;
}

void printArrSheet(ArrSheet* arrsheet){//输出数组表
	cout << "ArrSheet表内容" << endl;
	cout << "下界：" << arrsheet->low;
	cout << "上界：" << arrsheet->up;
	cout << "成员长度：" << arrsheet->clen;
	printTypeSheet(arrsheet->type);
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

string searchType(string funcname, string name) {//查非全局用户定义和临时变量类型
	for (unsigned i = 0; i < symbollist_main_g.size(); i++) {
		if (symbollist_main_g.at(i)->func == funcname) {
			for (unsigned j = 0; j < symbollist_main_g.at(i)->symbollist.size(); j++) {
				if (symbollist_main_g.at(i)->symbollist.at(j)->content == name) {
					return symbollist_main_g.at(i)->symbollist.at(j)->type->typevalue;
				}
			}
		}
	}
	if (symbollistToFunc_g != NULL)
	{
		for (unsigned i = 0; i < symbollistToFunc_g->symbollist.size(); i++) {
			if (symbollistToFunc_g->symbollist.at(i)->content == name) {
				return symbollistToFunc_g->symbollist.at(i)->type->typevalue;
			}
		}
	}
	return "no";
}

string searchType(string name) {//查全局用户定义变量类型
	for (unsigned i = 0; i < globalsymbollistToFunc_g->symbollist.size(); i++) {
		if (globalsymbollistToFunc_g->symbollist.at(i)->content == name) {
			return globalsymbollistToFunc_g->symbollist.at(i)->type->typevalue;
		}
	}
	return "no";
}
string searchValue(string funcname, string name) {//查非全局变量和临时变量值
	for (unsigned i = 0; i < symbollist_main_g.size(); i++) {
		if (symbollist_main_g.at(i)->func == funcname) {
			for (unsigned j = 0; j < symbollist_main_g.at(i)->symbollist.size(); j++) {
				if (symbollist_main_g.at(i)->symbollist.at(j)->content == name) {
					if (symbollist_main_g.at(i)->symbollist.at(j)->category == "f") {
						return ((FuncSheet*)symbollist_main_g.at(i)->symbollist.at(j)->addr)->value;
					}
					else {
						if (((LevelAndOffsetAndValue*)(symbollist_main_g.at(i)->symbollist.at(j)->addr))->value != "I don't know till now") {
							return ((LevelAndOffsetAndValue*)(symbollist_main_g.at(i)->symbollist.at(j)->addr))->value;
						}
						else {
							return "no";
						}
					}
				}
			}
		}
	}
	if (symbollistToFunc_g != NULL) {
		for (unsigned i = 0; i < symbollistToFunc_g->symbollist.size(); i++) {
			if (name == symbollistToFunc_g->symbollist.at(i)->content) {
				if (((LevelAndOffsetAndValue*)(symbollistToFunc_g->symbollist.at(i))->addr)->value != "I don't know till now") {
					return ((LevelAndOffsetAndValue*)(symbollistToFunc_g->symbollist.at(i))->addr)->value;
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
	for (unsigned i = 0; i < globalsymbollistToFunc_g->symbollist.size(); i++) {
		if (globalsymbollistToFunc_g->symbollist.at(i)->content == name) {
			if (((LevelAndOffsetAndValue*)(globalsymbollistToFunc_g->symbollist.at(i)->addr))->value != "I don't know till now") {
				return ((LevelAndOffsetAndValue*)(globalsymbollistToFunc_g->symbollist.at(i)->addr))->value;
			}
			else {
				return "no";
			}
		}
	}
	return "no";
}

void saveTemp(string funcname, string name, string type, string value) {//保存临时变量类型和值
	for (unsigned i = 0; i < symbollist_main_g.size(); i++) {
		if (funcname == symbollist_main_g.at(i)->func) {
			MainSheet* tempMainSheet;
			
			tempMainSheet = new MainSheet;
			tempMainSheet->content = name;
			tempMainSheet->category = "vt";
			tempMainSheet->flag = 1;
				writeTypeSheet(type);
				tempMainSheet->type = typeSheet_g;
				typeSheet_g = NULL;
				writeLevelAndOffsetAndValue(((LevelAndOffsetAndValue*)(symbollist_main_g.at(i)->symbollist.at(0)->addr))->level, symbollist_main_g.at(i)->offset_max);
				if (type == "char") {
					symbollist_main_g.at(i)->offset_max += 1;
				}
				else if (type == "int") {
					symbollist_main_g.at(i)->offset_max += 4;
				}
				else if (type == "double") {
					symbollist_main_g.at(i)->offset_max += 8;
				}
				else if (type == "float") {
					symbollist_main_g.at(i)->offset_max += 4;
				}
				levelAndOffsetAndValue_g->value = value;
				tempMainSheet->addr = levelAndOffsetAndValue_g;
				levelAndOffsetAndValue_g = NULL;
				symbollist_main_g.at(i)->symbollist.push_back(tempMainSheet);
				tempMainSheet = NULL;
				return;
			
		}
	}

	if (symbollistToFunc_g != NULL && symbollistToFunc_g->func == funcname) {
		MainSheet* tempMainSheet;
		
			tempMainSheet = new MainSheet;
			tempMainSheet->content = name;
			tempMainSheet->category = "vt";
			tempMainSheet->flag = 1;
			writeTypeSheet(type);
			tempMainSheet->type = typeSheet_g;
			typeSheet_g = NULL;
			writeLevelAndOffsetAndValue(((LevelAndOffsetAndValue*)(symbollistToFunc_g->symbollist.at(0)->addr))->level, symbollistToFunc_g->offset_max);
			if (type == "char") {
				symbollistToFunc_g->offset_max += 1;
			}
			else if (type == "int") {
				symbollistToFunc_g->offset_max += 4;
			}
			else if (type == "double") {
				symbollistToFunc_g->offset_max += 8;
			}
			else if (type == "float") {
				symbollistToFunc_g->offset_max += 4;
			}
			levelAndOffsetAndValue_g->value = value;
			tempMainSheet->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			symbollistToFunc_g->symbollist.push_back(tempMainSheet);
			tempMainSheet = NULL;
		
	}

}
void saveAdmin(string funcname, string name, string value) {//保存用户的非全局变量的类型和值
	for (unsigned i = 0; i < symbollist_main_g.size(); i++) {
		if (funcname == symbollist_main_g.at(i)->func) {
			for (unsigned j = 0; j < symbollist_main_g.at(i)->symbollist.size(); j++) {
				if (name == symbollist_main_g.at(i)->symbollist.at(j)->content) {
					((LevelAndOffsetAndValue*)symbollist_main_g.at(i)->symbollist.at(j)->addr)->value = value;
					return;
				}
			}
		}
	}
	for (unsigned i = 0; i < symbollistToFunc_g->symbollist.size(); i++) {
		if (name == symbollistToFunc_g->symbollist.at(i)->content) {
			((LevelAndOffsetAndValue*)(symbollistToFunc_g->symbollist.at(i))->addr)->value = value;
		}
	}
}
void saveGlobal(string name, string value) {			//保存用户定义的全局变量
	unsigned i = 0;
	for (i = 0; i < globalsymbollistToFunc_g->symbollist.size(); i++) {
		if (name == globalsymbollistToFunc_g->symbollist.at(i)->content) {
			((LevelAndOffsetAndValue*)(globalsymbollistToFunc_g->symbollist.at(i)->addr))->value = value;
			break;
		}
	}
	if (i == globalsymbollistToFunc_g->symbollist.size()) {
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
		globalsymbollistToFunc_g->symbollist.push_back(mainSheet_g);
		mainSheet_g = NULL;
	}
	else {
		showErrowWhenCreateASheet("MainSheet");
	}
	
}

void writeGlobalToFile(string fileName) {
	ofstream ofs(fileName,ios::app);
	for (unsigned i = 0; i < globalsymbollistToFunc_g->symbollist.size(); i++) {
		writeMainSheetToFile(fileName, globalsymbollistToFunc_g->symbollist.at(i));
	}
	ofs.close();
}
void writesymbollistMain_gToFile(string fileName) {
	ofstream ofs(fileName,ios::app);
	for (unsigned int i = 0; i < symbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < symbollist_main_g.at(i)->symbollist.size(); j++) {
			writeMainSheetToFile(fileName, symbollist_main_g.at(i)->symbollist.at(j));
		}
	}
	ofs.close();
}


void writeMainSheetToFile(string funcName, MainSheet* mainSheet) {
	ofstream ofs(funcName,ios::app);
	ofs << "MainSheet表内容: " << endl;
	ofs << "标识符名字: " << mainSheet->content << endl;
	ofs << "标识符类型: " << endl; printTypeSheet(mainSheet->type);
	ofs << "标识符种类: " << mainSheet->category << endl;
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
	ofstream ofs(funcName,ios::app);
	ofs << "----" << "TypeSheet表内容: " << endl;
	ofs << "    " << "typeValue: " << typeSheet->typevalue << endl;
	//因为目前没有数组和结构体所以typeSheet的指针项为空
	ofs << endl;
	ofs.close();
}
void writeLevelAndOffsetAndValueToFile(string funcName, LevelAndOffsetAndValue* levelAndOffsetAndValue) {
	ofstream ofs(funcName,ios::app);
	ofs << "----" << "LevelAndOffsetAndValue表内容: " << endl;
	ofs << "    " << "level:  " << levelAndOffsetAndValue->level << endl;
	ofs << "    " << "offset: " << levelAndOffsetAndValue->offset << endl;
	ofs << "    " << "vlaue:  " << levelAndOffsetAndValue->value << endl;
	ofs << endl;
	ofs.close();
}
void writeFuncSheetToFile(string funcName, FuncSheet* funcsheet) {
	ofstream ofs(funcName,ios::app);
	ofs << "----" << "函数表内容: " << endl;
	ofs << "    " << "函数层次: " << funcsheet->level << endl;
	ofs << "    " << "形参个数: " << funcsheet->fnum << endl;
	ofs << "    " << "入口地址: " << funcsheet->entry << endl;
	ofs << "    " << "参数表: " << endl;
	for (unsigned i = 0; i < funcsheet->para->size(); i++) {
		writeParaSheetToFile(funcName, funcsheet->para->at(i));
	}
	ofs << endl;
	ofs.close();
}
void writeParaSheetToFile(string funcName, ParaSheet* paraSheet) {
	ofstream ofs(funcName,ios::app);
	ofs << "----" << "Para表内容: " << endl;
	ofs << "    " << "形参名: " << paraSheet->content << endl;
	ofs << "    " << "形参类型: "; writeTypeSheetToFile(funcName, paraSheet->type);
	ofs << "    " << "形参表的LevelAndOffsetAndValue表: "; writeLevelAndOffsetAndValueToFile(funcName, paraSheet->addr);
	ofs << endl;
	ofs.close();
}