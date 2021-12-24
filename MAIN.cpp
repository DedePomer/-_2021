// 1 -  создание функции, аргументом которой является указатель на функцию (26) + (18)
// 2 -  создание линейного односвязного списка из n элементов (24) + (17)
// 3 - создание программы с критической секцией (25) + факториал 
// 4 - создание и отладка файла DLL (22) + импорт функций (16) + LpwstrToFloat + FloatToLpwstr + Concat
// 5 - программа для считывания нескольких числовых значений из строки и из записи в числовой массив (разделитель - пробел) (14)
// 6 - программа для считывания числового значения из файла (с сохранением его в числовом формате) (15) + программа для считывания и записи строкового значения с использованием текстового файла (16)
// 7 - разработка программы-секундомера. в одном потоке идет отсчет времени, а в другом осуществляется управление секундомером (20)
// 8 - запись и считывание в регистор числовых данных (2)

#include "Header.h"

int time[3] = { 23,59,58 }; // для секундомера

int sum(int a, int b)
{
	return a + b;
}


CRITICAL_SECTION section = { 0 };// структура для критической секции
int _const = 0;

int main()
{
	system("chcp 1251 > null");
	int n = 0;
	while (1) 
	{
		scanf_s("%d", &n);
		switch (n)
		{
		case 0:
			return 0;
		case 1:
			printf("");
			int (*f)(int);
			f = sum;
			printf("%d\n", ArgumentFunction(f));
			
			break;

		case 2:
			printf("");
			SList* List = CreateList(4);
			ShowList(List);
			break;


		case 3:
			printf("");
			HANDLE Treads[3];// создание дискриптора потоков
			int n = 50, t1 =1, t2 = 2;

			//создание самих потоков
			Treads[0] = CreateThread(NULL, 0, Fact, &n, 0, 0);// поток с факториалом
			InitializeCriticalSection(&section);// инициализация критической секции
			Treads[1] = CreateThread(NULL, 0, CritSection, &t1, 0, 0);
			Treads[2] = CreateThread(NULL, 0, CritSection, &t2, 0, 0);
			WaitForMultipleObjects(3, Treads, TRUE, INFINITE);// ожидиние выполнения потоков
			DeleteCriticalSection(&section); // удаление критической секции
			break;

		case 4:
			printf("");
			HINSTANCE MyDll; // дескриптор для подключения DLL
			if (!(MyDll = LoadLibrary(DLLPATH))) //проверка подключения DLL
			{
				return 1;
			}
			StrToFloat stf = (StrToFloat)GetProcAddress(MyDll, "LpwstrTofloat");// присвое указателям функций из DLL
			FloatToStr fts = (FloatToStr)GetProcAddress(MyDll, "FloatToLpwstr");
			Con con = (Con)GetProcAddress(MyDll, "Concat");
			//проверка работы
			LPWSTR strNull = L"";
			WCHAR* strCon = L"Ответ: ";
			float value = -1325.2878;
			strNull = fts(&value);// перевод из float в строку
			value = 0;
			MessageBox(NULL, con(strCon, strNull), L"Ответ", MB_OK); //concat
			if (stf(strNull, &value))//перевод из строки во float
			{
				printf("Полученное число: %f\n", value);
			}
			FreeLibrary(MyDll);
			break;
			

		case 5:
			printf("");
			LPWSTR strSpace = L"1012.5 21.1 -1 0.6452 12 3268 -5632 -0.23 6 12";// строка которую введём в фугкцию
			LpwstrSpace(strSpace);	// функция преобразующая строку в массив					
			break;

		case 6:
			printf("");
			HANDLE FileI, FileO; //создание дискрипторов файла
			LPWSTR strFileI = L"123";
			DWORD d;				
			FileI = CreateFile(FILEPATH, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0); // открываем файл и присваиваем значение дискриптору
			if (!WriteFile(FileI, strFileI, wcslen(strFileI)*2, &d, NULL))
			{
				MessageBox(NULL, L"Данные не записанны", L"опа", MB_OK);
			}// записываем в файл
			CloseHandle(FileI);// закрываем дискриптор

			LPWSTR strFileO = (LPWSTR)calloc(100, 2);
			FileO = CreateFile(FILEPATH, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (!ReadFile(FileO, strFileO, 6, &d,NULL))
			{
				MessageBox(NULL, L"Данные не прочитаны", L"опа", MB_OK);
			}//читаем файл
			CloseHandle(FileO);
			
			HINSTANCE MyDll2;// библиотека с функцией преобразования числа из строки
			MyDll = LoadLibrary(DLLPATH);
			StrToFloat STF = (StrToFloat)GetProcAddress(MyDll, "LpwstrTofloat");
			float outNumber;
			STF(strFileO, &outNumber);// приобразуем строку в число
			break;

		case 7:
			printf("");	
			HANDLE Watch[2];
			Watch[0] = CreateThread(NULL, 0, WatchSec, time, 0, 0);
			Watch[1] = CreateThread(NULL, 0, WatchButtonPressing, Watch[0], 0, 0);
			WaitForMultipleObjects(2, Watch, TRUE, INFINITE);
			break;

		case 8:
			printf(""); //для  DWORD
			HKEY hKey;
			DWORD v = 991, v1;
			if (RegOpenKeyW(HKEY_CURRENT_USER, L"MyParam", &hKey) != 0)
			{
				return 1;
			}
			if (!RegSetValueExW(hKey, L"DwordParam", NULL, REG_DWORD, &v, 4))
			{
				printf("в реестре создался цифра\n");
			}
			DWORD dataType = 0, dataLen = 400;
			if (RegGetValueW(HKEY_CURRENT_USER, L"MyParam", L"DwordParam", RRF_RT_ANY, &dataType, &v1, &dataLen) == ERROR_SUCCESS)
			{
				printf("цифра %d\n", v1);
			}
			break;
		}
		system("pause");
		system("cls");
	}
	return 0;

}
// секундомер
DWORD WINAPI WatchSec(LPVOID param)
{
	int h , m , s;
	while (1)
	{
		h = ((int*)param)[0]; m = ((int*)param)[1]; s = ((int*)param)[2];
		if (s < 60)
		{
			s++;
		}
		if (s == 60 && m < 60)
		{
			m++;
			s = 0;
		}
		if (m == 60 && h < 24)
		{
			m = 0;
			h++;
		}
		if (h == 24)
		{
			h = 0; m = 0; s = 0;
		}
		printf("%02d %02d %02d\n", h, m, s);
		((int*)param)[0] = h; ((int*)param)[1] = m; ((int*)param)[2] = s;
		Sleep(1000);
	}
	ExitThread(0);
}
DWORD WINAPI WatchButtonPressing(LPVOID param)
{
	int SecItsStop = 1;
	while (1)
	{
		switch (_getch())
		{
		case 113:
		case 233:
			if (SecItsStop == 1)
			{
				SuspendThread((HANDLE)param);
				SecItsStop = 0;
			}
			else
			{
				ResumeThread((HANDLE)param);
				SecItsStop = 1;
			}
			break;
		case 119:
			TerminateThread((HANDLE)param, 0);
			ExitThread(0);
			break;
		case 101:
			time[0] = 0;
			time[1] = 0;
			time[2] = 0;
			system("cls");
			break;
		}
	}
	ExitThread(0);
}
// функция с аргументом указатель на функцию
int ArgumentFunction(int (*f)(int))
{
	return f(2,3);
}
// критическая секция
DWORD WINAPI CritSection(LPVOID param)// функция с критической секцией
{
	EnterCriticalSection(&section); // 
	for (size_t i = 0; i < 10; i++)
	{	
		_const++;
		printf("Поток - %d, значение - %d\n", *(int*)param, _const);
	}
	LeaveCriticalSection(&section);
}



