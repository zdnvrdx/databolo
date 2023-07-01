/* bolos.h */
#define NOMEARQUIVO "bolos.txt"
#define max_length_bolo 20
#define max_length_nome 100

struct data{
  int dia, mes, ano;
};

struct pedido{
  struct data quando;
  unsigned long long int cpf;
  char tipo_bolo[max_length_bolo];
  char nome_cliente[max_length_nome];
};
