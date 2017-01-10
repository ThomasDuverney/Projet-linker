#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <getopt.h>
#include <string.h>
#include "fonctionUtile.h"
#include "phase1.h"

void afficherParametres(){
			printf("Utilisation de myReadElf : ./myReadElf -option ...\n");
			printf("Options : -h <fichier> \t\t\t\t\t Affiche les infos contenu dans le header du fichier elf en paramètre\n");
			printf("\t -a <fichier> \t\t\t\t\t  Affiche toutes les informations du fichier elf passé en paramètre\n");
			printf("\t  -S <fichier> \t\t\t\t\t Affiche les informations de la table des sections du fichier elf passé en paramètre\n");
			printf("\t  -x <numéro (ou nom) de section> <fichier> \t Affiche le contenu de la section de numéro (ou nom) passé en pramaètre\n");
			printf("\t  -s <fichier> \t\t\t\t\t Affiche la table des symboles du fichier elf passé en paramètre\n");
			printf("\t  -r <fichier> \t\t\t\t\t Affichage des réallocations contenu dans le fichier elf passé en paramètre\n");
}

int main (int argc, char *argv[]){

  	FILE* fichierElf;
  
	int opt;
	static int do_header, do_sections, 
				do_hex_dump, do_symbols, 
				do_relocs; // flags pour les options

	// Tableau des options
	struct option longopts[] = {
		{ "all", no_argument, 0, 'a' },
		{ "header", no_argument, 0, 'h' },
		{ "sections", no_argument, 0, 'S'},
		{ "hex-dump", required_argument, 0, 'x'},
		{ "symbols", no_argument, 0, 's'},
		{ "relocs",	no_argument, 0, 'r'},
		{ NULL, 0, NULL, 0 }
	};

	// Parcours des options
	if(argc >= 3){
		char * sectionHexDump;
		while ((opt = getopt_long(argc, argv, "ahSsrx:", longopts, NULL)) != -1) {	
			switch(opt) {
				case 'a':
					do_header++;
					do_sections++;
					do_symbols++;
					do_relocs++;
					break;
				case 'h':
					do_header++;
					break;
				case 'S':
					do_sections++;
					break;
				case 'x':
					do_hex_dump++;
					sectionHexDump = optarg;
					break;
				case 's':
					do_symbols++;
					break;
				case 'r':
					do_relocs++;
					break;
				default:
					fprintf(stderr, "Option inconnue %c\n", opt);
					afficherParametres();
					exit(1);
			}
		}

		// Ouverture du fichier
		fichierElf = ouvrirFichier(argv[optind++]);
	
		ContenuFus * contenuFus = malloc(sizeof(ContenuFus));
		contenuFus->contenuElf1 = malloc(sizeof(ContenuElf));	
		
		Elf32_Shdr * TabHeaders = NULL;
		remplirStructure(fichierElf,contenuFus->contenuElf1,&TabHeaders);
		
		// Appel des fonctions nécessaires
		if(do_header){
			fonctionEtape1(contenuFus->contenuElf1->hdrElf);
		}
	
		if(do_sections){
			fonctionEtape2(contenuFus->contenuElf1->hdrElf,fichierElf,contenuFus->contenuElf1->TableNomSection,TabHeaders);
		}	

		if(do_hex_dump){
			fonctionEtape3(fichierElf,sectionHexDump,contenuFus->contenuElf1->hdrElf,contenuFus->contenuElf1->TableNomSection,TabHeaders);
		}	

		if(do_symbols){
			
			fonctionEtape4(fichierElf,contenuFus->contenuElf1->hdrElf,contenuFus->contenuElf1->TableNomSection,TabHeaders,contenuFus->contenuElf1->tabSymb,contenuFus->contenuElf1->symTableSize,contenuFus->contenuElf1->tableString);
		}	

		if(do_relocs){
			fonctionEtape5(contenuFus->contenuElf1->hdrElf,fichierElf,TabHeaders,contenuFus->contenuElf1->tabRela,contenuFus->contenuElf1->tabRelaSize);
		}
	
		// Fermeture du fichier
		fclose(fichierElf);
	}else{
		afficherParametres();
	}

  	return 0;
}
