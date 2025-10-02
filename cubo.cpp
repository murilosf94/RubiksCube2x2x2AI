//Projeto Cubo Mágico 2x2x2 com IA para a disciplina de PI: Sistemas Inteligentes e Aprendizado de Máquina
//Alunos: 
//Murilo de Souza Freitas (23012056);
//João Vitor de Athayde Abram (23005120);
//Joao Pedro Kafer Bachiega (23006014);
//Matheus Henrique de Oliveira Cesar (23004140).

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <deque> 
#include <unordered_set>
#include <array>
#include <cstring>
#include <queue>      
#include <functional> 
#include <memory>     
#include <algorithm>  

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

using namespace std;
#define ll long long

//habilita a utilizacao das cores para outros sistemas operacionais
void habilitarCores() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) return;
#endif
}

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//facilita a manipulacao do cubo para nao ter que repetir o array<array<int, 2>, 2> o tempo todo
using Face = array<array<int, 2>, 2>;

//classe cubo 2x2x2 (ESTADO DO CUBO)
class Cubo2x2 {
private:
    Face U, D, F, B, L, R; //declaro as faces

    //gira a face no sentido horario
    inline void GirarFaceHor(Face& face) {
        int temp = face[0][0];
        face[0][0] = face[1][0];
        face[1][0] = face[1][1];
        face[1][1] = face[0][1];
        face[0][1] = temp;
    }

    //codigo cores
    string cor(int color) const {
        string block = "██";
        switch (color) {
            case 0: return "\033[1;37m" + block + "\033[0m"; //branco
            case 1: return "\033[1;33m" + block + "\033[0m"; //amarelo
            case 2: return "\033[1;34m" + block + "\033[0m"; //azul
            case 3: return "\033[1;38;5;208m" + block + "\033[0m"; //laranja
            case 4: return "\033[1;32m" + block + "\033[0m"; //verde
            case 5: return "\033[1;31m" + block + "\033[0m"; //vermelho 
            default: return "??";
        }
    }

//classes publicas
public:
    //construtor para inicializar o cubo resolvido
    Cubo2x2() {
        U = {{{0, 0}, {0, 0}}};
        D = {{{1, 1}, {1, 1}}};
        F = {{{2, 2}, {2, 2}}};
        B = {{{4, 4}, {4, 4}}};
        L = {{{5, 5}, {5, 5}}};
        R = {{{3, 3}, {3, 3}}};
    }

    //retorna o nome do movimento dado o id do movimento
    string getNomeMovimento(int moveId) {
        const char* nomes[] = {"", "U", "D", "F", "B", "L", "R", "U'", "D'", "F'", "B'", "L'", "R'"};
        return nomes[moveId];
    }

    //dado o numero do movimento, faz o movimento correspondente
    void fazerMovimento(int moveId) {
        switch (moveId) {
            case 1:  moveU(); break;
            case 2:  moveD(); break;
            case 3:  moveF(); break;
            case 4:  moveB(); break;
            case 5:  moveL(); break;
            case 6:  moveR(); break;
            case 7:  moveU_antihor(); break;
            case 8:  moveD_antihor(); break;
            case 9:  moveF_antihor(); break;
            case 10: moveB_antihor(); break;
            case 11: moveL_antihor(); break;
            case 12: moveR_antihor(); break;
        }
    }

    //imprime o cubo no estado atual
    void printCube() const {
        cout << "\n" << endl;
        cout << "     ╔════╗" << endl;
        cout << "     ║" << cor(U[0][0]) << cor(U[0][1]) << "║  " << endl;
        cout << "     ║" << cor(U[1][0]) << cor(U[1][1]) << "║" << endl;
        cout << "╔════╬════╬════╦════╗" << endl;
        cout << "║" << cor(L[0][0]) << cor(L[0][1]) << "║" << cor(F[0][0]) << cor(F[0][1]) << "║" << cor(R[0][0]) << cor(R[0][1]) << "║" << cor(B[0][0]) << cor(B[0][1]) << "║" << endl;
        cout << "║" << cor(L[1][0]) << cor(L[1][1]) << "║" << cor(F[1][0]) << cor(F[1][1]) << "║" << cor(R[1][0]) << cor(R[1][1]) << "║" << cor(B[1][0]) << cor(B[1][1]) << "║" << endl;
        cout << "╚════╬════╬════╩════╝" << endl;
        cout << "     ║" << cor(D[0][0]) << cor(D[0][1]) << "║" << endl;
        cout << "     ║" << cor(D[1][0]) << cor(D[1][1]) << "║ " << endl;
        cout << "     ╚════╝" << endl;
    }

