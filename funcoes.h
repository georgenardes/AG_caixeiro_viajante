#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED

#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <time.h>

#define PROB_MELHOR 0.8
#define PROB_MUTACAO 0.1
#define PROB_GERAR_FILHO 0.9
#define NUM_ITERACOES 200


using namespace std;

/**
* Este struct representa uma solução individual do problema,
* através da qual deve ser possível demonstrar como o problema é
* resolvido e designar um nível de qualidade.
*/
struct Individuo {
    vector<int> cromossomo;      // sequencia de cidades
    int aptidao;            // distancia total (menor distancia melhor)
};

typedef vector<Individuo> Populacao;


/* define tipo Problema = matriz de distancia entre cidades */
typedef vector<vector<int>> Problema;
typedef vector<Problema> Problemas;



/**
    Declarações de protótipos de funções
*/

/* gera um double aleatorio entre 0 e 1*/
double gerarAleatorio();

/* processa cada linha do arquivo de entrada e retorna o problema*/
Problema processarLinha(const string& linha);

/* processa o arquivo de problemas */
Problemas lerProblemas(const string& caminho);

/* retorna a distancia entre duas cidades */
int retornaDistacia ( Problema problema, int cidade1, int cidade2);

/* Calcula aptidao do cromossomo */
int avaliar ( Individuo s,  Problema problema);

/* Gera cromossomo randomicamente */
vector<int> geraCromossomo ( Individuo s, int num_cidades);

/* Gera populacoa inicial */
Populacao populacaoInicial( Problema problema, int tamanho_populacao);

/* retorna o melhor individuo */
Individuo getMelhorIndividuo( Populacao populacao);

/* Torneio entre dois indivíduo */
int torneio(double p,  Populacao populacao);

/* cruzamento */
vector<Individuo> aplicarCruzamento( Individuo s1,  Individuo s2);

/* mutação em individuos */
vector<int> aplicarMutacao ( Individuo s);

/* seleciona dois indivíduos por torneio */
vector<int> selecionar( Populacao populacao);

/* evolui a população */
Populacao evoluir( Problema problema,  Populacao populacao);

/* gera filho*/
Individuo geraFilho(Individuo pai, Individuo mae);

/* printa uma população */
void printa_populacao(Populacao p);

/* calcula fatorial */
int calculaFatorial(int num);

#endif // FUNCOES_H_INCLUDED
