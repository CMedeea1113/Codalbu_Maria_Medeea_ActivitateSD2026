#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraStudent {
	int id;
	int varsta;
	float medie;
	char* nume;
	char* facultate;
	unsigned char grupa;
};
typedef struct StructuraStudent Student;

struct Nod {
	Student info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

int maxim(int a, int b) {
	return (a > b ? a : b);
}

int calculeazaInaltimeArbore(Nod* rad) {
	if (rad) {
		return maxim(
			calculeazaInaltimeArbore(rad->st),
			calculeazaInaltimeArbore(rad->dr)
		) + 1;
	}
	return 0;
}

int calculDiferentaInaltimi(Nod* rad) {
	if (rad != NULL) {
		return calculeazaInaltimeArbore(rad->st) - calculeazaInaltimeArbore(rad->dr);
	}
	return 0;
}

void afisareStudent(Student student) {
	printf("Id: %d\n", student.id);
	printf("Varsta: %d\n", student.varsta);
	printf("Medie: %.2f\n", student.medie);
	printf("Nume: %s\n", student.nume);
	printf("Facultate: %s\n", student.facultate);
	printf("Grupa: %c\n\n", student.grupa);
}

Student initStudent(int id, int varsta, float medie, const char* nume,
	const char* facultate, unsigned char grupa) {

	Student s;
	s.id = id;
	s.varsta = varsta;
	s.medie = medie;
	s.grupa = grupa;

	s.nume = (char*)malloc(strlen(nume) + 1);
	strcpy(s.nume, nume);

	s.facultate = (char*)malloc(strlen(facultate) + 1);
	strcpy(s.facultate, facultate);

	return s;
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

void adaugaStudentInArbore(Nod** rad, Student studentNou) {
	if (*rad == NULL) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = studentNou;
		nod->st = NULL;
		nod->dr = NULL;
		*rad = nod;
	}
	else {
		if ((*rad)->info.id > studentNou.id) {
			adaugaStudentInArbore(&((*rad)->st), studentNou);
		}
		else if ((*rad)->info.id < studentNou.id) {
			adaugaStudentInArbore(&((*rad)->dr), studentNou);
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
		afisareStudent(rad->info);
		afisareInordine(rad->dr);
	}
}

void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareStudent(rad->info);
		afisarePreordine(rad->st);
		afisarePreordine(rad->dr);
	}
}

Student getStudentByID(Nod* rad, int id) {
	Student s;
	s.id = -1;
	s.nume = NULL;
	s.facultate = NULL;

	if (rad) {
		if (rad->info.id == id) {
			s = rad->info;

			s.nume = (char*)malloc(strlen(rad->info.nume) + 1);
			strcpy(s.nume, rad->info.nume);

			s.facultate = (char*)malloc(strlen(rad->info.facultate) + 1);
			strcpy(s.facultate, rad->info.facultate);
		}
		else if (id < rad->info.id) {
			s = getStudentByID(rad->st, id);
		}
		else {
			s = getStudentByID(rad->dr, id);
		}
	}

	return s;
}

int determinaNumarNoduri(Nod* rad) {
	if (rad) {
		return determinaNumarNoduri(rad->st) +
			determinaNumarNoduri(rad->dr) + 1;
	}
	return 0;
}

float calculeazaSumaMediilor(Nod* rad) {
	if (rad) {
		return rad->info.medie +
			calculeazaSumaMediilor(rad->st) +
			calculeazaSumaMediilor(rad->dr);
	}
	return 0;
}

float calculeazaSumaMediilorDinFacultate(Nod* rad, const char* facultate) {
	if (rad) {
		float suma = calculeazaSumaMediilorDinFacultate(rad->st, facultate) +
			calculeazaSumaMediilorDinFacultate(rad->dr, facultate);

		if (strcmp(rad->info.facultate, facultate) == 0) {
			suma += rad->info.medie;
		}

		return suma;
	}
	return 0;
}

void dezalocareArboreStudenti(Nod** rad) {
	if (*rad) {
		dezalocareArboreStudenti(&(*rad)->st);
		dezalocareArboreStudenti(&(*rad)->dr);

		free((*rad)->info.nume);
		free((*rad)->info.facultate);
		free(*rad);

		*rad = NULL;
	}
}

int main() {
	Nod* rad = NULL;

	adaugaStudentInArbore(&rad, initStudent(1, 21, 8.75, "Ion Popescu", "Aerospatiala", 'A'));
	adaugaStudentInArbore(&rad, initStudent(2, 22, 9.10, "Maria Ionescu", "Automatica", 'B'));
	adaugaStudentInArbore(&rad, initStudent(3, 20, 7.80, "Andrei Georgescu", "Aerospatiala", 'C'));
	adaugaStudentInArbore(&rad, initStudent(4, 23, 9.50, "Elena Marin", "Transporturi", 'D'));
	adaugaStudentInArbore(&rad, initStudent(5, 21, 8.20, "Mihai Dinu", "Energetica", 'E'));

	printf("Afisare preordine:\n");
	afisarePreordine(rad);

	printf("Student cautat:\n");
	Student studentCautat = getStudentByID(rad, 4);

	if (studentCautat.id != -1) {
		afisareStudent(studentCautat);

		free(studentCautat.nume);
		free(studentCautat.facultate);
	}
	else {
		printf("Studentul nu a fost gasit.\n");
	}

	printf("Numar noduri: %d\n", determinaNumarNoduri(rad));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(rad));
	printf("Suma mediilor: %.2f\n", calculeazaSumaMediilor(rad));

	printf("Suma mediilor studentilor de la Aerospatiala: %.2f\n",
		calculeazaSumaMediilorDinFacultate(rad, "Aerospatiala"));

	dezalocareArboreStudenti(&rad);

	return 0;
}