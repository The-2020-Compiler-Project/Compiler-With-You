#include "�﷨����.h"
#include <algorithm>

int rr = 1;//��ʱ��������
int sem = 0;

string type_1;//����������
string type_2;

string value_1; //������ֵ
string value_2;

vector <token> L; //��Ծ��Ϣ
vector <token> LL;//��������

token left_n;
token right_n;
token res_n;
vector<QT>qt;

int func_num = 0;
mystack<token>SEM;

//����mystack�еĳ�Ա����
template<class T>
bool mystack<T>::Empty()
{
	if (top != -1)
	{
		return mys[top];
	}
	else
	{
		cout << "ջ��\n";
		exit(1);
	}
}
template<class T>
T  mystack<T>::TOP()
{
	if (top != -1)
	{
		return mys[top];
	}
	else
	{
		cout << "ջ��a\n";
		exit(1);
	}
}
template<class T>
T  mystack<T>::SEC()
{
	if (top != -1)
	{
		return mys[top - 1];
	}
	else
	{
		cout << "ջ��ֻ��һ��Ԫ��\n";
		exit(1);
	}
}
template<class T>
void mystack<T>::PUSH(T tp)
{
	if (top + 1 < maxsize)
	{
		mys[++top] = tp;
	}
	else
	{
		cout << "ջ��\n";
		exit(1);
	}
}
template<class T>
void mystack<T>::POP()
{
	if (top >= 0) top--;
	else
	{
		cout << "ջ��b\n";
		exit(1);
	}
}
template<class T>
int mystack<T>::SIZE()
{
	return top + 1;
}


void WriteinFileL(string name, string content) //д���ļ�
{
	ofstream myfile;
	myfile.open(name);
	myfile << content << endl;
	vector<token>::iterator iff;
	for (iff = L.begin(); iff != L.end(); iff++)
	{
		myfile << (*iff).name << "\t" << (*iff).type << endl;
	}
	myfile.close();
}

void PUSHSEM(token operand)
{//ѹջ���ѵ�ǰtoken�ṹ�������ַ�����SEM�У���ID��NUM���ս���������
	SEM.PUSH(operand);
	sem++;
}

bool CompareType()
{//�Ƚϴ�ջ��Ԫ������type_1��ջ��Ԫ������type_2�Ƿ���ͬ
	type_1 = searchType(func_name, SEM.SEC().name);
	left_n.name = SEM.SEC().name + func_name;
	if (type_1 == "no")
	{
		type_1 = searchType(SEM.SEC().name);
		left_n.name = SEM.SEC().name;
		if (type_1 == "no")
		{
			type_1 = searchType(func_name, to_string(stringToNum<int>(SEM.SEC().name)));
			left_n.name = SEM.SEC().name;
			if (type_1 == "no")
			{
				type_1 = searchType(to_string(stringToNum<int>(SEM.SEC().name)));
				left_n.name = SEM.SEC().name;
				if (type_1 == "no")
				{
					type_1 = searchType(SEM.SEC().name, SEM.SEC().name);
					left_n.name = SEM.SEC().name + to_string(func_num++);
					if (type_1 == "no")
					{
						Error(token_g.row, token_g.content, "��ʶ��δ����");
						return false;
					}
				}
			}
		}
	}

	type_2 = searchType(func_name, SEM.TOP().name);
	right_n.name = SEM.TOP().name + func_name;
	if (type_2 == "no")
	{
		type_2 = searchType(SEM.TOP().name);
		right_n.name = SEM.TOP().name;
		if (type_2 == "no")
		{
			type_2 = searchType(func_name, to_string(stringToNum<int>(SEM.TOP().name)));
			right_n.name = SEM.TOP().name;
			if (type_2 == "no")
			{
				type_2 = searchType(to_string(stringToNum<int>(SEM.TOP().name)));
				right_n.name = SEM.TOP().name;
				if (type_2 == "no")
				{
					type_2 = searchType(SEM.TOP().name, SEM.TOP().name);
					right_n.name = SEM.TOP().name + to_string(func_num);
					if (type_2 == "no")
					{
						Error(token_g.row, token_g.content, "��ʶ��δ����");
						return false;
					}
				}
			}
		}
	}
	if (type_1 != "no" && type_2 != "no" && type_1 == type_2)
		return true;
	else
		return false;
}

