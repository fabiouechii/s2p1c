#ifndef FUNCOESMAIN_H
#define FUNCOESMAIN_H

#define MAX_TRANSACOES 100
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
void atualizar();
int estadomenu;
void inicio();
int limite;

#endif