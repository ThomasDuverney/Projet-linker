#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <getopt.h>
#include <string.h>
#include "fonctionUtile.h"
#include "phase.h"

void afficherParametres(){
			printf("Utilisation de myReadElf : ./myReadElf -option ...\n");
			printf("Options : -h <fichier> \t\t\t\t\t Affiche les infos contenu dans le header du fichier elf en paramètre\n");
			printf("\t -a <fichier> \t\t\t\t\t  Affiche toutes les informations du fichier elf passé en paramètre\n");
			printf("\t  -S <fichier> \t\t\t\t\t Affiche les informations de la table des sections du fichier elf passé en paramètre\n");
			printf("\t  -x <numéro (ou nom) de section> <fichier> \t Affiche le contenu de la section de numéro (ou nom) passé en pramaètre\n");
			printf("\t  -s <fichier> \t\t\t\t\t Affiche la table des symboles du fichier elf passé en paramètre\n");
			printf("\t  -r <fichier> \t\t\t\t\t Affichage des réallocations contenu dans le fichier elf passé en paramètre\n");
			printf("\t  -f <fichier> \t\t\t\t\t Fusion des sections du fichier\n");
}

int main (int argc, char *argv[]){

	FILE* fichierElf;
  
	int opt;
	static int do_header, do_sections, 
				do_hex_dump, do_symbols, 
				do_relocs, do_fusion_sections; // flags pour les options

	// Tableau des options
	struct option longopts[] = {
		{ "all", no_argument, 0, 'a' },
		{ "header", no_argument, 0, 'h' },
		{ "sections", no_argument, 0, 'S'},
		{ "hex-dump", required_argument, 0, 'x'},
		{ "symbols", no_argument, 0, 's'},
		{ "relocs",	no_argument, 0, 'r'},
		{ "fusion-symbols", no_argument, 0, 'f'},
		{ NULL, 0, NULL, 0 }
	};

	// Parcours des incipales fonctions et chiers correspondants options
	if(argc >= 3){
		char * sectionHexDump;
		while ((opt = getopt_long(argc, argv, "ahSsrfx:", longopts, NULL)) != -1) {	
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
				case 'f':
					do_fusion_sections++;
					break;
				default:
					fprintf(stderr, "Option inconnue %c\n", opt);
					afficherParametres();
					exit(1);
			}
		}

		// Phase 1
		// Parcours des fichiers
		int i = optind;
		if(do_header || do_sections || do_hex_dump || do_symbols || do_relocs){
			while(argv[i] != NULL){
				fichierElf = ouvrirFichier(argv[i]);
				if(!fichierElf){
					printf("Impossible d'ouvrir le fichier %s\n", argv[i]);
					exit(1);
				}
			
				printf("\x1b[34mAffichage du fichier %s\n\x1b[0m\n", argv[i]);
			
				ContenuElf * contenuElf = malloc(sizeof(ContenuElf));	
				Elf32_Shdr * TabHeaders = NULL;
				remplirStructure(fichierElf,contenuElf,&TabHeaders);
		
				// Appel des fonctions nécessaires
				if(do_header){
					fonctionEtape1(contenuElf->hdrElf);
				}
	
				if(do_sections){
					fonctionEtape2(contenuElf->hdrElf,fichierElf,contenuElf->TableNomSection,TabHeaders);
				}	

				if(do_hex_dump){
					fonctionEtape3(fichierElf,sectionHexDump,contenuElf->hdrElf,contenuElf->TableNomSection,TabHeaders);
				}	

				if(do_symbols){
					fonctionEtape4(contenuElf->tabSymb,contenuElf->symTableSize,contenuElf->tableString);
				}	

				if(do_relocs){
					fonctionEtape5(contenuElf->hdrElf,fichierElf,TabHeaders,contenuElf->tabRela,contenuElf->tabRelaSize);
				}

				fclose(fichierElf);
				i++;
			}
		}

		// Phase 2
		if(do_fusion_sections){
			FILE * fichDest = ouvrirFichier(argv[optind]);
			FILE * secondFich = ouvrirFichier(argv[optind+1]);
			ContenuFus* contenuFus = remplirStructureFusion(fichDest, secondFich);
			fonctionEtape6(contenuFus);
		}
	}else{
		afficherParametres();
	}

	/*libererSectionInfos(tabSectionProgb1);
	libererSectionInfos(tabSectionProgb2);
	libererTabHeaders(TabHeaders);
	libererTabHeaders(TabHeaders2);
	libererMemoire(contenuFus);*/

	return 0;
}
