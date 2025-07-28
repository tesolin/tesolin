#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int opcaovalida();
int quantiavalida(char *produto);
int acharproduto(char *produto, int estoque, int operacao, int *valor);
int SomarSubtrair(int linha, int operacao, int quantia, int quantiaAnterior);
int verificacaoSN();
void listarItens();

FILE*arquivo;
FILE*arquivo2;

int main()
{
    int pular = 0;
    while(1)
    {
        if(pular == 0)
        {
            int resposta;
            char produto[100];
            char SN[5];
            int quantidade;
            int linha;
            int valor;
            printf("==================================\n");
            printf("Controle de Estoque\n");
            printf("==================================\n");
            printf("1. Adicionar Item\n2. Remover Item\n3. Listar Estoque\n4. Sair\n");
            resposta = opcaovalida();
            if(resposta == 1)
            {
                printf("Digite o nome do item: ");
                fgets(produto, sizeof(produto), stdin);
                produto[strcspn(produto, "\n")] = 0;
                printf("Digite a quantidade: ");
                quantidade = quantiavalida(produto);
                linha = acharproduto(produto, quantidade, 0, &valor);
                if(linha != 0)
                {
                    SomarSubtrair(linha, 0, quantidade, 0);
                }
                printf("Item adicionado com sucesso!");
            }
            else if(resposta == 2)
            {
                printf("Digite o nome do item: ");
                fgets(produto, sizeof(produto), stdin);
                produto[strcspn(produto, "\n")] = 0;
                printf("Digite a quantidade a ser removida: ");
                quantidade = quantiavalida(produto);
                linha = acharproduto(produto, quantidade, 1, &valor);
                if(linha != 0)
                {
                    if(valor >= quantidade)
                    {
                        if(SomarSubtrair(linha, 1, quantidade, valor) == 0)
                        {
                            printf("Item removido do estoque!");
                        }
                        else
                        {
                            printf("Quantidade atualizada com sucesso!");
                        }
                        pular = 1;
                        continue;
                    }
                    printf("Estoque insuficiente. Quantidade disponivel: %d",valor);
                    pular = 1;
                    continue;
                }
                printf("Item nao encontrado.");
            }
            else if(resposta == 3)
            {
                printf("==================================\n");
                printf("Estoque Atual\n");
                printf("==================================\n");
                listarItens();
            }
            else if(resposta == 4)
            {
                printf("Obrigado por usar o Controle de Estoques! Ate a proxima.");
                exit(0);
            }
        }
        printf("\nDeseja realizar outra operacao? (s/n): ");
        if(verificacaoSN() == 0)
        {
            printf("Obrigado por usar o Controle de Estoques! Ate a proxima.");
            exit(0);
        }
        pular = 0;
        system ("cls");

    }
}
int opcaovalida()
{
    char opcao[10];
    int numero;
    char extra;
    while(1)
    {
        fgets(opcao, sizeof(opcao), stdin);
        opcao[strcspn(opcao, "\n")] = 0;

        if(sscanf(opcao, "%d %c", &numero, &extra) == 1 && numero > 0 && numero < 5)
        {
            return numero;
        }
        else
        {
            printf("Opcao invalida! Tente novamente.\n");
        }
    }
}
int quantiavalida(char *produto)
{
    char quantidade[15];
    int numero;
    char extra;
    while(1)
    {
        fgets(quantidade, sizeof(quantidade), stdin);
        produto[strcspn(produto, "\n")] = 0;
        quantidade[strcspn(quantidade, "\n")] = 0;

        if(sscanf(quantidade, "%d %c", &numero, &extra) == 1 && numero > 0)
        {
            return numero;
        }
        else
        {
            printf("\033[F");
            printf("\033[2K");
            printf("Digite a quantidade: ");
        }
    }
}
int acharproduto(char *produto, int estoque, int operacao, int *valor)
 {
    arquivo = fopen("estoque.txt","r");
    char buffer[200];
    int i = 1;
    if(arquivo == NULL)
    {
        arquivo = fopen("estoque.txt","w");
        if(arquivo == NULL)
        {
            printf("Erro ao encontrar arquivo. Tente novamente mais tarde!");
            exit(1);
        }
        fclose(arquivo);
        arquivo = fopen("estoque.txt","r");
    }

    while (fgets(buffer, sizeof(buffer), arquivo))
    {
        i++;
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, produto) == 0)
        {
            if(operacao == 1)
            {
                fgets(buffer, sizeof(buffer), arquivo);
                buffer[strcspn(buffer, "\n")] = 0;
                *valor = atoi(buffer);
            }
            return i;

        }
    }
    if(operacao == 0)
    {
        int precisa_quebra = 0;
        fseek(arquivo, 0, SEEK_END);
        long tamanho = ftell(arquivo);

        if (tamanho > 0)
        {
            fseek(arquivo, -1, SEEK_END);
            if (fgetc(arquivo) != '\n')
            {
                precisa_quebra = 1;
            }
        }

        fclose(arquivo);
        arquivo = fopen("estoque.txt", "a");
        if (precisa_quebra) fprintf(arquivo, "\n");
        fprintf(arquivo, "%s\n%d", produto, estoque);

    }
    fclose(arquivo);
    return 0;
 }