string resvalue(string opeart)
{//��ջ��Ԫ��value_1��ջ��Ԫ��value_2�����߼����㣬������Ԫʽ������߼�����ֵ
	string type = "no";

	if (type == "no")
	{
		type = searchType(func_name, SEM.TOP().name);
		if (type == "no")
		{
			type = searchType(SEM.TOP().name);
			if (type == "no")
			{
				type = searchType(func_name, to_string(stringToNum<int>(SEM.TOP().name)));
				if (type == "no")
				{
					type = searchType(to_string(stringToNum<int>(SEM.TOP().name)));
					if (type == "no")
					{
						type = searchType(SEM.TOP().name, SEM.TOP().name);
						if (type == "no")
						{
							Error(token_g.row, token_g.content, "��ʶ��δ����");
							return "0";
						}
					}
				}
			}
		}
	}

	value_1 = searchValue(func_name, SEM.SEC().name);
	if (value_1 == "no")
	{
		value_1 = searchValue(SEM.SEC().name);
		if (value_1 == "no")
		{
			value_1 = searchValue(func_name, to_string(stringToNum<int>(SEM.SEC().name)));
			if (value_1 == "no")
			{
				value_1 = searchValue(to_string(stringToNum<int>(SEM.SEC().name)));
				if (value_1 == "no")
				{
					value_1 = searchValue(SEM.SEC().name, SEM.SEC().name);
					if (value_1 == "no")
					{
						Error(token_g.row, token_g.content, "��ʶ��δ��ֵ");
					}
				}
			}
		}
	}

	value_2 = searchValue(func_name, SEM.TOP().name);
	if (value_2 == "no")
	{
		value_2 = searchValue(SEM.TOP().name);
		if (value_2 == "no")
		{
			value_2 = searchValue(func_name, to_string(stringToNum<int>(SEM.TOP().name)));
			if (value_2 == "no")
			{
				value_2 = searchValue(to_string(stringToNum<int>(SEM.TOP().name)));
				if (value_2 == "no")
				{
					value_2 = searchValue(SEM.TOP().name, SEM.TOP().name);
					if (value_2 == "no")
					{
						Error(token_g.row, token_g.content, "��ʶ��δ��ֵ");
					}
				}
			}
		}
	}

	string res_empty;//��û��ƥ������ͣ��򷵻ؿմ�
	if (type == "int")
	{
		int Val_1 = stringToNum<int>(value_1);
		int Val_2 = stringToNum<int>(value_2);
		int res;

		if (opeart == "&&")
			res = Val_1 && Val_2;
		if (opeart == "||")
			res = Val_1 || Val_2;
		if (opeart == "!=")
			res = Val_1 != Val_2;
		if (opeart == "==")
			res = Val_1 == Val_2;
		if (opeart == ">")
			res = Val_1 > Val_2;
		if (opeart == ">= ")
			res = Val_1 >= Val_2;
		if (opeart == "<")
			res = Val_1 < Val_2;
		if (opeart == "<=")
			res = Val_1 && Val_2;
		if (opeart == "+")
			res = Val_1 + Val_2;
		if (opeart == "-")
			res = Val_1 - Val_2;
		if (opeart == "*")
			res = Val_1 * Val_2;
		if (opeart == "/")
			res = Val_1 / Val_2;
		return to_string(res);
	}
	else if (type == "double")
	{
		double Val_1 = stringToNum<double>(value_1);
		double Val_2 = stringToNum<double>(value_2);
		double res;
		if (opeart == "&&")
			res = Val_1 && Val_2;
		if (opeart == "||")
			res = Val_1 || Val_2;
		if (opeart == "!=")
			res = Val_1 != Val_2;
		if (opeart == "==")
			res = Val_1 == Val_2;
		if (opeart == ">")
			res = Val_1 > Val_2;
		if (opeart == ">= ")
			res = Val_1 >= Val_2;
		if (opeart == "<")
			res = Val_1 < Val_2;
		if (opeart == "<=")
			res = Val_1 && Val_2;
		if (opeart == "+")
			res = Val_1 + Val_2;
		if (opeart == "-")
			res = Val_1 - Val_2;
		if (opeart == "*")
			res = Val_1 * Val_2;
		if (opeart == "/")
			res = Val_1 / Val_2;
		return to_string(res);
	}
	else if (type == "float")
	{
		float Val_1 = stringToNum<float>(value_1);
		float Val_2 = stringToNum<float>(value_2);
		float res;
		if (opeart == "&&")
			res = Val_1 && Val_2;
		if (opeart == "||")
			res = Val_1 || Val_2;
		if (opeart == "!=")
			res = Val_1 != Val_2;
		if (opeart == "==")
			res = Val_1 == Val_2;
		if (opeart == ">")
			res = Val_1 > Val_2;
		if (opeart == ">= ")
			res = Val_1 >= Val_2;
		if (opeart == "<")
			res = Val_1 < Val_2;
		if (opeart == "<=")
			res = Val_1 && Val_2;
		if (opeart == "+")
			res = Val_1 + Val_2;
		if (opeart == "-")
			res = Val_1 - Val_2;
		if (opeart == "*")
			res = Val_1 * Val_2;
		if (opeart == "/")
			res = Val_1 / Val_2;
		return to_string(res);
	}
	else if (type == "char")
	{
		char Val_1 = stringToNum<char>(value_1);
		char Val_2 = stringToNum<char>(value_2);
		char res;
		if (opeart == "&&")
			res = Val_1 && Val_2;
		if (opeart == "||")
			res = Val_1 || Val_2;
		if (opeart == "!=")
			res = Val_1 != Val_2;
		if (opeart == "==")
			res = Val_1 == Val_2;
		if (opeart == ">")
			res = Val_1 > Val_2;
		if (opeart == ">= ")
			res = Val_1 >= Val_2;
		if (opeart == "<")
			res = Val_1 < Val_2;
		if (opeart == "<=")
			res = Val_1 && Val_2;
		if (opeart == "+")
			res = Val_1 + Val_2;
		if (opeart == "-")
			res = Val_1 - Val_2;
		if (opeart == "*")
			res = Val_1 * Val_2;
		if (opeart == "/")
			res = Val_1 / Val_2;
		return to_string(res);
	}
	return res_empty;
}

