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

typedef std::vector<Solucao> Populacao;


/**
    Declarações de protótipos de funções
*/

double gerarAleatorio(){
    // gera aleatorio entre 0.0 e 1.0
    return std::rand() / static_cast<double>(RAND_MAX);
}

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
Populacao populacaoInicial(Problema problema, int tamanho_populacao);

/* Torneio entre dois indivíduo */
int torneio(double p, Populacao populacao);

void evoluir(Problema problema, Populacao& populacao);

const Solucao& getMelhorIndividuo(Populacao populacao);

/* mutação em individuos */
void aplicarMutacao (Solucao& s);

/* seleciona dois indivíduos por torneio */
std::vector<int> selecionar(Populacao populacao);

/* cruzamento */
std::vector<Solucao> aplicarCruzamento(Solucao s1, Solucao s2);

/* gera filho*/
Solucao geraFilho(Solucao pai, Solucao mae);



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

    Populacao populacao;
    Problema problema = problemas[0];

    populacao = populacaoInicial(problema, 5);
    geraFilho(populacao[0], populacao[1]);


/*
    for (int prob = 0; prob < problemas.size(); prob++){
        Problema problema = problemas[prob];

        Populacao populacao;
        populacao = populacaoInicial(problema, 5);  // VERIFICAR SE 5 É UM TAMANHO ACEITAVELL ?!?!?!?!?!?!

        for (int i = 0; i < 200; i++) {
            evoluir(problema, populacao)

            Solucao melhor = getMelhorIndividuo(populacao);

            std::cout << "Melhor solucao encontrada tem distancia " << melhor.distancia << "\n";
        }

    }
    */





    /* Mostra populacao inicial gerada */
    for(int j = 0; j < 5; j ++){

        for(int i = 0; i < 5; i ++){
            cout << "cidade = " << populacao[j].trajeto[i] + 1 << "  ";
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
int retornaDistacia (Problema problema, int cidade1, int cidade2){
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

/* Calcula distancia do trajeto */
void avaliar (Solucao& s, Problema problema){

    int distancia_total = 0;
    int i;

    // calcula distancia total do trajeto gerado
    for(i = 0; i < s.trajeto.size() - 1; i ++){
        distancia_total += retornaDistacia(problema, s.trajeto[i], s.trajeto[i+1]);
    }

    // do ultimo para o primeiro
    distancia_total += retornaDistacia(problema, s.trajeto[i+1], s.trajeto[0]);

    // atribui a distancia no trajeto da solucao
    s.distancia = distancia_total;
}

/* Gera trajeto randomicamente */
void geraTrajeto (Solucao& s, int num_cidades){
    int aux = 0, valorCorreto;

    // gera trajeto
    for(int i = 0; i < num_cidades; i++){
        do{
            aux = rand()%num_cidades;                // gera um valor randomico
            valorCorreto = 1;

            for(int j = 0; j < s.trajeto.size(); j ++){//verifica se auxiliar é igual a algum valor do trajeto
                if(aux == s.trajeto.at(j))
                    valorCorreto = 0;
            }

        }while(valorCorreto == 0);

        s.trajeto.push_back(aux);                      // Coloca a cidade no trajeto
    }
}

/* Gera populacao inicial */
Populacao populacaoInicial(Problema problema, int tamanho_populacao){


    Populacao populacao;

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

/**
 * Recupera o indivíduo com maior aptidão de toda
 * a população atual (somente leitura).
 */
const Solucao& getMelhorIndividuo(Populacao populacao) {
    int melhor = 0;
    for (int i = 1; i < populacao.size(); i++) {
        if (populacao[i].distancia < populacao[melhor].distancia) {
            melhor = i;
        }
    }

    return populacao[melhor];
}

/* seleciona dois indivíduos por torneio */
std::vector<int> selecionar(Populacao populacao) {
    std::vector<int> individuos;

    // ===================

    double p = 0.8;

    int a = torneio(p, populacao);
    int b;

    do {
        b = torneio(p, populacao);
    } while (a == b);

    // ===================

    std::cout << "individuos selecionados " << a << " " << b << endl << endl;

    individuos.push_back(a);
    individuos.push_back(b);

    return individuos;
}

/* mutação em individuos */
void aplicarMutacao (Solucao& s){
    if(gerarAleatorio() < 0.05){
        int ponto1 = std::rand() % s.trajeto.size();
        int ponto2 = std::rand() % s.trajeto.size();
        std::swap(s.trajeto[ponto1], s.trajeto[ponto2]);
    }
}

void evoluir(Problema problema, Populacao& populacao) {

    Populacao novaPopulacao;

    while (novaPopulacao.size() < populacao.size()) {
        std::vector<int> pais = selecionar(populacao);

        std::vector<Solucao> filhos = aplicarCruzamento(populacao[pais[0]], populacao[pais[1]]);

        aplicarMutacao(filhos[0]);
        aplicarMutacao(filhos[1]);

        avaliar(filhos[0], problema);
        avaliar(filhos[1], problema);

        novaPopulacao.push_back(filhos[0]);
        novaPopulacao.push_back(filhos[1]);
    }

    // Elitismo (opcional, porém bom)
    int indice = std::rand() % novaPopulacao.size();
    novaPopulacao[indice] = getMelhorIndividuo(populacao);

    populacao = novaPopulacao;
}

/* cruzamento */
std::vector<Solucao> aplicarCruzamento(Solucao s1, Solucao s2){
    std::vector<Solucao> filhos;

    double chance = 0.9;

    // Gera filho com a probabilidade
    if (gerarAleatorio() < chance) {
        filhos.push_back(geraFilho(s1,s2));
        filhos.push_back(geraFilho(s2,s1));
    }

    return filhos;
}


/* Torneio entre dois indivíduo */
int torneio(double p, Populacao populacao) {


    int a = std::rand() % populacao.size();
    int b = std::rand() % populacao.size();

    if (gerarAleatorio() < p) {
        // Retornar o indivíduo de menor distancia
        if (populacao[a].distancia < populacao[b].distancia) {
            return a;
        } else {
            return b;
        }
    } else {
        // Retornar o indivíduo de maior distancia
        if (populacao[a].distancia > populacao[b].distancia) {
            return a;
        } else {
            return b;
        }
    }
}


Solucao geraFilho(Solucao pai, Solucao mae){
    // Atribui ao filho as aracteristicas do pai
    Solucao filho = pai;
    // Valor de um ponto até o final para pegar caracteristicas da mae
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
    // Retorna o filho
    return filho;
}
