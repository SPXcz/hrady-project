#include "pch.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "stdlib.h"
#include "conio.h"
#include "hrady_lib.h"

/*
	Input: FILE **file - p�ed�n� odkazem ukazatele na soubor ke zpracov�n� - tento soubor bude otev�en
		   const char *route - cesta k souboru
		   const char style[2] - zp�sob jak�m m� b�t soubor otev�en ("r", "w", "a", "r+", "w+", "a+")

	Funkce otev�e, inicializuje a ov��� soubor.

	Output: (FILE *file)
			True - Soubor byl otev�en
			False - Soubor nebvyl otev�en
*/


bool open_file(FILE **file, const char *route, const char style[2])
{
	fopen_s(file, route, style);

	if (*file == NULL)
	{
		printf("ERROR: %s has not been opened.\n", route);
		return false;
	}
	else return true;
}

/*
	Input - char **adresa - p�ed�n� stringu pro budouc� adresu souboru odkazem
			char *name - u�ivatelsk� jm�no
			bool oteviraci_db - pro soubor otev�rac� doby (aktu�ln� nepou�it�, p�ed�vejte hodnotu 'true')

	Slo�� adresu souboru na u�ivatelskou datab�zi

	Output - true - adresa byla slo�ena

*/


bool connect_personal_adress(char **adresa, char *name, bool oteviraci_db)
{
	int size, adresa_size;

	if (oteviraci_db)
	{
		size = strlen("files/") + 1;
		adresa_size = size + 4 + strlen(name);
		*adresa = (char*)malloc(adresa_size);														//Alokov�n� pam�ti pro z�pis celkov� adresy souboru s datab�t� u�ivatele

		strcpy_s(*adresa, adresa_size, "files/");													//"Ukon�en�" pole char� (ikdy� pole existuje i za znakem ukon�en� pole)

		strcat_s(*adresa, adresa_size, name);
		strcat_s(*adresa, adresa_size, ".txt");
	}
	else
	{
		size = strlen("files/oteviraci_hodiny_") + 1;
		adresa_size = size + 4 + strlen(name);
		*adresa = (char*)malloc(adresa_size);														//Alokov�n� pam�ti pro z�pis celkov� adresy souboru s datab�t� u�ivatele

		strcpy_s(*adresa, adresa_size, "files/oteviraci_hodiny_");									//"Ukon�en�" pole char� (ikdy� pole existuje i za znakem ukon�en� pole)

		strcat_s(*adresa, adresa_size, name);
		strcat_s(*adresa, adresa_size, ".txt");
	}

	return true;
}

/*
	Input: char *str1 - Prvn� string na kontrolu
		   char *str2 - Druh� string na kontrolu

	Zkontroluje kolik znak� je v �et�zci jin�ch a vr�t� po�et stejn�ch znak�.

	Output: (int) Po�et stejn�ch znak�.

*/

int custom_strcmp(const char *str1, const char *str2)
{
	int same = 0;

	for (int i = 0; str1[i] != '\0' && str2[i] != '\0'; i++)
	{
		if (str1[i] == str2[i])
		{
			same++;
		}
	}

	return same;
}

/*
	Input: int y - nov� pozice kurzoru

	Funkce p�esune kurzor na danou pozici

	Output: true - sou�adnice byly nastaveny

*/

bool gotoXY(int y)
{
	printf("%c[%d;%df", 0x1B, y, 0);
	return true;
}


/*
	Input: Results *results_new - struktura dat, kter� budou p�id�ny do datab�ze (ukazatel)
		   char *name - U�ivatelsk� jm�no
		   bool pridani_jmena - true -> Bude p�id�v�no jm�no
								false -> Nebude p�id�no jm�no

	Funkce provede samotn� akt p�id�n� polo�ky do datab�ze. Polo�ka je do datab�ze p�id�na podle po�ad� jm�na hradu/z�mku v abeced�.

	Output: true - v�e prob�hlo v po��dku
			false - n�jak� soubor nebyl otev�en
*/

