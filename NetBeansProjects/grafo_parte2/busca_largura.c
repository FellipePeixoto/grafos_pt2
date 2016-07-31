#include <stdlib.h>
#include <stdio.h>
#include "busca_largura.h"

Fila *Fcria(Grafo *g)
{
    Fila *f;
    
    f->ArestasRestantes = g->numArestas;
    f->ElementoIncial = NULL;
    f->ElementoAtual = NULL;
    f->ElementoFinal = NULL;
    f->pHead = NULL;
    
    f->QuantidadeCinzas = 0;
}

void BuscaLargura(Grafo* ptr, int partida)
{
    Fila *f = Fcria(ptr);
    
    if(f!=NULL)
    {
        int ID_vert;
        TipoVertice *NodoVertice;
        TipoVertice *VerticeVizinho;
        TipoAresta  *NodoAresta;
        TipoEstrela *NodoEstrela;
        /*colore todos os vertices de branco*/
        ColoreVertices(ptr,branco);
        
        /*todas as arestas sao definidas como n visitadas*/
        ArestaReset(ptr,0);
        
        /*busca pelo vertice de partida*/
        NodoVertice = GVpegaVertice(ptr->pv,partida);
        
        NodoVertice->cor = cinza;
        ImprimiDumpVertice(NodoVertice,-1);
        
        
        /*insere o vertice na qeue*/
        Finsere(f,NodoVertice);
        
        while(f->QuantidadeCinzas!=0)
        {
            NodoVertice = Fretira(f);
            ImprimiDumpVertice(NodoVertice,1);
            
            NodoEstrela = NodoVertice->estrela;
            
            while(VerificaVisitaEstrela(NodoVertice))
            {
                NodoAresta = NodoEstrela->aresta;
                
                NodoAresta->visitada = 1;
                ImprimiDumpAresta(NodoAresta);
                
                ID_vert = GVvizinho(ptr,NodoAresta->id,NodoVertice->id);
                
                VerticeVizinho = GVpegaVertice(ptr,ID_vert);
                
                if(VerticeVizinho->cor==branco)
                {
                    VerticeVizinho->cor == cinza;
                    ImprimiDumpVertice(VerticeVizinho,-1);
                    Finsere(f,VerticeVizinho);
                }
                
                NodoEstrela = NodoEstrela->prox;
            }
        }
    }
}

/*
 Insere elemento na fila de da busca
 
 _*f
 * Ponteiro para a estrutura fila
 * 
 _v
 * Ponteiro para um vertice*/
void Finsere(Fila *f,TipoVertice *v)
{
    /*para demais elementos, apos o ultimo*/
    if(f->ElementoFinal!=NULL)
    {
        /*fim da fila*/
        TipoVertice *ElementoVertice = f->ElementoFinal;
        
        /*novo elemento inserido, fim da fila*/
        ElementoVertice->prox = v;
        f->ElementoFinal = v;
        
        f->QuantidadeCinzas++;
    }
    /*primeiro elemento da fila*/
    else
    {
        f->ElementoAtual = v;
        f->ElementoFinal = v;
        f->ElementoIncial = v;
        
        f->QuantidadeCinzas++;
    }
}

/*retorna o vertice q esta ativado*/
TipoVertice *Fretira(Fila *f)
{
    return f->ElementoAtual;
}

int VerificaVisitaEstrela(TipoVertice *v)
{
    TipoEstrela *NodoEstrela = v->estrela;
    
    while(NodoEstrela!=NULL)
    {
        if(NodoEstrela->aresta->visitada==0)
        {
            return 1;
        }
        NodoEstrela = NodoEstrela->prox;
    }
    return 0;
}

void ColoreVertices(Grafo* p, Cores cor)
{
    TipoVertice *NodoVertice = p->pv;
    
    /*percorre a lista de vertices colorindo todos com a cor
     especificada*/
    while(NodoVertice!=NULL)
    {
        NodoVertice->cor = cor;
        
        NodoVertice = NodoVertice->prox;
    }
}

void ArestaReset(Grafo* p, int visita)
{
    TipoAresta *NodoAresta = p->pa;
    
    /*percorre a lista de arestas e define o valor .visitada=visita*/
    while(NodoAresta!=NULL)
    {
        NodoAresta->visitada = visita;
        
        NodoAresta = NodoAresta->prox;
    }
}

void ImprimiDumpVertice(TipoVertice *v,int ativado)
{
    if(ativado==1)
    {
        if(v->cor==branco)
            printf("Vertice - %i Cor branca. Foi ativado\n",v->id);
        if(v->cor==cinza)
            printf("Vertice - %i Cor cinza. Foi ativado\n",v->id);
        if(v->cor==preto)
            printf("Vertice - %i Cor preta. Foi ativado\n",v->id);
    }
    else if(ativado==0)
    {
        if(v->cor==branco)
            printf("Vertice - %i Cor branca. Foi Destativado\n",v->id);
        if(v->cor==cinza)
            printf("Vertice - %i Cor cinza. Foi Desativado\n",v->id);
        if(v->cor==preto)
            printf("Vertice - %i Cor preta. Foi Desativado\n",v->id);
    }
    else
    {
        if(v->cor==branco)
            printf("Vertice - %i Cor branca.\n",v->id);
        if(v->cor==cinza)
            printf("Vertice - %i Cor cinza.\n",v->id);
        if(v->cor==preto)
            printf("Vertice - %i Cor preta.\n",v->id);
    }
}

void ImprimiDumpAresta(TipoAresta *a)
{
    printf("Aresta visitada: %i\n",a->id);
}