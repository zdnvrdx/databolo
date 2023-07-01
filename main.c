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
  printf("-- Data ----- CPF ---- Bolo --\n");

  while(fread(&p,sizeof(p),1,arq)==1 && !feof(arq))
    printf("%02d/%02d/%04d %6llu %6s\n",
           p.quando.dia, p.quando.mes, p.quando.ano,
           p.cpf, p.tipo_bolo);
  printf("---------------------------------\n");
  fclose(arq);


  }

  void pedidoNovo(){
  FILE *arq;
  struct data nova;
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
    printf("\nData : ");
    scanf("%d/%d/%d", &nova.dia, &nova.mes, &nova.ano);
    /* Posiciona no inicio do arquivo */
    rewind(arq);
    achou = 0;
    /* Lê até o final ,procurando pela data */
    while(fread(&pd,sizeof(pd),1,arq)==1 && !feof(arq))
      if(nova.dia==pd.quando.dia &&
         nova.mes==pd.quando.mes &&
         nova.ano==pd.quando.ano){
          printf("Data ja cadastrada!\n");
          achou = 1;
          break;
    }
    if(achou==0){
      pd.quando.dia = nova.dia;
      pd.quando.mes = nova.mes;
      pd.quando.ano = nova.ano;
      printf("CPF (somente numeros): ");
      scanf("%llu", &pd.cpf);
      printf("tipo de bolo: ");
      scanf("%s", pd.tipo_bolo);
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

