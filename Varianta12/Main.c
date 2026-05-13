#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCarte {
	int id;
	int nrPagini;
	float pret;
	char* titlu;
	char* autor;
	unsigned char raft;
};
typedef struct StructuraCarte Carte;

struct Nod {
	Carte info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

int maxim(int a, int b) {
	return (a > b ? a : b);
}

int calculeazaInaltimeArbore(Nod* rad) {
	if (rad) {
		return maxim(calculeazaInaltimeArbore(rad->st),
			calculeazaInaltimeArbore(rad->dr)) + 1;
	}
	return 0;
}

int calculDiferentaInaltimi(Nod* rad) {
	if (rad != NULL) {
		return calculeazaInaltimeArbore(rad->st) - calculeazaInaltimeArbore(rad->dr);
	}
	return 0;
}

Carte initCarte(int id, int nrPagini, float pret,
	const char* titlu, const char* autor, unsigned char raft) {

	Carte c;
	c.id = id;
	c.nrPagini = nrPagini;
	c.pret = pret;
	c.raft = raft;

	c.titlu = (char*)malloc(strlen(titlu) + 1);
	strcpy(c.titlu, titlu);

	c.autor = (char*)malloc(strlen(autor) + 1);
	strcpy(c.autor, autor);

	return c;
}

void afisareCarte(Carte carte) {
	printf("Id: %d\n", carte.id);
	printf("Numar pagini: %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Raft: %c\n\n", carte.raft);
}

void rotireLaStanga(Nod** rad) {
	Nod* aux = (*rad)->dr;
	(*rad)->dr = aux->st;
	aux->st = (*rad);
	(*rad) = aux;
}

void rotireLaDreapta(Nod** rad) {
	Nod* aux = (*rad)->st;
	(*rad)->st = aux->dr;
	aux->dr = (*rad);
	(*rad) = aux;
}

void adaugaCarteInArbore(Nod** rad, Carte carteNoua) {
	if (*rad == NULL) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = carteNoua;
		nod->st = NULL;
		nod->dr = NULL;
		*rad = nod;
	}
	else {
		if ((*rad)->info.id > carteNoua.id) {
			adaugaCarteInArbore(&((*rad)->st), carteNoua);
		}
		else if ((*rad)->info.id < carteNoua.id) {
			adaugaCarteInArbore(&((*rad)->dr), carteNoua);
		}
	}

	int diferentaInaltimi = calculDiferentaInaltimi(*rad);

	if (diferentaInaltimi == 2) {
		if (calculDiferentaInaltimi((*rad)->st) == -1) {
			rotireLaStanga(&(*rad)->st);
		}
		rotireLaDreapta(rad);
	}

	if (diferentaInaltimi == -2) {
		if (calculDiferentaInaltimi((*rad)->dr) == 1) {
			rotireLaDreapta(&(*rad)->dr);
		}
		rotireLaStanga(rad);
	}
}

void afisareInordine(Nod* rad) {
	if (rad) {
		afisareInordine(rad->st);
		afisareCarte(rad->info);
		afisareInordine(rad->dr);
	}
}

void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareCarte(rad->info);
		afisarePreordine(rad->st);
		afisarePreordine(rad->dr);
	}
}

Carte getCarteByID(Nod* rad, int id) {
	Carte c;
	c.id = -1;
	c.titlu = NULL;
	c.autor = NULL;

	if (rad) {
		if (rad->info.id == id) {
			c = rad->info;

			c.titlu = (char*)malloc(strlen(rad->info.titlu) + 1);
			strcpy(c.titlu, rad->info.titlu);

			c.autor = (char*)malloc(strlen(rad->info.autor) + 1);
			strcpy(c.autor, rad->info.autor);
		}
		else if (id < rad->info.id) {
			c = getCarteByID(rad->st, id);
		}
		else {
			c = getCarteByID(rad->dr, id);
		}
	}

	return c;
}

int determinaNumarNoduri(Nod* rad) {
	if (rad) {
		return determinaNumarNoduri(rad->st) +
			determinaNumarNoduri(rad->dr) + 1;
	}
	return 0;
}

float calculeazaPretTotal(Nod* rad) {
	if (rad) {
		return rad->info.pret +
			calculeazaPretTotal(rad->st) +
			calculeazaPretTotal(rad->dr);
	}
	return 0;
}

float calculeazaPretulCartilorUnuiAutor(Nod* rad, const char* autor) {
	if (rad) {
		float suma = calculeazaPretulCartilorUnuiAutor(rad->st, autor) +
			calculeazaPretulCartilorUnuiAutor(rad->dr, autor);

		if (strcmp(rad->info.autor, autor) == 0) {
			suma += rad->info.pret;
		}

		return suma;
	}
	return 0;
}

void dezalocareArboreCarti(Nod** rad) {
	if (*rad) {
		dezalocareArboreCarti(&(*rad)->st);
		dezalocareArboreCarti(&(*rad)->dr);

		free((*rad)->info.titlu);
		free((*rad)->info.autor);
		free(*rad);

		*rad = NULL;
	}
}

int main() {
	Nod* rad = NULL;

	adaugaCarteInArbore(&rad, initCarte(1, 320, 45.50, "Ion", "Liviu Rebreanu", 'A'));
	adaugaCarteInArbore(&rad, initCarte(2, 250, 39.90, "Enigma Otiliei", "George Calinescu", 'B'));
	adaugaCarteInArbore(&rad, initCarte(3, 180, 29.50, "Moara cu noroc", "Ioan Slavici", 'A'));
	adaugaCarteInArbore(&rad, initCarte(4, 410, 55.00, "Baltagul", "Mihail Sadoveanu", 'C'));
	adaugaCarteInArbore(&rad, initCarte(5, 275, 42.75, "Padurea spanzuratilor", "Liviu Rebreanu", 'D'));

	printf("Afisare preordine:\n");
	afisarePreordine(rad);

	printf("Carte cautata:\n");
	Carte carteCautata = getCarteByID(rad, 4);

	if (carteCautata.id != -1) {
		afisareCarte(carteCautata);

		free(carteCautata.titlu);
		free(carteCautata.autor);
	}
	else {
		printf("Cartea nu a fost gasita.\n");
	}

	printf("Numar noduri: %d\n", determinaNumarNoduri(rad));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(rad));
	printf("Pret total carti: %.2f\n", calculeazaPretTotal(rad));

	printf("Pret total carti scrise de Liviu Rebreanu: %.2f\n",
		calculeazaPretulCartilorUnuiAutor(rad, "Liviu Rebreanu"));

	dezalocareArboreCarti(&rad);

	return 0;
}