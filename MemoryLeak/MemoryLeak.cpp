// MemoryLeak.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MemoryLeak.h"
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

long g_x = 0;
CRITICAL_SECTION g_cs;
using namespace std;
namespace ThreadTest
{
	DWORD WINAPI ThreadFunc1 (PVOID pvParam)
	{
		for (int i = 0;i<50000;i++)
		{
			++g_x;
		}
		return 0;
	}

	DWORD WINAPI ThreadFunc2 (PVOID pvParam)
	{
		for (int i = 0;i<50000;i++)
		{
			EnterCriticalSection(&g_cs);
			++g_x;
			LeaveCriticalSection(&g_cs);
		}
		char str[] ="- This, a sample string.";
		char * pch;
		printf ("Splitting string \"%s\" into tokens:\n",str);
		pch = strtok (str," ,.-");
		free(malloc(1000));
		delete(new int);
		return 0;
	}
	UINT WINAPI ThreadFunc3 (PVOID pvParam)
	{
		for (int i = 0;i<500000;i++)
		{
			EnterCriticalSection(&g_cs);
			++g_x;
			LeaveCriticalSection(&g_cs);
		}	
		// 	cout<<(TCHAR*)TEXT("线程id")<<GetCurrentThreadId()<<endl;
		delete(new int);
		printf ("%x\n",GetCurrentThreadId());
		_endthreadex( 0 );
		return 0;
	}
}

// 唯一的应用程序对象

CWinApp theApp;



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	_CrtDumpMemoryLeaks();

	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
			InitializeCriticalSection(&g_cs);
			int x = 0;
			UINT nThreadId1,nThreadId2;
			DWORD dwThreadId1,dwThreadId2 ;
			HANDLE hThread1, hThread2, hThread3, hThread4;
			// 	hThread1 = CreateThread(NULL,0,ThreadTest::ThreadFunc1,(PVOID)&x, 0, &dwThreadId1);
// 			hThread2 = CreateThread(NULL,0,ThreadTest::ThreadFunc2,(PVOID)&x, 0, &dwThreadId2);
// 			WaitForSingleObject( hThread2, INFINITE );

//		 	CloseHandle(hThread1);
// 			CloseHandle(hThread2);
// 			malloc(100);
			hThread3 =(HANDLE) _beginthreadex(NULL,0,&ThreadTest::ThreadFunc3,(PVOID)&x,0 , &nThreadId1);
// 			WaitForSingleObject( hThread3, INFINITE );
// 			CloseHandle(hThread3);
			hThread4 =(HANDLE) _beginthreadex(NULL,0,&ThreadTest::ThreadFunc3,(PVOID)&x,0 , &nThreadId2);
			HANDLE hThreads[]= {hThread3,hThread4};
// 			cout << TryEnterCriticalSection(&g_cs)<<endl;
// 			LeaveCriticalSection(&g_cs);
			WaitForMultipleObjects(2,hThreads,TRUE,INFINITE);
			
// 			CloseHandle(hThread4);
			std::cout << g_x << std::endl;
			DeleteCriticalSection(&g_cs);//删除临界区
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}
	return nRetCode;
}
