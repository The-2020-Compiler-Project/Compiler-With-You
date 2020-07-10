#include "�����.h"
#include <stack>
#include <queue>
#include <fstream>

extern vector<QT>resQT;

void firstview() //��һ�����,���������Ż������Ԫʽ��resQT
{
	vector<QT>::iterator it;//ָ��qt���ݵĵ�����
	vector<QT>block_qt;
	resQT.clear();
	int i = 1;
	for (it = qt.begin(); it != qt.end(); it++) //����qt
	{
		if (i != it->block) //itΪ��һ���������ڵ���Ԫʽ
		{
			createDAG(block_qt);
			createQT();
			createSYMBL();
			changeActive();
			addSYMBL(); //����һ��ȫ�ֵķ��ű�ΪDSEG����׼��
			createRes(); //�����Ż������Ԫʽ��resQT
			i++;
			block_qt.clear();//������һ�����������
		}
		block_qt.push_back(*it); //ȡ��Ԫʽ
	}
}

void secondview() //�ڶ������������ת���
{
	vector<QT>::iterator it; //����resQT�ĵ�����
	stack<token*>ifstack; //if����ջ
	stack<token*>whstack; //while����ջ
	stack<int>whstacknum; //while����ջ
	queue<int>forqueuenum; //for����������
	stack<int>fornum_level; //forǶ������ջ
	queue<token*>forqueue; //for����������
	stack<token*>for_level; //forǶ������ջ
	int i = 1; //��Ԫʽ�����
	for (it = resQT.begin(); it != resQT.end(); i++, it++)
	{
		if (it->Operat == "if")
		{
			ifstack.push(&it->res);
		}
		else if (it->Operat == "el")
		{
			ifstack.top()->name = to_string(i + 1);
			ifstack.pop();
			ifstack.push(&it->res);
		}
		else if (it->Operat == "ie")
		{
			ifstack.top()->name = to_string(i);
			ifstack.pop();
		}
		else if (it->Operat == "wh")
		{
			whstacknum.push(i);
		}
		else if (it->Operat == "do")
		{
			whstack.push(&it->res);
		}
		else if (it->Operat == "we")
		{
			it->res.name = to_string(whstacknum.top());
			whstacknum.pop();
			whstack.top()->name = to_string(i);
			whstack.pop();
		}
		else if (it->Operat == "for")
		{
			if (!forqueue.empty())
			{
				for_level.push(forqueue.front());
				forqueue.pop();
				fornum_level.push(forqueuenum.front());
				forqueuenum.pop();
			}
		}
		else if (it->Operat == "fordo")
		{
			forqueue.push(&it->opB);
			forqueue.push(&it->res);
			forqueuenum.push(i - 1);
			forqueuenum.push(i + 1);
		}
		else if (it->Operat == "jumpdo")
		{
			it->res.name = to_string(forqueuenum.front());
			forqueuenum.pop();
		}
		else if (it->Operat == "dos")
		{
			forqueue.front()->name = to_string(i + 1);
			forqueue.pop();
		}
		else if (it->Operat == "doe")
		{
			it->res.name = to_string(forqueuenum.front());
			forqueuenum.pop();
		}
		else if (it->Operat == "fore")
		{
			forqueue.front()->name = to_string(i);
			forqueue.pop();
			if (!for_level.empty())
			{
				forqueue.push(for_level.top());
				for_level.pop();
				forqueuenum.push(fornum_level.top());
				fornum_level.pop();
			}
		}
	}
}

void thirdview() //���������������Ŀ�����
{
	createDSEG(); //����DSEG��
	createCSEGBEGIN(); //����CSEG��
	createCode(resQT); //����Ŀ�����
	createCSEGEND(); //����CSEG��
}

void reviewresQT() //����resQT�����
{
	ofstream file;
	file.open("C:\\byresult\\resQT.txt");
	vector<QT>::iterator it;
	for (it = resQT.begin(); it != resQT.end(); it++)
	{
		file << it->Operat << "	" << it->opA.name <<
			"	" << it->opB.name << "	" <<
			it->res.name << endl;
	}
}

