#include <iostream>
#include <cstdlib>

#include "HuffmanTree.h"

#pragma once

using namespace std;

/*
作用：测试Huffman编码压缩、解压效果
参数：无
返回值：无
*/
void testHuffmanCases();

/*
作用：测试压缩、解压某个特定文件
参数：	string inputFileName			输入文件名（待压缩文件名）
			string outputFileName			压缩文件名
			string decompressName		解压文件名
返回值：无
*/
void testOnceCase(string inputFileName, string outputFileName, string decompressName);

int main()
{
// 	int homeworkNum = 2;//作业次数
// 	int testCaseNum = 1;	//测试文件的数量，第一个作业测试了所有11个数据文件
// 	string inputFileName, outputFileName;
// 	for (int i = 0; i < testCaseNum; i++)
// 	{
// 		cout<<"输入文件名："<<getInputFileName(homeworkNum, i)<<endl;
// 		cout<<"输出文件名："<<getOutputFileName(homeworkNum, i)<<endl;
// 
// 		inputFileName = getInputFileName(homeworkNum, i);
// 		outputFileName = getOutputFileName(homeworkNum, i);
// 		processKnapsack(inputFileName, outputFileName);
// 	}

	//测试Huffman编码
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

	//内置的测试用例
	//case 1:hello world hello world，文件大小：1KB
	inputFileNameList.push_back("test.txt");
	outputFileNameList.push_back("test.txt.binary");

	//case 2:《算法导论》第2版“16.3 哈夫曼编码”上给出的f e c b d a五个字母所组成的例子，重复100次所得的文件，文件大小：6KB
	inputFileNameList.push_back("test2.txt");
	outputFileNameList.push_back("test2.txt.binary");

	//case 3: 莎士比亚十四行诗（英文版），文件大小：32KB
	inputFileNameList.push_back("Shakespeare.txt");
	outputFileNameList.push_back("Shakespeare.txt.binary");

	//case 4: 哈利波特与魔法石（英文版），文件大小：453KB
	inputFileNameList.push_back("Harry Potter.txt");
	outputFileNameList.push_back("Harry Potter.txt.binary");

	//case 5: 圣经（英文版），文件大小：4302KB
	inputFileNameList.push_back("HolyBible.txt");
	outputFileNameList.push_back("HolyBible.txt.binary");

	while (1)
	{
		cout<<"请选择测试文件："<<endl;
		DISPLAY_LINE;
		cout<<"test（大小：1KB）\t\t\t\t1"<<endl;
		DISPLAY_LINE;
		cout<<"test2（大小：6KB）\t\t\t\t2"<<endl;
		DISPLAY_LINE;
		cout<<"Shakespeare.txt（大小：32KB）\t\t\t3"<<endl;
		DISPLAY_LINE;
		cout<<"Harry Potter.txt（大小：453KB）\t\t\t4"<<endl;
		DISPLAY_LINE;
		cout<<"Holy Bible.txt（大小：4302KB） \t\t\t5"<<endl;
		DISPLAY_LINE;
		cout<<"测试上述5个文件 \t\t\t\t6"<<endl;
		DISPLAY_LINE;
		cout<<"其他文件\t\t\t\t\t7"<<endl;
		DISPLAY_LINE;
		cout<<"退出\t\t\t\t\t\t8"<<endl;
		DISPLAY_LINE;
		cout<<"您的选择：";
		cin>>choice;

		intChoice = atoi(choice.c_str());
		if (intChoice == 0)
		{
			system("cls");
			cout<<"输入的不是有效数字，请重新输入！！"<<endl;
			cout<<"----------------------------------------------------------------------"<<endl;
		}else{
			switch(intChoice){
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				//默认的解压文件名的格式为：decompress_XXX，其中XXX表示源文件的名称
				testOnceCase(inputFileNameList[intChoice-1], outputFileNameList[intChoice-1], "decompress_" + inputFileNameList[intChoice-1]);
				DISPLAY_LINE;
				cout<<"输入任意一个字符（不能是空格、制表、换行符），然后按回车即可回到菜单……";
				cin>>str;
				system("cls");
				break;
			case 6:
				for (int i = 0; i < 5; i++)
				{
					testOnceCase(inputFileNameList[i], outputFileNameList[i], "decompress_" + inputFileNameList[i]);
					cout<<endl<<endl<<endl<<endl;
				}
				cout<<"输入任意一个字符（不能是空格、制表、换行符），然后按回车即可回到菜单……";
				cin>>str;
				system("cls");
				break;
			case 7:
				cout<<"请输入测试文件名称：";
				cin>>otherFileName;
				cout<<"请输入压缩文件名：";
				cin>>otherBinaryName;
				cout<<"请输入解压文件名：";
				cin>>otherDecompressName;
				testOnceCase(otherFileName, otherBinaryName, otherDecompressName);
				DISPLAY_LINE;
				cout<<"输入任意一个字符（不能是空格、制表、换行符），然后按回车即可回到菜单……";
				cin>>str;
				system("cls");
				break;
			case 8:
				exit(EXIT_SUCCESS);
				break;
			default:
				system("cls");
				cout<<"输入的不是有效数字，请重新输入！！"<<endl;
				break;
			}
		}
	}
}

void testOnceCase(string inputFileName, string outputFileName, string decompressName)
{
	cout<<"\t 输入文件名："<<inputFileName<<endl;;
	cout<<"\t 输出文件名："<<outputFileName<<endl;
	cout<<"\t 解压文件名："<<decompressName<<endl;
	DISPLAY_LINE;
	HuffmanTree * huffmanTree = new HuffmanTree(inputFileName, outputFileName);
	huffmanTree->setDecompressFileName(decompressName);

	//读取源文件
	cout<<"读取源文件开始……"<<endl;
	DISPLAY_LINE;
	huffmanTree->getFileContent();
	cout<<"文件读取完毕。"<<endl;
	DISPLAY_LINE;

	//创建优先级队列
	cout<<"开始创建优先级队列……"<<endl;
	DISPLAY_LINE;
	huffmanTree->getPriorityQueue();
	cout<<"优先级队列完成。"<<endl;
	DISPLAY_LINE;

	//创建Huffman树
	cout<<"开始创建Huffman树……"<<endl;
	DISPLAY_LINE;
	huffmanTree->getHuffmanTree();
	cout<<"Huffman树创建完成。"<<endl;
	DISPLAY_LINE;

	//获取字符映射表
	cout<<"开始创建映射表……"<<endl;
	DISPLAY_LINE;
	huffmanTree->getBitCodeMap();
	cout<<"映射表创建完成。"<<endl;
	DISPLAY_LINE;

	//压缩
	cout<<"开始压缩……"<<endl;
	DISPLAY_LINE;
	huffmanTree->compressByBuffer();
	cout<<"压缩结束。"<<endl;
	DISPLAY_LINE;

	//释放之前所申请的内存空间
	cout<<"开始析构……"<<endl;
	DISPLAY_LINE;
	huffmanTree->destroyFunc();
	cout<<"完成析构。"<<endl;
	DISPLAY_LINE;

	//解压
	cout<<"开始解压……"<<endl;
	DISPLAY_LINE;
	huffmanTree->decompressByBuffer();
	cout<<"解压完成。"<<endl;
	DISPLAY_LINE;
	huffmanTree->destroyFunc();

	cout<<"已完成压缩、解压操作。"<<endl;

	huffmanTree->showStatistics();
	delete huffmanTree;


	cout<<"测试结束！！"<<endl;
}