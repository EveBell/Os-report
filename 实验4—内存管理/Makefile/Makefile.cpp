# include <fstream>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
#pragma warning(disable:4996)��


struct operation
{
	int time;       //��ʼʱ��
	int block;      //�ڴ�ҳ��
	int oper;       //����
	int protection; //Ȩ��
};

int main()
{
	FILE* file;
	file = fopen("opfile", "wb");  
	operation op;

	for (int j = 0; j < 6; j++)    //0--������1--�ύ��2--����3--������4--���գ�5--�ͷ�
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
