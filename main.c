/* Projet Algorithmie :
Auteur : Kovac Barbara && de Font-Reaulx Pol
groupe Cb
Date : Avril 2016
Lieu: EPF-Sceaux
Fichier : Main.c
Nombre de fichiers: 7
Version 1.0

Ce programme compilé en langage C permet de jouer au jeu Embuscade. Pour gagner il doit parcourir toutes les cases tout
en évitant, si possible, les pirates en posant des drapeaux.
Le score du joueur est affiché au long du jeu et enregistré à la fin s'il le souhaite.
*/



//                                          Bibliothèques
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

//                                           Constantes
#define S 10 //nombre maximum de caracteres dans le pseudo du personnage
#define NombreDeLigne 12 //Nombre de lignes de la frille
#define NombreDeColonnes 10 //Nombre de colonnes de la grille

#define BLEU 9 //code couleur bleu pour l eau
#define GRIS 7 //code couleur gris
#define VERT 2 //code couleur vert pour la terre
#define NOIR 0 //code couleur noir
#define BLANC 15 //code couleur blanc
#define KAKI 6 //code couleur kaki pour les cases parcourues
#define ROUGE 4 //code couleur pour les drapeaux

#define ENTER 13 //code ascii touche enter
#define ECHAP 27 //code ascii touche echap
#define TOUCHEDROITE 77 //code ascii touche directionnelle droit
#define TOUCHEGAUCHE 75 //code ascii touche directionnelle gauche
#define TOUCHEHAUT 72 //code ascii touche directionnelle haut
#define TOUCHEBAS 80 //code ascii touche directionnelle bas
#define TOUCHEDMIN 100 //code ascii touche d
#define TOUCHEDMAJ 68 //code ascii touche D

#define PERSO 127 //code ascii pour le symbole du personnage
#define DRAPEAU 16 //code ascii pour le symbole du drapeau

#define RuseP 10 //ruse de tous les pirates

#define sauvegarde "save.txt" //fichier contenant la sauvegarde
#define carte1 "carte1.txt" //fichier contenant la carte du niveau facile
#define carte2 "carte2.txt" //fichier contenant la carte du niveau moyen
#define carte3 "carte3.txt" //fichier contenant la carte du niveau difficile
#define Score1 "score1.txt" //fihcier contenant les meilleurs scores du niveau facile
#define Score2 "score2.txt" //fichier contenant les meilleurs scores du niveau moyen
#define Score3 "score3.txt" //fichier contenant les meilleurs scores du niveau difficile


//                                          Structures de données
struct Personnage //stucture contenant les informations du personnage
{
    char pseudo[S]; //pseudo du personnage
    int endurance, ruse; //endurance et habilete du personnage
    int x, y; //abscisse et ordonné du personnage dans la grille
    int score; //score du joueur
};
typedef struct Personnage Sperso;

struct Pirates //structure contenant les informations des pirates
{
    int endurance; //endurance du pirate combattu
};
typedef struct Pirates Spirate;

struct Cases //structure contenant les informations sur les cases a afficher
{
    int nature; //0: mer - 1: parcourue - 2: non parcourue
    int occupe; //0: non occupée - 1: occupée
    int piege; //0: non plaçé - 1: plaçé
    int drapeau; //0: pas de drapeau - 1: placé
};
typedef struct Cases Scase;


//Déclarations des sous-programmes

void AfficherMenu(Scase [NombreDeLigne][NombreDeColonnes], Sperso*, Spirate, int, int); //fonction du menu
void creerPerso(Sperso*); //fonction pour creer le personnage
int Caracteristique(int*); //fonction retournant le nombre choisi pour une caracteristique
void BoucleJeu(Sperso* ,int ,Scase [NombreDeLigne][NombreDeColonnes], Spirate, int, int, int); //fonction contenant la boucle du jeu
void afficherGrille(Scase [NombreDeLigne][NombreDeColonnes],Sperso); //fonction qui affiche la grille
void afficherCase(Scase [NombreDeLigne][NombreDeColonnes],int ,int ); //fonction qui affiche une case selon les informations données sur celle ci
void couleurCase(int , int ); //fonction colorant la case
void touche(int* , int* , Scase [NombreDeLigne][NombreDeColonnes],int* ,Sperso*, Spirate, int*); //fonction qui decide quoi faire slon la touche chosie
void Noyade(Sperso*); //fonction qui dit au joueur que son personnage s est noye
void SorteDeCase(int , int , Scase [NombreDeLigne][NombreDeColonnes], Sperso* , Spirate, int*); //fonction qui decide quoi faire selon la case où est le joueur
void deplacement(int , int , Scase [NombreDeLigne][NombreDeColonnes]); //fonction qui deplace le personnage
void drapeau(Scase [NombreDeLigne][NombreDeColonnes], int i, int j); //fonction qui demande au joueur où placer le drapeau
void ChangerDrapeau(int, int, Scase [NombreDeLigne][NombreDeColonnes]); //fonction qui place le drapeau a la place voulue
int ChercheurPirate(Scase [NombreDeLigne][NombreDeColonnes], int, int); //fonction qui compte le nombre de pirate autour du personnage
int CompteCasesParcourues(Scase [NombreDeLigne][NombreDeColonnes]); //fonction qui compte le nombre de cases parcourues sur la grille
void Sauvegarde(Scase [NombreDeLigne][NombreDeColonnes],Sperso ,int ,int ,int); //fonction servant a sauvegarder la partie dans un fichier
void Chargement(Scase [NombreDeLigne][NombreDeColonnes], Sperso* ,int* ,int* ,int*); //fonction servant a charger une partie a partir d un fichier
void ChoisirNiveau(Scase [NombreDeLigne][NombreDeColonnes],Sperso* ,Spirate* ,int* ,int* ,int*); //fonction permettant au joueur de choisir le niveau de difficulte
void ChargerCarte(Scase [NombreDeLigne][NombreDeColonnes],Sperso* ,Spirate* , FILE*,int* ,int*); //fonction permettant de charger la grille voulue a partir d un fichier
void EnregistrerScore(Sperso, char*); //fonction permettant d enregistrer le score dans un fichier en fonction du niveau de difficulte
void LireScore(char*); //fonction permettant de lire les 5 meilleurs scores selon chaque niveau de difficulte



