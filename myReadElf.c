#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"
#include "phase1.h"


int main (int argc, char *argv[]){

  FILE* fichierElf;
  Elf32_Ehdr structElf32;
  
  if ( argv[1][0] == '-' && argc==3 ){
	  
	    switch(argv[1][1]){

	    case 'h':
	    
	      if(argc == 3){
	      
	      	 fichierElf = ouvrirFichier(argv[2]);
	      	 structElf32 = lireHeaderElf(fichierElf);
	      	 fonctionEtape1(structElf32);
	      	 
	      }else{
	      	 printf(" Essayez myReadElf -h \"nomfichier\""); 
	      }
	 
	      break;
	    case 'S':
	    	if(argc == 3){
	    	  
	    	   fichierElf = ouvrirFichier(argv[2]);
	      	   structElf32 = lireHeaderElf(fichierElf);
	      	 	  
			   fonctionEtape2(structElf32,fichierElf);
		  }else{
	      	 	printf(" Essayez myReadElf -S \"nomfichier\""); 
	      	  }
		  		   
	      break;
	    case 'x':
	      printf("x");
	      break;
	    case 't':
	      printf("t");
	      break;
	    case 'r':
	      printf("r");
	      break;
	    default:
	      printf("Ne correspont à aucun type");
	      break;
	      
	  }
  }else{
  		printf(" myReadElf nécessite une option -h, -S , -r ... ainsi qu'un nom de fichier"); 
  }

  //fclose(fichierElf);
  return 0;

}
