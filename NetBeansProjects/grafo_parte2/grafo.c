#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"

/*
 Cria um elemento na 
 lista de estrela do vertice
 
 _v     ponteiro para o tipo ListaVertices
 _a     ponteiro para o tipo ListaArestas

 Retorna o endereco da estrela criada ou NULL
 em caso de erro*/
static ListaEstrelas *GVcriaEstrela(ListaVertices *v,ListaArestas *a);

/*
 Destroi estrelas de um vertice 
 e arestas associadas ao vertice

 _p     ponteiro para grafo
 _e     ponteiro para o tipo ListaEstrelas
 
 Retorna NULL em todos os casos*/
static ListaEstrelas *GVdestroiEstrelas(Grafo *p, ListaEstrelas *e);

/*
 Destroi todas as estrelas associadas
 ao ID de uma aresta
 
 _p     ponteiro para o tipo lista de vertice
 _a     Identificador da aresta que vai ser deletada
 
 retorna NULL em todos os casos*/
static ListaEstrelas *GAdestroiEstrela(ListaVertices *v, int a);

/*
 Busca vertice na lista de vertices
 atraves de seu ID
 
 _vertices      ponteiro para o tipo ListaVertices
 _v             inteiro ID do vertice
 
 Retorna o identificador do vertice encontrado
 Ou NULL caso o vertice nao existir*/
static ListaVertices *GVpegaVertice(ListaVertices *vertices, int v);

/*Busca por aresta e retorna ser endereco
 * atraves de sua ID
 
 _p     ponteiro para o tipo Grafo
 _a     identificador da aresta alvo
 
 Retorna o ponteiro o endereco da aresta ou NULL
 em caso de erro*/
static ListaArestas *GApegaArestaEndereco(Grafo *p, int a);

Grafo* GGcriagrafo()
{
    Grafo *g;
    
    /*aloca uma estrutura do tipo grafo*/
    g = (Grafo*) malloc(sizeof(Grafo));
    
    /*confere se a estrutura foi alocada*/
    if(g!=NULL)
    {
        /*definicao dos valores iniciais padrao da estrutura*/
        g->numArestas       = 0;
        g->numVertices      = 0;
        g->pa               = NULL;
        g->pv               = NULL;        
        /*sementes recebem o valor -1, pois identificadores podem conter 0*/
        g->sementeArestas   = 0;
        g->sementeVertices  = 0;

        return g;
    }
    
    return NULL;
}

void GGdestroiGrafo(Grafo* p) 
{
    if(p!=NULL)
    {
        ListaVertices   *NodoVertice = p->pv;
        ListaArestas    *NodoAresta = p->pa;
        ListaEstrelas   *NodoEstrela;
                
        /*destroi todos os vertices*/
        while(NodoVertice!=NULL)
        {
            GVdestroiVertice(p,NodoVertice->id);
            NodoVertice = NodoVertice->prox;
        }
        
        /*TALVEZ ISSO NAO SEJA NECESSARIO
         *
         KDAJSLJNDASKDMSAÇDAS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
         
         */
        while(NodoAresta!=NULL)
        {
            GAdestroiAresta(p,NodoAresta->id);
            NodoAresta = NodoAresta->prox;
        }
        /*DLASJHDASKDSA>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
         LDKASHÇDLKASNLDKBSLK*/
        
        free(p);
    }
}

