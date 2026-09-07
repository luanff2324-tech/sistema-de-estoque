

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_ESTOQUE "estoque.txt"
#define MAX_NOME 60

typedef struct {
    int codigo;
    char nome[MAX_NOME];
    int quantidade;
    float preco;
} Produto;

/* Limpa o que ficou no teclado */
void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Lê um número inteiro */
int lerInteiro(char mensagem[]) {
    int valor;

    while (1) {
        printf("%s", mensagem);

        if (scanf("%d", &valor) == 1) {
            limparEntrada();
            return valor;
        }

        printf("Valor invalido. Tente novamente.\n");
        limparEntrada();
    }
}

/* Lê um número decimal */
float lerFloat(char mensagem[]) {
    float valor;

    while (1) {
        printf("%s", mensagem);

        if (scanf("%f", &valor) == 1) {
            limparEntrada();
            return valor;
        }

        printf("Valor invalido. Tente novamente.\n");
        limparEntrada();
    }
}

/* Lê texto */
void lerTexto(char mensagem[], char destino[], int tamanho) {
    printf("%s", mensagem);

    fgets(destino, tamanho, stdin);

    destino[strcspn(destino, "\n")] = '\0';
}

/* Cadastra um produto */
void cadastrarProduto() {
    Produto produto;
    Produto existente;

    FILE *arquivo;

    produto.codigo = lerInteiro("Codigo do produto: ");

    arquivo = fopen(ARQUIVO_ESTOQUE, "r");

    /* Verifica se o código já existe */
    if (arquivo != NULL) {

        while (fscanf(arquivo, "%d;%59[^;];%d;%f\n",
                      &existente.codigo,
                      existente.nome,
                      &existente.quantidade,
                      &existente.preco) == 4) {

            if (existente.codigo == produto.codigo) {
                printf("Ja existe um produto com esse codigo.\n");
                fclose(arquivo);
                return;
            }
        }

        fclose(arquivo);
    }

    lerTexto("Nome: ", produto.nome, MAX_NOME);

    produto.quantidade = lerInteiro("Quantidade inicial: ");

    produto.preco = lerFloat("Preco unitario: R$ ");

    if (produto.quantidade < 0 || produto.preco < 0) {
        printf("Quantidade e preco nao podem ser negativos.\n");
        return;
    }

    /* Abre o arquivo para adicionar */
    arquivo = fopen(ARQUIVO_ESTOQUE, "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(arquivo, "%d;%s;%d;%.2f\n",
            produto.codigo,
            produto.nome,
            produto.quantidade,
            produto.preco);

    fclose(arquivo);

    printf("Produto cadastrado com sucesso!\n");
}

/* Lista os produtos */
void listarProdutos() {
    Produto produto;
    FILE *arquivo;

    arquivo = fopen(ARQUIVO_ESTOQUE, "r");

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("\n=========== PRODUTOS ===========\n");

    while (fscanf(arquivo, "%d;%59[^;];%d;%f\n",
                  &produto.codigo,
                  produto.nome,
                  &produto.quantidade,
                  &produto.preco) == 4) {

        printf("\nCodigo: %d\n", produto.codigo);
        printf("Nome: %s\n", produto.nome);
        printf("Quantidade: %d\n", produto.quantidade);
        printf("Preco: R$ %.2f\n", produto.preco);
    }

    fclose(arquivo);
}

/* Movimenta o estoque */
void movimentarEstoque(int entrada) {
    Produto produtos[1000];
    int total = 0;
    int codigo;
    int quantidade;
    int encontrado = 0;

    FILE *arquivo;

    codigo = lerInteiro("Codigo do produto: ");
    quantidade = lerInteiro("Quantidade: ");

    if (quantidade <= 0) {
        printf("A quantidade deve ser maior que zero.\n");
        return;
    }

    arquivo = fopen(ARQUIVO_ESTOQUE, "r");

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    /* Lê todos os produtos */
    while (total < 1000 &&
           fscanf(arquivo, "%d;%59[^;];%d;%f\n",
                  &produtos[total].codigo,
                  produtos[total].nome,
                  &produtos[total].quantidade,
                  &produtos[total].preco) == 4) {

        total++;
    }

    fclose(arquivo);

    /* Procura o produto */
    for (int i = 0; i < total; i++) {

        if (produtos[i].codigo == codigo) {

            encontrado = 1;

            if (entrada == 1) {

                produtos[i].quantidade += quantidade;

                printf("Entrada realizada com sucesso!\n");

            } else {

                if (quantidade > produtos[i].quantidade) {

                    printf("Estoque insuficiente!\n");
                    printf("Disponivel: %d\n",
                           produtos[i].quantidade);

                    return;
                }

                produtos[i].quantidade -= quantidade;

                printf("Saida realizada com sucesso!\n");
            }

            printf("Quantidade atual: %d\n",
                   produtos[i].quantidade);

            break;
        }
    }

    if (!encontrado) {
        printf("Produto nao encontrado.\n");
        return;
    }

    /* Reescreve o arquivo inteiro */
    arquivo = fopen(ARQUIVO_ESTOQUE, "w");

    if (arquivo == NULL) {
        printf("Erro ao salvar o estoque.\n");
        return;
    }

    for (int i = 0; i < total; i++) {

        fprintf(arquivo, "%d;%s;%d;%.2f\n",
                produtos[i].codigo,
                produtos[i].nome,
                produtos[i].quantidade,
                produtos[i].preco);
    }

    fclose(arquivo);
}

/* Programa principal */
int main() {

    int opcao;

    do {

        printf("\n================================\n");
        printf("       SISTEMA DE ESTOQUE\n");
        printf("================================\n");

        printf("1 - Cadastrar produto\n");
        printf("2 - Listar produtos\n");
        printf("3 - Entrada de estoque\n");
        printf("4 - Saida de estoque\n");
        printf("0 - Sair\n");

        printf("================================\n");

        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {

            case 1:
                cadastrarProduto();
                break;

            case 2:
                listarProdutos();
                break;

            case 3:
                movimentarEstoque(1);
                break;

            case 4:
                movimentarEstoque(0);
                break;

            case 0:
                printf("Ate logo!\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}