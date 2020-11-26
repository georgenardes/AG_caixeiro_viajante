#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <time.h>

using namespace std;

/* define tipo Problema = matriz de distancia entre cidades */
typedef std::vector<std::vector<int>> Problema;
typedef std::vector<Problema> Problemas;


/**
 * Este struct representa uma solução individual do problema,
 * através da qual deve ser possível demonstrar como o problema é
 * resolvido e designar um nível de qualidade.
 */
struct Solucao {
    std::vector<int> trajeto; // sequencia de cidades
    int distancia;            // distancia total (menor distancia melhor)
};


/**
    Declarações de protótipos de funções
*/

/* processa cada linha do arquivo de entrada e retorna o problema*/
Problema processarLinha(const std::string& linha);

/* processa o arquivo de problemas */
Problemas lerProblemas(const std::string& caminho);

/* retorna a distancia entre duas cidades */
int retornaDistacia (Problema problema, int cidade1, int cidade2);

/* Calcula distancia do trajeto */
void avaliar (Solucao& s, Problema problema);

/* Gera trajeto randomicamente */
void geraTrajeto (Solucao& s, int num_cidades);

/* Gera populacoa inicial */
std::vector<Solucao> populacaoInicial(Problema problema, int tamanho_populacao);


/**
    main
*/

int main()
{
    srand(time(NULL));
    Problemas problemas;
    problemas = lerProblemas("/home/diogomarchi/Documentos/FACULDADE-MATERIAS/inteligenciaArtificial/AG_caixeiro_viajante/problemas.csv");
    //problemas = lerProblemas("C:/Users/User/Desktop/univali/IA/AG_caixeiro_viajante/problemas.csv");

    Solucao solucao;
    solucao.trajeto = geraTrajeto(5);
    solucao.distancia = avaliar(solucao, problemas[0]);
    std::vector<Solucao> populacao;
    populacao = populacaoInicial(problemas[0], 5);

    /* Mostra populacao inicial gerada */
    for(int j = 0; j < 5; j ++){

        for(int i = 0; i < 5; i ++){
            cout << "[" << i << "] = " << populacao[j].trajeto[i] + 1 << "  ";
        }

        cout << "\n distancia solucao[" << j << "] = " << populacao[j].distancia << endl;
        cout << endl << endl;
    }


    return 0;
}


/* processa cada linha do arquivo de entrada e retorna o problema*/
Problema processarLinha(const std::string& linha) {
    Problema resultado;
    std::vector<int> distancias;
    std::string pedaco;
    std::istringstream stream(linha);

    int qtd_cidades;

    /* primeira coluna qtd de cidades */
    std::getline(stream, pedaco, ';');
    if (!pedaco.empty()) {
        qtd_cidades = std::stoi(pedaco);
    }else{
        return resultado;
    }

    int cidade = 0;
    int qtd_lida = 0;

    while (std::getline(stream, pedaco, ';')) {
        if (!pedaco.empty()) {
            distancias.push_back(std::stoi(pedaco));
            qtd_lida++;
        }

        if (qtd_lida == (qtd_cidades-1)-cidade){
            distancias.emplace(distancias.begin(), 0);
            resultado.push_back(distancias);
            distancias.clear();
            qtd_lida = 0;
            cidade++;
        }
    }

    return resultado;
}

/* processa o arquivo de problemas */
Problemas lerProblemas(const std::string& caminho) {
    std::ifstream stream(caminho);

    std::string linha;
    Problemas dados;

    while (std::getline(stream, linha)) {
        if (linha.size() > 0 && linha[linha.size() - 1] == '\r') {
            linha.pop_back();
        }
        dados.push_back(processarLinha(linha));
    }

    cout << dados.size() << " prolemas lidos" << endl;
    return dados;
}

/* retorna a distancia entre duas cidades */
int retornaDistacia (Problema problema, int cidade1, int cidade2){

/* Calcula distancia do trajeto */
void avaliar (Solucao& s, Problema problema){

/* Gera trajeto randomicamente */
void geraTrajeto (Solucao& s, int num_cidades){
    int aux = 0, valorCorreto;

    // gera trajeto
    for(int i = 0; i < num_cidades; i++){
        do{
            aux = rand()%num_cidades;                // gera um valor randomico
            valorCorreto = 1;

            for(int j = 0; j < trajeto.size(); j ++){//verifica se auxiliar é igual a algum valor do trajeto
                if(aux == trajeto.at(j))
                    valorCorreto = 0;
            }

        }while(valorCorreto == 0);

        s.trajeto.push_back(aux);                      // Coloca a cidade no trajeto
    }
}

/* Gera populacao inicial */
std::vector<Solucao> populacaoInicial(Problema problema, int tamanho_populacao){
    std::vector<Solucao> populacao;

    int num_cidades = problema[0].size();
    int cidade_aleatoria = 0;

    // Gera populacao inicial chamando as funcoes: gerar trajeto e avaliar
    for(int i = 0; i < tamanho_populacao; i++){
        Solucao solucao;
        geraTrajeto(solucao, num_cidades);
        avaliar(solucao, problema);
        populacao.push_back(solucao);
    }

    return populacao;
}
    int distancia_total = 0;

    // calcula distancia total do trajeto gerado
    for(int i = 0; i < s.trajeto.size() - 1; i ++){
        distancia_total += retornaDistacia(problema, s.trajeto.at(i), s.trajeto.at(i+1));
    }

    // atribui a distancia no trajeto da solucao
    s.distancia = distancia_total;
}
    int distancia = 0;

    /* verifica se o valor da cidade está dentro dos limites */
    if (cidade1 > problema.size() || cidade2 > problema.size()){
        return 0;
    }

    /* verifica os indices */
    if (cidade1 < cidade2){
        distancia = problema[cidade1][cidade2-cidade1];
    } else if (cidade1 > cidade2){
        distancia = problema[cidade2][cidade1-cidade2];
    }

    return distancia;
}
