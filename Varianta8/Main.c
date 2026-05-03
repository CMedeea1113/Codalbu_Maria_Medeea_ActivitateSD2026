#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraStudent {
    int id;
    int varsta;
    float medie;
    char* numeStudent;
    char* facultate;
    unsigned char formaInvatamant;
};

typedef struct StructuraStudent Student;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
    Student info;
    Nod* next;
    Nod* prev;
};

struct ListaDubla {
    Nod* prim;
    Nod* ultim;
};

Student citireStudentDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Student s;

    s.id = -1;
    s.varsta = 0;
    s.medie = 0;
    s.numeStudent = NULL;
    s.facultate = NULL;
    s.formaInvatamant = '-';

    if (fgets(buffer, 100, file) == NULL) {
        return s;
    }

    aux = strtok(buffer, sep);
    if (aux == NULL) {
        return s;
    }

    s.id = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        s.id = -1;
        return s;
    }
    s.varsta = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        s.id = -1;
        return s;
    }
    s.medie = atof(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        s.id = -1;
        return s;
    }
    s.numeStudent = (char*)malloc(strlen(aux) + 1);
    strcpy(s.numeStudent, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        free(s.numeStudent);
        s.numeStudent = NULL;
        s.id = -1;
        return s;
    }
    s.facultate = (char*)malloc(strlen(aux) + 1);
    strcpy(s.facultate, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        free(s.numeStudent);
        free(s.facultate);
        s.numeStudent = NULL;
        s.facultate = NULL;
        s.id = -1;
        return s;
    }
    s.formaInvatamant = aux[0];

    return s;
}

void afisareStudent(Student s) {
    printf("Id: %d\n", s.id);
    printf("Varsta: %d\n", s.varsta);
    printf("Medie: %.2f\n", s.medie);
    printf("Nume student: %s\n", s.numeStudent);
    printf("Facultate: %s\n", s.facultate);
    printf("Forma invatamant: %c\n\n", s.formaInvatamant);
}

void afisareLista(ListaDubla lista) {
    Nod* p = lista.prim;

    while (p != NULL) {
        afisareStudent(p->info);
        p = p->next;
    }
}

void afisareListaInvers(ListaDubla lista) {
    Nod* p = lista.ultim;

    while (p != NULL) {
        afisareStudent(p->info);
        p = p->prev;
    }
}

void adaugaInLista(ListaDubla* lista, Student s) {
    if (s.id == -1) {
        return;
    }

    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = s;
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
            Student s = citireStudentDinFisier(file);
            adaugaInLista(lista, s);
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
        free(p->info.numeStudent);
        free(p->info.facultate);

        Nod* aux = p;
        p = p->next;
        free(aux);
    }

    free(*lista);
    *lista = NULL;
}

int main() {
    ListaDubla* lista = citireLista("studenti.txt");

    printf("Afisare lista normala:\n");
    afisareLista(*lista);

    printf("Afisare lista invers:\n");
    afisareListaInvers(*lista);

    dezalocareLista(&lista);

    return 0;
}