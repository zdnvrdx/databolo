/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "bolos.h"


void consultarPedidos();

void pedidoNovo();
  
void localizarPedidos();
  
int main(void)
{

    int flagMenu=-1;

    printf("██████╗░░█████╗░████████╗░█████╗░██████╗░░█████╗░██╗░░░░░░█████╗░\n");
    printf("██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗██╔══██╗██║░░░░░██╔══██╗\n");
    printf("██║░░██║███████║░░░██║░░░███████║██████╦╝██║░░██║██║░░░░░██║░░██║\n");
    printf("██║░░██║██╔══██║░░░██║░░░██╔══██║██╔══██╗██║░░██║██║░░░░░██║░░██║\n");
    printf("██████╔╝██║░░██║░░░██║░░░██║░░██║██████╦╝╚█████╔╝███████╗╚█████╔╝\n");
    printf("╚═════╝░╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═════╝░░╚════╝░╚══════╝░╚════╝░\n");


    while(flagMenu!=4){
    printf("\nDigite:\n 1 - NOVO PEDIDO\n 2 - CONSULTAR PEDIDOS\n 3 - LOCALIZAR PEDIDOS\n 4 - SAIR\n ");
    scanf("%d", &flagMenu);
    switch(flagMenu){
      case 1:
        pedidoNovo();
        flagMenu = 0;
        break;
      case 2:
        consultarPedidos();
         flagMenu = 0;
        break;
      case 3:
        localizarPedidos();
         flagMenu = 0;
        break;
      case 4:
        break;
      default:
        printf("DIGITO INCORRETO!\n");
        while (getchar() != '\n');
        continue;
    }
  }
    

  return(0);
}



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
  int data_valida = 0;
  int achou, cont_cpf;
  unsigned long long int temp_cpf;
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
      
      do
      {
        cont_cpf = 0;
        if(scanf("%llu", &pd.cpf)!=1){
          printf("CPF precisa conter apenas numeros. Tente novamente: ");
          while (getchar() != '\n');
        }
        else{
        temp_cpf = pd.cpf;
          while (temp_cpf!=0){
            temp_cpf /= 10;
            cont_cpf++;
          }
          if(cont_cpf != 11){
            printf("O CPF precisa de 11 digitos\n");
            printf("Informe o CPF novamente: ");
          }
        }
      } while (cont_cpf !=11);
      
      printf("tipo de bolo: ");
      scanf("%s", pd.tipo_bolo);
      printf("Data (DD/MM/AAAA): ");
      do
      {
        if(scanf("%d/%d/%d", &pd.quando.dia, &pd.quando.mes, &pd.quando.ano)!=3){
          printf("Data invalida. Tente novamente (DD/MM/AAAA): ");
          while (getchar() != '\n');
        }
        else if (pd.quando.dia < 1 || pd.quando.dia > 31 || pd.quando.mes < 1 || pd.quando.mes > 12) {
          if (pd.quando.dia < 1 || pd.quando.dia > 31) {
            printf("Dia invalido. Insira um valor entre 1 e 31.\n");
          }
          else if (pd.quando.mes < 1 || pd.quando.mes > 12) {
            printf("Mes invalido. Insira um valor entre 1 e 12.\n");
          }
          printf("Tente novamente (DD/MM/AAAA): ");
          while (getchar() != '\n');
        }
        else if ((pd.quando.dia > 30) &&
           (pd.quando.mes == 4 || pd.quando.mes == 6 || pd.quando.mes == 9 || pd.quando.mes == 11)) {
            printf("O mes %d contem apenas 30 dias. Insira um valor entre 1 e 30 para o dia.\n", pd.quando.mes);
            printf("Tente novamente (DD/MM/AAAA): ");
            while (getchar() != '\n');
          }
        else if (pd.quando.dia == 29 && pd.quando.mes == 2) {
          if ((pd.quando.ano % 4 == 0 && pd.quando.ano % 100 != 0) || pd.quando.ano % 400 == 0) {
            data_valida = 1;
         } 
          else {
            printf("O ano %d nao e bissexto. Informe novamente (DD/MM/AAAA): ", pd.quando.ano);
            
          }
        }
        else if(pd.quando.dia > 28 && pd.quando.mes == 2){
           printf("Fevereiro contem apenas 28 dias. Informe novamente (DD/MM/AAAA): ");
           
        }
        else{
          data_valida = 1;
        }
      } while (data_valida!=1);
      
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



  void localizarPedidos(){
    FILE *arq;
    char localiza[100];
    struct pedido pd;
    struct data localiza_data;
    int tipo_localiza, encontrados;
    char escolha;
    char cpf_str[12];
    arq=fopen(NOMEARQUIVO,"rb");
    if(arq==NULL){
      fprintf(stderr,"Nao abriu %s\n",NOMEARQUIVO);
      exit(1);
    }
    do{
      tipo_localiza = 0;
      encontrados = 0;
      printf("\nLocalizar por:\n");
      printf(" 1 - NOME  2 - CPF  3 - BOLO  4 - DATA\n");
      scanf("%d", &tipo_localiza);
      /* Posiciona no inicio do arquivo */
      rewind(arq);
      switch (tipo_localiza)
        {
        case 1:
          printf("\nNome: ");
          scanf(" %[^\n]", localiza);
          while(fread(&pd,sizeof(pd),1,arq)==1 && !feof(arq))
            if (strstr(pd.nome_cliente, localiza)!=NULL)
            {
              printf("%s %6llu %6s %02d/%02d/%04d\n",
              pd.nome_cliente, pd.cpf, pd.tipo_bolo, pd.quando.dia, pd.quando.mes, pd.quando.ano);
              encontrados++;
            }
          if (encontrados==0)
          {
            printf("\nNao foram encontrados resultados para a busca\n");
          }
          
          break;
        
        case 2:
          printf("\nCPF: ");
          scanf(" %[^\n]", localiza);
          while(fread(&pd,sizeof(pd),1,arq)==1 && !feof(arq)){
            snprintf(cpf_str, sizeof(cpf_str), "%011llu", pd.cpf);
            if (strstr(cpf_str, localiza)!=NULL)
            {
              printf("%s %6llu %6s %02d/%02d/%04d\n",
              pd.nome_cliente, pd.cpf, pd.tipo_bolo, pd.quando.dia, pd.quando.mes, pd.quando.ano);
              encontrados++;
            }
          }
          if (encontrados==0)
          {
            printf("\nNao foram encontrados resultados para a busca\n");
          }
          
          break;
        
        case 3:
          printf("\nTipo do bolo: ");
          scanf(" %[^\n]", localiza);
          while(fread(&pd,sizeof(pd),1,arq)==1 && !feof(arq)){
            if (strstr(pd.tipo_bolo, localiza)!=NULL)
            {
              printf("%s %6llu %6s %02d/%02d/%04d\n",
              pd.nome_cliente, pd.cpf, pd.tipo_bolo, pd.quando.dia, pd.quando.mes, pd.quando.ano);
              encontrados++;
            }
          }
          if (encontrados==0)
          {
            printf("\nNao foram encontrados resultados para a busca\n");
          }
          
          break;
        
        case 4:
          localiza_data.mes = -1, localiza_data.ano = -1;
          printf("Data: ");
          scanf("%d/%d/%d", &localiza_data.dia, &localiza_data.mes, &localiza_data.ano);
          while(fread(&pd,sizeof(pd),1,arq)==1 && !feof(arq)){
            if (( pd.quando.dia == localiza_data.dia) && (localiza_data.mes == -1 || pd.quando.mes == localiza_data.mes) && 
            (localiza_data.ano == -1 || pd.quando.ano == localiza_data.ano))
            {
              printf("%s %6llu %6s %02d/%02d/%04d\n",
              pd.nome_cliente, pd.cpf, pd.tipo_bolo, pd.quando.dia, pd.quando.mes, pd.quando.ano);
              encontrados++;
            }
          }
          if (encontrados==0)
          {
            printf("\nNao foram encontrados resultados para a busca\n");
          }
          
          break;
        
        
        default:
          printf("DIGITO INCORRETO\n");
          while (getchar() != '\n');
          continue;
        }
      
      printf("\nContinuar? [S/N]: ");
      scanf(" %c",&escolha);
    
    }while(toupper(escolha)=='S');
    fclose(arq);
  }
