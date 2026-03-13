#include <stdio.h>
#include <malloc.h>

typedef struct Depozit Depozit;

struct Depozit {
	int id;
	float suprafata;
	int volum;
	char* denumire;
	char clasa;
};
struct Depozit initializare(int id, float suprafata, int volum, char* denumire, char clasa) {
	struct Depozit s;
	s.id = id;
	s.suprafata = suprafata;
	s.volum = volum;
	s.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(s.denumire, denumire);

	s.clasa = clasa;

	return s;
}

Depozit copiaza(Depozit d) {
	return initializare(d.id, d.suprafata, d.volum, d.denumire, d.clasa);
}

void afisare(struct Depozit s) {
	printf("%d. Depozitul %s de clasa %c are o suprafata de %5.2f si un volum de %d.\n", s.clasa, s.denumire, s.id, s.suprafata, s.volum);
}

void afisareVector(struct Depozit* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(*(vector + i));
	}
}

struct Depozit* copiazaPrimeleElemente(Depozit* vector, int nrElemente, int nrElementeCopiate) {
	if (nrElementeCopiate < nrElemente) {
		struct Depozit* vectorNou = malloc(sizeof(Depozit) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++) {
			vectorNou[i] = copiaza(vector[i]);
		}
		return vectorNou;
	}
	else {
		return NULL;
	}
}

int main() {
	struct Depozit depozit = initializare(1, 34.3, 70, "Emag", 'A');
	afisare(depozit);

	int nrDepozite = 4;
	Depozit* vector = (Depozit*)malloc(sizeof(Depozit) * nrDepozite);
	vector[0] = initializare(2, 34, 45, "DPD", 'B');
	vector[1] = initializare(3, 56, 89, "Amazon", 'A');
	vector[2] = initializare(4, 45, 96, "DSC", 'A');
	vector[3] = initializare(5, 34, 67, "Emag", 'A');
	
	afisareVector(vector, nrDepozite);
	printf("\n\n");




}


