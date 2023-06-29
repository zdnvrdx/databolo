/* bolos.h */
#define NOMEARQUIVO "bolos.txt"
#define max_length_bolo 20
#define max_length_nome 50

struct data{
  int dia, mes, ano;
};

struct cliente{
  char nome[max_length_nome];
  long int cpf;
};

struct pedido{
  struct data quando;
  long int cpf;
  char tipo_bolo[max_length_bolo];
};