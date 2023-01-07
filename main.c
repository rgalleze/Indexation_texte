//
// Created by rgalleze on 03/05/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "tp4.h"

int main() {

    t_Index *index = creer_index();
    int selection;                                                                                              // ici est définie la variable utile lors de la navigation du menu (ex : selection = choix de l'utilisateur)
    char fname[MAX_WORD_SIZE],mot[MAX_WORD_SIZE],fnameOut[MAX_WORD_SIZE];
    t_Noeud *temp;
    do{                                                                                                       // tant que selection !=8 on reste dans le menu
        printf("\n1. Lecture et chargement d'un fichier dans un index.");
        printf("\n2. Afficher les caracteristiques de l'index.");
        printf("\n3. Afficher l'index.");
        printf("\n4. Rechercher un mot.");
        printf("\n5. Afficher le mot avec le maximum d'apparitions");
        printf("\n6. Afficher les occurrences d'un mot.");
        printf("\n7. Construire le texte a partir de l'index.");
        printf("\n8. Quitter.");
        printf("\nVeuillez saisir votre choix :");
        scanf("%d",&selection);                                                                     // On récupère le choix de l'utilisateur
        fflush(stdin);
        switch(selection)
        {

            case 1 :                                                                                        // Indexation d'un fichier texte
                if(index->racine!=NULL) libereIndex(index);                                                 // Si l'index n'est pas vide, on le libère
                index = creer_index();                                                                      // On en crée un nouveau
                printf("Veuillez saisir le fichier a lire :");
                scanf("%s",&fname);
                fflush(stdin);
                printf("\n---------------------------------------------");
                printf("\nLe nombre de mots lus dans le fichier est : %d",indexer_fichier(index,fname));
                printf("\n---------------------------------------------");
                break;

            case 2 :                                                                                        // Affichage des caractéristiques d'un index
                printf("\n---------------------------------------------");
                if(index->racine!=NULL){
                    printf("\nCaracteristiques de l'index : ");
                    printf("\n->Nombre de mots : %d",index->nb_mots_total);
                    printf("\n->Nombre de mots differents : %d",index->nb_mots_differents);
                    printf("\n->Nombre de phrases : %d",index->nb_phrases); // optionnel car ajouté
                    if(equilibre(index->racine)) printf("\n->L'arbre est equilibre.");
                    else printf("\n->L'arbre n'est pas equilibre.");
                }
                else printf("\nL'index est vide.");
                printf("\n---------------------------------------------");
                break;

            case 3:                                                                             // Affichage de l'index
                if(index->racine!=NULL){
                    afficher_index(index);
                    break;
                }
                else
                {
                    printf("\n---------------------------------------------");
                    printf("\nL'index est vide.");
                    printf("\n---------------------------------------------");
                }
                break;

            case 4 :                                                                            // Affichage des positions d'un mot dans le texte
                printf("Veuillez saisir le mot a rechercher :");
                scanf("%s",&mot);
                fflush(stdin);
                temp= rechercher_mot(index,mot);
                printf("\n---------------------------------------------");
                if(temp!=NULL){
                    printf("\nLe mot \"%s\" apparait aux positions : ",mot);
                    printf("\n(ligne,ordre,phrase)");
                    afficher_liste_positions(&temp->positions);
                }
                else printf("\nLe mot n'est pas dans l'index.\n");
                printf("---------------------------------------------");
                break;

            case 5 :                                                                            // Affichage du mot apparaissant le plus de fois
                printf("\n---------------------------------------------");
                afficher_max_apparition(index);
                printf("---------------------------------------------");
                break;

            case 6 :                                                                            // Affichage des phrases dans lesquelles se trouve le mot entré par l'utilisateur
                printf("Veuillez saisir le mot a rechercher :");
                scanf("%s",&mot);
                fflush(stdin);
                printf("\n---------------------------------------------");
                afficher_occurrences_mot(index, mot);
                printf("\n---------------------------------------------");

                break;

            case 7:                                                                             // Affichage du texte à écrire et écriture sur le fichier que l'utilisateur saisi
                printf("Veuillez saisir le fichier sur lequel ecrire :");
                scanf("%s",&fnameOut);
                fflush(stdin);
                printf("\n--------------------TEXTE ECRIT---------------------\n");
                construire_texte(index,fnameOut);
                printf("\n--------------------TEXTE ECRIT---------------------");
                break;

            default: break;
        }

    }while(selection != 8);                                                                     // Si on quitte le menu, on libère l'index
    if(index!=NULL) libereIndex(index);
    printf("\n--->Au revoir.");
    return 0;

}
