#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <time.h>

using namespace std;

    /**
    * Este struct representa uma solução individual do problema,
    * através da qual deve ser possível demonstrar como o problema é
    * resolvido e designar um nível de qualidade.
    */
    struct Solucao {
        vector<int> trajeto; // sequencia de cidades
        int idade;
        int distancia;            // distancia total (menor distancia melhor)
    };

    typedef vector<Solucao> Populacao;


    /* define tipo Problema = matriz de distancia entre cidades */
    typedef vector<vector<int>> Problema;
    typedef vector<Problema> Problemas;


    /**
        Declarações de protótipos de funções
    */

    double gerarAleatorio();

    /* processa cada linha do arquivo de entrada e retorna o problema*/
    Problema processarLinha(const string& linha);

    /* processa o arquivo de problemas */
    Problemas lerProblemas(const string& caminho);

    /* retorna a distancia entre duas cidades */
    int retornaDistacia ( Problema problema, int cidade1, int cidade2);

    /* Calcula distancia do trajeto */
    int avaliar ( Solucao s,  Problema problema);

    /* Gera trajeto randomicamente */
    vector<int> geraTrajeto ( Solucao s, int num_cidades);

    /* Gera populacoa inicial */
    Populacao populacaoInicial( Problema problema, int tamanho_populacao);

    Solucao getMelhorIndividuo( Populacao populacao);

    /* Torneio entre dois indivíduo */
    int torneio(double p,  Populacao populacao);

    /* cruzamento */
    vector<Solucao> aplicarCruzamento( Solucao s1,  Solucao s2);

    /* mutação em individuos */
    vector<int> aplicarMutacao ( Solucao s);

    /* seleciona dois indivíduos por torneio */
    vector<int> selecionar( Populacao populacao);

    Populacao evoluir( Problema problema,  Populacao populacao);

    /* gera filho*/
    Solucao geraFilho(Solucao pai, Solucao mae);

    void printa_populacao(Populacao p);

#endif // FUNCOES_H_INCLUDED
