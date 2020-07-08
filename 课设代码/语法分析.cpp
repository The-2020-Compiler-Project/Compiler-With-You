#include "stdio.h"
#include "语法分析.h"
#include "符号表.h"

Lexer scan;
Token token_g;

vector<string> T_num;//中间变量t动态数组
string id;_g//保存读到的单词
string type_g;//保存读到的单词类型

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
	token_g = scan.next_Token();//读一个单词
	ZeroOrMoreDeclation();//声明列表
	if (token_g.content == "$")
	{
		cout << "语法分析成功" << endl;
	}
	else
	{
		cout << "语法分析出错" <<token_g.content << endl;
	}
	return 0;
}


int Variable_Type()
{//变量类型
	if (token_g.content == "int" || token_g.content == "float" || token_g.content == "double" || token_g.content == "char")
	{
		type_g = token_g.content;//保存读到的类型
		token_g = scan.next_Token();
		return 1;
	}
	else
		return 0;
}

int ID()
{//标识符
	if (token_g.type == 'i')
	{
		id_g = token_g.content;//保存读到的单词
		id_qt = token_g.content;//四元式
		if (token_g.content[0] == 't')//生成中间变量
			T_num.push_back(token_g.content);
		token_g = scan.next_Token();
		return 1;
	}
	else
		return 0;
}

int NumCharId()
{//初始化常量
	if (token_g.type == 'n' || token_g.type == 'f')//整数或浮点数
	{
		token_g = scan.next_Token();
		return 1;
	}
	else if (token_g.type == 'c')//字符
	{
		token_g = scan.next_Token();
		return 1;
	}
	else if (ID() == 1)//标识符
		return 1;
	else
		return 0;
}

//声明部分
int ZeroOrMoreDeclation()
{
	if (Declaration() == 1)
	{
		if (ZeroOrMoreDeclation() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "错误！");
	}
	else//空
		return 1;
}

int Declaration()
{
	if (Const_Declaration() == 1)
		return 1;
	else if (Variable_Declaration() == 1)
		return 1;
	else
		return 0;
}

int Const_Declaration()
{//常量声明
	if (token_g.content == "const")
	{
		token_g = scan.next_Token();
		if (Variable_Type() == 1)
		{
			if (ID() == 1)
			{
				if (token_g.content == "=")
				{
					token_g = scan.next_Token();
					if (NumCharID() == 1)
					{
						if (token_g.content == ";")
						{
							token_g = scan.next_Token;
							return 1;
						}
						else
							Error(token_g.row, token_g.content, "缺少;");
					}
					else
						Error(token_g.row, token_g.content, "常量初始化错误");
				}
				else
					Error(token_g.row, token_g.content, "常量声明错误");
			}
			else
				Error(token_g.row, token_g.content, "常量声明错误");
		}
		else
			Error(token_g.row, token_g.content, "常量声明错误");
	}
	else
		return 0;
}
int Variable_Declaration()
{//其他变量声明
	if (Variable_Type() == 1)
	{
		if (ID() == 1)
		{
			if (Array_Declaration() == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "缺少;");
		}
		else
			Error(token_g.row, token_g.content, "变量声明错误");
	}
	else
		return 0;
}


