#include "HuffmanTree.h"

/*
	���ã��Ƚ�����HuffmanNode��Ȩֵ��С����������
	������	HuffmanNode* node1		��һ���ڵ�
				HuffmanNode* node2		�ڶ����ڵ�
	����ֵ����node1 < node2������true�����򷵻�false
*/
bool compareNodePtr(HuffmanNode* node1, HuffmanNode* node2)
{
	return node1->getWeight().num < node2->getWeight().num;
}


HuffmanTree::HuffmanTree()
{
	inputFileName = "";
	outputFileName = "";
	decompressFileName = "";

	fileBuffer = NULL;
	huffmanCodeBuffer = NULL;
	root = NULL;

	debug = false;
}

void HuffmanTree::setDecompressFileName(string str)
{
	decompressFileName = str;
}

	/*
	���ã����캯��
	������	inputName		��ѹ���ļ���
				outputName		ѹ������ļ���
	����ֵ����
	*/
HuffmanTree::HuffmanTree(string inputName, string outputName)
{
	inputFileName = inputName;
	outputFileName = outputName;

	decompressFileName = "";

	fileBuffer = NULL;
	huffmanCodeBuffer = NULL;
	root = NULL;
	
	debug = false;
}

	/*
	���ã���ȡҪѹ�����ļ����ݣ����ļ����ݱ�����fileBuffer��ָ��Ļ�������
	�������ޣ�ʹ��inputName��Ϊ�����ļ�����
	����ֵ�����ļ��ɹ�������0��ʧ�ܣ�����1
	*/
int HuffmanTree::getFileContent()
{
	int exitStatus;
	int i;

	exitStatus = EXIT_SUCCESS;

	startReadFileTime = time(NULL);

	inputFile.open(inputFileName.c_str(), ios::binary);
	if (!inputFile)
	{
		cout<<"���ļ�"<<inputFileName<<"ʧ��"<<endl;
		exitStatus = EXIT_FAILURE;
	}else{
		//��ȡԴ�ļ��е��ַ�����
		inputFile.seekg(0,ios::end);
		fileLength = inputFile.tellg();
		inputFile.seekg(0, ios::beg);

		fileBuffer = (char *)malloc(sizeof(char) * fileLength);
		for (i = 0; i < fileLength; i++)
		{
			fileBuffer[i] = '\0';
		}
		//��ȡԴ�ļ�
		inputFile.read(fileBuffer, fileLength);

		fileBufferSpace = fileLength;

		inputFile.close();
		inputFile.clear();
	}

	finishReadFileTime = time(NULL);


	return exitStatus;
}

	/*
	���ã���ʾԴ�ļ�������
	��������
	����ֵ����
	*/
void HuffmanTree::showFileBuffer()
{
	cout<<"file length = "<<fileLength<<endl;
	for(int i = 0; i < fileLength; i++)
	{
		cout<<fileBuffer[i]<<" ";
	}
	cout<<endl<<"finish print";
}

	/*
	���ã�ͳ��fileBuffer�и����ַ����ֵĴ����������ַ����ִ���������õ�һ�����ȼ����У�
		���������priorityQueue��
	��������
	����ֵ����
	*/
