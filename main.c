#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//  ---------------------------------- ---------------------------------- ----------------------------------

int cadastro(); 
int login();    
void formatar_cpf(const char* cpf, char* cpf_formatado);
int novo_usuario(const char *cpf_formatado);
int contar_cadastros();
int conferir_usuario(const char *cpf, const char *senha);
int menu();     
int saldo(); 
int criptocompra();   
int file_exists(const char *filename);
void limparTela();

//declara funcoes antes delas aparecerem p/ nao da erro
//  ---------------------------------- ---------------------------------- ----------------------------------

int limite = 10; //define limite de cadastros possiveis
int estadomenu = 0;

//  ---------------------------------- ---------------------------------- ----------------------------------

void limparTela() {
  #ifdef _WIN32     //limpa tela win
    system("cls");
  #else
    system("clear"); //limpa tela mac/linux
  #endif
}

//  ---------------------------------- ---------------------------------- ----------------------------------

// deixei de usar o formartar cpf, vou usar qnd for printar o cpf só

void formatar_cpf(const char* cpf, char* cpf_formatado) {
  if (strlen(cpf) != 11){
    cpf_formatado[0] = '\0';
    return;
  }

  snprintf(cpf_formatado, 15, "%c%c%c.%c%c%c.%c%c%c-%c%c", 
    cpf[0],cpf[1],cpf[2],
    cpf[3],cpf[4],cpf[5],
    cpf[6],cpf[7],cpf[8],
    cpf[9],cpf[10]);
}

//  ---------------------------------- ---------------------------------- ----------------------------------

void inicio() {


  int entrada;

  printf("-----------------------\nBem-vindo...\n-----------------------\n");
  printf("\nDigite a opcao desejada: \n\n");
  printf("1- Cadastro\n");
  printf("2- Login\n");
  printf("\nSua opcao: ");
  scanf("%d", &entrada);

  if (entrada == 1) { //leva p/ cadastro
      limparTela();
      cadastro();

  } else if (entrada == 2) { //leva p/ login
      limparTela();
      login();
  } else {
      printf("\n-----------------------\nOpcao invalida"); //mantem no inicio
      inicio();
  }
}

//  ---------------------------------- ----------------------------------

int cadastro() {

  printf("-----------------------\nArea de cadastro...\n-----------------------\n");


  char cpf[12];
  char senha[10];



  int quantidade_usuarios = contar_cadastros();
  if (quantidade_usuarios >= limite) {  //chama contagem de usuarios, para ver se ainda tem limite de cadastro, se nao ele nem tenta
    printf("\nLimite de usuarios atingido!");
    inicio();
  }


  printf("\nDigite o seu CPF (somente numeros): "); //pede cpf
  scanf("%s", cpf);

  if (strlen(cpf) != 11) { //verifica se cpf inserido possui 11 dig
    limparTela();
    printf("O CPF deve conter 11 digitos!\n");
    cadastro();
  }

  if (novo_usuario(cpf)) { //verifica se é novo usuario ou nao
    limparTela();
    printf("\nUsuario ja cadastrado!\n\n");
    login();
  }

  

  char novo_arquivo[30]; //grava novo usuario
  snprintf(novo_arquivo, sizeof(novo_arquivo), "Usuario%d.bin", quantidade_usuarios + 1);
  FILE *fp = fopen(novo_arquivo, "wb");
  if (!fp) {
    printf("Erro para abrir arquivo");
    return 0;
  }

  printf("\nDigite a sua senha: "); //pede senha
  scanf("%s", senha);
  printf("Sua senha: %s\n", senha);

  fprintf(fp, "CPF: %s\n", cpf); // grava e fecha arquivo novo
  fprintf(fp, "Senha: %s\n", senha);
  fclose(fp);

  limparTela();
  login();
}

//  ---------------------------------- ----------------------------------

int login() {

  
  char cpf[12];
  char senha[10];

  printf("-----------------------\nTela de login...\n-----------------------\n");

  printf("\n\nDigite o seu CPF: "); //pede cpf
  scanf("%s", cpf);

  if (strlen(cpf) != 11) { //verifica se cpf inserido tem 11 dig
    limparTela();
    printf("O CPF deve conter 11 digitos!\n");
    login();
  }

  printf("\nDigite a sua senha: "); //pede senha
  scanf("%s", senha);

   if (conferir_usuario(cpf, senha)) { //verifica se usuario inserido esta cadastrado
        limparTela();
        estadomenu=1;
        menu();   //certo -> vai pro menu
    } else {
        limparTela();
        printf("\nCPF ou senha incorretos!\n\n");
        inicio(); //errado -> volta inicio
    }
  
    return 0;
}

//  ---------------------------------- ----------------------------------