int SomarSubtrair(int linha, int operacao, int quantia, int quantiaAnterior)
{
    arquivo = fopen("estoque.txt","r");
    arquivo2 = fopen("estoque2.txt","w");
    int valor;
    char buffer[200];
    int i = 1;
    if(arquivo == NULL)
    {
        printf("Erro ao encontrar arquivo. Tente novamente mais tarde!");
        exit(1);
    }
    if(arquivo2 == NULL)
    {
        arquivo2 = fopen("estoque2.txt","w");
        if(arquivo2 == NULL)
        {
            printf("Erro ao encontrar arquivo. Tente novamente mais tarde!");
            exit(1);
        }
    }

    while (fgets(buffer, sizeof(buffer), arquivo))
    {
        if (operacao == 1 && i == (linha - 1) && quantia == quantiaAnterior)
        {
            fgets(buffer, sizeof(buffer), arquivo);
            i += 2;
            continue;
        }

        if (i == linha)
        {
            valor = atoi(buffer);

            if (operacao == 0)
            {
                valor += quantia;
            }
            else if (operacao == 1 && quantia < quantiaAnterior)
            {
                valor -= quantia;
            }

            sprintf(buffer, "%d\n", valor);
        }
        fprintf(arquivo2, "%s",buffer);
        i++;
    }
    fclose(arquivo2);
    fclose(arquivo);
    arquivo2 = fopen("estoque2.txt","r");
    arquivo = fopen("estoque.txt", "w");
    while(fgets(buffer, sizeof(buffer), arquivo2))
    {
        fprintf(arquivo, "%s", buffer);
    }
    fclose(arquivo2);
    fclose(arquivo);
    remove("estoque2.txt");
    if(operacao == 1 && quantia == quantiaAnterior)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int verificacaoSN()
{
    char SN[5];

    while (1)
    {
        fgets(SN, sizeof(SN), stdin);
        SN[strcspn(SN, "\n")] = 0;

        int contem_numero = 0;
        for (int i = 0; SN[i] != '\0'; i++)
        {
            if (isdigit(SN[i]))
            {
                contem_numero = 1;
                break;
            }
        }

        if (contem_numero)
        {
            printf("\033[F");
            printf("\033[2K");
            printf("Deseja realizar outra operação? (s/n): ");
            continue;
        }

        if (strcmp(SN, "S") == 0 || strcmp(SN, "s") == 0)
        {
            return 1;
        }
        else if (strcmp(SN, "N") == 0 || strcmp(SN, "n") == 0)
        {
            return 0;
        }
        else
        {
            printf("\033[F");
            printf("\033[2K");
            printf("Deseja realizar outra operação? (s/n): ");
        }
    }
}
void listarItens()
{
    arquivo = fopen("estoque.txt","r");
    char buffer[200];
    int i = 1;

    if(arquivo == NULL)
    {
        printf("\033[F");
        printf("\033[2K");
        printf("\033[F");
        printf("\033[2K");
        printf("\033[F");
        printf("\033[2K");
        printf("O estoque esta vazio.");
        return;
    }
    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0)
    {
        printf("\033[F");
        printf("\033[2K");
        printf("\033[F");
        printf("\033[2K");
        printf("\033[F");
        printf("\033[2K");
        printf("O estoque esta vazio.");
        return;
    }
    rewind(arquivo);
    while(fgets(buffer, sizeof(buffer), arquivo))
    {
        if(strlen(buffer) == 0)
        {
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        if(i%2 != 0)
        {
            printf("Nome: %s\n",buffer);
        }
        else
        {
            printf("Quantidade: %s\n\n",buffer);
        }
        i++;
    }
}
