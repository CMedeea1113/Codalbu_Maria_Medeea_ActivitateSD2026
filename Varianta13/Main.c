#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraComanda {
	int id;
	int nrProduse;
	float valoare;
	char* numeClient;
	char* orasLivrare;
	unsigned char status;
};
typedef struct StructuraComanda Comanda;

struct Nod {
	Comanda info;
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

Comanda initComanda(int id, int nrProduse, float valoare,
	const char* numeClient, const char* orasLivrare, unsigned char status) {

	Comanda c;
	c.id = id;
	c.nrProduse = nrProduse;
	c.valoare = valoare;
	c.status = status;

	c.numeClient = (char*)malloc(strlen(numeClient) + 1);
	strcpy(c.numeClient, numeClient);

	c.orasLivrare = (char*)malloc(strlen(orasLivrare) + 1);
	strcpy(c.orasLivrare, orasLivrare);

	return c;
}

void afisareComanda(Comanda comanda) {
	printf("Id: %d\n", comanda.id);
	printf("Numar produse: %d\n", comanda.nrProduse);
	printf("Valoare: %.2f\n", comanda.valoare);
	printf("Nume client: %s\n", comanda.numeClient);
	printf("Oras livrare: %s\n", comanda.orasLivrare);
	printf("Status: %c\n\n", comanda.status);
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

void adaugaComandaInArbore(Nod** rad, Comanda comandaNoua) {
	if (*rad == NULL) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = comandaNoua;
		nod->st = NULL;
		nod->dr = NULL;
		*rad = nod;
	}
	else {
		if ((*rad)->info.id > comandaNoua.id) {
			adaugaComandaInArbore(&((*rad)->st), comandaNoua);
		}
		else if ((*rad)->info.id < comandaNoua.id) {
			adaugaComandaInArbore(&((*rad)->dr), comandaNoua);
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
		afisareComanda(rad->info);
		afisareInordine(rad->dr);
	}
}

void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareComanda(rad->info);
		afisarePreordine(rad->st);
		afisarePreordine(rad->dr);
	}
}

Comanda getComandaByID(Nod* rad, int id) {
	Comanda c;
	c.id = -1;
	c.numeClient = NULL;
	c.orasLivrare = NULL;

	if (rad) {
		if (rad->info.id == id) {
			c = rad->info;

			c.numeClient = (char*)malloc(strlen(rad->info.numeClient) + 1);
			strcpy(c.numeClient, rad->info.numeClient);

			c.orasLivrare = (char*)malloc(strlen(rad->info.orasLivrare) + 1);
			strcpy(c.orasLivrare, rad->info.orasLivrare);
		}
		else if (id < rad->info.id) {
			c = getComandaByID(rad->st, id);
		}
		else {
			c = getComandaByID(rad->dr, id);
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

float calculeazaValoareTotala(Nod* rad) {
	if (rad) {
		return rad->info.valoare +
			calculeazaValoareTotala(rad->st) +
			calculeazaValoareTotala(rad->dr);
	}
	return 0;
}

float calculeazaValoareaComenzilorDinOras(Nod* rad, const char* orasLivrare) {
	if (rad) {
		float suma = calculeazaValoareaComenzilorDinOras(rad->st, orasLivrare) +
			calculeazaValoareaComenzilorDinOras(rad->dr, orasLivrare);

		if (strcmp(rad->info.orasLivrare, orasLivrare) == 0) {
			suma += rad->info.valoare;
		}

		return suma;
	}
	return 0;
}

void dezalocareArboreComenzi(Nod** rad) {
	if (*rad) {
		dezalocareArboreComenzi(&(*rad)->st);
		dezalocareArboreComenzi(&(*rad)->dr);

		free((*rad)->info.numeClient);
		free((*rad)->info.orasLivrare);
		free(*rad);

		*rad = NULL;
	}
}

int main() {
	Nod* rad = NULL;

	adaugaComandaInArbore(&rad, initComanda(1, 3, 250.50, "Ion Popescu", "Bucuresti", 'L'));
	adaugaComandaInArbore(&rad, initComanda(2, 5, 430.00, "Maria Ionescu", "Cluj", 'P'));
	adaugaComandaInArbore(&rad, initComanda(3, 2, 120.75, "Andrei Marin", "Bucuresti", 'L'));
	adaugaComandaInArbore(&rad, initComanda(4, 1, 89.99, "Elena Georgescu", "Iasi", 'A'));
	adaugaComandaInArbore(&rad, initComanda(5, 4, 310.25, "Mihai Dinu", "Brasov", 'P'));

	printf("Afisare preordine:\n");
	afisarePreordine(rad);

	printf("Comanda cautata:\n");
	Comanda comandaCautata = getComandaByID(rad, 4);

	if (comandaCautata.id != -1) {
		afisareComanda(comandaCautata);

		free(comandaCautata.numeClient);
		free(comandaCautata.orasLivrare);
	}
	else {
		printf("Comanda nu a fost gasita.\n");
	}

	printf("Numar noduri: %d\n", determinaNumarNoduri(rad));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(rad));
	printf("Valoare totala comenzi: %.2f\n", calculeazaValoareTotala(rad));

	printf("Valoare totala comenzi din Bucuresti: %.2f\n",
		calculeazaValoareaComenzilorDinOras(rad, "Bucuresti"));

	dezalocareArboreComenzi(&rad);

	return 0;
}