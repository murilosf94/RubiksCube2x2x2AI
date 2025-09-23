#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <deque> // Usado no BFS
#include <unordered_set>
#include <array>
#include <cstring>
#include <queue>      // para a fila de prioridade do A*
#include <functional> // std::greater no A*
#include <memory>     // std::shared_ptr
#include <algorithm>  // std::reverse

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

using namespace std;
#define ll long long

// --- Funções de Interface ---
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

// --- Estrutura de Dados otimizada ---
using Face = array<array<int, 2>, 2>;

class Cubo2x2 {
private:
    Face U, D, F, B, L, R;

    inline void GirarFaceHor(Face& face) {
        int temp = face[0][0];
        face[0][0] = face[1][0];
        face[1][0] = face[1][1];
        face[1][1] = face[0][1];
        face[0][1] = temp;
    }

    string cor(int color) const {
        string block = "██";
        switch (color) {
            case 0: return "\033[1;37m" + block + "\033[0m"; // Branco (U)
            case 1: return "\033[1;33m" + block + "\033[0m"; // Amarelo (D)
            case 2: return "\033[1;34m" + block + "\033[0m"; // Azul (F)
            case 3: return "\033[1;38;5;208m" + block + "\033[0m"; // Laranja (R)
            case 4: return "\033[1;32m" + block + "\033[0m"; // Verde (B)
            case 5: return "\033[1;31m" + block + "\033[0m"; // Vermelho (L)
            default: return "??";
        }
    }

    bool dfs(size_t profundidade_max, vector<int>& caminho) {
        if (this->estaResolvido()) {
            return true;
        }
        if (caminho.size() >= profundidade_max) {
            return false;
        }

        for (int i = 1; i <= 12; ++i) {
            fazerMovimento(i);
            caminho.push_back(i);
            if (dfs(profundidade_max, caminho)) {
                return true;
            }
            caminho.pop_back();
            fazerMovimentoInverso(i);
        }
        return false;
    }

    void fazerMovimentoInverso(int moveId) {
        switch (moveId) {
            case 1: moveU_antihor(); break;
            case 2: moveD_antihor(); break;
            case 3: moveF_antihor(); break;
            case 4: moveB_antihor(); break;
            case 5: moveL_antihor(); break;
            case 6: moveR_antihor(); break;
            case 7: moveU(); break;
            case 8: moveD(); break;
            case 9: moveF(); break;
            case 10: moveB(); break;
            case 11: moveL(); break;
            case 12: moveR(); break;
        }
    }

public:
    Cubo2x2() {
        U = {{{0, 0}, {0, 0}}};
        D = {{{1, 1}, {1, 1}}};
        F = {{{2, 2}, {2, 2}}};
        B = {{{4, 4}, {4, 4}}};
        L = {{{5, 5}, {5, 5}}};
        R = {{{3, 3}, {3, 3}}};
    }

    string getNomeMovimento(int moveId) {
        const char* nomes[] = {"", "U", "D", "F", "B", "L", "R", "U'", "D'", "F'", "B'", "L'", "R'"};
        return nomes[moveId];
    }

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

    inline uint64_t getHashCode() const {
        uint64_t hash = 0;
        const int* data = reinterpret_cast<const int*>(this);
        for (int i = 0; i < 24; ++i) {
            hash |= ((uint64_t)(data[i] & 0x7)) << (i * 3);
        }
        return hash;
    }

    bool operator==(const Cubo2x2& other) const {
        return memcmp(this, &other, sizeof(Cubo2x2)) == 0;
    }

