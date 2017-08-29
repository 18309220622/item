#define _CRT_SECURE_NO_WARNINGS 1

#include "FileCompress.h"
#include<windows.h>
#include "gtest/gtest.h" 
#include<fstream>
//#include"Heap.h"
//#include"HuffmanTree.h"

//先测建的堆
class TestHeap :public testing::Test
{
public:
	void SetUp()
	{
		int a[] = { 10, 11, 13, 12, 16, 18, 15, 17, 14, 19 };
		for (size_t i = 0; i < 10; ++i)
		{
			test_heap.Push(a[i]);
		}
	}

	void TearDown()
	{
		for (size_t i = 0; i < 10; ++i)
		{
			test_heap.Pop();
		}
	}
	Heap<int> test_heap;
};
 
TEST_F(TestHeap, TestLargeHeap)
{
	ASSERT_EQ(19, test_heap.Top());
	//test_heap.Pop();              //不能在测试中删除，会导致错误
	ASSERT_EQ(10, test_heap.Size());    
	ASSERT_FALSE(test_heap.Empty());	
}

//测HuffmanTree
class TestHuffmanTree :public testing::Test
{
public:
	void SetUp()
	{
		int a[] = { 0, 1, 2 };
		//int a[] = {0,1,2,3,4,5,6,7,8,9};
		test_huffman=new HuffmanTree<int>(a,20,'#');
	}
	void TearDown()
	{
		delete test_huffman;
	}
	HuffmanTree<int>* test_huffman;    //直接调析构函数会有风险
};
TEST_F(TestHuffmanTree,HuffmanTestCase)
{
	ASSERT_EQ(3, test_huffman->GetRoot()->_weight);
	//ASSERT_EQ(45, test_huffman->GetRoot()->_weight);	
}


//测试压缩文件
TEST(TestCompress, CompressCase) {
	FileCompress com;
	//string CompressFile = com.Compress("a.txt");
  string CompressFile = com.Compress("at.BIG");
	ASSERT_GT(CompressFile.size(), 1000);
}

//测试解压缩（对原文件和解压缩后的文件进行一行一行的对比）
bool Compare(const std::string& FileName, const std::string& UnCompressFile)
{
	std::ifstream fin(FileName.c_str());        //ifstream是从硬盘到内存
	std::ifstream fout(UnCompressFile.c_str());
	std::string fin_line;
	std::string fout_line;
	while (std::getline(fin, fin_line) && std::getline(fout, fout_line))
	{
		if (fin_line != fout_line)
			return false;
	}
	//判断两个文件是否都读到尾(如果两个文件每一行的内容都相同且都读到结尾了那么就返回true)
	if (fin.eof() && fout.eof())
		return true;
}
TEST(TestUnCompress,UnCompressCase)
{
	FileCompress com;
	//string CompressFile = com.Compress("a.txt");
  string CompressFile = com.Compress("at.BIG");
	string tmpFile=com.Uncompress(CompressFile.c_str());
	//bool ret = Compare("a.txt",tmpFile);
  bool ret = Compare("at.BIG",tmpFile);
	ASSERT_TRUE(ret);
}
int main(int argc, char* argv[])
{
	//多个测试用例时使用，如果不写，运行RUN_ALL_TESTS()时会全部测试，加上则只返回对应的测试结果  
	//testing::GTEST_FLAG(filter) = "test_case_name.test_name";
	//测试初始化
	testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
	RUN_ALL_TESTS();
	return  0;
}


//void test()
//{
//	FileCompress f;
//	cout << "开始压缩" << endl;
//	cout << "压缩用时:";
//	int start = GetTickCount();
//	//f.Compress("a.txt");
//	//f.Compress("input");
//	//f.Compress("at.BIG");
//	//f.Compress("01.jpg");
//	f.Compress("jq.png");
//	int end = GetTickCount();
//	cout << end - start << endl;
//	cout << "开始解压" << endl;
//	cout << "解缩用时:";
//	start = GetTickCount();
//	//f.Uncompress("a.txt");
//	//f.Uncompress("input");
//	//f.Uncompress("at.BIG");
//	//f.Uncompress("01.jpg");
//	f.Uncompress("jq.png");
//	end = GetTickCount();
//	cout << end - start << endl;
//
//}
//int main()
//{
//	test();
//	system("pause");
//	return 0;
//}
