#include "HuffmanTree.h"

/*
	作用：比较两个HuffmanNode的权值大小，用于排序
	参数：	HuffmanNode* node1		第一个节点
				HuffmanNode* node2		第二个节点
	返回值：若node1 < node2，返回true，否则返回false
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
	作用：构造函数
	参数：	inputName		待压缩文件名
				outputName		压缩后的文件名
	返回值：无
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
	作用：读取要压缩的文件内容，将文件内容保存在fileBuffer所指向的缓冲区中
	参数：无（使用inputName作为输入文件名）
	返回值：打开文件成功，返回0；失败，返回1
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
		cout<<"打开文件"<<inputFileName<<"失败"<<endl;
		exitStatus = EXIT_FAILURE;
	}else{
		//获取源文件中的字符个数
		inputFile.seekg(0,ios::end);
		fileLength = inputFile.tellg();
		inputFile.seekg(0, ios::beg);

		fileBuffer = (char *)malloc(sizeof(char) * fileLength);
		for (i = 0; i < fileLength; i++)
		{
			fileBuffer[i] = '\0';
		}
		//读取源文件
		inputFile.read(fileBuffer, fileLength);

		fileBufferSpace = fileLength;

		inputFile.close();
		inputFile.clear();
	}

	finishReadFileTime = time(NULL);


	return exitStatus;
}

	/*
	作用：显示源文件的内容
	参数：无
	返回值：无
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
	作用：统计fileBuffer中各个字符出现的次数，根据字符出现次数按升序得到一个优先级队列，
		结果保存在priorityQueue中
	参数：无
	返回值：无
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

	//使用char作为下标统计各个字符的出现次数
	for (i = 0; i < fileLength; i++)
	{
		allCharQueue[ fileBuffer[i] ].num = allCharQueue[ fileBuffer[i] ].num + 1;
	}

	//将出现次数大于0的char写入到优先级队列中
	for (loc = 0; loc < 256; loc++)
	{
		if (allCharQueue[loc].num > 0)
		{
			HuffmanNode * tmpNewNode = new HuffmanNode();
			tmpNewNode->setWeight(allCharQueue[loc].ch, allCharQueue[loc].num);
			priorityQueue.push_back(tmpNewNode);
		}
	}

	//使用下面这种方式也能够得出优先级队列，并且能够支持中文（因为没有限定char数组的个数），
	//但需要花费的时间比上一种方式要多很多（每个字符都需要遍历整个队列来看是否已经存了这个字符），
	//因此仍保留代码
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
		{//说明找到了对应的字符，在其基础上加1
			(*itr)->incWeight();
		}else{
		//没有找到，则追加一项
			HuffmanNode * tmpNewNode = new HuffmanNode();
			tmpNewNode->setWeight(ch, 1);
			priorityQueue.push_back(tmpNewNode);
		}
	}
	*/

	finishQueueTime = time(NULL);

	startSortTime = time(NULL);

	//使用快速排序，获得优先级队列（升序排列）
	priorityQueue.sort(compareNodePtr);

	finishSortTime = time(NULL);
}


	/*
	作用：根据priorityQueue所保存的字符优先级队列，生成Huffman树，root指向该树的根节点
	参数：无
	返回值：无
	*/