    //gera um código hash único para o estado atual do cubo para usar na tabela de hash e verificar se o estado já foi visitado
    inline uint64_t getHashCode() const {
        uint64_t hash = 0;
        const int* data = reinterpret_cast<const int*>(this);
        for (int i = 0; i < 24; ++i) {
            hash |= ((uint64_t)(data[i] & 0x7)) << (i * 3);
        }
        return hash;   //retorna um inteiro de 64 bits representando o estado do cubo

    }

    //compara se dois objetos Cubo2x2 estão no mesmo estado
    bool operator==(const Cubo2x2& other) const {
        return memcmp(this, &other, sizeof(Cubo2x2)) == 0;//`memcmp` é uma forma muito rápida de comparar os bytes dos dois objetos
    }

    //movimentos
    inline void moveU() { 
        GirarFaceHor(U);
         int temp0 = 
            F[0][0], temp1 = F[0][1]; F[0][0] = R[0][0];
            F[0][1] = R[0][1]; R[0][0] = B[0][0];
            R[0][1] = B[0][1]; B[0][0] = L[0][0];
            B[0][1] = L[0][1];
            L[0][0] = temp0;
            L[0][1] = temp1; 
        }

    inline void moveU_antihor() {
         moveU(); moveU(); moveU();
        }

    inline void moveD() {
        GirarFaceHor(D);
        int temp0 = 
            F[1][0], temp1 = F[1][1]; F[1][0] = L[1][0];
            F[1][1] = L[1][1]; L[1][0] = B[1][0]; L[1][1] = B[1][1];
            B[1][0] = R[1][0];
            B[1][1] = R[1][1];
            R[1][0] = temp0;
            R[1][1] = temp1;
    }

    inline void moveD_antihor() {
        moveD(); moveD(); moveD();
    }

    inline void moveF() {
        GirarFaceHor(F);
        int tempU0 =
            U[1][0], tempU1 = U[1][1];
            U[1][0] = L[0][1];
            U[1][1] = L[1][1];
            L[0][1] = D[0][1];
            L[1][1] = D[0][0];
            D[0][1] = R[1][0];
            D[0][0] = R[0][0];
            R[1][0] = tempU0;
            R[0][0] = tempU1;
        }

    inline void moveF_antihor() {
        moveF(); moveF(); moveF(); 
    }

    inline void moveB() {
        GirarFaceHor(B);
        int tempU0 = 
            U[0][0], tempU1 = U[0][1];
            U[0][0] = R[0][1];
            U[0][1] = R[1][1];
            R[0][1] = D[1][1];
            R[1][1] = D[1][0];
            D[1][1] = L[1][0];
            D[1][0] = L[0][0];
            L[1][0] = tempU0;
            L[0][0] = tempU1;
    }

    inline void moveB_antihor() {
        moveB(); moveB(); moveB();
    }

    inline void moveL() { 
        GirarFaceHor(L);
         int tempU0 = 
            U[0][0], tempU1 = U[1][0];
            U[0][0] = B[1][0];
            U[1][0] = B[0][0];
            B[1][0] = D[0][0];
            B[0][0] = D[1][0];
            D[0][0] = F[0][0];
            D[1][0] = F[1][0];
            F[0][0] = tempU0;
            F[1][0] = tempU1;
    }

    inline void moveL_antihor() {
         moveL(); moveL(); moveL();
    }

    inline void moveR() {
        GirarFaceHor(R);
        int tempU0 = U[0][1], tempU1 = U[1][1];
        U[0][1] = F[0][1];
        U[1][1] = F[1][1];
        F[0][1] = D[0][1];
        F[1][1] = D[1][1];
        D[0][1] = B[1][1];
        D[1][1] = B[0][1];
        B[1][1] = tempU0;
        B[0][1] = tempU1;
    }