bool OnAdd(Results *results_new, char *name, bool pridani_jmena)
{
	FILE *file, *tmp_f;
	bool added = false;
	char *adresa;
	Results *result = (Results*)malloc(sizeof(Results));
	connect_personal_adress(&adresa, name, true);
	if (!open_file(&file, adresa, "r")) return false;
	if (!open_file(&tmp_f, "files/tmp.txt", "w")) return false;

	while ((fscanf_s(file, "%s %s %s %s %s %s",
		result->hrad_zamek, sizeof(char) * 7,
		result->jmeno, sizeof(char) * 51,
		result->obec, sizeof(char) * 51,
		result->oteviraci_hodiny, sizeof(char) * 10,
		result->zaviraci_hodiny, sizeof(char) * 10,
		result->vstupne, sizeof(char) * 10) != EOF))
	{

		if (pridani_jmena && (strcmp(result->jmeno, results_new->jmeno) == 0))
		{
			printf("Duplikatni polozka.\n");
			fclose(tmp_f);
			fclose(file);
			return true;
		} 
		if ((strcmp(result->jmeno, results_new->jmeno) > 0) && !added)
			{
				fprintf(tmp_f, "%s %s %s %s %s %s\n", results_new->hrad_zamek, results_new->jmeno, results_new->obec, results_new->oteviraci_hodiny, results_new->zaviraci_hodiny, results_new->vstupne);

				added = true;
			}

		fprintf(tmp_f, "%s %s %s %s %s %s\n",
			result->hrad_zamek,
			result->jmeno,
			result->obec,
			result->oteviraci_hodiny,
			result->zaviraci_hodiny,
			result->vstupne);
	}

	if (!added)
	{
		fprintf(tmp_f, "%s %s %s %s %s %s\n", results_new->hrad_zamek, results_new->jmeno, results_new->obec, results_new->oteviraci_hodiny, results_new->zaviraci_hodiny, results_new->vstupne);
	}

	fclose(tmp_f);
	fclose(file);

	remove(adresa);
	rename("files/tmp.txt", adresa);

	return true;
}

/*
	Input - char *name - u�ivatelsk� jm�no

	Vytv��� kostru pro p�id�n� do datab�ze. Tj. vyzve u�ivatele, aby zadal polo�ky pro p�id�n�. (Fuknce pro samotn� p�id�n� je zavol�na na konci t�to funkce)

	Output - true - prob�hlo v po��dku
			 false - chyba p�i otev�r�n� souboru

*/

bool add(char *name)
{
	Results *results_new = (Results*)malloc(sizeof(Results));
	bool added = false;
	int vstupne;
	int radky = 0, hrad_zamek;
	do
	{
		printf("Jedna se o zamek nebo hrad? (zamek - 1; hrad - 2): ");
		scanf_s("%d", &hrad_zamek);

	} while (hrad_zamek != 1 && hrad_zamek != 2);

	if (hrad_zamek - 1)
	{
		strcpy_s(results_new->hrad_zamek, 6, "hrad");
	}
	else
	{
		strcpy_s(results_new->hrad_zamek, 6, "zamek");
	}

	printf("Jmeno: ");
	scanf_s("%s", results_new->jmeno, 50);

	printf("Obec: ");
	scanf_s("%s", results_new->obec, 50);

	printf("Oteviraci hodiny: ");
	scanf_s("%s", results_new->oteviraci_hodiny, 9);

	printf("Zaviraci hodiny: ");
	scanf_s("%s", results_new->zaviraci_hodiny, 9);

	printf("Vstup (v Kc): ");
	scanf_s("%s", results_new->vstupne, 10);

	if (!OnAdd(results_new, name, true)) return false;
	system("PAUSE");
	system("cls");
	return true;
}

/*
	Input: char *name - U�ivatelsk� jm�no
		   int table[51] - Tabulka mapuj�c� vztah mezi po�ad� p�i vypisov�n� a v samotn�m souboru (pou��v� se hlavn� ve funkci search())

	Funkce vytiskne v�echny polo�ky v datab�zi (Ov�em u� nevytv��� UI).

	Output: i = 0 - Nebyl otev�en soubor nebo je pr�zdn� datab�ze.
			i != 0 - Po�et prvk� v datab�zi (pou��v� se p�i vytv��en� UI)
*/

