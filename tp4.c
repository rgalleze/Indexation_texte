//
// Created by rgalleze on 03/05/2022.
//

#include "tp4.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//------------------------------------------FONCTIONS CREATION DES STRUCTURES-------------------------------------------

t_ListePositions* creer_liste_positions(){                                  //crée une liste de positions vide et initialise ses attributs
    t_ListePositions * listeP = malloc(sizeof(t_ListePositions));
    if(listeP!=0){
        listeP->debut = NULL;
        listeP->nb_elements = 0;
        return listeP;
    }
    return NULL;                                                            // renvoie NULL si échec lors de l'allocation mémoire
}

t_Phrase* creer_phrase(){                                                   //crée une phrase et initialise ses attributs
    t_Phrase *phrase = malloc(sizeof(t_Phrase));
    if(phrase!=0) {
        phrase->suivant = NULL;
        phrase->mot_debut = NULL;
        return phrase;
    }
    return NULL;                                                            // renvoie NULL si échec lors de l'allocation mémoire
}

t_Mot* creer_mot(){                                                        //crée un mot et initialise ses attributs
    t_Mot *mot = malloc(sizeof(t_Mot));
    if(mot!=0) {
        mot->mot = malloc(MAX_WORD_SIZE * sizeof(char));
        mot->suivant = NULL;
        return mot;
    }
    return NULL;                                                           // renvoie NULL si échec de l'allocation mémoire
}

t_Index* creer_index(){                                                    // crée un index et initialise ses attributs
    t_Index * index = malloc(sizeof(t_Index));
    if(index!=0){
        index->racine=NULL;
        index->nb_mots_differents = 0;
        index->nb_mots_total = 0;
        index->nb_phrases=0;
        return index;
    }
    return NULL;                                                           // renvoie NULL si échec de l'allocation mémoire
}

t_Noeud* creer_noeud(char *mot){                                           //crée un nœud et initialise ses attributs
    t_Noeud *newNode = malloc((sizeof(t_Noeud)));
    if(newNode!=0) {
        newNode->mot = malloc(MAX_WORD_SIZE * sizeof(char));
        strcpy(newNode->mot,mot);
        newNode->filsDroit = NULL;
        newNode->filsGauche = NULL;
        newNode->positions = *creer_liste_positions();
        newNode->nb_occurences = 1;                                        // si un nœud est créé, cela veut dire que le mot associé a été rencontré au moins une fois
        return newNode;
    }
    return NULL;                                                           // renvoie NULL si échec de l'allocation mémoire
}

//------------------------------------------FONCTIONS D'AFFICHAGE-------------------------------------------

void afficher_liste_positions(t_ListePositions *listeP){                   // La fonction affiche les différentes positions d'un mot
    if(listeP!=NULL) {
        t_Position *temp = listeP->debut;
        printf("\n");
        while (temp != NULL) {                                             // Parcours de la liste chaînée des positions
            printf("|----(l:%d, o:%d, p:%d)\n", temp->numero_ligne, temp->ordre, temp->numero_phrase);
            temp = temp->suivant;
        }
        printf("|\n");
    }
}


void afficher_index(t_Index *index){                                       // La fonction affiche tous les mots de l'index et leurs positions
    if(index->racine!=NULL){
        printf("--------------------INDEX--------------------\n");
        char* c = malloc(strlen("azertyuiopqsdfghjklmwxcvbn")+1);          // On initialise la liste des premiers caractères des mots que l'on va afficher dans l'index
        strcpy(c,"azertyuiopqsdfghjklmwxcvbn");
        parcours_affichage_index(index->racine, c);
        printf("---------------------------------------------");
    }
}