    // --- Movimentos ---
    inline void moveU() { GirarFaceHor(U); int temp0 = F[0][0], temp1 = F[0][1]; F[0][0] = R[0][0]; F[0][1] = R[0][1]; R[0][0] = B[0][0]; R[0][1] = B[0][1]; B[0][0] = L[0][0]; B[0][1] = L[0][1]; L[0][0] = temp0; L[0][1] = temp1; }
    inline void moveU_antihor() { moveU(); moveU(); moveU(); }
    inline void moveD() { GirarFaceHor(D); int temp0 = F[1][0], temp1 = F[1][1]; F[1][0] = L[1][0]; F[1][1] = L[1][1]; L[1][0] = B[1][0]; L[1][1] = B[1][1]; B[1][0] = R[1][0]; B[1][1] = R[1][1]; R[1][0] = temp0; R[1][1] = temp1; }
    inline void moveD_antihor() { moveD(); moveD(); moveD(); }
    inline void moveF() { GirarFaceHor(F); int tempU0 = U[1][0], tempU1 = U[1][1]; U[1][0] = L[0][1]; U[1][1] = L[1][1]; L[0][1] = D[0][1]; L[1][1] = D[0][0]; D[0][1] = R[1][0]; D[0][0] = R[0][0]; R[1][0] = tempU0; R[0][0] = tempU1; }
    inline void moveF_antihor() { moveF(); moveF(); moveF(); }
    inline void moveB() { GirarFaceHor(B); int tempU0 = U[0][0], tempU1 = U[0][1]; U[0][0] = R[0][1]; U[0][1] = R[1][1]; R[0][1] = D[1][1]; R[1][1] = D[1][0]; D[1][1] = L[1][0]; D[1][0] = L[0][0]; L[1][0] = tempU0; L[0][0] = tempU1; }
    inline void moveB_antihor() { moveB(); moveB(); moveB(); }
    inline void moveL() { GirarFaceHor(L); int tempU0 = U[0][0], tempU1 = U[1][0]; U[0][0] = B[1][0]; U[1][0] = B[0][0]; B[1][0] = D[0][0]; B[0][0] = D[1][0]; D[0][0] = F[0][0]; D[1][0] = F[1][0]; F[0][0] = tempU0; F[1][0] = tempU1; }
    inline void moveL_antihor() { moveL(); moveL(); moveL(); }
    inline void moveR() { GirarFaceHor(R); int tempU0 = U[0][1], tempU1 = U[1][1]; U[0][1] = F[0][1]; U[1][1] = F[1][1]; F[0][1] = D[0][1]; F[1][1] = D[1][1]; D[0][1] = B[1][1]; D[1][1] = B[0][1]; B[1][1] = tempU0; B[0][1] = tempU1; }
    inline void moveR_antihor() { moveR(); moveR(); moveR(); }

    inline bool estaResolvido() const {
        return (U[0][0] == U[0][1] && U[0][1] == U[1][0] && U[1][0] == U[1][1]) &&
               (D[0][0] == D[0][1] && D[0][1] == D[1][0] && D[1][0] == D[1][1]) &&
               (F[0][0] == F[0][1] && F[0][1] == F[1][0] && F[1][0] == F[1][1]) &&
               (B[0][0] == B[0][1] && B[0][1] == B[1][0] && B[1][0] == B[1][1]) &&
               (L[0][0] == L[0][1] && L[0][1] == L[1][0] && L[1][0] == L[1][1]) &&
               (R[0][0] == R[0][1] && R[0][1] == R[1][0] && R[1][0] == R[1][1]);
    }

    int calcularHeuristica() const {
        int pecas_fora_lugar = 0;
        const array<const Face*, 6> faces = {&U, &D, &F, &B, &L, &R};
        const int cores_corretas[6] = {0, 1, 2, 4, 5, 3}; // Cores de U, D, F, B, L, R

        for (int i = 0; i < 6; ++i) {
            const Face& face = *faces[i];
            int cor_correta = cores_corretas[i];
            if (face[0][0] != cor_correta) pecas_fora_lugar++;
            if (face[0][1] != cor_correta) pecas_fora_lugar++;
            if (face[1][0] != cor_correta) pecas_fora_lugar++;
            if (face[1][1] != cor_correta) pecas_fora_lugar++;
        }
        return (pecas_fora_lugar + 7) / 8;
    }