int GVcriaVertice(Grafo* p) 
{
    if(p!=NULL)
    {
        if(p->pv!=NULL)
        {
            ListaVertices *NodoAnterior;
            ListaVertices *NodoNovo;
            
            NodoNovo = (ListaVertices*) malloc(sizeof(ListaVertices));
            
            if(NodoNovo!=NULL)
            {
                /*recebe o primeiro elemento da lista de vertices*/
                NodoAnterior        = p->pv;
                
                /*percorre a lista de vertices*/
                while(NodoAnterior->prox!=NULL)
                {
                    NodoAnterior = NodoAnterior->prox;
                }
                
                /*o ultimo elemento da lista aponta para o novo ultimo elemento*/
                NodoAnterior->prox  = NodoNovo;
                
                /*defini as configuracoes padroes do vertice criado*/
                NodoNovo->id        = NodoAnterior->id + 1;
                NodoNovo->estrela   = NULL;
                NodoNovo->prox      = NULL;
                
                /*incrementa da quantidade de vertices*/
                p->numVertices++;
                
                return NodoNovo->id;
            }
        }
        else
        {
            ListaVertices *NodoNovo;
            
            NodoNovo = (ListaVertices*) malloc(sizeof(ListaVertices));
            
            if(NodoNovo!=NULL)
            {
                /*definicao padrao do primeiro elemento da lista*/
                NodoNovo->id        = 1;
                NodoNovo->estrela   = NULL;
                NodoNovo->prox      = NULL;
                
                /*cabeca da lista aponta para o primeiro elemento inserido*/
                p->pv = NodoNovo;
                
                /*incremento da quantidade de vertices*/
                p->numVertices++;
                
                return NodoNovo->id;
            }
        }
    }
    /*caso de erro*/
    return 0;
}

/*destroi um vertice*/
int GVdestroiVertice(Grafo* p, int v)
{
    /*confere se o grafo existe*/
    if(p!=NULL)
    {
        /*confere se a lista de vertices n aponta pra NULL*/
        if(p->pv!=NULL)
        {
            ListaVertices *NodoAtual;
            ListaVertices *NodoAnterior = NULL;
            
            /*nodo atual recebe o elemento do inicio da lista*/
            NodoAtual = p->pv;
            
            if(NodoAtual->id==v)
            {
                /*inicio da lista aponta para proximo vertice*/
                p->pv = NodoAtual->prox;
                
                /*destroi estrelas associadas ao vertice*/
                GVdestroiEstrelas(p,NodoAtual->estrela);
                
                /*libera a memoria para o sistema*/
                free(NodoAtual);
                p->numVertices--;
                
                /*fim do procedimento*/
                return 0;
            }
            else
            {
                /*como o primeiro elemento n e o alvo o anterior pode guardar
                 seu endereco para modificacao posterior*/
                NodoAnterior = NodoAtual;
                /*nodo atual anda um elemento na fila, uma vez q o anterior n
                 e o alvo*/
                NodoAtual = NodoAtual->prox;
                /*percorre a lista de vertices ate encontrar o elemento q
                 deve ser deletado*/
                while((NodoAtual!=NULL) && (NodoAtual->id!=v))
                {
                    NodoAnterior = NodoAtual;
                    NodoAtual = NodoAtual->prox;                    
                }
                
                /*confere se o elemento realmente existe*/
                if(NodoAtual!=NULL)
                {
                    /*nodo anterior recebe o elemento pos nodo atual para a lista
                     nao ser descontinuada*/
                    NodoAnterior->prox = NodoAtual->prox;

                    /*o elemento alvo e liberado*/
                    free(NodoAtual);
                    p->numVertices--;
                }
                else
                {
                    NodoAnterior->prox = NULL;
                }
                
                /*fim do procedimento*/
                return 0;
            }
        }
        /*a lista de vertices n existe*/
        return -2;
    }
    /*o grafo n foi criado*/
    return -1;
}

