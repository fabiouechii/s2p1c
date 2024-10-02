#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define MAX_TRANSACOES 100


//  ----------------------------------------------------------------------------------------------------
//Declara funções no início
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
void gerenciaExtrato(const char* tipo, float valor);


//  ----------------------------------------------------------------------------------------------------
// Estruturas
typedef struct {
    char cpf[12];
    char senha[10];
    int saldo;
    float bit;
    float eth;
    float rip;
} Usuario;


typedef struct {
    float bit;  // cot bit
    float eth;  //cot eth
    float rip;  //cot rip
} Cotacao;


typedef struct {
    char tipo[20];
    float valor;
    time_t data;
} Transacao;


//  ----------------------------------------------------------------------------------------------------
void atualizar() { //Atualizar/criar cotações
    FILE *fp;
    Cotacao cotacao;
    const char *nome_arquivo = "cotacao.bin";

    fp = fopen(nome_arquivo, "rb"); // Verifica se existe arquivo cotacao
    if (!fp) {
        fp = fopen(nome_arquivo, "wb"); // Se não existir, cria
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

    fread(&cotacao, sizeof(Cotacao), 1, fp); //Se ja existir, lê, atualiza e salva
    fclose(fp);

    srand(time(NULL));  //gera valor randon

    float variacao_bit = ((rand() % 11) - 5) / 100.0; // Variação entre -5% e 5% BITCOIN
    cotacao.bit += cotacao.bit * variacao_bit;

    float variacao_eth = ((rand() % 11) - 5) / 100.0; // Variação entre -5% e 5% ETHEREUM
    cotacao.eth += cotacao.eth * variacao_eth;

    float variacao_rip = ((rand() % 11) - 5) / 100.0; // Variação entre -5% e 5% RIPPLE
    cotacao.rip += cotacao.rip * variacao_rip;


    printf("-----------------------\nCotaoees atualizadas!\n-----------------------\n"); // Mostra novas cotações

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


//  ----------------------------------------------------------------------------------------------------
// Variaveis globais usadas
int limite = 10; //Define limite de 10, para cadastros realizados
int estadomenu = 0; //Estado do menu, ativo ou não
char cpf_usuario_logado[12]; //CPF salvo após login, para consulta e validação em outras funções


//  ----------------------------------------------------------------------------------------------------
void limparTela() {
  #ifdef _WIN32     // Limpa a tela para Win
    system("cls");
  #else
    system("clear"); // Limpa a tela para Mac e Linux
  #endif
}


//  ----------------------------------------------------------------------------------------------------
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

  if (entrada == 1) { //Leva para função cadastro
      limparTela();
      cadastro();

  } else if (entrada == 2) { // Leva para função loginn
      limparTela();
      login();
  } else {
      printf("\n-----------------------\nOpcao invalida"); //Mantem
      inicio();
  }
}


//  ----------------------------------------------------------------------------------------------------
int cadastro() {

    printf("-----------------------\nArea de cadastro...\n-----------------------\n");

    Usuario usuario;

    int quantidade_usuarios = contar_cadastros();
    if (quantidade_usuarios >= limite) {  //Teste limite de cadastros
        printf("\nLimite de usuarios atingido!\n");
        inicio();
    }

    printf("\nDigite o seu CPF (somente numeros): "); //Pede CPF
    scanf("%s", usuario.cpf);

    if (strlen(usuario.cpf) != 11) { //Verifica se CPF tem 11 digitos
        limparTela();
        printf("O CPF deve conter 11 digitos!\n");
        cadastro();
    }

    if (novo_usuario(usuario.cpf)) {  // Verifica se CPF já cadastrado
        limparTela();
        printf("\nUsuario ja cadastrado!\n\n");
        inicio();
    }

    char novo_arquivo[30];
    snprintf(novo_arquivo, sizeof(novo_arquivo), "Usuario%d.bin", quantidade_usuarios + 1);
    FILE *fp = fopen(novo_arquivo, "wb");
    if (!fp) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    printf("\nDigite a sua senha: "); //Pede senha
    scanf("%s", usuario.senha);

    usuario.saldo = 0; //Valores iniciais de novo usuário
    usuario.bit = 0;
    usuario.eth = 0;
    usuario.rip = 0;

    fwrite(&usuario, sizeof(Usuario), 1, fp);
    fclose(fp);

    limparTela();
    inicio();
    return 1;
}


//  ----------------------------------------------------------------------------------------------------
int login() {

  
  char cpf[12];
  char senha[10];

  printf("-----------------------\nTela de login...\n-----------------------\n");

  printf("\n\nDigite o seu CPF: "); //Pede CPF
  scanf("%s", cpf);

  if (strlen(cpf) != 11) { //Verifica se CPF tem 11 digitos
    limparTela();
    printf("O CPF deve conter 11 digitos!\n");
    login();
  }

  printf("\nDigite a sua senha: "); //Pede senha
  scanf("%s", senha);

   if (conferir_usuario(cpf, senha)) { // Verifica se usuario já cadastrado
        strcpy(cpf_usuario_logado, cpf); // Salva CPF de login
        limparTela();
        estadomenu=1;
        menu();   //Se ok, chama função MENU
    } else {
        limparTela();
        printf("\nCPF ou senha incorretos!\n\n");
        inicio(); //Se !ok, chama função INICIO novamente
    }
  
    return 0;
}


//  ----------------------------------------------------------------------------------------------------
int menu() {
    int opcao;
    printf("-----------------------\nMenu...\n-----------------------\n");  //Print das opções de MENU, só aparecem na primeira vez (pós login)

    printf("1. Consultar saldo\n");
    printf("2. Consultar extrato\n");
    printf("3. Depositar\n");
    printf("4. Sacar\n");
    printf("5. Comprar Criptomoeda\n");
    printf("6. Vender Criptomoeda\n");
    printf("7. Atualizar cotacao\n");
    printf("8. Sair\n");
    
    while (estadomenu ==1) {  // Loop para escolha das opções, até que realiza saida (opcão = 8)

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
                inicio();  //  Sai do loop de escolha de opção, e volta ao início
            default:
                printf("Opcao invalida!\n");
                menu();
        }

    }

    return 0;
}


