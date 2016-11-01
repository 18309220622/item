#pragma  once

#include"Heap.h"
#include<string>
#include<iostream>
using namespace std;

template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;
	T _weight;    //权值

	HuffmanTreeNode(const T& x)
		:_left(NULL)
		,_right(NULL)
		,_parent(NULL)
		,_weight(x)
	{}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}
	HuffmanTree(T* a,size_t size,const T& invalid)
	{
		assert(a);
		struct NodeLess 
		{
			bool operator()(const Node* l,const Node* r)
			{
				return l->_weight < r->_weight;
			}
		};
		Heap<Node*,NodeLess> MinHeap;
		for (size_t i=0;i<size;++i)
		{
			if (a[i]!=invalid)
			{
				MinHeap.Push(new Node(a[i]));
			}
		}
		while (MinHeap.Size() > 1)
		{
			Node* left=MinHeap.Top();
			MinHeap.Pop();
			Node* right=MinHeap.Top();
			MinHeap.Pop();

			//左右两节点权值相加构建父节点
			Node* root=new Node(left->_weight + right->_weight);
			root->_left = left;
			root->_right = right;
			left->_parent=root;
			right->_parent=root;

			MinHeap.Push(root);    //将节点和放入MinHeap重新调整
		}
		_root=MinHeap.Top();
	}

	Node* GetRoot()
	{
		return _root;
	}
protected:
	Node* _root;
};

