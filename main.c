/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "bolos.h"

void consultarPedidos();

void pedidoNovo();

void localizarPedidos();

void excluirPedidos();

int main(void)
{
    int flagMenu = -1;

    printf("██████╗░░█████╗░████████╗░█████╗░██████╗░░█████╗░██╗░░░░░░█████╗░\n");
    printf("██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗██╔══██╗██║░░░░░██╔══██╗\n");
    printf("██║░░██║███████║░░░██║░░░███████║██████╦╝██║░░██║██║░░░░░██║░░██║\n");
    printf("██║░░██║██╔══██║░░░██║░░░██╔══██║██╔══██╗██║░░██║██║░░░░░██║░░██║\n");
    printf("██████╔╝██║░░██║░░░██║░░░██║░░██║██████╦╝╚█████╔╝███████╗╚█████╔╝\n");
    printf("╚═════╝░╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═════╝░░╚════╝░╚══════╝░╚════╝░\n");

    while (flagMenu != 5) {
        printf("\nDigite:\n 1 - NOVO PEDIDO\n 2 - CONSULTAR PEDIDOS\n 3 - LOCALIZAR PEDIDOS\n 4 - EXCLUIR PEDIDOS\n 5 - SAIR\n ");
        scanf("%d", &flagMenu);
        switch (flagMenu) {
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
                excluirPedidos();
                flagMenu = 0;
                break;
            case 5:
                break;
            default:
                printf("DIGITO INCORRETO!\n");
                while (getchar() != '\n');
                continue;
        }
    }

    return (0);
}

void consultarPedidos() {
    FILE *arq;
    struct pedido p;
    arq = fopen(NOMEARQUIVO, "rb");
    if (arq == NULL) {
        fprintf(stderr, "Nao abriu %s\n", NOMEARQUIVO);
        exit(1);
    }
    printf("---------------------------------\n");
    printf("-- Nome ----- CPF ----- Bolo ---- Data --\n");

    while (fread(&p, sizeof(p), 1, arq) == 1 && !feof(arq)) {
        char nome_lower[100];
        strcpy(nome_lower, p.nome_cliente);
        for (int i = 0; i < strlen(nome_lower); i++) {
            nome_lower[i] = tolower(nome_lower[i]);
        }

        printf("%s %6llu %6s %02d/%02d/%04d\n",
               nome_lower, p.cpf, p.tipo_bolo, p.quando.dia, p.quando.mes, p.quando.ano);
    }
    printf("---------------------------------\n");
    fclose(arq);
}

