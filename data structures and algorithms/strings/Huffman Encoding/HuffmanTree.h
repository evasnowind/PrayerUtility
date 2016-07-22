#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>

#include "HuffmanNode.h"

#define MAX_INT_BIT_LENGTH 32			//每个unsigned int所能保存的二进制位数

using namespace std;


#define DISPLAY_LINE cout<<"--------------------------"<<endl

/*	保存Huffman编码：
	unsigned int code ：Huffman编码对应的unsigned int值（如“1110”对应的值为14）
	unsigned int num ：code对应的Huffman编码的长度
*/
typedef struct BitCode{
	unsigned int code;
	unsigned int num;
}BitCode;

class HuffmanTree
{
	private:
		ifstream inputFile;															//用于读取文件
		ofstream outputFile;														//用于写入文件
		string inputFileName;													//输入文件名（待压缩文件）
		string outputFileName;													//输出文件名（压缩后的文件）
		string decompressFileName;											//解压文件名
		HuffmanNode * root;													//Huffman树的根节点
		map<char, BitCode> codeMap;									// “字符-->哈夫曼编码”映射表
		list<HuffmanNode*> priorityQueue;							//根据出现次数按照升序排列的优先级队列

		char * fileBuffer;															//指向保存文件内容的缓冲区的指针
		unsigned long fileLength;												//保存输入文件中字符的个数
		
		int lastUseLength;															//压缩文件中最后一个unsigned int值所使用的位数

		unsigned long huffmanBufferLength;							//Huffman编码缓冲区的长度
		unsigned long huffmanCnt;											//当前保存Huffman编码的unsigned int数的个数
		unsigned int* huffmanCodeBuffer;								//指向Huffman编码缓冲区

		bool debug;
		//用于统计时间
		time_t startReadFileTime, finishReadFileTime;											//读取文件完成时间
		time_t startQueueTime, finishQueueTime;											//优先级队列完成时间
		time_t startSortTime, finishSortTime;												//排序完成时间
		time_t startTreeTime, finishTreeTime;												//Huffman树完成时间
		time_t startMapTime, finishMapTime;												//映射表完成时间
		time_t startCompressTime, finishCompressTime;										//压缩完成时间
		time_t startDecompressTime,finishDecompressTime;									//解压完成时间
		//用于统计所用空间
		unsigned long fileBufferSpace;									//读取文件缓冲区大小
		unsigned long huffmanCodeBufferSpace;					//压缩过程中Huffman编码缓冲区大小
		unsigned long deFileBufferSpace;								//解压过程中文件缓冲区大小
		unsigned long deHuffmanCodeBufferSpace;				//解压过程中Huffman编码缓冲区大小
public:
	HuffmanTree();
	/*
	作用：构造函数
	参数：	inputName		待压缩文件名
				outputName		压缩后的文件名
	返回值：无
	*/
	HuffmanTree(string inputName, string outputName);
	
	/*
	作用：设置解压文件名
	参数：string str 解压文件名
	返回值：无
	*/
	void setDecompressFileName(string str);

	/*
	作用：读取要压缩的文件内容，将文件内容保存在fileBuffer所指向的缓冲区中
	参数：无（使用inputName作为输入文件名）
	返回值：打开文件成功，返回0；失败，返回1
	*/
	int getFileContent();

	/*
	作用：显示源文件的内容
	参数：无
	返回值：无
	*/
	void showFileBuffer();

	/*
	作用：统计fileBuffer中各个字符出现的次数，根据字符出现次数按升序得到一个优先级队列，
		结果保存在priorityQueue中
	参数：无
	返回值：无
	*/
	void getPriorityQueue();

	/*
	作用：根据priorityQueue所保存的字符优先级队列，生成Huffman树，root指向该树的根节点
	参数：无
	返回值：无
	*/
	void getHuffmanTree();

	/*
	作用：根据Huffman树，获取得到每个字符的Huffman编码，保存到映射表codeMap中，
		codeMap中保存“char --> 哈夫曼编码”映射关系
	参数：无
	返回值：无
	*/
	void getBitCodeMap();

	/*
	作用：根据str获取01字符串对应的unsigned int值（例如，传入“1110”则返回14）
	参数：string str		保存01串的string
	返回值：01字符串对应的unsigned int值
	*/
	unsigned int getIntHuffmanCode(string str);

	/*
	作用：将fileBuffer中的字符逐个转换为Huffman编码，实现压缩，并将压缩结果写入到文件中
	参数：无
	返回值：写入文件成功，返回0；失败，返回1
	*/
	int compressByBuffer();

	/*
	作用：显示源文件编码后对应的Huffman编码
	参数：无
	返回值：无
	*/
	void showCompressResult();

	/*
	作用：解压缩，并将解压后得到的结果写入到新的文件中，文件名为“decompress_”+ 源文件名
	参数：无
	返回值：解压成功，返回0；失败则返回1
	*/
	int decompressByBuffer();

	/*
	作用：获取code第pos位的二进制数
	参数：	unsigned int code			保存Huffman编码的unsigned int数
				int pos							位数，取值范围为0~31
	返回值：code第pos位的二进制数
	*/
	int getPosBit(unsigned int code, int pos);

	/*
	作用：释放已经申请的内存空间
	参数：无
	返回值：无
	*/
	void destroyFunc();

	/*
	作用：释放Huffman树所申请的内存空间
	参数： HuffmanNode * parent
	返回值：无
	*/
	void destroyHuffmanTree(HuffmanNode * curNode);

	/*
	作用：显示各个操作的结束时间，所消耗的内存空间大小。
	注意：当前操作结束后，马上进行下一个操作，因此下一操作的开始时间即为上一操作结束时间。
	参数：无
	返回值：无
	*/
	void showStatistics();
};

/*
	作用：比较两个HuffmanNode的权值大小，用于排序
	参数：	HuffmanNode* node1		第一个节点
				HuffmanNode* node2		第二个节点
	返回值：若node1 < node2，返回true，否则返回false
*/
bool compareNodePtr(HuffmanNode* node1, HuffmanNode* node2);