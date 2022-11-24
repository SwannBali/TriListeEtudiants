#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LONG_NUMETUD 5 // Taille des Num_�tudiants


typedef struct Eleve Eleve; // Structure pour chaque �tudiant avec son Nom, la taille de sa ligne (pour pouvoir se d�placer plus tard), et un Indice pour les fichiers_OUT
struct Eleve {

	char* nom;
	int longLigne;
	int sommeLigne;
};


int Promotion(FILE* fichier);
long Recup_Donnes(FILE* fichier, int nb_etudiant, Eleve * Liste_Etud);

void fusion(Eleve tableau[],int deb1,int fin1,int fin2);
void tri_fusion_bis(Eleve tableau[],int deb,int fin);
void tri_fusion(Eleve tableau[],int longueur);

void restitution(Eleve* tab, FILE* fichier, int nbetudiant);




int main() {

	int nb_etudiant;
	long taille_Nom;
	char lettre = '_';

	Eleve* Liste_Etud;
	FILE* fichier = NULL;


	fichier = fopen("ListeIUT2021niveau2.txt", "r"); // Chemin absolu du fichier TEXTE d'origine � entrer pour �viter des probl�mes (Donc dans ton cas, faut le modifier)

	nb_etudiant = Promotion(fichier); // R�cup�re le nombre d'�tudiants du fichier via une diff�rene entre le dernier et le premier Num_�tudiant

	Liste_Etud = (Eleve*) malloc(ftell(fichier) - (LONG_NUMETUD + 3) * nb_etudiant + sizeof(int) * 2 * nb_etudiant); // Alloue un tableau un peu plus grand pour stocker tous les noms sans probl�mes

	rewind(fichier);

	taille_Nom = Recup_Donnes(fichier, nb_etudiant, Liste_Etud); // R�cup�re la taille exacte de toutes les chaines de caract�res qui contiennent les noms

	realloc(Liste_Etud, taille_Nom + sizeof(int) * 2 * nb_etudiant); // R�duit la taille du tableau, pour qu'il prenne EXACTEMENT la place n�cessaire en m�moire pour contenir les valeurs

	tri_fusion(Liste_Etud,nb_etudiant);

	restitution(Liste_Etud, fichier, nb_etudiant);


	for (int k = 0; k < nb_etudiant; k++) // PAS DANS LE PROGRAMME FINAL, juste pour afficher les donn�es r�cup, pour les tests
	{
		printf("%s  %d %d\n", Liste_Etud[k].nom, Liste_Etud[k].longLigne, Liste_Etud[k].sommeLigne);
	}

	// Ici se trouveront la/les fonctions pour le Tri et le placement final des donn�es


	free(Liste_Etud); // Lib�re l'espace m�moire du tableau une fois que les donn�es ont �t� utilis�s

}

long Recup_Donnes(FILE* fichier, int nb_etudiant, Eleve * Liste_Etud)
{
	char lettre = ' ';
	int taille_nom;
	int ligneChar;
	int sommeLigne = -1;
	long All_Nom_Char = 0;


	fseek(fichier, 1, SEEK_CUR); // Se positionne apr�s la premi�re lettre du fichier, car les Pr�noms commencent par une majuscule

	for (int i = 0; i < nb_etudiant; i++)
	{
		ligneChar = 0; // Sert � compter le nombre de Char sur chaque ligne, pour les futurs placements

		while (lettre < 65 || lettre > 90) // Tant que le caract�re r�cup�r� n'est pas une MAJUSCULE, alors nous savons que la donn�e n'est pas un NOM
		{
			lettre = fgetc(fichier);
			ligneChar++;
		}
		taille_nom = 0; // Sert � r�cup�rer la taille des noms, +1 pour le '\0' de fin de chaine de caract�re

		while (lettre >= 65 && lettre <= 90) // Tant que l'on arrive pas � la fin du Nom, on avance dans le fichier
		{
			taille_nom++;
			while (lettre >= 65 && lettre <= 90)
			{
				lettre = fgetc(fichier);
				taille_nom++;
				ligneChar++;
			}
			lettre = fgetc(fichier);
			ligneChar++;
		}
		ligneChar--;


		Liste_Etud[i].nom = (char*)malloc(taille_nom * sizeof(char)); // On alloue � une case du tableau, la place EXACTE pour le Nom et le '\0'

		fseek(fichier, -(taille_nom + 1), SEEK_CUR); // On se d�place juste avant le premier caract�re du Nom
		fgets(Liste_Etud[i].nom, taille_nom, fichier); // On r�cup�re le nom et le dernier caract�re (qui est vide) est remplac� par le '\0'


		while (lettre != '\n' && lettre != EOF) // On continue d'avancer jusqu'� la prochaine ligne, ou la fin du fichier pour la derni�re ligne
		{
			lettre = fgetc(fichier);
			ligneChar++;
		}

		sommeLigne += ligneChar + 1;
		Liste_Etud[i].sommeLigne = sommeLigne;
		Liste_Etud[i].longLigne = ligneChar; // On stocke la taille de chaque ligne dans le tableau, pour le r�cup�ration des Pr�noms et Num_�tudiants � la toute fin du programme

		All_Nom_Char += taille_nom; // On r�cup�re progressivement la taille totale des chaines de Noms, pour la reallocation du tableau, pour r�duire l'espace m�moire utilis�


		fseek(fichier, 1, SEEK_CUR); // On se replace apr�s la Majuscule du pr�nom, pour qu'elle ne soit pas prise en compte lors de la premi�re boucle
	}

	return All_Nom_Char;
}