/*Cria uma aresta*/
int GAcriaAresta(Grafo* p, int v1, int v2) 
{
    /*confere se os parametros passados sao validos*/
    if((p!=NULL) && (v1<p->sementeArestas) && (v2<p->sementeArestas))
    {
        if(p->pa!=NULL)
        {
            ListaArestas *NodoNovo;
            
            NodoNovo = (ListaArestas*) malloc(sizeof(ListaArestas));
            
            /*confere se foi alocado espaco para o novo nodo*/
            if(NodoNovo!=NULL)
            {
                ListaArestas *NodoAnterior;

                /*recebe o endereco do primeiro elemento da lista de arestas*/
                NodoAnterior = p->pa;

                while(NodoAnterior->prox!=NULL)
                {
                    NodoAnterior = NodoAnterior->prox;
                }
                
                NodoAnterior->prox = NodoNovo;
                
                /*id da aresta... Mesma coisa A id da anterior +1*/
                NodoNovo->id = NodoAnterior->id+1;
                
                /*origem recebe o endereco para o id de vertice*/
                NodoNovo->origem = GVpegaVertice(p->pv,v1);
                /*gera elemento na lista de estrelas de vertice
                 lista de vertice >> lista de estrelas*/
                GVcriaEstrela(GVpegaVertice(p->pv,v1),NodoNovo);
                
                /*destino recebe o endereco para o id de vertice*/
                NodoNovo->destino = GVpegaVertice(p->pv,v2);
                /*gera elemento na lista de estrelas de vertice
                 lista de vertice >> lista de estrelas*/
                GVcriaEstrela(GVpegaVertice(p->pv,v1),NodoNovo);
                
                NodoNovo->prox = NULL;
                
                /*incrementa a quantidade arestas*/
                p->numArestas++;
                
                /*fim da funcao*/
                return NodoNovo->id;
            }
        }
        else
        {
            ListaArestas *NodoNovo;
            
            NodoNovo = (ListaArestas*) malloc(sizeof(ListaArestas));
            
            if(NodoNovo!=NULL)
            {
                /*definicao dos valores padroes do primeiro elemento da lista
                 de arestas*/
                NodoNovo->id = 1;
                NodoNovo->prox = NULL;
                
                /*origem recebe o endereco para o id de vertice*/
                NodoNovo->origem = GVpegaVertice(p->pv,v1);
                /*gera elemento na lista de estrelas de vertice
                 lista de vertice >> lista de estrelas*/
                GVcriaEstrela(NodoNovo->origem,NodoNovo);
                
                /*destino recebe o endereco para o id de vertice*/
                NodoNovo->destino = GVpegaVertice(p->pv,v2);
                /*gera elemento na lista de estrelas de vertice
                 lista de vertice >> lista de estrelas*/
                GVcriaEstrela(NodoNovo->destino,NodoNovo);
                
                NodoNovo->prox = NULL;
                
                /*lista de arestas recebe o primeiro elemento*/
                p->pa = NodoNovo;
                
                /*incrementa a quantidade de arestas*/
                p->numArestas++;
                
                /*fim da funcao*/
                return NodoNovo->id;
            }
        }
    }
    /*parametros invalidos*/
    return -1;
}

int GAdestroiAresta(Grafo* p, int a)
{
    if(p!=NULL)
    {
        if(p->pa!=NULL)
        {
            ListaArestas *NodoAtual;
            ListaArestas *NodoAnterior = NULL;
            
            /*nodo atual recebe o elemento do inicio da lista*/
            NodoAtual = p->pa;
            
            if(NodoAtual->id==a)
            {
                /*lista aponta para o pos elemento de nodo atual*/
                p->pa = NodoAtual->prox;
                
                /*libera a memoria do elemento*/
                free(NodoAtual);
                
                /*fim do funcao*/
                return 0;
            }
            else
            {
                /*nodo anterior e o elemento q vai receber o endereco
                 para o elemento pos NodoAtual*/
                NodoAnterior = NodoAtual;
                NodoAtual = NodoAtual->prox;
                
                /*percorre a lista ate encontrar o elemento alvo*/
                while((NodoAtual!=NULL) && (NodoAtual->id!=a))
                {
                    NodoAnterior = NodoAtual;
                    NodoAtual = NodoAtual->prox;
                }
                
                /*confere se o elemento existe*/
                if(NodoAtual!=NULL)
                {
                    NodoAnterior->prox = NodoAtual->prox;
                    
                    /*memoria do elemento alvo e liberada*/
                    free(NodoAtual);
                    p->numArestas--;
                }
                else
                {
                    NodoAnterior = NULL;
                }
                /*fim da funcao*/
                return 0;
            }
        }
        /*nao exitem arestas*/
        return -2;
    }
    /*grafo nao existente*/
    return -1;
}