void HuffmanTree::getPriorityQueue()
{
	unsigned long i, count;
	int loc;
	vector<WeightEntry> allCharQueue;

	startQueueTime = time(NULL);

	for (loc = 0; loc < 256; loc++)
	{
		WeightEntry tmpEntry;
		tmpEntry.ch = loc;
		tmpEntry.num = 0;
		allCharQueue.push_back(tmpEntry);
	}

	//ʹ��char��Ϊ�±�ͳ�Ƹ����ַ��ĳ��ִ���
	for (i = 0; i < fileLength; i++)
	{
		allCharQueue[ fileBuffer[i] ].num = allCharQueue[ fileBuffer[i] ].num + 1;
	}

	//�����ִ�������0��charд�뵽���ȼ�������
	for (loc = 0; loc < 256; loc++)
	{
		if (allCharQueue[loc].num > 0)
		{
			HuffmanNode * tmpNewNode = new HuffmanNode();
			tmpNewNode->setWeight(allCharQueue[loc].ch, allCharQueue[loc].num);
			priorityQueue.push_back(tmpNewNode);
		}
	}

	//ʹ���������ַ�ʽҲ�ܹ��ó����ȼ����У������ܹ�֧�����ģ���Ϊû���޶�char����ĸ�������
	//����Ҫ���ѵ�ʱ�����һ�ַ�ʽҪ��ܶࣨÿ���ַ�����Ҫ�����������������Ƿ��Ѿ���������ַ�����
	//����Ա�������
	/*
	for (i = 0; i < fileLength; i++)
	{
		char ch = fileBuffer[i];
		list<HuffmanNode *>::iterator itr = priorityQueue.begin();
		while (itr != priorityQueue.end())
		{
			if ( (*itr)->getWeight().ch == ch)
			{
				break;
			}
		itr++;
		}
		if (itr != priorityQueue.end())
		{//˵���ҵ��˶�Ӧ���ַ�����������ϼ�1
			(*itr)->incWeight();
		}else{
		//û���ҵ�����׷��һ��
			HuffmanNode * tmpNewNode = new HuffmanNode();
			tmpNewNode->setWeight(ch, 1);
			priorityQueue.push_back(tmpNewNode);
		}
	}
	*/

	finishQueueTime = time(NULL);

	startSortTime = time(NULL);

	//ʹ�ÿ������򣬻�����ȼ����У��������У�
	priorityQueue.sort(compareNodePtr);

	finishSortTime = time(NULL);
}


	/*
	���ã�����priorityQueue��������ַ����ȼ����У�����Huffman����rootָ������ĸ��ڵ�
	��������
	����ֵ����
	*/
void HuffmanTree::getHuffmanTree()
{
	int n, i, cnt;
	HuffmanNode * tmp;
	HuffmanNode *leftChild;
	HuffmanNode *rightChild;
	WeightEntry tmpEntry;
	list<HuffmanNode *>::iterator tmpItr;

	//��ȡpriorityQueue��һ����������̲�����copyPriorityQueue�Ͻ��У�����Ӱ��
	//priorityQueue�е�����
	list<HuffmanNode *> copyPriorityQueue(priorityQueue);

	startTreeTime = time(NULL);
	
	//����Huffman��
	n = copyPriorityQueue.size();
	for (i = 0; i < n-1; i++)
	{
		tmp = new HuffmanNode();

		//�������ȼ���������С��������
		tmpItr = copyPriorityQueue.begin();
		leftChild = *tmpItr;
		copyPriorityQueue.pop_front();

		tmpItr = copyPriorityQueue.begin();
		rightChild = *tmpItr;
		copyPriorityQueue.pop_front();

		//������һ���µĽڵ�
		tmp->setLeftNode(leftChild);
		tmp->setRightNode(rightChild);
		leftChild->setParent(tmp);
		rightChild->setParent(tmp);

		tmpEntry.ch = '\0';
		tmpEntry.num = leftChild->getWeight().num + rightChild->getWeight().num;
		tmp->setWeight(tmpEntry);

		//���½ڵ���뵽���ȼ������У����ұ�֤�ö���������������
		cnt = 0;
		list<HuffmanNode*>::iterator itr = copyPriorityQueue.begin();
		while (itr != copyPriorityQueue.end())
		{
			if ( (*itr)->getWeight().num > tmpEntry.num)
			{
				break;
			}
			++cnt;
			++itr;
		}
		copyPriorityQueue.insert(itr, tmp);
	}
	//���������ֻʣ��һ���ڵ㣬�ýڵ㼴ΪHuffman���ĸ��ڵ�
	tmpItr = copyPriorityQueue.begin();
	root = *tmpItr;

	finishTreeTime = time(NULL);

}

	/*
	���ã�����Huffman������ȡ�õ�ÿ���ַ���Huffman���룬���浽ӳ���codeMap�У�
		codeMap�б��桰char --> ���������롱ӳ���ϵ
	��������
	����ֵ����
	*/
