/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: fellipe
 *
 * Created on 17 de Julho de 2016, 22:19
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grafo.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    Grafo *graf = GGcarregaGrafo("grafo1.txt");
    
    int i;
    
    ListaVertices *vertex;
    ListaEstrelas *estrela;
    
    ListaArestas *edges;
    for(edges=graf->pa;edges!=NULL;edges=edges->prox)
    {
        printf("%i-%i-%i || ",edges->id,edges->origem->id,edges->destino->id);        
    }

    printf("\n");
    
    for(vertex=graf->pv;vertex!=NULL;vertex=vertex->prox)
    {
        printf("%i- ",vertex->id);
        for(estrela=vertex->estrela;estrela!=NULL;estrela=estrela->prox)
        {
            printf("%i,",estrela->aresta->id);
        }
        printf(" || ");        
    }

    return (EXIT_SUCCESS);
}