//  ----------------------------------------------------------------------------------------------------
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


//  ----------------------------------------------------------------------------------------------------
int deposito() {
    printf("-----------------------\nDeposito...\n-----------------------\n");
    int valor_deposito;
    char senha[10];  // Variável para a senha

    // Solicita a senha novamente para conferir
    printf("\nDigite sua senha para confirmar o deposito: ");
    scanf("%s", senha);

    // Verifica se a senha está correta
    if (conferir_usuario(cpf_usuario_logado, senha)) {  // Conferindo o CPF logado e a senha inserida
        limparTela();
        printf("-----------------------\n");
        printf("Senha correta!\n");
        printf("-----------------------\n");
        printf("\nDigite o valor que deseja depositar: ");
        scanf("%d", &valor_deposito);
        printf("\n-----------------------\n");
        limparTela();
        
        // Procura o arquivo do usuário logado
        for (int i = 1; i <= limite; i++) {
            char nome_arquivo[30];
            snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

            if (file_exists(nome_arquivo)) {
                FILE *fp = fopen(nome_arquivo, "rb+");
                if (!fp) {
                    continue;
                }

                Usuario usuario;
                fread(&usuario, sizeof(Usuario), 1, fp);

                // Se encontrar o usuário correspondente ao CPF logado
                if (strcmp(usuario.cpf, cpf_usuario_logado) == 0) {
                    usuario.saldo += valor_deposito;
                    fseek(fp, 0, SEEK_SET); // Reposiciona o ponteiro do arquivo para o início
                    fwrite(&usuario, sizeof(Usuario), 1, fp);
                    fclose(fp);
                    printf("-----------------------\nDeposito realizado com sucesso!\n-----------------------\n");
                    printf("\nSaldo atual: %d\n", usuario.saldo);
                    // precisa adicionar isso aqui para ele adicionar no extrato
                    gerenciaExtrato("Deposito", valor_deposito); 
                    // mas tem que mudar o nome "Deposito" e o "valor_deposito"
                    return 1;
                }
                fclose(fp);
                
            }
        }
        printf("\nErro ao realizar o deposito!\n");
    } else {
        limparTela();
        printf("\nSenha incorreta! Tente novamente.\n\n");
        deposito();  // Se a senha estiver incorreta, tenta novamente
    }
    return 0;
}


