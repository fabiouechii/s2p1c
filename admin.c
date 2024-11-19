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
void cadastro_admin();
void excluir_admin();
void saldo_admin();
void extrato_admin();
void cadastrar_cripto();
void excluir_cripto();
void atualizar();
void limparTela() {     //pegar da main, nao fazer dnv !!!!!!
  #ifdef _WIN32     // Limpa a tela para Win
    system("cls");
  #else
    system("clear"); // Limpa a tela para Mac e Linux
  #endif
}


//  ----------------------------------------------------------------------------------------------------
// Estruturas
//typedef struct {}
    


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
void cadastro_admin() {
    printf("cadastrar investidor");
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


//  ----------------------------------------------------------------------------------------------------
int main() {
    limparTela();
    printf("-----------------------\nBem-vindo...\n-----------------------\n");
    printf("Tela de Administrador...\n-----------------------\n");

    login_admin();
    
    return 0;
}