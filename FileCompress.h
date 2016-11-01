#pragma once

#include"HuffmanTree.h"
#include<assert.h>
#include<algorithm>
#include<string>

typedef long long LongType;
struct CharInfo 
{
	unsigned char _ch;         //���ֵ��ַ�
	LongType _count;   //�ַ�����
	string _code;   //HuffmanCode

	CharInfo(LongType count=0)
		:_ch(0)
		,_count(count)  //_code�����ó�ʼ����string�����Ĭ�ϵĹ��캯��
	{}

	CharInfo operator+(const CharInfo& c)
	{
		CharInfo tmp;
		tmp._count=_count+c._count;
		return tmp;  
	}
	bool operator != (const CharInfo c)
	{
		return _count != c._count;
	}
	bool operator < (const CharInfo& c)const
	{
		return _count < c._count;
	}

};

class FileCompress
{
public:
	FileCompress()
	{
		for (int i=0;i<256;++i)
		{
			_str[i]._ch=i;
			//���ı��ļ��У����������ַ���ASCII�����ʽ��ţ�
			//ASCII��ķ�Χ��0-255�������ļ�ѹ������Ԫ�ظ���
			//Ϊ256��������Ϊ�ײ����ݽṹ������Ԫ������ΪCharInfo
			//�����ַ����ַ����ֵĴ������ַ����룻
		}
	}

	void Compress(const char* FileName)
	{
		assert(FileName);
		FILE* fout=fopen(FileName,"rb");  //�Զ�������ʽ�򿪣��������������
		assert(fout);

	
		//unsigned char ch=fgetc(fout);
		int ch=fgetc(fout);
		while(!feof(fout)) 
		{
			_str[ch]._count++;     //ͳ�Ƹ����ַ����ļ��г��ֵĴ���
			ch=fgetc(fout);	
		}

		//����ͳ�ƵĴ�����ΪȨֵ������������
		//count=0���൱��һ���Ƿ�ֵ�����ù�����huffmantree��
	   CharInfo invalid(0);
	   HuffmanTree<CharInfo> hf(_str,256,invalid);

	   //����Ҷ�ӽڵ�����Ӧ�ı���
	   string code;
	   _GenerateHuffmanCode(hf.GetRoot(),code);

	   //������д��ѹ���ļ���
	   string CompressFileName=FileName;
	   CompressFileName+=".compress";
	   FILE* Input=fopen(CompressFileName.c_str(),"wb");
	   assert(Input);
	   fseek(fout,0,SEEK_SET);
	   
	   char Inch=0;   //Ҫд��ѹ���ļ��ı���
	   int size=0;
	   ch=fgetc(fout);
	   while (!feof(fout))
	   {
		   string& code=_str[ch]._code;
		   for (size_t i=0;i < code.size();++i)
		   {
			    Inch<<=1; 
			   if (code[i]=='1')
			   {
				   Inch |= 1;
			   }
			   ++size;
			   if (size==8)
			   {
				   fputc(Inch,Input);  //��8���ֽھ�д��ѹ���ļ�
				   size=0;
				   Inch=0;
			   } 
		   }
		   ch=fgetc(fout);
	   }
	   //�����8���ֽڵĵ�������
	   if (size>0)
	   {
		   Inch<<= 8-size;
		   fputc(Inch,Input);
	   }

	   fclose(fout);   //�ر�Դ�ļ�
	   fclose(Input);  //�ر�ѹ���ļ�

	   //д�����ļ�
	   //��ʽ�磺
	   //a,3
	   //b,2
	   string ConfigFileName=FileName;
	   ConfigFileName+=".config";
	   FILE* finConfig=fopen(ConfigFileName.c_str(),"wb");
	   string line;
	   for (size_t i=0;i<256;++i)
	   {
		   if (_str[i]._count!=0)
		   {
			   line += _str[i]._ch;
			   line += ',';
			   char buff[25]={0};       //������ת�����ַ����洢
			   line += _itoa(_str[i]._count,buff,10); //10Ϊ���浽�ַ����е����ݵĽ��ƻ���
			   line +='\n';
			   fwrite(line.c_str(),1,line.size(),finConfig);
			   //fputs(line.c_str(),finConfig);
			   line.clear();
		   }
	   }
	   fclose(finConfig);   //�ر������ļ�
	}

