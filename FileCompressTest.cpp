#define _CRT_SECURE_NO_WARNINGS 1

#include "FileCompress.h"
#include<windows.h>

void test()
{
	FileCompress f;
	cout<<"��ʼѹ��"<<endl;
	cout<<"ѹ����ʱ:";
	int start=GetTickCount();
    //f.Compress("a.txt");
	//f.Compress("input");
	//f.Compress("at.BIG");
	//f.Compress("01.jpg");
	f.Compress("jq.png");
	int end=GetTickCount();
	cout<<end-start<<endl;
	cout<<"��ʼ��ѹ"<<endl;
	cout<<"������ʱ:";
	start=GetTickCount();
	//f.Uncompress("a.txt");
	//f.Uncompress("input");
	//f.Uncompress("at.BIG");
	//f.Uncompress("01.jpg");
	f.Uncompress("jq.png");
	end=GetTickCount();
	cout<<end-start<<endl;

}
int main()
{
	test();
	system("pause");
	return 0;
}