#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define MAX_USUARIOS 100
#define MAX_CRIPTOS 50

int estadomenu = 0; //Estado do menu, ativo ou não


//  ----------------------------------------------------------------------------------------------------
//Declara funções no início
int menu_admin();
int login_admin();
int cadastro_admin();
void excluir_admin();
void saldo_admin();
void extrato_admin();
void cadastrar_cripto();
void excluir_cripto();
void atualizar();
int contar_cadastros();
int file_exists(const char *filename);
int novo_usuario(const char *cpf_formatado);
int contar_cadastros();
void limparTela() {     //pegar da main, nao fazer dnv !!!!!!
  #ifdef _WIN32     // Limpa a tela para Win
    system("cls");
  #else
    system("clear"); // Limpa a tela para Mac e Linux
  #endif
}


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
    
int limite = 10;


//  ----------------------------------------------------------------------------------------------------
// Função de login do administrador
int login_admin() {
    char cpf[12], senha[20];

    printf("\n\nDigite o seu CPF: "); //Pede CPF
    scanf("%s", cpf);

    if (strlen(cpf) != 11) { //Verifica se CPF tem 11 digitos
    limparTela();
    printf("O CPF deve conter 11 digitos!\n");
    login_admin();
    }
    printf("Senha: ");
    scanf("%s", senha);

    if (strcmp(cpf, "12345678901") == 0 && strcmp(senha, "admin123") == 0) { //valor de cpf e senha cadastrado, exemplo 12345678901 e senha admin123
        limparTela();
        printf("Login bem-sucedido!\n");
        estadomenu=1;
        menu_admin();
    }

    limparTela();
    printf("CPF ou senha incorretos.\n");
    login_admin();
}


//  ----------------------------------------------------------------------------------------------------
// Menu principal
int menu_admin() {
    int opcao;
    printf("-----------------------\nMenu...\n-----------------------\n");  //Print das opções de MENU, só aparecem na primeira vez (pós login)

    printf("1. Cadastrar novo investidor\n");
    printf("2. Excluir investidor\n");
    printf("3. Consultar saldo de investidor\n");
    printf("4. Consultar extrato de investidor\n");
    printf("5. Cadastrar nova criptomoeda\n");
    printf("6. Excluir criptomoeda\n");
    printf("7. Atualizar cotações de criptomoedas\n");
    printf("8. Sair\n");
    
    while (estadomenu ==1) {  // Loop para escolha das opções, até que realiza saida (opcão = 8)

        printf("\nDigite a opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                limparTela();
                cadastro_admin();
                break;
            case 2:
                limparTela();
                excluir_admin();
                break;
            case 3:
                limparTela();
                saldo_admin();
                break;
            case 4:
                limparTela();
                extrato_admin();;
                break;
            case 5:
                limparTela();
                cadastrar_cripto();
                break;
            case 6:
                limparTela();
                excluir_cripto();
                break;
            case 7:
                limparTela();
                atualizar();
                break;
            case 8:
                limparTela();
                estadomenu=0;
                printf("Saindo do menu...\n");
                login_admin();  //  Sai do loop de escolha de opção, e volta ao início
            default:
                printf("Opcao invalida!\n");
                menu_admin();
        }

    }

    return 0;
}


//  ----------------------------------------------------------------------------------------------------
int cadastro_admin() {
    //printf("cadastrar investidor");
    printf("-----------------------\nArea de cadastro...\n-----------------------\n");

    Usuario usuario;

    int quantidade_usuarios = contar_cadastros();
    if (quantidade_usuarios >= limite) {  //Teste limite de cadastros
        printf("\nLimite de usuarios atingido!\n");
        menu_admin();
    }

    printf("\nDigite o CPF (somente numeros): "); //Pede CPF
    scanf("%s", usuario.cpf);

    if (strlen(usuario.cpf) != 11) { //Verifica se CPF tem 11 digitos
        limparTela();
        printf("O CPF deve conter 11 digitos!\n");
        cadastro_admin();
    }

    if (novo_usuario(usuario.cpf)) {  // Verifica se CPF já cadastrado
        limparTela();
        printf("\nUsuario ja cadastrado!\n\n");
        menu_admin();
    }

    char novo_arquivo[30];
    snprintf(novo_arquivo, sizeof(novo_arquivo), "Usuario%d.bin", quantidade_usuarios + 1);
    FILE *fp = fopen(novo_arquivo, "wb");
    if (!fp) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    printf("\nDigite a senha: "); //Pede senha
    scanf("%s", usuario.senha);

    usuario.saldo = 0; //Valores iniciais de novo usuário
    usuario.bit = 0;
    usuario.eth = 0;
    usuario.rip = 0;

    fwrite(&usuario, sizeof(Usuario), 1, fp);
    fclose(fp);

    limparTela();
    menu_admin();
    return 1;
}

void excluir_admin() {
    printf("excluir investidor");
}

void saldo_admin() {
    printf("consultar saldo");
}

void extrato_admin() {
    printf("consultar extrato");
}

void cadastrar_cripto() {
    printf("cadastrar cripto");
}

void excluir_cripto() {
    printf("excluir cripto");
}

void atualizar() {
    printf("atualizar cotacao");
}

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
    limparTela();
    printf("-----------------------\nBem-vindo...\n-----------------------\n");
    printf("Tela de Administrador...\n-----------------------\n");

    login_admin();
    
    return 0;
}