void HuffmanTree::getHuffmanTree()
{
	int n, i, cnt;
	HuffmanNode * tmp;
	HuffmanNode *leftChild;
	HuffmanNode *rightChild;
	WeightEntry tmpEntry;
	list<HuffmanNode *>::iterator tmpItr;

	//获取priorityQueue的一个拷贝，后继操作在copyPriorityQueue上进行，避免影响
	//priorityQueue中的数据
	list<HuffmanNode *> copyPriorityQueue(priorityQueue);

	startTreeTime = time(NULL);
	
	//建立Huffman树
	n = copyPriorityQueue.size();
	for (i = 0; i < n-1; i++)
	{
		tmp = new HuffmanNode();

		//弹出优先级队列中最小的两个数
		tmpItr = copyPriorityQueue.begin();
		leftChild = *tmpItr;
		copyPriorityQueue.pop_front();

		tmpItr = copyPriorityQueue.begin();
		rightChild = *tmpItr;
		copyPriorityQueue.pop_front();

		//创建出一个新的节点
		tmp->setLeftNode(leftChild);
		tmp->setRightNode(rightChild);
		leftChild->setParent(tmp);
		rightChild->setParent(tmp);

		tmpEntry.ch = '\0';
		tmpEntry.num = leftChild->getWeight().num + rightChild->getWeight().num;
		tmp->setWeight(tmpEntry);

		//将新节点加入到优先级队列中，并且保证该队列仍是升序排列
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
	//队列中最后只剩下一个节点，该节点即为Huffman树的根节点
	tmpItr = copyPriorityQueue.begin();
	root = *tmpItr;

	finishTreeTime = time(NULL);

}

	/*
	作用：根据Huffman树，获取得到每个字符的Huffman编码，保存到映射表codeMap中，
		codeMap中保存“char --> 哈夫曼编码”映射关系
	参数：无
	返回值：无
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
	//优先级队列中包含了文件中出现的所有字符，因此遍历该队列即可找出所有字符
	itr = priorityQueue.begin();
	while (itr != priorityQueue.end())
	{
		tmpNode = *itr;
		tmpKey = tmpNode->getWeight().ch;

		string tmpString;//保存二进制编码，遍历完获得完整的编码后再转换为unsigned int值

		//由于priorityQueue中保存的是Huffman树的叶子节点，因此逆向推导即可获取其Huffman编码，
		//规定：走左孩子则加一个“0”，走右孩子则加一个“1”
		tmpParent = tmpNode->getParentNode();
		while (tmpParent != NULL)
		{
			if (tmpParent->getLeftNode() == tmpNode)
			{//左孩子为0
				tmpString.insert(tmpString.begin(), '0');
			}else{
				//右孩子为1
				tmpString.insert(tmpString.begin(), '1');
			}
			tmpNode = tmpNode->getParentNode();
			tmpParent = tmpNode->getParentNode();
		}

		tmpCodeLength = tmpString.size();
		//将string形式的二进制编码转换为对应的unsigned int值
		tmpCode = getIntHuffmanCode(tmpString);

		BitCode tmpBitCode;
		tmpBitCode.code = tmpCode;
		tmpBitCode.num = tmpCodeLength;
		//将映射关系加入到codeMap中
		codeMap.insert(map<char, BitCode>::value_type (tmpKey, tmpBitCode));

		itr++;
	}

	finishMapTime = time(NULL);

}

	/*
	作用：根据str获取01字符串对应的unsigned int值（例如，传入“1110”则返回14）
	参数：string str		保存01串的string
	返回值：01字符串对应的unsigned int值
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
	作用：将fileBuffer中的字符逐个转换为Huffman编码，实现压缩，并将压缩结果写入到文件中
	参数：无
	返回值：写入文件成功，返回0；失败，返回1
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
	huffmanBufferLength = fileLength / 10;		//设Huffman编码缓冲区的初始大小为源文件的1/10
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
		{	//case 1：curBit中空余二进制位数足够保存下一个Huffman编码——
			//直接移位、保存即可
			curBit.num -= tmpBit.num;
			tmpBit.code <<= curBit.num;
			curBit.code |= tmpBit.code;	//在curBit中，不同字符的Huffman编码从高到低依次保存在code中
		}else{
			//case 2：curBit中空余二进制位数不够保存下一个Huffman编码——
			//使用nextBit保存curBit不能保存的部分
			nextBit.code = 0;
			//算出nextBit保存一部分后剩余的空间
			nextBit.num = MAX_INT_BIT_LENGTH - tmpBit.num + curBit.num;
			//注意：nextBit的code是直接复制tmpBit的code，利用移位来“挤掉”最前面不需要的部分
			nextBit.code |= tmpBit.code;
			nextBit.code <<= nextBit.num;//挤掉前面不需要的位数

			tmpBit.code >>=(tmpBit.num - curBit.num);//挤掉后面不需要的位数（已保存在nextBit最高的前几位）
			//将tmpBit中剩余部分（原有数据的前半部分）保存到curBit中，将curBit塞满
			curBit.code |= tmpBit.code;

			//将塞满数据的curBit.code保存起来
			huffmanCodeBuffer[tmpCodeCnt] = curBit.code;
			++tmpCodeCnt;
			
			if (tmpCodeCnt == huffmanBufferLength)
			{	//如果缓冲区空间不够，则申请一个比原来大2倍的空间，并将原缓冲区中的内容复制到新开辟的空间中
				huffmanBufferLength *= 2;
				unsigned int * tmpPrt = (unsigned int*)malloc(sizeof(unsigned int) * huffmanBufferLength);

				//复制数据
				for (unsigned int j = 0; j < tmpCodeCnt; j++)
				{
					tmpPrt[j] = huffmanCodeBuffer[j];
				}

				free(huffmanCodeBuffer);
				huffmanCodeBuffer = tmpPrt;
			}

			//更新curBit，以便保存下一个数据
			curBit.num = nextBit.num;
			curBit.code = nextBit.code;
		}
	}

	//将最后一个编码写入到缓冲区中
	huffmanCodeBuffer[tmpCodeCnt] = curBit.code;
	++tmpCodeCnt;
	//huffmanCnt保存压缩文件中有多少个编码数（有多少个unsigned int）
	huffmanCnt = tmpCodeCnt;

	//保存最后一个数字使用了多少位
	lastUseLength = MAX_INT_BIT_LENGTH - curBit.num;
	//保存Huffman编码缓冲区所用空间
	huffmanCodeBufferSpace = huffmanBufferLength;

	//将编码结果写入到文件中
	outputFile.open(outputFileName.c_str(), ofstream::out | ofstream::binary);

	if ( !outputFile)
	{
		cout<<"编码写入文件失败"<<endl;
		exitStatus = EXIT_FAILURE;
	}else{
		//获取优先级队列长度
		int queueLength = priorityQueue.size();
		/*
			将哈夫曼编码写入到文件分两步：
				1、在文件头写入辅助信息，以便解压——压缩前文件内字符个数，哈夫曼编码对应的unsigned int个数，
					最后一个unsigned int所使用的位数，优先级队列中struct WeightEntry的个数，优先级队列（写入struct WeightEntry）
				2、写入缓冲区中的哈夫曼编码
		*/
		outputFile.write((char*)&fileLength,  sizeof(unsigned long));		//写入压缩前文件内字符个数
		outputFile.write((char*)&huffmanCnt, sizeof(unsigned long));	//写入哈夫曼编码对应的unsigned int个数
		outputFile.write((char*)&lastUseLength, sizeof(int));					//写入最后一个unsigned int所使用的位数
		outputFile.write((char*)&queueLength, sizeof(queueLength));	//写入优先级队列中struct WeightEntry的个数

		//写入优先级队列（写入的是struct WeightEntry），以便解压时重建Huffman树
		list<HuffmanNode*>::iterator itr = priorityQueue.begin();
		while (itr != priorityQueue.end())
		{
			WeightEntry tmpEntry = (*itr)->getWeight();
			outputFile.write((char*)&tmpEntry, sizeof(tmpEntry));
			++itr;
		}

		//将Huffman编码缓冲区写入文件
		outputFile.write((char *)huffmanCodeBuffer, huffmanCnt * 4);

		outputFile.close();
		outputFile.clear();

		//压缩结束，释放fileBuffer申请的内存空间
		free(fileBuffer);
		fileBuffer = NULL;
	}

	finishCompressTime = time(NULL);

	return exitStatus;
}

	/*
	作用：显示源文件编码后对应的Huffman编码
	参数：无
	返回值：无
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
	作用：解压缩，并将解压后得到的结果写入到新的文件中，文件名为“decompress_”+ 源文件名
	参数：无
	返回值：解压成功，返回0；失败则返回1
	*/
