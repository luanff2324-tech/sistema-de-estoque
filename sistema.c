#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_ESTOQUE "estoque.dat"
#define MAX_NOME 60

typedef struct {
    int codigo;
    char nome[MAX_NOME];
    int quantidade;
    float preco;
} Produto;

static void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static int lerInteiro(const char *mensagem) {
    int valor;
    for (;;) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1) {
            limparEntrada();
            return valor;
        }
        printf("Valor invalido. Tente novamente.\n");
        limparEntrada();
    }
}

static float lerFloat(const char *mensagem) {
    float valor;
    for (;;) {
        printf("%s", mensagem);
        if (scanf("%f", &valor) == 1) {
            limparEntrada();
            return valor;
        }
        printf("Valor invalido. Tente novamente.\n");
        limparEntrada();
    }
}

static void lerTexto(const char *mensagem, char *destino, size_t tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, (int)tamanho, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = '\0';
    }
}

static int buscarProduto(FILE *arquivo, int codigo, Produto *produto, long *posicao) {
    rewind(arquivo);
    while (fread(produto, sizeof(Produto), 1, arquivo) == 1) {
        if (produto->codigo == codigo) {
            if (posicao != NULL) *posicao = ftell(arquivo) - (long)sizeof(Produto);
            return 1;
        }
    }
    return 0;
}

static void cadastrarProduto(void) {
    Produto produto, existente;
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "ab+");
    if (arquivo == NULL) { perror("Nao foi possivel abrir o arquivo"); return; }

    produto.codigo = lerInteiro("Codigo do produto: ");
    if (buscarProduto(arquivo, produto.codigo, &existente, NULL)) {
        printf("Ja existe um produto com esse codigo.\n");
        fclose(arquivo);
        return;
    }
    lerTexto("Nome: ", produto.nome, sizeof(produto.nome));
    produto.quantidade = lerInteiro("Quantidade inicial: ");
    produto.preco = lerFloat("Preco unitario: R$ ");

    if (produto.quantidade < 0 || produto.preco < 0) {
        printf("Quantidade e preco nao podem ser negativos.\n");
    } else {
        fseek(arquivo, 0, SEEK_END);
        fwrite(&produto, sizeof(Produto), 1, arquivo);
        printf("Produto cadastrado com sucesso.\n");
    }
    fclose(arquivo);
}

static void listarProdutos(void) {
    Produto produto;
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "rb");
    if (arquivo == NULL) { printf("Nenhum produto cadastrado.\n"); return; }

    printf("\n%-8s %-30s %-12s %s\n", "CODIGO", "NOME", "QTD", "PRECO");
    printf("----------------------------------------------------------------\n");
    while (fread(&produto, sizeof(Produto), 1, arquivo) == 1) {
        printf("%-8d %-30s %-12d R$ %.2f\n", produto.codigo, produto.nome,
               produto.quantidade, produto.preco);
    }
    fclose(arquivo);
}

static void movimentarEstoque(int entrada) {
    Produto produto;
    long posicao;
    int codigo = lerInteiro("Codigo do produto: ");
    int quantidade = lerInteiro("Quantidade: ");
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "rb+");

    if (arquivo == NULL) { printf("Nenhum produto cadastrado.\n"); return; }
    if (quantidade <= 0) { printf("A quantidade deve ser maior que zero.\n"); fclose(arquivo); return; }
    if (!buscarProduto(arquivo, codigo, &produto, &posicao)) {
        printf("Produto nao encontrado.\n");
    } else if (!entrada && quantidade > produto.quantidade) {
        printf("Estoque insuficiente. Disponivel: %d\n", produto.quantidade);
    } else {
        produto.quantidade += entrada ? quantidade : -quantidade;
        fseek(arquivo, posicao, SEEK_SET);
        fwrite(&produto, sizeof(Produto), 1, arquivo);
        printf("Estoque atualizado. Quantidade atual: %d\n", produto.quantidade);
    }
    fclose(arquivo);
}

int main(void) {
    int opcao;
    do {
        printf("\n=== SISTEMA DE ESTOQUE ===\n");
        printf("1 - Cadastrar produto\n2 - Listar produtos\n3 - Entrada de estoque\n");
        printf("4 - Saida de estoque\n0 - Sair\n");
        opcao = lerInteiro("Escolha uma opcao: ");
        switch (opcao) {
            case 1: cadastrarProduto(); break;
            case 2: listarProdutos(); break;
            case 3: movimentarEstoque(1); break;
            case 4: movimentarEstoque(0); break;
            case 0: printf("Ate logo!\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
    return 0;
}