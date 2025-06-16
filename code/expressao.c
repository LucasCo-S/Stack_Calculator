#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "expressao.h"

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

void stackItems(Stack *S, Item *I)
{
    I->previous = S->top;
    S->top = I;
    S->size++;
}

void unstack(Stack *S)
{
    if (S->size == 0)
    {
        return;
    }

    Item *current = S->top;
    S->top = current->previous;
    S->size--;
    free(current);
}

void printStack(Stack *S)
{
    Item *current = S->top;

    while (current != NULL)
    {
        printf("%c", current->value);
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

Item *createItem(char digit)
{
    Item *I = (Item *)malloc(sizeof(Item));

    if (I == NULL)
    {
        printf("Erro ao criar um item");
        return NULL;
    }

    I->value = digit;

    I->previous = NULL;

    return I;
}

int isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

char *getFormaInFixa(char *Str)
{
    if (Str[0] == '\0')
        return NULL;

    Stack *S = createStack();

    int i;

    for (i = 0; Str[i] != '\0'; i++)
    {
        char c = Str[i];

        if (isOperator(c))
        {
            Item *I = createItem(c);
            stackItems(S, I);
        }
        else if (c != ' ')
        {

            Item *I = createItem(c);
            stackItems(S, I);
        }
    }

    printStack(S);
}

char *getFormaPosFixa(char *Str)
{
    printf("Pegando forma posfixa");
    return Str;
}

float getValorPosFixa(char *StrPosFixa)
{
    printf("Calculando posfixa");
    return 0.1;
}

float getValorInFixa(char *StrInFixa)
{
    printf("Calculando infixa");
    return 0.1;
}
