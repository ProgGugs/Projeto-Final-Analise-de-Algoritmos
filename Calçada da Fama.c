#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Define o tipo Camera para armazenar dois inteiros em uma única struct
    Representando o intervalo da calçada que uma determinada câmera cobre */
typedef struct Camera {
    int inicio;
    int fim;
} Camera;

/* Função que define um tipo de comparação realizada para ordenar o vetor de intervalos
    Nesse caso, ele é ordenado de forma crescente, primeiro considerando o inicio e depois o fim do intervalo */
int compara_Camera(const void *a, const void *b) {
    Camera ponteiro_a = *(Camera*) a;
    Camera ponteiro_b = *(Camera*) b;

    int inicio_a = ponteiro_a.inicio;
    int inicio_b = ponteiro_b.inicio;
    int fim_a = ponteiro_a.fim;
    int fim_b = ponteiro_b.fim;
    if ((inicio_a - inicio_b) == 0) {
        return (fim_a - fim_b);
    }
    return (inicio_a - inicio_b);
}

/* Realiza a mesclagem entre intervalos sobrepostos por meio de uma estratégia gulosa
    Ex1: [1,5] e [3,8] --> [1,8]
    Ex2: [3,9] e [9,18] --> [3,18] */
int merge(Camera *cameras, int tam) {

    // Caso o vetor esteja vazio, retorna 0
    if (tam == 0) {
        return 0;
    }

    // Indice do último intervalo mesclado (começando na posição inicial do vetor)
    int ultimo_mesclado = 0;

    /* Iteração pelo vetor para a mesclagem dos intervalos usando estratégia gulosa,
    sempre considerando a melhor escolha local de mesclar os intervalos subsequentes 
    com sobreposição já que a ordenação anterior nos permite garantir que isso nos 
    levará para a melhor escolha global (maior número de mesclagens possíveis) */
    for (int i = 1; i < tam; i++) {
        /* Caso base para a a mesclagem 
        (inicio do intervalo atual é menor ou igual a fim do intervalo anterior) */
       if (cameras[i].inicio <= cameras[ultimo_mesclado].fim) {

            /* Confirma se o fim do intervalo atual é maior do que o 
                fim do intervalo atual para fazer a mesclagem */
            if (cameras[ultimo_mesclado].fim < cameras[i].fim) {
                cameras[ultimo_mesclado].fim = cameras[i].fim;
            }

       } else {
            /* Se não for possível fazer a mesclagem, apenas acrescenta o intervalo atual no vetor */
            ultimo_mesclado++;
            cameras[ultimo_mesclado] = cameras[i];
       }
    }
    // Retorna o novo valor do tamanho do vetor depois das mesclagens
    return ultimo_mesclado+1;
}

// Realiza uma busca binária para encontrar o intervalo de câmera que contém a Estrela
bool busca_binaria(Camera *cameras, int tam, int estrela) {

    // Define a posição inicial e final do vetor
    int pos_inicial = 0;
    int pos_final = tam-1;

    while (pos_inicial <= pos_final) {

        /* Calcula a posição do meio do vetor por meio de uma
            divisão inteira da posição inicial e final por 2 */
        int meio = (pos_inicial+pos_final) / 2;

        /* Se a estrela é menor do que o inicio do intervalo localizado no meio
            continuamos a busca apenas da parte esquerda (meio-1) do vetor */
        if (estrela < cameras[meio].inicio) {
            pos_final = meio-1;

        /* Se a estrela é maior do que o fim do intervalo localizado no meio
            continuamos a busca apenas da parte direita (meio+1) do vetor */
        } else if (estrela > cameras[meio].fim) {
            pos_inicial = meio+1;
        }
        /* Se a estrela não é nem menor e nem maior do que o inicio e o fim do intervalo 
            localizado no meio, encontramos o intervalo que a estrela está localizada (true) */
        else {
            return true;
        }
    }

    /* Após percorrer todo o vetor e não encontrar um intervalo que a estrela poderia estar 
        é retornada false como resposta para essa estrela não coberta por câmeras*/
    return false;
}

// Número de casos teste do problema
int casos = 0;

int main() {
    scanf("%d", &casos);
    for (int i = 1; i <= casos; i++) {
        /* Definindo as variáveis de entrada
            n -> Tamanho da calçada
            c -> Quantidade de câmeras */ 
        int n = 0, c = 0;
        scanf("%d %d", &n, &c);

        // Criando o vetor de intervalos que as câmeras cobrem
        Camera cameras[c*2];
        int tam = 0;

        // Acrecentando os intervalor no vetor criado de acordo com cada entrada
        for (int j = 0; j < c; j++) {
            int a = 0, b = 0;
            scanf("%d %d", &a, &b);
            Camera camera = {a, b};
            cameras[tam] = camera;
            tam++;
        }

        // Ordenando o vetor com os intervalos das câmeras em ordem crescente
        qsort(cameras, tam, sizeof(Camera), compara_Camera);
        // Realizando a mesclagem entre os intervalos sobrepostos
        tam = merge(cameras, tam);

        // Lendo a quantidade de estrelas na calçada (e)
        int e = 0;
        scanf("%d", &e);

        // Definindo variávies de controle para as estrelas e resultado final
        int estrela = 0;
        int resultado = 0;

        // Lendo a posição de cada estrela na calçada
        for (int j = 0; j < e; j++) {
            scanf("%d", &estrela);
            // Se a estrela está dentro de algum intervalo de câmera, é adicionada no resultado final
            if (busca_binaria(cameras, tam, estrela)) {
               resultado++;
            }
        }
        // Impressão do resultado final da busca, para cada caso teste
        printf("Caso #%d: %d\n", i, resultado);
    }
    return 0;
}