int GBexisteIdVertice(Grafo* p, int v) 
{
    /*verifica se o vertice procurado esta no intervalo maximo de vertices*/
    if(v<p->sementeVertices)
    {
        ListaVertices *NodoAux = p->pv;
        
        /*percorre a lista de vertices*/
        while(NodoAux!=NULL)
        {
            if(NodoAux->id==v)
            {
                /*retorna 1 caso o vertice for encontrado*/
                return 1;
            }
            NodoAux = NodoAux->prox;
        }
    }
    /*caso de erro*/
    return 0;
}

int GBexisteIdAresta(Grafo* p, int a) 
{
    /*verifica se a aresta procurada esta no intervalo maximo de arestas*/
    if(a<p->sementeArestas)
    {
        ListaArestas *NodoAux = p->pa;
        
        /*percorre a lista de vertices*/
        while(NodoAux!=NULL)
        {
            if(NodoAux->id=a)
            {
                /*retorna 1 caso a aresta for encontrada*/
                return 1;
            }
            NodoAux = NodoAux->prox;
        }
    }
    /*caso de erro*/
    return 0;
}

int GBexisteArestaDir(Grafo* p, int v1, int v2) 
{
    /*confere se os vertices estao no intervalo de vertices permitidos*/
    if((v1<p->sementeVertices) && (v1<p->sementeVertices))
    {
        ListaArestas *NodoAux = p->pa;
        
        /*percorre a lista de arestas*/
        while(NodoAux!=NULL)
        {
            /*A aresta e dirigida de v1 para v2, se sim retornar 1*/
            if((NodoAux->origem->id==v1) && (NodoAux->destino==v2))
            {
                return 1;
            }
            
            NodoAux = NodoAux->prox;
        }
    }
    /*caso false*/
    return 0;
}

int GBexisteAresta(Grafo* p, int v1, int v2) 
{
    /*confere se os vertices estao no intervalo de vertices permitidos*/
    if((v1<p->sementeVertices) && (v1<p->sementeVertices))
    {
        ListaArestas *NodoAux = p->pa;
        
        /*percorre a lista de arestas*/
        while(NodoAux!=NULL)
        {
            /*ambas condicoes abaixo verificam se existe uma aresta
             q conecta os vertices v1 e v2*/
            if((NodoAux->origem->id==v1) && (NodoAux->destino==v2))
            {
                return 1;
            }
            
            if((NodoAux->origem->id==v2) && (NodoAux->destino==v1))
            {
                return 1;
            }
            
            NodoAux = NodoAux->prox;
        }
    }
    /*caso false*/
    return 0;
}

int GApegaArestaDir(Grafo* p, int v1, int v2) 
{
    /*confere se os vertices estao no intervalo de vertices permitidos*/
    if((v1<p->sementeVertices) && (v1<p->sementeVertices))
    {
        ListaArestas *NodoAux = p->pa;
        
        /*percorre a lista de arestas*/
        while(NodoAux!=NULL)
        {
            /*A aresta e dirigida de v1 para v2, se sim retornar seu ID*/
            if((NodoAux->origem->id==v1) && (NodoAux->destino==v2))
            {
                return NodoAux->id;
            }
            
            NodoAux = NodoAux->prox;
        }
    }
    /*caso false*/
    return 0;
}

int GApegaAresta(Grafo* p, int v1, int v2) 
{
    /*confere se os vertices estao no intervalo de vertices permitidos*/
    if((v1<p->sementeVertices) && (v1<p->sementeVertices))
    {
        ListaArestas *NodoAux = p->pa;
        
        /*percorre a lista de arestas*/
        while(NodoAux!=NULL)
        {
            /*ambas condicoes abaixo verificam se existe uma aresta
             q conecta os vertices v1 e v2*/
            if((NodoAux->origem->id==v1) && (NodoAux->destino==v2))
            {
                return NodoAux->id;
            }
            
            if((NodoAux->origem->id==v2) && (NodoAux->destino==v1))
            {
                return NodoAux->id;
            }
            
            NodoAux = NodoAux->prox;
        }
    }
    /*caso false*/
    return 0;
}

