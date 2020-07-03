#include "stdio.h"
#include "语法分析.h"
#include "符号表.h"

Lexer scan;
Token token;

vector<string> T_num;//中间变量t动态数组
string id;//保存读到的单词
string type;//保存读到的单词类型

string id_qt;//四元式标识符
//sting num_qt;//四元式数组位置

void Error(int row, string content, string error)
{//错误分析
	cout << error << endl;
	cout << "错误行数：" << row << " 错误内容：" << content << endl;
	exit(1);//异常退出
}


int Recursive()
{//递归子程序分析法
	scan.init();//初始化
	token = scan.next_Token();//读一个单词
	Declaration_list();//声明列表
	if (token.content == "$")
	{
		cout << "语法分析成功" << endl;
	}
	else
	{
		cout << "语法分析出错" <<token.content << endl;
	}
	return 0;
}


int Variable_Type()
{//变量类型
	if (token.content == "int" || token.content == "float" || token.content == "double" || token.content == "char")
	{
		type = token.content;//保存读到的类型
		token = scan.next_Token();
		return 1;
	}
	else
		return 0;
}

int ID()
{//标识符
	if (token.type == 'i')
	{
		id = token.content;//保存读到的单词
		id_qt = token.content;//四元式
		if (token.content[0] == 't')//生成中间变量
			T_num.push_back(token.content);
		token = scan.next_Token();
		return 1;
	}
	else
		return 0;
}

int NumCharId()
{//初始化常量声明里的变量
	if (token.type == 'n' || token.type == 'f')//整数或浮点数
	{
		token = scan.next_Token();
		return 1;
	}
	else if (token.type == 'c')//字符
	{
		token = scan.next_Token();
		return 1;
	}
	else if (ID() == 1)//标识符
		return 1;
	else
		return 0;
}

//声明部分
int Variable_Type()
{//变量声明
	if (Variable_Type() == 1)
	{
		if (ID() == 1)
		{
			if (token.content == ";")
				return 1;
			else
				Error(token.row, token.content, "缺少;");
		}
		else
			Error(token.row, token.content, "变量声明错误");
	}
	else
		return 0;
}


int Const_Declaration()
{//常量声明
	if (token.content == "const")
	{
		token = scan.next_Token();
		if (Variable_Type() == 1)
		{
			if (ID() == 1)
			{
				if (token.content == "=")
				{
					token = scan.next_Token();
					if (NumCharID() == 1)
					{
						if (token.content == ";")
						{
							token = scan.next_Token;
							return 1;
						}
						else
							Error(token.row, token.content, "缺少;");
					}
					else
						Error(token.row, token.content, "常量初始化错误");
				}
				else
					Error(token.row, token.content, "常量声明错误");
			}
			else
				Error(token.row, token.content, "常量声明错误");
		}
		else
			Error(token.row, token.content, "常量声明错误");
	}
	else
		return 0;
}


int Array_Declaration()
{//数组声明

}

int Function_Declaration()
{//函数声明

}

int Declaration()
{
	if (Const_Declaration() == 1)
		return 1;
	else if (Array_Declaration() == 1)
		return 1;
	else if (Function_Declaration() == 1)
		return 1;
	else
		return 0;
}

int Declaration_list_1()
{
	if (Declaration() == 1)
	{
		if (Declaration_list_1() == 1)
				return 1;
		else
			Error(token.row,token.content,"错误！");
	}	
	else//空
		return 1;
}

int Declaration_list()
{
	if (Declaration() == 1)
	{
		if (Declaration_list_1() == 1)
			return 1;
		else
			Error(token.row,token.content,"错误！");
	}
	else
		return 0;
}

//算术表达式

int Factor()
{//因子部分尚未完善（涉及数组）

}

int MD_Expression_1()
{
	if (token.content == '*')
	{
		token == scan.next_Tkoen();
		if (Factor() == 1)
		{
			GEQ("*");//四元式生成函数
			if (MD_Expression_1 == 1)
				return 1;
			else
				Error(token.row, token.content, "乘法表达式错误！");
		}
		else
			Error(token.row, token.content, "乘除法表达式错误！");
	}
	if (token.content == '/')
	{
		token == scan.next_Tkoen();
		if (Factor() == 1)
		{
			GEQ("/");//四元式生成函数
			if (MD_Expression_1 == 1)
				return 1;
			else
				Error(token.row, token.content, "除发法表达式错误！");
		}
		else
			Error(token.row, token.content, "乘除法表达式错误！");
	}
	else
		return 1;
}

