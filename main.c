#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

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
    float bit;
    float eth;
    float rip;
} Usuario;

//strutura usuario

typedef struct {
    float bit;  // cot bit
    float eth;  //cot eth
    float rip;  //cot rip
} Cotacao;



//strutura da cortcao
//  ---------------------------------- ---------------------------------- ----------------------------------


void atualizar() {
    FILE *fp;
    Cotacao cotacao;
    const char *nome_arquivo = "cotacao.bin";

    fp = fopen(nome_arquivo, "rb"); //verifica se existe arquivo
    if (!fp) {
        fp = fopen(nome_arquivo, "wb"); //se n existe cria
        if (!fp) {
            printf("erro.\n");
            return;
        }

        cotacao.bit = 300.0;
        cotacao.eth = 100;
        cotacao.rip = 10;    

        fwrite(&cotacao, sizeof(Cotacao), 1, fp);
        fclose(fp);

        atualizar();
    }

    fread(&cotacao, sizeof(Cotacao), 1, fp); //se ja existe -> le e atualiza
    fclose(fp);

    srand(time(NULL));  //gera valor randon

    float variacao_bit = ((rand() % 11) - 5) / 100.0; //variacao entre -5 e 5% 
    cotacao.bit += cotacao.bit * variacao_bit;//BIT

    float variacao_eth = ((rand() % 11) - 5) / 100.0; //variacao entre -5 e 5% 
    cotacao.eth += cotacao.eth * variacao_eth;//ETH

    float variacao_rip = ((rand() % 11) - 5) / 100.0; //variacao entre -5 e 5% 
    cotacao.rip += cotacao.rip * variacao_rip;//rip


    printf("-----------------------\nCotaoees atualizadas!\n-----------------------\n");

    printf("\nNovas cotacoes:\n");
    printf("Bitcoin: %.2f\n", cotacao.bit);
    printf("Ethereum: %.2f\n", cotacao.eth);
    printf("Ripple: %.2f\n-----------------------\n", cotacao.rip);

    fp = fopen(nome_arquivo, "wb");
    if (!fp) {
        printf("erro\n");
        return;
    }

    fwrite(&cotacao, sizeof(Cotacao), 1, fp);
    fclose(fp);
}
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
                atualizar();
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
  printf("-----------------------\nSaldo...\n-----------------------\n");
    // Abre o arquivo do usuário logado para ler o saldo
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

            if (strcmp(usuario.cpf, cpf_usuario_logado) == 0) {
                // Se o CPF do usuário logado corresponde, exibe o saldo
                printf("Saldo em dinheiro: R$ %d\n", usuario.saldo);
                printf("Saldo em Bitcoin: %.4f BTC\n", usuario.bit);
                printf("Saldo em Ethereum: %.4f ETH\n", usuario.eth);
                printf("Saldo em Ripple: %.4f XRP\n", usuario.rip);
                printf("-----------------------\n");
                fclose(fp);
                return 1; // Retorna 1 indicando que a operação foi bem-sucedida
            }
            fclose(fp);
        }
    }
    printf("\nErro ao consultar saldo!\n");
    return 0; // Retorna 0 se não conseguir encontrar o saldo
}


//  ---------------------------------- ----------------------------------

int deposito() {
    int valor_deposito;
    printf("\nDigite o valor que deseja depositar: ");
    scanf("%d", &valor_deposito);

    for (int i = 1; i <= limite; i++) { //procura arquivo do usuario logado
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

        if (file_exists(nome_arquivo)) {
            FILE *fp = fopen(nome_arquivo, "rb+");
            if (!fp) {
                continue;
            }

            Usuario usuario;
            fread(&usuario, sizeof(Usuario), 1, fp);

            if (strcmp(usuario.cpf, cpf_usuario_logado) == 0) {
                usuario.saldo += valor_deposito;
                fseek(fp, 0, SEEK_SET);
                fwrite(&usuario, sizeof(Usuario), 1, fp);
                fclose(fp);
                printf("\nDeposito realizado com sucesso! Saldo atual: %d\n", usuario.saldo);
                return 1;
            }
            fclose(fp);
        }
    }
    printf("\nErro ao realizar o deposito!\n");
    return 0;
}




