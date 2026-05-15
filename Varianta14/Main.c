#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//====================================================
// STRUCTURA PRINCIPALA
//====================================================

struct StructuraProdus {
	int id;
	int stoc;
	float pret;
	char* denumire;
	char* categorie;
	unsigned char raft;
};

typedef struct StructuraProdus Produs;

Produs initProdus(int id, int stoc, float pret,
	const char* denumire,
	const char* categorie,
	unsigned char raft) {

	Produs p;

	p.id = id;
	p.stoc = stoc;
	p.pret = pret;
	p.raft = raft;

	p.denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy(p.denumire, denumire);

	p.categorie = (char*)malloc(strlen(categorie) + 1);
	strcpy(p.categorie, categorie);

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

void afisareProdus(Produs p) {

	printf("\nID: %d", p.id);
	printf("\nStoc: %d", p.stoc);
	printf("\nPret: %.2f", p.pret);
	printf("\nDenumire: %s", p.denumire);
	printf("\nCategorie: %s", p.categorie);
	printf("\nRaft: %c\n", p.raft);
}

//====================================================
// HEAP
//====================================================

struct Heap {
	int lungime;
	int nrElemente;
	Produs* vector;
};

typedef struct Heap Heap;

Heap initializareHeap(int lungime) {

	Heap h;

	h.lungime = lungime;
	h.nrElemente = 0;

	h.vector = (Produs*)malloc(sizeof(Produs) * lungime);

	return h;
}

void filtreazaHeap(Heap heap, int pozitie) {

	int st = 2 * pozitie + 1;
	int dr = 2 * pozitie + 2;

	int pozMin = pozitie;

	if (st < heap.nrElemente &&
		heap.vector[st].pret < heap.vector[pozMin].pret) {

		pozMin = st;
	}

	if (dr < heap.nrElemente &&
		heap.vector[dr].pret < heap.vector[pozMin].pret) {

		pozMin = dr;
	}

	if (pozMin != pozitie) {

		Produs aux = heap.vector[pozMin];
		heap.vector[pozMin] = heap.vector[pozitie];
		heap.vector[pozitie] = aux;

		if (pozMin < (heap.nrElemente - 2) / 2) {
			filtreazaHeap(heap, pozMin);
		}
	}
}

void inserareHeap(Heap* heap, Produs produs) {

	heap->vector[heap->nrElemente] = produs;
	heap->nrElemente++;

	for (int i = (heap->nrElemente - 2) / 2; i >= 0; i--) {
		filtreazaHeap(*heap, i);
	}
}

Produs extrageHeap(Heap* heap) {

	Produs p;
	p.id = -1;

	if (heap->nrElemente > 0) {

		p = heap->vector[0];

		heap->vector[0] =
			heap->vector[heap->nrElemente - 1];

		heap->vector[heap->nrElemente - 1] = p;

		heap->nrElemente--;

		for (int i = (heap->nrElemente - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}
	}

	return p;
}

void afisareHeap(Heap heap) {

	for (int i = 0; i < heap.nrElemente; i++) {
		afisareProdus(heap.vector[i]);
	}
}

//====================================================
// AVL
//====================================================

struct NodArbore {

	Produs info;

	struct NodArbore* st;
	struct NodArbore* dr;
};

typedef struct NodArbore NodArbore;

NodArbore* citireAVLdinFisier(const char* numeFisier) {

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

void inserareAVL(NodArbore** rad, Produs produs) {

	if (*rad == NULL) {

		NodArbore* nou =
			(NodArbore*)malloc(sizeof(NodArbore));

		nou->info = produs;

		nou->st = NULL;
		nou->dr = NULL;

		*rad = nou;
	}
	else {

		if (produs.id < (*rad)->info.id) {
			inserareAVL(&(*rad)->st, produs);
		}

		if (produs.id > (*rad)->info.id) {
			inserareAVL(&(*rad)->dr, produs);
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

void afisareSRD(NodArbore* rad) {

	if (rad) {

		afisareSRD(rad->st);
		afisareProdus(rad->info);
		afisareSRD(rad->dr);
	}
}

//====================================================
// STIVA + COADA
//====================================================

struct NodLista {

	int id;

	struct NodLista* next;
	struct NodLista* prev;
};

typedef struct NodLista NodLista;

struct ListaDubla {

	NodLista* first;
	NodLista* last;
};

typedef struct ListaDubla ListaDubla;

void push(ListaDubla* ld, int id) {

	NodLista* nou =
		(NodLista*)malloc(sizeof(NodLista));

	nou->id = id;

	nou->prev = NULL;
	nou->next = ld->first;

	if (ld->first == NULL) {

		ld->first = ld->last = nou;
	}
	else {

		ld->first->prev = nou;
		ld->first = nou;
	}
}

int pop(ListaDubla* ld) {

	int id = -1;

	if (ld->first) {

		id = ld->first->id;

		NodLista* aux = ld->first;

		ld->first = ld->first->next;

		if (ld->first == NULL) {
			ld->last = NULL;
		}

		free(aux);
	}

	return id;
}

void put(ListaDubla* ld, int id) {

	NodLista* nou =
		(NodLista*)malloc(sizeof(NodLista));

	nou->id = id;

	nou->next = NULL;
	nou->prev = ld->last;

	if (ld->last == NULL) {

		ld->first = ld->last = nou;
	}
	else {

		ld->last->next = nou;
		ld->last = nou;
	}
}

//====================================================
// HASHTABLE
//====================================================

struct NodHash {

	Produs info;
	struct NodHash* next;
};

typedef struct NodHash NodHash;

struct HashTable {

	int dim;
	NodHash** vector;
};

typedef struct HashTable HashTable;

HashTable initializareHashTable(int dim) {

	HashTable ht;

	ht.dim = dim;

	ht.vector =
		(NodHash**)malloc(sizeof(NodHash*) * dim);

	for (int i = 0; i < dim; i++) {
		ht.vector[i] = NULL;
	}

	return ht;
}

int functieHash(HashTable tabela, int id) {

	return id % tabela.dim;
}

void inserareHash(HashTable tabela, Produs produs) {

	int poz = functieHash(tabela, produs.id);

	NodHash* nou =
		(NodHash*)malloc(sizeof(NodHash));

	nou->info = produs;
	nou->next = NULL;

	if (tabela.vector[poz] == NULL) {

		tabela.vector[poz] = nou;
	}
	else {

		NodHash* aux = tabela.vector[poz];

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
}

void afisareHash(HashTable tabela) {

	for (int i = 0; i < tabela.dim; i++) {

		printf("\nBucket %d:\n", i);

		NodHash* aux = tabela.vector[i];

		while (aux) {

			afisareProdus(aux->info);
			aux = aux->next;
		}
	}
}

//====================================================
// MAIN
//====================================================

int main() {

	Produs p1 =
		initProdus(1, 20, 50.5,
			"Laptop", "Electronice", 'A');

	Produs p2 =
		initProdus(2, 15, 30.0,
			"Mouse", "Periferice", 'B');

	Produs p3 =
		initProdus(3, 40, 15.0,
			"Casti", "Audio", 'C');

	Produs p4 =
		initProdus(4, 10, 80.0,
			"Monitor", "Electronice", 'D');

	Produs p5 =
		initProdus(5, 25, 25.0,
			"Tastatura", "Periferice", 'E');

	//================ HEAP ================

	printf("\n========== HEAP ==========\n");

	Heap heap = initializareHeap(10);

	inserareHeap(&heap, p1);
	inserareHeap(&heap, p2);
	inserareHeap(&heap, p3);
	inserareHeap(&heap, p4);
	inserareHeap(&heap, p5);

	afisareHeap(heap);

	printf("\nExtrage:\n");
	afisareProdus(extrageHeap(&heap));

	//================ AVL ================

	printf("\n========== AVL ==========\n");

	NodArbore* rad = NULL;

	inserareAVL(&rad, p1);
	inserareAVL(&rad, p2);
	inserareAVL(&rad, p3);
	inserareAVL(&rad, p4);
	inserareAVL(&rad, p5);

	afisareSRD(rad);

	NodArbore* rad =
		citireAVLdinFisier("spectacole.txt");

	afisareSRD(rad);

	//================ STIVA ================

	printf("\n========== STIVA ==========\n");

	ListaDubla stiva;

	stiva.first = NULL;
	stiva.last = NULL;

	push(&stiva, 10);
	push(&stiva, 20);
	push(&stiva, 30);

	printf("Pop: %d\n", pop(&stiva));

	//================ COADA ================

	printf("\n========== COADA ==========\n");

	ListaDubla coada;

	coada.first = NULL;
	coada.last = NULL;

	put(&coada, 100);
	put(&coada, 200);
	put(&coada, 300);

	printf("Dequeue: %d\n", pop(&coada));

	//================ HASHTABLE ================

	printf("\n========== HASHTABLE ==========\n");

	HashTable tabela = initializareHashTable(5);

	inserareHash(tabela, p1);
	inserareHash(tabela, p2);
	inserareHash(tabela, p3);
	inserareHash(tabela, p4);
	inserareHash(tabela, p5);

	afisareHash(tabela);

	return 0;
}