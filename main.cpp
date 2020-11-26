#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

/* define tipo Problema = matriz de distancia entre cidades */
typedef std::vector<std::vector<int>> Problema;
typedef std::vector<Problema> Problemas;

/**
    Declarações de protótipos de funções
*/

/* processa cada linha do arquivo de entrada e retorna o problema*/
Problema processarLinha(const std::string& linha);

/* processa o arquivo de problemas */
Problemas lerProblemas(const std::string& caminho);

/* retorna a distancia entre duas cidades */
int retornaDistacia (Problema problema, int cidade1, int cidade2);


/**
    main
*/

int main()
{
    Problemas problemas;
    problemas = lerProblemas("C:/Users/User/Desktop/univali/IA/AG_caixeiro_viajante/problemas.csv");

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
        cout << "Lendo distancias entre " << qtd_cidades << " cidades..." << endl;
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
            cout << "cidade " << cidade+1 << " lida distancias " << qtd_lida << endl;
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
    int distancia = 0;

    /* verifica se o valor da cidade está dentro dos limites */
    if (cidade1 > problema.size() || cidade2 > problema.size()){
        return 0;
    }

    if (cidade1 < cidade2){
        distancia = problema[cidade1][cidade2-cidade1];
    } else if (cidade1 > cidade2){
        distancia = problema[cidade2][cidade1-cidade2];
    }

    return distancia;
}