void parcours_affichage_index(t_Noeud *racine, char* c) {                   // La fonction parcours les nœuds de l'index (parcours INFIXE, pour conserver l'ordre lexicographique) et affiche les mots et leurs positions
    if(racine==NULL){
        printf("");
    }
    else{
        t_Noeud *tmp=racine;
        parcours_affichage_index(tmp->filsGauche,c);                     // On parcourt les éléments de gauche
        if(strchr(c,tmp->mot[0])!=0){                                    // Si la première lettre du nœud est dans la liste des caractères
            if(tmp->mot[0]>='A' && tmp->mot[0]<='z')
            {   printf("\n%c\n",tmp->mot[0]-32);                         //  on l'affiche
                supprimer_caractere(c, tmp->mot[0]);                     // on la supprime de la liste
            }
        }
        char* wordMaj = malloc(MAX_WORD_SIZE * sizeof(char));            // on copie le mot
        if(wordMaj!=0) {
            strcpy(wordMaj, racine->mot);
            if (wordMaj[0] >= 'a' && wordMaj[0] <= 'z') wordMaj[0] = wordMaj[0] -32;      // On remplace son premier caractère par une lettre majuscule si nécessaire
            printf("|--%s", wordMaj);                                                     // On l'affiche
            afficher_liste_positions(&tmp->positions);                                    // On affiche ses positions
            parcours_affichage_index(tmp->filsDroit, c);                                  // On parcourt son arbre droit
        }
        free(wordMaj);                                                                    // On libère la zone mémoire allouée pour le mot
    }
}

void afficher_max_apparition(t_Index *index){
    if(index->racine!=NULL){
        t_Noeud *max = creer_noeud("");                                                   // On crée le nœud où nous allons enregistrer le nœud du mot ayant la plus grande occurrence
        if(max!=0) {                                                                      // si pas d'erreur lors de la création du nœud
            parcours_recherche_max(index->racine, max);                                   // max devient le nœud avec le plus grand nombre d'occurrences
            if (max->nb_occurences==0) printf("\nAucun mot dont le nombre d'occurence est plus grand que 0.\n");
            else printf("\nLe mot le plus present est : %s, avec %d apparation(s).\n", max->mot,max->nb_occurences);
            free(max);
        }
        else printf("\nErreur\n");
    }
    else printf("\nIndex vide\n");
}

void afficher_occurrences_mot(t_Index *index, char *mot){                        // La fonction affiche les phrases dans lesquelles le mot est présent
    t_Noeud *rech = rechercher_mot(index,mot);                                  // on recherche le mot dans l'index et récupère son nœud
    if(rech!=NULL){
        t_ListePhrase Liste_phrases = NULL;
        t_Position *tmp = rech->positions.debut;                                // On récupère la liste chaînée des positions du mot
        while (tmp != NULL) {
            ajouter_phrase(&Liste_phrases, tmp->numero_phrase);   // On ajoute à la liste de phrases, une cellule représentant le numéro de phrase
            tmp = tmp->suivant;
        }
        ajouter_mots_phrase(index->racine, &Liste_phrases);
        tmp = rech->positions.debut;                                                                // On retourne au debut de la liste des positions du mots
        int num_phrase = tmp->numero_phrase;
        printf("\nMot = %s", mot);
        printf("\nOccurences = %d\n", rech->nb_occurences);
        while (Liste_phrases != NULL && tmp != NULL) {                                              // Tant que l'on n'a pas affiché toutes les phrases, on continue
            while (tmp != NULL && num_phrase == tmp->numero_phrase) {                               // Tant que l'on n'a pas affiché toutes les différentes occurrences du mot dans une même phrase, on continue
                printf("| Ligne %d, mot : %d : ", tmp->numero_ligne, tmp->ordre);
                afficher_mot(Liste_phrases->mot_debut);
                printf("\n");
                num_phrase = tmp->numero_phrase;
                tmp = tmp->suivant;
            }
            if (Liste_phrases != NULL && tmp != NULL) {                                              // Si l'on n'a pas affiché toutes les phrases
                t_ListePhrase temp = Liste_phrases;
                Liste_phrases = Liste_phrases->suivant;                                              // On passe à la phrase suivante
                libereMot(temp->mot_debut);
                liberePhrase(temp);
                printf("| Ligne %d, mot : %d : ", tmp->numero_ligne, tmp->ordre);                    // On l'affiche
                afficher_mot(Liste_phrases->mot_debut);
                printf("\n");
                num_phrase = tmp->numero_phrase;
                tmp = tmp->suivant;
            }
        }
        return;
    }
    printf("\nLe mot n'est pas dans l'index.");

}