//  ---------------------------------- ----------------------------------

int saque() {
    int valor_saque;
    printf("\nDigite o valor que deseja sacar: ");
    scanf("%d", &valor_saque);

    for (int i = 1; i <= limite; i++) {
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

        if (file_exists(nome_arquivo)) {
            FILE *fp = fopen(nome_arquivo, "rb+"); //abre arquivo
            if (!fp) {
                continue;
            }

            Usuario usuario;
            fread(&usuario, sizeof(Usuario), 1, fp);

            if (strcmp(usuario.cpf, cpf_usuario_logado) == 0) {
                if (usuario.saldo >= valor_saque) { //ve se saldo ok
                    usuario.saldo -= valor_saque; //desconta saque
                    fseek(fp, 0, SEEK_SET);
                    fwrite(&usuario, sizeof(Usuario), 1, fp);
                    fclose(fp);
                    limparTela();
                    printf("\nSaque realizado com sucesso! Saldo atual: %d\n", usuario.saldo);
                    return 1;
                } else {
                    fclose(fp);
                    limparTela();
                    printf("\nSaldo insuficiente!\n");
                    saque();
                }
            }
            fclose(fp);
        }
    }
    printf("\nErro\n");
    return 0;
}

//  ---------------------------------- ----------------------------------

int compra() {
    int moeda = 0;
    int valor;
    float cotacao;
    float taxa;
    Cotacao cotacaoAtual;

    FILE *fp = fopen("cotacao.bin", "rb");
    if (!fp) {
        printf("erro\n");
        return 0;
    }

    fread(&cotacaoAtual, sizeof(Cotacao), 1, fp); //le cotacoes
    fclose(fp);

    //printf("Cotacao atual:\n"); 
    //printf("Bitcoin: %.2f\n", cotacaoAtual.bit);
    //printf("Ethereum: %.2f\n", cotacaoAtual.eth);
    //printf("Ripple: %.2f\n", cotacaoAtual.rip);

    printf("-----------------------\nComprar Criptomoeda...\n-----------------------\n");
    printf("\nDigite a moeda desejada: \n\n");
    printf("1- Bitcoin\n");
    printf("2- Ethereum\n");
    printf("3- Ripple\n");
    printf("\nSua opcao: ");
    scanf("%d", &moeda);



    if (moeda == 1) { 
        cotacao = cotacaoAtual.bit;
        taxa = 0.02;  //2% bitcoin

    } else if (moeda == 2) {
        cotacao = cotacaoAtual.eth;
        taxa = 0.01;  //1% eth

    } else if (moeda == 3) {
        cotacao = cotacaoAtual.rip;
        taxa = 0.01;  //1% rip

    } else {
        limparTela();
        printf("Digite 1, 2 ou 3\n"); // valor digitado é inválido
        compra(); // reinicia função
        return 0; // Não prossegue após chamada recursiva
    }

    limparTela();
    printf("\nCotacao: %.2f", cotacao);
    printf("\nDigite o valor em dinheiro que deseja usar para comprar a criptomoeda: ");
    scanf("%d", &valor);


    for (int i = 1; i <= limite; i++) { //procura saldo
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

        if (file_exists(nome_arquivo)) {
            FILE *fp = fopen(nome_arquivo, "rb+");
            if (!fp) {
                continue;
            }

            Usuario usuario;
            fread(&usuario, sizeof(Usuario), 1, fp);

            if (strcmp(usuario.cpf, cpf_usuario_logado) == 0) {
                if (usuario.saldo >= valor) {

                    float quantidade = (valor * (1 - taxa)) / cotacao; //qtd de criptos compradas

                    if (moeda == 1) {
                        printf("atualiza bit");
                        usuario.bit += quantidade;  //atualiza bit
                    } else if (moeda == 2) {
                        usuario.eth += quantidade;  //atualiza eth
                    } else if (moeda == 3) {
                        usuario.rip += quantidade;  //atualiza rip
                    }

                    printf("desconta saldo");
                    usuario.saldo -= valor; // Desconta o valor gasto

                    fseek(fp, 0, SEEK_SET);
                    fwrite(&usuario, sizeof(Usuario), 1, fp);
                    fclose(fp);

                    //limparTela();
                    printf("\nCompra realizada com sucesso! Voce comprou %.4f unidades da criptomoeda.\n", quantidade);
                    return 1;

                } else {

                    fclose(fp);
                    //limparTela();
                    printf("\nSaldo insuficiente para a compra!\n");
                    compra();
                    return 0;
                }
            }
            fclose(fp);
        }
    }
    printf("\nErro ao realizar a compra!\n");
    return 0;
}

