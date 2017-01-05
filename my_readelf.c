#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>



int main (int argc, char *argv[]){

  FILE* fichierElf = NULL;

  switch((*argv)[1]){

    case 'h':
      printf("le header");
      break;
    case 1:
      printf("R_ARM_PC24");
      break;
    case 2:
      printf("R_ARM_ABS32");
      break;
    case 3:
      printf("R_ARM_REL32");
      break;
    case 28:
      printf("R_ARM_CALL");
      break;
    case 29:
      printf("R_ARM_JUMP24");
      break;
    default:
      printf("Ne correspont à aucun type");
      break;
  }
  printf("\n");

  return 0;

}
