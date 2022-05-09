#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// particaoN: particao do QuickSort padrão
// particaoA: particao do QuickSort aleatório
// particaoM: particao do QuickSort mediana

// Variáveis Globais
int tQ = 0, cQ = 0; // trocas e comparações QuickSort
int tM = 0, cM = 0; // trocas e comparações QuickSortMediana
int tA = 0, cA = 0; // trocas e comparações QuickSortAleatório

//############################################################################################################

int *leArquivo(char nomeArquivo[], int tam)
{
    int *vet;
    int i = 0;
    FILE *arq;

    arq = fopen(nomeArquivo, "r");

    if (NULL == arq)
    { // arquivo não abriu
        fclose(arq);
        printf("Erro arquvio");
        return 1;
    }

    if (fscanf(arq, "%d", &vet) == 0)
    { // leitura com erro
        // vet->error = 2;
        // fclose(arq);
        printf("Erro1");
        return 2;
    }

    if ((vet = (int *)malloc(tam * sizeof(int))) == NULL)
    { // problema de alocamento de memória
        fclose(arq);
        printf("Problema alocamento de memória para vetor");
        return 3;
    }

    while (!feof(arq))
    {
        fscanf(arq, "%d", &vet[i]);
        i++;
    }
    fclose(arq);
    return vet;
}

//########################################## MEDIANA #################################################

void swap(int A[], int i, int j)
{
    int temp = A[i];
    A[i] = A[j];
    A[j] = temp;
    tM++;
}

//####################################### ALEATÓRIO ##################################################

void swapA(int A[], int i, int j)
{
    int temp = A[i];
    A[i] = A[j];
    A[j] = temp;
    tA++;
}

int menorigualAleatorio(int a, int b)
{
    ++cA;
    return (a <= b);
}

int menorAleatorio(int a, int b)
{
    ++cA;
    return (a < b);
}

//#############################  QUICK SORT ALEATÓRIO #################################################

int particaoA(int *vet, int inicio, int fim)
{
    // sorteia um índice aleatório entre inicio e fim
    int k;
    double d;
    // Observação: rand() é uma função fraca para gerar números aleatórios
    d = (double)rand() / ((double)RAND_MAX + 1);
    k = d * (fim - inicio + 1);
    int randomIndex = inicio + k;
    // coloca o pivô aleatório no fim para aplicar a partição de Cormen
    swapA(vet, randomIndex, fim);
    //*******************ALGORITMO DE PARTIÇAO DE CORMEN*********************
    // o pivo é o elemento final
    int pivo = vet[fim];
    int i = inicio - 1;
    int j;
    /*
     * Este laço irá varrer os vetores da esquerda para direira
     * procurando os elementos que são menores ou iguais ao pivô.
     * Esses elementos são colocados na partição esquerda.
     */
    for (j = inicio; j <= fim - 1; j++)
    {
        if (menorigualAleatorio(vet[j], pivo))
        {
            /*troca = troca+1;
            printf("\n\n%d" , troca);*/
            i = i + 1;
            swapA(vet, i, j);
        }
    }
    // coloca o pivô na posição de ordenação
    swapA(vet, i + 1, fim);
    return i + 1; // retorna a posição do pivô
}

void quicksortAleatorizado(int *vet, int inicio, int fim)
{
    if (menorAleatorio(inicio, fim))
    {
        // realiza a partição
        int q = particaoA(vet, inicio, fim);
        // ordena a partição esquerda
        quicksortAleatorizado(vet, inicio, q - 1);
        // ordena a partição direita
        quicksortAleatorizado(vet, q + 1, fim);
    }
}

//################################## MEDIANA #########################################################

int encontra_menorMediana(int a, int b)
{
    cM++;
    return (a < b);
}

int menorigualMediana(int a, int b)
{
    cM++;
    return (a <= b);
}

//###############################################################################################################

int particaoM(int A[], int inicio, int fim)
{
    // procura a mediana entre inicio, meio e fim
    int meio = (inicio + fim) / 2;
    int a = A[inicio];
    int b = A[meio];
    int c = A[fim];
    int medianaIndice; //índice da mediana
    // A sequência de if...else a seguir verifica qual é a mediana
    if (encontra_menorMediana(a, b))
    {
        if (encontra_menorMediana(b, c))
        {
            // a < b && b < c
            medianaIndice = meio;
        }
        else
        {
            if (encontra_menorMediana(a, c))
            {
                // a < c && c <= b
                medianaIndice = fim;
            }
            else
            {
                // c <= a && a < b
                medianaIndice = inicio;
            }
        }
    }
    else
    {
        if (encontra_menorMediana(c, b))
        {
            // c < b && b <= a
            medianaIndice = meio;
        }
        else
        {
            if (encontra_menorMediana(c, a))
            {
                // b <= c && c < a
                medianaIndice = fim;
            }
            else
            {
                // b <= a && a <= c
                medianaIndice = inicio;
            }
        }
    }
    // coloca o elemento da mediana no fim para poder usar o Quicksort de Cormen
    swap(A, medianaIndice, fim);

    //*******************ALGORITMO DE PARTIÇÃO DE CORMEN*********************
    // o pivo é o elemento final
    int pivo = A[fim];
    int i = inicio - 1;
    int j;
    /*
     * Este laço irá varrer os vetores da esquerda para direira
     * procurando os elementos que são menores ou iguais ao pivô.
     * Esses elementos são colocados na partição esquerda.
     */
    for (j = inicio; j <= fim - 1; j++)
    {
        if (menorigualMediana(A[j], pivo))
        {
            i = i + 1;
            swap(A, i, j);
        }
    }
    // coloca o pivô na posição de ordenação
    swap(A, i + 1, fim);
    return i + 1; // retorna a posição do pivô
}