    inline void moveR_antihor() {
        moveR(); moveR(); moveR();
    }

    //verifica se o cubo está resolvido (FUNCAO AVALIADORA)
    inline bool estaResolvido() const {
        return (U[0][0] == U[0][1] && U[0][1] == U[1][0] && U[1][0] == U[1][1]) &&
               (D[0][0] == D[0][1] && D[0][1] == D[1][0] && D[1][0] == D[1][1]) &&
               (F[0][0] == F[0][1] && F[0][1] == F[1][0] && F[1][0] == F[1][1]) &&
               (B[0][0] == B[0][1] && B[0][1] == B[1][0] && B[1][0] == B[1][1]) &&
               (L[0][0] == L[0][1] && L[0][1] == L[1][0] && L[1][0] == L[1][1]) &&
               (R[0][0] == R[0][1] && R[0][1] == R[1][0] && R[1][0] == R[1][1]);
    }

    //para o algoritmo A*
    int calcularHeuristica() const {
        int pecas_fora_lugar = 0;
        const array<const Face*, 6> faces = {&U, &D, &F, &B, &L, &R};
        const int cores_corretas[6] = {0, 1, 2, 4, 5, 3}; //cores de U, D, F, B, L, R

        for (int i = 0; i < 6; ++i) {
            const Face& face = *faces[i];
            int cor_correta = cores_corretas[i];
            if (face[0][0] != cor_correta) pecas_fora_lugar++;
            if (face[0][1] != cor_correta) pecas_fora_lugar++;
            if (face[1][0] != cor_correta) pecas_fora_lugar++;
            if (face[1][1] != cor_correta) pecas_fora_lugar++;
        }
        return (pecas_fora_lugar + 7) / 8; //ela conta o número de peças fora do lugar e divide por 8 (o máximo de peças que um movimento pode corrigir)
    }//garante que a heurística seja "admissível" (nunca superestima o custo), o que é crucial para o A*


    //embaralha o cubo com um número de movimentos baseado no nível escolhido pelo usuário
    int embaralhar() {
        limparTela();


        //configuração do gerador de números aleatórios
        unsigned seed = chrono::high_resolution_clock::now().time_since_epoch().count();
        mt19937 gerador(seed);
        uniform_int_distribution<int> distribuicao(1, 12);
        ll numMovimentos = 0;

        cout << "╔════════════════════════════════╗" << endl;
        cout << "║      NÍVEL DE EMBARALHAMENTO   ║" << endl;
        cout << "╠════════════════════════════════╣" << endl;
        cout << "║ 1. Fácil (3 movimentos)        ║" << endl;
        cout << "║ 2. Médio (5 movimentos)        ║" << endl;
        cout << "║ 3. Difícil (10 movimentos)     ║" << endl;
        cout << "╚════════════════════════════════╝" << endl;
        cout << "Escolha o nivel (1-3): ";
        int nivel;
        cin >> nivel;

        switch (nivel) {
            case 1: numMovimentos = 3; break;
            case 2: numMovimentos = 5; break;
            case 3: numMovimentos = 10; break;
            default:
                cout << "Nivel invalido. Usando médio.\n";
                numMovimentos = 5;
                break;
        }

        int movimentoAnterior = -1;
        vector<string> movimentos_realizados;
        movimentos_realizados.reserve(numMovimentos);

        for (int i = 0; i < numMovimentos; ++i) {
            int movimento;
            do {
                movimento = distribuicao(gerador);
            } while (movimento == movimentoAnterior);

            movimentoAnterior = movimento;
            fazerMovimento(movimento);
            movimentos_realizados.push_back(getNomeMovimento(movimento));
        }

        limparTela();
        cout << "\nMovimentos realizados para embaralhar: ";
        for (const string& mov : movimentos_realizados) {
            cout << mov << " ";
        }
        cout << endl;
        cout << "\nCubo embaralhado:" << endl;
        printCube();

        return nivel;
    }
};

// --- Estrutura de hash personalizada ---
//fornece a função de hash que o 'unordered_set' usará
struct CuboHasher {
    inline size_t operator()(const Cubo2x2& cubo) const {
        return cubo.getHashCode();//ele simplesmente chama o método 'getHashCode' do próprio cubo
    }
};