void afficher_mot(t_Mot *l){                                                                         // La fonction affiche les mots d'une phrase (d'une liste chaînée composée de mots)
    if(l!=NULL) {
        t_Mot *debut = l;
        while (l->suivant != NULL) {
            if (l == debut && l->mot[0] >= 'a' && l->mot[0] <= 'z') l->mot[0] = l->mot[0] -
                                                                                32;                 // si le mot est en tête de liste, sa première lettre est une majuscule
            printf("%s ", l->mot);
            l = l->suivant;
        }
        printf("%s.", l->mot);
    }
}


//------------------------------------------AUTRES FONCTIONS-------------------------------------------

int ajouter_position(t_ListePositions *listeP, int ligne, int ordre, int num_phrase) {      // La fonction ajoute une position dans une liste chaînée de position triée selon le numéro de ligne, l'ordre et le numéro de phrase
    t_Position *newPos = malloc(sizeof(t_Position));
    if (newPos != 0) {                                                                      // on initialise la position à créer
        newPos->numero_ligne = ligne;
        newPos->numero_phrase = num_phrase;
        newPos->ordre = ordre;
        newPos->suivant = NULL;
        if (listeP->debut == NULL) {                                                        // Si la liste est vide, le début de la liste est la position à ajouter
            listeP->debut = newPos;
            listeP->nb_elements++;
            return 1;
        }

        t_Position *tempPos = listeP->debut;
        t_Position *tempPosPrec = NULL;
        while (tempPos->suivant != NULL) {
            if (newPos->numero_ligne < tempPos->numero_ligne ||
                (newPos->numero_ligne == tempPos->numero_ligne && newPos->ordre < tempPos->ordre) ||
                (newPos->numero_phrase < tempPos->numero_phrase && newPos->numero_ligne == tempPos->numero_ligne &&
                 newPos->ordre ==
                 tempPos->ordre))                                           // Si la position est à placer avant tempPos (conditions pour une liste chaînée triée selon l'ordre, la ligne et le numéro de phrase)
            {
                newPos->suivant = tempPos;
                if (tempPosPrec != NULL) tempPosPrec->suivant = newPos;     // Si tempPosPrec n'est pas null, on a avancé dans la liste, donc on l'ajoute entre tempPosPrec et tempPos
                else listeP->debut = newPos;                                // Si tempPosPrec est null, on est resté au début de la liste, donc on l'ajoute dans la liste
                listeP->nb_elements++;
                return 1;
            }
            tempPosPrec = tempPos;
            tempPos = tempPos->suivant;
        }
        tempPos->suivant = newPos;                                          // Si on ne l'a toujours pas ajouté avant la fin, on l'ajoute à la fin
        listeP->nb_elements++;

        return 1;
    }
    return 0;
}

void transforme_to_min(char *str){                                          // La fonction transfome les lettres de l'alphabet d'une chaîne de caractères
    int i = 0;                                                              // en lettres minuscules si elles ne le sont pas
    for (i = 0; str[i] != '\0'; i ++)
    {
        if (str[i]  >= 'A' &&  str[i] <= 'Z')
            str[i] +=  'a' - 'A';
    }
}