int print_all_results(char *name, int table[51])
{
	Results results;
	FILE *f;
	char *adresa;
	int i = 0;

	system("CLS");

	printf("Posouvate se 'W' a 'S' (nahoru/dolu). Stisknete ESC pro navrat do menu. Stisknete ENTER.\n");
	printf("----------------\n");

	connect_personal_adress(&adresa, name, true);
	if (!open_file(&f, adresa, "r")) return 0;
	while ((fscanf_s(f, "%s %s %s %s %s %s",
		results.hrad_zamek, sizeof(char) * 7,
		results.jmeno, sizeof(char) * 51,
		results.obec, sizeof(char) * 51,
		results.oteviraci_hodiny, sizeof(char) * 10,
		results.zaviraci_hodiny, sizeof(char) * 10,
		results.vstupne, sizeof(char) * 10) != EOF))
	{
		table[i] = i;
		i++;
		printf("%d) %s - %s\n", i, results.jmeno, results.obec);
	}

	fclose(f);
	return i;
}

/*
	Input: Results result - Polo�ka ur�en� pro zm�nu
		   int index - Ur�uje ��dek v souboru vyhrazen� pro polo�ku
		   bool delete_update - true -> funkce byla zavol�na, aby editovala polo�ku
								false -> funkce byla zavol�na, aby smazala polo�ku
		   bool pridani_jmena - true -> Bude p�id�v�no jm�no
								false -> Nebude p�id�no jm�no

	Funkce zm�n� ur�enou polo�ku. M��e se jednat o smaz�n� nebo editaci.
	Samotn� editace prob�h� p�eps�n�m cel� b�val� datab�ze do prvn�ho pomocn�ho souboru bez

	Output - true - prob�hlo v po��dku
			 false - chyba p�i otev�r�n� souboru

*/

bool update(Results result, int index, char *name, bool delete_update, bool pridani_jmena)
{
	FILE *tmp, *file;
	char *adresa, c;
	int count = 0;
	connect_personal_adress(&adresa, name, true);

	if (!open_file(&tmp, "files/tmp.txt", "w")) return false;
	if (!open_file(&file, adresa, "r")) return false;

	while ((c = getc(file)) != EOF)
	{
		if (count != index)
		{
			putc(c, tmp);
		}

		if (c == '\n')
		{
			count++;
		}
	}

	fclose(file);
	fclose(tmp);
	remove(adresa);
	rename("files/tmp.txt", adresa);

	if (delete_update)
	{
		OnAdd(&result, name, pridani_jmena);
		printf("Polozka byla editovana.\n");
		system("PAUSE");
	}
	return true;
}

/*
	Input: Results *result - Ukazatel na polo�ku ur�enou k editaci
		   int index - ��slo ��dku v souboru
		   char *name - U�ivatelsk� jm�no

	Funkce vytv��� kostru pro editaci, tj. zaji��uje UI a pt� se u�ivatele na polo�ky, kter� chce editovat a na jejich hodnotu. (Fuknce pro samotnou editaci je zavol�na na konci t�to funkce)

	Output - true - prob�hlo v po��dku
			 false - chyba p�i otev�r�n� souboru
*/