void HuffmanTree::getBitCodeMap()
{
	list<HuffmanNode *>::iterator itr;
	HuffmanNode * tmpNode;
	HuffmanNode * tmpParent;
	char tmpKey;
	unsigned int tmpCode;
	unsigned int tmpCodeLength;
	int i;

	startMapTime = time(NULL);
	//���ȼ������а������ļ��г��ֵ������ַ�����˱����ö��м����ҳ������ַ�
	itr = priorityQueue.begin();
	while (itr != priorityQueue.end())
	{
		tmpNode = *itr;
		tmpKey = tmpNode->getWeight().ch;

		string tmpString;//��������Ʊ��룬�������������ı������ת��Ϊunsigned intֵ

		//����priorityQueue�б������Huffman����Ҷ�ӽڵ㣬��������Ƶ����ɻ�ȡ��Huffman���룬
		//�涨�����������һ����0�������Һ������һ����1��
		tmpParent = tmpNode->getParentNode();
		while (tmpParent != NULL)
		{
			if (tmpParent->getLeftNode() == tmpNode)
			{//����Ϊ0
				tmpString.insert(tmpString.begin(), '0');
			}else{
				//�Һ���Ϊ1
				tmpString.insert(tmpString.begin(), '1');
			}
			tmpNode = tmpNode->getParentNode();
			tmpParent = tmpNode->getParentNode();
		}

		tmpCodeLength = tmpString.size();
		//��string��ʽ�Ķ����Ʊ���ת��Ϊ��Ӧ��unsigned intֵ
		tmpCode = getIntHuffmanCode(tmpString);

		BitCode tmpBitCode;
		tmpBitCode.code = tmpCode;
		tmpBitCode.num = tmpCodeLength;
		//��ӳ���ϵ���뵽codeMap��
		codeMap.insert(map<char, BitCode>::value_type (tmpKey, tmpBitCode));

		itr++;
	}

	finishMapTime = time(NULL);

}

	/*
	���ã�����str��ȡ01�ַ�����Ӧ��unsigned intֵ�����磬���롰1110���򷵻�14��
	������string str		����01����string
	����ֵ��01�ַ�����Ӧ��unsigned intֵ
	*/
unsigned int HuffmanTree::getIntHuffmanCode(string str)
{
	unsigned int result;
	int i, tmp;

	result = 0;
	for (i = 0; i < str.size(); i++)
	{
		tmp = str[i] - '0';
		result<<=1;
		result |= tmp;
	}

	return result;
}

	/*
	���ã���fileBuffer�е��ַ����ת��ΪHuffman���룬ʵ��ѹ��������ѹ�����д�뵽�ļ���
	��������
	����ֵ��д���ļ��ɹ�������0��ʧ�ܣ�����1
	*/