t_Noeud* rechercher_mot(t_Index *index, char *mot){                                         // La fonction permet de savoir si un mot se trouve déjà dans l'arbre
    if(index->racine==NULL || mot==NULL) return NULL;
    t_Noeud* tempNode = index->racine;
    while(tempNode!=NULL){                                                                  // si strcasecmp retourne 0, le mot et le mot du nœud tempNode sont les mêmes, indépendamment des maj/min
        if(strcasecmp(tempNode->mot,mot)==0) break;
        else if(strcasecmp(tempNode->mot,mot)>0) tempNode = tempNode->filsGauche;           // si le strcasecmp > 0, le mot de tempNode est "plus grand" lexicographiquement que le mot, on cherche donc dans l'arbre gauche
        else tempNode = tempNode->filsDroit;                                                // sinon on cherche dans l'arbre droit
    }
    return tempNode;
}

int ajouter_noeud(t_Index *index, t_Noeud *noeud){                                           // La fonction ajoute un nœud dans l'index selon l'ordre lexicographique

    if(index!=NULL && noeud!=NULL){
        if (noeud->mot == NULL)
            return 0;                                                                        // Si le mot est vide, on n'ajoute pas le nœud
        t_Noeud *nd = rechercher_mot(index, noeud->mot);
        if (nd !=
            NULL) {                                                                          // Si le nœud existe déjà, on modifie ses attributs
            nd->nb_occurences = nd->nb_occurences +
                                noeud->nb_occurences;                                        // On ajoute le nombre d'occurrences du mot du nœud au nombre d'occurrences du nœud nd déjà existant dans l'index
            while (noeud->positions.debut != NULL) {                                         // On ajoute les positions de nœud à la liste des nœuds du nœud déjà existant
                ajouter_position(&nd->positions, noeud->positions.debut->numero_ligne, noeud->positions.debut->ordre,noeud->positions.debut->numero_phrase);noeud->positions.debut = noeud->positions.debut->suivant;
            }
            index->nb_mots_total = index->nb_mots_total + noeud->nb_occurences;                 // On rajoute le nombre d'occurrences du mot
            return 1;
        }                                                                                       // Sinon, on l'ajoute dans l'index
        t_Noeud *curr = index->racine;
        t_Noeud *prev = NULL;
        while (curr != NULL) {
            prev = curr;
            if (strcasecmp(noeud->mot, curr->mot) < 0) curr = curr->filsGauche;
            else curr = curr->filsDroit;
        }
        if (prev == NULL)
            index->racine = noeud;                                                              // Si prev est null => curr est null donc que l'index est vide. Sa racine est donc le noeud
        else if (strcasecmp(noeud->mot, prev->mot) < 0)
            prev->filsGauche = noeud;                                                           // Sinon, si le mot est plus petit que prev, on l'ajoute à droite de prev
        else prev->filsDroit = noeud;                                                           // Sinon on l'ajoute à gauche
        index->nb_mots_differents++;                                                            // On a ajouté un nouveau nœud, donc un nouveau mot
        index->nb_mots_total = index->nb_mots_total +
                               noeud->nb_occurences;                                            // On rajoute le nombre d'occurrences du mot
        return 1;
    }
    return 0;
}





int indexer_fichier(t_Index *index, char *filename){
    FILE *f;                                                                                                           // Création d'un pointeur vers le fichier
    f = fopen(filename, "r");                                                                            // Ouverture du fichier

    if(f==NULL){                                                                                                       // Si fichier inexistant ou erreur d'ouverture, on arrête
        printf("\nErreur lors de l'ouverture du fichier.");
        return 0;
    }
    char line[MAX_SENTENCE_SIZE];
    int nbMots = 0,nbPhrases =0,nbLignes = 0;
    while(fgets (line, MAX_SENTENCE_SIZE, f)!=NULL)                                                                // On récupère les lignes, tant que la ligne retournée n'est pas nulle
    {
        const char separators[5] = " \n,";
        char *mots;
        int finPhrase=0;
        mots = strtok(line, separators);                                                                     // On sépare la ligne en mots avec le séparateur (' ' ou '\n') et recupère le premier mot
        int ordreLigne = 0;
        while(mots!=NULL){
            finPhrase=0;
            if (strchr(mots, '.')!=NULL)                                                                      // Si '.' est inclus, cela signifie la fin d'une phrase
            {
                supprimer_caractere(mots, '.');
                finPhrase=1;                                                                                          // On indique qu'on est en fin de phrase
            }
            transforme_to_min(mots);// met tout le mot en minuscule
            t_Noeud *newNode = creer_noeud(mots);// On crée le noeud du mot
            ajouter_position(&newNode->positions,nbLignes,ordreLigne,nbPhrases);        // On ajoute la position actuelle
            ajouter_noeud(index,newNode);                                                                       // On l'ajoute au noeud
            mots= strtok(NULL, separators);                                                                  // On passe au mot suivant
            if(finPhrase)nbPhrases++;                                                                                 // Si on est en fin de phrase, le num de phrase du prochain mot est incrémenté
            ordreLigne++;
            nbMots++;
        }
        nbLignes++;
    }
    index->nb_phrases=nbPhrases;                                                                                      // On récupère le nombre de phrases total
    fclose(f);                                                                                                    // On ferme le fichier
    return nbMots;
}


