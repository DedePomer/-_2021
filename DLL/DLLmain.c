#include <Windows.h>
#include <Math.h>

BOOL WINAPI DllMain(HINSTANCE hlnstDll, DWORD dwReason, LPVOID IpReserved)
{
	BOOL bAllWentWell = TRUE;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	if (bAllWentWell)
		return TRUE;
	else
		return FALSE;
}

extern "C" __declspec(dllimport) BOOL LpwstrTofloat(LPWSTR , float* );
BOOL LpwstrTofloat(LPWSTR firstStr, float* firstFloat) // функция переводит из LPWSTR во float (нет проверки на корректность строки)
{
	if (firstStr[0] == L'\0')// проверка на пустую строкук
	{
		*firstFloat = 0;
		return FALSE;
	}
	BOOL haveMinus = FALSE;
	if (firstStr[0] == L'-')// проверка на минус
	{
		haveMinus = TRUE;
	}
	int indDot = 1, dotNumber = 0;
	for (size_t i = 0; i < wcslen(firstStr); i++)// ищём точку 
	{
		if (!(firstStr[i] == L'-'))
		{
			if (firstStr[i] == L'.')
			{
				indDot = i;
				dotNumber++;// кол-во точек
			}
		}
	}
	if (indDot == 0)//если точка первая то ошибка
	{
		*firstFloat = 0;
		return FALSE;
	}
	if (dotNumber != 0)//если точек больше 1 то ошибка
	{
		if (dotNumber > 1)
		{
			*firstFloat = 0;
			return FALSE;
		}
	}
	float outputNumber = 0, changeNumber = 0;
	for (size_t i = 0; i < wcslen(firstStr); i++)//преобразование во float
	{
		if (!(firstStr[i] == L'-'))// проверка на минус
		{
			if (dotNumber > 0)// если есть точки
			{
				if (i != indDot) // пока i != индефикатору точки
				{
					if (i < indDot) //числа до точки 
					{
						changeNumber += (firstStr[i] - L'0');
						outputNumber += changeNumber * pow(10, (indDot - (i + 1)));
					}
					if (i > indDot)// числа после точки
					{
						changeNumber += (firstStr[i] - L'0');
						outputNumber += changeNumber / pow(10, ((i)-indDot));
					}					
				}
			}
			else // если число целое
			{
				changeNumber += (firstStr[i] - L'0');
				outputNumber += changeNumber * pow(10, (wcslen(firstStr) - (i + 1)));
			}
			changeNumber = 0;
		}
	}
	if (haveMinus) // если был минус
	{
		outputNumber *= -1;
	}
	*firstFloat = outputNumber;
	return TRUE;
}