int Promotion(FILE* fichier)
{
	int nb_etudiant;
	int numetud_debut;
	char chaine[LONG_NUMETUD + 1];
	char lettre = '_';


	while (lettre != '\n' && lettre != EOF)
	{
		lettre = fgetc(fichier);
	}
	fseek(fichier, -(LONG_NUMETUD + 2), SEEK_CUR); // Se place au niveau du Num_�tudiant de la premi�re ligne, pour le r�cup�rer

	numetud_debut = atoi(fgets(chaine, LONG_NUMETUD + 1, fichier));
	rewind(fichier);

	fseek(fichier, -LONG_NUMETUD, SEEK_END); // Se place au niveau du Num_�tudiant de la derni�re ligne pour le calcul final

	nb_etudiant = atoi(fgets(chaine, LONG_NUMETUD + 1, fichier)) - numetud_debut; // Calcule le nombre d'�tudiant en fonction des num�ros de d�but et de fin

	return nb_etudiant + 1;
}

void fusion(Eleve tableau[],int deb1,int fin1,int fin2)
        {
        Eleve *table1;
        int deb2=fin1+1;
        int compt1=deb1;
        int compt2=deb2;
        int i;

        table1= (Eleve*) malloc((fin1-deb1+1)*1000);

        for(i=deb1;i<=fin1;i++)
            {
            table1[i-deb1]=tableau[i];
            }

        for(i=deb1;i<=fin2;i++)
            {
            if (compt1==deb2) //c'est que tous les éléments du premier tableau ont été utilisés
                {
                break; //tous les éléments ont donc été classés
                }
            else if (compt2==(fin2+1)) //c'est que tous les éléments du second tableau ont été utilisés
                {

                tableau[i]=table1[compt1-deb1]; //on ajoute les éléments restants du premier tableau
                compt1++;
                }
            else if (strcmp(table1[compt1-deb1].nom,tableau[compt2].nom)<0)
                {
                tableau[i]=table1[compt1-deb1]; //on ajoute un élément du premier tableau
                compt1++;
                }
            else
                {
                tableau[i]=tableau[compt2]; //on ajoute un élément du second tableau
                compt2++;
                }
            }
        free(table1);
        }


void tri_fusion_bis(Eleve tableau[],int deb,int fin)
        {
        if (deb!=fin)
            {
            int milieu=(fin+deb)/2;
            tri_fusion_bis(tableau,deb,milieu);
            tri_fusion_bis(tableau,milieu+1,fin);
            fusion(tableau,deb,milieu,fin);
            }
        }

void tri_fusion(Eleve tableau[],int longueur)
     {
     if (longueur>0)
            {
            tri_fusion_bis(tableau,0,longueur-1);
            }
     }

void restitution(Eleve* tab, FILE* fichier, int nb_etudiant)
{
	char* ligne = NULL;

	FILE* fichier_out = NULL;
	fichier_out = fopen("fichier_out.txt", "w");

	fseek(fichier, tab[0].sommeLigne - tab[0].longLigne, SEEK_SET);
	ligne = (char*)malloc(tab[0].longLigne);

	fgets(ligne, tab[0].longLigne, fichier);
	fputs(ligne, fichier_out);
	fprintf(fichier_out, "\n");

	fseek(fichier, 1, SEEK_CUR);

	for (int i = 1; i < nb_etudiant; i++)
	{
		ligne = (char*) realloc(ligne, tab[i].longLigne);

		fseek(fichier, tab[i].sommeLigne - tab[i - 1].sommeLigne - tab[i].longLigne, SEEK_CUR);

		fgets(ligne, tab[i].longLigne, fichier);
		fputs(ligne, fichier_out);
		fprintf(fichier_out, "\n");

		fseek(fichier, 1, SEEK_CUR);
	}

	free(ligne);

	fclose(fichier);
	fclose(fichier_out);
}
