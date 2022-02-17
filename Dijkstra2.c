#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FLSH gets(l)

/* definir variavies */
int destino, origem, vertices = 0;
int custo, *custos = NULL;

void dijkstra(int vertices,int origem,int destino,int *custos, FILE *log)
{
  fprintf(log, "Função: Dijkstra\n");

  int i,v, cont = 0;
  int *ant, *tmp;
  int *z; /* vertices para os quais se conhece o caminho minimo */
  double min;
  double dist[vertices]; /* vetor com os custos dos caminhos */


  /* aloca as linhas da matriz */
  ant = calloc (vertices, sizeof(int *));
  tmp = calloc (vertices, sizeof(int *));
  if (ant == NULL) {
    printf ("** Erro: Memoria Insuficiente **");
    exit(-1);
  }

  z = calloc (vertices, sizeof(int *));
  if (z == NULL) {
    printf ("** Erro: Memoria Insuficiente **");
    exit(-1);
  }

  for (i = 0; i < vertices; i++) {
    if (custos[(origem - 1) * vertices + i] !=- 1) {
      ant[i] = origem - 1;
      dist[i] = custos[(origem-1)*vertices+i];
    }
    else {
      ant[i]= -1;
      dist[i] = HUGE_VAL;
    }
    z[i]=0;
  }
  z[origem-1] = 1;
  dist[origem-1] = 0;

  /* Ciclo principal */
  do {
    /* Encontra o vertice que deve entrar em z */
    min = HUGE_VAL;
    for (i=0;i<vertices;i++)
      if (!z[i])
        if (dist[i]>=0 && dist[i]<min) {
          min=dist[i];v=i;
        }

    /* Calcula distancias dos novos vizinhos de z */
    if (min != HUGE_VAL && v != destino - 1) {
      z[v] = 1;
      for (i = 0; i < vertices; i++)
        if (!z[i]) {
          if (custos[v*vertices+i] != -1 && dist[v] + custos[v*vertices+i] < dist[i]) {
            dist[i] = dist[v] + custos[v*vertices+i];
            ant[i] =v;
          }
        }
    }
  } while (v != destino - 1 && min != HUGE_VAL);

  /* Mostra o Resultado da procura */
  printf("\tDe %d para %d: \t", origem, destino);
  if (min == HUGE_VAL) {
    printf("Nao Existe\n");
    printf("\tCusto: \t- \n");
  }
  else {
    i = destino;
    i = ant[i-1];
    while (i != -1) {
      // printf("<-%d",i+1);
      tmp[cont] = i+1;
      cont++;
      i = ant[i];
    }

    for (i = cont; i > 0 ; i--) {
      printf("%d -> ", tmp[i-1]);
    }
    printf("%d", destino);

    printf("\n\tCusto: %d\n",(int) dist[destino-1]);
  }
}


void cabecalho(FILE *log)
{
  fprintf(log, "Função: Cabeçalho\n");

  printf("Implementacao do Algoritmo de Dijasktra\n");
  printf("Comandos:\n");
  printf("\t d - Adicionar Grafo\n"
  "\t r - Procura Os Menores Caminhos no Grafo\n"
  "\t CTRL+C para Sair do programa\n");
  printf("-------- ");
}

int reiniciaString(char *vet, FILE *log){
    fprintf(log, "Função: reiniciaString\n");
    for(int i = 0; i < 10; i++){
        *(vet+i) = NULL;
    }
    return 0; // retorna o indice inicial
}

void add(FILE *arqDigrafo, FILE *log)
{
  fprintf(log, "Função: add\n");

  char aresta[10];
  if(arqDigrafo == NULL)
    printf("Erro ao abrir arquivo!");

  int i, j, k;

  do {
    printf("\nQual o numero de vertices (numero minimo = 2 ): ");
    scanf("%d",&vertices);
  } while (vertices < 2 );

  char ver[2];
  ver[0] = vertices + '0';
  ver[1] = '\n';
  fputs(ver, arqDigrafo);
  fclose(arqDigrafo);

  if (!custos)
    free(custos);
  custos = (int *) malloc(sizeof(int)*vertices*vertices);
  for (i = 0; i <= vertices * vertices; i++)
    custos[i] = -1;

  k = 0; // Percorre a string
  printf("Insira as arestas:\n");
  do {
    arqDigrafo = fopen("digrafo.txt", "a");
    do {
      printf("Origem da aresta (entre 1 e %d ou '0' para sair): ", vertices);
      scanf("%d",&origem);
    } while (origem < 0 || origem > vertices);

    // __________________ Adicionando a aresta a string ________________________
    aresta[k] = origem + '0';
    k++;
    aresta[k] = ' ';
    k++;
    /* ________________________________________________________*/

    if (origem) {
      do {
        printf("Destino da aresta (entre 1 e %d, menos %d): ", vertices, origem);
        scanf("%d", &destino);
      } while (destino < 1 || destino > vertices || destino == origem);
      aresta[k] = destino + '0';
      k++;
      aresta[k] = ' ';
      k++;

      do {
        printf("Custo (positivo) da aresta do vertice %d para o vertice %d: ",
        origem, destino);
        scanf("%d",&custo);
      } while (custo < 0);
      aresta[k] = custo + '0';
      k++;
      aresta[k] = '\n';
      arqDigrafo = fopen("digrafo.txt", "a");
      fputs(aresta, arqDigrafo);
      printf("%s\n", aresta);
      fclose(arqDigrafo);

      custos[(origem-1) * vertices + destino - 1] = custo;

      //fprintf(arqDigrafo, "%s\n",aresta); // gravando string no arquivo

      k = reiniciaString(aresta, log);
    }
  } while (origem);

}

void procurar(FILE *log)
{
  fprintf(log, "Função: Procurar\n");

  int i, j;

  /* Azul */
  printf("Lista dos Menores Caminhos no Grafo Dado: \n");

  for (i = 1; i <= vertices; i++) {
    for (j = 1; j <= vertices; j++)
      dijkstra(vertices, i,j, custos, log);
    printf("\n");
  }

  printf("ENTER para voltar ao menu inicial>\n");
  /* Volta cor normal */
  printf("\033[m");
}

int main(int argc, char **argv) {
  int i, j;
  char opcao[3], l[50];
  FILE *arqDigrafo = fopen("digrafo.txt", "wt");
  FILE *log = fopen("logProcessamento.txt", "wt");

  do {
    cabecalho(log);
    fclose(log);
    scanf("%s", &opcao);

    if ((strcmp(opcao, "d")) == 0) {
      log = fopen("logProcessamento.txt", "a");
      add(arqDigrafo, log);
      fclose(log);
    }
    FLSH;

    if ((strcmp(opcao, "r") == 0) && (vertices > 0) ) {
      log = fopen("logProcessamento.txt", "a");
      procurar(log);
      fclose(log);
      FLSH;
    }
  } while (opcao != "x");

  printf("\nOver & Out\n\n");

  return 0;
}
