#include "���ű�.h"
#include "�﷨����.h"
#include<fstream>

void checkSynbollist_main(string str) {		//���ط����ܱ������Ƿ����ض��壩
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (str == synbollist_main_g.at(i)->func) {
			string s = str + "�ض���";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			Error(token_g.row, token_g.content, s);
		}
	}
}
void checkSynbollistToFunc(string funcId, string str) {	//���غ������ű���ֲ������Ƿ��ض��壩
	for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
		if (str == synbollistToFunc_g->synbollist.at(i)->content) {
			string s = str + "�ض���";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			Error(token_g.row, token_g.content, s);
		}
	}
}

void checkParas(string str) {				//�����βα�(���β��Ƿ��ض���)
	for (unsigned i = 0; i < paras_g->size(); i++) {
		if (str == paras_g->at(i)->content) {
			string s = str + "�ض���";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			Error(token_g.row, token_g.content, s);
		}
	}
}

void checkGlobal(string str) {				//����ȫ�ֱ���
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (str == globalsynbollistToFunc_g->synbollist.at(i)->content) {
			string s = "ȫ�ֱ���" + str + "�ض���";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			Error(token_g.row, token_g.content, s);
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




//Ϊ��ʵ����6������������һЩģ�黯����......

void writeTypeSheet(string type) {		//дһ�����ͱ�
	if (typeSheet_g == NULL) {
		typeSheet_g = new TypeSheet;
		typeSheet_g->typevalue = type;
		typeSheet_g->tpoint = NULL;
	}
	else {
		showErrowWhenCreateASheet("TypeSheet");
	}
}

void writeLevelAndOffsetAndValue(int level, int offset) {	//дһ�ű����ű����� level offset vlaue
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
	string s = "�ڴ���һ��" + str + "����ʱ���ֳ�ͻ";
	clearAll_g();
	clearLists();
	clearGlobalVariable();
	Error(token_g.row, token_g.content, s);
}

/*�߼��������*/
void printGlobal() {											//���ȫ�ֱ���
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		printMainSheet(globalsynbollistToFunc_g->synbollist.at(i));
	}
}
void printSynbollistMain_g() {								//��������ܱ����������
	for (unsigned int i = 0; i < synbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
			printMainSheet(synbollist_main_g.at(i)->synbollist.at(j));
		}
	}
}
/*�ײ��������*/



/*�߼�������*/
void clearLists() {			//�ͷ����ô�����е�ָ��
	//synbollist_main_g
	for (unsigned int i = 0; i < synbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
			deleteMainSheet(synbollist_main_g.at(i)->synbollist.at(j));
		}
		delete synbollist_main_g.at(i);
	}
}

void clearAll_g() {			//�ͷ�����ȫ��ָ��Ͷ����е�ָ��,ֻ���ڳ���ʱ�����
	delete synbollistToFunc_g;			   //��������Ӧ�ķ��ű��ָ��
	delete mainSheet_g;					   //���ű���ָ��
	delete typeSheet_g;					   //���ͱ�ָ��
	delete funcSheet_g;					   //������ָ��
	delete paras_g;						   //������ָ��
	delete para_g;						   //��������ָ��
	delete levelAndOffsetAndValue_g;	   //LevelAndOffset��ָ��
	//�ͷŶ���ָ��
	while (!tempMainSheetpointers.empty()) {
		delete tempMainSheetpointers.front();
		tempMainSheetpointers.pop();
	}
}

void clearGlobalVariable() {	//�ͷ�ȫ�ֱ�������globalsynbollistToFunc_gָ��
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		deleteMainSheet(globalsynbollistToFunc_g->synbollist.at(i));
	}
}
/*�߼�������*/



