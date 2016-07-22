#include "HuffmanNode.h"

HuffmanNode::HuffmanNode()
{
	parent = NULL;
	left = NULL;
	right = NULL;
}

HuffmanNode::HuffmanNode(WeightEntry w)
{
	weight = w;
	parent = NULL;
	left = NULL;
	right = NULL;
}

HuffmanNode::~HuffmanNode()
{

}

HuffmanNode::HuffmanNode(WeightEntry w, HuffmanNode * l, HuffmanNode * r)
{
	weight= w;
	left = l;
	right = r;
	parent = NULL;
}

void HuffmanNode::setWeight(WeightEntry data)
{
	weight = data;
}

void HuffmanNode::setParent(HuffmanNode * p)
{
	parent = p;
}

void HuffmanNode::setLeftNode(HuffmanNode * l)
{
	left = l;
}

void HuffmanNode::setRightNode(HuffmanNode * r)
{
	right = r;
}

HuffmanNode * HuffmanNode::getParentNode()
{
	return parent;
}

HuffmanNode * HuffmanNode::getLeftNode()
{
	return left;
}

HuffmanNode * HuffmanNode::getRightNode()
{
	return right;
}

WeightEntry HuffmanNode::getWeight()
{
	return weight;
}

void HuffmanNode::incWeight()
{
	weight.num++;
}

void HuffmanNode::setWeight(char ch, int n)
{
	weight.ch = ch;
	weight.num = n;
}

bool HuffmanNode::hasChildren()
{
	if (left != NULL && right != NULL)
	{
		return true;
	}else{
		return false;
	}
}

// bool HuffmanNode::operator==(const HuffmanNode &node) const
// {
// 	return ( weight.num == node.getWeight().num);
/*}*/