bool edit(Results *result, int index, char *name)
{
	int curr = 3;
	char c, y_n;
	char change[51], change2[51];
	printf("\n\nEditace");
	gotoXY(curr);
	while ((c = _getch()) != 27)
	{
		if ((c == 's' || c == 'S') && curr < 7)
		{
			gotoXY(++curr);
		}
		else if ((c == 'w' || c == 'W') && curr > 3)
		{
			gotoXY(--curr);
		}
		else if (c == '\r')
		{
			system("CLS");

			switch (curr)
			{
			case 3:

				if (!strcmp(result->hrad_zamek, "zamek"))
				{
					printf("Chcete zmenit zamek na hrad? [Y/N]: ");
					y_n = _getch();

					if (y_n == 'Y' || y_n == 'y')
					{
						strcpy_s(result->hrad_zamek, sizeof(char) * 7, "hrad");
						if (!update(*result, index, name, true, false)) return false;
					}
				}
				else
				{
					printf("Chcete zmenit hrad na zamek? [Y/N]: ");
					y_n = _getch();

					if (y_n == 'Y' || y_n == 'y')
					{
						strcpy_s(result->hrad_zamek, sizeof(char) * 6, "zamek");
						if (!update(*result, index, name, true, false)) return false;
					}
				}

				break;

			case 4:

				printf("Nove jmeno: ");
				scanf_s("%s", &change, 50);

				printf("Opravdu chcete zmenit jmeno na %s? [Y/N]", change);
				y_n = _getch();

				if (y_n == 'Y' || y_n == 'y')
				{
					strcpy_s(result->jmeno, sizeof(char) * 50, change);
					if (!update(*result, index, name, true, true)) return false;
				}

				break;
			case 5:

				printf("Nova obec: ");
				scanf_s("%s", &change, 50);

				printf("Opravdu chcete zmenit jmeno obce na %s? [Y/N]", change);
				y_n = _getch();

				if (y_n == 'Y' || y_n == 'y')
				{
					strcpy_s(result->obec, sizeof(char) * 50, change);
					if (!update(*result, index, name, true, false)) return false;
				}

				break;
			case 6:

				printf("Nove oteviraci hodiny: ");
				scanf_s("%s", &change, 9);

				printf("Nove zaviraci hodiny: ");
				scanf_s("%s", &change2, 9);

				printf("Opravdu chcete zmenit oteviraci a zaviraci hodiny na %s - %s? [Y/N]", change, change2);
				y_n = _getch();

				if (y_n == 'Y' || y_n == 'y')
				{
					strcpy_s(result->oteviraci_hodiny, sizeof(char) * 9, change);
					strcpy_s(result->zaviraci_hodiny, sizeof(char) * 9, change2);

					if (!update(*result, index, name, true, false)) return false;
				}

				break;

			case 7:

				printf("Nova cena: ");
				scanf_s("%s", &change, 10);

				printf("Opravdu chcete zmenit cenu na %s? [Y/N]", change);
				y_n = _getch();

				if (y_n == 'Y' || y_n == 'y')
				{
					strcpy_s(result->vstupne, sizeof(char) * 10, change);
					if (!update(*result, index, name, true, false)) return false;
				}

				break;

			}

			break;
		}
	}
	return true;
}

/*
	Input: Results *result - Ukazatel na prvek, kter� m� b�t smaz�n.
		   int index - ��slo ��dku, na kter�m bude polo�ka smaz�na.
		   char *name - U�ivatelsk� jm�no

	Vytv��� jednoduch� UI pro pr�ci s maz�n�m. (Fuknce pro samotn� maz�n� je zavol�na na konci t�to funkce)

	Output - true - prob�hlo v po��dku
			 false - chyba p�i otev�r�n� souboru
*/

bool delete_entity(Results *result, int index, char *name)
{
	char c, y_n;
	char change[51], change2[51];
	printf("\n\nOpravdu chcete smazat polozku? [Y/N]\n");
	if ((c = _getch()) == 'Y' || c == 'y')
	{
		if (!update(*result, index, name, false, false)) return false;
		return true;
	}
	return false;
}

/*
	Input: Results result - Polo�ka ur�en� k otev�en�.
		   int index - ��slo ��dku, na kter�m se v souboru polo�ka nach�z�
		   char *name - U�ivatelsk� jm�no

	Otev�e zbyl� informace o polo�ce, kter� se v seznamu polo�ek nenach�z� (tento p��stup by byl prakti�t�j�� p�i vet��m mno�stv� atribut�).
	Vytv��� UI pro pr�ci s atributy a vol� funkce pro editaci a maz�n�.

	Output - true - prob�hlo v po��dku
			 false - chyba p�i otev�r�n� souboru (uvnit� n�jak� volan� funkce)
*/