int HuffmanTree::compressByBuffer()
{
	unsigned long i;
	int exitStatus;
	unsigned int tmpCodeCnt;
	char ch;
	bool flag;

	BitCode curBit, nextBit, tmpBit;
	map<char, BitCode>::iterator itr;

	exitStatus = EXIT_SUCCESS;
	
	startCompressTime = time(NULL);
	huffmanBufferLength = fileLength / 10;		//��Huffman���뻺�����ĳ�ʼ��СΪԴ�ļ���1/10
	huffmanCodeBuffer = (unsigned int *)malloc(sizeof(unsigned int) * huffmanBufferLength);

	flag = false;
	tmpCodeCnt = 0;
	curBit.code = 0;
	curBit.num = MAX_INT_BIT_LENGTH;
	for (i = 0; i < fileLength; i++)
	{
		ch = fileBuffer[i];
		itr = codeMap.find(ch);	

		tmpBit = itr->second;
		if (curBit.num >= tmpBit.num)
		{	//case 1��curBit�п��������λ���㹻������һ��Huffman���롪��
			//ֱ����λ�����漴��
			curBit.num -= tmpBit.num;
			tmpBit.code <<= curBit.num;
			curBit.code |= tmpBit.code;	//��curBit�У���ͬ�ַ���Huffman����Ӹߵ������α�����code��
		}else{
			//case 2��curBit�п��������λ������������һ��Huffman���롪��
			//ʹ��nextBit����curBit���ܱ���Ĳ���
			nextBit.code = 0;
			//���nextBit����һ���ֺ�ʣ��Ŀռ�
			nextBit.num = MAX_INT_BIT_LENGTH - tmpBit.num + curBit.num;
			//ע�⣺nextBit��code��ֱ�Ӹ���tmpBit��code��������λ������������ǰ�治��Ҫ�Ĳ���
			nextBit.code |= tmpBit.code;
			nextBit.code <<= nextBit.num;//����ǰ�治��Ҫ��λ��

			tmpBit.code >>=(tmpBit.num - curBit.num);//�������治��Ҫ��λ�����ѱ�����nextBit��ߵ�ǰ��λ��
			//��tmpBit��ʣ�ಿ�֣�ԭ�����ݵ�ǰ�벿�֣����浽curBit�У���curBit����
			curBit.code |= tmpBit.code;

			//���������ݵ�curBit.code��������
			huffmanCodeBuffer[tmpCodeCnt] = curBit.code;
			++tmpCodeCnt;
			
			if (tmpCodeCnt == huffmanBufferLength)
			{	//����������ռ䲻����������һ����ԭ����2���Ŀռ䣬����ԭ�������е����ݸ��Ƶ��¿��ٵĿռ���
				huffmanBufferLength *= 2;
				unsigned int * tmpPrt = (unsigned int*)malloc(sizeof(unsigned int) * huffmanBufferLength);

				//��������
				for (unsigned int j = 0; j < tmpCodeCnt; j++)
				{
					tmpPrt[j] = huffmanCodeBuffer[j];
				}

				free(huffmanCodeBuffer);
				huffmanCodeBuffer = tmpPrt;
			}

			//����curBit���Ա㱣����һ������
			curBit.num = nextBit.num;
			curBit.code = nextBit.code;
		}
	}

	//�����һ������д�뵽��������
	huffmanCodeBuffer[tmpCodeCnt] = curBit.code;
	++tmpCodeCnt;
	//huffmanCnt����ѹ���ļ����ж��ٸ����������ж��ٸ�unsigned int��
	huffmanCnt = tmpCodeCnt;

	//�������һ������ʹ���˶���λ
	lastUseLength = MAX_INT_BIT_LENGTH - curBit.num;
	//����Huffman���뻺�������ÿռ�
	huffmanCodeBufferSpace = huffmanBufferLength;

	//��������д�뵽�ļ���
	outputFile.open(outputFileName.c_str(), ofstream::out | ofstream::binary);

	if ( !outputFile)
	{
		cout<<"����д���ļ�ʧ��"<<endl;
		exitStatus = EXIT_FAILURE;
	}else{
		//��ȡ���ȼ����г���
		int queueLength = priorityQueue.size();
		/*
			������������д�뵽�ļ���������
				1�����ļ�ͷд�븨����Ϣ���Ա��ѹ����ѹ��ǰ�ļ����ַ������������������Ӧ��unsigned int������
					���һ��unsigned int��ʹ�õ�λ�������ȼ�������struct WeightEntry�ĸ��������ȼ����У�д��struct WeightEntry��
				2��д�뻺�����еĹ���������
		*/
		outputFile.write((char*)&fileLength,  sizeof(unsigned long));		//д��ѹ��ǰ�ļ����ַ�����
		outputFile.write((char*)&huffmanCnt, sizeof(unsigned long));	//д������������Ӧ��unsigned int����
		outputFile.write((char*)&lastUseLength, sizeof(int));					//д�����һ��unsigned int��ʹ�õ�λ��
		outputFile.write((char*)&queueLength, sizeof(queueLength));	//д�����ȼ�������struct WeightEntry�ĸ���

		//д�����ȼ����У�д�����struct WeightEntry�����Ա��ѹʱ�ؽ�Huffman��
		list<HuffmanNode*>::iterator itr = priorityQueue.begin();
		while (itr != priorityQueue.end())
		{
			WeightEntry tmpEntry = (*itr)->getWeight();
			outputFile.write((char*)&tmpEntry, sizeof(tmpEntry));
			++itr;
		}

		//��Huffman���뻺����д���ļ�
		outputFile.write((char *)huffmanCodeBuffer, huffmanCnt * 4);

		outputFile.close();
		outputFile.clear();

		//ѹ���������ͷ�fileBuffer������ڴ�ռ�
		free(fileBuffer);
		fileBuffer = NULL;
	}

	finishCompressTime = time(NULL);

	return exitStatus;
}

	/*
	���ã���ʾԴ�ļ�������Ӧ��Huffman����
	��������
	����ֵ����
	*/
