#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Spectacol {
	int id;
	int nrSpectatori;
	char* titlu;
	int nrSala;

};
typedef struct Spectacol spectacol;

spectacol initProdus(int id, int nrSpectatori, int nrSala,
	const char* titlu){

	spectacol p;

	p.id = id;
	p.nrSpectatori = nrSpectatori;
	p.nrSala = nrSala;

	p.titlu = (char*)malloc(strlen(titlu) + 1);
	strcpy(p.titlu, titlu);

	

	return p;
}
spectacol citireSpectacolDinFisier(FILE* file) {

	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;

	spectacol s;

	aux = strtok(buffer, sep);
	s.id = atoi(aux);

	s.nrSpectatori = atoi(strtok(NULL, sep));

	aux = strtok(NULL, sep);

	s.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(s.titlu, aux);

	s.nrSala = atoi(strtok(NULL, sep));

	return s;
}
void afisareProdus(spectacol p) {

	printf("\nID: %d", p.id);
	printf("\nnrSpectatori: %d", p.nrSpectatori);
	printf("\nnrSala: %.d", p.nrSala);
	printf("\ntitlu: %s", p.titlu);
	}


struct NodArbore {

	spectacol info;

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

void inserareAVL(NodArbore** rad, spectacol spectacol) {

	if (*rad == NULL) {

		NodArbore* nou =
			(NodArbore*)malloc(sizeof(NodArbore));

		nou->info = spectacol;

		nou->st = NULL;
		nou->dr = NULL;

		*rad = nou;
	}
	else {

		if (spectacol.id < (*rad)->info.id) {
			inserareAVL(&(*rad)->st, spectacol);
		}

		if (spectacol.id > (*rad)->info.id) {
			inserareAVL(&(*rad)->dr, spectacol);
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

static NodArbore* citireAVLdinFisier(const char* numeFisier) {

	NodArbore* rad = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {

		while (!feof(f)) {

			spectacol s = citireSpectacolDinFisier(f);

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