//  ----------------------------------------------------------------------------------------------------
int saque() {
    printf("-----------------------\nSaque...\n-----------------------\n");
    int valor_saque;
    char senha[10];  // variável para a senha

    // Solicita a senha novamente para conferir
    printf("Digite sua senha para confirmar o deposito: ");
    scanf("%s", senha);

    // Verifica se a senha está correta
    if (conferir_usuario(cpf_usuario_logado, senha)) {  // conferindo o CPF logado e a senha inserida
        limparTela();
        printf("-----------------------\n");
        printf("Senha correta!\n");
        printf("-----------------------\n");
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", 1); // Pego o arquivo do usuário
        FILE *fp = fopen(nome_arquivo, "rb+");
        if (!fp) {
            printf("Erro ao abrir o arquivo.\n");
            return 0;
        }
    
        Usuario usuario;
        while (fread(&usuario, sizeof(Usuario), 1, fp)) {
            if (strcmp(usuario.cpf, cpf_usuario_logado) == 0) {
                float valor_saque;
                printf("Digite o valor do saque: ");
                scanf("%f", &valor_saque);
                if (valor_saque <= 0) {
                    printf("Valor inválido!\n");
                    fclose(fp);
                    return 0;
                }
                if (usuario.saldo < valor_saque) {
                    printf("Saldo insuficiente!\n");
                    fclose(fp);
                    return 0;
                }
                usuario.saldo -= valor_saque; // Deduz o valor do saldo
                fseek(fp, -sizeof(Usuario), SEEK_CUR); // Volta para a posição do usuário
                fwrite(&usuario, sizeof(Usuario), 1, fp); // Atualiza dados no arquivo
                printf("Saque de R$ %.2f realizado com sucesso!\n", valor_saque);
                fclose(fp);
    
                // Adicione aqui a chamada para gerenciarExtrato
                gerenciaExtrato("Saque", valor_saque);
    
                return 1; // Finaliza a função com sucesso
            }
        }
        fclose(fp);
    }else {
        limparTela();
        printf("\nSenha incorreta! Tente novamente.\n\n");
        saque();  // Se a senha estiver incorreta, tenta novamente
    }
    return 0;
}


//  ----------------------------------------------------------------------------------------------------
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

    fread(&cotacaoAtual, sizeof(Cotacao), 1, fp); //Leitura das cotações
    fclose(fp);

    //debug
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
        printf("Digite 1, 2 ou 3\n"); // Valor digitado é inválido
        compra(); // Reinicia função
        return 0;
    }

    limparTela();
    printf("-----------------------\nCotacao: %.2f\n-----------------------\n", cotacao);
    printf("\nDigite o valor em dinheiro que deseja usar para comprar a criptomoeda: ");
    scanf("%d", &valor);
    limparTela();


    for (int i = 1; i <= limite; i++) { //Procura saldo
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

                    float quantidade = (valor * (1 - taxa)) / cotacao; //Quantidade de criptomoedas compradas

                    if (moeda == 1) {
                        //printf("atualiza bit");
                        usuario.bit += quantidade;  //Atualiza saldo BIT
                    } else if (moeda == 2) {
                        usuario.eth += quantidade;  //Atualiza saldo ETH
                    } else if (moeda == 3) {
                        usuario.rip += quantidade;  //Atualiza saldo RIP
                    }

                    //printf("desconta saldo");
                    usuario.saldo -= valor; // Desconta o valor gasto

                    fseek(fp, 0, SEEK_SET);
                    fwrite(&usuario, sizeof(Usuario), 1, fp);
                    fclose(fp);

                    //limparTela();
                    gerenciaExtrato("Compra", valor);
                    printf("\nCompra realizada com sucesso!\n\nCriptomoedas compradas: %.4f\nCotacao: %.2f\n-----------------------\n", quantidade, cotacao);
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

//  ----------------------------------------------------------------------------------------------------
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
        taxa = 0.03;  //3% bit
    } else if (moeda == 2) {
        cotacao = cotacaoAtual.eth;
        taxa = 0.02;  //2% eth
    } else if (moeda == 3) {
        cotacao = cotacaoAtual.rip;
        taxa = 0.01;  //1% rip
    } else {
        limparTela();
        printf("Digite 1, 2 ou 3\n"); // Valor digitado é inválido
        venda(); // Reinicia função
        return 0;
    }

    limparTela();
    printf("-----------------------\nCotacao: %.2f\n-----------------------\n", cotacao);
    printf("\nDigite a quantidade de criptomoeda que deseja vender: ");
    scanf("%f", &quantidade);
    limparTela();

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

                    gerenciaExtrato("Venda", valor_venda);
                    printf("\nVenda realizada com sucesso!\n\nCriptomoedas vendidas %.4f\nCotacao:  %.2f\nRecebeu em dinheiro: %.2f\n-----------------------\n", quantidade, cotacao, valor_venda);
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


