#pragma once
#ifndef _HEAD_CODE
#define _HEAD_CODE
#ifndef _HEAD_QT
#define _HEAD_QT
#include "��Ԫʽ.h"
#endif

#include <iostream>
#include <string>
#include <vector>

void createDSEG(); //����DSEG��
void createCSEGBEGIN(); //����CSEG��
void createCSEGEND(); //����CSEG��
void createCode(vector<QT>resQT); //����һ����Ԫʽ������һ�δ���
void code(QT temp, string lastOper, int i); //���ݵ�����Ԫʽ����CODE

#endif // !_HEAD_CODE
