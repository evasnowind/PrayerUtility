#include <iostream>
#include <cstdlib>

#include "HuffmanTree.h"

#pragma once

using namespace std;

/*
���ã�����Huffman����ѹ������ѹЧ��
��������
����ֵ����
*/
void testHuffmanCases();

/*
���ã�����ѹ������ѹĳ���ض��ļ�
������	string inputFileName			�����ļ�������ѹ���ļ�����
			string outputFileName			ѹ���ļ���
			string decompressName		��ѹ�ļ���
����ֵ����
*/
void testOnceCase(string inputFileName, string outputFileName, string decompressName);

int main()
{
// 	int homeworkNum = 2;//��ҵ����
// 	int testCaseNum = 1;	//�����ļ�����������һ����ҵ����������11�������ļ�
// 	string inputFileName, outputFileName;
// 	for (int i = 0; i < testCaseNum; i++)
// 	{
// 		cout<<"�����ļ�����"<<getInputFileName(homeworkNum, i)<<endl;
// 		cout<<"����ļ�����"<<getOutputFileName(homeworkNum, i)<<endl;
// 
// 		inputFileName = getInputFileName(homeworkNum, i);
// 		outputFileName = getOutputFileName(homeworkNum, i);
// 		processKnapsack(inputFileName, outputFileName);
// 	}

	//����Huffman����
	testHuffmanCases();

	return EXIT_SUCCESS;
}

void testHuffmanCases()
{
	int intChoice;
	string choice;
	string str;
	vector<string> inputFileNameList;
	vector<string> outputFileNameList;
	string otherFileName, otherBinaryName, otherDecompressName;

	//���õĲ�������
	//case 1:hello world hello world���ļ���С��1KB
	inputFileNameList.push_back("test.txt");
	outputFileNameList.push_back("test.txt.binary");

	//case 2:���㷨���ۡ���2�桰16.3 ���������롱�ϸ�����f e c b d a�����ĸ����ɵ����ӣ��ظ�100�����õ��ļ����ļ���С��6KB
	inputFileNameList.push_back("test2.txt");
	outputFileNameList.push_back("test2.txt.binary");

	//case 3: ɯʿ����ʮ����ʫ��Ӣ�İ棩���ļ���С��32KB
	inputFileNameList.push_back("Shakespeare.txt");
	outputFileNameList.push_back("Shakespeare.txt.binary");

	//case 4: ����������ħ��ʯ��Ӣ�İ棩���ļ���С��453KB
	inputFileNameList.push_back("Harry Potter.txt");
	outputFileNameList.push_back("Harry Potter.txt.binary");

	//case 5: ʥ����Ӣ�İ棩���ļ���С��4302KB
	inputFileNameList.push_back("HolyBible.txt");
	outputFileNameList.push_back("HolyBible.txt.binary");

	while (1)
	{
		cout<<"��ѡ������ļ���"<<endl;
		DISPLAY_LINE;
		cout<<"test����С��1KB��\t\t\t\t1"<<endl;
		DISPLAY_LINE;
		cout<<"test2����С��6KB��\t\t\t\t2"<<endl;
		DISPLAY_LINE;
		cout<<"Shakespeare.txt����С��32KB��\t\t\t3"<<endl;
		DISPLAY_LINE;
		cout<<"Harry Potter.txt����С��453KB��\t\t\t4"<<endl;
		DISPLAY_LINE;
		cout<<"Holy Bible.txt����С��4302KB�� \t\t\t5"<<endl;
		DISPLAY_LINE;
		cout<<"��������5���ļ� \t\t\t\t6"<<endl;
		DISPLAY_LINE;
		cout<<"�����ļ�\t\t\t\t\t7"<<endl;
		DISPLAY_LINE;
		cout<<"�˳�\t\t\t\t\t\t8"<<endl;
		DISPLAY_LINE;
		cout<<"����ѡ��";
		cin>>choice;

		intChoice = atoi(choice.c_str());
		if (intChoice == 0)
		{
			system("cls");
			cout<<"����Ĳ�����Ч���֣����������룡��"<<endl;
			cout<<"----------------------------------------------------------------------"<<endl;
		}else{
			switch(intChoice){
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				//Ĭ�ϵĽ�ѹ�ļ����ĸ�ʽΪ��decompress_XXX������XXX��ʾԴ�ļ�������
				testOnceCase(inputFileNameList[intChoice-1], outputFileNameList[intChoice-1], "decompress_" + inputFileNameList[intChoice-1]);
				DISPLAY_LINE;
				cout<<"��������һ���ַ��������ǿո��Ʊ����з�����Ȼ�󰴻س����ɻص��˵�����";
				cin>>str;
				system("cls");
				break;
			case 6:
				for (int i = 0; i < 5; i++)
				{
					testOnceCase(inputFileNameList[i], outputFileNameList[i], "decompress_" + inputFileNameList[i]);
					cout<<endl<<endl<<endl<<endl;
				}
				cout<<"��������һ���ַ��������ǿո��Ʊ����з�����Ȼ�󰴻س����ɻص��˵�����";
				cin>>str;
				system("cls");
				break;
			case 7:
				cout<<"����������ļ����ƣ�";
				cin>>otherFileName;
				cout<<"������ѹ���ļ�����";
				cin>>otherBinaryName;
				cout<<"�������ѹ�ļ�����";
				cin>>otherDecompressName;
				testOnceCase(otherFileName, otherBinaryName, otherDecompressName);
				DISPLAY_LINE;
				cout<<"��������һ���ַ��������ǿո��Ʊ����з�����Ȼ�󰴻س����ɻص��˵�����";
				cin>>str;
				system("cls");
				break;
			case 8:
				exit(EXIT_SUCCESS);
				break;
			default:
				system("cls");
				cout<<"����Ĳ�����Ч���֣����������룡��"<<endl;
				break;
			}
		}
	}
}

