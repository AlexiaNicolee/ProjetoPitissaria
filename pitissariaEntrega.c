/*
    Alunos: Rafael Santos, Alexia Nicole, Eduarda;
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <unistd.h>

#define MAX_CLIENTES 100
#define ENDEC "C:\\Users\\Public\\clientes"
#define AUX "C:\\Users\\Public\\aux"
#define PEDIDOS "C:\\Users\\Public\\pedidos"
#define SENHA 123

//Funções
void inicializa();
void limpar_arquivo();


//  variáveis globais
FILE *clientes_file;
FILE *pedidos_file;
FILE *aux_file;
int id;
int od;

typedef struct {
    char nome[50];
    char endereco[100];
    long int telefone;
    int cliente_id;
} Cliente;

Cliente cliente;
Cliente *vet_cliente;

typedef struct {
    char sabor[30];
    int quantidade;
    float preco;
    char forma_pagamento;
    char delivery;
    int ordem_pedido;
    char nome[50];
} Pedido;

Pedido pedido;
Pedido vet_pedido[];

void cadastrar_cliente() {
    system("cls");
    fflush(stdin);

    printf("Digite o nome do cliente: ");
    gets(cliente.nome);
    fflush(stdin);

    printf("Digite o endereço do cliente: ");
    gets(cliente.endereco);
    fflush(stdin);

    printf("Digite o telefone do cliente: ");
    scanf("%d", &cliente.telefone);
    fflush(stdin);

    vet_cliente = realloc(vet_cliente, id * sizeof(Cliente));

    // Abrir arquivo para escrita
    clientes_file = fopen(ENDEC, "ab+");

    if (clientes_file == NULL){
        printf("Erro ao abrir o arquivo de clientes.\n");
        getch();
        return 1;
    }else{
    // Escrever os dados do cliente no arquivo
        id++;
        cliente.cliente_id = id;
        fwrite(&cliente, sizeof(Cliente), 1, clientes_file);
        fclose(clientes_file);
        printf("Cliente cadastrado com sucesso!\n");
        Sleep(400);
    }
    system("cls");
}

void listar_clientes(){
    system("cls");
    clientes_file = fopen(ENDEC, "rb");
    //fseek(clientes_file, sizeof(Cliente), SEEK_SET);
    while(!feof(clientes_file)){
        int eof_control = fread(&cliente, sizeof(Cliente), 1, clientes_file);
        if(eof_control != 0){
            printf("Id do cliente: %d\n"
           "Numero de Telefone: %d\n"
           "Nome do Cliente: %s\n"
           "Endereço: %s\n\n"
           , cliente.cliente_id, cliente.telefone, cliente.nome, cliente.endereco);
        }
    }
    system("pause");
    system("cls");
}

void copiar_arquivo(FILE *livre, char endec1, FILE *alvo, char endec2){
    livre = fopen(endec1, "wb");
    alvo = fopen(endec2, "rb");
    if(livre == NULL || alvo == NULL){
        printf("Erro ao abrir um dos arquivos.\n");
    }else{
        while(!feof(alvo)){
            int eof_alvo = fread(&cliente, sizeof(Cliente), 1, alvo);
            if(eof_alvo != 0){
                fwrite(&cliente, sizeof(Cliente), 1, livre);
            }
        }
    }
}

void alterar_dado_cliente(){
    system("cls");
    fflush(stdin);
    int id_aux;
    int opcao;
    int novo_tel;
    char alteracao[100];

    aux_file = fopen(AUX, "wb");

    listar_clientes();
    printf("\n\n\n");
    printf("Selecione um Cliente para alterar.\n");
    printf("Digite o ID: ");
    scanf("%d", &id_aux);
    fflush(stdin);
    clientes_file = fopen(ENDEC, "ab+");
    while(!feof(clientes_file)){
        int eof_cliente = fread(&cliente, sizeof(Cliente), 1, clientes_file);
        if(eof_cliente != 0){
            if(id_aux != cliente.cliente_id){
                fwrite(&cliente, sizeof(Cliente), 1, aux_file);
            }else{
                printf("Qual operação deseja realizar: \n");
                printf(
                       "1 - Alterar nome.\n"
                       "2 - Alterar Endereco.\n"
                       "3 - Alterar número de Telefone.\n"
                       "4 - Abandonar alteração\n."
                       );
                printf("Resposta: ");
                scanf("%d", &opcao);
                fflush(stdin);
                pedidos_file = fopen(PEDIDOS, "rb+");
                if(opcao < 1 || opcao > 4){
                    printf("Operação inválida.\n");
                    system("pause");
                    system("cls");
                    return;
                }else{
                    if(opcao == 1){
                        printf("Digite o novo nome: ");
                        gets(alteracao);
                        strcpy(cliente.nome, alteracao);
                        rewind(clientes_file);
                        fseek(clientes_file, id_aux*sizeof(Cliente), SEEK_CUR);
                        fwrite(&cliente.nome, sizeof(Cliente), 1, aux_file);
                        printf("cliente nome: %s\nCliente id: %d\n", cliente.nome, cliente.cliente_id);
                        system("pause");
                        system("cls");
                        continue;
                    }
                }
            }
        }
    }
    fclose(aux_file);
    fclose(clientes_file);
    fclose(pedidos_file);
    int remocao = remove(clientes_file);
    if(remocao == 0){
        printf("Removido com sucesso.\n");
    }else{
        printf("Erro ao excluir.\n");
    }
    system("pause");
    system("cls");
}

void fazer_pedido() {
    system("cls");
    fflush(stdin);
    Pedido pedido;
    int opcao;
    float soma_total = 0;
    char resposta;
    int id_busca;

    printf("Cliente possui cadastro na Pitissaria?[s|n]: \n");
    scanf("%c", &resposta);
    fflush(stdin);
    if(resposta == 'n'){
        cadastrar_cliente();
    }else{
        if(clientes_file = fopen(ENDEC, "rb") == NULL){
            printf("Não foi identificado nenhum cliente cadastrado, por favor, cadastre um cliente para realizar o pedido.\n");
            fflush(stdin);
            printf("Cadastrar agora[s|n]: ");
            scanf("%d", &resposta);
            fflush(stdin);
            if(resposta != 'n' || resposta != 's'){
                printf("entrada inválida.");
                Sleep(300);
                system("cls");
                return;
            }else{
                if(resposta == 'n'){
                    return;
                }else{
                    if(resposta == 's'){
                        cadastrar_cliente();
                        return;
                    }
                }
            }
        }
        printf("\n===== FAZER PEDIDO DE PIZZA =====\n");
        printf("Escolha o sabor da pizza:\n");
        printf("1. Mussarela (R$30)\n");
        printf("2. Calabresa (R$35)\n");
        printf("3. Frango com Catupiry (R$40)\n");
        printf("4. Portuguesa (R$35)\n");
        printf("5. Marguerita (R$30)\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        fflush(stdin);

        switch (opcao) {
            case 1:
                strcpy(pedido.sabor, "Mussarela");
                pedido.preco = 30.0;
                break;
            case 2:
                strcpy(pedido.sabor, "Calabresa");
                pedido.preco = 35.0;
                break;
            case 3:
                strcpy(pedido.sabor, "Frango com Catupiry");
                pedido.preco = 40.0;
                break;
            case 4:
                strcpy(pedido.sabor, "Portuguesa");
                pedido.preco = 35.0;
                break;
            case 5:
                strcpy(pedido.sabor, "Marguerita");
                pedido.preco = 30.0;
                break;
            default:
                printf("Opção inválida!\n");
                return;
        }

        printf("Digite a quantidade desejada: ");
        scanf("%d", &pedido.quantidade);
        fflush(stdin);

        soma_total = pedido.preco * pedido.quantidade;
        printf("Valor total da compra: R$%.2f\n", soma_total);

        printf("Digite a forma de pagamento: [c(cartão)|d(dinheiro)|p(pix)\n");
        printf("resposta: ");
        scanf("%c", &pedido.forma_pagamento);
        fflush(stdin);

        printf("Delivery?[s|n]: ");
        scanf("%c", &pedido.delivery);
        fflush(stdin);

        printf("\n\nPara quem é esta Pizza?\n\n");
        listar_clientes();
        fflush(stdin);

        printf("\nDigite o id do cliente: ");
        scanf("%d", &id_busca);
        fflush(stdin);

        clientes_file = fopen(ENDEC, "rb");
        rewind(clientes_file);

        while(!feof(clientes_file)){
            int eof_cliente = fread(&cliente, sizeof(Cliente), 1, clientes_file);
            if(eof_cliente != 0){
                if(id_busca == cliente.cliente_id){
                    strcpy(pedido.nome, cliente.nome);
                    fclose(clientes_file);
                    pedidos_file = fopen(PEDIDOS, "ab+");
                    if (pedidos_file == NULL) {
                        printf("Erro ao abrir o arquivo de pedidos.\n");
                        exit(1);
                    }else{
                        od++;
                        pedido.ordem_pedido = od;
                        printf("\n od atual: %d\n", od);
                        fwrite(&pedido, sizeof(Pedido),1, pedidos_file);
                        Sleep(400);
                        fclose(pedidos_file);
                    }
                    printf("Pedido realizado com sucesso!\n");
                    break;
                }
            }else{
                printf("Id não encontrado.\n");
                system("pause");
            }
        }
    }
    system("cls");
}
void listar_pedidos(){
    system("cls");
    fflush(stdin);
    pedidos_file = fopen(PEDIDOS, "rb");
    while(!feof(pedidos_file)){
        int eof_pedido = fread(&pedido, sizeof(Pedido), 1, pedidos_file);
        if(eof_pedido != 0){
            printf("\n\nNúmero do pedido: %d\n"
                   "Sabor: %s\n"
                   "Quantidade: %d\n"
                   "Cliente: %s\n\n"
                   , pedido.ordem_pedido, pedido.sabor, pedido.quantidade, pedido.nome);
        }
    }
    system("pause");
    system("cls");
}

void limpar_arquivo(){
    char certeza;
    int senha;
    system("cls");
    fflush(stdin);

    printf("Deseja realmente excluir o arquivo?(a ação não pode ser desfeita)\n");
    printf("Resposta[s|n]: ");
    scanf("%c", &certeza);
    printf("Digite a senha: ");
    scanf("%d",&senha);
    if(certeza == 's' && senha == SENHA){
        clientes_file = fopen(ENDEC, "wb");
        pedidos_file = fopen(PEDIDOS, "wb");
        if(clientes_file == NULL || pedidos_file == NULL){
            printf("Erro ao apagar o arquivo.\n");
            system("pause");
        }else{
            fclose(clientes_file);
            fclose(pedidos_file);
        }
    }else{
        if(certeza == 'n'){
            system("cls");
            fflush(stdin);
            return ;
        }
        if(certeza != "s" || certeza != "n"){
            limpar_arquivo();
        }
    }
    system("pause");
    fflush(stdin);
    system("cls");
}

void inicializa(){

    if(!access(ENDEC, F_OK)){
        printf("Sistemas ok!\n");
        system("pause");
        system("cls");

        clientes_file = fopen(ENDEC, "rb");
        fseek(clientes_file, -(sizeof(Cliente)), SEEK_END);
        fread(&cliente, sizeof(Cliente), 1, clientes_file);
        id = cliente.cliente_id;
        rewind(clientes_file);
        fclose(clientes_file);

        pedidos_file = fopen(PEDIDOS, "rb");
        fseek(pedidos_file, -(sizeof(Pedido)), SEEK_END);
        fread(&pedido, sizeof(Pedido), 1, pedidos_file);
        od = pedido.ordem_pedido;
        rewind(pedidos_file);
        fclose(pedidos_file);

    }else{
        printf("ERRO: Arquivo(s) não encontrado. Criando arquivo(s)...\n");
        Sleep(200);
        clientes_file = fopen(ENDEC, "wb+");

        if(clientes_file == NULL){
            printf("Erro ao criar arquivo, por favor, corrija!\n");
            system("pause");
        }else{
            id = 0;
            od = 0;
            printf("Arquivo criado com sucesso.\n");
            fclose(clientes_file);
        }
    }
}

int main() {
    fflush(stdin);
    system("cls");
    setlocale(LC_ALL, "Portuguese");

    inicializa();
    int opcao;
    do {
        opcao = NULL;
        printf("\n===== MENU =====\n");
        printf("1. Cadastrar cliente\n");
        printf("2. Fazer pedido de pizza\n");
        printf("3. Listar Clientes\n");
        printf("4. Listar Pedidos.\n");
        printf("5. Alterar Dados\n");
        printf("6. Apagar Arquivos)\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrar_cliente();
                break;
            case 2:
                fazer_pedido();
                break;
            case 3:
                listar_clientes();
                break;
            case 4:
                listar_pedidos();
                break;
            case 5:
                alterar_dado_cliente();
                break;
            case 6:
                limpar_arquivo();
                break;
            case 0:
                opcao = 0;
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Por favor, escolha uma opção válida.\n");
                break;
        }
    } while (opcao != 0);
    free(vet_cliente);
    return 0;
}