	//��ѹ���ļ�
	void Uncompress(const char* filename)
	{	
		//�ȴ������ļ��ж��������ַ����ֵĸ���
		string ConfigFile = filename;
		ConfigFile += ".config";
		FILE* foutconfig = fopen(ConfigFile.c_str(), "rb");
		assert(foutconfig);
		string line;
		while (ReadLine(foutconfig,line))
		{
			//�������
			if(line.empty())
			{
				line += '\n';
				continue;
			}
			else 
			{
				unsigned char ch=line[0]; //ѹ��ͼƬʱ���������Է����и������Ҫ��unsigned char
				//char ch=line[0];
			    //ʹ��string::substr(pos)������ȡ�ַ����ֵĴ���
				_str[ch]._count = atoi(line.substr(2).c_str());
				line.clear();
			}
		}

		//���¹���Huffman��
		CharInfo invalid(0);
		HuffmanTree<CharInfo> hf(_str,256,invalid);

		//��ʼ��ѹ��
		string uncompressfilename=filename;
		uncompressfilename += ".uncompress";
		FILE *fin=fopen(uncompressfilename.c_str(),"wb");
		assert(fin);
		//�ȶ�ȡѹ���ļ�
		string compressfilename=filename;
		compressfilename += ".compress";
		FILE *fout=fopen(compressfilename.c_str(),"rb");
		assert(fout);

		//������Ȩֵ���ַ����ֵĴ����ܺ�
		HuffmanTreeNode<CharInfo> *root=hf.GetRoot();
		LongType chLen=root->_weight._count;
		HuffmanTreeNode<CharInfo> *cur=root;
		//int pos=8;
		int pos=7;
		//unsigned char ch=fgetc(fout);
		int ch=fgetc(fout);
		while (chLen>0)
		{
			//--pos;
			if (ch & (1 << pos))    //����ַ���ÿ��λ
				cur=cur->_right;    
		    else
				cur=cur->_left;     //0��1��
			--pos;

			if(cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch,fin); //����Ӧ�ַ�д���ѹ���ļ���
				cur=root;                   //һ��·�������ٴλص����ڵ�
				--chLen;
				if (chLen == 0)   //���е��ַ����������
					break;
			}

			//if(pos == 0)             
			//{
			//	ch=fgetc(fout);    //һ���ֽڽ�ѹ��ɣ��ٴλ�ȡ
			//	pos=8;
			//}
			if(pos==-1)
			{
				ch=fgetc(fout);
				pos=7;
			}
		}
		fclose(foutconfig);
		fclose(fin);
		fclose(fout);
	}
		
protected:
	void _GenerateHuffmanCode(HuffmanTreeNode<CharInfo>* root,string& code)
	{
		if (NULL==root)
		{
			return;
		}
		if (root->_left==NULL && root->_right==NULL)
		{
			_str[root->_weight._ch]._code=code;
		}
		_GenerateHuffmanCode(root->_left,code+'0');
		_GenerateHuffmanCode(root->_right,code+'1');
	}
	
	bool ReadLine(FILE *fOut, string& line)
	{
		//unsigned char ch = fgetc(fOut);
		int ch = fgetc(fOut);
		if (feof(fOut))
			return false;
		while (!feof(fOut) && ch !='\n')
		{
			line += ch;
			ch = fgetc(fOut);
		}
		return true;
	}
protected:
	CharInfo _str[256];
};















	//string line;
//		unsigned long long chLen = 0;
//		
//		
//
//
//		//�ؽ���������
//		CharInfo invalid;
//		HuffManTree<CharInfo> Tree(_info, 256, invalid);
//
//		//��ȡѹ���ļ�
//		string compressfilename = filename;
//		compressfilename += ".HuffMan";
//		FILE* FileCom = fopen(compressfilename.c_str(), "rb");
//		assert(FileCom);
//
//		string uncompressfilename = filename;
//		uncompressfilename += ".uncomp";
//		FILE* FileUnCom = fopen(uncompressfilename.c_str(), "wb");
//		assert(FileUnCom);
//
//		char ch = fgetc(FileCom);
//		HuffManTreeNode<CharInfo>* root = Tree.GetRoot();
//		HuffManTreeNode<CharInfo>* cur = root;
//
//		int pos = 8;
//		while (1)
//		{
//			if (cur->_left==NULL && cur->_right==NULL)
//			{
//				fputc(cur->_weight._ch, FileUnCom);
//				cur = root;
//				if (--chSize==0)
//				{
//					break;
//				}
//			}
//			--pos;
//			if (ch & (1<<pos))
//				cur = cur->_right;
//			else
//				cur = cur->_left;
//			if (pos==0)
//			{
//				ch = fgetc(FileCom);
//				pos = 8;
//			}
//		}
//
//		fclose(FileCom);
//		fclose(FileUnCom);
//		fclose(foutconfig);
//	}
//	}