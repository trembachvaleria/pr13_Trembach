// pr13_Trembach.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>

using namespace std;

struct Racer {
    int id;
    DWORD pid;
    HANDLE hProcess;
    DWORD priority;
    DWORD startTime;
    DWORD endTime;
};

Racer racers[5];
int racersCount = 0;

string priorityName(DWORD p)
{
    if (p == IDLE_PRIORITY_CLASS) return "IDLE";
    if (p == BELOW_NORMAL_PRIORITY_CLASS) return "BELOW_NORMAL";
    if (p == NORMAL_PRIORITY_CLASS) return "NORMAL";
    if (p == ABOVE_NORMAL_PRIORITY_CLASS) return "ABOVE_NORMAL";
    if (p == HIGH_PRIORITY_CLASS) return "HIGH";
    return "UNKNOWN";
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "\nГОНКА ПРОЦЕССОВ" << endl;
    cout << "Введите количество гонщиков (2-5): ";
    cin >> racersCount;
    if (racersCount > 5)
    {
        racersCount = 5;
    }

    string startStr, endStr;
    cout << "Введите диапазон чисел (например: 1 20000000): ";
    cin >> startStr >> endStr;

    cout << "\nСТАРТ" << endl;
    cout << "Пока гонка идет, вы можете нажать 'p', чтобы изменить приоритет!" << endl;

    for (int i = 0; i < racersCount; i++)
    {
        racers[i].id = i + 1;
        string cmd = "racer.exe " + startStr + " " + endStr;
        char* cmdChar = new char[cmd.length() + 1];
        strcpy_s(cmdChar, cmd.length() + 1, cmd.c_str());

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        if (CreateProcessA(NULL, cmdChar, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        {
            racers[i].pid = pi.dwProcessId;
            racers[i].hProcess = pi.hProcess;

            racers[i].priority = NORMAL_PRIORITY_CLASS;
            SetPriorityClass(racers[i].hProcess, racers[i].priority);

            racers[i].startTime = GetTickCount();
            CloseHandle(pi.hThread);
        }
        delete[] cmdChar;
    }

    int finishedC = 0;
    while (finishedC < racersCount)
    {
        finishedC = 0;

        for (int i = 0; i < racersCount; i++)
        {
            if (racers[i].endTime == 0)
            {
                DWORD exitCode;
                if (GetExitCodeProcess(racers[i].hProcess, &exitCode) && exitCode != STILL_ACTIVE)
                {
                    racers[i].endTime = GetTickCount();
                }
            }
            else
            {
                finishedC++;
            }
        }

        if (_kbhit())
        {
            char key = _getch();
            if (key == 'p' || key == 'P')
            {
                int id;
                cout << "\nВведите ID гонщика для изменения приоритета: ";
                cin >> id;

                if (id >= 1 && id <= racersCount && racers[id - 1].endTime == 0)
                {
                    cout << "Выберите приоритет (1-IDLE, 2-BELOW_NORMAL, 3-NORMAL, 4-ABOVE_NORMAL, 5-HIGH): ";
                    int pChoice;
                    cin >> pChoice;

                    if (pChoice == 1) racers[id - 1].priority = IDLE_PRIORITY_CLASS;
                    else if (pChoice == 2) racers[id - 1].priority = BELOW_NORMAL_PRIORITY_CLASS;
                    else if (pChoice == 3) racers[id - 1].priority = NORMAL_PRIORITY_CLASS;
                    else if (pChoice == 4) racers[id - 1].priority = ABOVE_NORMAL_PRIORITY_CLASS;
                    else if (pChoice == 5) racers[id - 1].priority = HIGH_PRIORITY_CLASS;

                    SetPriorityClass(racers[id - 1].hProcess, racers[id - 1].priority);
                    cout << "! Приоритет гонщика " << id << " изменен на " << priorityName(racers[id - 1].priority) << endl;
                }
            }
        }
        Sleep(50);
    }

    cout << "\nРЕЗУЛЬТАТЫ" << endl;
    for (int i = 0; i < racersCount; i++)
    {
        DWORD exitCode;
        GetExitCodeProcess(racers[i].hProcess, &exitCode);
        DWORD time = racers[i].endTime - racers[i].startTime;

        cout << "Гонщик №" << racers[i].id << " (PID: " << racers[i].pid << ")." << " Приоритет: " << priorityName(racers[i].priority) << ". Время: " << time << " мс" << ". Найдено: " << exitCode << " чисел." << endl;
    }

    for (int i = 0; i < racersCount; i++)
    {
        CloseHandle(racers[i].hProcess);
    }

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
