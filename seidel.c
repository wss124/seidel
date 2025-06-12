#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 3             // Ordem do sistema
#define TOL 0.0001      // Tolerância para o critério de parada
#define MAX_IT 1000     // Número máximo de iterações
#define SHOW_ITER 1     // 1 para mostrar iterações, 0 para ocultar

void lerDados(double A[N][N], double b[N], double x0[N]) {
    FILE *fp = fopen("entrada.txt", "r");
    if (!fp) {
        printf("\nErro ao abrir o arquivo 'entrada.txt'.\n");
        printf("Certifique-se que:\n");
        printf("- O arquivo existe no mesmo diretorio do executavel\n");
        printf("- O nome esta correto (entrada.txt)\n");
        printf("- O arquivo contem os dados no formato especificado\n\n");
        exit(1);
    }

    // Lê a matriz A
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fscanf(fp, "%lf", &A[i][j]) != 1) {
                printf("Erro ao ler elemento A[%d][%d]\n", i, j);
                fclose(fp);
                exit(1);
            }
        }
    }

    // Lê o vetor b
    for (int i = 0; i < N; i++) {
        if (fscanf(fp, "%lf", &b[i]) != 1) {
            printf("Erro ao ler elemento b[%d]\n", i);
            fclose(fp);
            exit(1);
        }
    }

    // Lê o vetor x0 (chute inicial)
    for (int i = 0; i < N; i++) {
        if (fscanf(fp, "%lf", &x0[i]) != 1) {
            x0[i] = 0.0; // Valor padrão se não fornecido
        }
    }

    fclose(fp);
}

void gauss_seidel(double A[N][N], double b[N], double x0[N]) {
    double x[N], x_ant[N];
    double erro;
    int iter;

    // Inicializa x com x0
    for (int i = 0; i < N; i++) {
        x[i] = x0[i];
        x_ant[i] = x0[i];
    }

    printf("\n=== METODO DE GAUSS-SEIDEL ===\n");
    printf("Tolerancia: %.6f\n", TOL);
    printf("Maximo de iteracoes: %d\n\n", MAX_IT);
    
    if (SHOW_ITER) {
        printf("Iteracao | Valores de x           | Erro Maximo\n");
        printf("---------|------------------------|------------\n");
    }

    for (iter = 1; iter <= MAX_IT; iter++) {
        erro = 0;
        
        for (int i = 0; i < N; i++) {
            double soma = 0;
            
            // Usa os valores já atualizados (característica do Seidel)
            for (int j = 0; j < i; j++) {
                soma += A[i][j] * x[j];
            }
            
            // Usa os valores ainda não atualizados
            for (int j = i + 1; j < N; j++) {
                soma += A[i][j] * x_ant[j];
            }
            
            x[i] = (b[i] - soma) / A[i][i];
            
            // Calcula erro para esta variável
            double diff = fabs(x[i] - x_ant[i]);
            if (diff > erro) {
                erro = diff;
            }
        }

        if (SHOW_ITER) {
            printf("%7d | ", iter);
            for (int i = 0; i < N; i++) {
                printf("x%d=%.6f ", i, x[i]);
            }
            printf("| %.6f\n", erro);
        }

        // Atualiza x_ant para próxima iteração
        for (int i = 0; i < N; i++) {
            x_ant[i] = x[i];
        }

        if (erro < TOL) {
            break;
        }
    }

    // Exibe resultado final
    printf("\n=== RESULTADO FINAL ===\n");
    printf("Iteracoes realizadas: %d\n", iter);
    printf("Erro final: %.6f\n", erro);
    printf("\nSolucao encontrada:\n");
    for (int i = 0; i < N; i++) {
        printf("x[%d] = %.6f\n", i, x[i]);
    }

    if (iter >= MAX_IT) {
        printf("\nATENCAO: Maximo de iteracoes atingido!\n");
        printf("O metodo pode nao ter convergido.\n");
    } else {
        printf("\nConvergencia alcancada com sucesso!\n");
    }
}

int main() {
    double A[N][N], b[N], x0[N] = {0};

    printf("\n========================================");
    printf("\nMETODO DE GAUSS-SEIDEL - SISTEMAS LINEARES %dx%d", N, N);
    printf("\n========================================\n");
    
    lerDados(A, b, x0);
    
    printf("\nDados lidos com sucesso do arquivo 'entrada.txt'\n");
    gauss_seidel(A, b, x0);

    printf("\n========================================\n");
    return 0;
}