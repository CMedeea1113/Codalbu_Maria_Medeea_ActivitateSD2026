#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCarte {
    int id;
    int nrPagini;
    float pret;
    char* titlu;
    char* autor;
    unsigned char categorie;
};

typedef struct StructuraCarte Carte;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
    Carte info;
    Nod* next;
    Nod* prev;
};

struct ListaDubla {
    Nod* prim;
    Nod* ultim;
};

Carte citireCarteDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Carte c;

    c.id = -1;
    c.nrPagini = 0;
    c.pret = 0;
    c.titlu = NULL;
    c.autor = NULL;
    c.categorie = '-';

    if (fgets(buffer, 100, file) == NULL) {
        return c;
    }

    aux = strtok(buffer, sep);
    if (aux == NULL) {
        return c;
    }

    c.id = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        c.id = -1;
        return c;
    }
    c.nrPagini = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        c.id = -1;
        return c;
    }
    c.pret = atof(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        c.id = -1;
        return c;
    }
    c.titlu = (char*)malloc(strlen(aux) + 1);
    strcpy(c.titlu, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        free(c.titlu);
        c.titlu = NULL;
        c.id = -1;
        return c;
    }
    c.autor = (char*)malloc(strlen(aux) + 1);
    strcpy(c.autor, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        free(c.titlu);
        free(c.autor);
        c.titlu = NULL;
        c.autor = NULL;
        c.id = -1;
        return c;
    }
    c.categorie = aux[0];

    return c;
}

void afisareCarte(Carte c) {
    printf("Id: %d\n", c.id);
    printf("Nr pagini: %d\n", c.nrPagini);
    printf("Pret: %.2f\n", c.pret);
    printf("Titlu: %s\n", c.titlu);
    printf("Autor: %s\n", c.autor);
    printf("Categorie: %c\n\n", c.categorie);
}

void afisareLista(ListaDubla lista) {
    Nod* p = lista.prim;

    while (p != NULL) {
        afisareCarte(p->info);
        p = p->next;
    }
}

void afisareListaInvers(ListaDubla lista) {
    Nod* p = lista.ultim;

    while (p != NULL) {
        afisareCarte(p->info);
        p = p->prev;
    }
}

void adaugaInLista(ListaDubla* lista, Carte c) {
    if (c.id == -1) {
        return;
    }

    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = c;
    nou->next = NULL;
    nou->prev = NULL;

    if (lista->ultim != NULL) {
        nou->prev = lista->ultim;
        lista->ultim->next = nou;
        lista->ultim = nou;
    }
    else {
        lista->prim = nou;
        lista->ultim = nou;
    }
}

ListaDubla* citireLista(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");

    ListaDubla* lista = (ListaDubla*)malloc(sizeof(ListaDubla));
    lista->prim = NULL;
    lista->ultim = NULL;

    if (file != NULL) {
        while (!feof(file)) {
            Carte c = citireCarteDinFisier(file);
            adaugaInLista(lista, c);
        }

        fclose(file);
    }
    else {
        printf("Fisierul nu a putut fi deschis.\n");
    }

    return lista;
}

void dezalocareLista(ListaDubla** lista) {
    if (lista == NULL || *lista == NULL) {
        return;
    }

    Nod* p = (*lista)->prim;

    while (p != NULL) {
        free(p->info.titlu);
        free(p->info.autor);

        Nod* aux = p;
        p = p->next;
        free(aux);
    }

    free(*lista);
    *lista = NULL;
}

int main() {
    ListaDubla* lista = citireLista("carti.txt");

    printf("Afisare lista normala:\n");
    afisareLista(*lista);

    printf("Afisare lista invers:\n");
    afisareListaInvers(*lista);

    dezalocareLista(&lista);

    return 0;
}