/*essa funcao retorna um inteiro a ID do vertice*/
int GVprimeiroVertice(Grafo* p) 
{
    return p->pv->id;
}

int GVproximoVertice(Grafo* p, int v1) 
{
    
}

int GAprimeiraAresta(Grafo* p)
{
    
}

int GAproximaAresta(Grafo* p, int a1) 
{
    
}

int GInumeroVertices(Grafo* p) 
{
    return p->numVertices;
}

int GInumeroVerticesMax(Grafo* p) 
{
    return p->sementeVertices;
}

int GInumeroArestas(Grafo* p) 
{
    return p->numArestas;
}

int GInumeroArestasMax(Grafo* p) 
{
    return p->sementeArestas;
}

Grafo* GGcarregaGrafo(char arq_nome[]) {
    FILE* arq;

    arq = fopen(arq_nome, "r");

    if (arq != NULL) {
        Grafo* ptr;
        int i;
        int num_vertices, num_arestas;
        int semente_vertice, semente_aresta;
        int verticeCriadoID, arestaCriadaID;
        int destino,origem;
        int VerticeID;
        int ArestaID;

        /*leitura da quantidade de arestas e vertices*/
        fscanf(arq, "%i %i", &num_vertices, &num_arestas);
        fscanf(arq, "%i %i", &semente_vertice, &semente_aresta);
        
        ptr = GGcriagrafo();
        ptr->sementeArestas = semente_aresta;
        ptr->sementeVertices = semente_vertice;
        
        for(i=0;i<num_vertices;i++)
        {
            /*identificadores do vertices*/
            fscanf(arq,"%i",&VerticeID);
            verticeCriadoID = GVcriaVertice(ptr);
            
            /*destroi os vertices desnecessarios*/
            while(verticeCriadoID!=VerticeID)
            {
                verticeCriadoID = GVcriaVertice(ptr);
                GVdestroiVertice(ptr,verticeCriadoID-1);
            }
        }
        
        /*cria a quantidade maxima de arestas*/        
        for(i=0;i<num_arestas;i++)
        {
            /*vetor de identificadores das arestas*/
            fscanf(arq,"%i %i %i",&ArestaID,&origem,&destino);
            arestaCriadaID = GAcriaAresta(ptr,origem,destino);
            
            /*destroi as arestas desnecessarias*/
            while(arestaCriadaID!=ArestaID)
            {
                arestaCriadaID = GAcriaAresta(ptr,origem,destino);
                GAdestroiEstrela(ptr->pv,arestaCriadaID-1);
                GAdestroiAresta(ptr,arestaCriadaID-1);
            }
        }

        fclose(arq);
        
        return ptr;
    }
    return NULL;
}

/*RECORTE 2 -----------------------------------------------------------------*/

int GAproxAresta(Grafo* p, int v, int a1)
{
        if((v<p->sementeVertices) && (a1<p->sementeArestas))
    {
        ListaArestas *NodoAux;
        
        /*percorre a lista de arestas*/
        while(NodoAux!=NULL)
        {
            /*dada a aresta e o vertice A aresta atual e compativel?*/
            if((NodoAux->destino->id==v) && (NodoAux->origem->id==v) && (NodoAux->id==a1))
            {
                /*passa para a proxima posicao para nao retornar a aresta
                 encontrada e sim a proxima*/
                NodoAux = NodoAux->prox;
                
                /*percorre a lista a partir da proxima posicao da aresta A1*/
                while(NodoAux!=NULL)
                {
                    /*caso a condicao seja verdadeira retorna a proxima aresta
                     de entrada no vertice*/
                    if(NodoAux->destino->id==v)
                    {
                        return NodoAux->id;
                    }
                    
                    NodoAux = NodoAux->prox;
                }
                
                /*caso de erro*/
                return 0;
            }
            
            NodoAux = NodoAux->prox;
        }
    }
    /*caso de erro*/
    return 0;
}

