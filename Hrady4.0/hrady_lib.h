#pragma once

/*
	Results
	Struktura v�sledk� z�skan�ch z datab�ze. Jeden ��dek v tex��ku je jedna struktura.
*/

typedef struct result {

	char hrad_zamek[7];
	char jmeno[51];
	char obec[51];
	char oteviraci_hodiny[10];
	char zaviraci_hodiny[10];
	char vstupne[11];

}Results;

/*
	Input: FILE **file - p�ed�n� odkazem ukazatele na soubor ke zpracov�n� - tento soubor bude otev�en
		   const char *route - cesta k souboru
		   const char style[2] - zp�sob jak�m m� b�t soubor otev�en ("r", "w", "a", "r+", "w+", "a+")

	Funkce otev�e, inicializuje a ov��� soubor.

	Output: (FILE *file)
			True - Soubor byl otev�en
			False - Soubor nebvyl otev�en
*/
bool open_file(FILE **file, const char *route, const char style[2]);


/*
	Input: char *name - p�ed�n�m odkazem u�ivatelsk�ho jm�na

	Funkce zajist� informace o autorizaci a autentizuje u�ivatele.

	Output: false - U�ivatel se rozhodl ukon�it proces p�ihla�ov�n� (p�ihl�en� se nepovedlo)
			true - U�ivatel byl p�ihl�en jako standartn� u�ivatel.

*/
bool login(char *name);


/*
	Input: char *name - u�ivatelsk� jm�no (p�ed�n� odkazem)

	Vyp�e menu pro pr�ci se samotnou datab�z�. Tak� vol� ostatn� funkce pro pr�ci se samotnou datab�z� a vypisuje chybov� hl�ky.

	Output: true - v�e v po��dku
*/
bool db_main(char *name);

/*
	Registruje u�ivatele a vytvo�� mu osobn� soubor s budouc� datb�z� hrad� a z�mk�.

	Output: True - u�ovatel byl registrov�n
			False - u�ivatle nem��e b�t registrov�n (nej�ast�ji nen� vytvo�en soubor files/login.txt)
*/
bool registation();