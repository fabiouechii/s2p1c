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
int compra();   
int file_exists(const char *filename);
void limparTela();
int deposito();
int saque();
int venda();
int extrato();
int saldo();


//declara funcoes antes delas aparecerem p/ nao da erro

typedef struct {
    char cpf[12];
    char senha[10];
    int saldo;
    int bit;
    int eth;
    int rip;
} Usuario;

//strutura usuario
//  ---------------------------------- ---------------------------------- ----------------------------------

int limite = 10; //define limite de cadastros possiveis
int estadomenu = 0; //estado do menu de opcoes
char cpf_usuario_logado[12]; //cpf salvo dps de login para consulta

//  ---------------------------------- ---------------------------------- ----------------------------------

void limparTela() {
  #ifdef _WIN32     //limpa tela win
    system("cls");
  #else
    system("clear"); //limpa tela mac/linux
  #endif
}

//  ---------------------------------- ---------------------------------- ----------------------------------

// usar no extrato

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

    Usuario usuario;

    int quantidade_usuarios = contar_cadastros();
    if (quantidade_usuarios >= limite) {  //chama contagem de usuarios, para ver se ainda tem limite de cadastro, se nao ele nem tenta
        printf("\nLimite de usuarios atingido!\n");
        inicio();
    }

    printf("\nDigite o seu CPF (somente numeros): "); //pede cpf
    scanf("%s", usuario.cpf);

    if (strlen(usuario.cpf) != 11) { //verifica se cpf inserido possui 11 dig
        limparTela();
        printf("O CPF deve conter 11 digitos!\n");
        cadastro();
    }

    if (novo_usuario(usuario.cpf)) {  //verifica se é novo usuario ou nao
        limparTela();
        printf("\nUsuario ja cadastrado!\n\n");
        login();
    }

    char novo_arquivo[30];
    snprintf(novo_arquivo, sizeof(novo_arquivo), "Usuario%d.bin", quantidade_usuarios + 1);
    FILE *fp = fopen(novo_arquivo, "wb");
    if (!fp) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    printf("\nDigite a sua senha: "); //pede senha
    scanf("%s", usuario.senha);

    usuario.saldo = 0; //valores iniciasi
    usuario.bit = 0;
    usuario.eth = 0;
    usuario.rip = 0;

    fwrite(&usuario, sizeof(Usuario), 1, fp);
    fclose(fp);

    limparTela();
    login();
    return 1;
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
        strcpy(cpf_usuario_logado, cpf); // SALVA cpf LOGADO para verificacao de arquivos
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
                extrato();
                break;
            case 3:
                limparTela();
                deposito();
                break;
            case 4:
                limparTela();
                saque();
                break;
            case 5:
                limparTela();
                compra();
                break;
            case 6:
                limparTela();
                venda();
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
                menu();
        }

    }

    return 0;
}


//  ---------------------------------- ----------------------------------

int saldo() {
  printf("saldo\n");
}


//  ---------------------------------- ----------------------------------

int deposito() {  
    printf("-----------------------\nDeposito...\n-----------------------\n");

    int saldo, bit, eth, rip;
    char senha_arquivo[10];  // senha certa

    for (int i = 1; i <= limite; i++) {
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

        if (file_exists(nome_arquivo)) {
            FILE *fp = fopen(nome_arquivo, "r+");
            if (!fp) {
                continue;
            }

            char linha[80];
            int cpf_encontrado = 0;
            while (fgets(linha, sizeof(linha), fp)) {
                if (strstr(linha, cpf_usuario_logado) != NULL) {
                    cpf_encontrado = 1;
                    //printf("aqui", cpf_usuario_logado);
                    fscanf(fp, "CPF: %*s\n");                   // pula cpf
                    fscanf(fp, "Senha: %s\n", senha_arquivo);   // le senha
                    fscanf(fp, "Saldo: %d\n", &saldo);          // le saldo
                    fscanf(fp, "Bit: %d\n", &bit);              // le bit
                    fscanf(fp, "Eth: %d\n", &eth);              // le eth
                    fscanf(fp, "Rip: %d\n", &rip);              // le rip
                    
                    int deposito;
                    printf("\nDigite o valor a ser depositado: ");
                    scanf("%d", &deposito);
                    saldo += deposito;  // adiciona saldo

                    rewind(fp);
                    fprintf(fp, "CPF: %s\n", cpf_usuario_logado);  // reescreve cpf
                    fprintf(fp, "Senha: %s\n", senha_arquivo);     // eescreve a senha
                    fprintf(fp, "Saldo: %d\n", saldo);             // atualiza saldo
                    fprintf(fp, "Bit: %d\n", bit);                 // reescreve bit
                    fprintf(fp, "Eth: %d\n", eth);                 // reescreve eth
                    fprintf(fp, "Rip: %d\n", rip);                 // reescreve rip
                    
                    fclose(fp);  // fehca arquivo
                    limparTela();
                    printf("\nDeposito realizado com sucesso!\n");
                    return 1;
                } 
        }
    }
    }
}




