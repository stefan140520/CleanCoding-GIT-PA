// Parcurgere graf cu DFS/BFS

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Structura nod pentru lista de adiacenta
typedef struct Nod
{
    int valoare;
    struct Nod *urmator;
} NOD;

// Structura graf
typedef struct Graf
{
    int numar_varfuri;
    int *vizitat;
    NOD **lista_adiacenta;
} GRAF;

// Structura coada pentru BFS
typedef struct Coada
{
    NOD *fata;
    NOD *spate;
} COADA;

// Functie pentru crearea unui nou nod
NOD *creeaza_nod(int valoare)
{
    NOD *nod_nou = (NOD *)malloc(sizeof(NOD));
    if (nod_nou == NULL)
    {
        fprintf(stderr, "Alocare memorie esuata\n");
        exit(EXIT_FAILURE);
    }
    nod_nou->valoare = valoare;
    nod_nou->urmator = NULL;
    return nod_nou;
}

// Functie pentru crearea unui graf cu 'numar_varfuri' varfuri
GRAF *creeaza_graf(int numar_varfuri)
{
    GRAF *graf = (GRAF *)malloc(sizeof(GRAF));
    if (graf == NULL)
    {
        fprintf(stderr, "Alocare memorie esuata\n");
        exit(EXIT_FAILURE);
    }

    graf->numar_varfuri = numar_varfuri;
    graf->lista_adiacenta = (NOD **)malloc(numar_varfuri * sizeof(NOD *));
    graf->vizitat = (int *)malloc(numar_varfuri * sizeof(int));

    if (graf->lista_adiacenta == NULL || graf->vizitat == NULL)
    {
        fprintf(stderr, "Alocare memorie esuata\n");
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
    if (sursa < 0 || sursa >= graf->numar_varfuri ||
        destinatie < 0 || destinatie >= graf->numar_varfuri)
    {
        printf("Indici de varf invalizi!\n");
        return;
    }

    NOD *nod_nou = creeaza_nod(destinatie);
    nod_nou->urmator = graf->lista_adiacenta[sursa];
    graf->lista_adiacenta[sursa] = nod_nou;

    nod_nou = creeaza_nod(sursa);
    nod_nou->urmator = graf->lista_adiacenta[destinatie];
    graf->lista_adiacenta[destinatie] = nod_nou;
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

        if (sursa < 0 || sursa >= graf->numar_varfuri ||
            destinatie < 0 || destinatie >= graf->numar_varfuri)
        {
            printf("Varfuri invalide! Introduceti valori intre 0 si %d.\n",
                   graf->numar_varfuri - 1);
            i--;
            continue;
        }

        adauga_muchie(graf, sursa, destinatie);
    }
}

// Functie pentru a crea o coada goala
COADA *creeaza_coada()
{
    COADA *coada = (COADA *)malloc(sizeof(COADA));
    if (coada == NULL)
    {
        fprintf(stderr, "Alocare memorie esuata\n");
        exit(EXIT_FAILURE);
    }
    coada->fata = NULL;
    coada->spate = NULL;
    return coada;
}

// Functie pentru a verifica daca coada este goala
bool este_coada_goala(COADA *coada)
{
    return coada->fata == NULL;
}

// Functie pentru a adauga un element in coada
void adauga_in_coada(COADA *coada, int valoare)
{
    NOD *nod_nou = creeaza_nod(valoare);

    if (coada->spate == NULL)
    {
        coada->fata = nod_nou;
        coada->spate = nod_nou;
        return;
    }

    coada->spate->urmator = nod_nou;
    coada->spate = nod_nou;
}

// Functie pentru a elimina un element din coada
int elimina_din_coada(COADA *coada)
{
    if (este_coada_goala(coada))
    {
        printf("Eroare: Coada este goala!\n");
        return -1;
    }

    NOD *temp = coada->fata;
    int valoare = temp->valoare;

    coada->fata = coada->fata->urmator;

    if (coada->fata == NULL)
    {
        coada->spate = NULL;
    }

    free(temp);

    return valoare;
}

