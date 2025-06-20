#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "expressao.h"

#define M_PI 3.14159265358979323846

const char *operators[] = {"+", "-", "*", "/", "^", "sen", "cos", "log", "tg", "raiz", NULL};

typedef enum
{
    OP_INVALID = -1,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    OP_LOG,
    OP_SEN,
    OP_COS,
    OP_TG,
    OP_RAIZ
} OperatorType;

typedef struct Item
{
    char *value;
    struct Item *previous;
} Item;

typedef struct Stack
{
    int size;
    Item *top;
} Stack;

int isOperatorStr(char *str)
{
    return (
        strcmp(str, "+") == 0 ||
        strcmp(str, "-") == 0 ||
        strcmp(str, "*") == 0 ||
        strcmp(str, "/") == 0 ||
        strcmp(str, "log") == 0 ||
        strcmp(str, "sen") == 0 ||
        strcmp(str, "cos") == 0 ||
        strcmp(str, "^") == 0 ||
        strcmp(str, "tg") == 0 ||
        strcmp(str, "raiz") == 0);
}

int containsOperator(const char *expr)
{
    int i;

    for (i = 0; expr[i] != '\0'; i++)
    {
        if (strchr("+-*/^", expr[i]) != NULL)
            return 1;
    }

    for (i = 0; operators[i] != NULL; i++)
    {
        if (strlen(operators[i]) > 1 && strstr(expr, operators[i]) != NULL)
            return 1;
    }

    return 0;
}

int validateInfixExpression(const char *expr)
{
    int parenthesesCount = 0;

    for (int i = 0; expr[i] != '\0'; i++)
    {
        if (expr[i] == '(')
            parenthesesCount++;
        else if (expr[i] == ')')
        {
            parenthesesCount--;
            if (parenthesesCount < 0)
                return -1;
        }
    }

    if (parenthesesCount != 0)
        return -1;

    if (!containsOperator(expr))
        return -1;

    return 0;
}

int validatePostfixExpression(const char *expr)
{
    char copy[300];
    strcpy(copy, expr);

    int stackSize = 0;

    char *token = strtok(copy, " ");

    while (token != NULL)
    {
        if (isOperatorStr(token))
        {

            if (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "log") == 0 || strcmp(token, "tg") == 0 || strcmp(token, "raiz") == 0)
            {
                if (stackSize < 1)
                    return -1;
            }
            else
            {
                if (stackSize < 2)
                    return -1;
                stackSize--;
            }
        }
        else
        {
            stackSize++;

            token = strtok(NULL, " ");
        }

        return (stackSize == 1) ? 0 : -1;
    }
}

void stackItems(Stack *S, Item *I)
{
    I->previous = S->top;
    S->top = I;
    S->size++;
}

Item *unstack(Stack *S)
{
    if (S->size == 0)
    {
        return NULL;
    }

    Item *current = S->top;

    S->top = current->previous;

    S->size--;

    return current;
}

void clearStack(Stack *S)
{
    while (S->size > 0)
    {
        Item *I = unstack(S);

        free(I->value);

        free(I);
    }
}

void printStack(Stack *S)
{
    Item *current = S->top;

    while (current != NULL)
    {
        printf("%s ", current->value);
        current = current->previous;
    }
}

Stack *createStack()
{
    Stack *stackNumbers = (Stack *)malloc(sizeof(Stack));

    if (stackNumbers == NULL)
    {
        printf("Erro ao criar a pilha");
        return NULL;
    }

    stackNumbers->size = 0;

    stackNumbers->top = NULL;

    return stackNumbers;
}

Item *createItem(char *str)
{
    Item *I = (Item *)malloc(sizeof(Item));

    if (I == NULL)
    {
        printf("Erro ao criar um item");
        return NULL;
    }

    I->value = strdup(str);

    I->previous = NULL;

    return I;
}

OperatorType getOperatorType(char *token)
{
    if (strcmp(token, "+") == 0)
        return OP_ADD;
    if (strcmp(token, "-") == 0)
        return OP_SUB;
    if (strcmp(token, "*") == 0)
        return OP_MUL;
    if (strcmp(token, "/") == 0)
        return OP_DIV;
    if (strcmp(token, "^") == 0)
        return OP_POW;
    if (strcmp(token, "log") == 0)
        return OP_LOG;
    if (strcmp(token, "sen") == 0)
        return OP_SEN;
    if (strcmp(token, "cos") == 0)
        return OP_COS;
    if (strcmp(token, "tg") == 0)
        return OP_TG;
    if (strcmp(token, "raiz") == 0)
        return OP_RAIZ;

    return OP_INVALID;
}

