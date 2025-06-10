#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressao.h"

int main() {
    int opcao;
    char entrada[512];

    while (1) {
        printf("\n==== MENU ====\n");
        printf("1 - Converter expressao infixada -> pós-fixada\n");
        printf("2 - Converter expressao pos-fixada -> infixada\n");
        printf("3 - Avaliar expressao infixada\n");
        printf("4 - Avaliar expressao pos-fixada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o \n do buffer

        switch (opcao) {
            case 1:
                printf("Digite a expressão infixada (use espaços entre os elementos):\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0'; // Remove o \n do final
                printf("Expressão pós-fixada: %s\n", getFormaPosFixa(entrada));
                break;

            case 2:
                printf("Digite a expressão pós-fixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';
                printf("Expressão infixada: %s\n", getFormaInFixa(entrada));
                break;

            case 3:
                printf("Digite a expressão infixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';
                printf("Expressão pós-fixada: %s\n", getFormaPosFixa(entrada));
                printf("Resultado: %.6f\n", getValorInFixa(entrada));
                break;

            case 4:
                printf("Digite a expressão pós-fixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';
                printf("Expressão infixada: %s\n", getFormaInFixa(entrada));
                printf("Resultado: %.6f\n", getValorPosFixa(entrada));
                break; 

            case 0:
                printf("Encerrando o programa.\n");
                return 0;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
