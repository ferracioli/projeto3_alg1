#include <stdlib.h>
#include "colecao.h"

struct _no {
    int valor;
    struct _no* esq;
    struct _no* dir;
    int altura;
} ;

struct _c {
    No* inicio;
    int estrutura_id;
};

//Funções auxiliares de uso interno :
void lista_destruir(Colecao **c);
void esvazia_lista(No *NO);
No *insere_ABB(No *raiz, No *NO, int id);
int busca_ABB(No *raiz, int valor);
void arvore_destruir(Colecao **c);
void esvazia_arvore(No *NO);
int fator_de_balanceamento(No *raiz);
No *rotacao_direita(No *jovem);
No *rotacao_esquerda(No *jovem);
void calcula_altura(No *raiz);


Colecao* cria_colecao(int estrutura_id) 
{
    /*Inicialmente precisamos apenas criar um ponteiro para uma coleção genérica e guardar seu ID.Só precisamos nos preocupar com o
    tratamento de cada tipo de estrutura nas próximas funções*/
    Colecao *c = (Colecao*)malloc(sizeof(Colecao));
    c->estrutura_id = estrutura_id;
    c->inicio = NULL;
    //A estrutura ainda está vazia, então seu ponteiro de início será nulo
    return c;
    /*OBS: a função main.c não verifica se o endereço da estrutura é nulo, então estou considerando um caso ideal com memória suficiente 
    para alocar tudo.Retornarei todos os ponteiros sem nenhuma verificação*/
}

No* cria_no(int valor)
{
    //Esta função não é chamada na main, portanto precisarei usá-la na função "adiciona" como forma de alocar um nó

    No *NO = (No*)malloc(sizeof(No));
    /*A exigência do PDF é "A atribuição dos ponteiros do nó deve ser feita durante a inserção de valores.", então deixarei seus ponteiros
    momentaneamente como nulos por questões de segurança*/
    NO->valor = valor;
    NO->esq = NULL;
    NO->dir = NULL;
    NO->altura = 0;
    //Se for uma lista, não precisamos mais fazer a verificação da altura, apenas para as árvores
    return NO;
}