bool open_entity(Results result, int index, char *name)
{
	char c;
	int curr = 9;

	system("CLS");
	printf("Posouvate se 'W' a 'S' (nahoru/dolu). Vraceni zpet stisknutim ESC. Polzku vyberete stisknutim ENTER.\n");
	printf("----------------\n");
	printf("Typ objektu: %s\n", result.hrad_zamek);
	printf("Jmeno %su: %s\n", result.hrad_zamek, result.jmeno);
	printf("Obec: %s\n", result.obec);
	printf("Oteviraci hodiny: %s - %s\n", result.oteviraci_hodiny, result.zaviraci_hodiny);
	printf("Cena: %s Kc\n", result.vstupne);
	printf("----------------\n");
	printf("EDITOVAT\n");
	printf("SMAZAT");

	gotoXY(curr);

	while ((c = _getch()) != 27)
	{
		if ((c == 's' || c == 'S') && curr == 9)
		{
			gotoXY(++curr);
		}
		else if ((c == 'w' || c == 'W') && curr == 10)
		{
			gotoXY(--curr);
		}
		else if (c == '\r')
		{
			if (curr == 9)
			{
				if (!edit(&result, index, name))
				{
					printf("Polozka nebyla editovana.\n");
				}
				gotoXY(11);
				//system("PAUSE");
				system("CLS");
				printf("Posouvate se 'W' a 'S' (nahoru/dolu). Vraceni zpet stisknutim ESC. Polzku vyberete stisknutim ENTER.\n");
				printf("----------------\n");
				printf("Typ objektu: %s\n", result.hrad_zamek);
				printf("Jmeno %su: %s\n", result.hrad_zamek, result.jmeno);
				printf("Obec: %s\n", result.obec);
				printf("Oteviraci hodiny: %s - %s\n", result.oteviraci_hodiny, result.zaviraci_hodiny);
				printf("Cena: %s Kc\n", result.vstupne);
				printf("----------------\n");
				printf("EDITOVAT\n");
				printf("SMAZAT");

				gotoXY(curr);
			}
			else if (curr == 10)
			{
				if (!delete_entity(&result, index, name))
				{
					printf("Polozka nebyla smazana.\n");
				}
				else
				{
					printf("Polozka byla smazana.\n");
				}

				system("PAUSE");
				break;
			}
		}
	}

	system("CLS");

	return true;
}

/*
	Input: char *name - U�ivatelsk� jm�no
		   char *str2 - Hledan� �et�zec
		   int *table - Tabulka vytv��ej�c� vztah mezi po�ad�m ��dku p�i vyps�n� seznamu polo�ek a ulo�en�m v souboru

	Vyhled� prvek v souboru podle jm�na hradu/z�mku a n�sledn� v�sledky vyp�e. P�i hledn�n� existuje p�esn� dan� mal� odchylka pro p��pad p�eklepu.

	Output: i = 0 - Nebyl otev�en soubor nebo nic nebylo nalezeno.
			i != 0 - Po�et vyhledan�ch prvk� v datab�zi (pou��v� se p�i vytv��en� UI)
*/

int search(char *name, char *str2, int *table)
{
	Results *result = (Results*)malloc(sizeof(Results));
	FILE *f;
	int results_count = 0, i = 0, k = 0;
	char *adresa;

	system("CLS");

	printf("Posouvate se 'W' a 'S' (nahoru/dolu). Stisknete ESC pro navrat do menu. Stisknete ENTER.\n");
	printf("----------------\n");

	connect_personal_adress(&adresa, name, true);
	if (!open_file(&f, adresa, "r")) return false;

	while ((fscanf_s(f, "%s %s %s %s %s %s",
		result->hrad_zamek, sizeof(char) * 7,
		result->jmeno, sizeof(char) * 51,
		result->obec, sizeof(char) * 51,
		result->oteviraci_hodiny, sizeof(char) * 10,
		result->zaviraci_hodiny, sizeof(char) * 10,
		result->vstupne, sizeof(char) * 10) != EOF))
	{
		if (custom_strcmp(result->jmeno, str2) > strlen(str2)*0.5)
		{
			printf("%d) %s - %s\n", k + 1, result->jmeno, result->obec);
			table[k] = i;
			k++;
		}
		i++;
	}

	fclose(f);
	free(result);
	return k;
}