int Array_Declaration()
{//数组及函数声明
	if (token_g.content == "[")
	{
		token_g = sacn.next_Token();
		if (token_g.type == 'n')
		{//如果[]中是整型常数
			int arrLength = stringToNum<int>(token_g.content);//生成数量等于arrLength的type_g型变量
			if (arrLength <= 0)
			{
				cout << "数组长度应该大于零" << endl;
				clearLists();//释放所有存入表中的指针
				clearGlobalVariable();//释放所有全局变量
				//clearGlobalVariable();//释放所有全局变量
				exit(1);
			}

			//首先判断数组声明是不是全局的
			if (synbollistToFunc_g != NULL)
			{//函数对应的符号表指针是局部的

				//插入一个占位符号
				if (mainSheet_g == NULL)
				{//新建符号表指针
					mainSheet_g = new MainSheet;
				}
				else
				{
					showErrowWhenCreateASheet("MainSheet");
				}
				//填入符号总表
				mainSheet_g->content = id_g;//标识符名
				writeLevelAndOffsetAndValue(-1, -1);//层次、偏移量初始化为-1
				mainSheet_g->addr = levelAndOffsetAndValue_g;//指向信息表
				levelAndOffsetAndValue_g = NULL;//初始化指针再利用

				writeTypeSheet(type_g);//填写类型表
				mainSheet_g->type = typeSheet_g;//根据类型表讲类型填到总表
				typeSheet_g = NULL;//初始化指针再利用

				synbollistToFunc_g->synbollist.push_back(mainSheet_g);//函数所对应的符号表的指针
				mainSheet_g = NULL;

				for (int i = 0; i < arrLength; i++)
				{//将数组元素依次填入
					string currentArrId = id_g + "[" + to_string(i) + "]";//要存入符号表的变量名，以符号串形式保存

					checkSynbollistToFunc(synbollistToFunc_g->func, currentArrId);//查函数符号表，检查是否重定义

					if (mainSheet_g == NULL)
					{//新建符号总表
						mainSheet_g = new MainSheet;
					}
					else
					{
						showErrowWhenCreateASheet("MainSheet");
					}
					mainSheet_g->content = currentArrId;//填名字
					mainSheet_g->category = "v";//种类是变量
					mainSheet_g->flag = 1;//判断地址指向表类型

					writeTypeSheet(type_g);
					mainSheet_g->type = typeSheet_g;
					typeSheet_g = NULL;

					writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);//填写层次？和偏移
					if (type_g == "int" || type_g == "float")
					{
						offset_g += 4;
					}
					else if (type_g == "char")
					{
						offset_g += 1;
					}
					else if (type_g == "double")
					{
						offset_g += 8;
					}
					else
					{
						cout << "出现未知类型" << type_g << ",其长度按照2个字节计算" << endl;
						offset_g += 2;
					}
					//至此一个LevelAndOffset表填完
					mainSheet_g->addr = levelAndOffsetAndValue_g;
					levelAndOffsetAndValue_g = NULL;
					synbollistToFunc_g->synbollist.push_back(mainSheet_g);
					mainSheet_g = NULL;
				}
			}
			else
			{//是全局的
				//插入一个占位符号
				if (mainSheet_g == NULL)
				{
					mainSheet_g = new MainSheet;
				}
				else
				{
					showErrowWhenCreateASheet("MainSheet");
				}
				//操作与非全局类似
				mainSheet_g->content = id_g;
				writeLevelAndOffsetAndValue(-1, -1);
				mainSheet_g->addr = levelAndOffsetAndValue_g;
				levelAndOffsetAndValue_g = NULL;

				writeTypeSheet(type_g);
				mainSheet_g->type = typeSheet_g;
				typeSheet_g = NULL;

				globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);//全局变量要要插入的指针global
				mainSheet_g = NULL;

				for (int i = 0; i < arrLength; i++) {
					string currentArrId = id_g + "[" + to_string(i) + "]";//要存入符号表的变量名

					checkGlobal(currentArrId);//查重全局变量

					if (mainSheet_g == NULL)
					{
						mainSheet_g = new MainSheet;
					}
					else
					{
						showErrowWhenCreateASheet("MainSheet");
					}
					mainSheet_g->content = currentArrId;
					mainSheet_g->category = "v";
					mainSheet_g->flag = 1;

					writeTypeSheet(type_g);
					mainSheet_g->type = typeSheet_g;
					typeSheet_g = NULL;

					writeLevelAndOffsetAndValue(0, global_offset_g);//全局变量填在第一层

					mainSheet_g->addr = levelAndOffsetAndValue_g;
					levelAndOffsetAndValue_g = NULL;
					globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
					mainSheet_g = NULL;

					if (type_g == "int" || type_g == "float")
					{
						global_offset_g += 4;//全局offet值
					}
					else if (type_g == "char")
					{
						global_offset_g += 1;
					}
					else if (type_g == "double")
					{
						global_offset_g += 8;
					}
					else {
						cout << "出现未知类型" << type_g << ",其长度按照2个字节计算" << endl;
						global_offset_g += 2;
					}
				}
			}

			token_g = scan.next_Token();
			if (token_g.content == "]")
			{
				token_g = scan.next_Token();
				if (EqualStringOrNumOrNull() == 1)
				{//数组声明后初始化
					if (token_g.content == ";") {
						token_g = scan.next_Token();
						return 1;
					}
					else {
						Error(token_g.row, token_g.content, "在应该是';'的地方不正确");
					}
				}
				else {
					Error(token_g.row, token_g.content, "数组或字符串不正确");
				}
			}
			else {
				Error(token_g.row, token_g.content, "变量或数组定义不正确");
			}
		}
		else {
			Error(token_g.row, token_g.content, "变量或数组定义不正确");
		}
	}

	else if (token_g.content == "=")
	{// 如果推导到了这里，说明在声明时对变量初始化。如：int a = 1;
		/*语义动作开始，这里是填符号表的时机*/
		token in;
		in = { id_qt,2,false };//标识符名，2表示非临时变量，活跃信息初始化为false
		PUSHSEM(in);//压入语义栈

		if (synbollistToFunc_g != NULL)
		{//说明声明的是局部变量，与前面的判断数组中每个元素的方法一致
			cout << type_g << " " << id_g << "是局部变量" << endl;
			/*这时对应的情况是：要填写char c
			  int f(int m,int n){
				  char c = 'a';
			  }
			 此时的全局指针情况：
			  synbollistToFunc_g 有值（即没有压入到synbollist_main_g）
			  其他所有全局指针为空
			*/

			//首先查重
			checkSynbollistToFunc(synbollistToFunc_g->func, id_g);	//是否重定义

			if (mainSheet_g == NULL)
			{
				mainSheet_g = new MainSheet;
			}
			else
			{
				showErrowWhenCreateASheet("MainSheet");
			}
			mainSheet_g->content = id_g;//填名字
			mainSheet_g->category = "v";//种类是变量
			mainSheet_g->flag = 1;

			writeTypeSheet(type_g);
			mainSheet_g->type = typeSheet_g;
			typeSheet_g = NULL;

			writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);//
			if (type_g == "int" || type_g == "float")
			{
				offset_g += 4;
			}
			else if (type_g == "char")
			{
				offset_g += 1;
			}
			else if (type_g == "double")
			{
				offset_g += 8;
			}
			else
			{
				cout << "出现未知类型" << type_g << ",其长度按照2个字节计算" << endl;
				offset_g += 2;
			}
			//至此一个LevelAndOffset表填完
			mainSheet_g->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			synbollistToFunc_g->synbollist.push_back(mainSheet_g);
			mainSheet_g = NULL;
		}
		else
		{//声明的是全局变量
			cout << type_g << " " << id_g << "是全局变量" << endl;
			/*这时对应的情况是：要填写char c
			  char c = 'a';
			  int f(int m,int n){
				  char b = 'a';
			  }
			 此时的全局指针情况：
			  全局指针均为空
			*/

			//首先查重
			checkGlobal(id_g);

			if (mainSheet_g == NULL)
			{
				mainSheet_g = new MainSheet;
			}
			else
			{
				showErrowWhenCreateASheet("MainSheet");
			}
			mainSheet_g->content = id_g;
			mainSheet_g->category = "v";
			mainSheet_g->flag = 1;

			writeTypeSheet(type_g);

			mainSheet_g->type = typeSheet_g;
			typeSheet_g = NULL;

			writeLevelAndOffsetAndValue(0, global_offset_g);

			mainSheet_g->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
			mainSheet_g = NULL;
			if (type_g == "int" || type_g == "float")
			{
				global_offset_g += 4;
			}
			else if (type_g == "char")
			{
				global_offset_g += 1;
			}
			else if (type_g == "double")
			{
				global_offset_g += 8;
			}
			else
			{
				cout << "出现未知类型" << type_g << ",其长度按照2个字节计算" << endl;
				global_offset_g += 2;
			}
		}
		/*语义动作结束*/
		token_g = scan.next_Token();
		if (ExpressionStatement() == 1)
		{
			ASSI();//赋值四元式
			return 1;
		}
		else
		{
			Error(token_g.row, token_g.content, "声明语句不正确");
		}
	}
	else if (token_g.content == "(")
	{//函数定义   int f (

		if (id_g == "main")
		{//主函数
			func_name = "main";
			MAIN();
		}
		else
		{
			token in;//普通函数定义
			in = { id_qt,2,false };
			PUSHSEM(in);
			func_name = id_qt;
			FUNC();
		}
		/*语义动作部分开始*/
		if (synbollistToFunc_g == NULL)
		{
			synbollistToFunc_g = new SynbollistToFunc;//创建一个指向SynbollistToFunc对象的指针
		}
		else
		{
			showErrowWhenCreateASheet("synbollistToFunc");
		}
		synbollistToFunc_g->func = id_g;//填这个对象的func域

		checkSynbollist_main(id_g);//查重，如果重定义了会结束程序

		if (mainSheet_g == NULL)
		{
			mainSheet_g = new MainSheet;//创建一个指向MainSheet对象的指针
		}
		else
		{
			showErrowWhenCreateASheet("MainSheet");
		}
		mainSheet_g->content = id_g;						//填标识符的名字
		mainSheet_g->category = "f";						//填标识符的种类
		mainSheet_g->flag = 0;								//填标识符的标志

		writeTypeSheet(type_g);//通过typeSheet_g新建一个类型表

		mainSheet_g->type = typeSheet_g;
		typeSheet_g = NULL;

		if (funcSheet_g == NULL)
		{
			funcSheet_g = new FuncSheet;//新建一个函数表
			if (type_g == "int")
			{
				funcSheet_g->value = "1";
			}
			else if (type_g == "float")
			{
				funcSheet_g->value = "1.0";
			}
			else if (type_g == "char")
			{
				funcSheet_g->value = "a";
			}
		}
		else
		{
			showErrowWhenCreateASheet("FuncSheet");
		}
		funcSheet_g->level = 1;//目前设置level为1 
		if (paras_g == NULL)
		{
			paras_g = new vector<ParaSheet*>; //创建一个指向形参表vector的指针
		}
		else
		{
			showErrowWhenCreateASheet("paras_g");
		}
		fnum_g = 0;	//设置形参个数为0
		offset_g = 0;//设置offset为0
		/*语义动作部分结束
		总结：
			目前
				synbollistToFunc_g还没有进入总vector
				mainSheet_g还有函数表没填写
				funcSheet_g还有形参个数，参数表，入口地址没写
			接下来要进入到 paraters
		*/

		/*记录函数名称*/
		func_name = id_g;
		/*记录函数名称*/

		token_g = scan.next_Token();
		if (Paraters() == 1) {
			if (token_g.content == ")") {		//函数形参表结束

				/*语义动作开始*/
				funcSheet_g->para = paras_g;	//将参数表填写到函数表
				paras_g = NULL;
				funcSheet_g->fnum = fnum_g;		//将参数个数写到函数表

				mainSheet_g->addr = funcSheet_g; //将函数表填写到MainSheet_g
				funcSheet_g = NULL;
				synbollistToFunc_g->synbollist.push_back(mainSheet_g);  //将MainSheet_g压入到synbollistToFunc_g->synbollist
				mainSheet_g = NULL;

				//接下来将tempMainSheetpointers中的指针压入到synbollistToFunc_g->synbollist
				while (!tempMainSheetpointers.empty()) {
					synbollistToFunc_g->synbollist.push_back(tempMainSheetpointers.front());
					tempMainSheetpointers.pop();
				}
				/*语义动作结束
				总结：
					目前，函数标识符已经填写
						  mianSheet_g为空
						  synbollistToFunc_g还没有压入总vector
				接下来：
					要进入函数体
				*/
				token_g = scan.next_Token();
				if (Block() == 1) {
					return 1;
				}
				else {
					Error(token_g.row, token_g.content, "函数声明不正确");
				}
			}
			else {
				Error(token_g.row, token_g.content, "函数声明不正确");
			}
		}
		else {
			Error(token_g.row, token_g.content, "函数声明不正确");
		}
	}
	else if (token_g.content == ";")
	{// 如果推导到了这里,说明是一个单纯的声明语句.如:int a;

		/*语义动作开始，这里是填符号表的时机*/
		if (synbollistToFunc_g != NULL) {													//说明声明的是局部变量
			cout << type_g << " " << id_g << "是局部变量" << endl;
			/*这时对应的情况是：要填写char c
			  int f(int m,int n){
				  char c = 'a';
			  }
			 此时的全局指针情况：
			  synbollistToFunc_g 有值（即没有压入到synbollist_main_g）
			  其他所有全局指针为空
			*/

			//首先查重
			checkSynbollistToFunc(synbollistToFunc_g->func, id_g);	//是否重定义

			if (mainSheet_g == NULL) {
				mainSheet_g = new MainSheet;
			}
			else {
				showErrowWhenCreateASheet("MainSheet");
			}
			mainSheet_g->content = id_g;		//填名字
			mainSheet_g->category = "v";		//种类是变量
			mainSheet_g->flag = 1;

			writeTypeSheet(type_g);
			mainSheet_g->type = typeSheet_g;
			typeSheet_g = NULL;

			writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);//
			if (type_g == "int" || type_g == "float")
			{
				offset_g += 4;
			}
			else if (type_g == "char")
			{
				offset_g += 1;
			}
			else if (type_g == "double")
			{
				offset_g += 8;
			}
			else
			{
				cout << "出现未知类型" << type_g << ",其长度按照2个字节计算" << endl;
				offset_g += 2;
			}
			//至此一个LevelAndOffset表填完
			mainSheet_g->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			synbollistToFunc_g->synbollist.push_back(mainSheet_g);
			mainSheet_g = NULL;
		}
		else
		{//声明的是全局变量
			cout << type_g << " " << id_g << "是全局变量" << endl;
			/*这时对应的情况是：要填写char c
			  char c = 'a';
			  int f(int m,int n){
				  char b = 'a';
			  }
			 此时的全局指针情况：
			  全局指针均为空
			*/

			//首先查重
			checkGlobal(id_g);

			if (mainSheet_g == NULL)
			{
				mainSheet_g = new MainSheet;
			}
			else
			{
				showErrowWhenCreateASheet("MainSheet");
			}
			mainSheet_g->content = id_g;
			mainSheet_g->category = "v";
			mainSheet_g->flag = 1;

			writeTypeSheet(type_g);

			mainSheet_g->type = typeSheet_g;
			typeSheet_g = NULL;

			writeLevelAndOffsetAndValue(0, global_offset_g);

			mainSheet_g->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
			mainSheet_g = NULL;
			if (type_g == "int" || type_g == "float")
			{
				global_offset_g += 4;
			}
			else if (type_g == "char")
			{
				global_offset_g += 1;
			}
			else if (type_g == "double")
			{
				global_offset_g += 8;
			}
			else
			{
				cout << "出现未知类型" << type_g << ",其长度按照2个字节计算" << endl;
				global_offset_g += 2;
			}
		}
		/*语义动作结束*/
		token_g = scan.next_Token();
		return 1;
	}
	else {
		return 0;
	}
}