//Programme principal
int main()
{
    Sperso monPerso; //On initialise le personnage et les valeurs qui lui correspondent
    Spirate Pirate; //On initialise les pirates et les valeurs qui leurs correspondent
    Pirate.endurance=0;
    monPerso.x=0;
    monPerso.y=0;
    monPerso.endurance=0;
    monPerso.ruse=0;
    monPerso.score=0;
    int CasesP=0,NombrePirate=0; //On initialise le nombre de cases qu on peut parcourir sur la carte et le nombre de pirates sur celle ci
    Scase Grille[NombreDeLigne][NombreDeColonnes]; //On initialise la grille

    AfficherMenu (Grille, &monPerso, Pirate, CasesP, NombrePirate); //on affiche le menu

    return 0;
}


//Sous-programmes

void AfficherMenu (Scase Grille[NombreDeLigne][NombreDeColonnes], Sperso* monPerso, Spirate Pirate, int CasesP, int NombrePirate)
{
    int choix, Jeu=1; //la boucle du jeu continue tant que Jeu=1
    int Niveau; //pour le choix du niveau
    printf("   MENU   \n"); //on affiche le menu
    printf("\t1 : Nouvelle partie \n");
    printf("\t2 : Continuer partie \n");
    printf("\t3 : Score \n");
    printf("\t4 : Regle du jeu \n");
    printf("\t5 : Quitter jeu \n");
    printf("Votre choix est : ");
    scanf("%d",&choix);

    switch (choix) //selon le chiffre choisi on effectue differente actions
    {
        case 1 : //On commence une nouvelle partie
            Jeu=1; // le jeu est en marche
            ChoisirNiveau(Grille,&(*monPerso),&Pirate,&CasesP,&NombrePirate,&Niveau);
            creerPerso(&(*monPerso));
            BoucleJeu(&(*monPerso), Jeu, Grille, Pirate, CasesP, NombrePirate,Niveau); //On lance le jeu
            break;
        case 2 : //On charge la partie a partir du fichier sauvegarde
            Chargement(Grille,&(*monPerso),&CasesP,&NombrePirate,&Niveau);
            BoucleJeu(&(*monPerso), Jeu, Grille, Pirate, CasesP, NombrePirate,Niveau);
            break;
        case 3: //On affiche les scores selon le niveau de difficulte
            while((Niveau>3)||(Niveau<1))
            {
                system("cls"); //on efface la console
                printf("Veuillez choisir le niveau de difficulte dont vous voulez voir les scores:\n1: Facile\n2: Moyen\n3: Difficile\n");
                scanf("%d",&Niveau);
            }
            system("cls"); //on efface la console
            if(Niveau==1)
                    {
                        LireScore(Score1); //On affiche le score du niveau facile
                    }
                    else if(Niveau==2)
                    {
                        LireScore(Score2); //On affiche le score du niveau moyen
                    }
                    else if(Niveau==3)
                    {
                        LireScore(Score3); //On affiche le score du niveau difficile
                    }
            printf("\n");
            system("pause"); //On met le jeu en pause tant que le joueur n appuie pas sur une touche pour qu il puisse lire les scores
            printf("\n\n");
            system("cls"); //On efface la console
            AfficherMenu(Grille, &(*monPerso), Pirate, CasesP, NombrePirate); //On affiche de nouveau le menu
            break;
        case 4 : //On affiche les regles
            system("cls"); //On efface la console
            printf("\n\tLe plateau du jeu est compose de 12 cases sur 10 : \n \
                    -les cases vertes representent l'ile \n \
                    -les cases bleues representent l'eau \n \
               Vous vous noierez si vous allez sur l eau. \n \
               Vous devez eviter les embuscades posees par les pirates. \n \
               Et lorsque vous pensez savoir ou se situe une embuscade, \n \
               positionnez un drapeau avec la touche d. \n \
               Vous povez vous deplacer grace aux fleches, vers le haut,\n \
               le bas, a droite et a gauche \n \
                Vous pouvez quitter le jeu et sauvegarder a tout moment grace a la touche Echap.\n\n");
            system("pause");
            system("cls");
            AfficherMenu(Grille, &(*monPerso), Pirate, CasesP, NombrePirate);
       break;
    case 5 :
        Jeu=2; // le jeu s arrete sans demande de sauvegarde
        BoucleJeu(&(*monPerso), Jeu, Grille, Pirate, CasesP, NombrePirate,Niveau);
       break;
    }
}

