#include "Header.h"

SList* CreateList(int n)
{
	SList* first = calloc(1, sizeof(SList));// �������������� ������ ���������
	SList* next, * early;// ������ ���������� � ����. ���������

	first->value = 1; // ����� ��������  ������� ���������
	first->NextStruct = NULL;

	early = first;
	for (size_t i = 0; i < n - 1; i++)
	{
		next = calloc(1, sizeof(SList)); // ����������� ����� ��� ����. ���������
		//next->value = early->value-1;
		early->NextStruct = next;// � ���������� ��������� ������ ��������� �� ����.
		early = next;
	}
	early->NextStruct = NULL;
	return first;
}
void ShowList(SList* list)
{
	int i = 1;
	while (list)
	{
		printf("��. ����� %d = %d\n", i, list->value);
		i++;
		list = list->NextStruct;
	}
}