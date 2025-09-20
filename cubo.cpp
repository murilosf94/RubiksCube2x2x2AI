#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <queue>
#include <unordered_set>
#include <functional>
#include <array>
#include <cstring>

using namespace std;
#define ll long long
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr);

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

    char cor(int color) const {
        static const char cores[] = {'W', 'Y', 'B', 'O', 'G', 'R'};
        return (color >= 0 && color <= 5) ? cores[color] : '?';
    }

public:
    Cubo2x2() {
        U = {{{0, 0}, {0, 0}}}; D = {{{1, 1}, {1, 1}}};
        F = {{{2, 2}, {2, 2}}}; B = {{{4, 4}, {4, 4}}};
        L = {{{5, 5}, {5, 5}}}; R = {{{3, 3}, {3, 3}}};
    }

    void printCube() const {
        cout << "    " << cor(U[0][0]) << " " << cor(U[0][1]) << '\n';
        cout << "    " << cor(U[1][0]) << " " << cor(U[1][1]) << '\n';

        for (int i = 0; i < 2; ++i) {
            cout << cor(L[i][0]) << " " << cor(L[i][1]) << " "
                 << cor(F[i][0]) << " " << cor(F[i][1]) << " "
                 << cor(R[i][0]) << " " << cor(R[i][1]) << " "
                 << cor(B[i][0]) << " " << cor(B[i][1]) << '\n';
        }

        cout << "    " << cor(D[0][0]) << " " << cor(D[0][1]) << '\n';
        cout << "    " << cor(D[1][0]) << " " << cor(D[1][1]) << '\n';
    }

    inline uint64_t getHashCode() const {
        uint64_t hash = 0;
        
        // Compactar todos os valores em um único uint64_t
        // Cada posição usa 3 bits (0-5), total = 24 posições * 3 bits = 72 bits
        const int* data = reinterpret_cast<const int*>(&U);
        for (int i = 0; i < 24; ++i) {
            hash |= ((uint64_t)(data[i] & 0x7)) << (i * 3);
        }
        return hash;
    }

    bool operator==(const Cubo2x2& other) const {
        return memcmp(this, &other, sizeof(Cubo2x2)) == 0;
    }

    inline void moveU() {
        GirarFaceHor(U);
        int temp0 = F[0][0], temp1 = F[0][1];
        F[0][0] = R[0][0]; F[0][1] = R[0][1];
        R[0][0] = B[0][0]; R[0][1] = B[0][1];
        B[0][0] = L[0][0]; B[0][1] = L[0][1];
        L[0][0] = temp0; L[0][1] = temp1;
    }
    
    inline void moveU_antihor() { moveU(); moveU(); moveU(); }

    inline void moveD() {
        GirarFaceHor(D);
        int temp0 = F[1][0], temp1 = F[1][1];
        F[1][0] = L[1][0]; F[1][1] = L[1][1];
        L[1][0] = B[1][0]; L[1][1] = B[1][1];
        B[1][0] = R[1][0]; B[1][1] = R[1][1];
        R[1][0] = temp0; R[1][1] = temp1;
    }

    inline void moveD_antihor() { moveD(); moveD(); moveD(); }

    inline void moveF() {
        GirarFaceHor(F);
        int tempU0 = U[1][0], tempU1 = U[1][1];
        U[1][0] = L[0][1]; U[1][1] = L[1][1];
        L[0][1] = D[0][1]; L[1][1] = D[0][0];
        D[0][1] = R[1][0]; D[0][0] = R[0][0];
        R[1][0] = tempU0; R[0][0] = tempU1;
    }

    inline void moveF_antihor() { moveF(); moveF(); moveF(); }

    inline void moveB() {
        GirarFaceHor(B);
        int tempU0 = U[0][0], tempU1 = U[0][1];
        U[0][0] = R[0][1]; U[0][1] = R[1][1];
        R[0][1] = D[1][1]; R[1][1] = D[1][0];
        D[1][1] = L[1][0]; D[1][0] = L[0][0];
        L[1][0] = tempU0; L[0][0] = tempU1;
    }

    inline void moveB_antihor() { moveB(); moveB(); moveB(); }

    inline void moveL() {
        GirarFaceHor(L);
        int tempU0 = U[0][0], tempU1 = U[1][0];
        U[0][0] = B[1][0]; U[1][0] = B[0][0];
        B[1][0] = D[0][0]; B[0][0] = D[1][0];
        D[0][0] = F[0][0]; D[1][0] = F[1][0];
        F[0][0] = tempU0; F[1][0] = tempU1;
    }
    
    inline void moveL_antihor() { moveL(); moveL(); moveL(); }

    inline void moveR() {
        GirarFaceHor(R);
        int tempU0 = U[0][1], tempU1 = U[1][1];
        U[0][1] = F[0][1]; U[1][1] = F[1][1];
        F[0][1] = D[0][1]; F[1][1] = D[1][1];
        D[0][1] = B[1][1]; D[1][1] = B[0][1];
        B[1][1] = tempU0; B[0][1] = tempU1;
    }

    inline void moveR_antihor() { moveR(); moveR(); moveR(); }

    inline bool estaResolvido() const {
        return (U[0][0] == U[0][1] && U[0][1] == U[1][0] && U[1][0] == U[1][1]) &&
               (D[0][0] == D[0][1] && D[0][1] == D[1][0] && D[1][0] == D[1][1]) &&
               (F[0][0] == F[0][1] && F[0][1] == F[1][0] && F[1][0] == F[1][1]) &&
               (B[0][0] == B[0][1] && B[0][1] == B[1][0] && B[1][0] == B[1][1]) &&
               (L[0][0] == L[0][1] && L[0][1] == L[1][0] && L[1][0] == L[1][1]) &&
               (R[0][0] == R[0][1] && R[0][1] == R[1][0] && R[1][0] == R[1][1]);
    }

    void embaralhar() {
        unsigned seed = chrono::high_resolution_clock::now()
                        .time_since_epoch().count();
        mt19937 gerador(seed);
        uniform_int_distribution<int> distribuicao(1, 12);

        ll numMovimentos = 0;
        cout << "\n---------------EMBARALHAMENTO---------------\n" << endl;
        cout << "1- Nivel facil: 5 movimentos" << endl;
        cout << "2- Nivel medio: 10 movimentos" << endl;
        cout << "3- Nivel dificil: 15 movimentos" << endl;

        cout << "Escolha o nivel de embaralhamento (1-3): ";
        int nivel;
        cin >> nivel;

        switch (nivel) {
            case 1: numMovimentos = 5; break;
            case 2: numMovimentos = 10; break;
            case 3: numMovimentos = 15; break;
            default:
                cout << "Nivel invalido. Usando nivel facil por padrao." << endl;
                numMovimentos = 5;
                break;
        }

        int movimentoAnterior = -1;
        vector<string> movimentos_realizados;
        movimentos_realizados.reserve(numMovimentos); 

        void (Cubo2x2::*movs[13])() = {
            nullptr, &Cubo2x2::moveU, &Cubo2x2::moveD, &Cubo2x2::moveF, &Cubo2x2::moveB,
            &Cubo2x2::moveL, &Cubo2x2::moveR, &Cubo2x2::moveU_antihor, &Cubo2x2::moveD_antihor,
            &Cubo2x2::moveF_antihor, &Cubo2x2::moveB_antihor, &Cubo2x2::moveL_antihor, &Cubo2x2::moveR_antihor
        };

        const char* nomes[13] = {
            "", "U", "D", "F", "B", "L", "R", "U'", "D'", "F'", "B'", "L'", "R'"
        };

        for (int i = 0; i < numMovimentos; ++i) {
            int movimento;
            do {
                movimento = distribuicao(gerador);
            } while (movimento == movimentoAnterior);
            
            movimentoAnterior = movimento;
            (this->*movs[movimento])();
            movimentos_realizados.push_back(nomes[movimento]);
        }

        cout << "\nMovimentos realizados: ";
        for (const string& mov : movimentos_realizados) {
            cout << mov << " ";
        }
        cout << endl;

        cout << "\nCubo embaralhado:" << endl;
        printCube();
    }
};

