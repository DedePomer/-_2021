#include "Header.h"



int fact(int n)// ������� ������� ��������� ��������� ����� (�����������)
{
    if (n == 0)
    {
        return 1;
    }
    else
        return n * fact(n - 1);
}
DWORD WINAPI Fact(LPVOID param)// ������� ������� ���������� � �����
{
    printf("%d! = %d\n", *(int*)param, fact(*(int*)param));
    ExitThread(0);
}