/*�ײ��������*/
void printMainSheet(MainSheet* mainSheet) {				//���MainSheet��	
	cout << "MainSheet������: " << endl;
	cout << "��ʶ������: " << mainSheet->content << endl;
	cout << "��ʶ������: " << endl; printTypeSheet(mainSheet->type);
	cout << "��ʶ������: " << mainSheet->category << endl;
	if (mainSheet->flag == 0) {
		printfFuncSheet((FuncSheet*)mainSheet->addr);
	}
	else if (mainSheet->flag == 1) {
		printLevelAndOffsetAndValue((LevelAndOffsetAndValue*)mainSheet->addr);
	}
}
void printTypeSheet(TypeSheet* typeSheet) {					//���TypeSheet��
	cout << "----" << "TypeSheet������: " << endl;
	cout << "    " << "typeValue: " << typeSheet->typevalue << endl;
	//��ΪĿǰû������ͽṹ������typeSheet��ָ����Ϊ��
	cout << endl;
}
void printLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue) {	//���LevelAndOffset��
	cout << "----" << "LevelAndOffset������: " << endl;
	cout << "    " << "level:  " << levelAndOffsetAndValue->level << endl;
	cout << "    " << "offset: " << levelAndOffsetAndValue->offset << endl;
	cout << "    " << "vlaue:  " << levelAndOffsetAndValue->value << endl;
}
void printfFuncSheet(FuncSheet* funcsheet) {				//���������
	cout << "----" << "����������: " << endl;
	cout << "    " << "�������: " << funcsheet->level << endl;
	cout << "    " << "�βθ���: " << funcsheet->fnum << endl;
	cout << "    " << "��ڵ�ַ: " << funcsheet->entry << endl;
	cout << "    " << "������: " << endl;
	for (unsigned i = 0; i < funcsheet->para->size(); i++) {
		printParaSheet(funcsheet->para->at(i));
	}
	cout << endl;
}
void printParaSheet(ParaSheet* paraSheet) {					//����βα�
	cout << "----" << "Para������: " << endl;
	cout << "    " << "�β���: " << paraSheet->content << endl;
	cout << "    " << "�β�����: "; printTypeSheet(paraSheet->type);
	cout << "    " << "�βα��LevelAndOffsetAndValue��: "; printLevelAndOffsetAndValue(paraSheet->addr);
	cout << endl;
}
/*�ײ��������*/



/*�ײ�������*/
void deleteMainSheet(MainSheet* mainSheet_p) {	//��ȫ����һ��MainSheet����
	if (mainSheet_p->flag == 0) {
		deleteFuncSheet((FuncSheet*)mainSheet_p->addr);
	}
	else if (mainSheet_p->flag == 1) {
		deleteLevelAndOffsetAndValue((LevelAndOffsetAndValue*)mainSheet_p->addr);
	}
	deleteTypeSheet(mainSheet_p->type);
	delete mainSheet_p;
	//cout << "�ɹ��ͷ���һ��MainSheet����" << endl;
}
void deleteTypeSheet(TypeSheet* typeSheet_p) {	//��ȫ����һ��TypeSheet����
	if (typeSheet_p == NULL)return;
	delete typeSheet_p;
	//cout << "�ɹ��ͷ���һ��TypeSheet����" << endl;
}
void deleteLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue_p) {	//��ȫ����һ��levelAndOffset����
	if (levelAndOffsetAndValue_p == NULL)return;
	delete levelAndOffsetAndValue_p;
	//cout << "�ɹ��ͷ���һ��LevelAndOffsetAndValue����" << endl;
}
void deleteFuncSheet(FuncSheet* funcSheet_p) {   //��ȫ����һ��FuncSheet����
	if (funcSheet_p == NULL) {
		return;
	}
	for (unsigned i = 0; i < funcSheet_p->para->size(); i++) {
		deletePara(funcSheet_p->para->at(i));
	}
	delete funcSheet_p;
	//cout << "�ɹ��ͷ���һ��FuncSheet����" << endl;
}
void deletePara(ParaSheet* para_p) {				//��ȫ����һ��ParaSheet����
	deleteLevelAndOffsetAndValue(para_p->addr);
	deleteTypeSheet(para_p->type);
	delete para_p;
	//cout << "�ɹ��ͷ���һ��Para����" << endl;
}
/*�ײ�������*/


