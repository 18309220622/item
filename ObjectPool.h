#pragma once

#include<vector>
#include<string>
#include<cstdlib>
#include<iostream>
using namespace std;

//������ʵ���ڴ�أ�ÿ���ڵ������һ���ڴ�
template<class T>
class ObjPool
{
	struct BlockNode
	{
		void* _memory;   //�ڴ��
		BlockNode* _next; //ָ����һ���ڵ��ָ��
		size_t _ObjNum;    //�ڴ����ĸ���

		BlockNode(size_t ObjNum)
			:_ObjNum(ObjNum)
			,_next(NULL)
		{
			_memory=malloc(_ItemSize*_ObjNum); //��������Ĵ�С���ڴ����ĸ����ĳ˻�
			if(_memory==NULL)
			{
				cout<<"out of memory"<<endl;
				exit(1);
			}
		}

		~BlockNode()
		{
			free(_memory);  //_memory��ͨ��malloc�ڶ��Ͽ��ٵ�
			_memory=NULL;
			_next=NULL;
			_ObjNum=0;
		}
	};
public:
	//�̶���С���ڴ��
	ObjPool(size_t InitNum=32,size_t MaxNum=100000)//һ���ڵ�������Ĭ�Ͽ�32�ֽڴ�С���ڴ�
		:_CountIn(0)
		,_MaxNum(MaxNum)
		,_LastDelete(NULL)
	{
		_first=_last=new BlockNode(InitNum);
	}
	~ObjPool()
	{
		//�ͷŽڵ�����ҵ��ڴ��
		BlockNode* cur=_first;
		while(cur)
		{
			BlockNode* del=cur;
			cur=cur->_next;
			delete del;   //���ýڵ����������
			del=NULL;
		}
		_first=_last=NULL;
		_CountIn=0;
	}
	//����ռ�
	T* New()
	{
		//1.����ʹ����ǰ�ͷŵĿռ�
		//2.ȥ�ڴ��������
		//3.�����µĽڵ����
		
		
		 
		if(_LastDelete)
		{
			T* obj=_LastDelete;
            //ǿתΪ����ָ���ٽ����þ�����ȷ��ȡ����ָ����ָ���ַ���ݣ�
			//�����32λ�����64Ϊ���������
			_LastDelete=*((T**)_LastDelete);
			//ʹ�õ��ǻ��������ڴ棬����û��Ҫ�ٶԵ�ǰ�ڵ����ü���_CountIn++
			return new(obj)T; //new��λ���ʽ
		}
		//���н����ڴ�鶼û�п���ʹ�õģ������¿����µĽڵ�
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
			_CountIn=0; //�¿��Ľڵ㣬��ǰ�ڵ����ü�����Ϊ0
			_last=_last->_next;
		}

		//�ڻ�û�õ��ڴ������ȥ����ʹ��
		T* obj = (T*)((char*)_last->_memory + _CountIn*_ItemSize);
		_CountIn++;
		return new(obj)T;
	}

	//ʹ����С���ڴ�󻹸��ڴ��ʱ��Delete�ȵ����������������Ȼ������ڴ�ͷ��������������
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
	//BlockNode�д洢��void* ��һ��ָ��,��������޶�Ҫ����һ���ܴ��void*ָ���С�Ķ���ռ�
		if(sizeof(void*) >= sizeof(T))
			return sizeof(void*);
		else
			return sizeof(T);
	}
protected:
	size_t _CountIn;   //��ǰ�ڵ����õļ���
	BlockNode* _first; //ָ�������ͷ
	BlockNode* _last; //ָ�������β
	size_t _MaxNum;  //�ڵ������ڴ��Ķ������
	static size_t _ItemSize;  //��������Ĵ�С
	T* _LastDelete;       //ָ������ͷŵĿռ�
};
template<class T>
size_t ObjPool<T>::_ItemSize=ObjPool<T>::Get_ItemSize(); //������þ�̬��Ա����