extern "C" __declspec(dllimport) LPWSTR FloatToLpwstr(float* firstFloat);
LPWSTR FloatToLpwstr(float* firstFloat) // переводит из float в LPWSTR
{
	BOOL haveMinus = FALSE;
	if ((*firstFloat) < 0)// проверка на наличие минуса
	{
		*firstFloat *= -1;
		haveMinus = TRUE;
	}
	float changeFloat = *firstFloat;
	int rankNumber = 0, inddot = 0, rankInt = 0, itsZero = 0; // кол во разрядов/ расположение точки/ кол-во целых чисел/ "это ноль"

	while (1) // считает кол-во символов в строке 
	{
		if ((int)changeFloat == 0 && itsZero == 0)// когда нет целых чисел
		{
			if (haveMinus)
			{
				inddot = 2;
				rankNumber += 10;
			}
			else
			{
				inddot = 1;
				rankNumber += 9;
			}
			itsZero++;
			rankInt++;
			break;
		}
		else if (changeFloat >= 1) // когда есть целые числа
		{
			changeFloat /= 10;
			rankNumber++;
			rankInt++;
			itsZero++;
		}
		else if (fmod(*firstFloat, 1) > 0) // если есть дробная часть (fmod остаток от деления для float)
		{
			if (haveMinus)
			{
				rankNumber++;
				inddot = rankNumber;
				rankNumber += 9;// семь символов после запятой + точка + "-"	
			}
			else
			{
				inddot = rankNumber;
				rankNumber += 8;// семь символов после запятой + точка	
			}
			break;
		}
		else if ((int)changeFloat == 0 && (int)(changeFloat * pow(10, rankInt)) == (int)*firstFloat)
		{
			if (haveMinus)
			{
				rankNumber++;
			}
			break;
		}
	}
	LPWSTR outputStr = (LPWSTR)calloc(rankNumber, sizeof(WCHAR));// строка которую мы выводим
	if (haveMinus)
	{
		outputStr[0] = L'-';
	}
	changeFloat = trunc(*firstFloat); // получаем целую часть
	int buffInt = 0;  
	for (int i = 0; rankInt != 0; i++)// записываем целую часть
	{
		if (haveMinus)
		{
			i++;
			haveMinus = FALSE;
		}
		changeFloat -= buffInt; //вычитаем для отделения не нужной части
		changeFloat /= pow(10, rankInt - 1); // получаем цифру самого левого разряда в changeFloat
		buffInt += (int)changeFloat * pow(10, rankInt - 1);// записываем то число разрад которого мы уже обработали
		rankInt--; // условие цикла
		outputStr[i] = 48 + changeFloat; // запись в строку
		changeFloat = trunc(*firstFloat);// записываю целую часть
	}
	if (inddot != 0)// запись дробной части
	{
		float buff;
		changeFloat = modff(*firstFloat, &buff); // получаем дробную часть (в buff хранится целая)
		outputStr[inddot] = L'.';
		for (int i = inddot + 1; i < rankNumber; i++)
		{
			changeFloat *= 10;// получаем цифру
			outputStr[i] = 48 + (int)changeFloat; // записываем её
			changeFloat = modff(changeFloat, &buff); // получаю остаток от уже изсеннёного числа
		}
	}
	outputStr[rankNumber] = L'\0';// конец строки
	return outputStr;
}

extern "C" __declspec(dllexport) LPWSTR Concat(LPWSTR, LPWSTR);
LPWSTR Concat(LPWSTR c1, LPWSTR c2) // объеденение двух строк
{
	int l1 = 0, l2 = 0;
	while (c1[l1] != L'\0')// длина первой строчки
	{
		l1++;
	}
	while (c2[l2] != L'\0')// длина второй
	{
		l2++;
	}
	LPWSTR outputStr = (LPWSTR)calloc(l1+l2, sizeof(WCHAR));// выходная строчка
	for (int i = 0; i < l1; i++)// записываю первую в выходную
	{
		outputStr[i] = c1[i];
	}
	for (int i = 0; i < l2; i++)// записываю вторую в выходную
	{
		outputStr[i + l1] = c2[i];
	}
	outputStr[l1 + l2] = L'\0';
	return outputStr;
}

//DWORD LpwstrToDword(LPSTR str)
//{
//	DWORD dw = 0;
//	if (str[0] != '-')
//	{
//		for (size_t i = 0; i < strlen(str); i++)
//		{
//			dw += (str[i] - '0');
//			dw *= 10;
//		}
//		return dw / 10;
//	}
//	else
//	{
//		for (size_t i = 1; i < strlen(str); i++)
//		{
//			dw += (str[i] - '0');
//			dw *= 10;
//		}
//		return dw / 10;
//	}
//}
//
//LPSTR DwordToLpstr(DWORD num)
//{
//	LPSTR str = calloc(200, sizeof(CHAR));
//	char c = ' ';
//	DWORD n = num, m = 0;
//	int y = 0;
//	for (int i = 10; num % (i / 10) != num; i *= 10)
//	{
//		m = n % i;
//		m = m / (i / 10);
//		c = 48 + m;
//		str[y] = c;
//		y++;
//	}
//	return ReverseStr(str);
//}
//
//LPSTR ReverseStr(LPSTR str)
//{
//	LPSTR strBuff = calloc(200, sizeof(CHAR));
//	int y = 0;
//	for (int i = strlen(str) - 1; i >= 0; i--)
//	{
//		strBuff[y] = str[i];
//		y++;
//	}
//	return strBuff;
//}