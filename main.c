#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PESSOAS 10

//  ----------------------------------

void limparTela() {
  #ifdef _WIN32
  system("cls");
  #endif
}

// ----------------------------------

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

//  ----------------------------------

void inicio(){

  int entrada;

  printf("Digite a opção desejada: \n\n");
  printf("Cadastro = 1\n");
  printf("Login = 2\n");
  printf("\nSua opção: ");
  scanf("%d", &entrada);

  if (entrada == 1) {
      printf("\n\nÁrea de cadastro!\n\n");
      cadastro();
  } else if (entrada == 2) {
      printf("\n\nÁrea de login!\n");
      login();
    } else {
      printf("\n\nOpção inválida!\n");
    }
}

// ---------------------------------- 

int cadastro(){

  char cpf[12];
  char cpf_formatado[15];
  char senha[10];

  // pedindo o CPF do usuário
  printf("Digite o seu CPF: ");
  scanf("%s", cpf);
  formatar_cpf(cpf, cpf_formatado);
  if (cpf_formatado[0] == '\0') {
    printf("O CPF deve conter 11 dígitos!\n");
    return 0;
  }

  if (novo_usuario(cpf_formatado)){
    printf("\nUsuário já cadastrado!");
    login();
    return 0;
    
  }else{
    novo_usuario();
  }
  
  int quantidade_usuarios = 0;
  if (quantidade_usuarios >= MAX_PESSOAS){
    printf("Limite de usuários atingido!");
    return 0;
  }

  char novo_arquivo[30];
  snprintf(novo_arquivo, sizeof(novo_arquivo), "Usuário%d.bin", quantidade_usuarios + 1);
  FILE *fp = fopen(novo_arquivo, "wb");
  if(!fp){
    printf("Erro para abrir aqruivo");
    return 0;
  }

  // pedindo a senha do usuário
  printf("\nDigite a sua senha: ");
  scanf("%s", senha);
  printf("Sua senha: %s\n", senha);

  // fechando o aquivo
  fprintf(fp, "CPF: %s\n", cpf_formatado);
  fprintf(fp, "Senha: %s\n", senha);
  fclose(fp);



  return 0;
}

// ----------------------------------

int login(){
  
  char cpf[12];
  char cpf_formatado[15];
  char senha[10];
  
  printf("\n\nDigite o seu CPF: ");
  scanf("%s", cpf);
  formatar_cpf(cpf, cpf_formatado);
  if (cpf_formatado[0] == '\0') {
    printf("O CPF deve conter 11 dígitos!\n");
    return 0;
  }

  printf("\nDigite a sua senha: ");
  scanf("%s", senha);

  if (conferindo_senha(senha)){
    printf("\nRedirecionando para o menu!");
    menu();
    return 0;
  }else{
    printf("Senha incorreta!");
  }  
}

// ----------------------------------

int menu(){
  
  int opcao[1];
  
  printf("\nDigite a opção desejada: \n");
  
  printf("1. Consultar saldo\n");
  printf("2. Consultar extrato\n");
  printf("3. Depositar\n");
  printf("4. Sacar\n");
  printf("5. Comprar Criptomoeda\n");
  printf("6. Vender Criptomoeda\n");
  printf("7. Atualizar cotação\n");
  printf("8. Sair\n");

  printf("\nDigite a opção desejada: \n");
  scanf("%d", opcao);

  if (opcao == 1){
    saldo();
  }
  if (opcao == 2){
    extrato();
  }
  if (opcao == 3){
    depositar();
  }
  if (opcao == 4){
    sacar();
  }
  if (opcao == 5){
    comprar_cripto();
  }
  if (opcao == 6){
    vender_cripto();
  }
  if (opcao == 7){
    cotacao();
  }
  if (opcao == 8){
    sair();
  }
}

// ----------------------------------

int saldo(){
  printf("saldo");
}

// ----------------------------------

int novo_usuario(const char *cpf_formatado){
  for (int i = 1; i <= MAX_PESSOAS; i++){
    char nome_arquivo[20];
    //armazenando o nome do arquivo no "nome_arquivo", de acordo com o tamanho do mesmo.
    snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuário%d.bin", i);

    // abrindo o arquivo como leitura
    if (file_exists(nome_arquivo)){
      FILE *fp = fopen(nome_arquivo, "rb");
      char linhas[80];

      //lendo cada linha do arquivo
      while (fgets(linhas, sizeof(linhas), fp)){
        // verificando se o cpf esta na linha, se estiver ele interrompe o loop
        if(strstr(linhas, cpf_formatado) != NULL){
          fclose(fp);
          return 1;
        }
      }
      fclose(fp);
    }
  }
  return 0;
}

// ----------------------------------

int conferindo_senha(const char *senha){
  for (int i = 1; i <= MAX_PESSOAS; i++){
    char nome_arquivo[20];
    //armazenando o nome do arquivo no "nome_arquivo", de acordo com o tamanho do mesmo.
    snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuário%d.bin", i);

    // abrindo o arquivo como leitura
    if (file_exists(nome_arquivo)){
      FILE *fp = fopen(nome_arquivo, "rb");
      char linhas[80];

      //lendo cada linha do arquivo
      while (fgets(linhas, sizeof(linhas), fp)){
        // verificando se o cpf esta na linha, se estiver ele interrompe o loop
        if(strstr(linhas, senha) != NULL){
          fclose(fp);
          return 1;
        }
      }
      fclose(fp);
    }
  }
  return 0;
}

// ----------------------------------

int file_exists(const char *filename){
  FILE *file = fopen(filename, "rb");
  if(file){
    fclose(file);
    return 1;
  }
  return 0;
}

// ----------------------------------

int main(){
  
  const char *filename = "cadastro.txt";
  if(file_exists(filename)){
    printf("O arquivo existe.\n");
  }else{
    printf("O arquivo não existe.\n");
  }
  
  inicio();
  limparTela();
  
  return 0;
}