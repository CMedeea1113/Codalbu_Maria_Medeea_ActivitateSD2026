#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Carte {
	int id;
	int numarPagini;
	char titlu;
	char autor;

};

typedef struct Carte Carte;

Carte initProdus(int id, int numarPagini,
	const char* titlu,
	const char* autor) {

	Carte p;

	p.id = id;
	p.numarPagini= numarPagini;
	
	p.autor = (char*)malloc(strlen(autor) + 1);
	strcpy(p.autor, autor);

	p.titlu = (char*)malloc(strlen(titlu) + 1);
	strcpy(p.titlu, titlu);

	return p;
}

Carte citireCarteDinFisier(FILE* file) {

	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;

	Carte s;

	aux = strtok(buffer, sep);
	s.id = atoi(aux);

	s.numarPagini = atoi(strtok(NULL, sep));

	aux = strtok(NULL, sep);

	s.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(s.titlu, aux);

	s.autor = (char*)malloc(strlen(aux) + 1);
	strcpy(s.titlu, aux);

	return s;
}

void afisareProdus(Carte p) {

	printf("\nID: %d", p.id);
	printf("\nStoc: %d", p.numarPagini);
	printf("\nTitlu: %s", p.titlu);
	printf("\nAutor: %s", p.autor);
	
}

struct NodArbore {

	Carte info;

	struct NodArbore* st;
	struct NodArbore* dr;
};

typedef struct NodArbore NodArbore;


int maxim(int a, int b) {
	return a > b ? a : b;
}

int calculeazaInaltime(NodArbore* rad) {

	if (rad) {

		return maxim(
			calculeazaInaltime(rad->st),
			calculeazaInaltime(rad->dr)
		) + 1;
	}

	return 0;
}

int calculDiferentaInaltimi(NodArbore* rad) {

	if (rad) {

		return calculeazaInaltime(rad->st) -
			calculeazaInaltime(rad->dr);
	}

	return 0;
}

void rotireStanga(NodArbore** rad) {

	NodArbore* aux = (*rad)->dr;

	(*rad)->dr = aux->st;
	aux->st = (*rad);

	(*rad) = aux;
}

void rotireDreapta(NodArbore** rad) {

	NodArbore* aux = (*rad)->st;

	(*rad)->st = aux->dr;
	aux->dr = (*rad);

	(*rad) = aux;
}

void inserareAVL(NodArbore** rad, Carte Carte) {

	if (*rad == NULL) {

		NodArbore* nou =
			(NodArbore*)malloc(sizeof(NodArbore));

		nou->info = Carte;

		nou->st = NULL;
		nou->dr = NULL;

		*rad = nou;
	}
	else {

		if (Carte.id < (*rad)->info.id) {
			inserareAVL(&(*rad)->st, Carte);
		}

		if (Carte.id > (*rad)->info.id) {
			inserareAVL(&(*rad)->dr, Carte);
		}
	}

	int diferenta = calculDiferentaInaltimi(*rad);

	if (diferenta == 2) {

		if (calculDiferentaInaltimi((*rad)->st) == -1) {
			rotireStanga(&(*rad)->st);
		}

		rotireDreapta(rad);
	}

	if (diferenta == -2) {

		if (calculDiferentaInaltimi((*rad)->dr) == 1) {
			rotireDreapta(&(*rad)->dr);
		}

		rotireStanga(rad);
	}
}
NodArbore* citireAVLdinFisier(const char* numeFisier) {

	NodArbore* rad = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {

		while (!feof(f)) {

			Carte s = citireCarteDinFisier(f);

			inserareAVL(&rad, s);
		}
	}

	fclose(f);

	return rad;
}

void afisareSRD(NodArbore* rad) {

	if (rad) {

		afisareSRD(rad->st);
		afisareProdus(rad->info);
		afisareSRD(rad->dr);
	}
}


int main() {
	NodArbore* rad =
		citireAVLdinFisier("spectacole.txt");

	afisareSRD(rad);
}
