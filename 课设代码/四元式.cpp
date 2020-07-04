#include<vector>
#include<string>

int rr = 1;//记录临时变量个数
int sem = 0;
int func_num;

string type_1;//操作数类型
string type_2;

string value_1; //操作数的值
string value_2;

//存入四元式的相关量
token left_n;
token right_n;
token res_n;
vector<QT>qt;
mystack<token> SEM;


#include <sstream>
//模板函数：将string类型变量转换为常用的数值类型（字符串的空格作为分界点，完成字符串中各类成分的拆分）
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;//返回常数
}

struct token 
{
	string name;
	int type;// 1常量，2非临时变量，3是临时变量
	bool active;//活跃信息
	token() { name = ""; type = 0; active = false; }
	token(string namet, int typet, bool activet) { name = namet; type = typet; active = activet; }
	void clear() { name = ""; type = 0; active = false; }
};

struct QT
{
	string Operat;//操作符
	token opA;
	token opB;
	token res;
	int block;//基本块
	QT() { Operat = ""; opA.clear(); opB.clear(); res.clear(); }
	QT(string Operatt, token opAt, token opBt, token rest) { Operat = Operatt; opA = opAt; opB = opBt; res = rest; }
	void clear() { Operat = ""; opA.clear(); opB.clear(); res.clear(); }
};

template<class T>
class mystack
{
	int top;
	T* mys;
	int maxsize;
public:
	mystack() :top(-1), maxsize(111)
	{
		mys = new T[maxsize];
		if (mys == NULL)
		{
			cerr << "动态存储分配失败！" << endl;
			exit(1);
		}
	}
	mystack(int size) :top(-1), maxsize(size)
	{
		mys = new T[maxsize];
		if (mys == NULL)
		{
			cerr << "动态存储分配失败！" << endl;
			exit(1);
		}
	}
	~mystack()
	{
		delete[] mys;
	}
	bool Empty();//是否为空
	void PUSH(T tp); //压栈
	T TOP();//返回栈顶元素
	T SEC();//返回次栈顶元素
	void POP();//出栈
	int SIZE();//栈大小
};

template<class T>
bool mystack<T>::Empty()
{
	if (top != -1)
		return mys[top];
	else
	{
		cout << "栈空\n";
		exit(1);
	}
}
template<class T>
T  mystack<T>::TOP()
{
	if (top != -1)
		return mys[top];
	else
	{
		cout << "栈空a\n";
		exit(1);
	}
}
template<class T>
T  mystack<T>::SEC()
{
	if (top != -1)
		return mys[top - 1];
	else
	{
		cout << "栈中只有一个元素\n";
		exit(1);
	}
}
template<class T>
void mystack<T>::PUSH(T tp)
{
	if (top + 1 < maxsize)
		mys[++top] = tp;
	else
	{
		cout << "栈满\n";
		exit(1);
	}
}
template<class T>
void mystack<T>::POP()
{
	if (top >= 0) 
		top--;
	else
	{
		cout << "栈空b\n";
		exit(1);
	}
}
template<class T>
int mystack<T>::SIZE()
{
	return top + 1;
}


void PUSHSEM(token operand)//压栈：把当前字符放入SEM中，在ID和NUM后调用
{
	SEM.PUSH(operand);
	sem++;
}