char *getFormaInFixa(char *Str)
{

    if (validatePostfixExpression(Str) == -1)
    {
        printf("Expressao invalida");
        return NULL;
    }

    if (Str[0] == '\0')
        return NULL;

    Stack *S = createStack();

    char *copy = strdup(Str);

    if (strchr(copy, ' ') == NULL)
    {
        printf("A sua expressao esta mal formatada. Utilize o molde a seguir: 3 4 + 5 *");
        return NULL;
    }

    char *token = strtok(copy, " ");

    while (token != NULL)
    {

        if (isOperatorStr(token))
        {
            if (strcmp(token, "log") == 0 || strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 || strcmp(token, "raiz") == 0)
            {

                Item *arg = unstack(S);

                char result[150];

                sprintf(result, "%s(%s)", token, arg->value);

                Item *I = createItem(result);

                stackItems(S, I);
            }
            else
            {

                Item *first = unstack(S);

                Item *second = unstack(S);

                if (first == NULL || second == NULL)
                {
                    printf("Operandos insuficientes");
                    clearStack(S);
                    return NULL;
                }

                char result[150];

                sprintf(result, "(%s %s %s)", second->value, token, first->value);

                Item *I = createItem(result);

                stackItems(S, I);
            }
        }
        else if (token != " ")
        {
            Item *I = createItem(token);

            stackItems(S, I);
        }

        token = strtok(NULL, " ");
    }

    Item *final = unstack(S);

    char *finalExpression = strdup(final->value);

    free(final->value);

    free(final);

    clearStack(S);

    return finalExpression;
}

int getPrecedence(char *op)
{
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
        return 2;
    if (strcmp(op, "^") == 0)
        return 3;
    if (strcmp(op, "log") == 0 || strcmp(op, "sen") == 0 || strcmp(op, "cos") == 0 || strcmp(op, "tg") == 0 || strcmp(op, "raiz") == 0)
        return 4;

    return 0;
}

#include <ctype.h>
#include <string.h>

void separateParentheses(char *destiny, char *origin)
{
    int i = 0, j = 0;

    while (origin[i] != '\0')
    {

        if (origin[i] == '(' || origin[i] == ')')
        {
            destiny[j++] = ' ';
            destiny[j++] = origin[i];
            destiny[j++] = ' ';
            i++;
        }

        else if (origin[i] == '^')
        {
            destiny[j++] = ' ';
            destiny[j++] = '^';
            destiny[j++] = ' ';
            i++;
        }

        else if ((strncmp(&origin[i], "log", 3) == 0 ||
                  strncmp(&origin[i], "sen", 3) == 0 ||
                  strncmp(&origin[i], "cos", 3) == 0 ||
                  strncmp(&origin[i], "tg", 2) == 0 ||
                  strncmp(&origin[i], "raiz", 4) == 0))
        {
            if (strncmp(&origin[i], "raiz", 4) == 0)
            {
                destiny[j++] = 'r';
                destiny[j++] = 'a';
                destiny[j++] = 'i';
                destiny[j++] = 'z';
                destiny[j++] = ' ';
                i += 4;
            }
            else if (strncmp(&origin[i], "tg", 2) == 0)
            {
                destiny[j++] = 't';
                destiny[j++] = 'g';
                destiny[j++] = ' ';
                i += 2;
            }
            else
            {
                destiny[j++] = origin[i++];
                destiny[j++] = origin[i++];
                destiny[j++] = origin[i++];
                destiny[j++] = ' ';
            }
        }
        else
        {
            destiny[j++] = origin[i++];
        }
    }

    destiny[j] = '\0';
}

