# include <fstream>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
#pragma warning(disable:4996)；


struct operation
{
	int time;       //起始时间
	int block;      //内存页数
	int oper;       //操作
	int protection; //权限
};

int main()
{
	FILE* file;
	file = fopen("opfile", "wb");  
	operation op;

	for (int j = 0; j < 6; j++)    //0--保留，1--提交，2--锁，3--解锁，4--回收，5--释放
		for (int i = 0; i < 5; i++)
		{
			op.time = rand() % 1000; 
			op.block = rand() % 5 + 1; 
			op.oper = j;
			op.protection = i;
			fwrite(&op, sizeof(operation), 1, file); 
		}
	return 0;
}