// --- Estruturas para DFS ---
// Estrutura para armazenar o estado na busca DFS iterativa
struct EstadoDFS {
    Cubo2x2 cubo;
    vector<int> caminho;
};

//resolve o cubo usando DFS com limitação de profundidade iterativa
void resolverComDFS(Cubo2x2& cuboInicial) {
    //marca o tempo de início para medir a duração da busca
    auto inicio = chrono::high_resolution_clock::now();
    //contador para o número de estados que exploramos
    int estadosExplorados = 0;

    //este é o laço do aprofundamento iterativo, que aumenta o limite de profundidade a cada passagem
    for (int profundidade_max = 1; profundidade_max <= 14; ++profundidade_max) {
        limparTela();
        cout << "Procurando solucao com ate " << profundidade_max << " movimentos..." << endl;

        //a pilha é a estrutura central do DFS, guardando os estados a visitar, é um vetor que usamos como pilha (LIFO)
        vector<EstadoDFS> pilha;
        
        //adiciona o estado inicial na pilha para começar a busca
        pilha.push_back({cuboInicial, {}});
        
        //um conjunto para guardar os estados já visitados e evitar loops
        unordered_set<Cubo2x2, CuboHasher> visitados;
        visitados.insert(cuboInicial);


        //o laço principal da busca, executa enquanto houver estados na pilha
        while (!pilha.empty()) {
            //pega o estado do topo da pilha
            EstadoDFS estadoAtual = std::move(pilha.back());
            //remove o estado da pilha pois já vamos processá-lo
            pilha.pop_back();
            //incrementa o contador de estados explorados
            estadosExplorados++;

            //verifica se o estado atual é a solução
            if (estadoAtual.cubo.estaResolvido()) {
                //se encontramos a solução, calculamos o tempo e mostramos os resultados
                auto fim = chrono::high_resolution_clock::now();
                auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

                //imprime todas as informações da solução encontrada
                cout << "\n=== SOLUCAO ENCONTRADA PELO DFS ===" << endl;
                cout << "Numero de movimentos: " << estadoAtual.caminho.size() << endl;
                cout << "Estados explorados: " << estadosExplorados << endl;
                cout << "Tempo de busca: " << duracao.count() << " ms" << endl;
                cout << "Caminho: ";
                for (int moveId : estadoAtual.caminho) {
                    cout << cuboInicial.getNomeMovimento(moveId) << " ";
                }
                estadoAtual.cubo.printCube();
                cout << endl;
                //encerra a função pois já encontramos a solução
                return;
            }

            //se o caminho atual atingiu o limite de profundidade, não continuamos por ele
            if (estadoAtual.caminho.size() >= static_cast<size_t>(profundidade_max)) {
                    continue;
            }

            //gera os próximos estados a partir do atual (FUNCÇÃO SUCESSORA)
            //itera sobre os 12 movimentos possíveis
            for (int i = 1; i <= 12; ++i) {
                //cria uma cópia do cubo para aplicar o movimento
                Cubo2x2 proximoCubo = estadoAtual.cubo;
                //realiza o movimento
                proximoCubo.fazerMovimento(i);
                
                //só adiciona o novo estado na pilha se ele ainda não foi visitado
                if (visitados.find(proximoCubo) == visitados.end()) {
                    
                    //marca o novo estado como visitado
                    visitados.insert(proximoCubo);
                    
                    //cria o novo caminho adicionando o movimento atual
                    vector<int> proximoCaminho = estadoAtual.caminho;
                    proximoCaminho.push_back(i);

                    //adiciona o novo estado e seu caminho na pilha para ser explorado
                    pilha.push_back({proximoCubo, std::move(proximoCaminho)});
                }
            }
        }
    }
    //se o laço terminar sem encontrar solução
    cout << "\nNao foi possivel encontrar uma solucao em tempo habil." << endl;
}

// --- Estruturas de dados para o BFS ---

//armazena um estado completo na busca BFS: o cubo, o caminho percorrido e o número de movimentos
struct EstadoCubo {
    Cubo2x2 cubo;
    uint32_t caminho_encoded;//em vez de um vetor de movimentos, armazena a sequência de IDs de movimento em um único inteiro usando operações de bits, economizando memória
    uint8_t num_movimentos;
    EstadoCubo(const Cubo2x2& c) : cubo(c), caminho_encoded(0), num_movimentos(0) {}
};

