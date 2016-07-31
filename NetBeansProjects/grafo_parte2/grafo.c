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
static TipoEstrela *GVcriaEstrela(TipoVertice *v,TipoAresta *a);

/*
 Destroi estrelas de um vertice 
 e arestas associadas ao vertice

 _p     ponteiro para grafo
 _e     ponteiro para o tipo ListaEstrelas
 
 Retorna NULL em todos os casos*/
static TipoEstrela *GVdestroiEstrelas(Grafo *p, TipoEstrela *e);

/*
 Destroi todas as estrelas associadas
 ao ID de uma aresta
 
 _p     ponteiro para o tipo lista de vertice
 _a     Identificador da aresta que vai ser deletada
 
 retorna NULL em todos os casos*/
static TipoEstrela *GAdestroiEstrelasAlvo(Grafo *p,TipoVertice *v, int a);

/*Busca por aresta e retorna ser endereco
 * atraves de sua ID
 
 _p     ponteiro para o tipo Grafo
 _a     identificador da aresta alvo
 
 Retorna o ponteiro o endereco da aresta ou NULL
 em caso de erro*/
static TipoAresta *GApegaArestaEndereco(Grafo *p, int a);

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
        TipoVertice   *NodoVertice = p->pv;
                
        /*destroi todos os vertices*/
        while(NodoVertice!=NULL)
        {
            GVdestroiVertice(p,NodoVertice->id);
            NodoVertice = NodoVertice->prox;
        }
        
        /*libera a estrutura primaria*/        
        free(p);
    }
}