struct CuboHasher {
    inline size_t operator()(const Cubo2x2& cubo) const {
        return cubo.getHashCode();
    }
};

struct EstadoCubo {
    Cubo2x2 cubo;
    uint32_t caminho_encoded; //codificar movimentos em bits
    uint8_t num_movimentos;
    
    EstadoCubo() : caminho_encoded(0), num_movimentos(0) {}
    EstadoCubo(const Cubo2x2& c) : cubo(c), caminho_encoded(0), num_movimentos(0) {}
};

string decodificarCaminho(uint32_t encoded, uint8_t num_movs) {
    if (num_movs == 0) return "";
    
    const char* nomes[] = {"", "U", "D", "F", "B", "L", "R", "U'", "D'", "F'", "B'", "L'", "R'"};
    string resultado;
    resultado.reserve(num_movs * 3); //estimativa de tamanho
    
    for (int i = 0; i < num_movs; ++i) {
        if (i > 0) resultado += " ";
        int mov = (encoded >> (i * 4)) & 0xF;
        resultado += nomes[mov];
    }
    return resultado;
}

void resolveComBFS(Cubo2x2& cuboInicial) {
    if (cuboInicial.estaResolvido()) {
        cout << "O cubo ja esta resolvido!" << endl;
        return;
    }

    auto inicio = chrono::high_resolution_clock::now();

    //usar deque ao invés de queue para melhor cache locality
    deque<EstadoCubo> fila;
    unordered_set<Cubo2x2, CuboHasher> visitados;
    
    visitados.reserve(100000);

    fila.emplace_back(cuboInicial);
    visitados.insert(cuboInicial);

    void (Cubo2x2::*movimentos[12])() = {
    &Cubo2x2::moveU,   &Cubo2x2::moveU_antihor,
    &Cubo2x2::moveD,   &Cubo2x2::moveD_antihor,
    &Cubo2x2::moveF,   &Cubo2x2::moveF_antihor,
    &Cubo2x2::moveB,   &Cubo2x2::moveB_antihor,
    &Cubo2x2::moveL,   &Cubo2x2::moveL_antihor,
    &Cubo2x2::moveR,   &Cubo2x2::moveR_antihor
};

    const uint8_t mov_codes[12] = {1, 7, 2, 8, 3, 9, 4, 10, 5, 11, 6, 12};

    int estadosExplorados = 0;
    const int MAX_ESTADOS = 4000000; 
    const int MAX_PROFUNDIDADE = 30; 

    while (!fila.empty() && estadosExplorados < MAX_ESTADOS) {
        EstadoCubo estadoAtual = std::move(fila.front());
        fila.pop_front();
        estadosExplorados++;

        if (estadoAtual.cubo.estaResolvido()) {
            auto fim = chrono::high_resolution_clock::now();
            auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
            
            string caminho = decodificarCaminho(estadoAtual.caminho_encoded, estadoAtual.num_movimentos);
            
            cout << "\n=== SOLUCAO ENCONTRADA ===" << endl;
            cout << "Movimentos necessarios: " << (int)estadoAtual.num_movimentos << endl;
            cout << "Estados explorados: " << estadosExplorados << endl;
            cout << "Estados na memoria: " << visitados.size() << endl;
            cout << "Tempo de execucao: " << duracao.count() << " ms" << endl;
            cout << "Velocidade: " << (estadosExplorados * 1000 / max(1, (int)duracao.count())) << " estados/segundo" << endl;
            cout << "Sequencia: " << caminho << endl;
            cout << "\nEstado final:" << endl;
            estadoAtual.cubo.printCube();
            return;
        }

        if (estadoAtual.num_movimentos >= MAX_PROFUNDIDADE) continue;

        for (int i = 0; i < 12; ++i) {
            Cubo2x2 proximoCubo = estadoAtual.cubo;
            (proximoCubo.*movimentos[i])();

            if (visitados.find(proximoCubo) == visitados.end()) {
                visitados.insert(proximoCubo);
                
                EstadoCubo proximoEstado;
                proximoEstado.cubo = std::move(proximoCubo);
                proximoEstado.num_movimentos = estadoAtual.num_movimentos + 1;
                
                if (proximoEstado.num_movimentos <= 6) { 
                    proximoEstado.caminho_encoded = estadoAtual.caminho_encoded | 
                        (((uint32_t)mov_codes[i]) << (estadoAtual.num_movimentos * 4));
                }
                
                fila.push_back(std::move(proximoEstado));
            }
        }

        if (estadosExplorados % 100000 == 0) {
            cout << "Estados: " << estadosExplorados << ", Fila: " << fila.size() 
                 << ", Memoria: " << visitados.size() << endl;
        }
    }

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

    if (estadosExplorados >= MAX_ESTADOS) {
        cout << "Limite de estados atingido após " << duracao.count() << " ms" << endl;
        cout << "Tente um embaralhamento mais simples." << endl;
    } else {
        cout << "Solução não encontrada em " << duracao.count() << " ms" << endl;
    }
}

