#include <stdio.h>
#include <stdlib.h>

void junta_arq(FILE *a, FILE *b){
  FILE *c = fopen("c.bin", "wb");
  char palavraA[10];
  char palavraB[10];
  while(fread(palavraA, sizeof(char), 10, a) != 0){
    while(fread(palavraB, sizeof(char), 10, b) != 0){
      fwrite(palavraA, sizeof(char), 10, c);
      fwrite(palavraB, sizeof(char), 10, c);
    }
    rewind(b);
  }
  fclose(c);
}

void main(){
  FILE *a = fopen("a.bin", "rb");
  FILE *b = fopen("b.bin", "rb");

  junta_arq(a, b);
  
  fclose(a);
  fclose(b);
}