char *getFormaPosFixa(char *Str)
{

    if (validateInfixExpression(Str) == -1)
    {
        printf("A expressao invalida");
        return NULL;
    }

    char formattedExpression[300];

    separateParentheses(formattedExpression, Str);

    if (strchr(formattedExpression, ' ') == NULL)
    {
        printf("A sua expressao esta mal formatada. Utilize o molde a seguir: (3 + 4) * 5\n");
        return NULL;
    }

    char *token = strtok(formattedExpression, " ");
    Stack *S = createStack();
    char output[300] = "";

    while (token != NULL)
    {
        if (strcmp(token, "(") == 0)
        {
            stackItems(S, createItem(token));
        }
        else if (isOperatorStr(token))
        {
            if (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 ||
                strcmp(token, "log") == 0 || strcmp(token, "tg") == 0 || strcmp(token, "raiz") == 0)
            {
                stackItems(S, createItem(token));
            }
            else
            {
                while (S->size > 0 && isOperatorStr(S->top->value) &&
                       getPrecedence(S->top->value) >= getPrecedence(token))
                {
                    Item *I = unstack(S);
                    strcat(output, I->value);
                    strcat(output, " ");
                    free(I->value);
                    free(I);
                }
                stackItems(S, createItem(token));
            }
        }
        else if (strcmp(token, ")") == 0)
        {
            Item *I;
            while ((I = unstack(S)) != NULL && strcmp(I->value, "(") != 0)
            {
                strcat(output, I->value);
                strcat(output, " ");
                free(I->value);
                free(I);
            }

            if (I != NULL)
            {
                free(I->value);
                free(I);
            }

            if (S->size > 0 && (strcmp(S->top->value, "sen") == 0 ||
                                strcmp(S->top->value, "cos") == 0 ||
                                strcmp(S->top->value, "log") == 0 ||
                                strcmp(S->top->value, "tg") == 0 ||
                                strcmp(S->top->value, "raiz") == 0))

            {
                Item *func = unstack(S);
                strcat(output, func->value);
                strcat(output, " ");
                free(func->value);
                free(func);
            }
        }
        else
        {
            strcat(output, token);

            strcat(output, " ");
        }

        token = strtok(NULL, " ");
    }

    Item *I;

    while ((I = unstack(S)) != NULL)
    {
        strcat(output, I->value);
        strcat(output, " ");
        free(I->value);
        free(I);
    }

    clearStack(S);
    free(S);

    return strdup(output);
}

float getValorPosFixa(char *StrPosFixa)
{

    if (validatePostfixExpression(StrPosFixa) == -1)
    {
        printf("Expressao invalida");
        return -1;
    }

    char *copy = strdup(StrPosFixa);

    if (strchr(copy, ' ') == NULL)
    {
        printf("A sua expressao esta mal formatada. Utilize o molde a seguir: 3 4 + 5 *");
        return -1;
    }

    Stack *S = createStack();

    char *token = strtok(copy, " ");

    while (token != NULL)
    {
        if (isOperatorStr(token))
        {
            if (strcmp(token, "log") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "sen") == 0 ||
                strcmp(token, "tg") == 0 || strcmp(token, "raiz") == 0)

            {
                Item *arg = unstack(S);

                float number = atof(arg->value);

                OperatorType op = getOperatorType(token);

                float value;

                char result[100];

                float rad = number * (M_PI / 180.0f);

                switch (op)
                {
                case OP_SEN:
                    value = sinf(rad);
                    break;

                case OP_COS:
                    value = cosf(rad);
                    break;

                case OP_TG:
                    value = tanf(rad);
                    break;

                case OP_LOG:
                    value = log10f(number);
                    break;

                case OP_RAIZ:
                    if (number < 0)
                    {
                        printf("Raiz de numero negativo invalida.\n");
                        return -1;
                    }
                    value = sqrtf(number);
                    break;

                default:
                    printf("Operador inválido\n");
                    return -1;
                }
                sprintf(result, "%.3f", value);

                Item *I = createItem(result);

                stackItems(S, I);
            }
            else
            {
                Item *secondNumber = unstack(S);

                float second = atof(secondNumber->value);

                Item *firstNumber = unstack(S);

                float first = atof(firstNumber->value);

                OperatorType op = getOperatorType(token);

                char result[100];

                float value;

                switch (op)
                {
                case OP_ADD:
                    value = first + second;
                    sprintf(result, "%.3f", value);
                    break;

                case OP_SUB:
                    value = first - second;
                    sprintf(result, "%.3f", value);
                    break;

                case OP_MUL:
                    value = first * second;
                    sprintf(result, "%.3f", value);
                    break;

                case OP_DIV:
                    if (second != 0)
                    {
                        value = first / second;
                        sprintf(result, "%.3f", value);
                    }
                    else
                    {
                        printf("O dividendo deve ser maior que zero");
                        return -1;
                    }
                    break;

                case OP_POW:
                    value = powf(first, second);
                    sprintf(result, "%.3f", value);
                    break;

                default:
                    printf("Operado invalido");
                    return -1;
                }

                Item *I = createItem(result);

                stackItems(S, I);
            }
        }
        else
        {
            Item *I = createItem(token);

            stackItems(S, I);
        }

        token = strtok(NULL, " ");
    }

    Item *final = unstack(S);

    float finalResult = atof(final->value);

    free(copy);

    clearStack(S);

    return finalResult;
}

float getValorInFixa(char *StrInFixa)
{
    if (validateInfixExpression(StrInFixa) == -1)
    {
        printf("A expressao invalida");
        return -1;
    }

    char *copy = strdup(StrInFixa);

    char *result = getFormaPosFixa(copy);

    if (result == NULL)
    {
        return -1;
    }

    int i;

    for (i = 0; result[i] != '\0'; i++)
    {
        if (result[i] == ',')
            result[i] = '.';
    }

    float calc = getValorPosFixa(result);

    return calc;
}