void jogador(Cubo2x2& cubo) {
    ll movimento = 0;
    while(movimento != -1) {
        cout << "\n---------------JOGADOR---------------\n" << endl;
        cubo.printCube();
        cout << "\n1-U, 2-D, 3-F, 4-B, 5-L, 6-R" << endl;
        cout << "7-U', 8-D', 9-F', 10-B', 11-L', 12-R'" << endl;
        cout << "SAIR: -1" << endl;
        cout << "Digite o movimento: ";
        cin >> movimento;

        void (Cubo2x2::*movs[13])() = {
            nullptr, &Cubo2x2::moveU, &Cubo2x2::moveD, &Cubo2x2::moveF, &Cubo2x2::moveB,
            &Cubo2x2::moveL, &Cubo2x2::moveR, &Cubo2x2::moveU_antihor, &Cubo2x2::moveD_antihor,
            &Cubo2x2::moveF_antihor, &Cubo2x2::moveB_antihor, &Cubo2x2::moveL_antihor, &Cubo2x2::moveR_antihor
        };

        if (movimento >= 1 && movimento <= 12) {
            (cubo.*movs[movimento])();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO!" << endl;
            } else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
        } else if (movimento == -1) {
            cout << "\nSaindo..." << endl;
        } else {
            cout << "\nMovimento invalido!" << endl;
        }
    }
}

int main() {
    fastio;
    
    Cubo2x2 cubo;

    cout << "Cubo inicial (resolvido):" << endl;
    cubo.printCube();

    cout << "\nDeseja embaralhar o cubo?" << endl;
    cout << "1- Sim\n2- Nao" << endl;
    int opcao;
    cin >> opcao;

    if (opcao == 1) {
        cubo.embaralhar();
    } else {
        cout << "\nCubo nao embaralhado." << endl;
    }

    cout << "\nComo deseja resolver o cubo?" << endl;
    cout << "1- Jogador\n2- IA (BFS)\n3- IA (DFS)\n4- IA (A*)" << endl;
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
            cout << "Funcao DFS ainda nao implementada." << endl;
            break;
        case 4:
            cout << "Funcao A* ainda nao implementada." << endl;
            break;
        default:
            cout << "Opcao invalida." << endl;
            break;
    }

    return 0;
}