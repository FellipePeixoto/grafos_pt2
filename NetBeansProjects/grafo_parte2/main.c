/* 
 * File:   main.c
 * Author: Fellipe Eduardo
 * Author: Danilo Alves
 *
 * Created on 17 de Julho de 2016, 22:19
 */

#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    Grafo *graf = GGcarregaGrafo("grafo1.txt");
    
    GVdestroiVertice(graf,1);
    
    TipoAresta *aux;
    
    printf("Arestas\n");
    for(aux= graf->pa;aux!=NULL;aux=aux->prox)
    {
        printf("%i %i %i\n",aux->id,aux->origem->id,aux->destino->id);
    }
    
    printf("||\n");
    
    TipoVertice *aux1;
    TipoEstrela *aux2;
    printf("Vertices\n");
    for(aux1= graf->pv;aux1!=NULL;aux1=aux1->prox)
    {
        printf("%i -",aux1->id);
        for(aux2=aux1->estrela;aux2!=NULL;aux2=aux2->prox)
        {
            printf(" %i ",aux2->aresta->id);
        }
        printf("\n");
    }
    
        
    return (EXIT_SUCCESS);
}