int GAprimaEntrada(Grafo* p, int v)
{
    if(v<p->sementeVertices)
    {
        return GVpegaVertice(p->pv,v)->estrela->aresta->id;
    }
}

int GAproxEntrada(Grafo* p, int v, int a1) 
{
    if((v<p->sementeVertices) && (a1<p->sementeArestas))
    {
        ListaArestas *NodoAux;
        
        /*percorre a lista de arestas*/
        while(NodoAux!=NULL)
        {
            /*dada a aresta e o vertice A aresta atual e compativel?*/
            if((NodoAux->destino->id==v) && (NodoAux->id==a1))
            {
                /*passa para a proxima posicao para nao retornar a aresta
                 encontrada e sim a proxima*/
                NodoAux = NodoAux->prox;
                
                /*percorre a lista a partir da proxima posicao da aresta A1*/
                while(NodoAux!=NULL)
                {
                    /*caso a condicao seja verdadeira retorna a proxima aresta
                     de entrada no vertice*/
                    if(NodoAux->destino->id==v)
                    {
                        return NodoAux->id;
                    }
                    
                    NodoAux = NodoAux->prox;
                }
                
                /*caso de erro*/
                return 0;
            }
            
            NodoAux = NodoAux->prox;
        }
    }
    /*caso de erro*/
    return 0;
}

int GAprimaSaida(Grafo* p, int v) 
{
    if(v<p->sementeVertices)
    {
        ListaEstrelas *NodoAux;
        
        NodoAux = GVpegaVertice(p->pv,v)->estrela;
        
        /*percorre a lista de estrela do vertice*/
        while(NodoAux!=NULL)
        {
            /*confere se a estrela aponta para uma aresta de saida do vertice*/
            if(NodoAux->aresta->origem->id==v)
            {
                /*retorna o identificador da aresta*/
                return NodoAux->aresta->id;
            }
            
            NodoAux = NodoAux->prox;
        }
    }
    /*caso de erro*/
    return 0;
}

int GAproxSaida(Grafo* p, int v, int a1) 
{
    if((v<p->sementeVertices) && (a1<p->sementeArestas))
    {
        ListaVertices *NodoVertice;
        
        NodoVertice = GVpegaVertice(p->pv,v);
        
    }
}

/*fiz um teste aqui                     POSSIVEL ERRO AQUI*/
int GBarestaLaco(Grafo* p, int a)
{
    if(a<p->sementeArestas)
    {
        if(GApegaArestaEndereco(p,a)->origem==GApegaArestaEndereco(p,a)->destino)
        {
            /*fim da funcao*/
            return 1;
        }
    }
    /*fim da funcao*/
    return 0;
}

/*retorna o identificador do vertice inicial da aresta*/
int GValfa(Grafo* p, int a) 
{
    if(a<p->sementeArestas)
    {
        ListaArestas *NodoAux = GApegaArestaEndereco(p,a);
        
        if(NodoAux!=NULL)
        {
            return NodoAux->id;
        }
        /*aresta inexistente*/
        return -2;
    }
    /*aresta invalida*/
    return -1;
}

/*
 Cria um elemento na 
 lista de estrela do vertice
 
 _v     ponteiro para o tipo ListaVertices
 _a     ponteiro para o tipo ListaArestas

 Retorna o endereco da estrela criada ou NULL
 em caso de erro*/
