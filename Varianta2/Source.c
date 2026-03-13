#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

typedef struct StructuraMasina Masina;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
	Masina info;
	Nod* next;
	Nod* prev;
	};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
	};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
	m1.serie = *strtok(NULL, sep);
	return m1;


}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr.usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume Sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(ListaDubla lista) {
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;

	}
}
void afisareInversaListaMasini(ListaDubla lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;

	}
}

	void adaugaMasinaInLista(ListaDubla * lista, Masina masinaNoua) {
		Nod* nou = malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->next = nou->prev = NULL;
		if (lista->ultim) {
			nou->prev = lista->ultim;
			lista->ultim->next = nou;
			lista->ultim = nou;
		}
		else {
			lista->ultim = lista->prim = nou;
		}
	}

	ListaDubla* citireLDMasiniDinFisier(const char* numeFisier) {
		FILE* file = fopen(numeFisier, "r");
		if (file) {
			ListaDubla* lista = malloc(sizeof(ListaDubla));
			lista->prim = NULL;
			lista->ultim = NULL;
			while (!feof(file)) {
				adaugaMasinaInLista(lista, citireMasinaDinFisier(file));
			}
			fclose(file);
			return lista;
		}
		else {
			ListaDubla* lista = malloc(sizeof(ListaDubla));
			lista->prim = NULL;
			lista->ultim = NULL;
			return lista;
		}
		
	}

	void dezalocareLDMasini(ListaDubla** lista) {
		Nod* p = (*lista)->prim;
		while (p) {
			free(p->info.model);
			free(p->info.numeSofer);
			Nod* aux = p;
			p = p->next;
			free(aux);
		}
		free(*lista);
		*lista = NULL;
	}

	int main() {
		ListaDubla* lista = citireLDMasiniDinFisier("masini.txt");
		afisareInversaListaMasini(*lista);
		dezalocareLDMasini(&lista);
	}





