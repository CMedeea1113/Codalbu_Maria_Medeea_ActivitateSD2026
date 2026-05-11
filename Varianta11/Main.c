#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraPacient {
	int id;
	int varsta;
	float costTratament;
	char* nume;
	char* diagnostic;
	unsigned char salon;
};
typedef struct StructuraPacient Pacient;

struct Nod {
	Pacient info;
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

Pacient initPacient(int id, int varsta, float costTratament,
	const char* nume, const char* diagnostic, unsigned char salon) {

	Pacient p;
	p.id = id;
	p.varsta = varsta;
	p.costTratament = costTratament;
	p.salon = salon;

	p.nume = (char*)malloc(strlen(nume) + 1);
	strcpy(p.nume, nume);

	p.diagnostic = (char*)malloc(strlen(diagnostic) + 1);
	strcpy(p.diagnostic, diagnostic);

	return p;
}

void afisarePacient(Pacient pacient) {
	printf("Id: %d\n", pacient.id);
	printf("Varsta: %d\n", pacient.varsta);
	printf("Cost tratament: %.2f\n", pacient.costTratament);
	printf("Nume: %s\n", pacient.nume);
	printf("Diagnostic: %s\n", pacient.diagnostic);
	printf("Salon: %c\n\n", pacient.salon);
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

void adaugaPacientInArbore(Nod** rad, Pacient pacientNou) {
	if (*rad == NULL) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = pacientNou;
		nod->st = NULL;
		nod->dr = NULL;
		*rad = nod;
	}
	else {
		if ((*rad)->info.id > pacientNou.id) {
			adaugaPacientInArbore(&((*rad)->st), pacientNou);
		}
		else if ((*rad)->info.id < pacientNou.id) {
			adaugaPacientInArbore(&((*rad)->dr), pacientNou);
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
		afisarePacient(rad->info);
		afisareInordine(rad->dr);
	}
}

void afisarePreordine(Nod* rad) {
	if (rad) {
		afisarePacient(rad->info);
		afisarePreordine(rad->st);
		afisarePreordine(rad->dr);
	}
}

Pacient getPacientByID(Nod* rad, int id) {
	Pacient p;
	p.id = -1;
	p.nume = NULL;
	p.diagnostic = NULL;

	if (rad) {
		if (rad->info.id == id) {
			p = rad->info;

			p.nume = (char*)malloc(strlen(rad->info.nume) + 1);
			strcpy(p.nume, rad->info.nume);

			p.diagnostic = (char*)malloc(strlen(rad->info.diagnostic) + 1);
			strcpy(p.diagnostic, rad->info.diagnostic);
		}
		else if (id < rad->info.id) {
			p = getPacientByID(rad->st, id);
		}
		else {
			p = getPacientByID(rad->dr, id);
		}
	}

	return p;
}

int determinaNumarNoduri(Nod* rad) {
	if (rad) {
		return determinaNumarNoduri(rad->st) +
			determinaNumarNoduri(rad->dr) + 1;
	}
	return 0;
}

float calculeazaCostTotal(Nod* rad) {
	if (rad) {
		return rad->info.costTratament +
			calculeazaCostTotal(rad->st) +
			calculeazaCostTotal(rad->dr);
	}
	return 0;
}

float calculeazaCostTratamentPentruDiagnostic(Nod* rad, const char* diagnostic) {
	if (rad) {
		float suma = calculeazaCostTratamentPentruDiagnostic(rad->st, diagnostic) +
			calculeazaCostTratamentPentruDiagnostic(rad->dr, diagnostic);

		if (strcmp(rad->info.diagnostic, diagnostic) == 0) {
			suma += rad->info.costTratament;
		}

		return suma;
	}
	return 0;
}

void dezalocareArborePacienti(Nod** rad) {
	if (*rad) {
		dezalocareArborePacienti(&(*rad)->st);
		dezalocareArborePacienti(&(*rad)->dr);

		free((*rad)->info.nume);
		free((*rad)->info.diagnostic);
		free(*rad);

		*rad = NULL;
	}
}

int main() {
	Nod* rad = NULL;

	adaugaPacientInArbore(&rad, initPacient(1, 45, 1200.50, "Ion Popescu", "Raceala", 'A'));
	adaugaPacientInArbore(&rad, initPacient(2, 62, 3500.00, "Maria Ionescu", "Diabet", 'B'));
	adaugaPacientInArbore(&rad, initPacient(3, 30, 800.75, "Andrei Marin", "Raceala", 'A'));
	adaugaPacientInArbore(&rad, initPacient(4, 51, 2700.00, "Elena Georgescu", "Hipertensiune", 'C'));
	adaugaPacientInArbore(&rad, initPacient(5, 39, 1500.25, "Mihai Dinu", "Fractura", 'D'));

	printf("Afisare preordine:\n");
	afisarePreordine(rad);

	printf("Pacient cautat:\n");
	Pacient pacientCautat = getPacientByID(rad, 4);

	if (pacientCautat.id != -1) {
		afisarePacient(pacientCautat);

		free(pacientCautat.nume);
		free(pacientCautat.diagnostic);
	}
	else {
		printf("Pacientul nu a fost gasit.\n");
	}

	printf("Numar noduri: %d\n", determinaNumarNoduri(rad));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(rad));
	printf("Cost total tratamente: %.2f\n", calculeazaCostTotal(rad));

	printf("Cost total pentru diagnosticul Raceala: %.2f\n",
		calculeazaCostTratamentPentruDiagnostic(rad, "Raceala"));

	dezalocareArborePacienti(&rad);

	return 0;
}