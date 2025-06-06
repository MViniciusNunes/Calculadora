#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>  // já deve estar, pois usa sin/cos/etc

#define M_PI 3.14159265358979323846

// Função interna (privada) para conversão de graus para radianos
static float grausParaRadianos(float graus) {
    return graus * M_PI / 180.0;
}

#include "expressao.h"

// Definição da pilha de strings para getFormaInFixa
typedef struct NodeInfixa {
    char *expr;
    struct NodeInfixa *next;
} NodeInfixa;

typedef struct {
    NodeInfixa *top;
} PilhaInfixa;

PilhaInfixa *criarPilhaInfixa() {
    PilhaInfixa *p = (PilhaInfixa *)malloc(sizeof(PilhaInfixa));
    p->top = NULL;
    return p;
}

void empilharInfixa(PilhaInfixa *p, char *expr) {
    NodeInfixa *n = (NodeInfixa *)malloc(sizeof(NodeInfixa));
    n->expr = expr;
    n->next = p->top;
    p->top = n;
}

char *desempilharInfixa(PilhaInfixa *p) {
    if (!p->top) return NULL;
    NodeInfixa *n = p->top;
    char *expr = n->expr;
    p->top = n->next;
    free(n);
    return expr;
}

int ehNumeroInfixa(char *token) {
    char *endptr;
    strtod(token, &endptr);
    return endptr != token && *endptr == '\0';
}

int ehOperadorInfixa(char *token) {
    return strlen(token) == 1 && strchr("+-*/^", token[0]);
}

int ehFuncaoInfixa(char *token) {
    // Exemplo: sin, cos, log, etc.
    return (strcmp(token, "sin") == 0 || strcmp(token, "cos") == 0 ||
            strcmp(token, "tan") == 0 || strcmp(token, "log") == 0 ||
            strcmp(token, "sqrt") == 0);
}

// Retorna a forma inFixa de Str (posFixa)
char *getFormaInFixa(char *Str) {
    PilhaInfixa *pilha = criarPilhaInfixa();
    char *tokens = strdup(Str);
    char *token = strtok(tokens, " ");

    while (token) {
        if (ehNumeroInfixa(token)) {
            empilharInfixa(pilha, strdup(token));
        } else if (ehOperadorInfixa(token)) {
            // Binário: desempilha 2
            char *b = desempilharInfixa(pilha);
            char *a = desempilharInfixa(pilha);
            int len = strlen(a) + strlen(b) + 5;
            char *expr = (char *)malloc(len);
            snprintf(expr, len, "(%s %s %s)", a, token, b);
            free(a); free(b);
            empilharInfixa(pilha, expr);
        } else if (ehFuncaoInfixa(token)) {
            // Unário: desempilha 1
            char *x = desempilharInfixa(pilha);
            int len = strlen(token) + strlen(x) + 4;
            char *expr = (char *)malloc(len);
            snprintf(expr, len, "%s(%s)", token, x);
            free(x);
            empilharInfixa(pilha, expr);
        }
        token = strtok(NULL, " ");
    }
    free(tokens);

    char *resultado = desempilharInfixa(pilha);
    // Liberar pilha
    free(pilha);
    return resultado;
}

// Definição da pilha de strings (ponteiros para char*)
typedef struct Node {
    char *op;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
} Pilha;

Pilha *criarPilha() {
    Pilha *p = (Pilha *)malloc(sizeof(Pilha));
    p->top = NULL;
    return p;
}

void empilhar(Pilha *p, char *op) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->op = op;
    n->next = p->top;
    p->top = n;
}

char *desempilhar(Pilha *p) {
    if (!p->top) return NULL;
    Node *n = p->top;
    char *op = n->op;
    p->top = n->next;
    free(n);
    return op;
}

char *topo(Pilha *p) {
    if (!p->top) return NULL;
    return p->top->op;
}

int ehNumero(char *token) {
    char *endptr;
    strtod(token, &endptr);
    return endptr != token && *endptr == '\0';
}

int ehOperador(char *token) {
    return strlen(token) == 1 && strchr("+-*/^", token[0]);
}

int ehFuncao(char *token) {
    return (strcmp(token, "sin") == 0 || strcmp(token, "cos") == 0 ||
            strcmp(token, "tan") == 0 || strcmp(token, "log") == 0 ||
            strcmp(token, "sqrt") == 0);
}