void supprimer_caractere(char *str, char c) {                 // La fonction supprime un caractère c présent dans une chaine str
    int next = 0, index = 0;
    for(; str[index] != 0; next++) {                          // Tant que nous ne sommes pas à la fin de la phrase, on incrémente next

        if(str[next] == c && str[next])                       // Si la lettre du mot est égale au caractère recherché et différente de null
            continue;                                         // On passe à la boucle suivante
        str[index] = str[next];                               // Sinon on remplace la lettre à l'indice index par la lettre à l'indice next (on viendra écraser la lettre = c à la prochaine boucle)
        index++;
        if(!str[next])                                       // Si la lettre à l'indice next est nulle, on s'arrête
            return;
    }
}






void parcours_recherche_max(t_Noeud *racine, t_Noeud *max){              // La fonction affecte à nd le nœud du mot ayant la plus grande occurrence, parcours préfixe
    if(racine!=NULL){
        if(racine->nb_occurences > max->nb_occurences) *max=*racine;      // si le nombre d'occurrences de la racine est plus grand, alors le nd prend la valeur pointée par racine
        parcours_recherche_max(racine->filsDroit, max);            // on regarde les nœuds de l'arbre gauche
        parcours_recherche_max(racine->filsGauche, max);           // on regarde les nœuds de l'arbre droit
    }
}





int ajouter_phrase(t_ListePhrase * ListePhrases, int numero_phrase){             // La fonction ajoute une phrase parmi la liste, une phrase
    t_Phrase *new = creer_phrase();
        new->numero_phrase = numero_phrase;
        if( *ListePhrases==NULL ) {                                                // Si la liste est vide, alors elle pointe vers la nouvelle phrase
            new->suivant = NULL;
            *ListePhrases = new;
        }
        else{
            if(recherche_phrase(*ListePhrases,numero_phrase)!=NULL){       // Si une phrase portant le même numéro existe, on ne l'ajoute pas
                return 1 ;
            }
            t_Phrase *tmp1 = *ListePhrases;
            t_Phrase *tmp2;
            while(tmp1!= NULL){                                                  // Sinon, on l'ajoute à la fin de la liste
                tmp2 = tmp1;
                tmp1 = tmp1->suivant;
            }
            tmp2->suivant = new;
        }
        return 0;

}



t_Phrase* recherche_phrase(t_Phrase* tPhrase,int numero_phrase){        // La fonction cherche parmi la liste chaînée tPhrase, si une cellule phrase ne porte pas le même numéro
    t_Phrase *tmp1 = tPhrase;
    while(tmp1!=NULL && tmp1->numero_phrase != numero_phrase){
        tmp1 = tmp1->suivant;
    }
    return tmp1;

}