int EqualStringOrNumOrNull()
{//声明数组后初始化or不初始化
	if (token_g.content == "=")
	{
		token_g = scan.next_Token();
		if (StringOrNum() == 1)
		{
			return 1;
		}
		else
		{
			Error(token_g.row, token_g.content, "在应该是字符串或数组的地方不正确");
		}
	}
	else {
		return 1;
	}
}

int StringOrNum()
{//数组初始化
	if (token_g.content == "{")
	{
		token_g = scan.next_Token();
		if (token_g.type == 'n' || token_g.type == 'f')
		{//整型或浮点型常数
			if (token_g.type == 'n')
			{//整型
				type_g = "int";
			}
			else
			{
				type_g = "float";
			}
			if (synbollistToFunc_g != NULL)
			{//函数定义
				if (!checkSynbollistToFuncNum(token_g.content))
				{
					if (mainSheet_g == NULL)
					{
						mainSheet_g = new MainSheet;
					}
					else
					{
						showErrowWhenCreateASheet("MainSheet");
					}
					if (type_g == "int")
					{
						mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
					}
					else
					{
						mainSheet_g->content = token_g.content;		//填名字
					}
					mainSheet_g->category = "v";		//种类是变量
					mainSheet_g->flag = 1;

					writeTypeSheet(type_g);
					mainSheet_g->type = typeSheet_g;
					typeSheet_g = NULL;

					writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);
					levelAndOffsetAndValue_g->value = token_g.content;//填活动记录里的值
					mainSheet_g->addr = levelAndOffsetAndValue_g;
					levelAndOffsetAndValue_g = NULL;
					synbollistToFunc_g->synbollist.push_back(mainSheet_g);
					mainSheet_g = NULL;
				}
			}
			else
			{//全局
				if (!checkGlobalNum(token_g.content))
				{
					if (mainSheet_g == NULL)
					{
						mainSheet_g = new MainSheet;
					}
					else
					{
						showErrowWhenCreateASheet("MainSheet");
					}
					if (type_g == "int")
					{
						mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
					}
					else
					{
						mainSheet_g->content = token_g.content;		//填名字
					}
					mainSheet_g->category = "v";
					mainSheet_g->flag = 1;

					writeTypeSheet(type_g);

					mainSheet_g->type = typeSheet_g;
					typeSheet_g = NULL;

					writeLevelAndOffsetAndValue(0, global_offset_g);
					levelAndOffsetAndValue_g->value = token_g.content;
					mainSheet_g->addr = levelAndOffsetAndValue_g;
					levelAndOffsetAndValue_g = NULL;
					globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
					mainSheet_g = NULL;
				}
			}

			/*数组生成四元式
			token in;
			in = { id_qt + "[" + to_string(arr_num_qt++) + "]",2,false };
			PUSHSEM(in);
			in = { token_g.content,1,false };
			PUSHSEM(in);
			ASSI();
			数组生成四元式*/

			token_g = scan.next_Token();
			while (token_g.content == ",")
			{
				token_g = scan.next_Token();
				if (token_g.type == 'n' || token_g.type == 'f')
				{
					if (token_g.type == 'n')
					{
						type_g = "int";
					}
					else
					{
						type_g = "float";
					}
					if (synbollistToFunc_g != NULL)
					{
						if (!checkSynbollistToFuncNum(token_g.content))
						{
							if (mainSheet_g == NULL)
							{
								mainSheet_g = new MainSheet;
							}
							else
							{
								showErrowWhenCreateASheet("MainSheet");
							}
							if (type_g == "int")
							{
								mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
							}
							else
							{
								mainSheet_g->content = token_g.content;//填名字
							}
							mainSheet_g->category = "v";//种类是变量
							mainSheet_g->flag = 1;

							writeTypeSheet(type_g);
							mainSheet_g->type = typeSheet_g;
							typeSheet_g = NULL;

							writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);//
							levelAndOffsetAndValue_g->value = token_g.content;
							mainSheet_g->addr = levelAndOffsetAndValue_g;
							levelAndOffsetAndValue_g = NULL;
							synbollistToFunc_g->synbollist.push_back(mainSheet_g);
							mainSheet_g = NULL;
						}
					}
					else {
						if (!checkGlobalNum(token_g.content)) {
							if (mainSheet_g == NULL) {
								mainSheet_g = new MainSheet;
							}
							else {
								showErrowWhenCreateASheet("MainSheet");
							}
							if (type_g == "int") {
								mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
							}
							else {
								mainSheet_g->content = token_g.content;		//填名字
							}
							mainSheet_g->category = "v";
							mainSheet_g->flag = 1;

							writeTypeSheet(type_g);

							mainSheet_g->type = typeSheet_g;
							typeSheet_g = NULL;

							writeLevelAndOffsetAndValue(0, global_offset_g);
							levelAndOffsetAndValue_g->value = token_g.content;
							mainSheet_g->addr = levelAndOffsetAndValue_g;
							levelAndOffsetAndValue_g = NULL;
							globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
							mainSheet_g = NULL;
						}
					}


					/*数组生成四元式
					token in;
					in = { id_qt + "[" + to_string(arr_num_qt++) + "]",2 ,false };
					PUSHSEM(in);
					in = { token_g.content,1 ,false };
					PUSHSEM(in);
					ASSI();
					/*数组生成四元式*/

					token_g = scan.next_Token();
				}
				else
				{
					Error(token_g.row, token_g.content, "变量或数组定义不正确");
				}
			}
			if (token_g.content == "}")
			{
				arr_num_qt = 0;//数组位置归零
				token_g = lexer_g.createAToken();
				return 1;
			}
			else
			{
				Error(token_g.row, token_g.content, "变量或数组定义不正确");
			}
		}
		else
		{
			Error(token_g.row, token_g.content, "变量或数组定义不正确");
		}
	}
	/*else if (token_g.type == 's')
	{//字符串
		int arr_length = token_g.content.length();
		for (int arr_i = 1; arr_i < arr_length - 1; arr_i++)
		{
			token in;
			in = { id_qt + "[" + to_string(arr_num_qt++) + "]", 2 ,false };
			PUSHSEM(in);
			in = { token_g.content.substr(arr_i, 1) ,3 ,false };
			PUSHSEM(in);
			ASSI();
		}
		token_g = scan.next_Token();
		arr_num_qt = 0;//数组位置归零
		return 1;
	}*/
	else
	 {
		return 0;
	}
}


