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
	Elf32_Ehdr structElf32;
  
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
		structElf32 = lireHeaderElf(fichierElf);

		// Appel des fonctions nécessaires
		if(do_header){
			fonctionEtape1(structElf32);
		}
	
		if(do_sections){
			fonctionEtape2(structElf32,fichierElf);
		}	

		if(do_hex_dump){
			printf("Section : %s\n", sectionHexDump);
			fonctionEtape3(fichierElf,sectionHexDump, structElf32);
		}	

		if(do_symbols){
			fonctionEtape4(fichierElf,structElf32);
		}	

		if(do_relocs){
			fonctionEtape5(structElf32,fichierElf);
		}
	
		// Fermeture du fichier
		fclose(fichierElf);
	}else{
		afficherParametres();
	}

  	return 0;
}