//  ---------------------------------- ----------------------------------

int saque() {  
    printf("-----------------------\nSaque...\n-----------------------\n");

    int saldo, bit, eth, rip;
    char senha_arquivo[10];  // senha do arquivo
    char senha_inserida[10]; // senha inserida

    for (int i = 1; i <= limite; i++) {
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

        if (file_exists(nome_arquivo)) {
            FILE *fp = fopen(nome_arquivo, "r+");
            if (!fp) {
                continue;
            }

            char linha[80];
            int cpf_encontrado = 0;
            while (fgets(linha, sizeof(linha), fp)) {
                if (strstr(linha, cpf_usuario_logado) != NULL) {
                    cpf_encontrado = 1;              
                    fscanf(fp, "CPF: %*s\n");                   // pula cpf
                    fscanf(fp, "Senha: %s\n", senha_arquivo);   // le senha
                    fscanf(fp, "Saldo: %d\n", &saldo);          // le saldo
                    fscanf(fp, "Bit: %d\n", &bit);              // le bit
                    fscanf(fp, "Eth: %d\n", &eth);              // le eth
                    fscanf(fp, "Rip: %d\n", &rip);              // le rip
                      

                    printf("\nDigite sua senha: ");
                    scanf("%s", senha_inserida);

                    if (strcmp(senha_inserida, senha_arquivo) != 0) {
                            fclose(fp);
                            limparTela();
                            printf("Senha incorreta! Operacao cancelada.\n");  //senha errada -> reinicia programa
                            estadomenu = 0;
                            inicio();
                            return 0;
                        }

                        int saque;
                        printf("\nDigite o valor de saque: ");
                        scanf("%d", &saque);

                        if (saque > saldo) {
                            limparTela();
                            printf("Saldo insuficiente!\n-----------------------\n");
                            fclose(fp);
                            return 0;
                        }

                        saldo -= saque;  // desconta saque do saldo

                        rewind(fp);
                        fprintf(fp, "CPF: %s\n", cpf_usuario_logado);  // reescreve cpf
                        fprintf(fp, "Senha: %s\n", senha_arquivo);     // eescreve a senha
                        fprintf(fp, "Saldo: %d\n", saldo);             // atualiza saldo
                        fprintf(fp, "Bit: %d\n", bit);                 // reescreve bit
                        fprintf(fp, "Eth: %d\n", eth);                 // reescreve eth
                        fprintf(fp, "Rip: %d\n", rip);                 // reescreve rip
                        
                        fclose(fp);  //fecha o arquivo
                        limparTela();
                        printf("Saque realizado com sucesso!\n");
                        return 1;
                }
              }
            fclose(fp);
        }
    }
}

//  ---------------------------------- ----------------------------------

int compra() {
  int moeda=0;
  int m_if=0;
  int valor;
  float cot_bit, cot_eth, cot_rip,cotacao;



  FILE *fp = fopen("Cotacao.bin", "r+");
  fscanf(fp, "Bit: %f\n", &cot_bit);              // le cotacao bit
  fscanf(fp, "Eth: %f\n", &cot_eth);              // le cotacao eth
  fscanf(fp, "Rip: %f\n", &cot_rip);              // le cotacao rip
  

  printf("-----------------------\nComprar Criptomoeda...\n-----------------------\n");
  printf("\nDigite a moeda desejada: \n\n");
  printf("1- Bitcoin\n");
  printf("2- Ethereum\n");
  printf("3- Ripple\n");
  printf("\nSua opcao: ");
  scanf("%d", &moeda);
  fclose(fp);

  if (moeda == 1){
    cotacao = cot_bit;
  }
  else if(moeda == 2){
    cotacao = cot_eth;
  }
  else if(moeda == 3){
    cotacao = cot_rip;
  }
  else{
    limparTela();
    printf("Digite 1, 2 ou 3\n"); //valor digitado e invalido
    compra(); //reinicia fucao
  }
 


  printf("Cotacao: %d", cotacao);  
  printf("\nValor da compra: ");
  scanf("%d", &valor);

}
      















//  ---------------------------------- ----------------------------------
int venda(){
  printf("venda");
}
//  ---------------------------------- ----------------------------------
int extrato(){
  printf("extrato");
}
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

            Usuario usuario;
            fread(&usuario, sizeof(Usuario), 1, fp);
            fclose(fp);

            if (strcmp(usuario.cpf, cpf) == 0) {
                    printf("CPF encontrado\n");
                    return 1;
            }
        }
    }
    printf("CPF nao encontrado\n");
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

            Usuario usuario;
            fread(&usuario, sizeof(Usuario), 1, fp);
            fclose(fp);

            if (strcmp(usuario.cpf, cpf) == 0 && strcmp(usuario.senha, senha) == 0) {
                return 1;  //cpf e senha encontrados
              }
        }
    }
    return 0; //cpf e senha NAO encontrados
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