    int embaralhar() {
        limparTela();
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

    void resolverComDFS() {
        for (int profundidade_max = 1; profundidade_max <= 14; ++profundidade_max) {
            limparTela();
            cout << "Procurando solucao com ate " << profundidade_max << " movimentos..." << endl;
            vector<int> caminho;
            Cubo2x2 copia = *this;
            if (copia.dfs(profundidade_max, caminho)) {
                cout << "\nSolucao encontrada!" << endl;
                cout << "Numero de movimentos: " << caminho.size() << endl;
                cout << "Caminho: ";
                for (int moveId : caminho) {
                    cout << getNomeMovimento(moveId) << " ";
                }
                printCube();
                cout << endl;
                return;
            }
        }
        cout << "\nNao foi possivel encontrar uma solucao em tempo habil." << endl;
    }
};

// --- Estruturas de dados para o Hasher (usado em BFS e A*) ---
struct CuboHasher {
    inline size_t operator()(const Cubo2x2& cubo) const {
        return cubo.getHashCode();
    }
};

// --- Estruturas de dados para o BFS ---
struct EstadoCubo {
    Cubo2x2 cubo;
    uint32_t caminho_encoded;
    uint8_t num_movimentos;
    EstadoCubo(const Cubo2x2& c) : cubo(c), caminho_encoded(0), num_movimentos(0) {}
};

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

// --- Solver BFS ---
void resolveComBFS(Cubo2x2& cuboInicial) {
    if (cuboInicial.estaResolvido()) {
        cout << "O cubo ja esta resolvido!" << endl;
        return;
    }
    auto inicio = chrono::high_resolution_clock::now();
    deque<EstadoCubo> fila;
    unordered_set<Cubo2x2, CuboHasher> visitados;
    visitados.reserve(100000);
    fila.emplace_back(cuboInicial);
    visitados.insert(cuboInicial);

    void (Cubo2x2::*movimentos[12])() = {
        &Cubo2x2::moveU, &Cubo2x2::moveU_antihor, &Cubo2x2::moveD, &Cubo2x2::moveD_antihor,
        &Cubo2x2::moveF, &Cubo2x2::moveF_antihor, &Cubo2x2::moveB, &Cubo2x2::moveB_antihor,
        &Cubo2x2::moveL, &Cubo2x2::moveL_antihor, &Cubo2x2::moveR, &Cubo2x2::moveR_antihor
    };
    const uint8_t mov_codes[12] = {1, 7, 2, 8, 3, 9, 4, 10, 5, 11, 6, 12};
    int estadosExplorados = 0;
    const int MAX_PROFUNDIDADE = 8;

    while (!fila.empty()) {
        EstadoCubo estadoAtual = std::move(fila.front());
        fila.pop_front();
        estadosExplorados++;

        if (estadoAtual.cubo.estaResolvido()) {
            auto fim = chrono::high_resolution_clock::now();
            auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
            string caminho = decodificarCaminho(estadoAtual.caminho_encoded, estadoAtual.num_movimentos);

            cout << "\n=== SOLUCAO ENCONTRADA PELO BFS ===" << endl;
            estadoAtual.cubo.printCube();
            cout << "Movimentos: " << (int)estadoAtual.num_movimentos << endl;
            cout << "Estados explorados: " << estadosExplorados << endl;
            cout << "Tempo: " << duracao.count() << " ms" << endl;
            cout << "Sequencia: " << caminho << endl;
            return;
        }

        if (estadoAtual.num_movimentos >= MAX_PROFUNDIDADE) continue;

        for (int i = 0; i < 12; ++i) {
            Cubo2x2 proximoCubo = estadoAtual.cubo;
            (proximoCubo.*movimentos[i])();
            if (visitados.find(proximoCubo) == visitados.end()) {
                visitados.insert(proximoCubo);
                EstadoCubo proximoEstado(proximoCubo);
                proximoEstado.num_movimentos = estadoAtual.num_movimentos + 1;
                proximoEstado.caminho_encoded = estadoAtual.caminho_encoded | (((uint32_t)mov_codes[i]) << (estadoAtual.num_movimentos * 4));
                fila.push_back(std::move(proximoEstado));
            }
        }
    }
    cout << "Solucao nao encontrada dentro do limite de profundidade." << endl;
}


// Estrutura para representar um nó na busca A* (MODIFICADA)
struct NodeAStar {
    Cubo2x2 cubo;
    int g; // Custo do início até o nó atual (nº de movimentos)
    int h; // Custo estimado (heurística) do nó atual até o objetivo

    // Em vez de um vetor, armazenamos o pai e o movimento que nos trouxe até aqui
    shared_ptr<NodeAStar> parent;
    int move_from_parent;

    // Custo total f(n) = g(n) + h(n)
    int f() const {
        return g + h;
    }