void pedidoNovo() {
    FILE *arq;
    char novo_nome[100];
    struct pedido pd;
    int data_valida = 0;
    int cont_cpf;
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

        for (int i = 0; i < strlen(novo_nome); i++) {
            novo_nome[i] = tolower(novo_nome[i]);
        }

        strcpy(pd.nome_cliente, novo_nome);
        printf("CPF (somente numeros): ");

        do {
            cont_cpf = 0;
            if (scanf("%llu", &pd.cpf) != 1) {
                printf("CPF precisa conter apenas numeros. Tente novamente: ");
                while (getchar() != '\n');
            } else {
                temp_cpf = pd.cpf;
                while (temp_cpf != 0) {
                    temp_cpf /= 10;
                    cont_cpf++;
                }
                if (cont_cpf != 11) {
                    printf("O CPF precisa de 11 digitos\n");
                    printf("Informe o CPF novamente: ");
                }
            }
        } while (cont_cpf != 11);

        printf("tipo de bolo: ");
        scanf("%s", pd.tipo_bolo);
      for (int i = 0; i < strlen(pd.tipo_bolo); i++) {
                    pd.tipo_bolo[i] = tolower(pd.tipo_bolo[i]);
                }
        printf("Data (DD/MM/AAAA): ");
        do {
            if (scanf("%d/%d/%d", &pd.quando.dia, &pd.quando.mes, &pd.quando.ano) != 3) {
                printf("Data invalida. Tente novamente (DD/MM/AAAA): ");
                while (getchar() != '\n');
            } else if (pd.quando.dia < 1 || pd.quando.dia > 31 || pd.quando.mes < 1 || pd.quando.mes > 12) {
                if (pd.quando.dia < 1 || pd.quando.dia > 31) {
                    printf("Dia invalido. Insira um valor entre 1 e 31.\n");
                } else if (pd.quando.mes < 1 || pd.quando.mes > 12) {
                    printf("Mes invalido. Insira um valor entre 1 e 12.\n");
                }
                printf("Tente novamente (DD/MM/AAAA): ");
                while (getchar() != '\n');
            } else if ((pd.quando.dia > 30) &&
                       (pd.quando.mes == 4 || pd.quando.mes == 6 || pd.quando.mes == 9 || pd.quando.mes == 11)) {
                printf("O mes %d contem apenas 30 dias. Insira um valor entre 1 e 30 para o dia.\n",
                       pd.quando.mes);
                printf("Tente novamente (DD/MM/AAAA): ");
                while (getchar() != '\n');
            } else if (pd.quando.dia == 29 && pd.quando.mes == 2) {
                if ((pd.quando.ano % 4 == 0 && pd.quando.ano % 100 != 0) || pd.quando.ano % 400 == 0) {
                    data_valida = 1;
                } else {
                    printf("O ano %d nao e bissexto. Informe novamente (DD/MM/AAAA): ", pd.quando.ano);
                }
            } else if (pd.quando.dia > 28 && pd.quando.mes == 2) {
                printf("Fevereiro contem apenas 28 dias. Informe novamente (DD/MM/AAAA): ");
            } else {
                data_valida = 1;
            }
        } while (data_valida != 1);

        /* Posiciona o cursor no final do arquivo */
        fseek(arq,0,SEEK_END);
        /* Inclui novo registro */
        fwrite(&pd,sizeof(pd),1,arq);

        printf("\nContinuar? [S/N]: ");
        scanf(" %c",&escolha);
    } while(toupper(escolha)=='S');
    fclose(arq);
}


