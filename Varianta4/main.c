#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraStudent {
	int id;
	int varsta;
	float medie;
	char* nume;
	unsigned char grupa;
};
typedef struct StructuraStudent Student;

struct Nod {
	Student student;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

Student citireStudentDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;
	Student s;

	aux = strtok(buffer, sep);
	s.id = atoi(aux);

	s.varsta = atoi(strtok(NULL, sep));
	s.medie = (float)atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	s.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.nume, strlen(aux) + 1, aux);

	s.grupa = *strtok(NULL, sep);

	return s;
}

void afisareStudent(Student student) {
	printf("Id: %d\n", student.id);
	printf("Varsta: %d\n", student.varsta);
	printf("Medie: %.2f\n", student.medie);
	printf("Nume: %s\n", student.nume);
	printf("Grupa: %c\n\n", student.grupa);
}

void afisareListaStudentiDeLaInceput(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareStudent(p->student);
		p = p->next;
	}
}

void afisareListaStudentiDeLaSfarsit(ListaDubla lista) {
	Nod* p = lista.last;
	while (p) {
		afisareStudent(p->student);
		p = p->prev;
	}
}

void adaugaStudentInLista(ListaDubla* lista, Student studentNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->student = studentNou;
	nou->next = NULL;
	nou->prev = lista->last;

	if (lista->last != NULL) {
		lista->last->next = nou;
	}
	else {
		lista->first = nou;
	}

	lista->last = nou;
	lista->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lista, Student studentNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->student = studentNou;
	nou->next = lista->first;
	nou->prev = NULL;

	if (lista->first != NULL) {
		lista->first->prev = nou;
	}
	else {
		lista->last = nou;
	}

	lista->first = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDStudentiDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;

	if (f == NULL) {
		printf("Fisierul nu a putut fi deschis.\n");
		return lista;
	}

	while (!feof(f)) {
		long pozitie = ftell(f);
		char buffer[100];
		if (fgets(buffer, 100, f) != NULL) {
			fseek(f, pozitie, SEEK_SET);
			adaugaStudentInLista(&lista, citireStudentDinFisier(f));
		}
	}

	fclose(f);
	return lista;
}

void dezalocareLDStudenti(ListaDubla* lista) {
	Nod* p = lista->first;
	while (p != NULL) {
		Nod* aux = p;
		p = p->next;
		free(aux->student.nume);
		free(aux);
	}

	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

float calculeazaMedieGenerala(ListaDubla lista) {
	if (lista.nrNoduri == 0) {
		return 0;
	}
	else {
		float suma = 0;
		Nod* p = lista.first;
		while (p != NULL) {
			suma += p->student.medie;
			p = p->next;
		}
		return suma / lista.nrNoduri;
	}
}

void stergeStudentDupaID(ListaDubla* lista, int id) {
	if (lista->first == NULL) {
		return;
	}

	Nod* p = lista->first;
	while (p && p->student.id != id) {
		p = p->next;
	}

	if (p == NULL) {
		return;
	}

	if (p->prev != NULL) {
		p->prev->next = p->next;
	}
	else {
		lista->first = p->next;
	}

	if (p->next != NULL) {
		p->next->prev = p->prev;
	}
	else {
		lista->last = p->prev;
	}

	free(p->student.nume);
	free(p);
	lista->nrNoduri--;
}

char* getNumeStudentCuMediaMaxima(ListaDubla lista) {
	if (lista.first) {
		Nod* max = lista.first;
		Nod* p = lista.first->next;

		while (p) {
			if (p->student.medie > max->student.medie) {
				max = p;
			}
			p = p->next;
		}

		char* nume = (char*)malloc(strlen(max->student.nume) + 1);
		strcpy_s(nume, strlen(max->student.nume) + 1, max->student.nume);
		return nume;
	}
	else {
		return NULL;
	}
}

int main() {
	ListaDubla lista = citireLDStudentiDinFisier("studenti.txt");
	afisareListaStudentiDeLaInceput(lista);

	printf("\n\nAfisare de la sfarsit:\n");
	afisareListaStudentiDeLaSfarsit(lista);

	printf("\n\nMedia generala: %.2f\n", calculeazaMedieGenerala(lista));

	char* nume = getNumeStudentCuMediaMaxima(lista);
	if (nume != NULL) {
		printf("Studentul cu media maxima este: %s\n", nume);
		free(nume);
	}

	stergeStudentDupaID(&lista, 2);
	printf("\n\nDupa stergere:\n");
	afisareListaStudentiDeLaInceput(lista);

	dezalocareLDStudenti(&lista);

	return 0;
}