#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraRezervare {
    int id;
    int nrNopti;
    float pret;
    char* numeHotel;
    char* numeClient;
    unsigned char tipCamera;
};

typedef struct StructuraRezervare Rezervare;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
    Rezervare info;
    Nod* next;
    Nod* prev;
};

struct ListaDubla {
    Nod* prim;
    Nod* ultim;
};

Rezervare citireRezervareDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Rezervare r;

    fgets(buffer, 100, file);

    aux = strtok(buffer, sep);
    r.id = atoi(aux);

    r.nrNopti = atoi(strtok(NULL, sep));
    r.pret = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    r.numeHotel = malloc(strlen(aux) + 1);
    strcpy_s(r.numeHotel, strlen(aux) + 1, aux);

    aux = strtok(NULL, sep);
    r.numeClient = malloc(strlen(aux) + 1);
    strcpy_s(r.numeClient, strlen(aux) + 1, aux);

    r.tipCamera = *strtok(NULL, sep);

    return r;
}

void afisareRezervare(Rezervare rezervare) {
    printf("Id: %d\n", rezervare.id);
    printf("Nr. nopti: %d\n", rezervare.nrNopti);
    printf("Pret: %.2f\n", rezervare.pret);
    printf("Nume hotel: %s\n", rezervare.numeHotel);
    printf("Nume client: %s\n", rezervare.numeClient);
    printf("Tip camera: %c\n\n", rezervare.tipCamera);
}

void afisareListaRezervari(ListaDubla lista) {
    Nod* p = lista.prim;
    while (p) {
        afisareRezervare(p->info);
        p = p->next;
    }
}

void afisareInversaListaRezervari(ListaDubla lista) {
    Nod* p = lista.ultim;
    while (p) {
        afisareRezervare(p->info);
        p = p->prev;
    }
}

void adaugaRezervareInLista(ListaDubla* lista, Rezervare rezervareNoua) {
    Nod* nou = malloc(sizeof(Nod));
    nou->info = rezervareNoua;
    nou->next = NULL;
    nou->prev = NULL;

    if (lista->ultim) {
        nou->prev = lista->ultim;
        lista->ultim->next = nou;
        lista->ultim = nou;
    }
    else {
        lista->prim = nou;
        lista->ultim = nou;
    }
}

ListaDubla* citireLDRezervariDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");

    ListaDubla* lista = malloc(sizeof(ListaDubla));
    lista->prim = NULL;
    lista->ultim = NULL;

    if (file) {
        while (!feof(file)) {
            Rezervare r = citireRezervareDinFisier(file);
            adaugaRezervareInLista(lista, r);
        }

        fclose(file);
    }

    return lista;
}

void dezalocareLDRezervari(ListaDubla** lista) {
    Nod* p = (*lista)->prim;

    while (p) {
        free(p->info.numeHotel);
        free(p->info.numeClient);

        Nod* aux = p;
        p = p->next;
        free(aux);
    }

    free(*lista);
    *lista = NULL;
}

int main() {
    ListaDubla* lista = citireLDRezervariDinFisier("rezervari.txt");

    afisareInversaListaRezervari(*lista);

    dezalocareLDRezervari(&lista);

    return 0;
}