void testOnceCase(string inputFileName, string outputFileName, string decompressName)
{
	cout<<"\t �����ļ�����"<<inputFileName<<endl;;
	cout<<"\t ����ļ�����"<<outputFileName<<endl;
	cout<<"\t ��ѹ�ļ�����"<<decompressName<<endl;
	DISPLAY_LINE;
	HuffmanTree * huffmanTree = new HuffmanTree(inputFileName, outputFileName);
	huffmanTree->setDecompressFileName(decompressName);

	//��ȡԴ�ļ�
	cout<<"��ȡԴ�ļ���ʼ����"<<endl;
	DISPLAY_LINE;
	huffmanTree->getFileContent();
	cout<<"�ļ���ȡ��ϡ�"<<endl;
	DISPLAY_LINE;

	//�������ȼ�����
	cout<<"��ʼ�������ȼ����С���"<<endl;
	DISPLAY_LINE;
	huffmanTree->getPriorityQueue();
	cout<<"���ȼ�������ɡ�"<<endl;
	DISPLAY_LINE;

	//����Huffman��
	cout<<"��ʼ����Huffman������"<<endl;
	DISPLAY_LINE;
	huffmanTree->getHuffmanTree();
	cout<<"Huffman��������ɡ�"<<endl;
	DISPLAY_LINE;

	//��ȡ�ַ�ӳ���
	cout<<"��ʼ����ӳ�����"<<endl;
	DISPLAY_LINE;
	huffmanTree->getBitCodeMap();
	cout<<"ӳ�������ɡ�"<<endl;
	DISPLAY_LINE;

	//ѹ��
	cout<<"��ʼѹ������"<<endl;
	DISPLAY_LINE;
	huffmanTree->compressByBuffer();
	cout<<"ѹ��������"<<endl;
	DISPLAY_LINE;

	//�ͷ�֮ǰ��������ڴ�ռ�
	cout<<"��ʼ��������"<<endl;
	DISPLAY_LINE;
	huffmanTree->destroyFunc();
	cout<<"���������"<<endl;
	DISPLAY_LINE;

	//��ѹ
	cout<<"��ʼ��ѹ����"<<endl;
	DISPLAY_LINE;
	huffmanTree->decompressByBuffer();
	cout<<"��ѹ��ɡ�"<<endl;
	DISPLAY_LINE;
	huffmanTree->destroyFunc();

	cout<<"�����ѹ������ѹ������"<<endl;

	huffmanTree->showStatistics();
	delete huffmanTree;


	cout<<"���Խ�������"<<endl;
}