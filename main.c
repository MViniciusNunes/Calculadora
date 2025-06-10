#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

int main() {
    int opcao;
    char entrada[512];

    while (1) {
        printf("\n==== MENU ====\n");
        printf("1 - Traduzir expressao infixada -> pos-fixada\n");
        printf("2 - Traduzir expressao pos-fixada -> infixada\n");
        printf("3 - Avaliar expressao infixada\n");
        printf("4 - Avaliar expressao pos-fixada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        char buffer[10];
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite a expressao infixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';
                
                char *posFixa = getFormaPosFixa(entrada);
                if (posFixa == NULL) {
                    printf("Nao foi possivel traduzir a expressao.\n");
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
                    printf("Nao foi possivel traduzir a expressao.\n");
                } else {
                    printf("Expressao infixada: %s\n", inFixa);
                }
                break;

            case 3:
                printf("Digite a expressao infixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';
                
                float resultadoInfixa = getValorInFixa(entrada);
                if (isnan(resultadoInfixa) || isinf(resultadoInfixa)) {
                    printf("Nao foi possivel calcular o valor da expressao.\n");
                } else {
                    printf("Resultado: %f\n", resultadoInfixa);
                }
                break;

            case 4:
                printf("Digite a expressao pos-fixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';

                float resultadoPosfixa = getValorPosFixa(entrada);
                if (isnan(resultadoPosfixa) || isinf(resultadoPosfixa)) {
                    printf("Nao foi possivel calcular o valor da expressao.\n");
                } else {
                    printf("Resultado: %f\n", resultadoPosfixa);
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