    // Sobrecarga do operador '>' para a fila de prioridade (que é uma max-heap por padrão)
    // Queremos uma min-heap, então invertemos a lógica para que o menor 'f' tenha maior prioridade.
    bool operator>(const NodeAStar& other) const {
        if (f() == other.f()) {
            return g < other.g;
        }
        return f() > other.f();
    }
};

// Estrutura de comparação para a fila de prioridade usar com ponteiros
struct CompareNodePtr {
    bool operator()(const shared_ptr<NodeAStar>& a, const shared_ptr<NodeAStar>& b) const {
        return *a > *b; // Dereferencia os ponteiros para usar o operator> do NodeAStar
    }
};

void resolveComAStar(Cubo2x2& cuboInicial) {
    if (cuboInicial.estaResolvido()) {
        cout << "O cubo ja esta resolvido!" << endl;
        return;
    }

    auto inicio = chrono::high_resolution_clock::now();

    // Fila de prioridade agora armazena ponteiros e usa o comparador customizado
    priority_queue<shared_ptr<NodeAStar>, vector<shared_ptr<NodeAStar>>, CompareNodePtr> openSet;

    // Conjunto para armazenar os estados já visitados
    unordered_set<Cubo2x2, CuboHasher> closedSet;
    closedSet.reserve(200000);

    // Criando o nó inicial com shared_ptr
    auto startNode = make_shared<NodeAStar>();
    startNode->cubo = cuboInicial;
    startNode->g = 0;
    startNode->h = cuboInicial.calcularHeuristica();
    startNode->parent = nullptr;
    startNode->move_from_parent = 0; // Nenhum movimento para o nó inicial

    openSet.push(startNode);
    int estadosExplorados = 0;

    while (!openSet.empty()) {
        // Pega o nó com o menor f() da fila (agora é um ponteiro)
        auto currentNode = openSet.top();
        openSet.pop();
        estadosExplorados++;

        // Se o estado já foi visitado, ignora.
        if (closedSet.count(currentNode->cubo)) {
            continue;
        }
        closedSet.insert(currentNode->cubo);

        // Verifica se chegamos ao estado resolvido
        if (currentNode->cubo.estaResolvido()) {
            auto fim = chrono::high_resolution_clock::now();
            auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

            // --- RECONSTRUÇÃO DO CAMINHO ---
            vector<int> caminho_final;
            auto tempNode = currentNode;
            while (tempNode->parent != nullptr) {
                caminho_final.push_back(tempNode->move_from_parent);
                tempNode = tempNode->parent;
            }
            reverse(caminho_final.begin(), caminho_final.end());

            cout << "\n=== SOLUCAO ENCONTRADA PELO A* ===" << endl;
            cout << "Movimentos: " << caminho_final.size() << endl;
            cout << "Estados explorados: " << estadosExplorados << endl;
            cout << "Tempo: " << duracao.count() << " ms" << endl;
            cout << "Sequencia: ";
            for (int moveId : caminho_final) {
                cout << cuboInicial.getNomeMovimento(moveId) << " ";
            }
            currentNode->cubo.printCube();
            cout << endl;
            return;
        }

        // Gera os sucessores
        for (int i = 1; i <= 12; ++i) {
            Cubo2x2 proximoCubo = currentNode->cubo;
            proximoCubo.fazerMovimento(i);

            if (closedSet.find(proximoCubo) == closedSet.end()) {
                auto neighborNode = make_shared<NodeAStar>();
                neighborNode->cubo = proximoCubo;
                neighborNode->parent = currentNode;
                neighborNode->move_from_parent = i;
                neighborNode->g = currentNode->g + 1;
                neighborNode->h = neighborNode->cubo.calcularHeuristica();
                openSet.push(neighborNode);
            }
        }
    }
    cout << "Nao foi possivel encontrar uma solucao." << endl;
}


// --- Modo Jogador ---
void jogador(Cubo2x2& cubo) {
    ll movimento = 0;
    while (movimento != -1) {
        limparTela();
        cout << "---------------JOGADOR---------------\n" << endl;
        cubo.printCube();
        cout << "\n╔═══════════════ MOVIMENTOS ═══════════════╗" << endl;
        cout << "║ Horario:    1-U  2-D  3-F  4-B  5-L  6-R   ║" << endl;
        cout << "║ Anti-hor.:  7-U' 8-D' 9-F' 10-B' 11-L' 12-R' ║" << endl;
        cout << "╠════════════════════════════════════════════╣" << endl;
        cout << "║ Para SAIR, digite -1                       ║" << endl;
        cout << "╚════════════════════════════════════════════╝" << endl;
        cout << "Digite o movimento desejado: ";
        cin >> movimento;

        if (movimento >= 1 && movimento <= 12) {
            cubo.fazerMovimento(movimento);
            if (cubo.estaResolvido()) {
                limparTela();
                cubo.printCube();
                cout << "\n\033[1;32mCUBO RESOLVIDO!\033[0m" << endl;
                movimento = -1; // Sair do loop
            }
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
        // Habilita a codificação UTF-8 no console do Windows
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
            return 0;
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
                cubo.resolverComDFS();
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