#ifndef GRAFOS_H
#define GRAFOS_H

#include "busca_largura.h"

typedef struct Vertice TipoVertice;
typedef struct Aresta TipoAresta;
typedef struct estrela TipoEstrela;
typedef struct grafo Grafo;

struct estrela{
    /*ponteiro referencia o proximo elemento na lista*/
    TipoEstrela   *prox;
    /*ponteiro para as respectivas arestas conectadas ao vertice*/
    TipoAresta    *aresta;
};

struct Vertice {
    /*identificador do vertice*/
    int           id;
    /*ponteiro referencia o proximo elemento na lista*/
    TipoVertice   *prox;
    /*ponteiro para a lista de conexoes do vertice*/
    TipoEstrela   *estrela;
    
    Cores         cor;
};

struct Aresta {
    /*identificador da aresta*/
    int             id;
    /*ponteiro referencia o proximo elemento na lista*/
    TipoAresta   *prox;
    /*ponteiro para o endereco do vertice de partida da aresta*/
    TipoVertice   *origem;
    /*ponteiro para o endereco do vertice de chegada da aresta*/
    TipoVertice   *destino;
    
    int visitada; 
};

/*estrutura do grafo*/
struct grafo {
    /*vertices e arestas no grafo*/
    int             numVertices;
    int             numArestas;
    
    /*identificadores do ultimo vertice e aresta, respectivamente*/
    int             sementeVertices;
    int             sementeArestas; 
    
  
    /*ponteiro para as Listas de arestas e vertices, respectivamente*/
    TipoAresta    *pa;
    TipoVertice   *pv;
};

/*  v - indica a quantidade de vertices
 *  a - indica a quantidade de arestas
 * G - tipo grafo
 * V - tipo vertice
 * A - tipo aresta
 * B - tipo booleano
 * I - tipo inteiro
 * F - tipo numero real*/
/*------------------------------------------------------------------------------
 * ----------------------OPERACOES DO GRAFO-------------------------------------
 * ---------------------------------------------------------------------------*/
/*Cria um grafo
 na entrada 'a' e 'v' tem de ser maior de 0
 na saida 'G' deve ter memoria para o numero especificado de arestas 
 e vertices */
Grafo* GGcriagrafo();

/*Destroi o grafo indicado
 na entrada e necessario passar o ponteiro do grafo
 na saida liberar a memoria de G (do grafo destruido)*/
void GGdestroiGrafo(Grafo* p);

/*Cria vertice
 na entrada e necessario informar o grafo de destino
 na saida retornar o indentificador do vertice*/
int GVcriaVertice(Grafo* p);

/*Destroi vertice
na entrada e necessario informar o ponteiro para o grafo e ID do vertice
 na saida retornar identificador nulo*/
int GVdestroiVertice(Grafo *p, int v);

/*Cria aresta entre dois vertices de um grafo
 na entrada e necessario informar o grafo e os vertices que vao ser ligados
 na saida retornar o identificador da aresta*/
int GAcriaAresta(Grafo* p, int v1, int v2);

/*Destroi a aresta
 na entrada e necessario informar o ponteiro pro grafo e ID da aresta
 na saida retirnar identificador nulo*/
int GAdestroiAresta(Grafo *p, int a);

/*Verifica se o vertice informado existe no grafo
 na entrada e necessario informar o grafo e o identificador do vertice
 na saida deve retornar 1 caso o vertice exista ou 0 caso contrario*/
int GBexisteIdVertice(Grafo* p, int v);

/*Verifica se a aresta informada existe no grafo
 na entrada e necessario informar o grafo e o identificador da aresta
 na saida deve retornar 1 caso a aresta exista ou 0 caso contrario*/
int GBexisteIdAresta(Grafo* p, int a);

/*Verifica se o grafo possui aresta dirigida aos vertives especificados
 na entrada e necessario informar o grafo e os identificadores de arestas
 na saida deve retornar 1 no caso existir aresta dirigida ou 0 caso contrario*/
int GBexisteArestaDir(Grafo* p, int v1, int v2);

/*Verifica se a aresta e nao dirigida de v1 a v2
 na entrada e necessario informar o grafo e os identificadores de arestas
 na saida deve retornar 1 caso nao seja dirigida ou 0 caso contrario*/
int GBexisteAresta(Grafo* p, int v1, int v2);

/*Retorna o identificador de uma aresta dirigida
na entrada e necessario informar o grafo, vertice de saida e o de destino
na saida deve retornar o identificador da aresta ou 0 caso na exista*/
int GApegaArestaDir(Grafo* p, int v1, int v2);

/*Retorna o identificador da aresta nao dirigida requisitada
 na entrada e necessario informar o grafo e os identificadores dos vertices
 interligados pela aresta
 na saida deve retornar o identificador da aresta ou 0 caso nao exista*/