void creerPerso(Sperso* monPerso)
{
    int NombrePoint=20; //On donne le nombre de points que le joeur peut repartir
    printf("\nEntrez votre pseudo : ");
    scanf("%s",(*monPerso).pseudo); //On enregistre le pseudo du joueur
    printf("\nVous disposez de %d points a disposer entre vos caracteristiques. \n", NombrePoint);
    printf("Nombre de vies attribue a votre personnage: ");
    (*monPerso).endurance=Caracteristique(&NombrePoint); //La fonction retourne l endurance du personnage que le joueur a choisi tout en enlevant les points attribués à NombrePoint
    printf("Nombre de ruse attribue a votre personnage: ");
    (*monPerso).ruse=Caracteristique(&NombrePoint); //La fonction retourne la ruse du personnage que le joueur a choisi
    printf("\n\tVotre personnage s appelle: %s. Il a une endurance de %d et une ruse de %d . \n\tBonne chance!\n\n",(*monPerso).pseudo,(*monPerso).endurance,(*monPerso).ruse);
    system("pause");
    system("cls");
}

int Caracteristique(int* NombrePoint)
{
    int cara;
    scanf("%d",&cara);
    while((cara>(*NombrePoint)||(cara<1))) //Le joueur ne peut initialiser sa caracteristique qu entre 1 et NombrePoint
    {
        printf("\n\tNombre invalide! Veuillez entrer un chiffre entre 1 et %d : ", *NombrePoint); //Message d erreur
        scanf("%d",&cara);
    }
    (*NombrePoint)=(*NombrePoint)-cara; //On enleve les points deja attribues au nombre total de points
    return cara; //On retourne l entier choisi par le joueur
}

void BoucleJeu(Sperso* monPerso ,int Jeu ,Scase Grille[NombreDeLigne][NombreDeColonnes], Spirate Pirate, int CasesP, int NombrePirate, int Niveau)
{
    int choix1;
    int sauvegarder=0; //On initialise le choix pour sauvegarder
    int CasesParcourues=9,choix2, PirateInitial, CasesTotal=0; //On intialise le nombre de cases deja parcourues, le nombre de pirates sur la carte
    PirateInitial=NombrePirate; //On sauvegarde le nombre de pirates que la carte contient initialement dans la variable PirateInitial
    while((Jeu==1)&&((*monPerso).endurance>0)&&(CasesParcourues!=CasesTotal)) //Le jeu continue tant que Jeu=1, que l endurance ne descend pas en dessous de 1 et que le nombre de cases parcourues par le joueur n est pas egal au nombre de cases qu il faut parcourir pour gagner
    {
        CasesTotal=CasesP; //Le nombre de cases a parcourir pour gagner est egal au nombre de cases parcourables sur la grille
        CasesTotal=CasesTotal-NombrePirate; //On enleve de ce nombre le nombre de pirates encore sur la carte
        afficherGrille(Grille,*monPerso); //On affiche la grille
        touche(&(*monPerso).x,&(*monPerso).y,Grille,&Jeu,&(*monPerso),Pirate,&NombrePirate); //Le joueur appuie sur une touche
        system("cls");
        CasesParcourues=CompteCasesParcourues(Grille); //La fonction retourne le nombre de cases deja parcourues par le personnage
    }

    if(Jeu==0) //Lorsque que le jeu s arrete car on a appuye sur echap
    {
        while((sauvegarder!=2)&&(sauvegarder!=1)) //On choisit de sauvegarder ou pas
        {
            printf("\nVoulez vous sauvegarder?\n Tapez 1 pour oui ou 2 pour non: ");
            scanf("%d",&sauvegarder);
        }
        if(sauvegarder==1) //Si le joueur dit oui on sauvegarde toutes les informations necessaires
        {
            Sauvegarde(Grille,*monPerso,CasesP,NombrePirate,Niveau);
            printf("\n\tVous avez bien sauvegarder!\n");
        }
        printf("\n\n\n\t\tMerci d avoir jouer a ce jeu!\n\n\n"); //message de confirmation
        system("pause");
        exit(0); //On quitte le programme
    }
    else if(Jeu==2) //Si le joueur choisit de quitter le jeu a partir du menu
    {
        printf("\n\n\n\t\tMerci d avoir jouer a ce jeu!\n\n\n");
        system("pause");
        exit(0); //On quitte le programme
    }
    else if((*monPerso).endurance<=0) //Si le joueur meurt
    {
        printf("\n\n\n\t\tVous etes mort!!\n\n\n");
        exit(0); //On quitte le programme
    }
    else if(CasesParcourues==CasesTotal) //Si le joueur a gagne en parcourant toutes les cases
    {
        printf("\n\n\n\t\t\tBravo!!! Vous avez gagne!!!\n");
        printf("\n\n Voulez vous enregistrer votre score? \n ");
        printf("\n");
        while((choix1!=1)&&(choix1!=2))
            {
                printf("Tapez 1 pour oui ou 2 pour non: ");
                scanf("%d",&choix1);
                if(choix1==1)
                {
                    if(Niveau==1) //On enregistre son score selon le niveau qu il a choisi au depart
                    {
                        EnregistrerScore(*monPerso,Score1);
                    }
                    else if(Niveau==2)
                    {
                        EnregistrerScore(*monPerso,Score2);
                    }
                    else if(Niveau==3)
                    {
                        EnregistrerScore(*monPerso,Score3);
                    }
                }
            }
    }
    system("cls");
    printf("Voulez vous rejouer? \n Tapez 1 pour oui ou 2 pour non: ");
    scanf("%d",&choix2);
    switch(choix2) //Selon le choix du joueur on quitte le jeu ou on reaffiche le menu
    {
        case 1:
            system("cls");
            AfficherMenu(Grille, &(*monPerso), Pirate, CasesP, PirateInitial);
            break;
        case 2:
            system("cls");
            printf("\n\n\n\t\tMerci d avoir jouer a ce jeu!\n\n\n");
            system("pause");
            exit(0); //On quitte le jeu
        default:
            printf("\tSaisie non valide\n"); //Message d erreur
            system("pause");
            break;
    }
}