int GVcriaVertice(Grafo* p) 
{
    if(p!=NULL)
    {
        if(p->pv!=NULL)
        {
            TipoVertice *VerticeAnterior;
            TipoVertice *VerticeNovo;
            
            VerticeNovo = (TipoVertice*) malloc(sizeof(TipoVertice));
            
            if(VerticeNovo!=NULL)
            {
                /*recebe o primeiro elemento da lista de vertices*/
                VerticeAnterior        = p->pv;
                
                /*percorre a lista de vertices*/
                while(VerticeAnterior->prox!=NULL)
                {
                    VerticeAnterior = VerticeAnterior->prox;
                }
                
                /*o ultimo elemento da lista aponta para o novo ultimo elemento*/
                VerticeAnterior->prox  = VerticeNovo;
                
                /*defini as configuracoes padroes do vertice criado*/
                VerticeNovo->id        = VerticeAnterior->id + 1;
                VerticeNovo->estrela   = NULL;
                VerticeNovo->prox      = NULL;
                
                /*incrementa da quantidade de vertices*/
                p->numVertices++;
                
                return VerticeNovo->id;
            }
        }
        else
        {
            TipoVertice *VerticeNovo;
            
            VerticeNovo = (TipoVertice*) malloc(sizeof(TipoVertice));
            
            if(VerticeNovo!=NULL)
            {
                /*definicao padrao do primeiro elemento da lista*/
                VerticeNovo->id        = 1;
                VerticeNovo->estrela   = NULL;
                VerticeNovo->prox      = NULL;
                
                /*cabeca da lista aponta para o primeiro elemento inserido*/
                p->pv = VerticeNovo;
                
                /*incremento da quantidade de vertices*/
                p->numVertices++;
                
                return VerticeNovo->id;
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
            TipoVertice *VerticeAtual;
            TipoVertice *VerticeAnterior = NULL;
            
            /*nodo atual recebe o elemento do inicio da lista*/
            VerticeAtual = p->pv;
            
            if(VerticeAtual->id==v)
            {
                /*inicio da lista aponta para proximo vertice*/
                p->pv = VerticeAtual->prox;
                
                /*destroi estrelas associadas ao vertice*/
                GVdestroiEstrelas(p,VerticeAtual->estrela);
                
                /*libera a memoria para o sistema*/
                free(VerticeAtual);
                p->numVertices--;
                
                /*fim do procedimento*/
                return 0;
            }
            else
            {
                /*como o primeiro elemento n e o alvo o anterior pode guardar
                 seu endereco para modificacao posterior*/
                VerticeAnterior = VerticeAtual;
                /*nodo atual anda um elemento na fila, uma vez q o anterior n
                 e o alvo*/
                VerticeAtual = VerticeAtual->prox;
                /*percorre a lista de vertices ate encontrar o elemento q
                 deve ser deletado*/
                while((VerticeAtual!=NULL) && (VerticeAtual->id!=v))
                {
                    VerticeAnterior = VerticeAtual;
                    VerticeAtual = VerticeAtual->prox;                    
                }
                
                /*confere se o elemento realmente existe*/
                if(VerticeAtual!=NULL)
                {
                    /*nodo anterior recebe o elemento pos nodo atual para a lista
                     nao ser descontinuada*/
                    VerticeAnterior->prox = VerticeAtual->prox;

                    /*o elemento alvo e liberado*/
                    free(VerticeAtual);
                    p->numVertices--;
                }
                else
                {
                    VerticeAnterior->prox = NULL;
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
            TipoAresta *ArestaNova;
            
            ArestaNova = (TipoAresta*) malloc(sizeof(TipoAresta));
            
            /*confere se foi alocado espaco para o novo nodo*/
            if(ArestaNova!=NULL)
            {
                TipoAresta *ArestaAnterior;

                /*recebe o endereco do primeiro elemento da lista de arestas*/
                ArestaAnterior = p->pa;

                while(ArestaAnterior->prox!=NULL)
                {
                    ArestaAnterior = ArestaAnterior->prox;
                }
                
                ArestaAnterior->prox = ArestaNova;
                
                /*id da aresta... Mesma coisa A id da anterior +1*/
                ArestaNova->id = ArestaAnterior->id+1;
                
                /*origem recebe o endereco para o id de vertice*/
                ArestaNova->origem = GVpegaVertice(p->pv,v1);
                /*gera elemento na lista de estrelas de vertice
                 lista de vertice >> lista de estrelas*/
                GVcriaEstrela(ArestaNova->origem,ArestaNova);
                
                /*destino recebe o endereco para o id de vertice*/
                ArestaNova->destino = GVpegaVertice(p->pv,v2);
                /*gera elemento na lista de estrelas de vertice
                 lista de vertice >> lista de estrelas*/
                GVcriaEstrela(ArestaNova->destino,ArestaNova);
                
                ArestaNova->prox = NULL;
                
                /*incrementa a quantidade arestas*/
                p->numArestas++;
                
                /*fim da funcao*/
                return ArestaNova->id;
            }
        }
        else
        {
            TipoAresta *ArestaNova;
            
            ArestaNova = (TipoAresta*) malloc(sizeof(TipoAresta));
            
            if(ArestaNova!=NULL)
            {
                /*definicao dos valores padroes do primeiro elemento da lista
                 de arestas*/
                ArestaNova->id = 1;
                ArestaNova->prox = NULL;
                
                /*origem recebe o endereco para o id de vertice*/
                ArestaNova->origem = GVpegaVertice(p->pv,v1);
                /*gera elemento na lista de estrelas de vertice
                 lista de vertice >> lista de estrelas*/
                GVcriaEstrela(ArestaNova->origem,ArestaNova);
                
                /*destino recebe o endereco para o id de vertice*/
                ArestaNova->destino = GVpegaVertice(p->pv,v2);
                /*gera elemento na lista de estrelas de vertice
                 lista de vertice >> lista de estrelas*/
                GVcriaEstrela(ArestaNova->destino,ArestaNova);
                
                ArestaNova->prox = NULL;
                
                /*lista de arestas recebe o primeiro elemento*/
                p->pa = ArestaNova;
                
                /*incrementa a quantidade de arestas*/
                p->numArestas++;
                
                /*fim da funcao*/
                return ArestaNova->id;
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
            TipoAresta *ArestaAtual;
            TipoAresta *ArestaAnterior = NULL;
            
            /*nodo atual recebe o elemento do inicio da lista*/
            ArestaAtual = p->pa;
            
            if(ArestaAtual->id==a)
            {
                /*lista aponta para o pos elemento de nodo atual*/
                p->pa = ArestaAtual->prox;
                
                /*libera a memoria do elemento*/
                free(ArestaAtual);
                
                /*fim do funcao*/
                return 0;
            }
            else
            {
                /*nodo anterior e o elemento q vai receber o endereco
                 para o elemento pos NodoAtual*/
                ArestaAnterior = ArestaAtual;
                ArestaAtual = ArestaAtual->prox;
                
                /*percorre a lista ate encontrar o elemento alvo*/
                while((ArestaAtual!=NULL) && (ArestaAtual->id!=a))
                {
                    ArestaAnterior = ArestaAtual;
                    ArestaAtual = ArestaAtual->prox;
                }
                
                /*confere se o elemento existe*/
                if(ArestaAtual!=NULL)
                {
                    ArestaAnterior->prox = ArestaAtual->prox;
                    
                    /*memoria do elemento alvo e liberada*/
                    //GAdestroiEstrelasAlvo(ArestaAtual->origem,ArestaAtual->id);
                    //GAdestroiEstrelasAlvo(ArestaAtual->destino,ArestaAtual->id);
                    free(ArestaAtual);
                    p->numArestas--;
                }
                
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
        TipoVertice *NodoAux = p->pv;
        
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
        TipoAresta *NodoAux = p->pa;
        
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
        TipoAresta *NodoAux = p->pa;
        
        /*percorre a lista de arestas*/
        while(NodoAux!=NULL)
        {
            /*A aresta e dirigida de v1 para v2, se sim retornar 1*/
            if((NodoAux->origem->id==v1) && (NodoAux->destino->id==v2))
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
        TipoAresta *NodoAux = p->pa;
        
        /*percorre a lista de arestas*/
        while(NodoAux!=NULL)
        {
            /*ambas condicoes abaixo verificam se existe uma aresta
             q conecta os vertices v1 e v2*/
            if((NodoAux->origem->id==v1) && (NodoAux->destino->id==v2))
            {
                return 1;
            }
            
            else if((NodoAux->origem->id==v2) && (NodoAux->destino->id==v1))
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
        TipoAresta *NodoAux = p->pa;
        
        /*percorre a lista de arestas*/
        while(NodoAux!=NULL)
        {
            /*A aresta e dirigida de v1 para v2, se sim retornar seu ID*/
            if((NodoAux->origem->id==v1) && (NodoAux->destino->id==v2))
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
        TipoAresta *NodoAux = p->pa;
        
        /*percorre a lista de arestas*/
        while(NodoAux!=NULL)
        {
            /*ambas condicoes abaixo verificam se existe uma aresta
             q conecta os vertices v1 e v2*/
            if((NodoAux->origem->id==v1) && (NodoAux->destino->id==v2))
            {
                return NodoAux->id;
            }
            
            else if((NodoAux->origem->id==v2) && (NodoAux->destino->id==v1))
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
    return GVpegaVertice(p->pv,v1+1)->id;
}

int GAprimeiraAresta(Grafo* p)
{
    return p->pa->id;
}

int GAproximaAresta(Grafo* p, int a1) 
{
    return GApegaArestaEndereco(p,a1+1)->id;
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
        
        fscanf(arq,"%i %i %i",&ArestaID,&origem,&destino);
        
        /*cria a quantidade maxima de arestas*/        
        for(i=1;i<=semente_aresta;i++)
        {
            if(ArestaID==i)
            {
                arestaCriadaID = GAcriaAresta(ptr,origem,destino);
                GApegaArestaEndereco(ptr,arestaCriadaID)->id = ArestaID;
                fscanf(arq,"%i %i %i",&ArestaID,&origem,&destino);
            }
        }

        fclose(arq);
        
        return ptr;
    }
    return NULL;
}

int GBsalvaGrafo(Grafo* p, char arq_nome[]) 
{
    FILE* arq;

    arq = fopen(arq_nome, "w");

    if (arq != NULL)
    {

        fprintf(arq, "%i %i\n", GInumeroVertices(p), GInumeroArestas(p));
        fprintf(arq, "%i %i\n", p->sementeVertices,p->sementeArestas);

        /*variavel de controle*/
        int i;
        
        /*impressao dos IDS dos vertices*/
        TipoVertice *AuxVertice = p->pv;
        while(AuxVertice)
        {
            fprintf(arq,"%i\n",AuxVertice->id);
            AuxVertice = AuxVertice->prox;
        }
        
        TipoAresta *AuxAresta = p->pa;
        while(AuxAresta!=NULL)
        {
            fprintf(arq, "%i %i %i\n",AuxAresta->id, AuxAresta->origem->id,AuxAresta->destino->id);
            AuxAresta = AuxAresta->prox;
        }

        fclose(arq);
        return 1;
    }
    return 0;
}

int GIpegaGrau(Grafo* p, int v) 
{
    TipoEstrela *AuxEstrela = GVpegaVertice(p->pv,v)->estrela;
    
    /*confere se o vertice informado existe*/
    if (v <= p->sementeVertices) 
    {
        int grau = 0;
        
        /*percorre a lista de estrela do vertice
         enquanto houver elementos na lista de estrelas do vertice
         a variavel grau e incrementada, contando a quantidade de
         arestas estao associadas aquele vertice*/
        while(AuxEstrela!=NULL)
        {
            grau++;
            AuxEstrela = AuxEstrela->prox;
        }
        
        return grau;
    }
    return 0;
}

int GAprimaAresta(Grafo* p, int v) {
    /*confere se o vertice existe no grafo*/
    if(v<p->sementeVertices)
    {
        return GVpegaVertice(p->pv,v)->estrela->aresta->id;
    }
}

int GAproxAresta(Grafo* p, int v, int a1)
{
        if((v<p->sementeVertices) && (a1<p->sementeArestas))
    {
        TipoEstrela *NodoEstrela;
        
        NodoEstrela = GVpegaVertice(p->pv,v)->estrela;
        
        /*percorre a lista de arestas*/
        while(NodoEstrela!=NULL)
        {
            if(NodoEstrela->aresta->id==a1)
            {
                /*se aresta a1 trata se de uma aresta laco
                 com esse tipo de busca ela deve ser identificada
                 para nao ser retornada*/
                if(!GBarestaLaco(p,a1))
                {
                    /*retorna a proxima aresta apos a1*/
                    if(NodoEstrela->prox!=NULL)
                    {
                        return NodoEstrela->prox->aresta->id;
                    }
                }
                /*caso a aresta seja aresta laco*/
                else if(NodoEstrela->prox->prox!=NULL)
                {
                    return NodoEstrela->prox->prox->aresta->id;
                }
            }
            
            NodoEstrela = NodoEstrela->prox;
        }
    }
    /*caso de erro*/
    return 0;
}

int GAprimaEntrada(Grafo* p, int v)
{
    if(v<p->sementeVertices)
    {
        TipoEstrela *NodoEstrela;
        
        NodoEstrela = GVpegaVertice(p->pv,v)->estrela;
        
        /*percorre a lista de vertices*/
        while(NodoEstrela!=NULL)
        {
            /*caso a estrela estiver saindo*/
            if(NodoEstrela->aresta->destino->id==v)
            {
                return NodoEstrela->aresta->id;
            }
            
            NodoEstrela = NodoEstrela->prox;
        }
    }
    /*parametro invalido*/
    return -1;
}

int GAproxEntrada(Grafo* p, int v, int a1) 
{
    if((v<p->sementeVertices) && (a1<p->sementeArestas))
    {
        TipoEstrela *NodoEstrela;
        
        /*recebe ponteiro para a lista de estrelas do vertice V*/
        NodoEstrela = GVpegaVertice(p->pv,v)->estrela;
        
        /*percorre a lista de estrelas*/
        while(NodoEstrela!=NULL)
        {
            /*busca pela aresta a1*/
            if(NodoEstrela->aresta->id==a1)
            {
                /*busca pela proxima entrada no vertice*/
                while(NodoEstrela!=NULL)
                {
                    if(NodoEstrela->aresta->destino->id==v)
                    {
                        return NodoEstrela->aresta->id;
                    }
                    NodoEstrela = NodoEstrela->prox;
                }
            }
            
            NodoEstrela = NodoEstrela->prox;
        }
    }
    /*parametros invalidos*/
    return -1;
}

int GAprimaSaida(Grafo* p, int v) 
{
    if(v<p->sementeVertices)
    {
        TipoEstrela *NodoEstrela;
        
        NodoEstrela = GVpegaVertice(p->pv,v)->estrela;
        
        /*percorre a lista de estrela do vertice*/
        while(NodoEstrela!=NULL)
        {
            /*confere se a estrela aponta para uma aresta de saida do vertice*/
            if(NodoEstrela->aresta->origem->id==v)
            {
                /*retorna o identificador da aresta*/
                return NodoEstrela->aresta->id;
            }
            
            NodoEstrela = NodoEstrela->prox;
        }
    }
    /*caso de erro*/
    return 0;
}

int GAproxSaida(Grafo* p, int v, int a1) 
{
    if((v<p->sementeVertices) && (a1<p->sementeArestas))
    {
        TipoEstrela *NodoEstrela;
        
        /*recebe a lista de estrelas do vertice V*/
        NodoEstrela = GVpegaVertice(p->pv,v)->estrela;
        
        /*percorre a lista de estrelas*/
        while(NodoEstrela!=NULL)
        {
            /*busca pela aresta a1*/
            if(NodoEstrela->aresta->id==a1)
            {
                /*percorre ate encontrar a proxima aresta depois de a1*/
                while(NodoEstrela!=NULL)
                {
                    if(NodoEstrela->aresta->origem->id==v)
                    {
                        return NodoEstrela->aresta->id;
                    }
                }
            }
            
            NodoEstrela = NodoEstrela->prox;
        }
        /*nao achou*/
        return 0;
    }
    /*parametros invalidos*/
    return -1;
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
        return GApegaArestaEndereco(p,a)->origem->id;
    }
    /*aresta inexistente*/
    return -2;
}

int GVomega(Grafo* p, int a)
{
    if(a<p->sementeArestas)
    {
        return GApegaArestaEndereco(p,a)->destino->id;
    }
}

int GVvizinho(Grafo* p, int a, int v1)
{
    if((a<p->sementeArestas)&&(v1<p->sementeVertices))
    {
        if(GValfa(p,a)==v1)
        {
            return GApegaArestaEndereco(p,a)->destino->id;
        }
        else if(GVomega(p,a)==v1)
        {
            return GApegaArestaEndereco(p,a)->origem->id;
        }
    }
}

/*
 Cria um elemento na 
 lista de estrela do vertice
 
 _v     ponteiro para o tipo ListaVertices
 _a     ponteiro para o tipo ListaArestas

 Retorna o endereco da estrela criada ou NULL
 em caso de erro*/
static TipoEstrela *GVcriaEstrela(TipoVertice *v,TipoAresta *a)
{
    TipoEstrela *EstrelaNova;
    TipoEstrela *EstrelaAnterior;
    EstrelaNova = (TipoEstrela*) malloc(sizeof(TipoEstrela));
    
    if((a!=NULL) && (EstrelaNova!=NULL) &&  (v!=NULL))
    {
        EstrelaNova->prox = NULL;
        EstrelaNova->aresta = a;
        
        if(v->estrela!=NULL)
        {
            /*nodo anterior recebe o primeiro elemento da lista de estrelas*/
            EstrelaAnterior = v->estrela;
            /*percorre a lista de estrelas ate encontrar o ultimo elemento*/
            while(EstrelaAnterior->prox!=NULL)
            {
                EstrelaAnterior = EstrelaAnterior->prox;
            }
            
            /*ultimo nodo aponta para o novo elemento*/
            EstrelaAnterior->prox = EstrelaNova;
            
            /*fim do procedimento*/
            return EstrelaNova;
        }
        /*caso seja a primeira estrela*/
        else
        {
            /*lista de estrelas aponta para o primeiro elemento inserido*/
            v->estrela = EstrelaNova;

            /*fim do procedimento*/
            return EstrelaNova;
        }
    }
}

/*
 Destroi estrelas de um vertice 
 e arestas associadas ao vertice

 _p     ponteiro para grafo
 _e     ponteiro para o tipo ListaEstrelas
 
 Retorna NULL em todos os casos*/
static TipoEstrela *GVdestroiEstrelas(Grafo *p, TipoEstrela *e)
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
 
 _p     ponteiro para o primeiro elemento da lista de vertices
 _a     Identificador da aresta que vai ser deletada
 
 retorna NULL em todos os casos*/
static TipoEstrela *GAdestroiEstrelasAlvo(Grafo *p,TipoVertice *v, int a)
{
    
    return NULL;
}

/*
 Busca vertice na lista de vertices
 atraves de seu ID
 
 _vertices      ponteiro para o tipo ListaVertices
 _v             inteiro ID do vertice
 
 Retorna o identificador do vertice encontrado
 Ou NULL caso o vertice nao existir*/
TipoVertice *GVpegaVertice(TipoVertice *vertices, int v)
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
static TipoAresta *GApegaArestaEndereco(Grafo *p, int a)
{
    TipoAresta *AuxAresta = p->pa;
    
    /*percorre a lista de arestas*/
    while(AuxAresta!=NULL)
    {
        /*caso encontre a aresta com o ID a, retorna seu endereco*/
        if(AuxAresta->id==a)
        {
            /*fim da funcao*/
            return AuxAresta;
        }
        
        AuxAresta = AuxAresta->prox;
    }
    
    /*caos de erro*/
    return NULL;
}

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
                
                VerticeVizinho = GVpegaVertice(ptr->pv,ID_vert);
                
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