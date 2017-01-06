#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"
#include "phase1.h"


int main (int argc, char *argv[]){

  FILE* fichierElf;
  Elf32_Ehdr structElf32;
  
  if (argc>=3 && argv[1][0] == '-' && strlen(argv[1]) == 2){
	  
	    switch(argv[1][1]){
		
	    case 'h':
	    
	      if(argc == 3){
	      
	      	 fichierElf = ouvrirFichier(argv[2]);
	      	 structElf32 = lireHeaderElf(fichierElf);
	      	 fonctionEtape1(structElf32);
	     	 fclose(fichierElf);
	      }else{
	   
	      	 printf(" Essayez myReadElf -h \"nomFichier\"\n"); 
	   
	      }
	 
	      break;
	    case 'S':
	    	if(argc == 3){
	    	  
	    	   fichierElf = ouvrirFichier(argv[2]);
	      		structElf32 = lireHeaderElf(fichierElf);
			   fonctionEtape2(structElf32,fichierElf);
			  fclose(fichierElf);
			}else{

	      	 	printf(" Essayez myReadElf -S \"nomFichier\"\n"); 

	      	}
		  		   
	      break;
	    case 'x':
	    	
	    	if(argc == 4){
	    		
	    		fichierElf = ouvrirFichier(argv[3]);
	      	 	structElf32 = lireHeaderElf(fichierElf);
	      	 	fonctionEtape3(fichierElf,argv[2],structElf32);
	      	 	fclose(fichierElf);
	    		
	    	}else{
	    		
	    		printf("Essayez myReadElf -x \"numéroDeSection\" \"nomFichier\"\n");
	    		
	    	}
	      
	      break;
	    case 's':
	      
	      if(argc == 3){
	      	
	      	printf("Etape 4\n");
	      	
	      }else{
	      	
	      	printf("Essayez myReadElf -s \"nomFichier\"\n");
	      	
	      }
	      
	      break;
	    case 'r':
	      
	      if(argc == 3){
	      
	      	fichierElf = ouvrirFichier(argv[2]);
	      	structElf32 = lireHeaderElf(fichierElf);
	      	fonctionEtape5(structElf32,fichierElf);
	      	fclose(fichierElf);
	      	
	      }else{
	      	
	      	printf("Essayez myReadElf -r \"nomFichier\"\n");	
	      	
	      }
	      
	      break;
	    default:
	      printf("Ne correspond à aucun type\n");
	      break;
	      
	  }

  }else{
  		printf("Utilisation de myReadElf : ./readElf -option ...\n");
  		printf("Options : -h <fichier> \t\t\t\t\t Affiche les infos contenu dans le header du fichier elf en paramètre\n");
  		printf("\t  -S <fichier> \t\t\t\t\t Affiche les informations de la table des sections du fichier elf passé en paramètre\n");
  		printf("\t  -x <numéro (ou nom) de section> <fichier> \t Affiche le contenu de la section de numéro (ou nom) passé en pramaètre\n");
  		printf("\t  -s <fichier> \t\t\t\t\t Affiche la table des symboles du fichier elf passé en paramètre\n");
  		printf("\t  -r <fichier> \t\t\t\t\t Affichage des réallocations contenu dans le fichier elf passé en paramètre\n");
  }

  return 0;

}