static ListaEstrelas *GVcriaEstrela(ListaVertices *v,ListaArestas *a)
{
    ListaVertices *NodoAux = v;
    ListaEstrelas *NodoAtual;
    ListaEstrelas *NodoAnterior;
    NodoAtual = (ListaEstrelas*) malloc(sizeof(ListaEstrelas));
    
    if((a!=NULL) && (NodoAtual!=NULL) &&  (NodoAux!=NULL))
    {
        NodoAtual->prox = NULL;
        NodoAtual->aresta = a;
        
        if(NodoAux->estrela!=NULL)
        {
            /*nodo anterior recebe o primeiro elemento da lista de estrelas*/
            NodoAnterior = NodoAux->estrela;
            /*percorre a lista de estrelas ate encontrar o ultimo elemento*/
            while(NodoAnterior->prox!=NULL)
            {
                NodoAnterior = NodoAnterior->prox;
            }
            
            /*ultimo nodo aponta para o novo elemento*/
            NodoAnterior = NodoAtual;
            
            /*fim do procedimento*/
            return NodoAtual;
        }
        /*caso seja a primeira estrela*/
        else
        {
            /*lista de estrelas aponta para o primeiro elemento inserido*/
            v->estrela = NodoAtual;

            /*fim do procedimento*/
            return NodoAtual;
        }
    }
}

/*
 Destroi estrelas de um vertice 
 e arestas associadas ao vertice

 _p     ponteiro para grafo
 _e     ponteiro para o tipo ListaEstrelas
 
 Retorna NULL em todos os casos*/
static ListaEstrelas *GVdestroiEstrelas(Grafo *p, ListaEstrelas *e)
{
    /*procedimento recursivo que deleta as estrelas e respectivas
     arestas*/
    if(e!=NULL)
    {
        GAdestroiAresta(p,e->aresta->id);
        free(e);
        return GVdestroiEstrelas(p,e->prox);
    }
    /*condicao de parada da funcao recursiva*/
    else
    {
        return NULL;
    }
}

/*
 Destroi todas as estrelas associadas
 ao ID de uma aresta
 
 _p     ponteiro para o tipo lista de vertice
 _a     Identificador da aresta que vai ser deletada
 
 retorna NULL em todos os casos*/
static ListaEstrelas *GAdestroiEstrela(ListaVertices *v, int a)
{
    ListaVertices *AuxVertice = v;
    ListaEstrelas *NodoAnterior;
    ListaEstrelas *NodoAtual;

    /*percorre a lista de vertices*/
    while(AuxVertice!=NULL)
    {
        NodoAtual = AuxVertice->estrela;
        /*percorre a lista de estrelas do vertice em busca de arestas associadas
         aquela estrela*/
        while(NodoAtual!=NULL)
        {
            /*caso o nodo atual estiver associado aquela aresta ele deve ser
             deletado*/
            if(NodoAtual->aresta->id==a)
            {
                NodoAnterior = NodoAtual->prox;
                free(NodoAtual);
                NodoAtual = NodoAnterior->prox;
            }

            NodoAnterior = NodoAtual;
            NodoAtual = NodoAtual->prox;
        }
        
        AuxVertice = AuxVertice->prox;
    }
    
    return NULL;
}

/*
 Busca vertice na lista de vertices
 atraves de seu ID
 
 _vertices      ponteiro para o tipo ListaVertices
 _v             inteiro ID do vertice
 
 Retorna o identificador do vertice encontrado
 Ou NULL caso o vertice nao existir*/
static ListaVertices *GVpegaVertice(ListaVertices *vertices, int v)
{    
    /*procedimento recursivo que procura na lista vertices um elemento pelo seu
     ID*/
    if((vertices!=NULL) && (vertices->id!=v))
    {
        return GVpegaVertice(vertices->prox,v);
    }
    /*condicao de parada*/
    else
    {
        return vertices;
    }
}

/*Busca por aresta e retorna ser endereco
 * atraves de sua ID
 
 _p     ponteiro para o tipo Grafo
 _a     identificador da aresta alvo
 
 Retorna o ponteiro o endereco da aresta ou NULL
 em caso de erro*/
static ListaArestas *GApegaArestaEndereco(Grafo *p, int a)
{
    ListaArestas *NodoAux = p->pa;
    
    /*percorre a lista de arestas*/
    while(NodoAux!=NULL)
    {
        /*caso encontre a aresta com o ID a, retorna seu endereco*/
        if(NodoAux->id==a)
        {
            /*fim da funcao*/
            return NodoAux;
        }
        
        NodoAux = NodoAux->prox;
    }
    
    /*caos de erro*/
    return NULL;
}