bool CompareType() 
{//比较次栈顶元素类型type_1和栈顶元素类型type_2是否相同
	type_1 = searchType(func_name, SEM.SEC().name);
	left_n.name = SEM.SEC().name + func_name;
	if (type_1 == "no") {
		type_1 = searchType(SEM.SEC().name);
		left_n.name = SEM.SEC().name;
		if (type_1 == "no") {
			type_1 = searchType(func_name, to_string(stringToNum<int>(SEM.SEC().name)));
			left_n.name = SEM.SEC().name;
			if (type_1 == "no") {
				type_1 = searchType(to_string(stringToNum<int>(SEM.SEC().name)));
				left_n.name = SEM.SEC().name;
				if (type_1 == "no") {
					type_1 = searchType(SEM.SEC().name, SEM.SEC().name);
					left_n.name = SEM.SEC().name + to_string(func_num++);
					if (type_1 == "no") {
						Error(token.row, token.content, "标识符未定义");
						return false;
					}
				}
			}
		}
	}

	type_2 = searchType(func_name, SEM.TOP().name);
	right_n.name = SEM.TOP().name + func_name;
	if (type_2 == "no") {
		type_2 = searchType(SEM.TOP().name);
		right_n.name = SEM.TOP().name;
		if (type_2 == "no") {
			type_2 = searchType(func_name, to_string(stringToNum<int>(SEM.TOP().name)));
			right_n.name = SEM.TOP().name;
			if (type_2 == "no") {
				type_2 = searchType(to_string(stringToNum<int>(SEM.TOP().name)));
				right_n.name = SEM.TOP().name;
				if (type_2 == "no") {
					type_2 = searchType(SEM.TOP().name, SEM.TOP().name);
					right_n.name = SEM.TOP().name + to_string(func_num);
					if (type_2 == "no") {
						Error(token.row, token.content, "标识符未定义");
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
{//次栈顶元素value_1和栈顶元素value_2进行逻辑运算，返回四元式结果的逻辑运算值
	string type = "no";

	if (type == "no") {
		type = searchType(func_name, SEM.TOP().name);
		if (type == "no") {
			type = searchType(SEM.TOP().name);
			if (type == "no") {
				type = searchType(func_name, to_string(stringToNum<int>(SEM.TOP().name)));
				if (type == "no") {
					type = searchType(to_string(stringToNum<int>(SEM.TOP().name)));
					if (type == "no") {
						type = searchType(SEM.TOP().name, SEM.TOP().name);
						if (type == "no") {
							Error(token.row, token.content, "标识符未定义");
							return false;
						}
					}
				}
			}
		}
	}

	value_1 = searchValue(func_name, SEM.SEC().name);
	if (value_1 == "no") {
		value_1 = searchValue(SEM.SEC().name);
		if (value_1 == "no") {
			value_1 = searchValue(func_name, to_string(stringToNum<int>(SEM.SEC().name)));
			if (value_1 == "no") {
				value_1 = searchValue(to_string(stringToNum<int>(SEM.SEC().name)));
				if (value_1 == "no") {
					value_1 = searchValue(SEM.SEC().name, SEM.SEC().name);
					if (value_1 == "no") {
						Error(token.row, token.content, "标识符未赋值");
					}
				}
			}
		}
	}

	value_2 = searchValue(func_name, SEM.TOP().name);
	if (value_2 == "no") {
		value_2 = searchValue(SEM.TOP().name);
		if (value_2 == "no") {
			value_2 = searchValue(func_name, to_string(stringToNum<int>(SEM.TOP().name)));
			if (value_2 == "no") {
				value_2 = searchValue(to_string(stringToNum<int>(SEM.TOP().name)));
				if (value_2 == "no") {
					value_2 = searchValue(SEM.TOP().name, SEM.TOP().name);
					if (value_2 == "no") {
						Error(token.row, token.content, "标识符未赋值");
					}
				}
			}
		}
	}

	string res_empty;//若没有匹配的类型，则返回空串
	if (type == "int") {

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
	else if (type == "double") {
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
	else if (type == "float") {
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
	else if (type == "char") {
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


void GEQ(string operat)
{//运算符四元式产生式
	{
		char t = 't';
		string ress = 't' + to_string(rr++);//临时结果变量生成
		//T_num.begin(), T_num.end()为输入迭代器，[T_num.begin(), T_num.end()) 用于指定该函数的查找范围；ress为要查找的目标元素
		//先查找某个临时变量是否产生
		vector<string>::iterator result = find(T_num.begin(), T_num.end(), ress);

		while (result != T_num.end()) //没查到则生成新的临时变量
		{
			string ress = 't' + to_string(rr++);//临时结果变量生成，rr为全局变量记录临时变量个数
			vector<string>::iterator result = find(T_num.begin(), T_num.end(), ress);
		}

		left_n.type = SEM.SEC().type;//左操作数为次栈顶元素
		left_n.active = false;
		right_n.type = SEM.TOP().type;//右操作数为栈顶元素
		right_n.active = false;

		token Res;//初始化一个用于暂存信息的临时变量
		Res.name = ress;
		Res.type = 3;//type=3表示临时变量
		Res.active = false;

		res_n.type = 3;//写入四元式的临时变量
		if (func_name != "no")//判断函数中的变量or全局变量
			res_n.name = ress + func_name;
		else
			res_n.name = ress;

		res_n.active = false;

		if (CompareType() == true) {
			string value = resvalue(operat);//计算临时变量值，并保存为string类型

			if (func_name == "no") {
				saveGlobal(Res.name, type_1, value);//保存全局临时变量结果
			}
			else {
				saveTemp(func_name, Res.name, type_1, value);//保存临时变量结果
			}
			qt.push_back({ operat, left_n , right_n , res_n });//压入四元式区

			//弹出栈顶的两个操作对象
			SEM.POP();
			SEM.POP();

			PUSHSEM(Res);
		}
		else {
			Error(token.row, token.content, "类型不匹配");
		}
	}
}

void ASSI()
{//赋值四元式生成：运算符=  第一操作数为SEM栈顶，第二操作数为空 结果res为次栈顶，栈顶和次栈顶弹栈
	token null_point;//空操作对象
	null_point.name = "";
	null_point.type = 0;

	int flag = 0;//标志变量没有赋值的四元式
	//初始化四元式
	left_n.type = SEM.SEC().type;
	left_n.active = false;
	right_n.type = SEM.TOP().type;
	right_n.active = false;

	if (SEM.TOP().type == 1)//等号右侧是常量 
	{
		if (func_name != "no") //不是用户定义的全局变量
		{
			if (CompareType() == true)
			{
				if (type_1 == "int")
				{//次栈顶为整型常量，需要用to_string()转换常数为字符串，查询栈顶元素
					if (searchValue(func_name, to_string(stringToNum<int>(SEM.TOP().name))) != "no")
					{//栈顶元素为非全局变量和临时变量值，将栈顶元素值赋值给次栈顶元素
						saveAdmin(func_name, SEM.SEC().name, searchValue(func_name, to_string(stringToNum<int>(SEM.TOP().name))));
					}
					else
					{//栈顶元素为全局变量值
						saveAdmin(func_name, SEM.SEC().name, searchValue(to_string(stringToNum<int>(SEM.TOP().name))));
					}
				}
				else
				{//其他类型常量直接查询name，无需转换，与上述操作一致
					if (searchValue(func_name, SEM.TOP().name) != "no") 、
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
				Error(token.row, token.content, "类型不匹配");
		}
		else if (func_name == "no") //等号右侧是用户定义的全局变量
		{
			if (CompareType() == true)
			{
				if (type_1 == "int")//赋值函数变为保存用户定义的全局变量函数
					saveGlobal(SEM.SEC().name, searchValue(to_string(stringToNum<int>(SEM.TOP().name))));
				else
					saveGlobal(SEM.SEC().name, searchValue(SEM.TOP().name));
			}
			else
				Error(token.row, token.content, "类型不匹配");
		}
	}

	else if (SEM.TOP().type == 3 || SEM.TOP().type == 2)//等号右侧是变量 
	{
		if (func_name != "no") //不是用户定义的全局变量
		{
			if (CompareType() == true)
			{
				if (searchValue(func_name, SEM.TOP().name) == "no")
				{//查栈顶元素非全局和临时变量值
					if (searchValue(SEM.TOP().name) == "no")
					{//查栈顶元素全局变量值
						if (searchValue(SEM.TOP().name, SEM.TOP().name) == "no")
						{//
							flag = 1;
							Error(token.row, token.content, "变量未赋值");
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
				Error(token.row, token.content, "类型不匹配");
		}
		else if (func_name == "no") //是用户定义的全局变量
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
							Error(token.row, token.content, "变量未赋值");
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
				Error(token.row, token.content, "类型不匹配");
		}
	}

	if (flag == 1) {
		qt.push_back({ "=", right_n, null_point , left_n });
	}
	else {
		qt.push_back({ "=", right_n, null_point , left_n });
	}

	//清空两个元素后指向栈顶元素
	SEM.POP();
	SEM.POP();
}
