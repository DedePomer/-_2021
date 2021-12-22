#include "Header.h"

SList* CreateList(int n)
{
	SList* first = calloc(1, sizeof(SList));// инициализируем первую структуру
	SList* next, * early;// создаЄм предыдущую и след. структуру

	first->value = 1; // задаЄм значение  впервой структуре
	first->NextStruct = NULL;

	early = first;
	for (size_t i = 0; i < n - 1; i++)
	{
		next = calloc(1, sizeof(SList)); // освобождаем место под след. структуру
		//next->value = early->value-1;
		early->NextStruct = next;// в предыдущей структуре делаем указатель на след.
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
		printf("Ёл. номер %d = %d\n", i, list->value);
		i++;
		list = list->NextStruct;
	}
}