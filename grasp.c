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
#include <sodium/randombytes.h>

int city_num=0;


int* listNonVisitedIdxs(int *tsp_idxs_i,int *visitedIdxs,int n_unvisited)
{
	int l=0;
	int n = n_unvisited;
	int *nonVisited = malloc(n*sizeof(int));
	int i,j,k;
	j=0;
	for(i=0;i<city_num;i++)
	{
		k = tsp_idxs_i[i];
		if(!visitedIdxs[k])
		{
			nonVisited[j++]=k;
		}
	}
	return nonVisited;
}


// Função para o cálculo da distância euclidiana 2D
double euc_2d_distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

// Função que realiza a busca gulosa
void printRoute(int* route, char* legend)
{
	if(legend==NULL)
		return;
	int i;
	printf("<details><summary> %s</summary><blockquote>\n",legend);

	// Exibindo a rota encontrada
	for(i=0;i<(city_num+1);i++)
	{
		// if(i%10==0) printf("\n");
		printf("%d ",route[i]);
	}
	printf("\n");
	printf("</blockquote></details>\n");
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(float arr[], int idxs[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    /* create temp arrays */
    int *L, *R;
	L = malloc(n1*sizeof(int));
	R = malloc(n2*sizeof(int));
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = idxs[l + i];
    for (j = 0; j < n2; j++)
        R[j] = idxs[m + 1 + j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (arr[L[i]] <= arr[R[j]]) {
            idxs[k] = L[i];
            i++;
        }
        else {
            idxs[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        idxs[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        idxs[k] = R[j];
        j++;
        k++;
    }
	free(L);
	free(R);
}
 
/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(float arr[],int idxs[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort(arr, idxs, l, m);
        mergeSort(arr, idxs, m + 1, r);
 
        merge(arr, idxs, l, m, r);
    }
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
float elapsed_time=0;
int* stopwatch1arg(int* (*function)(), void* arg1, void *arg2){
	clock_t start, end;
	int* func_return;
	arg1 = (float**) arg1;
	arg2 = (int**) arg2;
	start = clock();
	func_return = (int*)function(arg1,arg2);
	end = clock();
	elapsed_time = (float) (end-start)/CLOCKS_PER_SEC;
	return func_return;
}
void stopwatch2arg(void (*function)(), void* arg1, void* arg2){
	clock_t start, end;
	arg1 = (int*) arg1;
	arg2 = (float**) arg2;
	start = clock();
	function(arg1,arg2);
	end = clock();
	elapsed_time = (float) (end-start)/CLOCKS_PER_SEC;
}


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
void do2Opt(int* route, int i, int j)
{
	reverse(route, i + 1, j + 1);
}

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

float currentLength=0;
int countOnes(int* vetor)
{
	int i;
	for(i=0;i<city_num;i++)
		if(vetor[i])
			return i;
	return -1;
}

int grand(int *unvisited,int n_unvisited,float alpha)
{
	int r = floor((float)n_unvisited*alpha);
	if(r<0) r=0;
	return randombytes_uniform(r);
}

int* graspConstructionAlgorithm(float **tsp, int **tsp_idxs)
{
	// declarando variáveis

	float sum = 0;
	int counter = 0;
	int j = 0, i = 0;
	int n_unvisited=city_num;
	int *unvisited_tsp_idx_i=NULL;
	float alpha = 1;
	float min = (float)INT_MAX; // inicializando o mínimo como máximo
	int* visitedRouteList = malloc(sizeof(int)*city_num);

	// inicializando vetor de rotas visitadas com zeros
	for(i=0;i<city_num;i++){
		visitedRouteList[i]=0;
	}
	// começando do(a) primeiro(a) índice/cidade, "0"
	// declarando ele(a) como visitado(a)
	visitedRouteList[0] = 1;
	n_unvisited--;
	int* route = malloc(city_num*sizeof(int)); // criando vetor para a rota (vetor "s")

	// Percorrendo a matriz de adjacência
	// matriz tsp[][]
	i=0;
	int l=0;
	route[0]=1;
	counter++;
	while(n_unvisited>0)
	{
		unvisited_tsp_idx_i = listNonVisitedIdxs(tsp_idxs[i],visitedRouteList,n_unvisited);
		j = grand(unvisited_tsp_idx_i,n_unvisited,alpha);
		j = unvisited_tsp_idx_i[j];
		sum+= tsp[i][j];
		route[counter++] = j + 1;
		visitedRouteList[j]=1;
		n_unvisited--;
		i = j;
		free(unvisited_tsp_idx_i);
	}
	
	// Adicionando a primeira cidade como sendo a última, e somando sua distância

	sum += tsp[0][route[counter-1]-1];
	route[counter]=1;

	// Exibindo o custo calculado	
	printRoute(route,"GREEDY ROUTE");
	printf("<p>GREEDY COST: %.2f\n", sum);

	free(visitedRouteList);
	return route;
}

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

	// printRoute(route,"GREEDY + LOCAL SEARCH ROUTE");
	// printf("<p>LOCAL SEARCH COST: %.2f\n", curLength);
	currentLength = curLength;
}

void initTspIdxs(int **tsp_idxs, float **tsp){
	int i,j;
	for(i=0;i<city_num;i++)
	{
		for(j=0;j<city_num;j++)
		{
			tsp_idxs[i][j]=j;
		}
		mergeSort(tsp[i],tsp_idxs[i],0,city_num-1);
	}
	// for(i=0;i<city_num;i++)
	// {
	// 	for(j=0;j<city_num;j++)
	// 	{
	// 		printf("%.0f ",tsp[i][tsp_idxs[i][j]]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n");
	return;
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
	int **tsp_idxs = (int**)malloc(city_num*sizeof(int*));
	if(coordinates==NULL||tsp==NULL||tsp_idxs==NULL)
	{
		printf("ERRO: malloc com problema!\n");
		return -1;
	}
    for(i=0;i<city_num;i++)
    {
        coordinates[i] = (float*) malloc(2 * sizeof(float));
        tsp[i] = (float*) malloc(city_num * sizeof(float));
		tsp_idxs[i] = (int*) malloc(city_num*sizeof(int));
		if(coordinates[i]==NULL||tsp[i]==NULL||tsp_idxs[i]==NULL)
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
	initTspIdxs(tsp_idxs,tsp);

	double total_time=0;
	double total_length=0;
	float best_length=(float)INT_MAX;
	float best_time=0;
	float time_aux=0;
	int* route;
	for(i=0;i<100;i++)
	{
		time_aux=0;
		// Executando o algoritmo grasp
		route = (int*) stopwatch1arg(graspConstructionAlgorithm,tsp,tsp_idxs);
		time_aux += (double)elapsed_time;
		// Executando o algoritmo busca local
		stopwatch2arg(localSearch, route, tsp);
		total_time += (double)time_aux + (double)elapsed_time;
		total_length += (double)currentLength;
		if(currentLength<best_length)
		{
			best_length = currentLength;
			best_time = time_aux + elapsed_time;
		}
	}
	float mean_time = (float)(total_time/100.0);
	float mean_length = (float)(total_length/100.0);

	printf("<p>GRASP BEST COST: %.2f\n", best_length);
	printf("<br>GRASP MEAN COST: %.2f\n", mean_length);
	printf("<br>GRAP MEAN ELAPSED TIME (s): %e\n",mean_time);
	printf("<br>TOTAL ELAPSED TIME (s): %e\n",total_time);

	// Liberando a memória utilizada
    for(i=0;i<city_num;i++)
    {
        free(coordinates[i]);
        free(tsp[i]);
		free(tsp_idxs[i]);
    }
    free(coordinates);
    free(tsp);
	free(route);
	free(tsp_idxs);
	
    return 0;

}