int Statement()
{
	if (token_g.content == "printf")
	{	// statement --> printf'('expression')'';' 开始
		token_g = scan.next_Token();						//读入下一个token
		if (token_g.content == "(")
		{
			token_g = scan.next_Token();					//读入下一个token
			if (Expression() == 1)
			{
				if (token_g.content == ")")
				{
					token_g = scan.next_Token();			//读入下一个token
					if (token_g.content == ";")
					{
						token_g = scan.next_Token();		//读入下一个token
						return 1;
					}
					else
						Error(token_g.row, token_g.content, "在应该读入';'的地方出错");
				}
				else
					Error(token_g.row, token_g.content, "在应该读入')'的地方出错");
			}
			else
				Error(token_g.row, token_g.content, "printf语句表达式出错");
		}
		else
			Error(token_g.row, token_g.content, "在应该读入'('的地方出错");//行， 内容，错误
	}		// statement --> printf'('expression')'';'结束
	else if (Block() == 1)
	{	// 复合语句statement --> block
		return 1;
	}
	else if (token_g.content == "if")
	{	// statement --> IF '(' experssion ')' statement ElSE statement
		token_g = scan.next_Token();
		if (token_g.content == "(")
		{
			token_g = scan.next_Token();
			if (Expression() == 1)
			{
				if (token_g.content == ")")
				{
					IF();/*if语句四元式生成：运算符if   第一运算对象为栈顶元素SEM[sem],第二运算对象和结果为空
		 栈顶元素弹栈*/
					token_g = scan.next_Token();
					if (Statement() == 1)
					{
						if (token_g.content == "else")
						{
							EL();/*else语句四元式生成：运算符el 其他为空 SEM无操作*/
							token_g = scan.next_Token();
							if (Statement() == 1)
							{
								IE();/*结束四元式：运算符ie 其他为空 SEM无操作*/
								return 1;
							}
							else
								Error(token_g.row, token_g.content, "else体出错");
						}
						else
							Error(token_g.row, token_g.content, "if 语句没有else");
					}
					else
						Error(token_g.row, token_g.content, "if体出错");
				}
				else
					Error(token_g.row, token_g.content, "在应该读入')'的地方出错");
			}
			else
				Error(token_g.row, token_g.content, "if语句表达式出错");
		}
		else
			Error(token_g.row, token_g.content, "在应该读入'('的地方出错");
	}  // statement --> IF '(' experssion ')' statement ElSE statement 结束
	else if (token_g.content == "while")
	{  // statement --> WHILE '(' expression ')' statement
		WH();/*while循环头函数：wh 其他为空 SEM无操作*/
		token_g = scan.next_Token();
		if (token_g.content == "(")
		{
			scan.next_Token();
			if (Expression() == 1)
			{
				if (token_g.content == ")")
				{
					DO();/*do函数：do 第一操作数为栈顶元素 第二为空 结果为空
			 栈顶元素弹出*/
					token_g = scan.next_Token();
					if (Statement() == 1)
					{
						WE();/*循环尾：we 其他空 SEM无操作*/
						return 1;
					}
					else
						Error(token_g.row, token_g.content, "while体出错");
				}
				else
					Error(token_g.row, token_g.content, "在应该读入')'的地方出错");
			}
			else
				Error(token_g.row, token_g.content, "while语句表达式出错");
		}
		else
			Error(token_g.row, token_g.content, "在应该读入'('的地方出错");
	}  // statement --> WHILE '(' expression ')' statement 结束
	/*else if (token_g.content == "for")
	 {//FOR '(' varDeclaration @ expression ';' expression';' expression ')' statement
	//for语句相关四元式生成：FOR FORDO JUMPD0 DOS DOE FORE
		token_g = scan.next_Token();
		if (token_g.content == "(") {
			token_g = lexer_g.createAToken();
			if (ExpressionStatement() == 1) {
				FOR();//for循环开始标记
				if (Expression() == 1) {
					FORDO();//for循环，循环条件判断
					if (token_g.content == ";") {
						token_g = lexer_g.createAToken();
						if (Expression() == 1) {
							JUMPDO();//跳转回循环条件判断处
							if (token_g.content == ")") {
								token_g = lexer_g.createAToken();
								DOS();
								if (Statement() == 1) {
									DOE();//for循环执行块结尾
									FORE();//for循环结束位置
									return 1;
								}
								else {
									Error(token_g.row, token_g.content, "for语句体错误");
								}
							}
							else {
								Error(token_g.row, token_g.content, "在应该读入')'的地方出错");
							}
						}
						else {
							Error(token_g.row, token_g.content, "for语句出错");
						}
					}
					else {
						Error(token_g.row, token_g.content, "在应该读入';'的地方出错");
					}
				}
				else {
					Error(token_g.row, token_g.content, "for语句出错");
				}
			}
			else {
				Error(token_g.row, token_g.content, "for语句的第一个分号前缺少表达式或变量声明");
			}
		}
		else {
			Error(token_g.row, token_g.content, "在应该读入'('的地方出错");
		}
	}*/
	else if (ExpressionStatement() == 1)
	 {
		return 1;
	}
	else if (token_g.content == "return")
	 {
		token_g = scan.next_Token();
		if (ExpressionStatement() == 1) 
		{
			RET();
		}
		else
			Error(token_g.row, token_g.content, "return语句错误");
	}
	return 0;
}