int menu() {
    int opcao;
    printf("-----------------------\nMenu...\n-----------------------\n");  //printa opcoes antes do loop pra n ficar repetitivo

    printf("1. Consultar saldo\n");
    printf("2. Consultar extrato\n");
    printf("3. Depositar\n");
    printf("4. Sacar\n");
    printf("5. Comprar Criptomoeda\n");
    printf("6. Vender Criptomoeda\n");
    printf("7. Atualizar cotacao\n");
    printf("8. Sair\n");
    
    while (estadomenu ==1) {  // loop ate usuario sair

        printf("\nDigite a opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                limparTela();
                saldo();
                break;
            case 2:
                limparTela();
                printf("extrato\n");
                break;
            case 3:
                limparTela();
                printf("depositar\n");
                break;
            case 4:
                limparTela();
                printf("sacar\n");
                break;
            case 5:
                limparTela();
                criptocompra();
                break;
            case 6:
                limparTela();
                printf("vender cripto\n");
                break;
            case 7:
                limparTela();
                printf("cotacao\n");
                break;
            case 8:
                limparTela();
                estadomenu=0;
                printf("Saindo do menu...\n");
                inicio();  //  sai do loop e volta inicio
            default:
                printf("Opcao invalida!\n");
                break;
        }

    }

    return 0;
}


//  ---------------------------------- ----------------------------------

int saldo() {
  printf("saldo\n");
}
//  ---------------------------------- ----------------------------------

int criptocompra(){

int moeda=0;
int m_if=0;
int valor;

  printf("-----------------------\nComprar Criptomoeda...\n-----------------------\n");
  printf("\nDigite a moeda desejada: \n\n");
  printf("1- Bitcoin\n");
  printf("2- Ethereum\n");
  printf("3- Ripple\n");
  printf("\nSua opcao: ");
  scanf("%d", &moeda);

  if ((moeda == 1) || (moeda == 2) || (moeda == 3)) {
    m_if = 1;
    printf("foi");
  }
  else{
    limparTela();
    printf("Digite 1, 2 ou 3\n");
    criptocompra();
  }


  
  if (m_if == 1){
    //puxar cotacao
    printf("Cotacao:\n");
    printf("\nValor da compra: ");
    scanf("%d", &valor);
    //puxar saldo
    if (vsaldo >= valor){
      //calculo de criptos comprada
      //saldo - valor
      //salva saldo
      //salva cripto
    }
    else{
      printf("Saldo Insuficiente");
    }


  }





}


int consulta_cotacao();



int verifica_saldo();
int atualiza_saldo();










//  ---------------------------------- ----------------------------------


//  ---------------------------------- ----------------------------------

int novo_usuario(const char *cpf) {  // verifica se novo usuario ja existe ou nao
    for (int i = 1; i <= limite; i++) {
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

        // verifica arquivo
        if (file_exists(nome_arquivo)) {
            FILE *fp = fopen(nome_arquivo, "rb");
            if (!fp) {
                continue;
            }

            char linha[80];
            while (fgets(linha, sizeof(linha), fp)) {
                // verifica CPF 
                if (strstr(linha, cpf) != NULL) {
                    fclose(fp);
                    printf("cpf encontrado\n");
                    return 1;  
                }
            }
            fclose(fp);
        }
    }
    printf("cpf nao encontrado\n");
    return 0;  
}


//  ---------------------------------- ----------------------------------

int conferir_usuario(const char *cpf, const char *senha) {  // verifica se usuario no login ta certo ou nao
    for (int i = 1; i <= limite; i++) {
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

        if (file_exists(nome_arquivo)) {
            FILE *fp = fopen(nome_arquivo, "rb");
            if (!fp) {
                continue;
            }

            char linha[80];
            int cpf_encontrado = 0;
            while (fgets(linha, sizeof(linha), fp)) {
                if (strstr(linha, cpf) != NULL) {
                    cpf_encontrado = 1;
                } else if (cpf_encontrado && strstr(linha, senha) != NULL) {
                    fclose(fp);
                    return 1;  // cpf e senha encontrados
                }
            }
            fclose(fp);
        }
    }
    return 0;  // cpf e senha nao encontrados
}
//  ---------------------------------- ----------------------------------

int file_exists(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file) {
    fclose(file);
    return 1;
  }
  return 0;
}

//  ---------------------------------- ----------------------------------

int contar_cadastros() {  // conta se ja esta no limite dew cadastros
  int count = 0;

  for (int i = 1; i <= limite; i++) {
    char nome_arquivo[30];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);
    if (file_exists(nome_arquivo)) {
      count++;
    }
  }
  return count;
}

//  ---------------------------------- ----------------------------------

int main() {

  const char *filename = "cadastro.txt";
  if (file_exists(filename)) {
    printf("O arquivo existe.\n");
  } else {
    printf("O arquivo nao existe.\n");
  }

  limparTela();   //limpa tela dps chama inicio
  inicio();
  

  return 0;
}