int MD_Expression()
{
	if (Factor() == 1)
	{
		if (MD_Expression_1() == 1)
			return 1;
		else
			Error(token.row, token.content, "加减法表达式错误！");
	}
	else
		return 0;
}

int Arithmetic_Expression_1()
{
	if (token.content == '+')
	{
		token == scan.next_Tkoen();
		if (MD_Expression() == 1)
		{
			GEQ("+");//四元式生成函数
			if (Arithmetic_Expression_1 == 1)
				return 1;
			else
				Error(token.row, token.content, "加法表达式错误！");
		}	
		else
			Error(token.row, token.content, "加减法表达式错误！");
	}
	if (token.content == '-')
	{
		token == scan.next_Tkoen();
		if (MD_Expression() == 1)
		{
			GEQ("-");//四元式生成函数
			if (Arithmetic_Expression_1 == 1)
				return 1;
			else
				Error(token.row, token.content, "减法表达式错误！");
		}
		else
			Error(token.row, token.content, "加减法表达式错误！");
	}
	else//空
		return 1;
}

int Arithmetic_Expression()
{
	if (MD_Expression() == 1)
	{
		if (Arithmetic_Expression_1() == 1)
			return 1;
		else
			Error(token.row, token.content, "错误！");
	}
	else
		return 0;
}

//逻辑表达式
int Arithmetic_Expression_2()
{
	if (token.content == ">")
	{
		token = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ(">");//四元式产生式
			return 1;
		}
		else
			Error(token.row, token.content, "逻辑比较表达式错误！");
	}
	if (token.content == ">=")
	{
		token = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ(">=");//四元式产生式
			return 1;
		}
		else
			Error(token.row, token.content, "逻辑比较表达式错误！");
	}
	if (token.content == "<")
	{
		token = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("<");//四元式产生式
			return 1;
		}
		else
			Error(token.row, token.content, "逻辑比较表达式错误！");
	}
	if (token.content == "<=")
	{
		token = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("<=");//四元式产生式
			return 1;
		}
		else
			Error(token.row, token.content, "逻辑比较表达式错误！");
	}
	else
		return 1;
}

int Equality()
{
	if (Arithmetic_Expression() == 1)
	{
		if (Arithmetic_Expression_2() == 1)
			return 1;
		else
			Error(token.row, token.content, "逻辑表达式错误！");
	}
	else
		return 0;
}

int Equality_2()
{
	if (token.content == "!=")
	{
		token = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("!=");//四元式产生式
			return 1;
		}
		else
			Error(token.row, token.content, "逻辑表达式错误！");
	}
	if (token.content == "==")
	{
		token = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("==");//四元式产生式
			return 1;
		}
		else
			Error(token.row, token.content, "逻辑表达式错误！");
	}
	else
		return 1;
}
int Logical_Expression()
{
	if (Equality() == 1)
	{
		if (Equality_2() == 1)
			return 1;
		else
			Error(token.row, token.content, "逻辑表达式错误！");
	}
	else
		return 0;
}

int Other_Logical()
{
	if (token.content == "&&")
	{
		token = scan.next_Token();
		if (Logical_Expression() == 1)
		{
			GEQ("&&");//四元式产生式
			if (Other_Logical() == 1)
				return 1;
		}
		else
			Error(token.row, token.content, "逻辑表达式错误！");
	}
	if (token.content == "||")
	{
		token = scan.next_Token();
		if (Logical_Expression() == 1)
		{
			GEQ("||");//四元式产生式
			if (Other_Logical() == 1)
				return 1;
		}
		else
			Error(token.row, token.content, "逻辑表达式错误！");
	}
	else
		return 1;
}
//赋值语句
int Assignment()
{
	if (Logical_Expression() == 1)
	{
		if (Other_Logical() == 1)
			return 1;
		else
			Error(token.row, token.content, "赋值语句错误！");
	}
	else
		return 0;
}

int Assignment_1()
{
	if (token.content == "=")
	{
		token = scan.next_Token();
		if (Assignment() == 1)
		{
			ASSI();//赋值四元式产生式
			return 1;
		}
		else
			Error(token.row, token.content, "赋值语句错误！");
	}
	else
		return 1;
}

int Expression()
{
	if (Assiginment() == 1)
	{
		if (Assignment_1() == 1)
			return 1;
		else
			Error(token.row, token.content, "赋值语句错误！");
	}
	else
		return 0;
}