//  ---------------------------------- ----------------------------------

int venda() {
    int moeda = 0;
    float quantidade;
    float cotacao;
    float taxa;
    Cotacao cotacaoAtual;

    FILE *fp = fopen("cotacao.bin", "rb");
    if (!fp) {
        printf("erro\n");
        return 0;
    }

    fread(&cotacaoAtual, sizeof(Cotacao), 1, fp); // Le cotacoes
    fclose(fp);

    printf("-----------------------\nVender Criptomoeda...\n-----------------------\n");
    printf("\nDigite a moeda desejada: \n\n");
    printf("1- Bitcoin\n");
    printf("2- Ethereum\n");
    printf("3- Ripple\n");
    printf("\nSua opcao: ");
    scanf("%d", &moeda);

    if (moeda == 1) {
        cotacao = cotacaoAtual.bit;
        taxa = 0.02;  //2% de taxa para bitcoin
    } else if (moeda == 2) {
        cotacao = cotacaoAtual.eth;
        taxa = 0.01;  //1% de taxa para ethereum
    } else if (moeda == 3) {
        cotacao = cotacaoAtual.rip;
        taxa = 0.01;  //1% de taxa para ripple
    } else {
        limparTela();
        printf("Digite 1, 2 ou 3\n"); // Valor digitado é inválido
        venda(); // Reinicia função
        return 0; // Não prossegue após chamada recursiva
    }

    limparTela();
    printf("\nCotacao: %.2f", cotacao);
    printf("\nDigite a quantidade de criptomoeda que deseja vender: ");
    scanf("%f", &quantidade);

    for (int i = 1; i <= limite; i++) { // Procura saldo
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

        if (file_exists(nome_arquivo)) {
            FILE *fp = fopen(nome_arquivo, "rb+");
            if (!fp) {
                continue;
            }

            Usuario usuario;
            fread(&usuario, sizeof(Usuario), 1, fp);

            if (strcmp(usuario.cpf, cpf_usuario_logado) == 0) {
                float valor_venda = quantidade * cotacao * (1 - taxa);  // Valor recebido pela venda

                if ((moeda == 1 && usuario.bit >= quantidade) || 
                    (moeda == 2 && usuario.eth >= quantidade) || 
                    (moeda == 3 && usuario.rip >= quantidade)) {

                    // Atualiza saldo da criptomoeda
                    if (moeda == 1) {
                        usuario.bit -= quantidade;  // Remove bitcoins
                    } else if (moeda == 2) {
                        usuario.eth -= quantidade;  // Remove ethereum
                    } else if (moeda == 3) {
                        usuario.rip -= quantidade;  // Remove ripple
                    }

                    usuario.saldo += valor_venda;  // Adiciona o valor da venda ao saldo em dinheiro

                    fseek(fp, 0, SEEK_SET);
                    fwrite(&usuario, sizeof(Usuario), 1, fp);
                    fclose(fp);

                    printf("\nVenda realizada com sucesso! Voce vendeu %.4f unidades da criptomoeda e recebeu %.2f em dinheiro.\n", quantidade, valor_venda);
                    return 1;
                } else {
                    fclose(fp);
                    printf("\nQuantidade insuficiente de criptomoedas para realizar a venda!\n");
                    return 0;
                }
            }
            fclose(fp);
        }
    }
    printf("\nErro ao realizar a venda!\n");
    return 0;
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