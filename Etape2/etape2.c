#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

//<=> readelf -S file1.o

char* lire_nom(Elf32_Ehdr structElf32, Elf32_Shdr structSectionHeader, FILE* fich);

int main(int argc, char *argv[]){
	
	FILE* fichierElf = NULL;
	Elf32_Ehdr structElf32;
	Elf32_Shdr structSectionHeader;
	int i;//variable de boucle
    
	if(argc != 2){
    	printf("Syntaxe : ./etape2 file\n");
    	exit(1);
    }
    
    fichierElf = fopen ( argv[1], "rb" );  
    
    if (fichierElf==NULL) 
    {
    	printf ("\nFile error\n"); 
  		exit (1);
  	}

  	if (!fread(&structElf32, sizeof(structElf32), 1, fichierElf))
  	{
    	printf("failed to read elf header\n");
    	exit(1);
  	}
    
  	fseek(fichierElf,structElf32.e_shoff,0); 
   	
   	//structElf32.e_shentsize <=> taille de la table des sections
   	
  	//printf("Numéro||Nom||Type||Addr||Décal||Taille||Flags||Link||Info||Al\n");

  	for(i=0;i<structElf32.e_shnum;i++){
  	
  		if (!fread(&structSectionHeader, sizeof(structSectionHeader), 1, fichierElf))
  		{
    		printf("failed to read elf section header\n");
    		exit(1);
  		}

  		//printf("%i||",i);
  		
  		//printf("%s||",lire_nom(structElf32,structSectionHeader, fichierElf));
  		lire_nom(structElf32,structSectionHeader, fichierElf);
  		printf("\n");
  			
  	}
   	
	return 0;
	
}

char* lire_nom(Elf32_Ehdr structElf32, Elf32_Shdr structSectionHeader, FILE* fich){
	
	char* nom;
	char c;
	int i = 0;
	long pos = ftell(fich);
	
	fseek(fich,structElf32.e_shstrndx + structSectionHeader.sh_name,0); 
	
	while(c!='\0'){

		fread(&c,1,1,fich);
		i++;
		printf("%c\n",c+'0');
		//nom[i]=c;
		
	}
	
	fseek(fich,pos,0); 
	
	return nom;
	
}

