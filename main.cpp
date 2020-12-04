#include <iostream>
#include "funcoes.h"


/**
    main
*/

int main()
{
    srand(time(NULL));
    Problemas problemas;

     // problemas = lerProblemas("/home/diogomarchi/Documentos/FACULDADE-MATERIAS/inteligenciaArtificial/AG_caixeiro_viajante/problemas.csv");
    problemas = lerProblemas("C:/Users/User/Desktop/univali/IA/AG_caixeiro_viajante/input20.csv");


    for (int prob = 0; prob < problemas.size(); prob++){
        printf("\n\n ------- INICIANDO PROBLEMA %d --------------\n\n", prob);

        // seleciona um problema
        Problema problema = problemas[prob];

        // quantidade de individuos
        int qtd_individuos;
        cout << "Defina a quantidade de individuos na populacao ";
        cin >> qtd_individuos;

        cout << "A populacao tera " << qtd_individuos << " individuos " << endl;
        cout << "O numero de iteracoes " << NUM_ITERACOES << endl;
        cout << "Probabilidade de sobreviver o melhor " << PROB_MELHOR << endl;
        cout << "Probabilidade de mutacao " << PROB_MUTACAO << endl;
        cout << "Probabilidade de gerar filhos " << PROB_GERAR_FILHO << endl;
        getchar();


        // gera população inicial
        Populacao populacao;
        populacao = populacaoInicial(problema, qtd_individuos);

        // mostra população
        printa_populacao(populacao);

        // melhor solucao
        Individuo melhor;

        // inicia processo de evolucao
        for (int i = 0; i < NUM_ITERACOES; i++) {

            // evolui populacao
            populacao = evoluir(problema, populacao);

            // printa populacao evoluida
            // printa_populacao(populacao);

            // retorna melhor individuo da populacao
            melhor = getMelhorIndividuo(populacao);

            // exibe melhor solucao
            cout << "Melhor solucao ";
            for(int j = 0; j < melhor.cromossomo.size(); j++)
                cout << melhor.cromossomo[j] << " ";
            cout << " tem distancia " << melhor.aptidao << " na iteracao " << i << "\n";
        }

        // printa populacao final
        printa_populacao(populacao);
        printf("\n\n ------- PROBLEMA %d FINALIZADO --------------\n\n", prob);

        getchar();
    }
    return 0;
}