int Block()
{//作用域：一个或多个
	if (token_g.content == "{")
	{
		token_g = scan.next_Token();
		if (ZeroOrMoreDeclation() == 1)
		{
			if (token_g.content == "}")
			{
				/*语义动作开始*/
				if (synbollistToFunc_g != NULL) {
					synbollistToFunc_g->offset_max = offset_g;			//判断是否建立了一个SynbollistToFunc对象										 
					synbollist_main_g.push_back(synbollistToFunc_g);     //如果建立了，将synbollistToFunc_g指针压入符号总表
					synbollistToFunc_g = NULL;								 //将synbollistToFunc_g设为空
				}
				/*语义动作结束
				总结：
					目前，synbollistToFunc_g为空
				*/
				token_g = scan.next_Token();
				return 1;
			}
			else 
			{
				Error(token_g.row, token_g.content, "应该是右括号的地方出错");
			}
		}
		else
		{
			Error(token_g.row, token_g.content, "出错");
		}
	}
	else 
	{
		return 0;
	}
}

int ExpressionStatement()
{//为数组赋值
	if (Expression() == 1)
	{
		if (token_g.content == ";")
		{
			token_g = scan.next_Token();
			return 1;
		}
		else
		{
			Error(token_g.row, token_g.content, "缺少分号");
		}
	}
	else
		return 0;
}