//decodifica o caminho de movimentos armazenado no inteiro 'caminho_encoded' de volta para uma string legível
string decodificarCaminho(uint32_t encoded, uint8_t num_movs) {
    if (num_movs == 0) return "";
    const char* nomes[] = {"", "U", "D", "F", "B", "L", "R", "U'", "D'", "F'", "B'", "L'", "R'"};
    string resultado;
    resultado.reserve(num_movs * 3);
    for (int i = 0; i < num_movs; ++i) {
        if (i > 0) resultado += " ";
        int mov = (encoded >> (i * 4)) & 0xF;
        resultado += nomes[mov];
    }
    return resultado;
}

// --- ESTRUTURAS PARA A* ---

//representa um "nó" na árvore de busca do A*
struct NodeAStar {
    Cubo2x2 cubo; //o estado do cubo neste nó
    int g; //custo do início até o nó atual (nº de movimentos)
    int h; //custo estimado (heurística) do nó atual até a solução

    //em vez de um vetor, armazenamos o pai e o movimento que nos trouxe até aqui
    shared_ptr<NodeAStar> pai; //um ponteiro para o nó pai, para reconstruir o caminho no final
    int move_from_pai; //o movimento que levou do pai até este nó

    // Custo total f(n) = g(n) + h(n)
    int f() const {
        return g + h; //retorna o valor usado para priorizar os nós na busca
    }

    //sobrecarga do operador '>' para a fila de prioridade (que é uma max-heap por padrão)
    //queremos uma min-heap, então invertemos a lógica para que o menor 'f' tenha maior prioridade
    bool operator>(const NodeAStar& other) const {
        if (f() == other.f()) {
            return g < other.g;
        }
        return f() > other.f();
    }
};

//objeto de comparação para a fila de prioridade do A*
//a fila padrão é uma max-heap (prioriza o maior), mas queremos uma min-heap
//(priorizar o menor f()). Esta estrutura inverte a lógica de comparação para alcançar isso
struct CompareNodePtr {
    bool operator()(const shared_ptr<NodeAStar>& a, const shared_ptr<NodeAStar>& b) const {
        return *a > *b;
    }
};

