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
#include "busca_largura.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    Grafo *graf = GGcarregaGrafo("grafo1.txt");
    
    int i;
    
    BuscaLargura(graf,1);
        
    return (EXIT_SUCCESS);
}