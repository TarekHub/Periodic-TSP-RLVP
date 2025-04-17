#include "INCLUDE/MOW-P.h"


static char *FullName(const char *Name, double Quality);


void WriteInitTours(char *FileName, double Quality){

    FILE *ToursFile;
    Node *N, *Ni;
    int offset, calculatedDimension ;
    char *FullFileName;
    time_t Now;

    if (FileName == 0)
        return;
    FullFileName = FullName(FileName, Quality);
    Now = time(&Now);

    ToursFile = fopen(FullFileName, "w");

    for(int h = 0; h < TimeHorizon; h++){

      //calculatedDimension = GetDimension(BestSolution, h);
      //assert(calculatedDimension == BestToursDimension[h]);

      int dayDimension = BestToursDimension[h];

	  fprintf(ToursFile, "DAY = %d\n", h+1);
      fprintf(ToursFile, "LENGTH = %d\n", BestToursCost[h]);
      fprintf(ToursFile, "DIMENSION = %d\n", dayDimension);

      offset = h * (Dimension + 1);
      N = &BestSolution[offset + 1];
      for(int i = 0; i < dayDimension; i++){
          fprintf(ToursFile, "%d\n", N->Id);
          N = N->Suc;
      }
      assert(N->Id == 1);
      fprintf(ToursFile, "\n");

    }

    fclose(ToursFile);
    free(FullFileName);
}

static char *FullName(const char *Name, double Quality)
{
    char *NewName = NULL, *CostBuffer, *Pos, *TempName;

    // Rechercher le symbole "$"
    if (!(Pos = strstr(Name, "$"))) {
        NewName = (char *)calloc(strlen(Name) + 1, sizeof(char));
        if (!NewName) {
            perror("Allocation failed");
            return NULL;
        }
        strcpy(NewName, Name);
        return NewName;
    }

    // Créer un tampon pour convertir la qualité en chaîne
    CostBuffer = (char *)malloc(64 * sizeof(char)); // Suffisant pour stocker un double
    if (!CostBuffer) {
        perror("Allocation failed");
        return NULL;
    }
    snprintf(CostBuffer, 64, "%.6lf", Quality); // Conversion avec 6 décimales

    // Construire la nouvelle chaîne
    do {
        size_t PrefixLen = Pos - Name; // Longueur avant le "$"
        size_t NewLen = PrefixLen + strlen(CostBuffer) + strlen(Pos + 1) + 1;

        TempName = (char *)calloc(NewLen, sizeof(char));
        if (!TempName) {
            perror("Allocation failed");
            free(CostBuffer);
            free(NewName);
            return NULL;
        }

        // Construire la nouvelle chaîne
        strncpy(TempName, Name, PrefixLen); // Copie la partie avant "$"
        TempName[PrefixLen] = '\0';         // Terminer la chaîne
        strcat(TempName, CostBuffer);       // Ajouter la valeur de Quality
        strcat(TempName, Pos + 1);          // Ajouter la partie après "$"

        free(NewName); // Libérer l'ancienne chaîne
        NewName = TempName; // Mettre à jour avec la nouvelle chaîne
        Name = NewName; // Mettre à jour la position de départ pour recherche

    } while ((Pos = strstr(Name, "$"))); // Continuer si un autre "$" est trouvé

    free(CostBuffer); // Libérer le tampon
    return NewName;
}