//soluciona o cubo usando o algoritmo Breadth-First Search (BFS); explora todos os estados possíveis em camadas
void resolveComBFS(Cubo2x2& cuboInicial) {
    //se o cubo já está resolvido, não há nada a fazer.
    if (cuboInicial.estaResolvido()) {
        cout << "O cubo ja esta resolvido!" << endl;
        return;
    }

    //inicialização: prepara as estruturas de dados e o cronômetro.
    auto inicio = chrono::high_resolution_clock::now(); //inicia a contagem de tempo

    //a fila (deque) é o coração do BFS. Armazena os estados a serem visitados, fifo garantindo a exploração por camadas de profundidade
    deque<EstadoCubo> fila;

    
    //armazena um hash de cada estado já visitado para evitar trabalho redundante e loops infinitos
    unordered_set<Cubo2x2, CuboHasher> visitados;
    visitados.reserve(100000); //pré-aloca memória para evitar realocações custosas.

    //adiciona o estado inicial à fila e ao conjunto de visitados para começar a busca
    fila.emplace_back(cuboInicial);
    visitados.insert(cuboInicial);

    //movimentos
    //em vez de um 'switch-case' dentro do loop, usamos um array de ponteiros para os movimentos
    void (Cubo2x2::*movimentos[12])() = {
        &Cubo2x2::moveU, &Cubo2x2::moveU_antihor, &Cubo2x2::moveD, &Cubo2x2::moveD_antihor,
        &Cubo2x2::moveF, &Cubo2x2::moveF_antihor, &Cubo2x2::moveB, &Cubo2x2::moveB_antihor,
        &Cubo2x2::moveL, &Cubo2x2::moveL_antihor, &Cubo2x2::moveR, &Cubo2x2::moveR_antihor
    };

    const uint8_t mov_codes[12] = {1, 7, 2, 8, 3, 9, 4, 10, 5, 11, 6, 12};//mapeia o índice do array 'movimentos' para o ID público do movimento (1-12) para codificação do caminho

    int estadosExplorados = 0;
    const int MAX_PROFUNDIDADE = 8; //limite para evitar uso excessivo de memória

    //LOOP PRINCIPAL DA BUSCA: continua enquanto houver estados na fila para explorar
    while (!fila.empty()) {
        //pega o estado da frente da fila
        EstadoCubo estadoAtual = std::move(fila.front());//`std::move` é uma otimização que transfere a posse do objeto sem fazer uma cópia completa
        fila.pop_front(); //remove o estado da fila
        estadosExplorados++;

        //checa se o estado atual é a solução
        if (estadoAtual.cubo.estaResolvido()) {
            //calcula a duração da busca
            auto fim = chrono::high_resolution_clock::now();
            auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
            //converte o caminho codificado em uma string legível
            string caminho = decodificarCaminho(estadoAtual.caminho_encoded, estadoAtual.num_movimentos);

            //imprime os resultados e encerra a função
            cout << "\n=== SOLUCAO ENCONTRADA PELO BFS ===" << endl;
            cout << "Caminho: " << caminho << endl;
            cout << "Estados explorados: " << estadosExplorados << endl;
            cout << "Tempo de busca: " << duracao.count() << " ms" << endl;
            estadoAtual.cubo.printCube();
            return;
        }

        //pula a exploração se o caminho já for muito longo
        if (estadoAtual.num_movimentos >= MAX_PROFUNDIDADE) continue;

        //gera sucessores (FUNCAO SUCESSORA)
        //para o estado atual, gera todos os 12 estados possíveis aplicando cada movimento
        for (int i = 0; i < 12; ++i) {
            Cubo2x2 proximoCubo = estadoAtual.cubo; //cria uma cópia para modificar
            (proximoCubo.*movimentos[i])(); //aplica o movimento usando o ponteiro da função

            //verifica se o novo estado gerado ja foi visitado
            if (visitados.find(proximoCubo) == visitados.end()) { //se '.find' retorna '.end', não foi encontrado
                
                //se é um estado novo, o adicionamos ao conjunto de visitados
                visitados.insert(proximoCubo);
                
                //criamos um novo 'EstadoCubo' para este novo estado
                EstadoCubo proximoEstado(proximoCubo);
                proximoEstado.num_movimentos = estadoAtual.num_movimentos + 1;

                //Pega o caminho do pai e adiciona o novo movimento.
                //Usa o operador OR bit-a-bit (|) e deslocamento de bits (<<).
                //Cada movimento (1-12) precisa de 4 bits. Deslocamos o código do novo movimento
                //para a esquerda pelo número de movimentos já feitos * 4.
                proximoEstado.caminho_encoded = estadoAtual.caminho_encoded | (((uint32_t)mov_codes[i]) << (estadoAtual.num_movimentos * 4));
                
                //adiciona o novo estado ao final da fila para ser explorado no futuro
                fila.push_back(std::move(proximoEstado));
            }
        }
    }
    //se o loop terminar e nenhuma solução for encontrada (dentro do limite), informa o usuário
    cout << "Solucao nao encontrada dentro do limite de profundidade." << endl;
}