int Expression()
{
	if (Assignment() == 1)
	{
		if (Assignment_1() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "赋值语句错误！");
	}
	else
		return 0;
}



//赋值语句
int Assignment()
{
	if (Logical_Expression() == 1)
	{
		if (Other_Logical() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "赋值语句错误！");
	}
	else
		return 0;
}

int Assignment_1()
{
	if (token_g.content == "=")
	{
		token_g = scan.next_Token();
		if (Assignment() == 1)
		{
			ASSI();//赋值四元式产生式
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "赋值语句错误！");
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
			Error(token_g.row, token_g.content, "逻辑表达式错误！");
	}
	else
		return 0;
}

int Other_Logical()
{
	if (token_g.content == "&&")
	{
		token_g = scan.next_Token();
		if (Logical_Expression() == 1)
		{
			GEQ("&&");//四元式产生式
			if (Other_Logical() == 1)
				return 1;
		}
		else
			Error(token_g.row, token_g.content, "逻辑表达式错误！");
	}
	if (token_g.content == "||")
	{
		token_g = scan.next_Token();
		if (Logical_Expression() == 1)
		{
			GEQ("||");//四元式产生式
			if (Other_Logical() == 1)
				return 1;
		}
		else
			Error(token_g.row, token_g.content, "逻辑表达式错误！");
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
			Error(token_g.row, token_g.content, "逻辑表达式错误！");
	}
	else
		return 0;
}

