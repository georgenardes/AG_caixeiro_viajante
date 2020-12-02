#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <time.h>

using namespace std;

/* define tipo Problema = matriz de distancia entre cidades */
typedef vector<vector<int>> Problema;
typedef vector<Problema> Problemas;


/**
 * Este struct representa uma solu��o individual do problema,
 * atrav�s da qual deve ser poss�vel demonstrar como o problema �
 * resolvido e designar um n�vel de qualidade.
 */
struct Solucao {
    vector<int> trajeto; // sequencia de cidades
    int idade;
    int distancia;            // distancia total (menor distancia melhor)
};

typedef vector<Solucao> Populacao;


/**
    Declara��es de prot�tipos de fun��es
*/

double gerarAleatorio(){
    // gera aleatorio entre 0.0 e 1.0
    return rand() / static_cast<double>(RAND_MAX);
}

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

/* Torneio entre dois indiv�duo */
int torneio(double p,  Populacao populacao);

/* cruzamento */
vector<Solucao> aplicarCruzamento( Solucao s1,  Solucao s2);

/* muta��o em individuos */
vector<int> aplicarMutacao ( Solucao s);

/* seleciona dois indiv�duos por torneio */
vector<int> selecionar( Populacao populacao);

Populacao evoluir( Problema problema,  Populacao populacao);

/* gera filho*/
Solucao geraFilho(Solucao pai, Solucao mae);


void printa_populacao(Populacao p)
{
    cout << "\n\n Exibindo populacao \n\n";
    for(int i = 0; i < p.size(); i++){
        cout << "individuo " << i << " da populacao = ";
        for(int j = 0; j < p[i].trajeto.size(); j++)
            cout << p[i].trajeto[j] << " ";
        cout << " distancia = " << p[i].distancia << " idade = " << p[i].idade;
        cout << endl;
    }
    cout << endl;
}


/**
    main
*/

