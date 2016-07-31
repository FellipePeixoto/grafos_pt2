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
    
    Grafo *graf = GGcarregaGrafo("grafo2.txt");
    
    GBsalvaGrafo(graf,"vaidan.txt");
    
    //BuscaLargura(graf,1);
        
    return (EXIT_SUCCESS);
}