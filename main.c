#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#define TAILLE_MOT 6

int CHIFFREMENT(char *etat_hex, char *cle_maitre_hex, char *cipher);
int DECHIFFREMENT(char *chiffre_hex, char *cle_maitre_hex, char *clair);

void CADENCEMENT_CLE_OPTI(uint32_t m_key, uint32_t *sous_cles);
int  CHIFFREMENT_OPTI(uint32_t etat, uint32_t *sous_cles);
int  CHIFFREMENT_DOUBLE_OPTI(uint32_t message, uint32_t k1, uint32_t k2);

int main(int argc, char const *argv[])  { 
    // Vecteurs de test pour le chiffrement
    char vecteurs_test[4][3][TAILLE_MOT+1] = { {"000000", "000000", "bb57e6"},
                                             {"ffffff", "000000", "739293"},
                                             {"000000", "ffffff", "1b56ce"},
                                             {"f955b9", "d1bd2d", "47a929"} };

    uint32_t vecteurs_int[4][3] = { {0x000000, 0x000000, 0xbb57e6},
                                     {0xffffff, 0x000000, 0x739293},
                                     {0x000000, 0xffffff, 0x1b56ce},
                                     {0xf955b9, 0xd1bd2d, 0x47a929} };
    
    clock_t begin, end;
    double time_spent;
    
    char res_chiffre[TAILLE_MOT+1];
    printf("\033[33mAncien chiffrement:\n\033[00m");
    begin = clock();
    for (int i = 0; i < 4; i++)  {
        CHIFFREMENT(vecteurs_test[i][0], vecteurs_test[i][1], res_chiffre);
        if (!strcmp(vecteurs_test[i][2], res_chiffre))
            printf("\033[32mPassed: %s | %s | %s\033[00m\n", vecteurs_test[i][0], vecteurs_test[i][1], vecteurs_test[i][2]);
        else
            printf("\033[31mFailed: %s | %s | %s || %s\033[00m\n", vecteurs_test[i][0], vecteurs_test[i][1], vecteurs_test[i][2], res_chiffre);
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Temps écoulé: %f seconds\n", time_spent);
    

    printf("\033[33mNouveau chiffrement:\n\033[00m");
    size_t MAX = 2 ;
    uint32_t res;
    uint32_t sous_cles[11];
    begin = clock();
    for (int i = 0; i < MAX; i++)  {
        // Génération des 11 sous clés de 24 bits chacune via algo. cadencement de clés
        CADENCEMENT_CLE_OPTI(i, sous_cles);
        res = CHIFFREMENT_OPTI(vecteurs_int[0][0], sous_cles);
        // if (res == vecteurs_int[i][2])
        //     printf("\033[32mPassed: %06x | %06x | %06x\033[00m\n", vecteurs_int[i][0], vecteurs_int[i][1], vecteurs_int[i][2]);
        // else
        //     printf("\033[31mFailed: %06x | %06x | %06x || %06x\033[00m\n", vecteurs_int[i][0], vecteurs_int[i][1], vecteurs_int[i][2], res);
    
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Temps écoulé: %f secondes pour %ld de chiffrements\n", time_spent, MAX);
    

    
    // Chaînes de sortie des chiffrements et déchiffrements
    char res_clair[TAILLE_MOT+1];
    // char res_chiffre[TAILLE_MOT+1];

    printf("\033[33mChiffrement - Vecteurs de test:\n\033[00m");
    printf("        Clair  |  Clé   | Cipher\n");
    for (size_t i = 0; i < 4; i++)  {
        CHIFFREMENT(vecteurs_test[i][0], vecteurs_test[i][1], res_chiffre);
        if (!strcmp(vecteurs_test[i][2], res_chiffre))
            printf("\033[32mPassed: %s | %s | %s\033[00m\n", vecteurs_test[i][0], vecteurs_test[i][1], vecteurs_test[i][2]);
        else
            printf("\033[31mFailed: %s | %s | %s || %s\033[00m\n", vecteurs_test[i][0], vecteurs_test[i][1], vecteurs_test[i][2], res_chiffre);
    }

    printf("\n\033[33mDéhiffrement - Vecteurs de test:\n\033[00m");
    printf("        Cipher |  Clé   | Clair\n");
    for (size_t i = 0; i < 4; i++)  {
        DECHIFFREMENT(vecteurs_test[i][2], vecteurs_test[i][1], res_clair);
        if (!strcmp(vecteurs_test[i][0], res_clair))
            printf("\033[32mPassed: %s | %s | %s\033[00m\n", vecteurs_test[i][2], vecteurs_test[i][1], vecteurs_test[i][0]);
        else
            printf("\033[31mFailed: %s | %s | %s || %s\033[00m\n", vecteurs_test[i][2], vecteurs_test[i][1], vecteurs_test[i][0], res_clair);
    }

    printf("\n\033[33mChiffrement puis déchiffrement de messages quelconques:\n\033[00m");
    char clairs[][3][TAILLE_MOT+1] = { {"19234e", "ce98f1", "314159"},
                                       {"abc123", "456def", "abcdef"},
                                       {"198ad6", "000023", "0000aa"}
                                      };

    printf("        Clair  |  Clé   | \033[34mCipher\033[0m | \033[35mDéchiffré\033[0m\n");
    for (size_t i = 0; i < 2; i++)  {
        CHIFFREMENT(clairs[i][0], clairs[i][1], res_chiffre);
        DECHIFFREMENT(res_chiffre, clairs[i][1], res_clair);

        if (!strcmp(clairs[i][0], res_clair))
            printf("\033[32mPassed: %s | %s | \033[34m%s | \033[35m%s\033[00m\n", clairs[i][0], clairs[i][1], res_chiffre, res_clair);
        else
            printf("\033[31mFailed: %s | %s | \033[34m%s | \033[35m%s\033[00m\n", clairs[i][0], clairs[i][1], res_chiffre, res_clair);
    }
    
    
    return 0;
}

