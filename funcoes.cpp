#include "funcoes.h"
#include <iostream>

void printa_populacao(Populacao p)
{
    cout << "\n\n Exibindo populacao \n\n";
    for(int i = 0; i < p.size(); i++){
        cout << "individuo " << i << " da populacao = ";
        for(int j = 0; j < p[i].cromossomo.size(); j++)
            cout << p[i].cromossomo[j] << " ";
        cout << " aptidao = " << p[i].aptidao;
        cout << endl;
    }
    cout << endl;
}

double gerarAleatorio(){
    // gera aleatorio entre 0.0 e 1.0
    return rand() / static_cast<double>(RAND_MAX);
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

/* Calcula aptidao do cromossomo */
int avaliar ( Individuo s,  Problema problema){

    int distancia_total = 0;
    int i;

    int tam = s.cromossomo.size();

    // calcula aptidao total do cromossomo gerado
    for(i = 0; i < tam - 1; i ++){
        distancia_total += retornaDistacia(problema, s.cromossomo[i], s.cromossomo[i+1]);
    }

    // do ultimo para o primeiro
    distancia_total += retornaDistacia(problema, s.cromossomo[i+1], s.cromossomo[0]);

    return distancia_total;
}

/* Gera trajeto(cromossomos) randomicamente */
vector<int> geraCromossomo ( Individuo s, int num_cidades){
    vector<int> cromossomo;

    int aux = 0, valorCorreto;

    // gera cromossomo
    for(int i = 0; i < num_cidades; i++){
        do{
            aux = rand()%num_cidades;                // gera um valor randomico
            valorCorreto = 1;

            for(int j = 0; j < cromossomo.size(); j ++){//verifica se auxiliar é igual a algum valor do cromossomo
                if(aux == cromossomo.at(j)){
                    valorCorreto = 0;
                    break;
                }
            }
        }while(valorCorreto == 0);

        cromossomo.push_back(aux);                      // Coloca a cidade no cromossomo
    }

    return cromossomo;
}

/* Gera populacao inicial */
Populacao populacaoInicial( Problema problema, int tamanho_populacao){
    Populacao populacao;

    int num_cidades = problema[0].size();
    int cidade_aleatoria = 0;

    // Gera populacao inicial chamando as funcoes: gerar cromossomo e avaliar
    for(int i = 0; i < tamanho_populacao; i++){
        Individuo individuo;
        individuo.cromossomo = geraCromossomo(individuo, num_cidades);
        individuo.aptidao = avaliar(individuo, problema);
        populacao.push_back(individuo);
    }

    return populacao;
}

/**
 * Recupera o indivíduo com maior aptidão de toda
 * a população atual (somente leitura).
 */
Individuo getMelhorIndividuo( Populacao populacao) {

    Individuo s;

    int melhor = 0;
    for (int i = 1; i < populacao.size(); i++) {
        if (populacao[i].aptidao < populacao[melhor].aptidao) {
            melhor = i;
        }
    }

    s = populacao[melhor];

    return s;
}

/* Torneio entre dois indivíduo */
int torneio(double p,  Populacao populacao) {

    int a = rand() % populacao.size();
    int b = rand() % populacao.size();

    if (gerarAleatorio() < p) {
        // Retornar o indivíduo de menor distancia (melhor aptidao)
        if (populacao[a].aptidao < populacao[b].aptidao) {
            return a;
        } else {
            return b;
        }
    } else {
        // Retornar o indivíduo de maior distancia (pior aptidao)
        if (populacao[a].aptidao > populacao[b].aptidao) {
            return a;
        } else {
            return b;
        }
    }
}

/* mutação em individuos */
vector<int> aplicarMutacao ( Individuo s){
    vector<int> mutado = s.cromossomo;

    int ponto1 = rand() % s.cromossomo.size();
    int ponto2 = rand() % s.cromossomo.size();
    swap(mutado[ponto1], mutado[ponto2]);

    return mutado;

}

/* cruzamento */
vector<Individuo> aplicarCruzamento( Individuo s1,  Individuo s2){
    vector<Individuo> filhos;

    // Gera aleatorio para verificar se os pais terão filhos
    if (gerarAleatorio() < PROB_GERAR_FILHO) {
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

        // selecona dos individuos para serem pais
        vector<int> pais = selecionar(populacao);

        // aplica cruzamento e gera os filhos
        vector<Individuo> filhos = aplicarCruzamento(populacao[pais[0]], populacao[pais[1]]);

        // gera aleatorio para verificar se deve aplicar mutacao
        if(gerarAleatorio() < PROB_MUTACAO)
            filhos[0].cromossomo = aplicarMutacao(filhos[0]);
        if(gerarAleatorio() < PROB_MUTACAO)
            filhos[1].cromossomo = aplicarMutacao(filhos[1]);

        // avaliacao dos filhos gerados
        filhos[0].aptidao = avaliar(filhos[0], problema);
        filhos[1].aptidao = avaliar(filhos[1], problema);

        // adiciona os filhos a nova populacao
        novaPopulacao.push_back(filhos[0]);
        novaPopulacao.push_back(filhos[1]);
    }

    // Elitismo (opcional, porém bom)
    int indice = rand() % novaPopulacao.size();
    novaPopulacao[indice] = getMelhorIndividuo(populacao);

    return novaPopulacao;
}


/* seleciona dois indivíduos por torneio */
vector<int> selecionar( Populacao populacao) {
    vector<int> individuos;

    // ===================

    int a = torneio(PROB_MELHOR, populacao);
    int b;

    do {
        b = torneio(PROB_MELHOR, populacao);
    } while (a == b);

    // ===================


    individuos.push_back(a);
    individuos.push_back(b);

    return individuos;
}


Individuo geraFilho(Individuo pai, Individuo mae){
    // Atribui ao filho as aracteristicas do pai
    Individuo filho = pai;

    // Valor de um ponto até o final para pegar caracteristicas da mae
    int ponto;

    // para previnir que o ponto não esteja nos limites (0 ou tamanho maximo)
    do{
        ponto = rand() % pai.cromossomo.size();
    }while(ponto == 0 || ponto == pai.cromossomo.size()-1);

    int pos = ponto;
    bool controle = true;

    for(int i = ponto; i < (mae.cromossomo.size() + ponto); i ++){

        controle = true;
        // verificar se o valor da mae ja esta no filho
        for(int j = 0; j < ponto; j ++){
            if(filho.cromossomo[j] == mae.cromossomo[i % mae.cromossomo.size()]){
                controle = false;
                break;
            }
        }
        // Se e um valor aceitavel, coloca no filho
        if(controle == true){
            filho.cromossomo[pos] = mae.cromossomo[i % mae.cromossomo.size()];
            pos++;
        }
    }

    // inicializacao
    filho.aptidao = 0;

    // Retorna o filho
    return filho;
}

/* retorna o fatorial de um numero */
int calculaFatorial(int num){
    int numFatorial = 1;

    // Calcula fatorial
    for(int i = 1; i < num; i ++)
        numFatorial += numFatorial * i;

    return numFatorial;
}
