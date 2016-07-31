/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   busca_largura.h
 * Author: fellipe
 *
 * Created on 31 de Julho de 2016, 01:08
 */

#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

#include "grafo.h"

typedef enum cores {preto=0,cinza=1,branco=2} Cores;
typedef struct fila Fila;

/*
 estrutura para vertice
 Grava 
 
/*
 estrutura fila
 Grava
 
 _QuantidadePretos
 * inteiro quantidade de vertices pretos na fila 
 _QuantidadeCinzas
 * inteiro quantidade de vertices cinza na fila
 _QuantidadeBrancos
 * inteiro quantidade de vertices brancos na fila
 _ElementoInicial
 * index do elemento inicial do vetor de fila
 _ElementoFinal
 * index do elemento atualmente atendido do vetor de fila
 _VetorVertices
 * tipo ElementoVertice Vetor com os elementos vertice*/
struct fila
{
    int         QuantidadeCinzas;
    int         ArestasRestantes;
    TipoVertice *ElementoIncial;
    TipoVertice *ElementoAtual;
    TipoVertice *ElementoFinal;
    TipoVertice *pHead;
};

void BuscaLargura(Grafo *ptr, int partida);

Fila *Fcria(Grafo *g);

void Finsere(Fila *f,TipoVertice *v);

TipoVertice *Fretira(Fila *f);

int VerificaVisitaEstrela(TipoVertice *v);

void ColoreVertices(Grafo *p, Cores cor);
        
void ArestaReset(Grafo *p, int visita);

void ImprimiDumpVertice(TipoVertice *v,int ativado);

void ImprimiDumpAresta(TipoAresta *a);

#endif /* BUSCA_LARGURA_H */