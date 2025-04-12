#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Program pentru a determina daca exista un drum direct intre doua restaurante intr-o retea de tip graf */

typedef struct Nod
{
    int valoare;
    struct Nod *urmator;
} NOD;

typedef struct Graf
{
    int numar_varfuri;
    int *vizitat;
    NOD **lista_adiacenta;
} GRAF;

typedef struct Stiva
{
    int varf;
    int capacitate;
    int *vector;
} STIVA;

// Functie pentru crearea unui nou nod
NOD *creeaza_nod(int varf)
{
    NOD *nod_nou = (NOD *)malloc(sizeof(NOD));
    if (nod_nou == NULL)
    {
        fprintf(stderr, "Alocare de memorie esuata\n");
        exit(EXIT_FAILURE);
    }
    nod_nou->valoare = varf;
    nod_nou->urmator = NULL;
    return nod_nou;
}

// Functie pentru crearea unui graf cu 'numar_varfuri' varfuri
GRAF *creeaza_graf(int numar_varfuri)
{
    GRAF *graf = (GRAF *)malloc(sizeof(GRAF));
    if (graf == NULL)
    {
        fprintf(stderr, "Alocare de memorie esuata\n");
        exit(EXIT_FAILURE);
    }

    graf->numar_varfuri = numar_varfuri;
    graf->vizitat = (int *)malloc(numar_varfuri * sizeof(int));
    graf->lista_adiacenta = (NOD **)malloc(numar_varfuri * sizeof(NOD *));

    if (graf->vizitat == NULL || graf->lista_adiacenta == NULL)
    {
        fprintf(stderr, "Alocare de memorie esuata\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numar_varfuri; i++)
    {
        graf->lista_adiacenta[i] = NULL;
        graf->vizitat[i] = 0;
    }

    return graf;
}

// Functie pentru adaugarea unei muchii in graful neorientat
void adauga_muchie(GRAF *graf, int sursa, int destinatie)
{
    NOD *nod_nou = creeaza_nod(destinatie);
    nod_nou->urmator = graf->lista_adiacenta[sursa];
    graf->lista_adiacenta[sursa] = nod_nou;

    nod_nou = creeaza_nod(sursa);
    nod_nou->urmator = graf->lista_adiacenta[destinatie];
    graf->lista_adiacenta[destinatie] = nod_nou;
}

// Functie pentru crearea unei stive
STIVA *creeaza_stiva(int capacitate)
{
    STIVA *stiva = (STIVA *)malloc(sizeof(STIVA));
    if (stiva == NULL)
    {
        fprintf(stderr, "Alocare de memorie esuata\n");
        exit(EXIT_FAILURE);
    }

    stiva->vector = (int *)malloc(capacitate * sizeof(int));
    if (stiva->vector == NULL)
    {
        fprintf(stderr, "Alocare de memorie esuata\n");
        exit(EXIT_FAILURE);
    }

    stiva->varf = -1;
    stiva->capacitate = capacitate;

    return stiva;
}

// Functie pentru adaugarea unui element in stiva
void push(STIVA *stiva, int element)
{
    stiva->vector[++stiva->varf] = element;
}

// Functie pentru a verifica daca stiva este goala
bool este_goala(STIVA *stiva)
{
    return stiva->varf == -1;
}

// Functie pentru resetarea vectorului de vizitare
void resetare_vizitate(GRAF *graf)
{
    for (int i = 0; i < graf->numar_varfuri; i++)
    {
        graf->vizitat[i] = 0;
    }
}

// Parcurgere DFS si popularea stivei date
void parcurgere_DFS(GRAF *graf, int varf_start, STIVA *stiva)
{
    graf->vizitat[varf_start] = 1;

    push(stiva, varf_start);

    NOD *temp = graf->lista_adiacenta[varf_start];
    while (temp != NULL)
    {
        int adiacent = temp->valoare;
        if (!graf->vizitat[adiacent])
        {
            parcurgere_DFS(graf, adiacent, stiva);
        }
        temp = temp->urmator;
    }
}

// Functie pentru a verifica daca exista un drum intre doua varfuri
bool exista_drum(GRAF *graf, int start, int sfarsit)
{
    STIVA *stiva = creeaza_stiva(graf->numar_varfuri);

    resetare_vizitate(graf);

    parcurgere_DFS(graf, start, stiva);

    bool drum_exista = graf->vizitat[sfarsit] != 0;

    free(stiva->vector);
    free(stiva);

    return drum_exista;
}

// Functie pentru introducerea muchiilor de la utilizator
void introducere_muchii(GRAF *graf, int numar_muchii)
{
    int sursa, destinatie;

    printf("Introduceti %d muchii (format: sursa destinatie)\n", numar_muchii);
    printf("Nota: Varfurile sunt numerotate de la 0 la %d\n", graf->numar_varfuri - 1);

    for (int i = 0; i < numar_muchii; i++)
    {
        printf("Muchia %d: ", i + 1);
        scanf("%d %d", &sursa, &destinatie);

        if (sursa < 0 || sursa >= graf->numar_varfuri || destinatie < 0 || destinatie >= graf->numar_varfuri)
        {
            printf("Varfuri invalide! Va rugam sa introduceti valori intre 0 si %d.\n", graf->numar_varfuri - 1);
            i--;
            continue;
        }

        adauga_muchie(graf, sursa, destinatie);
    }
}

// Functie pentru eliberarea memoriei alocate pentru graf
void elibereaza_graf(GRAF *graf)
{
    if (graf == NULL)
        return;

    for (int i = 0; i < graf->numar_varfuri; i++)
    {
        NOD *curent = graf->lista_adiacenta[i];
        while (curent != NULL)
        {
            NOD *temp = curent;
            curent = curent->urmator;
            free(temp);
        }
    }

    free(graf->lista_adiacenta);
    free(graf->vizitat);
    free(graf);
}

int main()
{
    int numar_restaurante, numar_drumuri;
    int restaurant_start, restaurant_final;

    printf("Introduceti numarul de restaurante: ");
    scanf("%d", &numar_restaurante);

    if (numar_restaurante <= 0)
    {
        printf("Numarul de restaurante trebuie sa fie pozitiv.\n");
        return EXIT_FAILURE;
    }

    printf("Introduceti numarul de drumuri directe intre restaurante: ");
    scanf("%d", &numar_drumuri);

    if (numar_drumuri < 0)
    {
        printf("Numarul de drumuri nu poate fi negativ.\n");
        return EXIT_FAILURE;
    }

    GRAF *graf = creeaza_graf(numar_restaurante);

    introducere_muchii(graf, numar_drumuri);

    printf("\nVerificarea existentei drumurilor directe intre restaurante\n");
    printf("Introduceti restaurantul de pornire: ");
    scanf("%d", &restaurant_start);

    printf("Introduceti restaurantul de destinatie: ");
    scanf("%d", &restaurant_final);

    if (restaurant_start < 0 || restaurant_start >= numar_restaurante ||
        restaurant_final < 0 || restaurant_final >= numar_restaurante)
    {
        printf("Numere de restaurante invalide! Va rugam sa introduceti valori intre 0 si %d.\n", numar_restaurante - 1);
        elibereaza_graf(graf);
        return EXIT_FAILURE;
    }

    if (exista_drum(graf, restaurant_start, restaurant_final))
    {
        printf("DA! Exista un drum de la restaurantul %d la restaurantul %d.\n",
               restaurant_start, restaurant_final);
    }
    else
    {
        printf("NU! Nu exista drum de la restaurantul %d la restaurantul %d.\n",
               restaurant_start, restaurant_final);
    }

    elibereaza_graf(graf);

    return EXIT_SUCCESS;
}