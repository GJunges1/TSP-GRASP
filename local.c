/**
 * TAREFA 2 (Busca Local)
 * 
 * Problema: Caixeiro viajante simétrico (STSP)
 * Heurística: Método Guloso + Busca Local
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

// Função que imprime a rota atual
void printRoute(int* route, char* legend)
{
	if(legend==NULL)
		return;
	int i;
	printf("<details><summary> %s</summary><blockquote>\n",legend);

	// Exibindo a rota encontrada
	for(i=0;i<(city_num+1);i++)
	{
		if(i%10==0) printf("\n");
		printf("%d ",route[i]);
	}
	printf("\n");
	printf("</blockquote></details>\n");
}
int* greedySearch(float **tsp)
{
	int* route = (int*) malloc(sizeof(int)*(city_num+1)); // criando vetor para a rota (vetor "s")
	if(route==NULL)
	{
		printf("ERRO: malloc com problema!\n");
		return NULL;
	}
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
	route[0]=1;

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
				route[counter+1] = j + 1;
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
			visitedRouteList[route[counter+1] - 1] = 1;
			j = 0;
			i = route[counter+1] - 1;
			counter++;
		}
	}

	// Adicionando a primeira cidade como sendo a última, e somando sua distância

	sum += tsp[0][route[counter]-1];
	route[counter+1]=1;

	// Exibindo o custo calculado	
	printRoute(route,"GREEDY ROUTE");
	printf("<p>GREEDY COST: %.2f\n", sum);
	return route;
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

// Função que marca o tempo de execução da função GREEDYSEARCH:
float elapsed_time=0;
int* stopwatch1arg(int* (*function)(), void* arg){
	clock_t start, end;
	int* func_return;
	arg = (float**) arg;
	start = clock();
	func_return = (int*)function(arg);
	end = clock();
	elapsed_time = (float) (end-start)/CLOCKS_PER_SEC;
	return func_return;
}

// Função que marca o tempo de execução da função LOCALSEARCH:
void stopwatch2arg(void (*function)(), void* arg1, void* arg2){
	clock_t start, end;
	arg1 = (int*) arg1;
	arg2 = (float**) arg2;
	start = clock();
	function(arg1,arg2);
	end = clock();
	elapsed_time = (float) (end-start)/CLOCKS_PER_SEC;
}

// Função que inverte os elementos de um vetor entre os índices 'start' e 'end'
void reverse(int* route, int start, int end)
{
	int i, j, d = end - start, aux;
	for(i=0;i<d/2;i++)
	{
		aux = route[start+i];
		route[start+i]=route[end-i];
		route[end-i]=aux;
	}
	if(d%2==1){
		aux = route[start+d/2];
		route[start+d/2]=route[end-d/2];
		route[end-d/2]=aux;
	}
}

// Função que realiza a troca de vértices de uma rota (2-opt)
void do2Opt(int* route, int i, int j)
{
	reverse(route, i + 1, j + 1);
}


// Função que calcula e retorna o custo total da rota atual
float pathLength(int* route, float** tsp)
{
	if(city_num<=1) return 0;
	int i;
	float length = 0;
	for(i=0;i<city_num;i++)
	{
		length += tsp[route[i]-1][route[i+1]-1];
	}
	return length;
}


// Função para a implementação da busca local
void localSearch(int* route, float** tsp)
{
	// vector<Point> path = ...a vector of x,y points...; // The starting vertex is not included at the end
	float curLength = pathLength(route, tsp);                // Squared length of the entire path, including the distance from last vertex to the first
	// printf("%.2f\n",curLength);
	int foundImprovement = 1;
	float lengthDelta=0;
	while (foundImprovement)
	{
		foundImprovement = 0;

		for(int i=0; i < city_num-2; i++) {
			for(int j=i+1; j < city_num-1; j++) {
				lengthDelta = - tsp[route[i]-1][route[i+1]-1] - tsp[route[j+1]-1][route[j+2]-1] \
								+ tsp[route[i]-1][route[j+1]-1]+ tsp[route[i+1]-1][route[j+2]-1];

				// If the length of the path is reduced, do a 2-opt swap
				if (lengthDelta < -1e-4f) {
					// printf("%.2f\n%d %d %d %d\n%d %d %d %d\n\n",lengthDelta,i,i+1,j,j+1,i,j,i+1,j+1);
					do2Opt(route, i, j);
					curLength += lengthDelta;
					// printf("[%.2f ; %.2f] ",lengthDelta,curLength);
					foundImprovement = 1;
				}
			}
		}
	}

	printRoute(route,"GREEDY + LOCAL SEARCH ROUTE");
	printf("<p>LOCAL SEARCH COST: %.2f\n", curLength);

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
	int* route = (int*) stopwatch1arg(greedySearch,tsp);

	printf("<br>GREEDY ELAPSED TIME (s): %e\n",elapsed_time);
	
	stopwatch2arg(localSearch, route, tsp);

	printf("<br>LOCAL SEARCH ELAPSED TIME (s): %e\n",elapsed_time);

	// Liberando a memória utilizada
    for(i=0;i<city_num;i++)
    {
        free(coordinates[i]);
        free(tsp[i]);
    }
    free(coordinates);
    free(tsp);
	free(route);
	
    return 0;

}