int main()
{
    srand(time(NULL));
    Problemas problemas;
    Solucao solucao;

    // problemas = lerProblemas("/home/diogomarchi/Documentos/FACULDADE-MATERIAS/inteligenciaArtificial/AG_caixeiro_viajante/problemas.csv");
    problemas = lerProblemas("C:/Users/User/Desktop/univali/IA/AG_caixeiro_viajante/problemas.csv");


    for (int prob = 0; prob < problemas.size(); prob++){
        printf("\n\n ------- INICIANDO PROBLEMA %d --------------\n\n", prob);
        Problema problema = problemas[prob];

        Populacao populacao;
        populacao = populacaoInicial(problema, 10);  // VERIFICAR SE 5 � UM TAMANHO ACEITAVELL ?!?!?!?!?!?!
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


/* processa cada linha do arquivo de entrada e retorna o problema*/
Problema processarLinha(const string& linha) {
    Problema resultado;
    vector<int> distancias;
    string pedaco;
    istringstream stream(linha);

    int qtd_cidades;

    /* primeira coluna qtd de cidades */
    getline(stream, pedaco, ';');
    if (!pedaco.empty()) {
        qtd_cidades = stoi(pedaco);
    }else{
        return resultado;
    }

    int cidade = 0;
    int qtd_lida = 0;

    while (getline(stream, pedaco, ';')) {
        if (!pedaco.empty()) {
            distancias.push_back(stoi(pedaco));
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
Problemas lerProblemas(const string& caminho) {
    ifstream stream(caminho);

    string linha;
    Problemas dados;

    while (getline(stream, linha)) {
        cout << linha << endl;
        if (linha.size() > 0 && linha[linha.size() - 1] == '\r') {
            linha.pop_back();
        }
        dados.push_back(processarLinha(linha));
    }

    cout << dados.size() << " prolemas lidos" << endl;
    return dados;
}

/* retorna a distancia entre duas cidades */
int retornaDistacia ( Problema problema, int cidade1, int cidade2){
    int distancia = 0;

    /* verifica se o valor da cidade est� dentro dos limites */
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

/* Calcula distancia do trajeto */
int avaliar ( Solucao s,  Problema problema){

    int distancia_total = 0;
    int i;

    int tam = s.trajeto.size();

    // calcula distancia total do trajeto gerado
    for(i = 0; i < tam - 1; i ++){
        distancia_total += retornaDistacia(problema, s.trajeto[i], s.trajeto[i+1]);
    }

    // do ultimo para o primeiro
    distancia_total += retornaDistacia(problema, s.trajeto[i+1], s.trajeto[0]);

    return distancia_total;
}

/* Gera trajeto randomicamente */
vector<int> geraTrajeto ( Solucao s, int num_cidades){
    vector<int> trajeto;

    int aux = 0, valorCorreto;

    // gera trajeto
    for(int i = 0; i < num_cidades; i++){
        do{
            aux = rand()%num_cidades;                // gera um valor randomico
            valorCorreto = 1;

            for(int j = 0; j < trajeto.size(); j ++){//verifica se auxiliar � igual a algum valor do trajeto
                if(aux == trajeto.at(j)){
                    valorCorreto = 0;
                    break;
                }

            }

        }while(valorCorreto == 0);

        trajeto.push_back(aux);                      // Coloca a cidade no trajeto
    }

    return trajeto;
}

/* Gera populacao inicial */
Populacao populacaoInicial( Problema problema, int tamanho_populacao){
    Populacao populacao;

    int num_cidades = problema[0].size();
    int cidade_aleatoria = 0;

    // Gera populacao inicial chamando as funcoes: gerar trajeto e avaliar
    for(int i = 0; i < tamanho_populacao; i++){
        Solucao solucao;
        solucao.trajeto = geraTrajeto(solucao, num_cidades);
        solucao.distancia = avaliar(solucao, problema);
        solucao.idade = 0;
        populacao.push_back(solucao);
    }

    return populacao;
}

/**
 * Recupera o indiv�duo com maior aptid�o de toda
 * a popula��o atual (somente leitura).
 */
Solucao getMelhorIndividuo( Populacao populacao) {

    Solucao s;

    int melhor = 0;
    for (int i = 1; i < populacao.size(); i++) {
        if (populacao[i].distancia < populacao[melhor].distancia) {
            melhor = i;
        }
    }

    s = populacao[melhor];

    return s;
}

/* Torneio entre dois indiv�duo */
int torneio(double p,  Populacao populacao) {

    int a = rand() % populacao.size();
    int b = rand() % populacao.size();

    if (gerarAleatorio() < p) {
        // Retornar o indiv�duo de menor distancia
        if (populacao[a].distancia < populacao[b].distancia) {
            return a;
        } else {
            return b;
        }
    } else {
        // Retornar o indiv�duo de maior distancia
        if (populacao[a].distancia > populacao[b].distancia) {
            return a;
        } else {
            return b;
        }
    }
}

/* muta��o em individuos */
vector<int> aplicarMutacao ( Solucao s){
    vector<int> mutado = s.trajeto;

    if(gerarAleatorio() < 0.05){
        int ponto1 = rand() % s.trajeto.size();
        int ponto2 = rand() % s.trajeto.size();
        swap(mutado[ponto1], mutado[ponto2]);
    }

    return mutado;

}

/* cruzamento */
vector<Solucao> aplicarCruzamento( Solucao s1,  Solucao s2){
    vector<Solucao> filhos;

    double chance = 0.9;

    // Gera filho com a probabilidade
    if (gerarAleatorio() < chance) {
        filhos.push_back(geraFilho(s1,s2));
        filhos.push_back(geraFilho(s2,s1));
    } else {
        filhos.push_back(s1);
        filhos.push_back(s2);
    }

    return filhos;
}


Populacao evoluir( Problema problema,  Populacao populacao) {

    Populacao novaPopulacao;

    while (novaPopulacao.size() < populacao.size()) {
        vector<int> pais = selecionar(populacao);
        vector<Solucao> filhos = aplicarCruzamento(populacao[pais[0]], populacao[pais[1]]);

        filhos[0].trajeto = aplicarMutacao(filhos[0]);
        filhos[1].trajeto = aplicarMutacao(filhos[1]);

        filhos[0].distancia = avaliar(filhos[0], problema);
        filhos[1].distancia = avaliar(filhos[1], problema);

        novaPopulacao.push_back(filhos[0]);
        novaPopulacao.push_back(filhos[1]);
    }

    for(int i = 0; i < novaPopulacao.size(); i++){
        novaPopulacao[i].idade++;
    }

    // Elitismo (opcional, por�m bom)
    int indice = rand() % novaPopulacao.size();
    novaPopulacao[indice] = getMelhorIndividuo(populacao);

    return novaPopulacao;
}


/* seleciona dois indiv�duos por torneio */
vector<int> selecionar( Populacao populacao) {
    vector<int> individuos;

    // ===================

    double p = 0.8;

    int a = torneio(p, populacao);
    int b;

    do {
        b = torneio(p, populacao);
    } while (a == b);

    // ===================


    individuos.push_back(a);
    individuos.push_back(b);

    return individuos;
}


Solucao geraFilho(Solucao pai, Solucao mae){
    // Atribui ao filho as aracteristicas do pai
    Solucao filho = pai;
    // Valor de um ponto at� o final para pegar caracteristicas da mae
    int ponto = rand() % pai.trajeto.size();
    int pos = ponto;
    bool controle = true;

    for(int i = ponto; i < (mae.trajeto.size() + ponto); i ++){

        controle = true;
        // verificar se o valor da mae ja esta no filho
        for(int j = 0; j < ponto; j ++){
            if(filho.trajeto[j] == mae.trajeto[i % mae.trajeto.size()]){
                controle = false;
                break;
            }
        }
        // Se e um valor aceitavel, coloca no filho
        if(controle == true){
            filho.trajeto[pos] = mae.trajeto[i % mae.trajeto.size()];
            pos++;
        }
    }

    // inicializacao
    filho.idade = 0;
    filho.distancia = 0;

    // Retorna o filho
    return filho;
}