int GApegaAresta(Grafo* p, int v1, int v2);

/*
 Busca vertice na lista de vertices
 atraves de seu ID
 
 _vertices      ponteiro para o tipo ListaVertices
 _v             inteiro ID do vertice
 
 Retorna o identificador do vertice encontrado
 Ou NULL caso o vertice nao existir*/
TipoVertice *GVpegaVertice(TipoVertice *vertices, int v);

/*Retorna  o identificador do primeiro vertice criado no grafo
 *na entrada e necessario informar o grafo
 *na saida deve retornar o identificador do primeiro vertice encontrado*/
int GVprimeiroVertice(Grafo* p);

/*Retorna o proximo vertice a partir do vertice informado
 na entrada e necessario informar o grafo e o vertice inicial
 na saida e informado o vertice sucessor de "v1"*/
int GVproximoVertice(Grafo* p, int v1);

/*Retorna o identificador da primeira aresta do grafo
 na entrada deve ser informado o grafo
 na saida deve retornar o identificador da aresta ou 0 caso contrario*/
int GAprimeiraAresta(Grafo* p);

/*Retorna o identificador da segunda aresta do grafo
na entrada deve ser informado o grafo e a primeira aresta encontrada
na saida retorna o identificador da segunda aresta no grafo*/
int GAproximaAresta(Grafo* p, int a1);

/*Retorna a quantidade de vertices que o grafo informado possui*/
int GInumeroVertices(Grafo* p);

/*Retorna a quantidade maxima de vertices que o grafo informado comporta*/
int GInumeroVerticesMax(Grafo* p);

/*Retorna a quantidade de arestas que o grafo informado possui*/
int GInumeroArestas(Grafo* p);

/*Retorna a quantidade de arestas que o grafo informado comporta*/
int GInumeroArestasMax(Grafo* p);

/*Cria e carrega um grafo armazenado em um arquivo*/
/*na entrada e necessario informar o nome do arquivo*/
/*na saida retorna o ponteiro para o grafo criado. Caso o grafo nao exista
 ou seja, nao foi criado, retornar NULL*/
Grafo* GGcarregaGrafo(char arq_nome[]);

/*Salva o grafo informado em arquivo
 na entrada o grafo deve ser informado, tanto como tb o nome do arquivo
 de destino, o arquivo q rebera o grafo
 na saida deve retornar 1 caso a operacao obtiver sucesso, caso contrario
 retornar 0*/
int GBsalvaGrafo(Grafo* p, char arq_nme[]);

/*------------------------------------------------------------------------------
 * ----------------------OPERACOES DO VERTICE-----------------------------------
 * ---------------------------------------------------------------------------*/
/*Retorna a quantidade de aresta incidem no vertice informado
na entrada e necessario informar o grafo e o vertice
na saida retornar a quantidade de arestas incidentes no vertice*/
int GIpegaGrau(Grafo* p, int v);

/*Retorna o identificador da primeira aresta na estrela do vertice, em grafos
 nao dirigidos*/
int GAprimaAresta(Grafo* p, int v);

/*Retorna o identificador da proxima aresta a partir da aresta informada
na entrada deve ser informado o grafo nao dirigido, o vertice almejado 
 * e a aresta de
referencia*/
int GAproxAresta(Grafo* p, int v, int a1);

/*Retorna o identificador da primeira aresta na estrela do vertice, em grafos
 dirigidos*/
int GAprimaEntrada(Grafo* p, int v);

/*Retorna o identificador da proxima aresta a partir da aresta informada
na entrada deve ser informado o grafo dirigido, o vertice almejado e a aresta de
referencia*/
int GAproxEntrada(Grafo* p, int v, int a1);

/*Retorna o identificador da primeira aresta na estrela de saida do vertice, 
em grafos dirigidos*/
int GAprimaSaida(Grafo* p, int v);

/*Retorna o identificador da proxima aresta na estrela de saida do vertice, 
em grafos dirigidos*/
int GAproxSaida(Grafo* p, int v, int a1);
/*------------------------------------------------------------------------------
 * ----------------------OPERACOES DA ARESTA------------------------------------
 * ---------------------------------------------------------------------------*/
/*Verifica se a aresta almejada e uma aresta laco
 na saida retorna 1 caso seja uma aresta laco, caso contrario 0*/
int GBarestaLaco(Grafo* p, int a);

/*Retorna o identificador do vertice de partida da aresta*/
int GValfa(Grafo* p, int a);

/*Retorna o identificador do vertice de chegada da aresta*/
int GVomega(Grafo* p, int a);

/*Retorna o identificador do vertice que esta no extremo da aresta
na entrada recebe o grafo, a aresta e o vertice de partida
na saida e retornado o vertice v2 no extremo oposto ao vertice v1*/
int GVvizinho(Grafo* p, int a, int v1);

#endif /* GRAFOS_H */