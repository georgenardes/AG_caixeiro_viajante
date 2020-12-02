#include <iostream>
#include "funcoes.h"


/**
    main
*/

int main()
{
    srand(time(NULL));
    Problemas problemas;
    Solucao solucao;

     problemas = lerProblemas("/home/diogomarchi/Documentos/FACULDADE-MATERIAS/inteligenciaArtificial/AG_caixeiro_viajante/problemas.csv");
    //problemas = lerProblemas("C:/Users/User/Desktop/univali/IA/AG_caixeiro_viajante/problemas.csv");


    for (int prob = 0; prob < problemas.size(); prob++){
        printf("\n\n ------- INICIANDO PROBLEMA %d --------------\n\n", prob);
        Problema problema = problemas[prob];

        Populacao populacao;
        populacao = populacaoInicial(problema, 10);  // VERIFICAR SE 5 É UM TAMANHO ACEITAVELL ?!?!?!?!?!?!
        printa_populacao(populacao);

        Solucao melhor;

        for (int i = 0; i < 200; i++) {
            populacao = evoluir(problema, populacao);

            printa_populacao(populacao);

            melhor = getMelhorIndividuo(populacao);

            cout << "Melhor solucao ";
            for(int j = 0; j < melhor.trajeto.size(); j++)
                cout << melhor.trajeto[j] << " ";
            cout << " tem distancia " << melhor.distancia << " idade " << melhor.idade << " na iteracao " << i << "\n";
        }

        printa_populacao(populacao);
        printf("\n\n ------- PROBLEMA %d FINALIZADO --------------\n\n", prob);

        getchar();
    }


    /* Mostra populacao inicial gerada
    for(int j = 0; j < 5; j ++){

        for(int i = 0; i < 5; i ++){
            cout << "cidade = " << populacao[j].trajeto[i] + 1 << "  ";
        }

        cout << "\n distancia solucao[" << j << "] = " << populacao[j].distancia << endl;
        cout << endl << endl;
    }

*/
    return 0;
}