int Equality_2()
{
	if (token_g.content == "!=")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("!=");//四元式产生式
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "逻辑表达式错误！");
	}
	if (token_g.content == "==")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("==");//四元式产生式
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "逻辑表达式错误！");
	}
	else
		return 1;
}
int Arithmetic_Expression()
{
	if (MD_Expression() == 1)
	{
		if (Arithmetic_Expression_1() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "错误！");
	}
	else
		return 0;
}

//逻辑表达式
int Arithmetic_Expression_2()
{
	if (token_g.content == ">")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ(">");//四元式产生式
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "逻辑比较表达式错误！");
	}
	if (token_g.content == ">=")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ(">=");//四元式产生式
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "逻辑比较表达式错误！");
	}
	if (token_g.content == "<")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("<");//四元式产生式
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "逻辑比较表达式错误！");
	}
	if (token_g.content == "<=")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("<=");//四元式产生式
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "逻辑比较表达式错误！");
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
			Error(token_g.row, token_g.content, "加减法表达式错误！");
	}
	else
		return 0;
}

int Arithmetic_Expression_1()
{
	if (token_g.content == '+')
	{
		token_g == scan.next_Tkoen();
		if (MD_Expression() == 1)
		{
			GEQ("+");//四元式生成函数
			if (Arithmetic_Expression_1 == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "加法表达式错误！");
		}
		else
			Error(token_g.row, token_g.content, "加减法表达式错误！");
	}
	if (token_g.content == '-')
	{
		token_g == scan.next_Tkoen();
		if (MD_Expression() == 1)
		{
			GEQ("-");//四元式生成函数
			if (Arithmetic_Expression_1 == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "减法表达式错误！");
		}
		else
			Error(token_g.row, token_g.content, "加减法表达式错误！");
	}
	else//空
		return 1;
}


