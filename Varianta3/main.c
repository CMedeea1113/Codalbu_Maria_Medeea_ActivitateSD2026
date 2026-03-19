#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Film Film;

struct Film {
	int id;
	int anLansare;
	char* regizor;
	char* titlu;
};

struct Film initializare(int id, int anLansare, const char* regizor, const char* titlu) {
	struct Film s;
	s.id = id;
	s.anLansare = anLansare;

	s.regizor = (char*)malloc(sizeof(char) * (strlen(regizor) + 1));
	strcpy(s.regizor, regizor);

	s.titlu = (char*)malloc(sizeof(char) * (strlen(titlu) + 1));
	strcpy(s.titlu, titlu);

	return s;
}

Film copiaza(Film d) {
	return initializare(d.id, d.anLansare, d.regizor, d.titlu);
}

void afisare(struct Film s) {
	printf("%d. Filmul %s regizat de %s a fost lansat in anul %d.\n",
		s.id, s.titlu, s.regizor, s.anLansare);
}

void afisareVector(struct Film* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(*(vector + i));
	}
}

struct Film* copiazaPrimeleNElemente(Film* vector, int nrElemente, int nrElementeCopiate) {
	if (nrElementeCopiate < nrElemente) {
		struct Film* vectorNou = (Film*)malloc(sizeof(Film) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++) {
			vectorNou[i] = copiaza(vector[i]);
		}
		return vectorNou;
	}
	else {
		return NULL;
	}
}

void dezalocare(struct Film** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].titlu);
		free((*vector)[i].regizor);
	}
	free(*vector);
	*nrElemente = 0;
	*vector = NULL;
}

void copiazaAnumiteElemente(struct Film* vector, char nrElemente, float prag, struct Film** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].anLansare < prag) {
			(*dimensiune)++;
		}
	}

	*vectorNou = (Film*)malloc(sizeof(Film) * (*dimensiune));
	*dimensiune = 0;

	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].anLansare < prag) {
			(*vectorNou)[*dimensiune] = copiaza(vector[i]);
			(*dimensiune)++;
		}
	}
}

struct Film getPrimulElementConditionat(struct Film* vector, int nrElemente, const char* titluCautat) {
	Film s;
	s.id = -1;
	s.regizor = NULL;
	s.titlu = NULL;

	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(titluCautat, vector[i].titlu) == 0) {
			s = copiaza(vector[i]);
			return s;
		}
	}
	return s;
}

int main() {
	struct Film film = initializare(1, 1994, "Frank Darabont", "The Shawshank Redemption");
	afisare(film);

	int nrFilme = 3;
	Film* vector = (Film*)malloc(sizeof(Film) * nrFilme);

	vector[0] = initializare(2, 2010, "Christopher Nolan", "Inception");
	vector[1] = initializare(3, 1997, "James Cameron", "Titanic");
	vector[2] = initializare(4, 1972, "Francis Ford Coppola", "The Godfather");

	afisareVector(vector, nrFilme);
	printf("\n\n");

	int nrCopiate = 2;
	Film* scurt = copiazaPrimeleNElemente(vector, nrFilme, nrCopiate);

	afisareVector(scurt, nrCopiate);
	dezalocare(&scurt, &nrCopiate);

	Film* filtrat = NULL;
	int dimensiune = 0;
	copiazaAnumiteElemente(vector, nrFilme, 2000, &filtrat, &dimensiune);

	printf("\nVector filtrat:\n");
	afisareVector(filtrat, dimensiune);
	dezalocare(&filtrat, &dimensiune);

	Film filmCautat = getPrimulElementConditionat(vector, nrFilme, "Titanic");
	printf("\nFilm cautat:\n");
	if (filmCautat.id != -1) {
		afisare(filmCautat);
		free(filmCautat.titlu);
		free(filmCautat.regizor);
	}
	else {
		printf("Filmul nu a fost gasit.\n");
	}

	free(film.titlu);
	free(film.regizor);
	dezalocare(&vector, &nrFilme);

	return 0;
}