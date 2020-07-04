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