void HuffmanTree::showCompressResult()
{
	cout<<endl;
	for (unsigned long i = 0; i < huffmanCnt; i++)
	{
		unsigned int tmp = huffmanCodeBuffer[i];
		cout<<dec<<tmp<<" hex:";
		cout<<hex<<huffmanCodeBuffer[i]<<endl;
	}
}

	/*
	���ã���ѹ����������ѹ��õ��Ľ��д�뵽�µ��ļ��У��ļ���Ϊ��decompress_��+ Դ�ļ���
	��������
	����ֵ����ѹ�ɹ�������0��ʧ���򷵻�1
	*/
int HuffmanTree::decompressByBuffer()
{
	int exitStatus;

	exitStatus = EXIT_SUCCESS;

	startDecompressTime = time(NULL);
	inputFile.open(outputFileName.c_str(), ios::binary);
	if (!inputFile)
	{
		cout<<"���ļ�"<<inputFileName<<"ʧ��"<<endl;
		exitStatus = EXIT_FAILURE;
	}else{
		string decompress;//��ѹ�ļ�����decompress_XXX
		unsigned long deFileLength;
		unsigned long deHuffmanCnt;
		int deLastUseLength;
		int deQueueLength;
		unsigned long i;
		list<HuffmanNode*> dePriorityQueue;
		unsigned int* deCodeBuffer;
		char * deFileBuffer;

		HuffmanNode * curNode;
		int curBinaryBit;
		int loc;
		unsigned int tmpCode;
		unsigned long deFileCnt;
		unsigned long crNum;

		if (decompressFileName == "")
		{//���û�����ý�ѹ�ļ�������Ĭ�����Ƶĸ�ʽΪdecompress_XXX
			decompress = "decompress_" + inputFileName; 
		}else{
			decompress = decompressFileName;
		}

		/*
			��ȡ�ļ���ʼ���ֵĸ�����Ϣ��
				ѹ��ǰ�ļ����ַ������������������Ӧ��unsigned int���������һ��unsigned int��ʹ�õ�λ����
				���ȼ�������struct WeightEntry�ĸ��������ȼ����У�д��struct WeightEntry��
		*/
		inputFile.read((char*)&deFileLength, sizeof(deFileLength));
		inputFile.read((char*)&deHuffmanCnt, sizeof(deHuffmanCnt));
		inputFile.read((char*)&deLastUseLength, sizeof(deLastUseLength));
		inputFile.read((char*)&deQueueLength, sizeof(deQueueLength));

		if (debug)
		{
			cout<<"deFileLength = "<<deFileLength<<endl;
			cout<<"deHuffmanCnt = "<<deHuffmanCnt<<endl;
			cout<<"deLastUseLength = "<<deLastUseLength<<endl;
			cout<<"deQueueLength = "<<deQueueLength<<endl;
		}

		//������ѹ�������������ѹ��Ľ��
		//ע�⣺����Ҫ���ļ�β�����Ͻ�����ʶ��NULL�������Ҫ������һ��char
		deFileBuffer = (char *)malloc(sizeof(char) * (deFileLength + 1));
		for (i = 0; i < (deFileLength + 1); i++)
		{
			deFileBuffer[i] = '\0';
		}

		//�����ѹ��������С
		deFileBufferSpace = deFileLength;

		//��ȡ���ȼ�����
		for (i = 0; i < deQueueLength; i++)
		{
			HuffmanNode * deNode = new HuffmanNode();
			WeightEntry deWeight;

			inputFile.read((char*)&deWeight, sizeof(WeightEntry));

			deNode->setWeight(deWeight);
			dePriorityQueue.push_back(deNode);
		}
		//����Huffman���뻺����������ѹ���ļ��е�����Huffman����
		deCodeBuffer = (unsigned int*)malloc(sizeof(unsigned int) * deHuffmanCnt);

		//�����ѹHuffman���뻺����
		deHuffmanCodeBufferSpace = deHuffmanCnt;

		for (i = 0; i < deHuffmanCnt; i++)
		{
			unsigned int tmpCode;

			inputFile.read((char*)&tmpCode, sizeof(unsigned int));
			deCodeBuffer[i] = tmpCode; 
		}
		/*
		��deCodeBufferֱ�Ӹ�ֵ��HuffmanTree��˽�г�ԱhuffmanCodeBuffer��
		dePriorityQueueֱ�Ӹ�ֵ��HuffmanTree��˽�г�ԱpriorityQueue�������Ϳ���
		ֱ��ʹ��getHuffmanTree()�����ؽ�Huffman����������д�ؽ��Ĵ���
		*/
		huffmanCodeBuffer = deCodeBuffer;
		priorityQueue = dePriorityQueue;
		this->getHuffmanTree();
		
		inputFile.close();
		inputFile.clear();

		//��ʼ����
		//�Ƚ����������һ������֮������б���
		curNode = root;
		curBinaryBit = 0;
		deFileCnt = 0;
		crNum = 0;
		for (i = 0; i < deHuffmanCnt - 1; i++)
		{
			tmpCode = deCodeBuffer[i];

			for (loc = MAX_INT_BIT_LENGTH-1; loc >= 0; loc--)
			{
				curBinaryBit = getPosBit(tmpCode, loc);

				if (! curNode->hasChildren())
				{	//���û�к��ӣ�˵���Ѿ��ߵ�����������Ҷ�ӽڵ㣬�����һ���ַ�����˽��䱣�浽��ѹ�ļ��������У�
					//���ҽ�curNode����Ϊroot�����¿�ʼ����
					if (curNode->getWeight().ch == 10 && deFileBuffer[deFileCnt - 1] == 13)
					{	/*	����½�ѹ�����ַ�ΪLF���ҽ�ѹ��������ǰһ���ַ�ΪCR��˵�������˻س����У�CR LF��������C���п��лὫ
							LF�Զ�ת��ΪCR LF�������Ҫȥ����ѹ���������ѱ����CR�������ظ�����֤��ѹ�ļ���Դ�ļ���ȫһ�¡�
						*/
						deFileBuffer[deFileCnt - 1] = 10; //��ǰһ���ַ�����ΪLF
						++crNum;
					}else{
						deFileBuffer[deFileCnt] = curNode->getWeight().ch;
						++deFileCnt;
					}
					
					curNode = root;
				}
				curNode = (curBinaryBit == 0)? curNode->getLeftNode():curNode->getRightNode();
			}
		}

		//�������һ������
		tmpCode = deCodeBuffer[deHuffmanCnt - 1];
		for (loc = MAX_INT_BIT_LENGTH - 1; loc >= (MAX_INT_BIT_LENGTH - deLastUseLength); loc--)
		{
			curBinaryBit = getPosBit(tmpCode, loc);
			if (! curNode->hasChildren())
			{	//���û�к��ӣ�˵���Ѿ��ߵ�����������Ҷ�ӽڵ㣬�����һ���ַ�����˽��䱣�浽��ѹ�ļ��������У�
				//���ҽ�curNode����Ϊroot�����¿�ʼ����
				deFileBuffer[deFileCnt] = curNode->getWeight().ch;
				++deFileCnt;
				curNode = root;
			}
			curNode = (curBinaryBit == 0)? curNode->getLeftNode():curNode->getRightNode();
		}

		//�����һ���ַ�д�뵽��������
		deFileBuffer[deFileCnt] = curNode->getWeight().ch;
		++deFileCnt;
		//���ļ�β������NULL����ʾ�ļ�����������д���ļ�ʱ�ļ�β�����������
		deFileBuffer[deFileCnt] = NULL;

		outputFile.open(decompress.c_str(), ios::trunc);
		if (!outputFile)
		{
			cout<<"��ѹ���ļ���ʧ��"<<endl;
			exitStatus = EXIT_FAILURE;
		}else{
			outputFile<<deFileBuffer;
			outputFile.close();
			outputFile.clear();
		}
		free(deFileBuffer);
	}

	finishDecompressTime = time(NULL);

	return exitStatus;
}

	/*
	���ã���ȡcode��posλ�Ķ�������
	������	unsigned int code			����Huffman�����unsigned int��
				int pos							λ����ȡֵ��ΧΪ0~31
	����ֵ��code��posλ�Ķ�������
	*/
