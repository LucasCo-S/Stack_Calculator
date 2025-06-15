#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para usar strcspn
#include "expressao.h"

typedef enum
{
    INFIX = 1,
    POSFIX = 2
} MainOption;

typedef enum
{
    GET = 1,
    CALCULATE = 2
} SubOption;

int main()
{
    Expressao *data = (Expressao *)malloc(sizeof(Expressao));

    if (data == NULL)
    {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    int mainOption, subOption;

    printf("Escolha o tipo de operacao:\n");
    printf("1 - Infixa\n");
    printf("2 - Posfixa\n");
    scanf("%d", &mainOption);
    getchar();

    system("cls");

    char *response;

    float value;

    switch (mainOption)
    {
    case INFIX:

        printf("Digite a forma infixa:\n");
        fgets(data->inFixa, sizeof(data->inFixa), stdin);
        // Acha a posição de \n e substitui por \0 que simboliza o fim da string
        data->inFixa[strcspn(data->inFixa, "\n")] = '\0';

        system("cls");

        printf("O que deseja fazer?\n");
        printf("1 - Obter posfixa\n");
        printf("2 - Calcular\n");
        scanf("%d", &subOption);
        getchar();

        system("cls");

        switch (subOption)
        {
        case GET:
            response = getFormaPosFixa(data->inFixa);
            printf("Posfixa: %s\n", response);
            break;

        case CALCULATE:
            value = getValorInFixa(data->inFixa);
            printf("Resultado: %.2f\n", value);
            break;

        default:
            printf("Opção inválida!\n");
            break;
        }
        break;

    case POSFIX:
        printf("Digite a forma posfixa:\n");
        fgets(data->posFixa, sizeof(data->posFixa), stdin);
        data->posFixa[strcspn(data->posFixa, "\n")] = '\0';

        system("cls");

        printf("O que deseja fazer?\n");
        printf("1 - Obter infixa\n");
        printf("2 - Calcular\n");
        scanf("%d", &subOption);
        getchar();

        system("cls");

        switch (subOption)
        {
        case GET:
            response = getFormaInFixa(data->posFixa);
            printf("Infixa: %s\n", response);
            break;

        case CALCULATE:
            value = getValorPosFixa(data->posFixa);
            printf("Resultado: %.2f\n", value);
            break;

        default:
            printf("Opção inválida!\n");
            break;
        }
        break;

    default:
        printf("Opção inválida!\n");
        break;
    }

    free(data);
    return 0;
}
