-obetivo do projeto

atualizar nosso banco digital, criando o "programa" do administrador, tendo funcoes como, criar e excluir investidor, verificar extrato e saldo de investidor, alem da exclusao e criacao de novas criptomoedas.

-a forma de compilacao e execucao

compliar e executar arquivo main.c (programa do usuario/investidor)

gcc -c FUNCOESMAIN.c

gcc FUNCOESMAIN.o main.c -o a.exe

./a.exe


compliar e executar arquivo admin.c(programa do administrador)

gcc -c FUNCOESMAIN.c

gcc FUNCOESMAIN.o admin.c -o b.exe

./b.exe


-uma breve explicacao sobre a forma de uso do programa (administrador)

iniciando o programa e aberto uma tela de login do administrador (cpf: 12345678901 e senha: admin123).

após a validacao do login o administrador acessa a tela de menu com as opcoes de 1 a 8.

a funcao cadastrar usuario funciona de forma semelhante a primeira etapa (nao foi implementado a inscricao de um nome ao investidor), o admin inseri os valores de cpf e senha para novo usuario, é feita uma verificacao para evitar que seja feito um cadastro duplicado

a funcao excluir usuario o adminstrador insere um cpf, e entao recebe as informacoes daquele usuario antes de poder confirmar a exclusao

a funcao atualizar executa a mesma funcao utilizada no projeto 1

as outras funcoes nao foram finalizadas a tempo

para sair e necessario escolher a opcao 8

-lista com os nomes e matricula dos participantes do grupo (o projeto pode ser feitos em duplas ou trios)

Fabio Coelho Uechi Martins         - 52.124.011-9

Victor Augusto Montanari Meneguin  - 52.124.004-4
