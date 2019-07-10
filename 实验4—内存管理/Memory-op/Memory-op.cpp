# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <iostream>
# include <fstream>
using namespace std;
#pragma warning(disable:4996)��


struct operation
{
	int time;    //��ʼʱ��
	int block;   //�ڴ�ҳ��
	int oper;    //����
	int protection;  //Ȩ��
};

struct trace
{
	LPVOID start; 
	long size;    
};

HANDLE allo, trac; 

//����allocator�̵߳��ڴ���Ϊ�������
DWORD Tracker(LPDWORD lpdwparm) 
{
	ofstream outfile; 
	outfile.open("out.txt");

	for (int i = 0; i <= 30; i++)
	{
		WaitForSingleObject(trac, INFINITE);
		outfile << i << endl;

		SYSTEM_INFO info;  //ϵͳ��Ϣ
		GetSystemInfo(&info);
		outfile << "dwActiveProcessorMask" << "\t" << info.dwActiveProcessorMask << endl;
		outfile << "dwAllocationGranularity" << "\t" << info.dwAllocationGranularity << endl;
		outfile << "dwNumberOfProcessors" << "\t" << info.dwNumberOfProcessors << endl;
		outfile << "dwOemId" << "\t" << info.dwOemId << endl;
		outfile << "dwPageSize" << "\t" << info.dwPageSize << endl;
		outfile << "dwProcessorType" << "\t" << info.dwProcessorType << endl;
		outfile << "lpMaximumApplicationAddress" << "\t" << info.lpMaximumApplicationAddress << endl;
		outfile << "lpMinimumapplicationAddress" << "\t" << info.lpMinimumApplicationAddress << endl;
		outfile << "wProcessorArchitecture" << "\t" << info.wProcessorArchitecture << endl;
		outfile << "wProcessorLevel" << "\t" << info.wProcessorLevel << endl;
		outfile << "wProcessorRevision" << "\t" << info.wProcessorRevision << endl;
		outfile << "wReserved" << "\t" << info.wReserved << endl;
		outfile << "************************************************************" << endl;


		//�ڴ�״��
		MEMORYSTATUS status; 
		GlobalMemoryStatus(&status);
		outfile << "dwAvailPageFile" << "\t" << status.dwAvailPageFile << endl;
		outfile << "dwAvailPhys" << "\t" << status.dwAvailPhys << endl;
		outfile << "dwAvailVirtual" << "\t" << status.dwAvailVirtual << endl;
		outfile << "dwLength" << "\t" << status.dwLength << endl;
		outfile << "dwMemoryLoad" << "\t" << status.dwMemoryLoad << endl;
		outfile << "dwTotalPageFile" << "\t" << status.dwTotalPageFile << endl;
		outfile << "dwTotalPhy" << "\t" << status.dwTotalPhys << endl;
		outfile << "dwTotalVirtual" << "\t" << status.dwTotalVirtual << endl;
		outfile << "**************************************************" << endl;

		MEMORY_BASIC_INFORMATION mem; //�ڴ������Ϣ
		VirtualQuery(info.lpMinimumApplicationAddress, &mem, sizeof(MEMORY_BASIC_INFORMATION));
		outfile << "AllocationBase" << "\t" << mem.AllocationBase << endl;
		outfile << "AllocationProtect" << "\t" << mem.AllocationProtect << endl;
		outfile << "BaseAddress" << "\t" << mem.BaseAddress << endl;
		outfile << "Protect" << "\t" << mem.Protect << endl;
		outfile << "RegionSize" << "\t" << mem.RegionSize << endl;
		outfile << "State" << "\t" << mem.State << endl;
		outfile << "Type" << "\t" << mem.Type << endl;
		outfile << "*********************************************************" << endl;

		//�ͷ��ź���
		ReleaseSemaphore(allo, 1, NULL);
	}
	return 0;
}
//ģ���ڴ�������߳�
void Allocator() 
{
	trace traceArray[5];
	int index = 0;
	FILE* file;
	file = fopen("opfile", "rb");
	operation op;
	SYSTEM_INFO info;
	DWORD temp;
	GetSystemInfo(&info);
	for (int i = 0; i < 30; i++)
	{
		WaitForSingleObject(allo, INFINITE);
		cout << i << ":";
		fread(&op, sizeof(operation), 1, file);
		Sleep(op.time); //ִ��ʱ��
		GetSystemInfo(&info);
		switch (op.protection)
		{
		case 0:
		{
			index = 0;
			temp = PAGE_READONLY;
			break;
		}
		case 1:
			temp = PAGE_READWRITE;
			break;
		case 2:
			temp = PAGE_EXECUTE;
			break;
		case 3:
			temp = PAGE_EXECUTE_READ;
			break;
		case 4:
			temp = PAGE_EXECUTE_READWRITE;
			break;
		default:
			temp = PAGE_READONLY;
		}
		switch (op.oper)
		{
		case 0:  //����
		{
			cout << "reserve now " << endl;
			traceArray[index].start = VirtualAlloc(NULL, op.block * info.dwPageSize,
				MEM_RESERVE, PAGE_NOACCESS);
			traceArray[index++].size = op.block * info.dwPageSize;
			cout << "starting address:" << traceArray[index - 1].start << "\t\t" << "size:"
				<< traceArray[index - 1].size << endl;
			break;
		}
		case 1: //�ύ
		{
			cout << "commit now " << endl;
			traceArray[index].start = VirtualAlloc(traceArray[index].start,
				traceArray[index].size, MEM_COMMIT, temp);
			index++;
			cout << "starting address:" << traceArray[index - 1].start << "\t\t" << "size:"
				<< traceArray[index - 1].size << endl;
			break;
		}
		case 2: //��ס
		{
			cout << "lock now" << endl;
			cout << "starting address:" << traceArray[index].start << "\t\t" << "size:" <<
				traceArray[index].size << endl;
			if (!VirtualLock(traceArray[index].start, traceArray[index++].size))
				cout << GetLastError() << endl; 
			break;
		}
		case 3: //����
		{
			cout << "unlock now" << endl;
			cout << "starting address:" << traceArray[index].start << "\t\t" << "size:"
				<< traceArray[index].size << endl;
			if (!VirtualUnlock(traceArray[index].start, traceArray[index++].size))
				cout << GetLastError() << endl;
			break;
		}
		case 4: //����
		{
			cout << "decommit now" << endl;
			cout << "starting address:" << traceArray[index].start << "\t\t"
				<< "size:" << traceArray[index].size << endl;
			if (!VirtualFree(traceArray[index].start, traceArray[index++].size,
				MEM_DECOMMIT))
				cout << GetLastError() << endl;
			break;
		}
		case 5: //�ͷ�
		{
			cout << "release now" << endl;
			cout << "starting address:" << traceArray[index].start << "\t\t" <<
				"size:" << traceArray[index].size << endl;
			if (!VirtualFree(traceArray[index++].start, 0, MEM_RELEASE))
				cout << GetLastError() << endl;
			break;
		}
		default:
			cout << "error" << endl;
		}
		ReleaseSemaphore(trac, 1, NULL); //�ͷ��ź���
	}
}
//������
int main()
{
	DWORD dwThread;
	HANDLE handle[2];
	handle[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Tracker, NULL, 0,
		&dwThread);
	handle[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Allocator, NULL, 0,
		&dwThread);
	allo = CreateSemaphore(NULL, 0, 1, "allo");//�ź���
	trac = CreateSemaphore(NULL, 1, 1, "trac");//�ź���

	WaitForMultipleObjects(2, handle, TRUE, INFINITE);
	return 0;
}
