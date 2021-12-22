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
BOOL LpwstrTofloat(LPWSTR firstStr, float* firstFloat) // ������� ��������� �� LPWSTR �� float (��� �������� �� ������������ ������)
{
	if (firstStr[0] == L'\0')// �������� �� ������ �������
	{
		*firstFloat = 0;
		return FALSE;
	}
	BOOL haveMinus = FALSE;
	if (firstStr[0] == L'-')// �������� �� �����
	{
		haveMinus = TRUE;
	}
	int indDot = 1, dotNumber = 0;
	for (size_t i = 0; i < wcslen(firstStr); i++)// ���� ����� 
	{
		if (!(firstStr[i] == L'-'))
		{
			if (firstStr[i] == L'.')
			{
				indDot = i;
				dotNumber++;// ���-�� �����
			}
		}
	}
	if (indDot == 0)//���� ����� ������ �� ������
	{
		*firstFloat = 0;
		return FALSE;
	}
	if (dotNumber != 0)//���� ����� ������ 1 �� ������
	{
		if (dotNumber > 1)
		{
			*firstFloat = 0;
			return FALSE;
		}
	}
	float outputNumber = 0, changeNumber = 0;
	for (size_t i = 0; i < wcslen(firstStr); i++)//�������������� �� float
	{
		if (!(firstStr[i] == L'-'))// �������� �� �����
		{
			if (dotNumber > 0)// ���� ���� �����
			{
				if (i != indDot) // ���� i != ������������ �����
				{
					if (i < indDot) //����� �� ����� 
					{
						changeNumber += (firstStr[i] - L'0');
						outputNumber += changeNumber * pow(10, (indDot - (i + 1)));
					}
					if (i > indDot)// ����� ����� �����
					{
						changeNumber += (firstStr[i] - L'0');
						outputNumber += changeNumber / pow(10, ((i)-indDot));
					}					
				}
			}
			else // ���� ����� �����
			{
				changeNumber += (firstStr[i] - L'0');
				outputNumber += changeNumber * pow(10, (wcslen(firstStr) - (i + 1)));
			}
			changeNumber = 0;
		}
	}
	if (haveMinus) // ���� ��� �����
	{
		outputNumber *= -1;
	}
	*firstFloat = outputNumber;
	return TRUE;
}

extern "C" __declspec(dllimport) LPWSTR FloatToLpwstr(float* firstFloat);
LPWSTR FloatToLpwstr(float* firstFloat) // ��������� �� float � LPWSTR
{
	BOOL haveMinus = FALSE;
	if ((*firstFloat) < 0)// �������� �� ������� ������
	{
		*firstFloat *= -1;
		haveMinus = TRUE;
	}
	float changeFloat = *firstFloat;
	int rankNumber = 0, inddot = 0, rankInt = 0, itsZero = 0; // ��� �� ��������/ ������������ �����/ ���-�� ����� �����/ "��� ����"

	while (1) // ������� ���-�� �������� � ������ 
	{
		if ((int)changeFloat == 0 && itsZero == 0)// ����� ��� ����� �����
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
		else if (changeFloat >= 1) // ����� ���� ����� �����
		{
			changeFloat /= 10;
			rankNumber++;
			rankInt++;
			itsZero++;
		}
		else if (fmod(*firstFloat, 1) > 0) // ���� ���� ������� ����� (fmod ������� �� ������� ��� float)
		{
			if (haveMinus)
			{
				rankNumber++;
				inddot = rankNumber;
				rankNumber += 9;// ���� �������� ����� ������� + ����� + "-"	
			}
			else
			{
				inddot = rankNumber;
				rankNumber += 8;// ���� �������� ����� ������� + �����	
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
	LPWSTR outputStr = (LPWSTR)calloc(rankNumber, sizeof(WCHAR));// ������ ������� �� �������
	if (haveMinus)
	{
		outputStr[0] = L'-';
	}
	changeFloat = trunc(*firstFloat); // �������� ����� �����
	int buffInt = 0;  
	for (int i = 0; rankInt != 0; i++)// ���������� ����� �����
	{
		if (haveMinus)
		{
			i++;
			haveMinus = FALSE;
		}
		changeFloat -= buffInt; //�������� ��� ��������� �� ������ �����
		changeFloat /= pow(10, rankInt - 1); // �������� ����� ������ ������ ������� � changeFloat
		buffInt += (int)changeFloat * pow(10, rankInt - 1);// ���������� �� ����� ������ �������� �� ��� ����������
		rankInt--; // ������� �����
		outputStr[i] = 48 + changeFloat; // ������ � ������
		changeFloat = trunc(*firstFloat);// ��������� ����� �����
	}
	if (inddot != 0)// ������ ������� �����
	{
		float buff;
		changeFloat = modff(*firstFloat, &buff); // �������� ������� ����� (� buff �������� �����)
		outputStr[inddot] = L'.';
		for (int i = inddot + 1; i < rankNumber; i++)
		{
			changeFloat *= 10;// �������� �����
			outputStr[i] = 48 + (int)changeFloat; // ���������� �
			changeFloat = modff(changeFloat, &buff); // ������� ������� �� ��� ���������� �����
		}
	}
	outputStr[rankNumber] = L'\0';// ����� ������
	return outputStr;
}

extern "C" __declspec(dllexport) LPWSTR Concat(LPWSTR, LPWSTR);
LPWSTR Concat(LPWSTR c1, LPWSTR c2) // ����������� ���� �����
{
	int l1 = 0, l2 = 0;
	while (c1[l1] != L'\0')// ����� ������ �������
	{
		l1++;
	}
	while (c2[l2] != L'\0')// ����� ������
	{
		l2++;
	}
	LPWSTR outputStr = (LPWSTR)calloc(l1+l2, sizeof(WCHAR));// �������� �������
	for (int i = 0; i < l1; i++)// ��������� ������ � ��������
	{
		outputStr[i] = c1[i];
	}
	for (int i = 0; i < l2; i++)// ��������� ������ � ��������
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