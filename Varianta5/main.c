#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraZbor {
    int id;
    int durataMinute;
    float pretBilet;
    char* destinatie;
    unsigned char terminal;
};
typedef struct StructuraZbor Zbor;

struct Nod {
    Zbor zbor;
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

Zbor citireZborDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";

    fgets(buffer, 100, file);

    char* aux;
    Zbor z;

    aux = strtok(buffer, sep);
    z.id = atoi(aux);

    z.durataMinute = atoi(strtok(NULL, sep));
    z.pretBilet = (float)atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    z.destinatie = (char*)malloc(strlen(aux) + 1);
    strcpy(z.destinatie, aux);

    z.terminal = *strtok(NULL, sep);

    return z;
}

void afisareZbor(Zbor zbor) {
    printf("Id zbor: %d\n", zbor.id);
    printf("Durata (minute): %d\n", zbor.durataMinute);
    printf("Pret bilet: %.2f\n", zbor.pretBilet);
    printf("Destinatie: %s\n", zbor.destinatie);
    printf("Terminal: %c\n\n", zbor.terminal);
}

void afisareListaZboruriDeLaInceput(ListaDubla lista) {
    Nod* p = lista.first;
    while (p) {
        afisareZbor(p->zbor);
        p = p->next;
    }
}

void afisareListaZboruriDeLaSfarsit(ListaDubla lista) {
    Nod* p = lista.last;
    while (p) {
        afisareZbor(p->zbor);
        p = p->prev;
    }
}

void adaugaZborInLista(ListaDubla* lista, Zbor zborNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->zbor = zborNou;
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

void adaugaLaInceputInLista(ListaDubla* lista, Zbor zborNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->zbor = zborNou;
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

ListaDubla citireLDZboruriDinFisier(const char* numeFisier) {
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
            adaugaZborInLista(&lista, citireZborDinFisier(f));
        }
    }

    fclose(f);
    return lista;
}

void dezalocareLDZboruri(ListaDubla* lista) {
    Nod* p = lista->first;
    while (p != NULL) {
        Nod* aux = p;
        p = p->next;
        free(aux->zbor.destinatie);
        free(aux);
    }

    lista->first = NULL;
    lista->last = NULL;
    lista->nrNoduri = 0;
}

float calculeazaPretMediu(ListaDubla lista) {
    if (lista.nrNoduri == 0) {
        return 0;
    }
    else {
        float suma = 0;
        Nod* p = lista.first;
        while (p != NULL) {
            suma += p->zbor.pretBilet;
            p = p->next;
        }
        return suma / lista.nrNoduri;
    }
}

void stergeZborDupaID(ListaDubla* lista, int id) {
    if (lista->first == NULL) {
        return;
    }

    Nod* p = lista->first;
    while (p && p->zbor.id != id) {
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

    free(p->zbor.destinatie);
    free(p);
    lista->nrNoduri--;
}

char* getDestinatieCuPretMaxim(ListaDubla lista) {
    if (lista.first) {
        Nod* max = lista.first;
        Nod* p = lista.first->next;

        while (p) {
            if (p->zbor.pretBilet > max->zbor.pretBilet) {
                max = p;
            }
            p = p->next;
        }

        char* destinatie = (char*)malloc(strlen(max->zbor.destinatie) + 1);
        strcpy(destinatie, max->zbor.destinatie);
        return destinatie;
    }
    else {
        return NULL;
    }
}

int main() {
    ListaDubla lista = citireLDZboruriDinFisier("zboruri.txt");
    afisareListaZboruriDeLaInceput(lista);

    printf("\n\nAfisare de la sfarsit:\n");
    afisareListaZboruriDeLaSfarsit(lista);

    printf("\n\nPret mediu bilete: %.2f\n", calculeazaPretMediu(lista));

    char* destinatie = getDestinatieCuPretMaxim(lista);
    if (destinatie != NULL) {
        printf("Destinatia cu pretul maxim este: %s\n", destinatie);
        free(destinatie);
    }

    stergeZborDupaID(&lista, 2);
    printf("\n\nDupa stergere:\n");
    afisareListaZboruriDeLaInceput(lista);

    dezalocareLDZboruri(&lista);

    return 0;
}