int MD_Expression_1()
{
	if (token_g.content == '*')
	{
		token_g == scan.next_Tkoen();
		if (Factor() == 1)
		{
			GEQ("*");//四元式生成函数
			if (MD_Expression_1 == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "乘法表达式错误！");
		}
		else
			Error(token_g.row, token_g.content, "乘除法表达式错误！");
	}
	if (token_g.content == '/')
	{
		token_g == scan.next_Tkoen();
		if (Factor() == 1)
		{
			GEQ("/");//四元式生成函数
			if (MD_Expression_1 == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "除法表达式错误！");
		}
		else
			Error(token_g.row, token_g.content, "乘除法表达式错误！");
	}
	else
		return 1;
}

//算术表达式
/*int Factor()
{//因子
	if (token_g.content == "!") {
		token_g = scan.next_Token();
		if (Primary_2() == 1) {
			NOT();
			return 1;
		}
		else {
			Error(token_g.row, token_g.content, "!表达式错误");
		}
	}
	if (token_g.content == "-") {
		token_g = scan.next_Token();
		if (Primary_2() == 1) {
			Minus();
			return 1;
		}
		else {
			Error(token_g.row, token_g.content, "-表达式错误");
		}
	}
	else if (Primary_2() == 1)
		return 1;
	else
		return 0;
}*/

int Factor()
{//因子：表达式或变量
	if (token_g.content == "(")
	{
		token_g = scan.next_Token();
		if (Expression() == 1)
		{
			if (token_g.content == ")")
			{
				token_g = scan.next_Token();
				return 1;
			}
			else
				Error(token_g.row, token_g.content, "缺少右括号");
		}
		else
			Error(token_g.row, token_g.content, "括号表达式错误");
	}
	else if (Primary() == 1)
		return 1;
	else
		return 0;
}
}int Primary()
{//初始化变量，需要查表填表
	if (token_g.type == 'n' || token_g.type == 'f')
	{//整型或字符型常数
		if (token_g.type == 'n')
		{
			type_g = "int";
		}
		else
		{
			type_g = "float";
		}
		if (synbollistToFunc_g != NULL)
		{//非全局
			if (!checkSynbollistToFuncNum(token_g.content))
			{
				if (mainSheet_g == NULL)
				{
					mainSheet_g = new MainSheet;
				}
				else
				{
					showErrowWhenCreateASheet("MainSheet");
				}
				if (type_g == "int")
				{
					mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
				}
				else {
					mainSheet_g->content = token_g.content;//填名字
				}
				mainSheet_g->category = "v";//种类是变量
				mainSheet_g->flag = 1;

				writeTypeSheet(type_g);
				mainSheet_g->type = typeSheet_g;
				typeSheet_g = NULL;

				writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);
				levelAndOffsetAndValue_g->value = token_g.content;
				mainSheet_g->addr = levelAndOffsetAndValue_g;
				levelAndOffsetAndValue_g = NULL;
				synbollistToFunc_g->synbollist.push_back(mainSheet_g);
				mainSheet_g = NULL;
			}
		}
		else 
		{
			if (!checkGlobalNum(token_g.content))
			{
				if (mainSheet_g == NULL)
				{
					mainSheet_g = new MainSheet;
				}
				else
				{
					showErrowWhenCreateASheet("MainSheet");
				}
				if (type_g == "int")
				{
					mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
				}
				else {
					mainSheet_g->content = token_g.content;
				}
				mainSheet_g->category = "v";
				mainSheet_g->flag = 1;

				writeTypeSheet(type_g);

				mainSheet_g->type = typeSheet_g;
				typeSheet_g = NULL;

				writeLevelAndOffsetAndValue(0, global_offset_g);
				levelAndOffsetAndValue_g->value = token_g.content;
				mainSheet_g->addr = levelAndOffsetAndValue_g;
				levelAndOffsetAndValue_g = NULL;
				globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
				mainSheet_g = NULL;
			}
		}

		/***************************************/
		token in;
		in = { token_g.content,1,false };
		PUSHSEM(in);//加入语义栈
		/***************************************/
		token_g = scan.next_Token();
		return 1;

	}
	else if (token_g.type == 'c')
	{
		/***************************************/
		token in;
		in = { token_g.content,1,false };
		PUSHSEM(in);//加入语义栈
		/***************************************/
		token_g = scan.next_Token();
		return 1;
	}
	else if (ID() == 1)
	{
		if (ArrFunc() == 1)
		{
			return 1;
		}
		else
		{
			Error(token_g.row, token_g.content, "不正确");
		}
	}
	else
	{
		return 0;
	}
}