void afficherGrille(Scase Grille[NombreDeLigne][NombreDeColonnes],Sperso monPerso)
{
    int i,j;
    int compteurPirate; //On initialise un compteur de pirates
    compteurPirate=ChercheurPirate(Grille,monPerso.x,monPerso.y); //la fonction renvoie le nombre de pieges posés autour du joueur, que l on enregistre dans le compteur de pîrates
    couleurCase(NOIR,GRIS); //On met la premiere barre en gris
    for(i=0;i<NombreDeLigne;i++)
    {
        for(j=0;j<NombreDeColonnes;j++)
        {
            printf("|"); // on affiche une barre pour créer le cote des cases
            afficherCase(Grille,i,j); // la fonction permet d'afficher les cases et leur couleur en fonction des informations sur la case
        }
        printf("|");// on affiche le cote droit des cases sur la 12e ligne
        printf("\n"); //On va a la ligne pour afficher 10 cases sur une ligne
    }
    printf("%s\n",monPerso.pseudo); //on affiche le pseudo du personnage sur la grille
    printf("Vie: %d\t Ruse: %d\n", monPerso.endurance, monPerso.ruse);//on affiche le nombre de points d'endurance et de ruse restant
    printf("Nombre de pirates autour: %d\n",compteurPirate);//on affiche le nombre de pieges autour du personnage
    printf("Score: %d\n",monPerso.score);//on affiche le score du personnage sur la grille
    printf("\n\n Appuyer sur une touche...\n");
}


void afficherCase(Scase Case[NombreDeLigne][NombreDeColonnes],int i,int j)
{
    if(Case[i][j].occupe==0)
    {
        if(Case[i][j].drapeau==1) // la case contient un drapeau
        {
            couleurCase(NOIR,ROUGE); // le fond de la case contenant le drapeau est rouge
            printf("_%c_",DRAPEAU); // on affiche un drapeau sur la case choisie par le joueur
            couleurCase(NOIR,GRIS); // on laisse les barres de separation entre chaque case en noir sur foncd gris
        }
        else if(Case[i][j].drapeau==0) // la case ne contient pas de drapeau
        {
            switch(Case[i][j].nature)
            {
                case 0:
                    couleurCase(NOIR,BLEU);  // le texte de la case est noir et le fond est bleu
                    printf("___");// on affiche le bas de la case
                    couleurCase(NOIR,GRIS);// on laisse les barres de separation entre chaque case en noir sur foncd gris
                    break;
                case 1:
                    couleurCase(NOIR,KAKI); //le texte de la case est noir et le fond est kaki
                    printf("___");
                    couleurCase(NOIR,GRIS);// on laisse les barres de separation entre chaque case en noir sur foncd gris
                    break;
                case 2: // pour les cases non parcourues
                    couleurCase(NOIR,VERT); //le texte de la case est noir et le fond est vert
                    printf("___");// on affiche le bas de la case
                    couleurCase(NOIR,GRIS);// on laisse les barres de separation entre chaque case en noir sur foncd gris
                    break;
            }
        }
    }

    else if(Case[i][j].occupe==1) // la case est occupée
    {
            switch(Case[i][j].nature)
        {
            case 0: // pour les cases mer
                couleurCase(NOIR,BLEU);//le texte de la case est noir et le fond est bleu
                printf("___");// on affiche le bas de la case
                couleurCase(NOIR,GRIS);// on laisse les barres de separation entre chaque case en noir sur foncd gris
                break;
            case 1:// pour les cases parcourues
                couleurCase(NOIR,KAKI);//le texte de la case est noir et le fond est kaki
                printf("_%c_",PERSO);// on affiche le bas de la case avec le personnage
                couleurCase(NOIR,GRIS);// on laisse les barres de separation entre chaque case en noir sur foncd gris
                break;
            case 2:// pour les cases non parcourues
                couleurCase(NOIR,VERT);//le texte de la case est noir et le fond est vert
                printf("_%c_",PERSO); //on affiche le bas de la case avec le personnage
                couleurCase(NOIR,GRIS);// on laisse les barres de separation entre chaque case en noir sur foncd gris
                break;
        }
    }
    else
    {
        printf("Erreur case ni occupee, ni vide"); //
        system("pause");
        exit(EXIT_FAILURE);
    }
}