int HuffmanTree::getPosBit(unsigned int code, int pos)
{
	return (code & (1 << pos)) != 0? 1:0;
}

void HuffmanTree::destroyFunc()
{
	if (fileBuffer != NULL)
	{
		free(fileBuffer);
	}
	if (huffmanCodeBuffer != NULL)
	{
		free(huffmanCodeBuffer);
	}
	
	codeMap.clear();
	priorityQueue.clear();

	if (root != NULL)
	{
		destroyHuffmanTree(root);
	}
}

	/*
	���ã��ͷ�Huffman����������ڴ�ռ�
	������ HuffmanNode * parent
	����ֵ����
	*/
void HuffmanTree::destroyHuffmanTree(HuffmanNode * curNode)
{
	//�Եݹ鷽ʽ�ͷ�����Huffman�ڵ�������Ŀռ�
	if (curNode->hasChildren())
	{
		destroyHuffmanTree(curNode->getLeftNode());
		destroyHuffmanTree(curNode->getRightNode());
		delete curNode;
	}else{
		//û�к��ӣ�˵����Ҷ�ӽڵ㣬ֱ��ɾ������
		HuffmanNode * tmpNode;
		tmpNode = curNode->getParentNode();
		if (tmpNode->getLeftNode() == curNode)
		{
			tmpNode->setLeftNode(NULL);
		}else{
			tmpNode->setRightNode(NULL);
		}
		delete curNode;
	}
}

