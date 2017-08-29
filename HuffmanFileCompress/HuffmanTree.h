#pragma  once

#include"Heap.h"
#include<string>
#include<iostream>
using namespace std;

template<typename T>
struct HuffmanNode
{
	T _weight;
	HuffmanNode<T>* _left;
	HuffmanNode<T>* _right;
	HuffmanNode<T>* _parent;
	HuffmanNode(const T& weight = T())
		:_weight(weight)
		,_left(NULL)
		,_right(NULL)
		,_parent(NULL)
	{}
};

template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;
	T _weight;    //权值

	HuffmanTreeNode(const T& x)
		:_left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _weight(x)
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
	HuffmanTree(T* a, size_t size, const T& invalid)
	{
		assert(a);
		struct NodeLess
		{
			bool operator()(const Node* l, const Node* r)
			{
				return l->_weight < r->_weight;
			}
		};
		//将这组数据建成一个最小堆，堆中元素的类型是Node*,这是为了保存后面结点的父节点 
		Heap<Node*, NodeLess> MinHeap;
		for (size_t i = 0; i<size; ++i)
		{
			if (a[i] != invalid)
			{
				MinHeap.Push(new Node(a[i]));
			}
		}
		//用huffman算法，从堆里面取出最小的两个结点并删除，将这两个结点构成一棵树在插入堆中  
		while (MinHeap.Size() > 1)
		{
			Node* left = MinHeap.Top();
			MinHeap.Pop();
			Node* right = MinHeap.Top();
			MinHeap.Pop();

			//左右两节点权值相加构建父节点
			Node* root = new Node(left->_weight + right->_weight);
			root->_left = left;
			root->_right = right;
			left->_parent = root;
			right->_parent = root;

			MinHeap.Push(root);    //将节点和放入MinHeap重新调整
		}
		_root = MinHeap.Top();
	}

	Node* GetRoot()
	{
		return _root;
	}

	HuffmanTree<T>& operator=(HuffmanTree<T>& ht)
	{
		if (&ht != this)
		{
			swap(_root,ht._root);
		}
		return *this;
	}

	~HuffmanTree()
	{
		if (_root != NULL)
		{
			Destory(_root);
		}
	}
protected:
	void Destory(Node* root)
	{
		if (root == NULL)
			return;
		Node* cur = root;
		Destory(cur->_left);
		Destory(cur->_right);
		delete cur;
		cur = NULL;
	}

	Node* _root;
};
