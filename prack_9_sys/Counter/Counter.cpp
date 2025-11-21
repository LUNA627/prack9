#include <windows.h>
#include <iostream>
#include <cstdio>
#include <locale>
using namespace std;


int wmain(int argc, wchar_t* argv[])
{
    setlocale(LC_ALL, "Russian");

    void* ptr = nullptr;

    swscanf_s(argv[1], L"%p", &ptr);

    HANDLE hThread = (HANDLE)ptr;



    int choice;

    do {
        wcout << "1 — приостановить\n";
        wcout << "2 — возобновить\n";
        wcout << "3 — выйти\n";
        wcin >> choice;

        switch (choice) {
            case 1: {
                SuspendThread(hThread);
                break;
            }

            case 2: {
                ResumeThread(hThread);
                break;
            }

            case 3: {
                return 0;
            }
        }
    }
    while (choice != 3);


    return 0;
}
