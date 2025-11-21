#include <iostream>
#include <windows.h>
using namespace std;


struct ProcessNode {
    HANDLE hProcess;
    ProcessNode* next;
};

ProcessNode* g_head = nullptr;

void AddProcess(HANDLE _process) {
    ProcessNode* newNodeProcess = new ProcessNode();
    newNodeProcess->hProcess = _process;
    newNodeProcess->next = g_head;
    g_head = newNodeProcess;
}


void StartProcess(const wchar_t* path) {

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);


    if (CreateProcess(
        path,
        NULL,
        NULL, NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        CloseHandle(pi.hThread);
        AddProcess(pi.hProcess);
    }
}


void StopProcess() {
    if (g_head == nullptr) {
        cout << "Нет запущеного процесса" << endl;
        return;
    }

    ProcessNode* current = g_head;
    g_head = g_head->next;

    TerminateProcess(current->hProcess, 0);
    CloseHandle(current->hProcess);
    delete current;
    

}







DWORD WINAPI thread(LPVOID lpParam) {
    int count = 0;
    while (true) {
       count++;
        Sleep(1000);
        cout << "count = " << count << endl;
    }
    return 0;
}


int main()
{
    setlocale(0, "RUS");

    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, NULL, 0, NULL);
    if (hThread == NULL) {
        return GetLastError();
    } 

    HANDLE hInherThread;

    if (!DuplicateHandle(
        GetCurrentProcess(),
        hThread,
        GetCurrentProcess(),
        &hInherThread,
        0,
        TRUE,
        DUPLICATE_SAME_ACCESS
    ))
    {
        return GetLastError();
    }



    wchar_t cmd[256];
    wcscpy_s(cmd, L"C:\\Users\\79126\\Desktop\\prack_9_sys\\x64\\Debug\\Counter.exe ");
    wchar_t symvolHandle[20];
    swprintf_s(symvolHandle, L"%p", hInherThread);
    wcscat_s(cmd, symvolHandle);


    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);


    if (!CreateProcessW(
        NULL,
        cmd,
        NULL,
        NULL,
        TRUE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        return GetLastError();
    }


    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hInherThread);
    CloseHandle(hThread);


    int choice;
    do {
        cout << "===== Меню =====" << endl;
        cout << "1 - Открыть ворд" << endl;
        cout << "2 - Открыть блокнот" << endl;
        cout << "3 - Открыть пэинт" << endl;
        cout << "4 - Открыть эксель" << endl;

        cout << "5 - Закрыть" << endl;
        cout << "0 - Выйти" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                StartProcess(L"C:\\Program Files\\Microsoft Office\\root\\Office16\\WINWORD.EXE");
                break;
            }
            case 2: {
                StartProcess(L"C:\\Windows\\System32\\notepad.exe");  
                break;
            }
            case 3: {
                StartProcess(L"C:\\Windows\\System32\\mspaint.exe");  
                break;
            }
            case 4: {
                StartProcess(L"C:\\Program Files\\Microsoft Office\\root\\Office16\\EXCEL.EXE");  
                break;
            }
            case 5: {
                StopProcess();
                break;
            }
            case 0: {
                StopProcess();
                return 0;
            }

            default: {
                cout << "Неверный выбор" << endl;
                return 1;
            }
        } 
    }
    while (choice != 0);


    return 0;
}