void GEQ(string operat)/*������Ԫʽ�����������&&��||����=��==��>��>=��<��<=��+��-��*��/
						SEM��ջ���ʹ�ջ��Ϊ��һ�͵ڶ�������
						resΪ�Զ����ɵ���ʱ����
						��ջ���ʹ�ջ����գ����ɵ���ʱ��������*/
{//�������Ԫʽ����ʽ
	{
		char t = 't';
		string ress = 't' + to_string(rr++);//��ʱ�����������
		//T_num.begin(), T_num.end()Ϊ�����������[T_num.begin(), T_num.end()) ����ָ���ú����Ĳ��ҷ�Χ��ressΪҪ���ҵ�Ŀ��Ԫ��
		//�Ȳ���ĳ����ʱ�����Ƿ����
		vector<string>::iterator result = find(T_num.begin(), T_num.end(), ress);

		while (result != T_num.end()) //û�鵽�������µ���ʱ����
		{
			string ress = 't' + to_string(rr++);//��ʱ����������ɣ�rrΪȫ�ֱ�����¼��ʱ��������
			vector<string>::iterator result = find(T_num.begin(), T_num.end(), ress);
		}

		left_n.type = SEM.SEC().type;//�������Ϊ��ջ��Ԫ��
		left_n.active = false;
		right_n.type = SEM.TOP().type;//�Ҳ�����Ϊջ��Ԫ��
		right_n.active = false;

		token Res;//��ʼ��һ�������ݴ���Ϣ����ʱ����
		Res.name = ress;
		Res.type = 3;//type=3��ʾ��ʱ����
		Res.active = false;

		res_n.type = 3;//д����Ԫʽ�Ľ��
		if (func_name != "no")//�жϺ����еı���orȫ�ֱ���
			res_n.name = ress + func_name;
		else
			res_n.name = ress;

		res_n.active = false;

		if (CompareType() == true)
		{
			string value = resvalue(operat);//������ʱ����ֵ��������Ϊstring����
			if (func_name == "no")
			{
				saveGlobal(Res.name, type_1, value);//����ȫ����ʱ�������
			}
			else
			{
				saveTemp(func_name, Res.name, type_1, value);//������ʱ�������
			}
			qt.push_back({ operat, left_n , right_n , res_n });//ѹ����Ԫʽ��

			//����ջ����������������
			SEM.POP();
			SEM.POP();

			PUSHSEM(Res);
		}
		else
		{
			Error(token_g.row, token_g.content, "���Ͳ�ƥ��");
		}
	}
}
void ASSI()
{//��ֵ��Ԫʽ���ɣ������=  ��һ������ΪSEMջ�����ڶ�������Ϊ�� ���resΪ��ջ����ջ���ʹ�ջ����ջ
	token null_point;//�ղ�������
	null_point.name = "";
	null_point.type = 0;

	int flag = 0;//��־����û�и�ֵ����Ԫʽ
	//��ʼ����Ԫʽ
	left_n.type = SEM.SEC().type;
	left_n.active = false;
	right_n.type = SEM.TOP().type;
	right_n.active = false;

	if (SEM.TOP().type == 1)//�Ⱥ��Ҳ��ǳ��� 
	{
		if (func_name != "no") //�����û������ȫ�ֱ���
		{
			if (CompareType() == true)
			{
				if (type_1 == "int")
				{//��ջ��Ϊ���ͳ�������Ҫ��to_string()ת������Ϊ�ַ�������ѯջ��Ԫ��
					if (searchValue(func_name, to_string(stringToNum<int>(SEM.TOP().name))) != "no")
					{//ջ��Ԫ��Ϊ��ȫ�ֱ�������ʱ����ֵ����ջ��Ԫ��ֵ��ֵ����ջ��Ԫ��
						saveAdmin(func_name, SEM.SEC().name, searchValue(func_name, to_string(stringToNum<int>(SEM.TOP().name))));
					}
					else
					{//ջ��Ԫ��Ϊȫ�ֱ���ֵ
						saveAdmin(func_name, SEM.SEC().name, searchValue(to_string(stringToNum<int>(SEM.TOP().name))));
					}
				}
				else
				{//�������ͳ���ֱ�Ӳ�ѯname������ת��������������һ��
					if (searchValue(func_name, SEM.TOP().name) != "no")
					{
						saveAdmin(func_name, SEM.SEC().name, searchValue(func_name, SEM.TOP().name));
					}
					else
					{
						saveAdmin(func_name, SEM.SEC().name, searchValue(SEM.TOP().name));
					}
				}
			}
			else
				Error(token_g.row, token_g.content, "���Ͳ�ƥ��");
		}
		else if (func_name == "no") //�Ⱥ��Ҳ����û������ȫ�ֱ���
		{
			if (CompareType() == true)
			{
				if (type_1 == "int")//��ֵ������Ϊ�����û������ȫ�ֱ�������
					saveGlobal(SEM.SEC().name, searchValue(to_string(stringToNum<int>(SEM.TOP().name))));
				else
					saveGlobal(SEM.SEC().name, searchValue(SEM.TOP().name));
			}
			else
				Error(token_g.row, token_g.content, "���Ͳ�ƥ��");
		}
	}

	else if (SEM.TOP().type == 3 || SEM.TOP().type == 2)//�Ⱥ��Ҳ��Ǳ��� 
	{
		if (func_name != "no") //�����û������ȫ�ֱ���
		{
			if (CompareType() == true)
			{
				if (searchValue(func_name, SEM.TOP().name) == "no")
				{//��ջ��Ԫ�ط�ȫ�ֺ���ʱ����ֵ
					if (searchValue(SEM.TOP().name) == "no")
					{//��ջ��Ԫ��ȫ�ֱ���ֵ
						if (searchValue(SEM.TOP().name, SEM.TOP().name) == "no")
						{
							flag = 1;
							Error(token_g.row, token_g.content, "����δ��ֵ");
						}
						else
							saveAdmin(func_name, SEM.SEC().name, searchValue(SEM.TOP().name, SEM.TOP().name));
					}
					else
						saveAdmin(func_name, SEM.SEC().name, searchValue(SEM.TOP().name, SEM.TOP().name));
				}
				else
					saveAdmin(func_name, SEM.SEC().name, searchValue(func_name, SEM.TOP().name));
			}
			else
				Error(token_g.row, token_g.content, "���Ͳ�ƥ��");
		}
		else if (func_name == "no") //���û������ȫ�ֱ���
		{
			if (CompareType() == true)
			{
				if (searchValue(func_name, SEM.TOP().name) == "no")
				{
					if (searchValue(SEM.TOP().name) == "no")
					{
						if (searchValue(SEM.TOP().name, SEM.TOP().name) == "no")
						{
							flag = 1;
							Error(token_g.row, token_g.content, "����δ��ֵ");
						}
						else
							saveGlobal(SEM.SEC().name, searchValue(SEM.TOP().name, SEM.TOP().name));
					}
					else
						saveGlobal(SEM.SEC().name, searchValue(SEM.TOP().name));
				}
				else
					saveGlobal(SEM.SEC().name, searchValue(func_name, SEM.TOP().name));
			}
			else
				Error(token_g.row, token_g.content, "���Ͳ�ƥ��");
		}
	}

	if (flag == 0)
	{
		qt.push_back({ "=", right_n, null_point , left_n });
	}

	//�������Ԫ�غ�ָ��ջ��Ԫ��
	SEM.POP();
	SEM.POP();
}

