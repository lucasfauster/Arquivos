#include <stdio.h>
#include <stdlib.h>

typedef struct{
  char nome[40];
  char cpf[11];
  int conta_corrente;
  int agencia;
  float saldo;
}Cliente;

Cliente * consulta(char cpf[11], FILE *f){
  fseek(f, 41, SEEK_SET); //pula o nome do primeiro registro
  char cpf_atual[11];
  int tam_pulo = (sizeof(int)*2) + sizeof(float) + 40; //distância até o cpf do próximo registro

  fread(cpf_atual, sizeof(char), 11, f);
  while(cpf_atual != cpf && !feof(f)){
    fseek(f, tam_pulo, SEEK_CUR);
    fread(cpf_atual, sizeof(char), 11, f);
  }

  if(cpf_atual  == cpf){
    Cliente * cli = (Cliente*)malloc(sizeof(Cliente));
    fseek(f, -51, SEEK_CUR); //volta para o campo nome do registro desejado
    fread(cli->nome, sizeof(char), 40, f);
    fread(cli->cpf, sizeof(char), 11, f);
    fread(cli->conta_corrente, sizeof(int), 1, f);
    fread(cli->agencia, sizeof(int), 1, f);
    fread((void*)&cli->saldo, sizeof(float), 1, f); // gambiarra para ler floats do arquivo
    return cli;
  }
  return NULL; // se o cpf não foi encontrado
}

void main(){
  FILE *clientes = fopen("clientes.bin", "rb");
  Cliente *c = consulta("12345678901", clientes);
}