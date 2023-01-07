//
// Created by rgalleze on 03/05/2022.
//

#ifndef TP4_TP4_H
#define TP4_TP4_H
#define MAX_WORD_SIZE 100            // taille max qu'un mot peut avoir (nombre de caractères)
#define MAX_SENTENCE_SIZE 100000    // taille max qu'une phrase peut avoir (nombre de caractères)

//-------------------STRUCTURES---------------------------
typedef struct t_position{
    int numero_ligne;
    int ordre;
    int numero_phrase;
    struct t_position* suivant;

}t_Position;

typedef struct t_listePositions{
    t_Position* debut;
    int nb_elements;
}t_ListePositions;

typedef struct t_noeud{
    char* mot;
    int nb_occurences;
    t_ListePositions positions;
    struct t_noeud* filsGauche;
    struct t_noeud* filsDroit;
}t_Noeud;

typedef struct t_index{
    t_Noeud* racine;
    int nb_mots_differents;
    int nb_mots_total;
    int nb_phrases; // attribut ajouté pour la construction du texte à partir de l'index
}t_Index;

//-------------------STRUCTURES AJOUTEES--------------------------
typedef struct t_mot{
    char *mot;
    int numero_ligne;
    int ordre;
    struct t_mot* suivant;
}t_Mot;

typedef struct t_phrase{
    t_Mot *mot_debut;
    int numero_phrase;
    struct t_phrase* suivant;
}t_Phrase;

typedef t_Phrase * t_ListePhrase;

//-------------------FONCTIONS---------------------------
t_ListePositions* creer_liste_positions();
int ajouter_position(t_ListePositions *listeP, int ligne, int ordre, int num_phrase);
t_Index* creer_index();
t_Noeud* rechercher_mot(t_Index *index, char *mot);
int ajouter_noeud(t_Index *index, t_Noeud *noeud);
int indexer_fichier(t_Index *index, char *filename);
void afficher_index(t_Index *index);
void afficher_max_apparition(t_Index *index);
void afficher_occurrences_mot(t_Index *index, char *mot);
void construire_texte(t_Index *index, char *filename);

//-------------------FONCTIONS AJOUTEES---------------------------
void ajouter_mots_phrase(t_Noeud *racine, t_ListePhrase* liste_phrases);
t_Phrase* recherche_phrase(t_Phrase* tPhrase,int numero_phrase);
void afficher_mot(t_Mot *l);
t_Phrase* creer_phrase();
int ajouter_mot(t_Mot ** tMot,int numero_ligne,int ordre, char* mot);
t_Mot* creer_mot();
int ajouter_phrase(t_ListePhrase * ListePhrases, int numero_phrase);
void afficher_liste_positions(t_ListePositions *listeP);
t_Noeud* creer_noeud(char *mot);
void supprimer_caractere(char *str, char c);
void parcours_affichage_index(t_Noeud *racine, char* c);
void parcours_recherche_max(t_Noeud *racine, t_Noeud *nd);
void libereNoeud(t_Noeud *racine);
void libereIndex(t_Index *index);
void libereListe(t_Position *liste);
void libereMot(t_Mot* liste);
void liberePhrase(t_Phrase *phrase);
int hauteur(t_Noeud *racine);
int equilibre(t_Noeud *racine);
void transforme_to_min(char *str);

#endif //TP4_TP4_H