//soluciona o cubo usando o algoritmo A*; explora os estados mais promissores primeiro com base em uma heurística
void resolveComAStar(Cubo2x2& cuboInicial) {
    if (cuboInicial.estaResolvido()) {
        cout << "O cubo ja esta resolvido!" << endl;
        return;
    }

    //inicializa
    auto inicio = chrono::high_resolution_clock::now();

    //o 'openSet' é o coração do A*. É uma fila de prioridade
    //em vez de fifo, ela sempre mantém o nó com o menor custo `f()` no topo
    //o custo `f()` é a soma de `g()` (custo real até agora) e `h()` (custo estimado até o final)
    //usamos ponteiros inteligentes (`shared_ptr`) para gerenciar a memória dos nós,
    //que formam uma árvore de busca com links para seus "pais".
    priority_queue<shared_ptr<NodeAStar>, vector<shared_ptr<NodeAStar>>, CompareNodePtr> openSet;

    unordered_set<Cubo2x2, CuboHasher> closedSet; //armazena os estadosque já foram totalmente explorados para não os processar novamente.
    closedSet.reserve(200000);

    //cria o nó inicial para o estado de partida
    auto startNode = make_shared<NodeAStar>();
    startNode->cubo = cuboInicial;
    startNode->g = 0; //custo para chegar ao início é 0
    startNode->h = cuboInicial.calcularHeuristica(); //calcula a estimativa inicial
    startNode->pai = nullptr; //o nó inicial não tem pai
    startNode->move_from_pai = 0;

    //adiciona o nó inicial à fila de prioridade
    openSet.push(startNode);
    int estadosExplorados = 0;

    //continua enquanto houver nós promissores no openSet
    while (!openSet.empty()) {
        //pega o nó com a MAIOR prioridade (ou seja, o menor custo `f()`)
        auto atualNode = openSet.top();
        openSet.pop();
        estadosExplorados++;

        //se já exploramos este estado (possivelmente por um caminho mais caro que chegou aqui antes), pulamos
        if (closedSet.count(atualNode->cubo)) {
            continue;
        }
        //adiciona o estado atual ao closedSet para marcar como "explorado"
        closedSet.insert(atualNode->cubo);

        //checa se o nó atual contém a solução
        if (atualNode->cubo.estaResolvido()) {
            auto fim = chrono::high_resolution_clock::now();
            auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

            //RECONSTRUÇÃO DO CAMINHO:
            //diferente do BFS, o caminho não é armazenado em cada nó
            //em vez disso, subimos a árvore de busca do nó final até o inicial, seguindo os ponteiros `pai` e coletando os movimentos
            vector<int> caminho_final;
            auto tempNode = atualNode;
            while (tempNode->pai != nullptr) {
                caminho_final.push_back(tempNode->move_from_pai);
                tempNode = tempNode->pai; //sobe para o pai
            }
            //o caminho foi construído de trás para frente, então o invertemos
            reverse(caminho_final.begin(), caminho_final.end());

            cout << "\n=== SOLUCAO ENCONTRADA PELO A* ===" << endl;
            cout << "Movimentos: " << caminho_final.size() << endl;
            cout << "Estados explorados: " << estadosExplorados << endl;
            cout << "Tempo: " << duracao.count() << " ms" << endl;
            cout << "Sequencia: ";
            for (int moveId : caminho_final) {
                cout << cuboInicial.getNomeMovimento(moveId) << " ";
            }

            atualNode->cubo.printCube();
            cout << endl;            return;
        }

        //gera sucessores (FUNCAO SUCESSORA)
        //itera sobre todos os 12 movimentos possíveis para gerar os "vizinhos" do nó atual
        for (int i = 1; i <= 12; ++i) {
            Cubo2x2 proximoCubo = atualNode->cubo;
            proximoCubo.fazerMovimento(i);

            //se o estado vizinho já está no closedSet, ignoramos
            if (closedSet.find(proximoCubo) == closedSet.end()) {
                //cria um novo nó para o estado vizinho
                auto vizinhoNode = make_shared<NodeAStar>();
                vizinhoNode->cubo = proximoCubo;
                vizinhoNode->pai = atualNode; //o nó atual é o pai do vizinho
                vizinhoNode->move_from_pai = i; //armazena o movimento que nos trouxe aqui
                vizinhoNode->g = atualNode->g + 1; //o custo real é o do pai + 1
                vizinhoNode->h = vizinhoNode->cubo.calcularHeuristica(); //calcula a nova heurística

                //adiciona o novo nó vizinho à fila de prioridade
                //ela o posicionará automaticamente com base em seu custo `f() = g() + h()`
                openSet.push(vizinhoNode);
            }
        }
    }
    //se o openSet ficar vazio, significa que não há mais caminhos a explorar e nenhuma solução foi encontrada
    cout << "Nao foi possivel encontrar uma solucao." << endl;
}