// Functie pentru a afisa graful
void afiseaza_graf(GRAF *graf)
{
    printf("\nStructura grafului (liste de adiacenta):\n");
    for (int i = 0; i < graf->numar_varfuri; i++)
    {
        printf("Varful %d: ", i);
        NOD *temp = graf->lista_adiacenta[i];
        while (temp)
        {
            printf("%d ", temp->valoare);
            temp = temp->urmator;
        }
        printf("\n");
    }
}

// Functie pentru a reseta lista de vizitare
void reseteaza_lista_vizitare(GRAF *graf)
{
    for (int i = 0; i < graf->numar_varfuri; i++)
    {
        graf->vizitat[i] = 0;
    }
}

// Functie pentru parcurgerea grafului in adancime (DFS)
void parcurgere_DFS(GRAF *graf, int varf)
{
    graf->vizitat[varf] = 1;
    printf("%d ", varf);

    NOD *temp = graf->lista_adiacenta[varf];
    while (temp != NULL)
    {
        int varf_adiacent = temp->valoare;
        if (graf->vizitat[varf_adiacent] == 0)
        {
            parcurgere_DFS(graf, varf_adiacent);
        }
        temp = temp->urmator;
    }
}

// Functie pentru parcurgerea grafului in latime (BFS)
void parcurgere_BFS(GRAF *graf, int varf_start)
{
    COADA *coada = creeaza_coada();

    graf->vizitat[varf_start] = 1;
    adauga_in_coada(coada, varf_start);

    printf("%d ", varf_start);

    while (!este_coada_goala(coada))
    {
        int varf_curent = elimina_din_coada(coada);

        NOD *temp = graf->lista_adiacenta[varf_curent];
        while (temp != NULL)
        {
            int varf_adiacent = temp->valoare;

            if (graf->vizitat[varf_adiacent] == 0)
            {
                graf->vizitat[varf_adiacent] = 1;
                adauga_in_coada(coada, varf_adiacent);
                printf("%d ", varf_adiacent);
            }

            temp = temp->urmator;
        }
    }

    free(coada);
}

// Functie pentru eliberarea memoriei grafului
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
    int numar_varfuri, numar_muchii;
    int varf_start_dfs, varf_start_bfs;

    printf("Introduceti numarul de varfuri ale grafului: ");
    scanf("%d", &numar_varfuri);

    if (numar_varfuri <= 0)
    {
        printf("Numarul de varfuri trebuie sa fie pozitiv!\n");
        return EXIT_FAILURE;
    }

    printf("Introduceti numarul de muchii ale grafului: ");
    scanf("%d", &numar_muchii);

    if (numar_muchii < 0)
    {
        printf("Numarul de muchii nu poate fi negativ!\n");
        return EXIT_FAILURE;
    }

    GRAF *graf = creeaza_graf(numar_varfuri);

    introducere_muchii(graf, numar_muchii);

    afiseaza_graf(graf);

    printf("\nIntroduceti varful de start pentru parcurgerea DFS: ");
    scanf("%d", &varf_start_dfs);

    if (varf_start_dfs < 0 || varf_start_dfs >= numar_varfuri)
    {
        printf("Varf invalid! Introduceti o valoare intre 0 si %d.\n", numar_varfuri - 1);
        elibereaza_graf(graf);
        return EXIT_FAILURE;
    }

    printf("Parcurgere DFS incepand cu varful %d: ", varf_start_dfs);
    parcurgere_DFS(graf, varf_start_dfs);
    printf("\n");

    reseteaza_lista_vizitare(graf);

    printf("\nIntroduceti varful de start pentru parcurgerea BFS: ");
    scanf("%d", &varf_start_bfs);

    if (varf_start_bfs < 0 || varf_start_bfs >= numar_varfuri)
    {
        printf("Varf invalid! Introduceti o valoare intre 0 si %d.\n", numar_varfuri - 1);
        elibereaza_graf(graf);
        return EXIT_FAILURE;
    }

    printf("Parcurgere BFS incepand cu varful %d: ", varf_start_bfs);
    parcurgere_BFS(graf, varf_start_bfs);
    printf("\n");

    elibereaza_graf(graf);

    return EXIT_SUCCESS;
}