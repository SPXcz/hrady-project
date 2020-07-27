// Hrady3.0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "stdio.h"
#include "hrady_lib.h"
#include "windows.h"



int main()
{
	FILE *f;
	int hlavni_menu_choice = 0;
	int db_choice = 0;
	char name[21];

	if (!open_file(&f, "files/login.txt", "a"))
	{
		if(!CreateDirectory(L"files", NULL)) return false;
		if(!open_file(&f, "files/login.txt", "a")) return false;
	}

	fclose(f);

	do
	{
		printf("Vitejte v databazem system hradu a zamku.\nZadej 1 pro prihlaseni, 2 pro registraci, [x>2] pro ukonceni: ");
		scanf_s("%d", &hlavni_menu_choice);

		switch (hlavni_menu_choice)
		{
		case 1:
			if (login(name)) {
					printf("Vitejete v databazi hradu a zamku %s!\n", name);
					system("PAUSE");
					system("CLS");
					db_main(name);
			}
			break;
		case 2:
			if (!registation()) printf("Nebyl jste registrovan.\n");
			break;
		}

	} while (hlavni_menu_choice <= 2);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
