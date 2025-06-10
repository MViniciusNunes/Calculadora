#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "expressao.h"

#define MAX 512

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//==================================================================
// ESTRUTURAS DE PILHA
//==================================================================

// --- Pilha de Strings (Para conversões de notação) ---
typedef struct {
    char itens[MAX][MAX];
    int topo;
} PilhaStr;

void initPilhaStr(PilhaStr *p) { p->topo = -1; }
int isEmptyStr(PilhaStr *p) { return p->topo == -1; }
void pushStr(PilhaStr *p, char *val) { if (p->topo < MAX - 1) strcpy(p->itens[++(p->topo)], val); }
char* popStr(PilhaStr *p) { return !isEmptyStr(p) ? p->itens[(p->topo)--] : NULL; }
char* peekStr(PilhaStr *p) { return !isEmptyStr(p) ? p->itens[p->topo] : NULL; }

// --- Pilha de Floats (Para cálculo de valores) ---
typedef struct {
    float itens[MAX];
    int topo;
} PilhaFloat;

void initPilhaFloat(PilhaFloat *p) { p->topo = -1; }
int isEmptyFloat(PilhaFloat *p) { return p->topo == -1; }
void pushFloat(PilhaFloat *p, float val) { if (p->topo < MAX - 1) p->itens[++(p->topo)] = val; }
float popFloat(PilhaFloat *p) { return !isEmptyFloat(p) ? p->itens[(p->topo)--] : 0.0; }


//==================================================================
// FUNÇÕES AUXILIARES
//==================================================================

int prioridade(char *op) {
    if (strcmp(op, "sen") == 0 || strcmp(op, "cos") == 0 || strcmp(op, "tg") == 0 ||
        strcmp(op, "log") == 0 || strcmp(op, "raiz") == 0) return 4;
    if (strcmp(op, "^") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    return 0; // Para parênteses
}

int ehOperador(char *token) {
    return strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
           strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
           strcmp(token, "%") == 0 || strcmp(token, "^") == 0 ||
           strcmp(token, "log") == 0 || strcmp(token, "sen") == 0 ||
           strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
           strcmp(token, "raiz") == 0;
}

//==================================================================
// FUNÇÕES PRINCIPAIS
//==================================================================

/**
 * Converte Infixa para Pós-fixa (SUA NOVA VERSÃO INTEGRADA).
 */
char *getFormaPosFixa(char *Str) {
    static char saida[MAX] = "";
    PilhaStr pilha;
    initPilhaStr(&pilha);
    saida[0] = '\0';

    char token[MAX];
    int i = 0;
    while (Str[i] != '\0') {
        if (isspace(Str[i])) {
            i++;
            continue;
        }

        if (isdigit(Str[i]) || Str[i] == '.') {
            int j = 0;
            while (isdigit(Str[i]) || Str[i] == '.') {
                token[j++] = Str[i++];
            }
            token[j] = '\0';
            strcat(saida, token);
            strcat(saida, " ");
        } else if (isalpha(Str[i])) {
            int j = 0;
            while (isalpha(Str[i])) {
                token[j++] = Str[i++];
            }
            token[j] = '\0';
            pushStr(&pilha, token);
        } else if (Str[i] == '(') {
            token[0] = '(';
            token[1] = '\0';
            pushStr(&pilha, token);
            i++;
        } else if (Str[i] == ')') {
            while (!isEmptyStr(&pilha) && strcmp(peekStr(&pilha), "(") != 0) {
                strcat(saida, popStr(&pilha));
                strcat(saida, " ");
            }
            if (!isEmptyStr(&pilha)) popStr(&pilha); // remove '('
            i++;
        } else { // Operador
            token[0] = Str[i];
            token[1] = '\0';
            // Adicionada a verificação de parênteses para segurança
            while (!isEmptyStr(&pilha) && strcmp(peekStr(&pilha), "(") != 0 && prioridade(peekStr(&pilha)) >= prioridade(token)) {
                strcat(saida, popStr(&pilha));
                strcat(saida, " ");
            }
            pushStr(&pilha, token);
            i++;
        }
    }

    while (!isEmptyStr(&pilha)) {
        strcat(saida, popStr(&pilha));
        strcat(saida, " ");
    }

    int len = strlen(saida);
    if (len > 0 && saida[len - 1] == ' ')
        saida[len - 1] = '\0';

    return saida;
}

/**
 * Converte Pós-fixa para Infixa.
 */
char *getFormaInFixa(char *StrPosFixa) {
    static char inFixaResult[MAX];
    PilhaStr pilha;
    initPilhaStr(&pilha);

    char strCpy[MAX];
    strcpy(strCpy, StrPosFixa);
    char *token = strtok(strCpy, " ");

    while (token != NULL) {
        if (!ehOperador(token)) {
            pushStr(&pilha, token);
        } else {
            char op2[MAX], op1[MAX], temp[MAX];
            strcpy(op2, popStr(&pilha));

            if (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 || strcmp(token, "log") == 0 || strcmp(token, "raiz") == 0) {
                 sprintf(temp, "%s(%s)", token, op2);
            } else {
                strcpy(op1, popStr(&pilha));
                sprintf(temp, "(%s %s %s)", op1, token, op2);
            }
            pushStr(&pilha, temp);
        }
        token = strtok(NULL, " ");
    }
    strcpy(inFixaResult, popStr(&pilha));
    return inFixaResult;
}

/**
 * Calcula o valor de uma expressão Pós-fixa.
 */
float getValorPosFixa(char *StrPosFixa) {
    PilhaFloat pilha;
    initPilhaFloat(&pilha);

    char strCpy[MAX];
    strcpy(strCpy, StrPosFixa);
    char *token = strtok(strCpy, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            pushFloat(&pilha, atof(token));
        } else {
            float val2, val1;
            val2 = popFloat(&pilha);

            if (strcmp(token, "+") == 0) { val1 = popFloat(&pilha); pushFloat(&pilha, val1 + val2); }
            else if (strcmp(token, "-") == 0) { val1 = popFloat(&pilha); pushFloat(&pilha, val1 - val2); }
            else if (strcmp(token, "*") == 0) { val1 = popFloat(&pilha); pushFloat(&pilha, val1 * val2); }
            else if (strcmp(token, "/") == 0) { val1 = popFloat(&pilha); pushFloat(&pilha, val1 / val2); }
            else if (strcmp(token, "%") == 0) { val1 = popFloat(&pilha); pushFloat(&pilha, fmod(val1, val2)); }
            else if (strcmp(token, "^") == 0) { val1 = popFloat(&pilha); pushFloat(&pilha, pow(val1, val2)); }
            else if (strcmp(token, "log") == 0) { pushFloat(&pilha, log10(val2)); }
            else if (strcmp(token, "raiz") == 0) { pushFloat(&pilha, sqrt(val2)); }
            else if (strcmp(token, "sen") == 0) { double rad = val2 * M_PI / 180.0; pushFloat(&pilha, sin(rad)); }
            else if (strcmp(token, "cos") == 0) { double rad = val2 * M_PI / 180.0; pushFloat(&pilha, cos(rad)); }
            else if (strcmp(token, "tg") == 0) { double rad = val2 * M_PI / 180.0; pushFloat(&pilha, tan(rad)); }
        }
        token = strtok(NULL, " ");
    }
    return popFloat(&pilha);
}

/**
 * Calcula o valor de uma expressão Infixa.
 */
float getValorInFixa(char *StrInFixa) {
    char *posFixa = getFormaPosFixa(StrInFixa);
    return getValorPosFixa(posFixa);
}