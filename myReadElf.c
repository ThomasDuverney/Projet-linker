#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"
#include "phase1.h"


int main (int argc, char *argv[]){

  FILE* fichierElf;
  Elf32_Ehdr structElf32;
  
  if ( argv[1][0] == '-' && argc>=3 && strlen(argv[1]) == 2){
	  
	    switch(argv[1][1]){

	    case 'h':
	    
	      if(argc == 3){
	      
	      	 fichierElf = ouvrirFichier(argv[2]);
	      	 structElf32 = lireHeaderElf(fichierElf);
	      	 fonctionEtape1(structElf32);
	      	 
	      }else{
	   
	      	 printf(" Essayez myReadElf -h \"nomFichier\""); 
	   
	      }
	 
	      break;
	    case 'S':
	    	if(argc == 3){
	    	  
	    	   fichierElf = ouvrirFichier(argv[2]);
	      	   structElf32 = lireHeaderElf(fichierElf);
			   fonctionEtape2(structElf32,fichierElf);

			}else{

	      	 	printf(" Essayez myReadElf -S \"nomFichier\""); 

	      	}
		  		   
	      break;
	    case 'x':
	    	
	    	if(argc == 4){
	    		
	    		fichierElf = ouvrirFichier(argv[3]);
	      	 	structElf32 = lireHeaderElf(fichierElf);
	      	 	fonctionEtape3(fichierElf,argv[2],structElf32);
	    		
	    	}else{
	    		
	    		printf("Essayez myReadElf -x \"numéroDeSection\" \"nomFichier\"");
	    		
	    	}
	      
	      break;
	    case 's':
	      
	      if(argc == 3){
	      	
	      	printf("Etape 4");
	      	
	      }else{
	      	
	      	printf("Essayez myReadElf -s \"nomFichier\"");
	      	
	      }
	      
	      break;
	    case 'r':
	      
	      if(argc == 3){
	      
	      	fichierElf = ouvrirFichier(argv[2]);
	      	structElf32 = lireHeaderElf(fichierElf);
	      	fonctionEtape5(structElf32,fichierElf);
	      
	      }else{
	      	
	      	printf("Essayez myReadElf -r \"nomFichier\"");	
	      	
	      }
	      
	      break;
	    default:
	      printf("Ne correspont à aucun type");
	      break;
	      
	  }
	  fclose(fichierElf);
  }else{
  		printf("Utilisation de myReadElf : ./readElf -option ...\n");
  		printf("Options : -h <fichier> \t\t Affiche les infos contenu dans le header du fichier elf en paramètre");
  		printf("\t -S <fichier> \t\t
  }

  return 0;

}
