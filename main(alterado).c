/* prog32.c */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "bolos.h"


void consultarPedidos(){
    FILE *arq;
  struct pedido p;
  arq=fopen(NOMEARQUIVO,"rb");
  if(arq==NULL){
    fprintf(stderr,"Nao abriu %s\n",NOMEARQUIVO);
    exit(1);
  }
  printf("---------------------------------\n");
  
  printf("-- Nome ----- CPF ----- Bolo ---- Data --\n");

  while(fread(&p,sizeof(p),1,arq)==1 && !feof(arq))
    printf("%s %6llu %6s %02d/%02d/%04d\n",
           p.nome_cliente, p.cpf, p.tipo_bolo, p.quando.dia, p.quando.mes, p.quando.ano);
  printf("---------------------------------\n");
  fclose(arq);


  }

  void pedidoNovo(){
  FILE *arq;
  char novo_nome[100];
  struct pedido pd;
  int achou;
  char escolha;
  /*Tenta abrir para leitura/escrita*/
  arq=fopen(NOMEARQUIVO,"r+b");
  if(arq==NULL){ /* Se não abriu, tenta criar novo */
    arq = fopen(NOMEARQUIVO,"w+b");
    if(arq==NULL){
      fprintf(stderr,"Nao consegui criar %s\n", NOMEARQUIVO);
      
      exit(1);
    }
  }
  do{
    printf("\nNome: ");
    scanf(" %[^\n]", novo_nome);
    /* Posiciona no inicio do arquivo */
    rewind(arq);
    achou = 0;
    /* Lê até o final ,procurando pela data */
    while(fread(&pd,sizeof(pd),1,arq)==1 && !feof(arq))
      if(strcmp(novo_nome, pd.nome_cliente)==0){
          printf("Cliente ja cadastrado!\n");
          
          achou = 1;
          break;
    }
    if(achou==0){
      strcpy(pd.nome_cliente, novo_nome);
      printf("CPF (somente numeros): ");
      scanf("%llu", &pd.cpf);
      printf("tipo de bolo: ");
      scanf("%s", pd.tipo_bolo);
      printf("Data : ");
      scanf("%d/%d/%d", &pd.quando.dia, &pd.quando.mes, &pd.quando.ano);
      /* Posiciona o cursor no final do arquivo */
      fseek(arq,0,SEEK_END);
      /* Inclui novo registro */
      fwrite(&pd,sizeof(pd),1,arq);
    }
    printf("\nContinuar? [S/N]: ");
    scanf(" %c",&escolha);
  }while(toupper(escolha)=='S');
  fclose(arq);
  }


int main(void)
{

    int flagMenu=-1;

    printf("██████╗░░█████╗░████████╗░█████╗░██████╗░░█████╗░██╗░░░░░░█████╗░\n");
    printf("██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗██╔══██╗██║░░░░░██╔══██╗\n");
    printf("██║░░██║███████║░░░██║░░░███████║██████╦╝██║░░██║██║░░░░░██║░░██║\n");
    printf("██║░░██║██╔══██║░░░██║░░░██╔══██║██╔══██╗██║░░██║██║░░░░░██║░░██║\n");
    printf("██████╔╝██║░░██║░░░██║░░░██║░░██║██████╦╝╚█████╔╝███████╗╚█████╔╝\n");
    printf("╚═════╝░╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═════╝░░╚════╝░╚══════╝░╚════╝░\n");


    while(flagMenu!=3){
    printf("Digite:\n 1 - NOVO PEDIDO\n 2 - CONSULTAR PEDIDOS\n 3 - SAIR\n ");
    scanf("%d", &flagMenu);
    switch(flagMenu){
      case 1:
        pedidoNovo();
        break;
      case 2:
        consultarPedidos();
        break;
      case 3:
        break;
      default:
        printf("DIGITO INCORRETO!\n");
    }
  }
    

  return(0);
}

