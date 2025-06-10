#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

int main() {
    int opcao = -1; // Inicializa com um valor inválido
    char entrada[512];

    while (1) {
        printf("\n==== MENU ====\n");
        printf("1 - Traduzir expressao infixada -> pos-fixada\n");
        printf("2 - Traduzir expressao pos-fixada -> infixada\n");
        printf("3 - Avaliar expressao infixada\n");
        printf("4 - Avaliar expressao pos-fixada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        // --- LÓGICA DE VALIDAÇÃO DE ENTRADA DO MENU ---
        char buffer[512];
        char char_extra;
        fgets(buffer, sizeof(buffer), stdin);

        // Tenta ler um inteiro e verifica se há algo a mais na linha.
        // A entrada só é válida se sscanf conseguir ler EXATAMENTE 1 item (o número).
        if (sscanf(buffer, "%d %c", &opcao, &char_extra) != 1) {
            opcao = -1; // Define como inválida se houver lixo ou se não for um número.
        }
        
        //------------------------------------------------

        switch (opcao) {
            case 1:
                printf("Digite a expressao infixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';
                
                // Substitui vírgula por ponto na entrada
                for (int i = 0; entrada[i]; i++) {
                    if (entrada[i] == ',') entrada[i] = '.';
                }

                char *posFixa = getFormaPosFixa(entrada);
                if (posFixa == NULL) {
                    printf(">> ERRO: Nao foi possivel traduzir a expressao.\n");
                } else {
                    printf("Expressao pos-fixada: %s\n", posFixa);
                }
                break;

            case 2:
                printf("Digite a expressao pos-fixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';

                char *inFixa = getFormaInFixa(entrada);
                if (inFixa == NULL) {
                    printf(">> ERRO: Nao foi possivel traduzir a expressao.\n");
                } else {
                    printf("Expressao infixada: %s\n", inFixa);
                }
                break;

            case 3:
                printf("Digite a expressao infixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';
                
                // Substitui vírgula por ponto na entrada
                for (int i = 0; entrada[i]; i++) {
                    if (entrada[i] == ',') entrada[i] = '.';
                }

                float resultadoInfixa = getValorInFixa(entrada);
                if (isnan(resultadoInfixa) || isinf(resultadoInfixa)) {
                    printf(">> ERRO: Nao foi possivel calcular o valor da expressao.\n");
                } else {
                    if (fabs(resultadoInfixa - (int)resultadoInfixa) < 0.00001) {
                        printf("Resultado: %d\n", (int)resultadoInfixa);
                    } else {
                        printf("Resultado: %.2f\n", resultadoInfixa);
                    }
                }
                break;

            case 4:
                printf("Digite a expressao pos-fixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';

                float resultadoPosfixa = getValorPosFixa(entrada);
                if (isnan(resultadoPosfixa) || isinf(resultadoPosfixa)) {
                    printf(">> ERRO: Nao foi possivel calcular o valor da expressao.\n");
                } else {
                    if (fabs(resultadoPosfixa - (int)resultadoPosfixa) < 0.00001) {
                        printf("Resultado: %d\n", (int)resultadoPosfixa);
                    } else {
                        printf("Resultado: %.2f\n", resultadoPosfixa);
                    }
                }
                break;

            case 0:
                printf("Encerrando o programa.\n");
                return 0;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}