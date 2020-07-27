#pragma once

/*
	Results
	Struktura vısledkù získanıch z databáze. Jeden øádek v texáku je jedna struktura.
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
	Input: FILE **file - pøedání odkazem ukazatele na soubor ke zpracování - tento soubor bude otevøen
		   const char *route - cesta k souboru
		   const char style[2] - zpùsob jakım má bıt soubor otevøen ("r", "w", "a", "r+", "w+", "a+")

	Funkce otevøe, inicializuje a ovìøí soubor.

	Output: (FILE *file)
			True - Soubor byl otevøen
			False - Soubor nebvyl otevøen
*/
bool open_file(FILE **file, const char *route, const char style[2]);


/*
	Input: char *name - pøedáním odkazem uivatelského jména

	Funkce zajistí informace o autorizaci a autentizuje uivatele.

	Output: false - Uivatel se rozhodl ukonèit proces pøihlašování (pøihlášení se nepovedlo)
			true - Uivatel byl pøihlášen jako standartní uivatel.

*/
bool login(char *name);


/*
	Input: char *name - uivatelské jméno (pøedání odkazem)

	Vypíše menu pro práci se samotnou databází. Také volá ostatní funkce pro práci se samotnou databází a vypisuje chybové hlášky.

	Output: true - vše v poøádku
*/
bool db_main(char *name);

/*
	Registruje uivatele a vytvoøí mu osobní soubor s budoucí datbází hradù a zámkù.

	Output: True - uovatel byl registrován
			False - uivatle nemùe bıt registrován (nejèastìji není vytvoøen soubor files/login.txt)
*/
bool registation();