void couleurCase(int CouleurDuTexte, int CouleurDeFond)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,CouleurDeFond*16+CouleurDuTexte); //On donne au texte affiche et a son fond une certaine couleur
}

void touche(int* i, int* j, Scase Grille[NombreDeLigne][NombreDeColonnes],int* Jeu, Sperso* monPerso, Spirate pirate, int* NombrePirate )
{
    int touche, choix;
    touche=getch(); //Le joueur appuie sur une touche et on ne lui demande pas de confirmation
    switch(touche)
    {
        case TOUCHEDMIN: //touche d pour demander où afficher le drapeau
            drapeau(Grille,*i,*j);
            break;
        case TOUCHEDMAJ: //touche D pour demander où afficher le drapeau
            drapeau(Grille,*i,*j);
            break;
        case ECHAP: //touche echap pour quitter le jeu en demandant de sauvegarder
            system("cls");
            printf("Etes vous sur de vouloir quitter? \nSi oui appuyez sur 1, sinon appuyez sur 2: "); //Confirmation
            scanf("%d",&choix);
            if(choix==1)
            {
                *Jeu=0; //On quitte la boucle
            }
            break;

        case TOUCHEDROITE: //Touche directionnelle droite pour aller a droite
            Grille[*i][*j].occupe=0; //Le joueur n occupe plus la case où il etait avant
            if((Grille[*i][(*j)+1].nature!=0)&&(Grille[*i][(*j)+1].drapeau!=1)) //S il n y a pas de drapeau sur la case et que la case n est pas de l eau
            {
                (*j)++; //On incremente la position verticale du personnage
                deplacement(*i,*j,Grille); //On deplace le personnage
                (*monPerso).score++; //Le personnage s etant deplace on incremente le score
                SorteDeCase(*i,*j,Grille,&(*monPerso),pirate,&(*NombrePirate)); //On regarde ce qu il y a sur la case où se trouve le personnage
            }
            else if(Grille[*i][(*j)+1].nature==0) //Si la case où se deplace le personnage est la mer
            {
                Noyade(&(*monPerso)); //Le personnage se noit
            }
            else if(Grille[*i][(*j)+1].drapeau==1) //Si le personnage veut se deplacer sur une case qui contient un drapeau
            {
                printf("\n\tDeplacement impossible sur une case marquee d un drapeau \n\n"); //Message d erreur
                Grille[*i][*j].occupe=1; //On reoccupe la case où se trouve toujours le personnage
                system("pause");
            }
            break;

        case TOUCHEGAUCHE: //Touche directionnelle gauche pour aller a gauche, meme chose que precedemment
            Grille[*i][*j].occupe=0;
            if((Grille[*i][(*j)-1].nature!=0)&&(Grille[*i][(*j)-1].drapeau!=1))
            {
                (*j)--;
                deplacement(*i,*j,Grille);
                (*monPerso).score++;
                SorteDeCase(*i,*j,Grille,&(*monPerso),pirate,&(*NombrePirate));
            }
            else if(Grille[*i][(*j)-1].nature==0)
            {
                Noyade(&(*monPerso));
            }
            else if(Grille[*i][(*j)-1].drapeau==1)
            {
                printf("\n\tDeplacement impossible sur une case marquee d un drapeau \n\n");
                Grille[*i][*j].occupe=1;
                system("pause");
            }
            break;

        case TOUCHEHAUT: //Touche directionnelle haut pour aller en haut, meme chose que precedemment
            Grille[*i][*j].occupe=0;
            if((Grille[(*i)-1][*j].nature!=0)&&(Grille[(*i)-1][*j].drapeau!=1))
            {
                (*i)--;
                deplacement(*i,*j,Grille);
                (*monPerso).score++;
                SorteDeCase(*i,*j,Grille,&(*monPerso),pirate,&(*NombrePirate));
            }
            else if(Grille[(*i)-1][*j].nature==0)
            {
                Noyade(&(*monPerso));
            }
            else if(Grille[(*i)-1][*j].drapeau==1)
            {
                printf("\n\tDeplacement impossible sur une case marquee d un drapeau \n\n");
                Grille[*i][*j].occupe=1;
                system("pause");
            }
            break;

        case TOUCHEBAS: //Touche directionnelle bas pour aller en bas, meme chose que precedemment
            Grille[*i][*j].occupe=0;
            if((Grille[(*i)+1][*j].nature!=0)&&(Grille[(*i)+1][*j].drapeau!=1))
            {
                (*i)++;
                deplacement(*i,*j,Grille);
                (*monPerso).score++;
                SorteDeCase(*i,*j,Grille,&(*monPerso),pirate,&(*NombrePirate));
            }
            else if(Grille[(*i)+1][*j].nature==0)
            {
                Noyade(&(*monPerso));
            }
            else if(Grille[(*i)+1][*j].drapeau==1)
            {
                printf("\n\tDeplacement impossible sur une case marquee d un drapeau \n\n");
                Grille[*i][*j].occupe=1;
                system("pause");
            }
            break;
    }
}

