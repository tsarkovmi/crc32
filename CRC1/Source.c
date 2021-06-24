#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h> 
#include <stdio.h> 


int Count_Check_mas = 0;
//int Check_end = 0;
int Count_Zeros = 0;

void Zapolnit_Bukvu(int *mas, FILE *fin)
{
	int symb = fgetc(fin);//Считали символ из файла
	if (symb == -1)// Конец файла, забивает массив нулями, чтобы нормально досчитать СРС и не выключить программу 
	{
			Count_Zeros += 8;
		if (Count_Zeros > 32)
			return;
		for (int i = 0; i < 8; i++)
			mas[i] = 0;
		//Check_end = 1; 
		return;
	}
	for (int i = 0; i < 8; i++)//Перевод символа в двоичное представление 
	{
		char s = 1;
		int bit = s & symb;
		symb = symb >> 1;
		mas[7 - i] = bit;
	}
}

void Perenos(int *Delimoe, int *Mas_for_Copy, int *Symbol)
{
	for (int i = 0; i < 31; i++)
		Mas_for_Copy[i] = Delimoe[i + 1];

	for (int i = 0; i < 31; i++)
		Delimoe[i] = Mas_for_Copy[i];
	Delimoe[31] = Symbol[Count_Check_mas];
	Count_Check_mas++;

}



int main()
{
	//printf("%d", EOF);
	system("chcp 1251>nul");
	int p = 0;
	printf("Выберите действие:\n1-Посчитать CRC32 из файла input.txt\n2-Закрыть программу\n");
	scanf("%d", &p);
	if (p == 2)return 0;
	FILE *fin = fopen("input.txt", "rb");
	if (fin == NULL) //проверка на наличие файла
	{
		printf("Файл отсутвует!");
		getch();
		return 0;
	}
	int Delimoe[32];
	int Polynom[32];
	int Mas_for_Copy[31];
	int Symbol[8];
	long Poly = 0xF17D91F8;
	for (int i = 0; i < 32; i++)//Цикл для получения полинома в двоичном представлении
	{
		char s = 1;
		int bit = s & Poly;
		Poly = Poly >> 1;
		Polynom[31 - i] = bit;
	}
	Zapolnit_Bukvu(Symbol, fin);
	for (int i = 0; i < 8; i++)
		Delimoe[i] = Symbol[i];
	Zapolnit_Bukvu(Symbol, fin);
	for (int i = 8; i < 16; i++)
		Delimoe[i] = Symbol[i - 8];
	Zapolnit_Bukvu(Symbol, fin);
	for (int i = 16; i < 24; i++)
		Delimoe[i] = Symbol[i - 16];
	Zapolnit_Bukvu(Symbol, fin);
	for (int i = 24; i < 32; i++)
		Delimoe[i] = Symbol[i - 24];//Здесь у нас делимое имеет 32 разряда
	Zapolnit_Bukvu(Symbol, fin);
	while (1)
	{
		while (1)
		{
			if (Delimoe[0] == 0)
			{
				Perenos(Delimoe, Mas_for_Copy, Symbol);
				//printf("Perenesli odin nolik\n");

			}
			else
			{
				Perenos(Delimoe, Mas_for_Copy, Symbol);
				//printf("Perenesli odin\n");//Перенос единички))))
				if (Count_Check_mas == 8)//Вероятно, делается проверка на наличие буквы в входном потоке, если счёткчик 8, значит у нас биты закончились, и нужно пополнить их
				{
					Zapolnit_Bukvu(Symbol, fin);
					Count_Check_mas = 0;
				}
				if (Count_Zeros > 32)
					break;
				//for (int i = 0; i < 32; i++)//Просто контроль делимого 
				//	printf("%d", Delimoe[i]);
				//printf("\n");
				//for (int i = 0; i < 32; i++)//Просто контроль полинома 
				//	printf("%d", Polynom[i]);
				//printf("\n");
				for (int i = 0; i < 32; i++)//Ксорим делимое с полиномом
					Delimoe[i] = Delimoe[i] ^ Polynom[i];
				//for (int i = 0; i < 32; i++)//Просто контроль делимого
				//	printf("%d", Delimoe[i]);
				/*printf("\n");
				printf("\n");*/
				if (Count_Zeros > 32)
					break;
			}
			if (Count_Check_mas == 8)//Тут мы тоже пополняем запас букв
			{
				Zapolnit_Bukvu(Symbol, fin);
				Count_Check_mas = 0;
			}
			if (Count_Zeros > 32)
			{
				if (Delimoe[0] == 0)
				{
					long long crc = 0;
					for (int i = 0; i < 32; i++)
					{
						crc = crc | Delimoe[i];
						crc = crc << 1;
					}
					crc = crc >> 1;
					printf("%llX", crc);
					system("pause");
					return;
				}
				else
					break;
			}
		}
		/*for (int i = 0; i < 32; i++)
			printf("%d", Delimoe[i]);
		printf("\n");
		for (int i = 0; i < 32; i++)
			printf("%d", Polynom[i]);
		printf("\n");*/
		for (int i = 0; i < 32; i++)
			Delimoe[i] = Delimoe[i] ^ Polynom[i];
		/*for (int i = 0; i < 32; i++)
			printf("%d", Delimoe[i]);
		printf("\n");
		printf("\n");*/
		if (Count_Zeros > 32)
			break;
	}
	//printf("\n");
	/*for (int i = 0; i < 32; i++)
		printf("%d", Delimoe[i]);*/
	//printf("\n");
	long long crc = 0;
	for (int i = 0; i < 32; i++)
	{
		crc = crc | Delimoe[i];
		crc = crc << 1;
	}
	crc = crc >> 1;
	printf("%llX\n", crc);
	system("pause");
}