void IF()/*if�����Ԫʽ���ɣ������if   ��һ�������Ϊջ��Ԫ��SEM[sem],�ڶ��������ͽ��Ϊ��
		 ջ��Ԫ�ص�ջ*/
{
	token null_point;
	null_point.name = "";
	null_point.type = 0;

	qt.push_back({ "if", right_n, null_point, null_point });

	SEM.POP();

}
void EL()/*else�����Ԫʽ���ɣ������el ����Ϊ�� SEM�޲���*/
{
	token null_point;
	null_point.name = "";
	null_point.type = 0;

	qt.push_back({ "el", null_point , null_point , null_point });

}
void IE()/*������Ԫʽ�������ie ����Ϊ�� SEM�޲���*/
{
	token null_point;
	null_point.name = "";
	null_point.type = 0;

	qt.push_back({ "ie",null_point , null_point, null_point });

}
void WH()/*whileѭ��ͷ������wh ����Ϊ�� SEM�޲���*/
{
	token null_point;
	null_point.name = "";
	null_point.type = 0;

	qt.push_back({ "wh", null_point , null_point , null_point });

}
void DO()/*do������do ��һ������Ϊջ��Ԫ�� �ڶ�Ϊ�� ���Ϊ��
		 ջ��Ԫ�ص���*/
{
	token null_point;
	null_point.name = "";
	null_point.type = 0;

	qt.push_back({ "do", right_n, null_point , null_point });

	SEM.POP();
}
void WE()/*ѭ��β��we ������ SEM�޲���*/
{
	token null_point;
	null_point.name = "";
	null_point.type = 0;

	qt.push_back({ "we", null_point ,null_point, null_point });

}

void RET()//RETURN��䣺��һ������
{
	token null_point;
	null_point.name = "";
	null_point.type = 0;

	if (SEM.TOP().type == 3)
	{
		right_n.active = false;
		right_n.name = SEM.TOP().name + func_name;
		right_n.type = 3;
	}
	else if (SEM.TOP().type == 2)
	{
		right_n.active = false;
		right_n.name = SEM.TOP().name + func_name;
		right_n.type = 2;
	}
	else if (SEM.TOP().type == 1)
	{
		right_n.active = false;
		right_n.name = SEM.TOP().name;
		right_n.type = 1;
	}

	qt.push_back({ "RET",right_n ,null_point,null_point });
}

void WriteQt(string name, string content)
{
	ofstream myfile;
	myfile.open(name);
	myfile << content << endl;
	vector<QT>::iterator iff;
	for (iff = qt.begin(); iff != qt.end(); iff++)
	{

		myfile << (*iff).Operat << "\t" << (*iff).opA.name << "\t" << (*iff).opB.name << "\t" << (*iff).res.name << "\t" << (*iff).block << endl;
	}
	myfile.close();
}