int ajouter_mot(t_Mot ** tMot,int numero_ligne,int ordre, char* mot){                                                                           // La fonction ajoute un mot à une phrase. La liste chaînée est triée selon la ligne et l'ordre
    t_Mot *new = creer_mot();
    if(new!=0) {
        strcpy(new->mot, mot);                                                                                                                  // On initialise les attributs du mot
        new->numero_ligne = numero_ligne;
        new->ordre = ordre;

        if (*tMot == NULL || (*tMot)->ordre > ordre && (*tMot)->numero_ligne >= numero_ligne || numero_ligne <(*tMot)->numero_ligne) {          // Si la liste est vide ou que le mot est à placer au début, la liste pointe vers ce mot
            new->suivant = *tMot;
            *tMot = new;
            return 1;
        }
        t_Mot *tmp1 = (*tMot);
        t_Mot *tmp2 = tmp1;

        while (tmp1 != NULL && tmp1->numero_ligne < numero_ligne) {                                                                             // Sinon on atteint la ligne
            tmp2 = tmp1;
            tmp1 = tmp1->suivant;
        }

        while (tmp1 != NULL && tmp1->ordre < ordre && tmp1->numero_ligne == numero_ligne) {                                                     // Puis on atteint l'ordre
            tmp2 = tmp1;
            tmp1 = tmp1->suivant;
        }
        new->suivant = tmp1;                                                                                                                    // On ajoute le mot
        tmp2->suivant = new;

        return 1;
    }
    return 0;

}


void ajouter_mots_phrase(t_Noeud *racine, t_ListePhrase* liste_phrases){                                                    // La fonction parcourt l'index (INFIXE), et ajoute le mot à la bonne phrase si son numéro correspond à une phrase présente dans la liste
    t_Noeud *tmp = racine;
    if(tmp==NULL || liste_phrases == NULL ){
        return;
    }
    else{
        ajouter_mots_phrase(tmp->filsGauche, liste_phrases);                                                                // On lit le sous-arbre gauche
        t_Position *tmp2 = tmp->positions.debut;
        while(tmp2!=NULL){                                                                                                  // Tant que l'on n'a pas lu toutes les positions du mot
            t_Phrase *rech = recherche_phrase(*liste_phrases, tmp2->numero_phrase);                                         // On regarde si le numéro de la phrase du mot est dans la liste des phrases
            if(rech!=NULL){                                                                                                 // Si c'est le cas
                ajouter_mot(&rech->mot_debut,tmp2->numero_ligne,tmp2->ordre,tmp->mot);                                      // On ajoute le mot à la phrase
            }
            tmp2 = tmp2->suivant;                                                                                           // On passe à la position suivante
        }

        ajouter_mots_phrase(tmp->filsDroit, liste_phrases);                                                                 // On lit le sous-arbre droit
    }
}