/*
	Input: char *name - U�ivatelsk� jm�no
		   bool all - true - v�sledek vyps�n� cel� datab�ze
					  false - v�sledek vyps�n� vyhled�v�n�

	Vytvo�� UI pro pr�ci s datab�z�. D� se pou��t jak p�i hled�n�, tak p�i jednoduch�m vypisov�n�. Vol� funkci pro v�pis bli���ch detail� k polo�ce.

	Output: true - pr�ce prob�hla �sp�n�
			false - datab�ze je pr�zdn�
*/

bool print_db(char *name, bool all)
{
	Results result;
	FILE *f;
	int table[51];
	char c, *adresa, search_str[51];
	int curr = 3;
	int no_items;

	if (all)
	{
		no_items = print_all_results(name, table);
		if (!no_items) return false;
	}
	else
	{
		printf("Co chcete hledat?: ");
		scanf_s("%s", search_str, 51);
		no_items = search(name, search_str, table);
		if (!no_items) return false;
	}

	gotoXY(curr);

	while ((c = _getch()) != 27)
	{
		if ((c == 's' || c == 'S') && curr < (no_items + 2))
		{
			gotoXY(++curr);
		}
		else if ((c == 'w' || c == 'W') && curr > 3)
		{
			gotoXY(--curr);
		}
		else if (c == '\r')
		{
			connect_personal_adress(&adresa, name, true);
			if (!open_file(&f, adresa, "r")) return false;
			for (int i = 0; (i < table[curr - 3] + 1) && (fscanf_s(f, "%s %s %s %s %s %s",
				result.hrad_zamek, sizeof(char) * 7,
				result.jmeno, sizeof(char) * 51,
				result.obec, sizeof(char) * 51,
				result.oteviraci_hodiny, sizeof(char) * 10,
				result.zaviraci_hodiny, sizeof(char) * 10,
				result.vstupne, sizeof(char) * 10) != EOF); i++);

			fclose(f);
			open_entity(result, table[curr - 3], name);

			if (all)
			{
				no_items = print_all_results(name, table);
				if (!no_items) return false;
			}
			else
			{
				search(name, search_str, table);
			}

			curr = 3;
			gotoXY(curr);
		}
	}

	system("CLS");

	return true;
}

/*
	Input: char *name - U�ivatelsk� jm�no

	P�id� prvky z extern�ho souboru (dle v�b�ru u�tivatele) do ofici�ln� datab�ze. Extern� datab�ze mus� spl�ovat stejn� standardy jako ofici�ln� datab�ze.
	Zept� se u�ivatele na jm�no souboru (celou cestu) a n�sledn� podle abecedy vlo�� jednotliv� polo�ky do ofici�ln� datab�ze.

	Output - true - prob�hlo v po��dku
			 false - chyba p�i otev�r�n� souboru (uvnit� n�jak� volan� funkce)
*/

bool add_another_database(char *name)
{
	FILE *f;
	Results *result = (Results*)malloc(sizeof(Results));
	char file_name[45];
	system("CLS");

	printf("Jmeno textoveho souboru: ");
	scanf_s("%s", file_name, 44);

	if (!open_file(&f, file_name, "r")) return false;

	while ((fscanf_s(f, "%s %s %s %s %s %s",
		result->hrad_zamek, sizeof(char) * 7,
		result->jmeno, sizeof(char) * 51,
		result->obec, sizeof(char) * 51,
		result->oteviraci_hodiny, sizeof(char) * 10,
		result->zaviraci_hodiny, sizeof(char) * 10,
		result->vstupne, sizeof(char) * 10) != EOF))
	{
		if (!OnAdd(result, name, true)) return false;
	}
	system("PAUSE");
	system("cls");
	fclose(f);
	return true;
}

/*
	Input: char *name - u�ivatelsk� jm�no (p�ed�n� odkazem)

	Vyp�e menu pro pr�ci se samotnou datab�z�. Tak� vol� ostatn� funkce pro pr�ci se samotnou datab�z� a vypisuje chybov� hl�ky.

	Output: true - v�e v po��dku
*/