void Noyade(Sperso* monPerso)
{
    printf("\n\n Pauvre fou! Vous avez saute dans l eau mais ne sachant pas nage, vous vous etes noye...\n\n");
    (*monPerso).endurance=0; //Le personnage meurt, on sort de la boucle
    system("pause");
}

void SorteDeCase(int i, int j, Scase Grille[NombreDeLigne][NombreDeColonnes], Sperso* monPerso, Spirate Pirate, int* NombrePirate)
{
    if(Grille[i][j].piege==1) //S il y a un pirate sur la case
    {
        printf("\nVous etes tombe dans une embuscade des pirates!!!\n\n");
        system("pause");
        printf("\n");
        while(((*monPerso).endurance>0)&&(Pirate.endurance>0)) //On continue le combat tant que le personnage ou le pirate n est pas mort
        {
        int RusePerso, RusePiege;
        srand (time(NULL)); //On initialise la fonction srand sur le temps actuel
        RusePerso = (rand() % (*monPerso).ruse+1); //On genere une valeur aleatoire pour la ruse du personnage entre 0 et la ruse que le joueur lui a affecte au debut de la partie
        RusePiege = (rand() % ( RuseP+1)); //On genere une valeur aleatoire pour la ruse du pirate entre 0 et la ruse donnee en constante
        if(RusePerso>RusePiege) //La ruse du personnage est plus elevee que celle du pirate
            {
                printf("Bravo vous avez reussi a toucher le pirate!\n\n");
                Pirate.endurance=Pirate.endurance-2; //On enleve de la vie au pirate
                printf("\t Vie du personnage: %d \n\t Vie du pirate: %d \n\n",(*monPerso).endurance,Pirate.endurance); //On affiche la vie des deux
                system("pause");
            }

            else if(RusePiege==0 ||RusePerso==0) //Les deux valeurs sont egales a 0 --> erreur
            {
                printf("Erreur combat\n\n"); //Message d erreur
                system("pause");
            }

            else if(RusePiege>RusePerso) //La ruse du personnage est moins elevee que celle du pirate
            {
                printf("Le pirate vous a attrape et vous avez pris un coup...\n\n");
                (*monPerso).endurance=(*monPerso).endurance-2; //On enleve de la vie au personnage
                printf("\t Vie du personnage: %d \n\t Vie du pirate: %d \n\n",(*monPerso).endurance,Pirate.endurance);
                system("pause");
            }

            else //Les deux valeurs sont egales
            {
                printf("Vous avez touche le pirate mais celui ci vous a retourne le coup!\n\n");
                (*monPerso).endurance=(*monPerso).endurance-1;
                Pirate.endurance=Pirate.endurance-1;
                printf("\t Vie du personnage: %d \n\t Vie du pirate: %d \n\n",(*monPerso).endurance,Pirate.endurance);

                system("pause");
            }
        }

        if(Pirate.endurance<=0) //On a battu le pirate
        {
            printf("Bravo, vous avez defait cet infame pirate! \n\n");
            (*NombrePirate)=(*NombrePirate)-1;
            system("pause");
            Grille[i][j].piege=0;
        }

    }
}

void deplacement(int i, int j, Scase Grille[NombreDeLigne][NombreDeColonnes])
{
    Grille[i][j].nature=1; //On met la case sur laquelle on est en case parcourue
    Grille[i][j].occupe=1; //On pose le personnage sur la case
}

void drapeau(Scase Grille[NombreDeLigne][NombreDeColonnes], int i, int j)
{
    int touche;
    printf("\n Veuillez placer ou retirer le drapeau avec les touches directionnelles \n\n");
    touche=getch(); //La varaible touche obtient la valeur dela touche sur laquelle le joueur appuie
    if(touche==224) //Le code ascii des touches directionnelles commencant toutes par 224 puis ayant une seconde valeur on met cette valeur pour pouvoir les differencier
    {
        touche=getch();
        switch(touche)
        {
        case TOUCHEDROITE: //On pose un drapeau sur la case a droite du personnage
            ChangerDrapeau(i,j+1,Grille);
            break;
        case TOUCHEGAUCHE: //On pose un drapeau sur la case a gauche du personnage
            ChangerDrapeau(i,j-1,Grille);
            break;
        case TOUCHEHAUT: //On pose un drapeau sur la case en haut du personnage
            ChangerDrapeau(i-1,j,Grille);
            break;
        case TOUCHEBAS: //On pose un drapeau sur la case en bas du personnage
            ChangerDrapeau(i+1,j,Grille);
            break;
        default: //Erreur
            printf("\tSaisie non valide\n");
            system("pause");
            break;
        }
    }
    else //Erreur
    {
        printf("\tSaisie non valide\n");
        system("pause");
    }
}

void ChangerDrapeau(int i, int j, Scase Grille[NombreDeLigne][NombreDeColonnes])
{
    if(Grille[i][j].drapeau==1) //S il y a deja un drapeau sur la case on l enleve
        {
            Grille[i][j].drapeau=0;
        }
    else if(Grille[i][j].drapeau==0) //S il n y a pas de drapeau sur la case on le rajoute
        {
            Grille[i][j].drapeau=1;
        }
}

