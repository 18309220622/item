#pragma once

#include"HuffmanTree.h"
#include<assert.h>
#include<algorithm>
#include<string>

typedef long long LongType;
struct CharInfo 
{
	unsigned char _ch;         //出现的字符
	LongType _count;   //字符个数
	string _code;   //HuffmanCode

	CharInfo(LongType count=0)
		:_ch(0)
		,_count(count)  //_code不需用初始化，string会调用默认的构造函数
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
			//在文本文件中，数据是以字符的ASCII码的形式存放，
			//ASCII码的范围是0-255，所以文件压缩中用元素个数
			//为256的数组作为底层数据结构，其中元素类型为CharInfo
			//包括字符，字符出现的次数，字符编码；
		}
	}

	void Compress(const char* FileName)
	{
		assert(FileName);
		FILE* fout=fopen(FileName,"rb");  //以二进制形式打开，否则读不到汉字
		assert(fout);

	
		//unsigned char ch=fgetc(fout);
		int ch=fgetc(fout);
		while(!feof(fout)) 
		{
			_str[ch]._count++;     //统计各种字符在文件中出现的次数
			ch=fgetc(fout);	
		}

		//根据统计的次数作为权值构建哈弗曼树
		//count=0的相当于一个非法值，不用构建到huffmantree中
	   CharInfo invalid(0);
	   HuffmanTree<CharInfo> hf(_str,256,invalid);

	   //生成叶子节点所对应的编码
	   string code;
	   _GenerateHuffmanCode(hf.GetRoot(),code);

	   //将编码写入压缩文件中
	   string CompressFileName=FileName;
	   CompressFileName+=".compress";
	   FILE* Input=fopen(CompressFileName.c_str(),"wb");
	   assert(Input);
	   fseek(fout,0,SEEK_SET);
	   
	   char Inch=0;   //要写入压缩文件的编码
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
				   fputc(Inch,Input);  //满8个字节就写入压缩文件
				   size=0;
				   Inch=0;
			   } 
		   }
		   ch=fgetc(fout);
	   }
	   //最后不满8个字节的单独处理
	   if (size>0)
	   {
		   Inch<<= 8-size;
		   fputc(Inch,Input);
	   }

	   fclose(fout);   //关闭源文件
	   fclose(Input);  //关闭压缩文件

	   //写配置文件
	   //格式如：
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
			   char buff[25]={0};       //将次数转换成字符串存储
			   line += _itoa(_str[i]._count,buff,10); //10为保存到字符串中的数据的进制基数
			   line +='\n';
			   fwrite(line.c_str(),1,line.size(),finConfig);
			   //fputs(line.c_str(),finConfig);
			   line.clear();
		   }
	   }
	   fclose(finConfig);   //关闭配置文件
	}

	//解压缩文件
	void Uncompress(const char* filename)
	{	
		//先从配置文件中读出各种字符出现的个数
		string ConfigFile = filename;
		ConfigFile += ".config";
		FILE* foutconfig = fopen(ConfigFile.c_str(), "rb");
		assert(foutconfig);
		string line;
		while (ReadLine(foutconfig,line))
		{
			//空行情况
			if(line.empty())
			{
				line += '\n';
				continue;
			}
			else 
			{
				unsigned char ch=line[0]; //压缩图片时崩溃，调试发现有负数情况要用unsigned char
				//char ch=line[0];
			    //使用string::substr(pos)函数提取字符出现的次数
				_str[ch]._count = atoi(line.substr(2).c_str());
				line.clear();
			}
		}

		//重新构建Huffman树
		CharInfo invalid(0);
		HuffmanTree<CharInfo> hf(_str,256,invalid);

		//开始解压缩
		string uncompressfilename=filename;
		uncompressfilename += ".uncompress";
		FILE *fin=fopen(uncompressfilename.c_str(),"wb");
		assert(fin);
		//先读取压缩文件
		string compressfilename=filename;
		compressfilename += ".compress";
		FILE *fout=fopen(compressfilename.c_str(),"rb");
		assert(fout);

		//根结点的权值是字符出现的次数总和
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
			if (ch & (1 << pos))    //检查字符的每个位
				cur=cur->_right;    
		    else
				cur=cur->_left;     //0左，1右
			--pos;

			if(cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch,fin); //将对应字符写入解压缩文件中
				cur=root;                   //一条路径走完再次回到根节点
				--chLen;
				if (chLen == 0)   //所有的字符都处理完成
					break;
			}

			//if(pos == 0)             
			//{
			//	ch=fgetc(fout);    //一个字节解压完成，再次获取
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
//		//重建哈夫曼树
//		CharInfo invalid;
//		HuffManTree<CharInfo> Tree(_info, 256, invalid);
//
//		//读取压缩文件
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