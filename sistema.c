
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "estoque.txt"
#define TAM_NOME 100

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    int quantidade;
    float preco;
} Produto;

/* Limpar teclado */
void limparTeclado() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Ler inteiro */
int lerInteiro(const char *mensagem) {
    int valor;

    while (1) {
        printf("%s", mensagem);

        if (scanf("%d", &valor) == 1) {
            limparTeclado();
            return valor;
        }

        printf("Digite um numero valido!\n");
        limparTeclado();
    }
}

/* Ler preço */
float lerPreco(const char *mensagem) {
    float valor;

    while (1) {
        printf("%s", mensagem);

        if (scanf("%f", &valor) == 1) {
            limparTeclado();
            return valor;
        }

        printf("Digite um valor valido!\n");
        limparTeclado();
    }
}

/* Ler nome */
void lerNome(const char *mensagem, char *nome) {
    printf("%s", mensagem);

    fgets(nome, TAM_NOME, stdin);

    nome[strcspn(nome, "\n")] = '\0';
}

/* Verificar se produto existe */
int produtoExiste(int codigo) {
    FILE *arquivo;
    Produto produto;

    arquivo = fopen(ARQUIVO, "r");

    if (arquivo == NULL) {
        return 0;
    }

    while (fscanf(arquivo, "%d|%99[^|]|%d|%f\n",
                  &produto.codigo,
                  produto.nome,
                  &produto.quantidade,
                  &produto.preco) == 4) {

        if (produto.codigo == codigo) {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);

    return 0;
}

/* CADASTRAR */
void cadastrarProduto() {
    Produto produto;
    FILE *arquivo;

    printf("\n===== CADASTRAR PRODUTO =====\n");

    produto.codigo = lerInteiro("Codigo: ");

    if (produtoExiste(produto.codigo)) {
        printf("ERRO: Esse codigo ja esta cadastrado!\n");
        return;
    }

    lerNome("Nome: ", produto.nome);

    produto.quantidade =
        lerInteiro("Quantidade: ");

    produto.preco =
        lerPreco("Preco: R$ ");

    if (produto.quantidade < 0) {
        printf("A quantidade nao pode ser negativa!\n");
        return;
    }

    if (produto.preco < 0) {
        printf("O preco nao pode ser negativo!\n");
        return;
    }

    arquivo = fopen(ARQUIVO, "a");

    if (arquivo == NULL) {
        printf("ERRO ao abrir o arquivo!\n");
        return;
    }

    fprintf(arquivo, "%d|%s|%d|%.2f\n",
            produto.codigo,
            produto.nome,
            produto.quantidade,
            produto.preco);

    fclose(arquivo);

    printf("\nProduto cadastrado com sucesso!\n");
}

/* LISTAR */
void listarProdutos() {
    FILE *arquivo;
    Produto produto;
    int encontrou = 0;

    printf("\n===== LISTA DE PRODUTOS =====\n");

    arquivo = fopen(ARQUIVO, "r");

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    while (fscanf(arquivo, "%d|%99[^|]|%d|%f\n",
                  &produto.codigo,
                  produto.nome,
                  &produto.quantidade,
                  &produto.preco) == 4) {

        encontrou = 1;

        printf("\nCodigo: %d\n", produto.codigo);
        printf("Nome: %s\n", produto.nome);
        printf("Quantidade: %d\n", produto.quantidade);
        printf("Preco: R$ %.2f\n", produto.preco);
        printf("-----------------------------\n");
    }

    fclose(arquivo);

    if (!encontrou) {
        printf("Nenhum produto cadastrado.\n");
    }
}

/* ENTRADA */
void entradaEstoque() {
    FILE *arquivo;
    FILE *temporario;

    Produto produto;

    int codigo;
    int quantidade;
    int encontrou = 0;

    printf("\n===== ENTRADA DE ESTOQUE =====\n");

    codigo = lerInteiro("Codigo do produto: ");

    quantidade = lerInteiro("Quantidade de entrada: ");

    if (quantidade <= 0) {
        printf("A quantidade deve ser maior que zero!\n");
        return;
    }

    arquivo = fopen(ARQUIVO, "r");

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    temporario = fopen("temp.txt", "w");

    if (temporario == NULL) {
        printf("ERRO ao criar arquivo temporario!\n");
        fclose(arquivo);
        return;
    }

    while (fscanf(arquivo, "%d|%99[^|]|%d|%f\n",
                  &produto.codigo,
                  produto.nome,
                  &produto.quantidade,
                  &produto.preco) == 4) {

        if (produto.codigo == codigo) {
            produto.quantidade += quantidade;
            encontrou = 1;
        }

        fprintf(temporario, "%d|%s|%d|%.2f\n",
                produto.codigo,
                produto.nome,
                produto.quantidade,
                produto.preco);
    }

    fclose(arquivo);
    fclose(temporario);

    if (!encontrou) {
        remove("temp.txt");

        printf("Produto nao encontrado!\n");
        return;
    }

    remove(ARQUIVO);

    if (rename("temp.txt", ARQUIVO) != 0) {
        printf("ERRO ao atualizar o arquivo!\n");
        return;
    }

    printf("Entrada realizada com sucesso!\n");
    printf("Quantidade adicionada: %d\n", quantidade);
}

/* SAIDA */
void saidaEstoque() {
    FILE *arquivo;
    FILE *temporario;

    Produto produto;

    int codigo;
    int quantidade;
    int encontrou = 0;

    printf("\n===== SAIDA DE ESTOQUE =====\n");

    codigo = lerInteiro("Codigo do produto: ");

    quantidade = lerInteiro("Quantidade de saida: ");

    if (quantidade <= 0) {
        printf("A quantidade deve ser maior que zero!\n");
        return;
    }

    arquivo = fopen(ARQUIVO, "r");

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    temporario = fopen("temp.txt", "w");

    if (temporario == NULL) {
        printf("ERRO ao criar arquivo temporario!\n");
        fclose(arquivo);
        return;
    }

    while (fscanf(arquivo, "%d|%99[^|]|%d|%f\n",
                  &produto.codigo,
                  produto.nome,
                  &produto.quantidade,
                  &produto.preco) == 4) {

        if (produto.codigo == codigo) {

            encontrou = 1;

            if (quantidade > produto.quantidade) {
                printf("\nEstoque insuficiente!\n");
                printf("Estoque atual: %d\n",
                       produto.quantidade);

                fclose(arquivo);
                fclose(temporario);

                remove("temp.txt");

                return;
            }

            produto.quantidade -= quantidade;
        }

        fprintf(temporario, "%d|%s|%d|%.2f\n",
                produto.codigo,
                produto.nome,
                produto.quantidade,
                produto.preco);
    }

    fclose(arquivo);
    fclose(temporario);

    if (!encontrou) {
        remove("temp.txt");

        printf("Produto nao encontrado!\n");
        return;
    }

    remove(ARQUIVO);

    if (rename("temp.txt", ARQUIVO) != 0) {
        printf("ERRO ao atualizar o arquivo!\n");
        return;
    }

    printf("Saida realizada com sucesso!\n");
    printf("Quantidade retirada: %d\n", quantidade);
}

/* MENU PRINCIPAL */
int main() {

    int opcao;

    do {

        printf("\n");
        printf("=================================\n");
        printf("       SISTEMA DE ESTOQUE\n");
        printf("=================================\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Listar produtos\n");
        printf("3 - Entrada de estoque\n");
        printf("4 - Saida de estoque\n");
        printf("0 - Sair\n");
        printf("=================================\n");

        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {

            case 1:
                cadastrarProduto();
                break;

            case 2:
                listarProdutos();
                break;

            case 3:
                entradaEstoque();
                break;

            case 4:
                saidaEstoque();
                break;

            case 0:
                printf("\nPrograma encerrado. Ate logo!\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