int ChercheurPirate(Scase Grille[NombreDeLigne][NombreDeColonnes], int i, int j)
{
    int compteur=0; //On initialise un compteur qui s incrementera pour chaque pirate autour du personnage
    if(Grille[i+1][j].piege==1)
    {
        compteur++;
    }
    if(Grille[i-1][j].piege==1)
    {
        compteur++;
    }
    if(Grille[i][j+1].piege==1)
    {
        compteur++;
    }
    if(Grille[i][j-1].piege==1)
    {
        compteur++;
    }
    if(Grille[i-1][j-1].piege==1)
    {
        compteur++;
    }
    if(Grille[i-1][j+1].piege==1)
    {
        compteur++;
    }
    if(Grille[i+1][j+1].piege==1)
    {
        compteur++;
    }
    if(Grille[i+1][j-1].piege==1)
    {
        compteur++;
    }
    return compteur; //On retourne la valeur du compteur qu on obtient a la fin
}

int CompteCasesParcourues(Scase Grille[NombreDeLigne][NombreDeColonnes])
{
    int compteur=0; //On iniitialise le compteur qui s incremente pour chaque case deja parcourue par le personnage
    int i,j;
    for(i=0;i<NombreDeLigne;i++)
    {
        for(j=0;j<NombreDeColonnes;j++)
        {
            if(Grille[i][j].nature==1)
            {
                compteur++;
            }
        }
    }
    return compteur; //On retourne la valeur du compteur qu on obtient a la fin
}

void Sauvegarde(Scase Grille[NombreDeLigne][NombreDeColonnes],Sperso monPerso,int CasesP,int NombrePirate,int Niveau)
{
    int i,j;
    FILE* save; //On intialise la variable qui servira a ouvrir le fichier
    if((save=fopen(sauvegarde,"w+")) == NULL) //on ouvre le fichier qui va contenir la sauvegarde
    {
        fprintf(stderr, " \n Impossible d ouvrir le fichier %s \n", sauvegarde); //message d'erreur
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(save," %s\n %d %d\n %d %d\n %d %d %d\n %d\n",monPerso.pseudo,monPerso.endurance,monPerso.ruse,monPerso.x,monPerso.y,CasesP,NombrePirate,monPerso.score,Niveau); //On inscrit dans le fichier les differentes valeurs du personnage
        for(i=0;i<NombreDeLigne;i++)
        {
            for(j=0;j<NombreDeColonnes;j++)
            {
                fprintf(save,"%d %d %d %d   ",Grille[i][j].nature,Grille[i][j].occupe,Grille[i][j].piege,Grille[i][j].drapeau); //On sauvegarde la grille
            }
            fprintf(save,"\n");
        }
        fclose(save); //On ferme le fichier ouvert precedemment
        system("cls");
    }

}

void Chargement(Scase Grille[NombreDeLigne][NombreDeColonnes],Sperso* monPerso,int* CasesP,int* NombrePirate,int* Niveau)
{
    int i,j;
    FILE* charge;
    if((charge=fopen(sauvegarde,"r")) == NULL) //on ouvre le fichier qui va contenir la sauvegarde
    {
        fprintf(stderr, " \n Impossible d ouvrir le fichier %s \n", sauvegarde); //message d'erreur
        exit(EXIT_FAILURE);
    }
    else
    {
        fscanf(charge,"%s %d %d %d %d %d %d %d %d",(*monPerso).pseudo,&(*monPerso).endurance,&(*monPerso).ruse,&(*monPerso).x,&(*monPerso).y,&(*CasesP),&(*NombrePirate),&(*monPerso).score,&(*Niveau)); //On lit les informations sur le personnage et on les affecte aux variables
        for(i=0;i<NombreDeLigne;i++)
        {
            for(j=0;j<NombreDeColonnes;j++) //On lit la grille et on l affecte
            {
                fscanf(charge,"%d",&Grille[i][j].nature);
                fscanf(charge,"%d",&Grille[i][j].occupe);
                fscanf(charge,"%d",&Grille[i][j].piege);
                fscanf(charge,"%d",&Grille[i][j].drapeau);
            }
        }
        fclose(charge); //On ferme le fichier
        system("cls");
        printf("\n\n\t\tVous avez charge la partie de %s !!\n",(*monPerso).pseudo); //Message de confirmation
        system("pause");
        system("cls");
    }
}