void construire_texte(t_Index *index, char *filename){
    if(index->racine!=NULL) {
        FILE *output_file = fopen(filename, "w+");                                                                          // On ouvre/crée le fichier en mode lecture
        t_ListePhrase Liste_phrases = NULL;
        for(int i =0;i<index->nb_phrases;i++)ajouter_phrase(&Liste_phrases,i);                                              // Pour chaque phrase de notre index, on crée sa cellule correspondante
        ajouter_mots_phrase(index->racine, &Liste_phrases);                                                                 // On ajoute aux phrases leurs mots associés
        t_Mot *lastWordLastSentence=Liste_phrases->mot_debut;                                                               // Sert à récupérer le mot de la dernière phrase lue
        while(Liste_phrases!=NULL){                                                                                         // Tant que l'on n'a pas lu toutes les phrases
            char sentence[MAX_SENTENCE_SIZE]="";                                                                            // On crée une phrase
            t_Mot *tmp = Liste_phrases->mot_debut;
            if(lastWordLastSentence->numero_ligne<tmp->numero_ligne)strcat(sentence,"\n");                                  // Si le mot de la dernière phrase lue n'a pas le même numéro de ligne, on ajoute un saut de ligne.
            while(tmp!=NULL){
                if(tmp==Liste_phrases->mot_debut && tmp->mot[0]>='a'&& tmp->mot[0]<='z') tmp->mot[0] = tmp->mot[0]-32;      // Si le mot est au début de la phrase, on transforme sa première lettre en majuscule
                strcat(sentence,tmp->mot);                                                                                  // On l'ajoute à la phrase
                if(tmp->suivant==NULL) strcat(sentence,". ");                                                               // On ajoute un point à la phrase s'il s'agit du dernier mot de la liste
                else if(tmp->numero_ligne!=tmp->suivant->numero_ligne) strcat(sentence,"\n");                               // Sinon si le mot suivant n'a pas le même numéro de ligne, on ajoute un saut de ligne
                else strcat(sentence," ");                                                                                  // Sinon on ajoute un espace
                lastWordLastSentence=tmp;                                                                                   // Le dernier mot lu prend sa valeur
                tmp = tmp->suivant;
            }
            printf("%s",sentence);
            fwrite(sentence, 1, strlen(sentence), output_file);         // On écrit la phrase dans le fichier
            libereMot(Liste_phrases->mot_debut);
            t_ListePhrase temp = Liste_phrases;
            Liste_phrases=Liste_phrases->suivant;                                              // On passe à la phrase suivante.
            liberePhrase(temp);
        }

        printf("\n\nLe fichier \"%s\" a ete complete (et cree si inexistant).", filename);
        fclose(output_file);                                                               // On ferme le fichier
    }
    else printf("L'index est vide. Aucune ecriture effectuee.");
}

void libereMot(t_Mot *liste){                           // Libère l'espace mémoire que prend l'object de type t_Mot
    t_Mot *tmp;
    while (liste != NULL)
    {
        tmp = liste;
        liste = liste->suivant;
        free(tmp->mot);                         // On libère l'espace mémoire allouée au mot
        tmp->mot=NULL;
        free(tmp);                              // On libère l'espace mémoire allouée à l'objet de type t_Mot
        tmp = NULL;
    }
}

void liberePhrase(t_Phrase *phrase){                    // Libère l'espace mémoire que prend un objet de type phrase
    free(phrase);
    phrase=NULL;
    }

void libereListe(t_Position *liste)                                      // Libère l'espace mémoire que prend la liste des positions d'un nœud
{

    t_Position *tmp;
    while (liste != NULL)
    {
        tmp = liste;
        liste = liste->suivant;
        free(tmp);
    }

}


void libereNoeud(t_Noeud *racine)                                      // Libère l'espace mémoire que prennent les nœuds (parcours PREFIXE)
{
    if(racine!=NULL) {
        libereListe((racine->positions.debut));
        free(racine->mot);
        racine->mot=NULL;
        libereNoeud(racine->filsGauche);
        libereNoeud(racine->filsDroit);
        free(racine);
        racine=NULL;
    }

}

void libereIndex(t_Index *index){                                       // Libère l'espace mémoire que prend l'arbre
    libereNoeud(index->racine);
    index->racine=NULL;
    free(index);
    index=NULL;
}

int hauteur(t_Noeud *racine){                                           // Calcule la hauteur d'un arbre
    if (racine!=NULL)
    {
        int hauteurGauche = hauteur(racine->filsGauche);
        int hauteurDroit = hauteur(racine->filsDroit);
        if (hauteurDroit>hauteurGauche)  return 1+ hauteurDroit;
        return 1+hauteurGauche;
    }
    else return 0;
}

int equilibre(t_Noeud *racine){           // Retourne 1 si l'arbre est équilibré sinon 0
    if(racine==NULL) return 1;
    if(hauteur(racine->filsGauche)==hauteur(racine->filsDroit) || hauteur(racine->filsDroit)==hauteur(racine->filsGauche)-1 || hauteur(racine->filsDroit)-1==hauteur(racine->filsGauche))
    {
        return -1 + equilibre(racine->filsDroit)+ equilibre(racine->filsGauche);
    }
    return 0;
}