int precedencia(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

// Retorna a forma posFixa de Str (inFixa)
char *getFormaPosFixa(char *Str) {
    // Começa a conversão
    Pilha *pilha = criarPilha();
    char *saida = (char *)malloc(1024); // Tamanho fixo para simplificar
    saida[0] = '\0';

    char *tokens = strdup(Str);
    char *token = strtok(tokens, " ");

    while (token) {
        if (ehNumero(token)) {
            strcat(saida, token);
            strcat(saida, " ");
        } else if (ehFuncao(token)) {
            empilhar(pilha, strdup(token));
        } else if (ehOperador(token)) {
            while (topo(pilha) && 
                   (ehOperador(topo(pilha)) || ehFuncao(topo(pilha))) &&
                   ((ehFuncao(topo(pilha))) ||
                    (precedencia(topo(pilha)[0]) > precedencia(token[0])) ||
                    (precedencia(topo(pilha)[0]) == precedencia(token[0]) && token[0] != '^'))) {
                strcat(saida, desempilhar(pilha));
                strcat(saida, " ");
            }
            empilhar(pilha, strdup(token));
        } else if (strcmp(token, "(") == 0) {
            empilhar(pilha, strdup(token));
        } else if (strcmp(token, ")") == 0) {
            while (topo(pilha) && strcmp(topo(pilha), "(") != 0) {
                strcat(saida, desempilhar(pilha));
                strcat(saida, " ");
            }
            if (topo(pilha) && strcmp(topo(pilha), "(") == 0) {
                free(desempilhar(pilha)); // Remove "("
            }
            // Se topo for função, desempilha também
            if (topo(pilha) && ehFuncao(topo(pilha))) {
                strcat(saida, desempilhar(pilha));
                strcat(saida, " ");
            }
        }
        token = strtok(NULL, " ");
    }

    // Desempilha o que sobrou
    while (topo(pilha)) {
        strcat(saida, desempilhar(pilha));
        strcat(saida, " ");
    }

    // Remove espaço final
    int len = strlen(saida);
    if (len > 0 && saida[len-1] == ' ')
        saida[len-1] = '\0';

    free(tokens);
    free(pilha);
    return saida;
}
// Pilha de floats
typedef struct NodeFloat {
    float val;
    struct NodeFloat *next;
} NodeFloat;

typedef struct {
    NodeFloat *top;
} PilhaFloat;

PilhaFloat *criarPilhaFloat() {
    PilhaFloat *p = (PilhaFloat *)malloc(sizeof(PilhaFloat));
    p->top = NULL;
    return p;
}

void empilharFloat(PilhaFloat *p, float val) {
    NodeFloat *n = (NodeFloat *)malloc(sizeof(NodeFloat));
    n->val = val;
    n->next = p->top;
    p->top = n;
}

float desempilharFloat(PilhaFloat *p) {
    if (!p->top) return 0.0f;
    NodeFloat *n = p->top;
    float val = n->val;
    p->top = n->next;
    free(n);
    return val;
}

int ehNumeroFloat(char *token) {
    char *endptr;
    strtod(token, &endptr);
    return endptr != token && *endptr == '\0';
}

int ehOperadorFloat(char *token) {
    return strlen(token) == 1 && strchr("+-*/^", token[0]);
}

int ehFuncaoFloat(char *token) {
    return (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 ||
            strcmp(token, "tg")  == 0 || strcmp(token, "log") == 0 ||
            strcmp(token, "sqrt") == 0);
}

float aplicarOperador(char *op, float a, float b) {
    switch (op[0]) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return powf(a, b);
        default: return 0.0f;
    }
}

float aplicarFuncao(char *func, float x) {
    if (strcmp(func, "sen") == 0) return sinf(grausParaRadianos(x));
    if (strcmp(func, "cos") == 0) return cosf(grausParaRadianos(x));
    if (strcmp(func, "tg")  == 0) return tanf(grausParaRadianos(x));
    if (strcmp(func, "log") == 0) return log10f(x);  // log de base 10
    if (strcmp(func, "sqrt") == 0) return sqrtf(x);
    // Versão antiga para compatibilidade
    if (strcmp(func, "sin") == 0) return sinf(x);
    if (strcmp(func, "tan") == 0) return tanf(x);
    return 0.0f;
}

float getValorPosFixa(char *StrPosFixa) {
    PilhaFloat *pilha = criarPilhaFloat();
    char *tokens = strdup(StrPosFixa);
    char *token = strtok(tokens, " ");

    while (token) {
        if (ehNumeroFloat(token)) {
            empilharFloat(pilha, atof(token));
        } else if (ehOperadorFloat(token)) {
            float b = desempilharFloat(pilha);
            float a = desempilharFloat(pilha);
            float res = aplicarOperador(token, a, b);
            empilharFloat(pilha, res);
        } else if (ehFuncaoFloat(token)) {
            float x = desempilharFloat(pilha);
            float res = aplicarFuncao(token, x);
            empilharFloat(pilha, res);
        }
        token = strtok(NULL, " ");
    }

    float resultado = desempilharFloat(pilha);
    free(tokens);
    free(pilha);
    return resultado;
}
// Calcula o valor de Str (na forma inFixa)
float getValorInFixa(char *StrInFixa) {
    char *exprPosFixa = getFormaPosFixa(StrInFixa);
    float resultado = getValorPosFixa(exprPosFixa);
    free(exprPosFixa);
    return resultado;
}
