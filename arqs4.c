#include <stdio.h>
#include <stdlib.h>

typedef struct{
  char nome[40];
  char cpf[11];
  int conta_corrente;
  int agencia;
  float saldo;
}Cliente;


Cliente * lerPrimeiro(FILE *f){
  fseek(f, 0, SEEK_SET);
  Cliente *cli = (Cliente*)malloc(sizeof(Cliente));
  int tam_pulo = 11 + (sizeof(int)*2) + sizeof(float);

  while(fread(cli->nome, 40, 1, f) != 40){ // se não leu o nome pula pro próximo cliente
    fseek(f, tam_pulo, SEEK_CUR);
  }

  if(!feof(f)){
    fread(cli->cpf, sizeof(char), 11, f);
    fread(cli->conta_corrente, sizeof(int), 1, f);
    fread(cli->agencia, sizeof(int), 1, f);
    fread((void*)&cli->saldo, sizeof(float), 1, f); // gambiarra para ler floats do arquivo
    return cli;
  }
  return NULL;
}

int calculaTamanho(FILE *f){
  fseek(f, 0, SEEK_END);
  return ftell(f);
}

Cliente * procuraMenorSaldo(FILE *f, Cliente * cli){
  Cliente * atual = (Cliente*)malloc(sizeof(Cliente));
  Cliente * candidato = cli; // candidato a ser o cliente de menor saldo
  int posCandidato;

  while(!feof(f)){// le todos os registros desdo comeco
    fread(atual->nome, sizeof(char), 40, f);
    fread(atual->cpf, sizeof(char), 11, f);
    fread(atual->conta_corrente, sizeof(int), 1, f);
    fread(atual->agencia, sizeof(int), 1, f);
    fread((void*)&atual->saldo, sizeof(float), 1, f); // gambiarra para ler floats do arquivo

    if(atual->saldo && atual->saldo < candidato->saldo){ // compara o saldo do registro lido
      candidato = atual; // atualiza o menor encontrado até agora
      posCandidato = ftell(f);
    }
  }

  posCandidato -= sizeof(Cliente); // posição do registro com menor saldo
  fseek(f, posCandidato, SEEK_SET); 
  Cliente vazio;
  fwrite(&vazio, sizeof(Cliente), 1, f); // escreve um registro vazio no lugar
  
  return candidato;
}

FILE * ordenaPorSaldo(FILE *f){
  FILE * saida = fopen("indice.bin", "wb");
  int tamClientes = calculaTamanho(f);
  Cliente * candidato = lerPrimeiro(f); // le o primeiro cliente encontrado

  while(calculaTamanho(saida) < tamClientes){
    candidato = procuraMenorSaldo(f, candidato);
    fwrite(candidato, sizeof(Cliente), 1, f);
    candidato = lerPrimeiro(f);
  }
  
  return saida;
}

void main(){
  FILE *clientes = fopen("clientes.bin", "rb");
  FILE *indices = ordenaPorSaldo(clientes);
  fclose(clientes);
  fclose(indices);
}