/*����Ҫʵ�ֵ�7������*/
//û�鵽����no
string searchType(string funcname, string name) {//���ȫ���û��������ʱ��������
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

string searchType(string name) {//��ȫ���û������������
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (globalsynbollistToFunc_g->synbollist.at(i)->content == name) {
			return globalsynbollistToFunc_g->synbollist.at(i)->type->typevalue;
		}
	}
	return "no";
}
string searchValue(string funcname, string name) {//���ȫ�ֱ�������ʱ����ֵ
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
string searchValue(string name) {//��ȫ���û��������ֵ
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
void saveTemp(string funcname, string name, string type, string value) {//������ʱ�������ͺ�ֵ
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
void saveAdmin(string funcname, string name, string value) {//�����û��ķ�ȫ�ֱ��������ͺ�ֵ
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
void saveGlobal(string name, string value) {			//�����û������ȫ�ֱ���
	unsigned i = 0;
	for (i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (name == globalsynbollistToFunc_g->synbollist.at(i)->content) {
			((LevelAndOffsetAndValue*)(globalsynbollistToFunc_g->synbollist.at(i)->addr))->value = value;
			break;
		}
	}
	if (i == globalsynbollistToFunc_g->synbollist.size()) {
		cout << "û�鵽Ҫ��ı���" << endl;
	}
}

void saveGlobal(string name, string type, string value) {			//����ȫ����ʱ����

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
	ofs << "MainSheet������: " << endl;
	ofs << "��ʶ������: " << mainSheet->content << endl;
	ofs << "��ʶ������: " << endl; printTypeSheet(mainSheet->type);
	ofs << "��ʶ������: " << mainSheet->category << endl;
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
	ofs <<  "TypeSheet������: " << endl;
	ofs << "typeValue: " << typeSheet->typevalue << endl;
	ofs << endl;
	ofs.close();
}
void writeLevelAndOffsetAndValueToFile(string funcName, LevelAndOffsetAndValue* levelAndOffsetAndValue) {
	ofstream ofs(funcName, ios::app);
	//ofs << "----" << "LevelAndOffsetAndValue������: " << endl;
	ofs << "level:  " << levelAndOffsetAndValue->level << endl;
	ofs <<"offset: " << levelAndOffsetAndValue->offset << endl;
	ofs <<"vlaue:  " << levelAndOffsetAndValue->value << endl;
	ofs << endl;
	ofs.close();
}
void writeFuncSheetToFile(string funcName, FuncSheet* funcsheet) {
	ofstream ofs(funcName, ios::app);
	ofs << "����������: " << endl;
	ofs << "�������: " << funcsheet->level << endl;
	ofs <<  "�βθ���: " << funcsheet->fnum << endl;
	//ofs << "    " << "��ڵ�ַ: " << funcsheet->entry << endl;
	//ofs << "    " << "������: " << endl;
	for (unsigned i = 0; i < funcsheet->para->size(); i++) {
		writeParaSheetToFile(funcName, funcsheet->para->at(i));
	}
	ofs << endl;
	ofs.close();
}
void writeParaSheetToFile(string funcName, ParaSheet* paraSheet) {
	ofstream ofs(funcName, ios::app);
	ofs <<  "������: " << endl;
	ofs <<  "�β���: " << paraSheet->content << endl;
    writeTypeSheetToFile(funcName, paraSheet->type);
	//ofs << "    " << "�βα��LevelAndOffsetAndValue��: "; writeLevelAndOffsetAndValueToFile(funcName, paraSheet->addr);
	ofs << endl;
	ofs.close();
}

/*����Ҫʵ�ֵ�7������*/
string TypeOfPara(string funcName, int pos) {
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (synbollist_main_g.at(i)->func == funcName) {
			return synbollist_main_g.at(i)->synbollist.at(pos)->type->typevalue;
		}
	}
	return "no";
}

//ͨ���������������βθ���
int NumOfPara(string funcName) {
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (synbollist_main_g.at(i)->func == funcName) {
			return ((FuncSheet*)(synbollist_main_g.at(i)->synbollist.at(0)->addr))->fnum;
		}
	}
	return -1;
}
