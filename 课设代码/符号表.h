#include<string>
#include<vector>
#include<iostream>

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

struct SynbollistToFunc {		//一个函数对应一张符号表
	vector<MainSheet*> synbollist;
	string func;
	int offset_max;				//当前offset最大值
};

//表的结构定义*****************************


extern vector<SynbollistToFunc*> synbollist_main_g;        //符号总表 
extern SynbollistToFunc* synbollistToFunc_g;			   //函数所对应的符号表的指针
extern SynbollistToFunc* globalsynbollistToFunc_g;		   //全局变量所要填入的地方	
extern MainSheet* mainSheet_g;					   //符号表项指针
extern TypeSheet* typeSheet_g;					   //类型表指针
extern FuncSheet* funcSheet_g;					   //函数表指针
extern vector<ParaSheet*>* paras_g;				   //参数表指针
extern ParaSheet* para_g;						   //参数表项指针
extern LevelAndOffsetAndValue* levelAndOffsetAndValue_g;		   //LevelAndOffset表指针