// TestACP.cpp --- Test for QueueUserAPC and threads
#include <windows.h>
#include <process.h>    // for _beginthreadex
#include <stdio.h>      // for printf

BOOL g_bTerminateAll = FALSE;

unsigned __stdcall thread_proc(void *)
{
    while (!g_bTerminateAll)
    {
        printf("thread_proc: sleep\n");
        SleepEx(INFINITE, TRUE);
        printf("thread_proc: awake\n");
    }
    return 0;
}

VOID NTAPI DoUserAPC(ULONG_PTR Parameter)
{
    printf("DoUserAPC(%p)\n", Parameter);
}

VOID NTAPI TerminateAPC(ULONG_PTR Parameter)
{
    printf("TerminateAPC(%p)\n", Parameter);
    g_bTerminateAll = TRUE;
}

int main(void)
{
    unsigned tid;
    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, thread_proc, NULL, 0, &tid);

    printf("main: sleep\n");
    Sleep(3 * 1000);
    printf("main: awake\n");

    QueueUserAPC(DoUserAPC, hThread, 0);

    printf("main: sleep\n");
    Sleep(3 * 1000);
    printf("main: awake\n");

    QueueUserAPC(DoUserAPC, hThread, 1);

    printf("main: sleep\n");
    Sleep(3 * 1000);
    printf("main: awake\n");

    QueueUserAPC(TerminateAPC, hThread, 2);

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return 0;
}