void HuffmanTree::showStatistics()
{
	cout<<"=========================================="<<endl;
	cout<<"----------  === ʱ��ͳ�� ===  ------------"<<endl;
	cout<<"���ļ�:"<<endl<<"\t��ʼʱ�䣺"<<ctime(&startReadFileTime)<<"\t����ʱ�䣺"<<ctime(&finishReadFileTime);
	cout<<"�������ȼ�����:"<<endl<<"\t��ʼʱ�䣺"<<ctime(&startQueueTime)<<"\t����ʱ�䣺"<<ctime(&finishQueueTime);
	cout<<"���ȼ���������:"<<endl<<"\t��ʼʱ�䣺"<<ctime(&startSortTime)<<"\t����ʱ�䣺"<<ctime(&finishSortTime);
	cout<<"����Huffman��:"<<endl<<"\t��ʼʱ�䣺"<<ctime(&startTreeTime)<<"\t����ʱ�䣺"<<ctime(&finishTreeTime);
	cout<<"����ӳ���:"<<endl<<"\t��ʼʱ�䣺"<<ctime(&startMapTime)<<"\t����ʱ�䣺"<<ctime(&finishMapTime);
	cout<<"ѹ��:"<<endl<<"\t��ʼʱ�䣺"<<ctime(&startCompressTime)<<"\t����ʱ�䣺"<<ctime(&finishCompressTime);
	cout<<"��ѹ:"<<endl<<"\t��ʼʱ�䣺"<<ctime(&startDecompressTime)<<"\t����ʱ�䣺"<<ctime(&finishDecompressTime);
	DISPLAY_LINE;
	cout<<"----------  === �ռ�ͳ�� ===  ------------"<<endl;
	cout<<"ѹ�����̣�"<<endl;
	cout<<"\t ��ȡ�ļ���������С��"<<fileBufferSpace<<" bytes"<<endl;
	cout<<"\t Huffman���뻺������С��"<<huffmanCodeBufferSpace<<" * 4 bytes"<<endl;
	cout<<"��ѹ���̣�"<<endl;
	cout<<"\t ��ȡ�ļ���������С��"<<deFileBufferSpace<<" bytes"<<endl;
	cout<<"\t Huffman���뻺������С��"<<deHuffmanCodeBufferSpace<<" * 4 bytes"<<endl;
	cout<<endl<<"ѹ���ȣ�ѹ���ļ���С/Դ�ļ���С�������ļ�ͷ�����ڽ�ѹ�ĸ�����Ϣ����"<<( (deHuffmanCodeBufferSpace * 4) * 1.0 / fileLength)<<endl;
}