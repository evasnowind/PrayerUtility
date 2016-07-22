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

#define MAX_INT_BIT_LENGTH 32			//ÿ��unsigned int���ܱ���Ķ�����λ��

using namespace std;


#define DISPLAY_LINE cout<<"--------------------------"<<endl

/*	����Huffman���룺
	unsigned int code ��Huffman�����Ӧ��unsigned intֵ���硰1110����Ӧ��ֵΪ14��
	unsigned int num ��code��Ӧ��Huffman����ĳ���
*/
typedef struct BitCode{
	unsigned int code;
	unsigned int num;
}BitCode;

class HuffmanTree
{
	private:
		ifstream inputFile;															//���ڶ�ȡ�ļ�
		ofstream outputFile;														//����д���ļ�
		string inputFileName;													//�����ļ�������ѹ���ļ���
		string outputFileName;													//����ļ�����ѹ������ļ���
		string decompressFileName;											//��ѹ�ļ���
		HuffmanNode * root;													//Huffman���ĸ��ڵ�
		map<char, BitCode> codeMap;									// ���ַ�-->���������롱ӳ���
		list<HuffmanNode*> priorityQueue;							//���ݳ��ִ��������������е����ȼ�����

		char * fileBuffer;															//ָ�򱣴��ļ����ݵĻ�������ָ��
		unsigned long fileLength;												//���������ļ����ַ��ĸ���
		
		int lastUseLength;															//ѹ���ļ������һ��unsigned intֵ��ʹ�õ�λ��

		unsigned long huffmanBufferLength;							//Huffman���뻺�����ĳ���
		unsigned long huffmanCnt;											//��ǰ����Huffman�����unsigned int���ĸ���
		unsigned int* huffmanCodeBuffer;								//ָ��Huffman���뻺����

		bool debug;
		//����ͳ��ʱ��
		time_t startReadFileTime, finishReadFileTime;											//��ȡ�ļ����ʱ��
		time_t startQueueTime, finishQueueTime;											//���ȼ��������ʱ��
		time_t startSortTime, finishSortTime;												//�������ʱ��
		time_t startTreeTime, finishTreeTime;												//Huffman�����ʱ��
		time_t startMapTime, finishMapTime;												//ӳ������ʱ��
		time_t startCompressTime, finishCompressTime;										//ѹ�����ʱ��
		time_t startDecompressTime,finishDecompressTime;									//��ѹ���ʱ��
		//����ͳ�����ÿռ�
		unsigned long fileBufferSpace;									//��ȡ�ļ���������С
		unsigned long huffmanCodeBufferSpace;					//ѹ��������Huffman���뻺������С
		unsigned long deFileBufferSpace;								//��ѹ�������ļ���������С
		unsigned long deHuffmanCodeBufferSpace;				//��ѹ������Huffman���뻺������С
public:
	HuffmanTree();
	/*
	���ã����캯��
	������	inputName		��ѹ���ļ���
				outputName		ѹ������ļ���
	����ֵ����
	*/
	HuffmanTree(string inputName, string outputName);
	
	/*
	���ã����ý�ѹ�ļ���
	������string str ��ѹ�ļ���
	����ֵ����
	*/
	void setDecompressFileName(string str);

	/*
	���ã���ȡҪѹ�����ļ����ݣ����ļ����ݱ�����fileBuffer��ָ��Ļ�������
	�������ޣ�ʹ��inputName��Ϊ�����ļ�����
	����ֵ�����ļ��ɹ�������0��ʧ�ܣ�����1
	*/
	int getFileContent();

	/*
	���ã���ʾԴ�ļ�������
	��������
	����ֵ����
	*/
	void showFileBuffer();

	/*
	���ã�ͳ��fileBuffer�и����ַ����ֵĴ����������ַ����ִ���������õ�һ�����ȼ����У�
		���������priorityQueue��
	��������
	����ֵ����
	*/
	void getPriorityQueue();

	/*
	���ã�����priorityQueue��������ַ����ȼ����У�����Huffman����rootָ������ĸ��ڵ�
	��������
	����ֵ����
	*/
	void getHuffmanTree();

	/*
	���ã�����Huffman������ȡ�õ�ÿ���ַ���Huffman���룬���浽ӳ���codeMap�У�
		codeMap�б��桰char --> ���������롱ӳ���ϵ
	��������
	����ֵ����
	*/
	void getBitCodeMap();

	/*
	���ã�����str��ȡ01�ַ�����Ӧ��unsigned intֵ�����磬���롰1110���򷵻�14��
	������string str		����01����string
	����ֵ��01�ַ�����Ӧ��unsigned intֵ
	*/
	unsigned int getIntHuffmanCode(string str);

	/*
	���ã���fileBuffer�е��ַ����ת��ΪHuffman���룬ʵ��ѹ��������ѹ�����д�뵽�ļ���
	��������
	����ֵ��д���ļ��ɹ�������0��ʧ�ܣ�����1
	*/
	int compressByBuffer();

	/*
	���ã���ʾԴ�ļ�������Ӧ��Huffman����
	��������
	����ֵ����
	*/
	void showCompressResult();

	/*
	���ã���ѹ����������ѹ��õ��Ľ��д�뵽�µ��ļ��У��ļ���Ϊ��decompress_��+ Դ�ļ���
	��������
	����ֵ����ѹ�ɹ�������0��ʧ���򷵻�1
	*/
	int decompressByBuffer();

	/*
	���ã���ȡcode��posλ�Ķ�������
	������	unsigned int code			����Huffman�����unsigned int��
				int pos							λ����ȡֵ��ΧΪ0~31
	����ֵ��code��posλ�Ķ�������
	*/
	int getPosBit(unsigned int code, int pos);

	/*
	���ã��ͷ��Ѿ�������ڴ�ռ�
	��������
	����ֵ����
	*/
	void destroyFunc();

	/*
	���ã��ͷ�Huffman����������ڴ�ռ�
	������ HuffmanNode * parent
	����ֵ����
	*/
	void destroyHuffmanTree(HuffmanNode * curNode);

	/*
	���ã���ʾ���������Ľ���ʱ�䣬�����ĵ��ڴ�ռ��С��
	ע�⣺��ǰ�������������Ͻ�����һ�������������һ�����Ŀ�ʼʱ�伴Ϊ��һ��������ʱ�䡣
	��������
	����ֵ����
	*/
	void showStatistics();
};

/*
	���ã��Ƚ�����HuffmanNode��Ȩֵ��С����������
	������	HuffmanNode* node1		��һ���ڵ�
				HuffmanNode* node2		�ڶ����ڵ�
	����ֵ����node1 < node2������true�����򷵻�false
*/
bool compareNodePtr(HuffmanNode* node1, HuffmanNode* node2);