void jogador(Cubo2x2& cubo) {
    ll movimento = 0;
    while (movimento != -1) {
        limparTela();
        
        cout << "---------------JOGADOR---------------\n" << endl;
        cubo.printCube();
        if (cubo.estaResolvido()) {
            cout << "\nCUBO RESOLVIDO!" << endl;
        }else{
            cout << "\nCUBO NAO RESOLVIDO!" << endl;
        }

        cout << "\n╔════════════════ MOVIMENTOS ══════════════════╗" << endl;
        cout << "║ Horario:    1-U  2-D  3-F  4-B  5-L  6-R     ║" << endl;
        cout << "║ Anti-hor.:  7-U' 8-D' 9-F' 10-B' 11-L' 12-R' ║" << endl;
        cout << "╠══════════════════════════════════════════════╣" << endl;
        cout << "║ Para SAIR, digite -1                         ║" << endl;
        cout << "╚══════════════════════════════════════════════╝" << endl;
        cout << "Digite o movimento desejado: ";
        cin >> movimento;

        if (movimento >= 1 && movimento <= 12) {
            cubo.fazerMovimento(movimento);
            
        } else if (movimento == -1) {
            cout << "\nSaindo..." << endl;
        } else {
            cout << "\nMovimento invalido!" << endl;
            cout << "\nPressione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
    }
}

int main() {
    #ifdef _WIN32
        //habilita a codificação UTF-8 no console do Windows
        system("chcp 65001 > nul");
    #endif

    habilitarCores();
    limparTela();

    Cubo2x2 cubo;

    cout << "BEM-VINDO AO SIMULADOR E SOLUCIONADOR DE CUBO MÁGICO 2x2x2!" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "Deseja jogar??" << endl;
    cout << "1- Sim" << endl;
    cout << "2- Nao" << endl;
    cout << "Escolha uma opcao: ";
    int jogar;
    cin >> jogar;
    if (jogar != 1) {
        cout << "Saindo..." << endl;
        return 0;
    }
    cout << "Cubo inicial (resolvido):" << endl;
    cubo.printCube();

    cout << "\n O que deseja fazer?" << endl;
    cout << "1- Interagir manualmente (Jogador)" << endl;
    cout << "2- Embaralhar o cubo" << endl;
    cout << "Escolha uma opcao: ";

    int acao, nivel;
    cin >> acao;

    switch(acao) {
        case 1:
            jogador(cubo);
            goto fim;
            break;
        case 2:
            nivel = cubo.embaralhar();
            break;
        default:
            limparTela();
            cout << "\nOpcao invalida." << endl;
            cubo.printCube();
    }

    if (nivel == 1 || nivel == 2) {
        cout << "\n Como deseja resolver o cubo?" << endl;
        cout << "1- Resolver manualmente (Jogador)" << endl;
        cout << "2- Solucionar com IA (BFS)" << endl;
        cout << "3- Solucionar com IA (DFS)" << endl;
        cout << "4- Solucionar com IA (A*)" << endl;
        cout << "Escolha uma opcao: ";
        int escolha;
        cin >> escolha;

        switch (escolha) {
            case 1:
                jogador(cubo);
                break;
            case 2:
                cout << "\nIniciando BFS..." << endl;
                resolveComBFS(cubo);
                break;
            case 3:
                cout << "\nIniciando DFS..." << endl;
                resolverComDFS(cubo);
                break;
            case 4:
                cout << "\nIniciando A*..." << endl;
                resolveComAStar(cubo);
                break;
            default:
                cout << "Opcao invalida." << endl;
                break;
        }
    } else if (nivel == 3) {
        cout << "\n Como deseja resolver o cubo?" << endl;
        cout << "1- Resolver manualmente (Jogador)" << endl;
        cout << "2- Solucionar com IA (A*)" << endl;
        cout << "Escolha uma opcao: ";
        int escolha;
        cin >> escolha;

        switch (escolha) {
            case 1:
                jogador(cubo);
                break;
            case 2:
                cout << "\nIniciando A*..." << endl;
                resolveComAStar(cubo);
                break;
            default:
                cout << "Opcao invalida." << endl;
                break;
        }
    }

    fim:
    cout << "\n Deseja jogar Novamente? (1- Sim, 2- Nao): ";
    int jogarNovamente;
    cin >> jogarNovamente;

    switch(jogarNovamente) {
        case 1:
            main();
            break;
        case 2:
            cout << "Saindo..." << endl;
            break;
        default:
            cout << "Opcao invalida. Saindo..." << endl;
            break;
    }

    return 0;
}