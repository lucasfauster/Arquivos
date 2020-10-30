#include <stdio.h>
#include <stdlib.h>

FILE * junta_arq(FILE *a, FILE *b){
  FILE *c = fopen("c.bin", "wb");
  char palavraA[10];
  char palavraB[10];
  while(fread(palavraA, sizeof(char), 10, a) != 0){
    while(fread(palavraB, sizeof(char), 10, b) != 0){
      fwrite(palavraA, sizeof(char), 10, c);
      fwrite(palavraB, sizeof(char), 10, c);
    }
  }
  return c;
}

void main(){
  FILE *a = fopen("a.bin", "wb+");
  FILE *b = fopen("b.bin", "wb+");
  
  FILE *c = junta_arq(a, b);
}