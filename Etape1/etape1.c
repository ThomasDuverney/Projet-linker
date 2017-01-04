#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
    
    
int main (int argc, char *argv[]){
    
    FILE* fichierElf = NULL;
    Elf32_Ehdr structElf32;
    
    if(argc >= 3){
    	 printf ("Un seul paramètre nécessaire en entrée \n"); 
    }else if(argc <=1 ){
    	 printf ("Rentrer un nom de fichier en paramètre \n"); 
    }
    else{
    	
    
    	 
    fichierElf = fopen ( argv[1], "rb" );  
    
    if (fichierElf==NULL) 
    {
        printf ("\nFile error"); 
        exit (1);
    }

    if (!fread(&structElf32, sizeof(structElf32), 1, fichierElf))
    {
            printf("failed to read elf header\n");
            exit(1);
    }
    
    if(structElf32.e_ident[EI_MAG0] ==0x7f && structElf32.e_ident[EI_MAG1] =='E' && structElf32.e_ident[EI_MAG2] =='L' && structElf32.e_ident[EI_MAG3] =='F'){
    
	    printf("En-tête ELF : \n");
	    
	// ------------LE NOMBRE MAGIQUE    
	    printf("Magique :\t");
     
	    for(int i=0;i<EI_NIDENT;++i) 
		printf("%x",structElf32.e_ident[i]);
		
	// ------------LA CLASSE
		printf("\nClass :\t");
		if(structElf32.e_ident[EI_CLASS]==1)
			printf("32-bit\n");
		else if(structElf32.e_ident[EI_CLASS] == 2)
	       	  	printf("64-bit\n");
	       	else
			printf("Invalid class\n");
			
	// ------------LES DONNEES
		printf("\nDonnées :\t");
		if(structElf32.e_ident[EI_DATA]==1)
			printf("little Indian\n");
		else if(structElf32.e_ident[EI_DATA] == 2)
	       	  	printf("Big Indian\n");
	       	else
			printf("Invalid data encoding\n");
			
	// ------------LA VERSION
		printf("Version :\t %i ",structElf32.e_ident[EI_VERSION]);
		if(structElf32.e_version == 0)
			  printf("(Invalid version)\n");
	       	else
	       		printf("(Current version)\n");
	       		
	// ------------OS/ABI
		printf("OS/ABI :\t");
		if(structElf32.e_ident[EI_OSABI] == 0)
			  printf("System V\n");
		else if(structElf32.e_ident[EI_OSABI] == 1)
	       	  	  printf("HP-UX\n");
	       	else if(structElf32.e_ident[EI_OSABI] == 2)
	       	  	  printf("NETbsd\n");
	       	else if(structElf32.e_ident[EI_OSABI] == 3)
	       	  	  printf("LINUX\n");
	       	else
	       		  printf("autres ou inconnu\n");	
		
	// ------------LA MACHINE 
	    printf("Machine :\t");
		
	    if(structElf32.e_machine == 1)
		  printf("AT&T WE 32100\n");
	    else if(structElf32.e_machine == 2)
	       	  printf("SPARC\n");
	    else if(structElf32.e_machine == 3)
	    	  printf("Intel 80386\n");
	    else if(structElf32.e_machine == 4)
	    	  printf("Motorola 68000\n");
	    else if(structElf32.e_machine == 5)
	    	  printf("Motorola 88000\n");
	    else if(structElf32.e_machine == 6)
	    	  printf("Intel 80860\n");
	    else if(structElf32.e_machine == 40)
	    	  printf("ARM\n");
	    else
		printf("Inconnu ou non spécifie\n");	
		  
	// -----------LE TYPE 
	    printf("Type :\t");   	  
	    if(structElf32.e_type == 1)
		printf("Relocatable\n");
	    else if(structElf32.e_type == 2)
		printf("Executable\n");
	    else if(structElf32.e_type == 3)
		printf("Shared Object\n");
	    else if(structElf32.e_type == 0)
		printf("No file type\n");
	    else
		printf("Inconnu ou non spécifie\n");
        
	     
	// -----------LA TAILLE DU HEADER
	    printf("Taille :\t %i (Bytes) \n",structElf32.e_ehsize);
	    
	// -----------DEBUT DE LA SECTION
	    printf("Début des en-têtes de section :\t %i (octet dans le fichier) \n",structElf32.e_shoff);
	    
	// -----------LA TABLE DES CHAINES 
	    printf("Table d'indexes des chaînes d'en-tête de section :\t %i\n",structElf32.e_shstrndx);   	  

    }
    fclose(fichierElf);
    }
    return 0;

}


