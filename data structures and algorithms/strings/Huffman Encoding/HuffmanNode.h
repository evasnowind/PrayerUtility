#include <iostream>

using namespace std;

typedef struct WeightEntry{
	char ch;
	unsigned int num;
}WeightEntry;

class HuffmanNode
{
private:
	WeightEntry  weight;
	HuffmanNode * parent;
	HuffmanNode * left;
	HuffmanNode * right;
public:
	HuffmanNode();
	HuffmanNode(WeightEntry w);
	HuffmanNode(WeightEntry w, HuffmanNode * l, HuffmanNode * r);
	~HuffmanNode();

	WeightEntry getWeight();
	HuffmanNode * getParentNode();
	HuffmanNode * getLeftNode();
	HuffmanNode * getRightNode();
	void setWeight(WeightEntry data);
	void setWeight(char ch, int n);
	void setParent(HuffmanNode * p);
	void setLeftNode(HuffmanNode * l);
	void setRightNode(HuffmanNode * r);
	void incWeight();
	bool hasChildren();
/*	bool operator==(const HuffmanNode &node) const;*/
};