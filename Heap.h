#pragma once

#include<iostream>
#include<assert.h>
#include<vector>
using namespace std;


template<class T>
struct Less
{
	bool operator()(const T& l,const T& r)
	{
		return l<r;
	}
};

template<class T>
struct Larger
{
	bool operator()(const T& l,const T& r)
	{
		return l>r;
	}
};

template<class T,class compare=Larger<T>>
class Heap
{
public:
	Heap()
	{}
	Heap(T* a,size_t size)
	{
		assert(a);
		_a.reserve(size);
		for (size_t i=0;i<size;++i)
		{
			_a.push_back(a[i]);
		}

		//建堆，从第一个非叶子节点的父节点开始调整
		for (int i=(size-2)/2;i>=0;--i)   
		{
			AdjustDown(i);
		}
	}

	void Push(const T& d)
	{
		_a.push_back(d);
		AdjustUp(_a.size()-1);
	}

	void Pop()
	{
		assert(!_a.empty());
		std::swap(_a[0],_a[_a.size()-1]);
		_a.pop_back();
		AdjustDown(0);
	}

	const T& Top()
	{
		assert(!_a.empty());
		return _a[0];
	}

	bool Empty()
	{
		return _a.empty();
	}

	size_t Size()
	{
		return _a.size();
	}

	void Print()
	{
		for (size_t i=0;i<_a.size();++i)
		{
			cout<<_a[i]<<" ";
		}
		cout<<endl;
	}
protected:
	void AdjustDown(size_t root)
	{
		compare com;
		size_t parent=root;
		size_t child=parent*2+1;   //先让child指向左孩子
		while (child<_a.size())
		{
			//保证右孩子不为空,比较左右孩子
			if (child+1<_a.size() && com(_a[child+1],_a[child]))
			{
				++child;
			}
			if (com(_a[child],_a[parent]))
			{
				std::swap(_a[parent],_a[child]);
				parent=child;
				child=parent*2+1;
			}
			else
				break;
		}
	}

	void AdjustUp(size_t child)
	{
		while (child>0)
		{
			compare com;
			size_t parent=(child-1)/2;
			if (com(_a[child],_a[parent]))
			{
				std::swap(_a[parent],_a[child]);
				child=parent;
			}
			else
				break;
		}
	}
protected:
	vector<T> _a;
};
