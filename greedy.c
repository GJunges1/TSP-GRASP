/**
 * TAREFA 1
 * 
 * Problema: Caixeiro viajante simétrico (STSP)
 * Heurística: Método guloso
 * Aluno: Gabriel Junges Baratto
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int city_num=0;

// Função para o cálculo da distância euclidiana 2D
double euc_2d_distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

// Função que realiza a busca gulosa
void greedySearch(float **tsp)
{
	// declarando variáveis

	float sum = 0;
	int counter = 0;
	int j = 0, i = 0;
	float min = (float)INT_MAX; // inicializando o mínimo como máximo
	int visitedRouteList[city_num];

	// inicializando vetor de rotas visitadas com zeros
	for(i=0;i<city_num;i++)
		visitedRouteList[i]=0;

	// começando do(a) primeiro(a) índice/cidade, "0"
	// declarando ele(a) como visitado(a)
	visitedRouteList[0] = 1;
	int route[city_num]; // criando vetor para a rota (vetor "s")

	// Percorrendo a matriz de adjacência
	// matriz tsp[][]
	i=0;
	while (i < city_num && j < city_num && counter < city_num - 1)
	{ // enquanto houverem cidades(vértices) inexploradas

		// Se esse caminho não foi visitado
		// então se o custo é menor que o mín-
		// imo então atualize o custo e salve
		// o vértice na rota
		if (j != i && (visitedRouteList[j] == 0))
		{
			if (tsp[i][j] < min)
			{
				min = tsp[i][j];
				route[counter] = j + 1;
			}
		}
		j++; // verificando próximo vértice

		// Se checou-se todos os caminhos
		// partindo-se do iésimo vértice
		// então soma-se o custo e parta
		// para o proximo vértice
		if (j == city_num)
		{
			sum += min;
			min = (float)INT_MAX;
			visitedRouteList[route[counter] - 1] = 1;
			j = 0;
			i = route[counter] - 1;
			counter++;
		}
	}

	// Adicionando a primeira cidade como sendo a última, e somando sua distância

	sum += tsp[0][route[counter-1]-1];
	route[counter]=1;

	// Exibindo o custo calculado	
	printf("MINIMUN COST IS : %d\n", (int)sum);

	// Exibindo a rota encontrada
	printf("GREEDY ROUTE:\n");
	for(i=0;i<city_num;i++)
	{
		printf("%d ",route[i]);
	}
	printf("\n");
}

// Função para calcular distâncias euclidianas 2D:
void calculateEuc2dDistances(float **coordinates,float **tsp)
{
	int i, j;
    for(i=0;i<city_num;i++)
        for(j=0;j<city_num;j++)
            tsp[i][j] = euc_2d_distance(coordinates[i][0],
                                coordinates[i][1],
                                coordinates[j][0],
                                coordinates[j][1]);
}

// Função que marca o tempo de execução de funções:
float stopwatch(void (*function)(), void* arg){
	clock_t start, end;
	arg = (float**) arg;
	start = clock();
	function(arg);
	end = clock();
	return (float) (end-start)/CLOCKS_PER_SEC;
}

int main()
{
	// OBS.: Este código foi preparado para receber instancias
	// de problemas da TSPLIB, mais especificamente problemas
	// que trabalhem com pontos euclidianos 2D

	// Lendo o número de cidades e pulando para as coordenadas:
	scanf("%*[^\n]\n%*[^\n]\n%*[^\n]\n"); // 			pulando três linhas...
	scanf("%*[^0-9]%d",&city_num); //				*** LENDO O NÚMERO DE CIDADES ***
	scanf("%*[^0-9]%*d%*[^0-9]"); // 					pulando três linhas...

	// Inicializando variáveis
    int i, j;
    float x, y;
    float **coordinates = (float**)malloc(city_num * sizeof(float*));
    float **tsp = (float**)malloc(city_num * sizeof(float*));
	if(coordinates==NULL||tsp==NULL)
	{
		printf("ERRO: malloc com problema!\n");
		return -1;
	}
    for(i=0;i<city_num;i++)
    {
        coordinates[i] = (float*) malloc(2 * sizeof(float));
        tsp[i] = (float*) malloc(city_num * sizeof(float));
		if(coordinates[i]==NULL||tsp[i]==NULL)
		{
			printf("ERRO: malloc com problema!\n");
			return -1;
		}	
    }

	// lendo as cordenadas euclidianas de entrada
    while(scanf(" %d %f %f", &i, &x, &y)!=0)
    {
        coordinates[i-1][0] = x;
        coordinates[i-1][1] = y;
    }

	// Populando a matriz de adjacência/distância
    calculateEuc2dDistances(coordinates, tsp);

	// Executando o algoritmo guloso
	float elapsed_time;
	elapsed_time = stopwatch(greedySearch,tsp);

	printf("ELAPSED TIME (s): %e\n",elapsed_time);

	// Liberando a memória utilizada
    for(i=0;i<city_num;i++)
    {
        free(coordinates[i]);
        free(tsp[i]);
    }
    free(coordinates);
    free(tsp);
	
    return 0;

}