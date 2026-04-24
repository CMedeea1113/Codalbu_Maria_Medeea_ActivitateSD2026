#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraComanda {
    int id;
    int nrProduse;
    float total;
    char* numeClient;
    char* tipMeniu;
    unsigned char tipLivrare;
};

typedef struct StructuraComanda Comanda;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
    Comanda info;
    Nod* next;
    Nod* prev;
};

struct ListaDubla {
    Nod* prim;
    Nod* ultim;
};

Comanda citireComandaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Comanda c;

    fgets(buffer, 100, file);

    aux = strtok(buffer, sep);
    if (aux == NULL) {
        c.id = -1;
        return c;
    }

    c.id = atoi(aux);
    c.nrProduse = atoi(strtok(NULL, sep));
    c.total = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    c.numeClient = malloc(strlen(aux) + 1);
    strcpy_s(c.numeClient, strlen(aux) + 1, aux);

    aux = strtok(NULL, sep);
    c.tipMeniu = malloc(strlen(aux) + 1);
    strcpy_s(c.tipMeniu, strlen(aux) + 1, aux);

    c.tipLivrare = *strtok(NULL, sep);

    return c;
}

void afisareComanda(Comanda c) {
    printf("Id: %d\n", c.id);
    printf("Nr produse: %d\n", c.nrProduse);
    printf("Total: %.2f\n", c.total);
    printf("Client: %s\n", c.numeClient);
    printf("Meniu: %s\n", c.tipMeniu);
    printf("Livrare: %c\n\n", c.tipLivrare);
}

void afisareLista(ListaDubla lista) {
    Nod* p = lista.prim;
    while (p) {
        afisareComanda(p->info);
        p = p->next;
    }
}

void afisareListaInvers(ListaDubla lista) {
    Nod* p = lista.ultim;
    while (p) {
        afisareComanda(p->info);
        p = p->prev;
    }
}

void adaugaInLista(ListaDubla* lista, Comanda c) {
    if (c.id == -1) return;

    Nod* nou = malloc(sizeof(Nod));
    nou->info = c;
    nou->next = NULL;
    nou->prev = NULL;

    if (lista->ultim) {
        nou->prev = lista->ultim;
        lista->ultim->next = nou;
        lista->ultim = nou;
    }
    else {
        lista->prim = lista->ultim = nou;
    }
}

ListaDubla* citireLista(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");

    ListaDubla* lista = malloc(sizeof(ListaDubla));
    lista->prim = NULL;
    lista->ultim = NULL;

    if (file) {
        while (!feof(file)) {
            Comanda c = citireComandaDinFisier(file);
            adaugaInLista(lista, c);
        }
        fclose(file);
    }

    return lista;
}

void dezalocareLista(ListaDubla** lista) {
    Nod* p = (*lista)->prim;

    while (p) {
        free(p->info.numeClient);
        free(p->info.tipMeniu);

        Nod* aux = p;
        p = p->next;
        free(aux);
    }

    free(*lista);
    *lista = NULL;
}

int main() {
    ListaDubla* lista = citireLista("comenzi.txt");

    afisareListaInvers(*lista);

    dezalocareLista(&lista);

    return 0;
}