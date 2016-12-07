#pragma once

#include<vector>
#include<string>
#include<cstdlib>
#include<iostream>
using namespace std;

//用链表实现内存池，每个节点下面挂一块内存
template<class T>
class ObjPool
{
	struct BlockNode
	{
		void* _memory;   //内存块
		BlockNode* _next; //指向下一个节点的指针
		size_t _ObjNum;    //内存对象的个数

		BlockNode(size_t ObjNum)
			:_ObjNum(ObjNum)
			,_next(NULL)
		{
			_memory=malloc(_ItemSize*_ObjNum); //单个对象的大小与内存对象的个数的乘积
			if(_memory==NULL)
			{
				cout<<"out of memory"<<endl;
				exit(1);
			}
		}

		~BlockNode()
		{
			free(_memory);  //_memory是通过malloc在堆上开辟的
			_memory=NULL;
			_next=NULL;
			_ObjNum=0;
		}
	};
public:
	//固定大小的内存池
	ObjPool(size_t InitNum=32,size_t MaxNum=100000)//一个节点下面先默认开32字节大小的内存
		:_CountIn(0)
		,_MaxNum(MaxNum)
		,_LastDelete(NULL)
	{
		_first=_last=new BlockNode(InitNum);
	}
	~ObjPool()
	{
		//释放节点下面挂的内存块
		BlockNode* cur=_first;
		while(cur)
		{
			BlockNode* del=cur;
			cur=cur->_next;
			delete del;   //调用节点的析构函数
			del=NULL;
		}
		_first=_last=NULL;
		_CountIn=0;
	}
	//申请空间
	T* New()
	{
		//1.优先使用以前释放的空间
		//2.去内存块中申请
		//3.申请新的节点对象
		
		
		 
		if(_LastDelete)
		{
			T* obj=_LastDelete;
            //强转为二级指针再解引用就能正确的取到该指针所指向地址内容，
			//解决了32位程序和64为程序的限制
			_LastDelete=*((T**)_LastDelete);
			//使用的是还回来的内存，所以没必要再对当前节点在用计数_CountIn++
			return new(obj)T; //new定位表达式
		}
		//所有结点的内存块都没有可以使用的，则重新开辟新的节点
		if(_CountIn==_last->_ObjNum)
		{
			size_t NewNodeObjNum=_last->_ObjNum * 2;
			if(NewNodeObjNum>=_MaxNum)
				_MaxNum=NewNodeObjNum;

			_last->_next=new BlockNode(NewNodeObjNum);
			if(_last->_next==NULL)
			{
				throw(-1);
			}
			_CountIn=0; //新开的节点，当前节点在用计数置为0
			_last=_last->_next;
		}

		//在还没用的内存块下面去分配使用
		T* obj = (T*)((char*)_last->_memory + _CountIn*_ItemSize);
		_CountIn++;
		return new(obj)T;
	}

	//使用完小块内存后还给内存池时，Delete先调对象的析构函数，然后将这块内存头插入自由链表中
	void Delete(T* ptr)
	{
		if(ptr)
		{
			ptr->~T();
		}
		*(T**)ptr=_LastDelete;
		_LastDelete=ptr;
	}

	static size_t Get_ItemSize()
	{
	//BlockNode中存储了void* 的一个指针,所以最低限度要开出一个能存放void*指针大小的对象空间
		if(sizeof(void*) >= sizeof(T))
			return sizeof(void*);
		else
			return sizeof(T);
	}
protected:
	size_t _CountIn;   //当前节点在用的计数
	BlockNode* _first; //指向链表的头
	BlockNode* _last; //指向链表的尾
	size_t _MaxNum;  //节点申请内存块的对象个数
	static size_t _ItemSize;  //单个对象的大小
	T* _LastDelete;       //指向最后释放的空间
};
template<class T>
size_t ObjPool<T>::_ItemSize=ObjPool<T>::Get_ItemSize(); //类外调用静态成员函数