bool db_main(char *name)
{
	int db_choice = 0;
	do {
		printf("Databaze uzivatele %s.\n", name);
		printf("Stisknete: 1 - Vlozeni noveho hradu/zamku; 2 - Vypsani databaze; 3 - Vyhledani v databazi; 4 - Pridani souboru do databaze; [x>4] do hlavniho menu.\n");
		scanf_s("%d", &db_choice);
		switch (db_choice)
		{
		case 1:
			system("CLS");
			if (!add(name)) printf("ERROR: Polozka nebyla pridana.\n");
			break;
		case 2:
			if (!print_db(name, true)) printf("Prazdna databaze.\n");
			break;
		case 3:
			if (!print_db(name, false)) printf("Nic nebylo nalezeno.\n");
			break;
		case 4:
			if (!add_another_database(name)) printf("Soubor nebyl nalezen.\n");
			break;
		}
	} while (db_choice <= 4);

	system("CLS");
	return true;
}

/*
	Input: char *name - p�ed�n�m odkazem u�ivatelsk�ho jm�na

	Funkce zajist� informace o autorizaci a autentizuje u�ivatele.

	Output: false - U�ivatel se rozhodl ukon�it proces p�ihla�ov�n� (p�ihl�en� se nepovedlo)
			true - U�ivatel byl p�ihl�en jako standartn� u�ivatel.

*/

bool login(char *name)
{
	int r = 0;
	char password[21], a[21], b[21], admin_password[21];
	FILE *file;

	while (!r)
	{
		if (!open_file(&file, "files/login.txt", "r")) return false;

		printf("Jmeno: ");
		scanf_s("%s", name, 20);

		printf("Heslo (pokud si heslo nepamatejete, napiste \"end\"): ");
		scanf_s("%s", password, 20);

		if (!strcmp(password, "end")) return false;

		//string a, b;
		while (fscanf_s(file, "%s %s", a, sizeof(char) * 21, b, sizeof(char) * 21) != EOF)
		{
			if (!strcmp(a, name) && !strcmp(b, password)) { fclose(file); return true; }
		}

		fclose(file);
		printf("Spatne jmeno nebo heslo.\n");
	}

	return false;
}

/*
	Registruje u�ivatele a vytvo�� mu osobn� soubor s budouc� datb�z� hrad� a z�mk�.

	Output: True - u�ovatel byl registrov�n
			False - u�ivatle nem��e b�t registrov�n (nej�ast�ji nen� vytvo�en soubor files/login.txt)
*/

bool registation()
{
	FILE *chfile, *concfile, *file;
	char a[21], b[21], *adresa;
	int r = 1, adresa_size, size;
	char name[21], password[21];

	while (r)
	{
		if (!open_file(&chfile, "files/login.txt", "r")) return false;
		printf("Jmeno: ");
		scanf_s("%20s", name, 20);

		r = 0;
		while (fscanf_s(chfile, "%s %s", a, sizeof(char) * 21, b, sizeof(char) * 21) != EOF)  //Test jestli ji� nen� v datab�zi stejn� u�ivatelsk� jm�no.
		{
			if (!strcmp(a, name)) {
				r = 1; printf("Zadejte jine jmeno.\n"); break;
			}

			if (!strcmp(b, "end")) {
				r = 1; printf("Zadejte jine heslo.\n"); break;   //Heslo nem��e b�t "end" proto�e je tato hodnota vyhrazena pro zru�en� p�ihl�en� ve funkci "login"
			}
		}
		fclose(chfile);
	}

	printf("Heslo: ");
	scanf_s("%s", password, 20);

	connect_personal_adress(&adresa, name, true);

	if (!open_file(&concfile, adresa, "w")) return false;										//Vytvo�en� souboru s budouc� datab�z� hrad� pro dan�ho u�ivatele
	fclose(concfile);

	if (!open_file(&file, "files/login.txt", "a")) return false;
	fprintf(file, "%s %s\n", name, password);													//Zaps�n� jm�na a hesla do datab�ze p�ihla�ovac�ch �daj�
	printf("Byl jste registrovan pod jmenem %s.\n", name);

	fclose(file);
	system("PAUSE");
	system("CLS");

	return true;
}