void ChoisirNiveau(Scase Grille[NombreDeLigne][NombreDeColonnes],Sperso* monPerso,Spirate* Pirate,int* CasesP,int* NombrePirate,int* Niveau)
{
    int choix=0;
    FILE* fichier;
    while((choix>3)||(choix<1)) //Le joueur choisit le niveau de difficulte
    {
        system("cls");
        printf("Veuillez choisir votre niveau de difficulte souhaite:\n1: Facile\n2: Moyen\n3: Difficile\n");
        scanf("%d",&choix);
    }
    if(choix==1) //Niveau facile
    {
        if((fichier=fopen(carte1,"r")) == NULL) //on ouvre le fichier qui contient le niveau
        {
            fprintf(stderr, " \n Impossible d ouvrir le fichier %s \n", carte1); //message d'erreur
            exit(EXIT_FAILURE);
        }
        else
        {
            ChargerCarte(Grille,&(*monPerso),&(*Pirate),fichier,&(*CasesP),&(*NombrePirate)); //On lit la carte qui correspond au niveau
        }
    }
    else if(choix==2) //Niveau moyen
    {
        if((fichier=fopen(carte2,"r")) == NULL) //on ouvre le fichier qui contient le niveau
        {
            fprintf(stderr, " \n Impossible d ouvrir le fichier %s \n", carte2); //message d'erreur
            exit(EXIT_FAILURE);
        }
        else
        {
            ChargerCarte(Grille,&(*monPerso),&(*Pirate),fichier,&(*CasesP),&(*NombrePirate)); //On lit la carte qui correspond au niveau
        }
    }
    else if(choix==3) //Niveau difficile
    {
        if((fichier=fopen(carte3,"r")) == NULL) //on ouvre le fichier qui contient le niveau
        {
            fprintf(stderr, " \n Impossible d ouvrir le fichier %s \n", carte3); //message d'erreur
            exit(EXIT_FAILURE);
        }
        else
        {
            ChargerCarte(Grille,&(*monPerso),&(*Pirate),fichier,&(*CasesP),&(*NombrePirate)); //On lit la carte qui correspond au niveau
        }
    }
    *Niveau=choix; //Le choix du niveau est sauvegarder dans la variable Niveau pour pouvoir ensuite enregistrer le score dans le bon fichier
}

void ChargerCarte(Scase Grille[NombreDeLigne][NombreDeColonnes],Sperso* monPerso,Spirate* Pirate,FILE* carte,int* CasesP,int* NombrePirate)
{
    int i,j;
    fscanf(carte,"%d %d %d %d %d",&(*monPerso).x,&(*monPerso).y,&(*Pirate).endurance,&(*CasesP),&(*NombrePirate)); //On charge les informations sur la postion du joueur et la vie du pirate(qui augmente selon la difficulte)
    for(i=0;i<NombreDeLigne;i++)
        {
            for(j=0;j<NombreDeColonnes;j++)
            {
                fscanf(carte,"%d",&Grille[i][j].nature); //On charge les valeurs de la carte
                fscanf(carte,"%d",&Grille[i][j].occupe);
                fscanf(carte,"%d",&Grille[i][j].piege);
                fscanf(carte,"%d",&Grille[i][j].drapeau);
            }
        }
    fclose(carte); //On ferme le fichier
}

void EnregistrerScore(Sperso monPerso, char* Score)
{
    FILE* fichier;
    int i;
    int flag=1; //valeur boolenne initialise ici sur le VRAI
    Sperso a; //variable pour l echange
    Sperso perso[6]; //Tableau contenant au maximum 6 structures Sperso
    if((fichier=fopen(Score,"r")) == NULL) //on ouvre le fichier qui contient les meilleurs scores
        {
            fprintf(stderr, " \n Impossible d ouvrir le fichier %s \n", Score); //message d'erreur
            exit(EXIT_FAILURE);
        }
        else
        {
            for(i=0;i<5;i++)
            {
                fscanf(fichier,"%s %d",perso[i].pseudo,&perso[i].score); //On lit les informations contenus dans le fichier
            }
        }
        fclose(fichier); //On ferme le fichier
        perso[5]=monPerso; //On affecte la variable du personnage actuel au dernier element du tableau

        while(flag==1) //Algorithme de tri a bulles, la boucle continue tant que flag a la valeur VRAI
            {
                flag=0; //flag obtient la valeur FAUX
                for(i=0;i<6;i++)
                {
                    if(perso[i].score>perso[i+1].score)
                    {
                        a=perso[i]; //On echange les valeurs des structures
                        perso[i]=perso[i+1];
                        perso[i+1]=a;
                        flag=1; //flag obtient la valeur VRAI
                    }
                }
            }

            if((fichier=fopen(Score,"w")) == NULL) //on ouvre le fichier qui va contenir les meilleurs scores
        {
            fprintf(stderr, " \n Impossible d ouvrir le fichier %s \n", Score); //message d'erreur
            exit(EXIT_FAILURE);
        }
        else
        {
            for(i=0;i<5;i++)
            {
                fprintf(fichier,"%s %d\n",perso[i].pseudo,perso[i].score); //On inscrit dans le fichier les 5 meilleurs scores, le scores le plus mauvais est donc efface
            }
        }
        fclose(fichier); //On ferme le fichier
        printf("\nEnregistrement du fichier complete!\n\n"); //Message de confirmation
        system("pause");
}

void LireScore(char* Score)
{
    int i;
    FILE* fichier;
    Sperso perso[5]; //On initialise un tableau qui contiendra au maximum 5 structures Sperso
    system("cls");
    printf("\t\tLes meilleurs scores:\n\n");
    if((fichier=fopen(Score,"r")) == NULL) //on ouvre le fichier contient les scores
        {
            fprintf(stderr, " \n Impossible d ouvrir le fichier %s \n", Score); //message d'erreur
            exit(EXIT_FAILURE);
        }
        else
        {
            for(i=0;i<5;i++)
            {
                fscanf(fichier,"%s %d",perso[i].pseudo,&perso[i].score); //Ont lit les valeurs des scores et les pseudos qui vont avec
                printf("\t%s : %d\n",perso[i].pseudo,perso[i].score); //On les affiche
            }
        }
        fclose(fichier); //On ferme le fichier
}