//  ----------------------------------------------------------------------------------------------------
void gerenciaExtrato(const char* tipo, float valor) {
    // Abre o arquivo do usuário logado
    for (int i = 1; i <= limite; i++) {
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

        if (file_exists(nome_arquivo)) {
            FILE *fp = fopen(nome_arquivo, "rb+"); // Abre o arquivo em modo de leitura e escrita
            if (!fp) {
                printf("Erro ao abrir o arquivo do usuario.\n");
                continue;
            }

            Usuario usuario;
            fread(&usuario, sizeof(Usuario), 1, fp);

            // Se o CPF do usuário corresponde ao CPF logado
            if (strcmp(usuario.cpf, cpf_usuario_logado) == 0) {
                Transacao transacao;
                strcpy(transacao.tipo, tipo);
                transacao.valor = valor;
                transacao.data = time(NULL);

                // Move o ponteiro para o final do arquivo e adiciona a transação
                fseek(fp, 0, SEEK_END);
                fwrite(&transacao, sizeof(Transacao), 1, fp);
                fclose(fp);
                //printf("Transacao adicionada ao extrato!\n");
                return;
            }

            fclose(fp);
        }
    }
    printf("Erro ao adicionar transacao no arquivo\n");
}


//  ----------------------------------------------------------------------------------------------------
int extrato() {
    // Abre o arquivo do usuário logado
    for (int i = 1; i <= limite; i++) {
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "Usuario%d.bin", i);

        if (file_exists(nome_arquivo)) {
            FILE *fp = fopen(nome_arquivo, "rb");
            if (!fp) {
                printf("Erro ao abrir o arquivo do usuário.\n");
                continue;
            }

            Usuario usuario;
            fread(&usuario, sizeof(Usuario), 1, fp);

            // Se o CPF do usuário corresponde ao CPF logado
            if (strcmp(usuario.cpf, cpf_usuario_logado) == 0) {
                Transacao transacao;

                printf("-----------------------\nExtrato...\n-----------------------\n");

                // Move o ponteiro após os dados do usuário e comece a ler as transações
                while (fread(&transacao, sizeof(Transacao), 1, fp)) {
                    printf("Tipo: %s, Valor: R$ %.2f, Data: %s", 
                            transacao.tipo, transacao.valor, ctime(&transacao.data));
                }

                fclose(fp);
                return 1; // Extrato exibido com sucesso
            }

            fclose(fp);
        }
    }
    printf("Erro ao consultar extrato!\n");
    return 0;
}


//  ----------------------------------------------------------------------------------------------------
int novo_usuario(const char *cpf) {  // Verifica se usuario já existe ou não
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
                    //printf("CPF encontrado\n");
                    return 1;
            }
        }
    }
    //printf("CPF nao encontrado\n");
    return 0;
}


//  ----------------------------------------------------------------------------------------------------
int conferir_usuario(const char *cpf, const char *senha) {  // Verifica se usuario no login está OK
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
                return 1;  //CPF e senha encontrados
              }
        }
    }
    return 0; //CPF e senha não encontrados
}


//  ----------------------------------------------------------------------------------------------------
int file_exists(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file) {
    fclose(file);
    return 1;
  }
  return 0;
}


//  ----------------------------------------------------------------------------------------------------
int contar_cadastros() {  // Verifica se atingiu limite de cadastros
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


//  ----------------------------------------------------------------------------------------------------
int main() {
  limparTela();   //Limpa tela e começa
  inicio();

  return 0;
}