void localizarPedidos() {
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
        switch (tipo_localiza) {
            case 1:
                printf("\nNome: ");
                scanf(" %[^\n]", localiza);
                for (int i = 0; i < strlen(localiza); i++) {
                    localiza[i] = tolower(localiza[i]);
                }
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
                for (int i = 0; i < strlen(localiza); i++) {
                    localiza[i] = tolower(localiza[i]);
                }
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
                for (int i = 0; i < strlen(localiza); i++) {
                    localiza[i] = tolower(localiza[i]);
                }
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



void excluirPedidos() {
    FILE *arq;
    FILE *arq_temp;
    struct pedido pd;
    char nome_excl[100];
    unsigned long long int cpf_excl;
    int data_valida = 0, excluidos = 0;
    int cont_cpf;
    unsigned long long int temp_cpf;
    char tipo_bolo_excl[30];
    struct data data_excl;

    printf("Digite os parametros do pedido a ser excluido\n");
    printf("Nome: ");
    scanf(" %[^\n]", nome_excl);
  for (int i = 0; i < strlen(nome_excl); i++) {
                    nome_excl[i] = tolower(nome_excl[i]);
                }
    printf("CPF (somente numeros): ");
  

    do {
        cont_cpf = 0;
        if (scanf("%llu", &cpf_excl) != 1) {
            printf("CPF precisa conter apenas numeros. Tente novamente: ");
            while (getchar() != '\n');
        } else {
            temp_cpf = cpf_excl;
            while (temp_cpf != 0) {
                temp_cpf /= 10;
                cont_cpf++;
            }
            if (cont_cpf != 11) {
                printf("O CPF precisa de 11 digitos\n");
                printf("Informe o CPF novamente: ");
            }
        }
    } while (cont_cpf != 11);

    printf("tipo de bolo: ");
    scanf("%s", tipo_bolo_excl);
  for (int i = 0; i < strlen(tipo_bolo_excl); i++) {
                    tipo_bolo_excl[i] = tolower(tipo_bolo_excl[i]);
                }
    printf("Data (DD/MM/AAAA): ");
    do {
        if (scanf("%d/%d/%d", &data_excl.dia, &data_excl.mes, &data_excl.ano) != 3) {
            printf("Data invalida. Tente novamente (DD/MM/AAAA): ");
            while (getchar() != '\n');
        } else if (data_excl.dia < 1 || data_excl.dia > 31 || data_excl.mes < 1 || data_excl.mes > 12) {
            if (data_excl.dia < 1 || data_excl.dia > 31) {
                printf("Dia invalido. Insira um valor entre 1 e 31.\n");
            } else if (data_excl.mes < 1 || data_excl.mes > 12) {
                printf("Mes invalido. Insira um valor entre 1 e 12.\n");
            }
            printf("Tente novamente (DD/MM/AAAA): ");
            while (getchar() != '\n');
        } else if ((data_excl.dia > 30) &&
                   (data_excl.mes == 4 || data_excl.mes == 6 || data_excl.mes == 9 || data_excl.mes == 11)) {
            printf("O mes %d contem apenas 30 dias. Insira um valor entre 1 e 30 para o dia.\n", data_excl.mes);
            printf("Tente novamente (DD/MM/AAAA): ");
            while (getchar() != '\n');
        } else if (data_excl.dia == 29 && data_excl.mes == 2) {
            if ((data_excl.ano % 4 == 0 && data_excl.ano % 100 != 0) || data_excl.ano % 400 == 0) {
                data_valida = 1;
            } else {
                printf("O ano %d nao e bissexto. Informe novamente (DD/MM/AAAA): ", data_excl.ano);
            }
        } else if (data_excl.dia > 28 && data_excl.mes == 2) {
            printf("Fevereiro contem apenas 28 dias. Informe novamente (DD/MM/AAAA): ");
        } else {
            data_valida = 1;
        }
    } while (data_valida != 1);

    arq = fopen(NOMEARQUIVO, "rb");
    if (arq == NULL) {
        fprintf(stderr, "Nao foi possivel abrir o arquivo %s\n", NOMEARQUIVO);
        exit(1);
    }

    // Cria um arquivo temporario
    arq_temp = fopen("temp.txt", "wb");
    if (arq_temp == NULL) {
        fprintf(stderr, "Nao foi possivel criar o arquivo temporario\n");
        fclose(arq);
        exit(1);
    }

    // Copia cada pedido do arquivo original, menos o que será excluido
    while (fread(&pd, sizeof(pd), 1, arq) == 1 && !feof(arq)) {
        if (strcasecmp(pd.nome_cliente, nome_excl) == 0 &&
          pd.cpf == cpf_excl &&
          pd.quando.dia == data_excl.dia && pd.quando.mes == data_excl.mes && pd.quando.ano == data_excl.ano &&
          strcasecmp(pd.tipo_bolo, tipo_bolo_excl) == 0) {
          printf("Deseja realmente excluir o pedido de %s? (S/N): ", pd.nome_cliente);
          char confirm;
          scanf(" %c", &confirm);
          if (tolower(confirm) == 's') {
            excluidos++;
              continue;
        }
          else excluidos--;  
}

        // Escreva o pedido atual no arquivo temporário
        fwrite(&pd, sizeof(pd), 1, arq_temp);
    }

    // Fecha os arquivos de entrada e temporários
    fclose(arq);
    fclose(arq_temp);

    // Remove o arquivo original
    if (remove(NOMEARQUIVO) != 0) {
        fprintf(stderr, "Erro ao excluir o arquivo %s\n", NOMEARQUIVO);
        exit(1);
    }

    // Renomeia o arquivo temporário para o arquivo original
    if (rename("temp.txt", NOMEARQUIVO) != 0) {
        fprintf(stderr, "Erro ao renomear o arquivo temporario\n");
        exit(1);
    }
    if (excluidos == 1)
        printf("\nPedido excluido com sucesso\n");
    else if (excluidos == -1)
        printf("\nPedido nao excluido\n");
    else
        printf("\nNao foram encontrados pedidos com esses parametros\n");
}