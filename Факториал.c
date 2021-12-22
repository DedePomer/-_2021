#include "Header.h"



int fact(int n)// функция которая вычисляет факториал числа (рекурсивная)
{
    if (n == 0)
    {
        return 1;
    }
    else
        return n * fact(n - 1);
}
DWORD WINAPI Fact(LPVOID param)// функция которая помещается в поток
{
    printf("%d! = %d\n", *(int*)param, fact(*(int*)param));
    ExitThread(0);
}