int HuffmanTree::decompressByBuffer()
{
	int exitStatus;

	exitStatus = EXIT_SUCCESS;

	startDecompressTime = time(NULL);
	inputFile.open(outputFileName.c_str(), ios::binary);
	if (!inputFile)
	{
		cout<<"打开文件"<<inputFileName<<"失败"<<endl;
		exitStatus = EXIT_FAILURE;
	}else{
		string decompress;//解压文件名：decompress_XXX
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
		{//如果没有设置解压文件名，则默认名称的格式为decompress_XXX
			decompress = "decompress_" + inputFileName; 
		}else{
			decompress = decompressFileName;
		}

		/*
			读取文件起始部分的辅助信息：
				压缩前文件内字符个数，哈夫曼编码对应的unsigned int个数，最后一个unsigned int所使用的位数，
				优先级队列中struct WeightEntry的个数，优先级队列（写入struct WeightEntry）
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

		//创建解压缓冲区，保存解压后的结果
		//注意：由于要在文件尾部加上结束标识符NULL，因此需要多申请一个char
		deFileBuffer = (char *)malloc(sizeof(char) * (deFileLength + 1));
		for (i = 0; i < (deFileLength + 1); i++)
		{
			deFileBuffer[i] = '\0';
		}

		//保存解压缓冲区大小
		deFileBufferSpace = deFileLength;

		//读取优先级队列
		for (i = 0; i < deQueueLength; i++)
		{
			HuffmanNode * deNode = new HuffmanNode();
			WeightEntry deWeight;

			inputFile.read((char*)&deWeight, sizeof(WeightEntry));

			deNode->setWeight(deWeight);
			dePriorityQueue.push_back(deNode);
		}
		//创建Huffman编码缓冲区，保存压缩文件中的所有Huffman编码
		deCodeBuffer = (unsigned int*)malloc(sizeof(unsigned int) * deHuffmanCnt);

		//保存解压Huffman编码缓冲区
		deHuffmanCodeBufferSpace = deHuffmanCnt;

		for (i = 0; i < deHuffmanCnt; i++)
		{
			unsigned int tmpCode;

			inputFile.read((char*)&tmpCode, sizeof(unsigned int));
			deCodeBuffer[i] = tmpCode; 
		}
		/*
		将deCodeBuffer直接赋值给HuffmanTree的私有成员huffmanCodeBuffer，
		dePriorityQueue直接赋值给HuffmanTree的私有成员priorityQueue，这样就可以
		直接使用getHuffmanTree()方法重建Huffman树，不必重写重建的代码
		*/
		huffmanCodeBuffer = deCodeBuffer;
		priorityQueue = dePriorityQueue;
		this->getHuffmanTree();
		
		inputFile.close();
		inputFile.clear();

		//开始解码
		//先解析出除最后一个编码之外的所有编码
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
				{	//如果没有孩子，说明已经走到哈夫曼树的叶子节点，解码出一个字符，因此将其保存到解压文件缓冲区中，
					//并且将curNode重置为root，重新开始解码
					if (curNode->getWeight().ch == 10 && deFileBuffer[deFileCnt - 1] == 13)
					{	/*	如果新解压出的字符为LF并且解压缓冲区中前一个字符为CR，说明碰到了回车换行（CR LF），由于C运行库中会将
							LF自动转换为CR LF，因此需要去掉解压缓冲区中已保存的CR，避免重复，保证解压文件与源文件完全一致。
						*/
						deFileBuffer[deFileCnt - 1] = 10; //将前一个字符更改为LF
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

		//解析最后一个编码
		tmpCode = deCodeBuffer[deHuffmanCnt - 1];
		for (loc = MAX_INT_BIT_LENGTH - 1; loc >= (MAX_INT_BIT_LENGTH - deLastUseLength); loc--)
		{
			curBinaryBit = getPosBit(tmpCode, loc);
			if (! curNode->hasChildren())
			{	//如果没有孩子，说明已经走到哈夫曼树的叶子节点，解码出一个字符，因此将其保存到解压文件缓冲区中，
				//并且将curNode重置为root，重新开始解码
				deFileBuffer[deFileCnt] = curNode->getWeight().ch;
				++deFileCnt;
				curNode = root;
			}
			curNode = (curBinaryBit == 0)? curNode->getLeftNode():curNode->getRightNode();
		}

		//将最后一个字符写入到缓冲区中
		deFileBuffer[deFileCnt] = curNode->getWeight().ch;
		++deFileCnt;
		//在文件尾部加上NULL，表示文件结束，否则写入文件时文件尾部会出现乱码
		deFileBuffer[deFileCnt] = NULL;

		outputFile.open(decompress.c_str(), ios::trunc);
		if (!outputFile)
		{
			cout<<"解压缩文件打开失败"<<endl;
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
	作用：获取code第pos位的二进制数
	参数：	unsigned int code			保存Huffman编码的unsigned int数
				int pos							位数，取值范围为0~31
	返回值：code第pos位的二进制数
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
	作用：释放Huffman树所申请的内存空间
	参数： HuffmanNode * parent
	返回值：无
	*/
void HuffmanTree::destroyHuffmanTree(HuffmanNode * curNode)
{
	//以递归方式释放所有Huffman节点所申请的空间
	if (curNode->hasChildren())
	{
		destroyHuffmanTree(curNode->getLeftNode());
		destroyHuffmanTree(curNode->getRightNode());
		delete curNode;
	}else{
		//没有孩子，说明是叶子节点，直接删除即可
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
	cout<<"----------  === 时间统计 ===  ------------"<<endl;
	cout<<"读文件:"<<endl<<"\t开始时间："<<ctime(&startReadFileTime)<<"\t结束时间："<<ctime(&finishReadFileTime);
	cout<<"创建优先级队列:"<<endl<<"\t开始时间："<<ctime(&startQueueTime)<<"\t结束时间："<<ctime(&finishQueueTime);
	cout<<"优先级队列排序:"<<endl<<"\t开始时间："<<ctime(&startSortTime)<<"\t结束时间："<<ctime(&finishSortTime);
	cout<<"创建Huffman树:"<<endl<<"\t开始时间："<<ctime(&startTreeTime)<<"\t结束时间："<<ctime(&finishTreeTime);
	cout<<"创建映射表:"<<endl<<"\t开始时间："<<ctime(&startMapTime)<<"\t结束时间："<<ctime(&finishMapTime);
	cout<<"压缩:"<<endl<<"\t开始时间："<<ctime(&startCompressTime)<<"\t结束时间："<<ctime(&finishCompressTime);
	cout<<"解压:"<<endl<<"\t开始时间："<<ctime(&startDecompressTime)<<"\t结束时间："<<ctime(&finishDecompressTime);
	DISPLAY_LINE;
	cout<<"----------  === 空间统计 ===  ------------"<<endl;
	cout<<"压缩过程："<<endl;
	cout<<"\t 读取文件缓冲区大小："<<fileBufferSpace<<" bytes"<<endl;
	cout<<"\t Huffman编码缓冲区大小："<<huffmanCodeBufferSpace<<" * 4 bytes"<<endl;
	cout<<"解压过程："<<endl;
	cout<<"\t 读取文件缓冲区大小："<<deFileBufferSpace<<" bytes"<<endl;
	cout<<"\t Huffman编码缓冲区大小："<<deHuffmanCodeBufferSpace<<" * 4 bytes"<<endl;
	cout<<endl<<"压缩比（压缩文件大小/源文件大小，不计文件头部用于解压的辅助信息）："<<( (deHuffmanCodeBufferSpace * 4) * 1.0 / fileLength)<<endl;
}