void adiciona(Colecao* c, int valor)
{
    //As árvores não aceitam elementos iguais, portanto precisamos verificar se o valor já existe nelas
    if(c->estrutura_id == 4 || c->estrutura_id == 5){
        if(existe(c, valor))
            return;
    }

    //Criação do novo nó :
    No *NO = cria_no(valor);

    switch(c->estrutura_id){

        case 1:
            if(c->inicio == NULL){
                //Se for o nó cabeça da lista ordenada
                c->inicio = NO;
                NO->esq = NULL;
                NO->dir = NULL;
            }
            else{
                //Inserção ordenada em uma lista não vazia :

                No *caminho = c->inicio;
                while((caminho->valor < valor) && (caminho->dir != NULL))
                    //Enquanto o próximo elemento for maior e não for o fim da lista
                    caminho = caminho->dir;
                if(caminho->esq == NULL && caminho->dir == NULL){
                    //A lista possui apenas um nó, e o laço anterior não nos mostra onde devemos inserir o novo nó
                    if(valor > caminho->valor){
                        //insere no fim
                        NO->esq = caminho;
                        NO->dir = NULL;
                        caminho->dir = NO;
                    }
                    else{
                        //insere no início
                        NO->dir = caminho;
                        NO->esq = NULL;
                        caminho->esq = NO;
                        c->inicio = NO;
                    }
                }
                if(valor > caminho->valor){
                    //Inserção à direita do nó
                    if(caminho->dir != NULL)
                        caminho->dir->esq = NO;
                    NO->dir = caminho->dir;
                    NO->esq = caminho;
                    caminho->dir = NO;

                }

                else{
                    //Inserção à esquerda do nó
                    if(caminho->esq != NULL)
                        caminho->esq->dir = NO;
                    NO->esq = caminho->esq;
                    NO->dir = caminho;
                    caminho->esq = NO;
                }
            return;

        case 2:
            if(c->inicio == NULL){
                //Criação do nó cabeça para uma lista com inserção no final
                c->inicio = NO;
                NO->esq = NULL;
                NO->dir = NULL;
            }
            else{
                //Vamos até o fim da lista e inserimos
                No *caminho = c->inicio;
                while(caminho->dir != NULL)
                    //Enquanto não chegou no fim da lista
                    caminho = caminho->dir;

                //Inserimos NO à direita de caminho
                caminho->dir = NO;
                NO->esq = caminho;
                NO->dir = NULL;
            }   
            return;

        case 3:
            if(c->inicio == NULL){
                //Criação do nó cabeça da lista com inserção no início
                c->inicio = NO;
                NO->esq = NULL;
                NO->dir = NULL;
            }
            else{
                //NO será o novo nó inicial
                NO->esq = NULL;
                NO->dir = c->inicio;
                c->inicio->esq = NO;
                c->inicio = NO;
            }   
            return;

        case 4:
            if(c->inicio == NULL){
                //Criação da raiz da árvore binária de busca
                c->inicio = NO;
                NO->esq = NULL;
                NO->dir = NULL;
                NO->altura = 0;
                //A raiz tem altura 0
            }
            else{
                //Inserimos na árvore de forma ordenada, mas sem verificar o balanceamento da árvore
                c->inicio = insere_ABB(c->inicio, NO, c->estrutura_id);
            }   
            return;

        case 5:
            if(c->inicio == NULL){
                //Criação da raiz da árvore AVL
                c->inicio = NO;
                NO->esq = NULL;
                NO->dir = NULL;
                NO->altura = 0;
            }
            else{
                /*Inserimos na árvore de forma ordenada e verificamos o balanceamento. A função é a mesma da anterior
                pois o id da estrutura decidirá se a função precisa se preocupar com balanceamento ou não*/
                 c->inicio = insere_ABB(c->inicio, NO, c->estrutura_id);
            }   
            return;
    }
}

int existe(Colecao* c, int valor)
{
    //A busca será feita de acordo com o tipo de estrutura e retornará 1 caso o valor exista ou 0 caso não exista

    No *caminha = c->inicio;
    switch(c->estrutura_id){
        case 1:
            //O laço ocorre enquanto os valores da lista não forem maiores que o da busca.
            while(caminha != NULL && caminha->valor <= valor){
                if(valor == caminha->valor)
                    return 1;
                caminha = caminha->dir;
            } 
            return 0;

        case 2:
            //Como a lista não está ordenada, precisamos percorrê-la inteiramente
            while(caminha != NULL){
                if(valor == caminha->valor)
                    return 1;
                caminha = caminha->dir;
            }
            return 0;

        case 3:
            //Análoga à lista com inserção no fim, pois ambas estão desordenadas
            while(caminha != NULL){
                if(valor == caminha->valor)
                    return 1;
                caminha = caminha->dir;
            }
            return 0;

        case 4:
            //chamaremos uma função recursiva para a árvore binária de busca
            return busca_ABB(c->inicio, valor);

        case 5:
            /*A árvore AVL usa a mesma ideia da árvore binária de busca, porém de forma balanceada.Portanto, podemos usar a
            mesma função da árvore binária de busca. O diferencial é a disposição dos elementos na árvore AVL, que pode ser
            mais rápida ou no pior caso com mesmo tempo de busca da árvore binária de busca*/
            return busca_ABB(c->inicio, valor);
    }
}
void destroi(Colecao* c)
{
    if(c->estrutura_id == 1 || c->estrutura_id == 2 || c->estrutura_id == 3){
        //Para as três listas o processo será igual
        lista_destruir(&c);
    }
    else{
        //As duas árvores só diferem na disposição de nós, mas a forma de liberação fica igual
        arvore_destruir(&c);
    }
    return;
}

//Implementação das funções auxiliares

void lista_destruir(Colecao **c){
    //É necessário um ponteiro para ponteiro para uma liberação correta de memória, pois apenas um ponteiro não desaloca a estrutura de fato
    esvazia_lista((*c)->inicio);
    free(*c);
    *c = NULL;
    //Esvaziamos a lista e depois liberamos a coleção
    return;
}

void esvazia_lista(No *NO){
    //Função recursiva que chega até o último elemento, desaloca e retorna repetindo o mesmo procedimento
    if(NO == NULL)
        //caso a função seja chamada para uma lista vazia
        return;

    if(NO->dir != NULL){
        //A recursão nos levará até o último elemento
        esvazia_lista(NO->dir);
    }

    //Desalocamos o nó e retornamos :
    free(NO);
    NO = NULL;

    return;
}

No *insere_ABB(No *raiz, No *NO, int id){
    //Faz a inserção em uma árvore binária de busca

    //A variável id verifica se é uma árvore AVL, e assim poder fazer as rotações se necessário

    if(raiz == NULL){
         raiz = NO;
    }
    else if(NO->valor < raiz->valor){
        //Se o novo nó for menor que o atual : olhamos o filho esquerdo
        raiz->esq = insere_ABB(raiz->esq, NO, id);

        if(id == 5){
            //Exclusivo das árvores AVL
            if(fator_de_balanceamento(raiz) == 2){
                if(NO->valor < raiz->esq->valor)
                    //Se o nó folha está em uma das pontas
                    raiz = rotacao_direita(raiz);
                else{
                    //Se o novo nó for uma das folhas do meio
                    //Faz a rotação esquerda direita
                    raiz->esq = rotacao_esquerda(raiz->esq);
                    raiz = rotacao_direita(raiz);
                }
            } 
        }
    }

    else if(NO->valor > raiz->valor){
        //De forma análoga, se o nó é maior que a raiz olhamos o filho direito
        raiz->dir = insere_ABB(raiz->dir, NO, id);

        if(id == 5){
            if(fator_de_balanceamento(raiz) == -2){
                if(NO->valor > raiz->dir->valor)
                    //O nó inserido é uma folha em um dos extremos
                    raiz = rotacao_esquerda(raiz);
                else{
                    //Faz a rotação direita esquerda
                    raiz->dir = rotacao_direita(raiz->dir);
                    raiz = rotacao_esquerda(raiz);
                }
            }
        }
    }
    //Não verificamos se os dois tem o mesmo valor pois isso foi feito anteriormente

    //Atribuição da altura do nó :
    calcula_altura(raiz);

    //A função foi implementada dessa forma para atualizar a altura de todos envolvidos na inserção e evitar erros
    return raiz;
}

int busca_ABB(No *raiz, int valor){
    /*Se o valor é menor que o valor da raiz, olhamos o filho esquerdo, se é maior, olhamos o filho direito.
    Se o valor é igual ao desejado retornamos 1 e se chegarmos em um vetor nulo retornamos 0*/
    if(raiz == NULL)
        return 0;

    if(valor < raiz->valor)
        busca_ABB(raiz->esq, valor);

    else if(valor > raiz->valor)
        busca_ABB(raiz->dir, valor);

    else
        return 1;
}

void arvore_destruir(Colecao **c){
    //Assim como para as listas, precisamos de um ponteiro para ponteiro
    esvazia_arvore((*c)->inicio);
    free(*c);
    *c = NULL;
    //liberamos a estrutura e fazemos seu ponteiro apontar para NULL por segurança
    return;
}

void esvazia_arvore(No *NO){
    //Função recursiva que chega nos nós-folha, libera memória e repete o procedimento
    if(NO == NULL)
        //caso a árvore esteja vazia
        return;

    esvazia_arvore(NO->esq);
    esvazia_arvore(NO->dir);

    //Desalocamos o nó e retornamos :
    free(NO);
    NO = NULL;

    return;
}

int fator_de_balanceamento(No *raiz){
    //Faz altura do filho esquerdo menos a altura do filho direito
    //OBS: filhos nulos tem altura -1
    if(raiz->esq == NULL && raiz->dir == NULL)
        return 0;
    else if(raiz->esq == NULL)
        return -1 - raiz->dir->altura;
    else if(raiz->dir == NULL)
        return raiz->esq->altura  + 1;
    else
        return raiz->esq->altura - raiz->dir->altura;
}

No *rotacao_direita(No *jovem){
    //Rotaciona os dois nós e atualiza as alturas
    No *aux = jovem->esq;
    jovem->esq = aux->dir;
    aux->dir = jovem;
    calcula_altura(jovem);
    calcula_altura(aux);
    return aux;
}

No *rotacao_esquerda(No *jovem){
    //Rotaciona para a esquerda e revê a altura dos dois nós alterados
    No *aux = jovem->dir;
    jovem->dir = aux->esq;
    aux->esq = jovem;
    calcula_altura(jovem);
    calcula_altura(aux);
    return aux;
}

void calcula_altura(No *raiz){
    //Função que verifica todas as possibilidades e atribui a altura da raiz
    //OBS: filhos nulos tem altura -1
    if(raiz->esq == NULL && raiz->dir == NULL)
        //Qualquer filho é nulo, então temos (-1)+1 = 0
        raiz->altura = 0;
    else if (raiz->esq == NULL)
        //Um dos dois não é nulo : pegamos seu valor e somamos 1
        raiz->altura = raiz->dir->altura + 1;
    else if (raiz->dir == NULL)
        //O mesmo do caso anterior
        raiz->altura = raiz->esq->altura + 1;
    else
        //Pegamos o maior valor e somamos 1
        if(raiz->esq->altura > raiz->dir->altura)
            raiz->altura = raiz->esq->altura + 1;
        else
            raiz->altura = raiz->dir->altura + 1;

    return;    
}