// Quicksort mediana de três
void quicksortMedianaDeTres(int A[], int inicio, int fim)
{
    if (encontra_menorMediana(inicio, fim))
    {
        // realiza a partição
        int mediana = particaoM(A, inicio, fim);
        // ordena a partição esquerda
        quicksortMedianaDeTres(A, inicio, mediana - 1);
        // ordena a partição direita
        quicksortMedianaDeTres(A, mediana + 1, fim);
    }
}

//################################################################################################################################

int encontra_menorII(int a, int b)
{
    cQ++;
    return (a < b);
}

int diferente(int a, int b)
{
    ++cQ;
    return (a != b);
}

//#############################################################################################################

int particaoP(int *vet, int inicio, int fim)
{
    int pivo = vet[inicio];
    int pos = inicio;
    int aux, i;

    for (i = inicio + 1; i <= fim; i++)
    {
        if (encontra_menorII(vet[i], pivo))
        {
            pos = pos + 1;
            if (diferente(i, pos))
            {
                aux = vet[i];
                vet[i] = vet[pos];
                vet[pos] = aux;
                ++tQ;
            }
        }
    }

    aux = vet[inicio];
    vet[inicio] = vet[pos];
    vet[pos] = aux;
    tQ++;

    return pos;
}

void quickSort(int *vet, int inicio, int fim)
{
    int pivo;
    if (encontra_menorII(inicio, fim))
    {
        pivo = particaoP(vet, inicio, fim);
        quickSort(vet, inicio, pivo - 1);
        quickSort(vet, pivo + 1, fim);
    }
}

//###################################################################################################################

void escolheSort(int *vet, int inicio, int fim)
{
    int escolhe, i, N;
    double start, end;
    printf("Escolha o Sort desejado: \n(1)Quick Sort:\n(2)Quick Sort Aleatorio:\n(3)Quick Sort Mediana\n");
    scanf("%d", &escolhe);
    switch (escolhe)
    {
    case 1:
        for (i = 0; i < 1; i++)
        {
            double cpu_time_used;
            start = clock();
            tQ = 0;
            cQ = 0;
            quickSort(vet, inicio, fim);
            end = clock();
            cpu_time_used = ((double)(end - start));
            double tempo = cpu_time_used/((CLOCKS_PER_SEC / 1000));
            printf("\n++++++++++++++++++++++++++++++++++++++++\n");
            printf("Quick Sort[%d]: %lf", i, tempo);
            printf("\nT = %d", tQ);
            printf("\nC = %d", cQ);
            resultados(tempo, tQ, cQ);
        }
        break;

    case 2:
        for (i = 0; i < 1; i++)
        {
            double cpu_time_used;
            start = clock();
            quicksortAleatorizado(vet, inicio, fim);
            end = clock();
            cpu_time_used = ((double)(end - start));
            double tempo = cpu_time_used/((CLOCKS_PER_SEC / 1000));
            printf("\n++++++++++++++++++++++++++++++++++++++++\n");
            printf("Quick Sort[%d]: %lf", i, tempo);
            printf("\nT = %d", tA);
            printf("\nC = %d", cA);
            resultados(tempo, tQ, cQ);
        }
        break;

    case 3:
        for (i = 0; i < 1; i++)
        {
            double cpu_time_used;
            start = clock();
            quicksortMedianaDeTres(vet, inicio, fim);
            end = clock();
            cpu_time_used = ((double)(end - start));
            double tempo = cpu_time_used/((CLOCKS_PER_SEC / 1000));
            printf("\n++++++++++++++++++++++++++++++++++++++++\n");
            printf("Quick Sort[%d]: %lf", i, tempo);
            printf("\nT = %d", tM);
            printf("\nC = %d", cM);
            resultados(tempo, tQ, cQ);
        }
        break;
    }
}

//########################################################################################################################

int printArquivo(int qtd,int *num){
 FILE *arq;
	int j;
  arq=fopen("vetor-ordenado","w");
  if(arq==NULL){
    return 1;
    }
  else
    for(j=0;j<qtd;j++){
      fprintf(arq,"%d, ",num[j]);
    }
    fclose(arq);
}

//########################################################################################################################

int resultados(double tempo,int tQ,int cQ){
 FILE *arq;
	int j;
  arq=fopen("resultados","a");
  if(arq==NULL){
    return 1;
    }
  else
    fprintf(arq,"\nTempo: %f\nTrocas: %d \nComparacoes: %d\n",tempo,tQ, cQ);
	fclose(arq);
}

//########################################################################################################################

int main()
{
    int *vet, N;
    char nomeArquivo[30];

    printf("Tamanho do arquivo: ");
    scanf("%d", &N);
    // printf("\nNome do arquivo: ");
    // scanf(" %s", nomeArquivo);

    vet = leArquivo("10.000 - 1", N);
    escolheSort(vet, 0, N);
    printArquivo(N, vet);

    // printf("%d %d", c, t);
    free(vet);
    return 0;
}
