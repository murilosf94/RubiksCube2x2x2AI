#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <cstdlib>
#include <unordered_set> // Adicionado para o DFS

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

using namespace std;
#define ll long long

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

using Face = vector<vector<int>>;

class Cubo2x2 {
private:
    Face U, D, F, B, L, R;

    void GirarFaceHor(Face& face) {
        int temp = face[0][0]; face[0][0] = face[1][0]; face[1][0] = face[1][1];
        face[1][1] = face[0][1]; face[0][1] = temp;
    }

    string cor(int color) const {
        string block = "██"; 
        switch (color) {
            case 0: return "\033[1;37m" + block + "\033[0m"; case 1: return "\033[1;33m" + block + "\033[0m";
            case 2: return "\033[1;34m" + block + "\033[0m"; case 3: return "\033[1;38;5;208m" + block + "\033[0m";
            case 4: return "\033[1;32m" + block + "\033[0m"; case 5: return "\033[1;31m" + block + "\033[0m";
            default: return "??";
        }
    }

    // --- SEÇÃO DE RESOLUÇÃO DFS (privada) ---
    bool dfs(int profundidade_max, vector<int>& caminho) {
        if (this->estaResolvido()) { return true; }
        if (caminho.size() >= profundidade_max) { return false; }
        
        for (int i = 1; i <= 12; ++i) {
            fazerMovimento(i);
            caminho.push_back(i);
            if (dfs(profundidade_max, caminho)) { return true; }
            caminho.pop_back();
            fazerMovimentoInverso(i);
        }
        return false;
    }

    void fazerMovimento(int moveId) {
        switch(moveId) {
            case 1: moveU(); break;  case 2: moveD(); break;  case 3: moveF(); break;
            case 4: moveB(); break;  case 5: moveL(); break;  case 6: moveR(); break;
            case 7: moveU_antihor(); break; case 8: moveD_antihor(); break;
            case 9: moveF_antihor(); break; case 10: moveB_antihor(); break;
            case 11: moveL_antihor(); break; case 12: moveR_antihor(); break;
        }
    }
    
    void fazerMovimentoInverso(int moveId) {
        switch(moveId) {
            case 1: moveU_antihor(); break; case 2: moveD_antihor(); break;
            case 3: moveF_antihor(); break; case 4: moveB_antihor(); break;
            case 5: moveL_antihor(); break; case 6: moveR_antihor(); break;
            case 7: moveU(); break; case 8: moveD(); break; case 9: moveF(); break;
            case 10: moveB(); break; case 11: moveL(); break; case 12: moveR(); break;
        }
    }
    
    string getNomeMovimento(int moveId) {
        const char* nomes[] = {"", "U", "D", "F", "B", "L", "R", "U'", "D'", "F'", "B'", "L'", "R'"};
        return nomes[moveId];
    }

public:
    Cubo2x2() {
        U = {{0, 0}, {0, 0}}; D = {{1, 1}, {1, 1}}; F = {{2, 2}, {2, 2}};
        B = {{4, 4}, {4, 4}}; L = {{5, 5}, {5, 5}}; R = {{3, 3}, {3, 3}};
    }

    void printCube() const {
        cout << "      ╔════╗" << endl; cout << "      ║" << cor(U[0][0]) << cor(U[0][1]) << "║  (U - Topo)" << endl;
        cout << "      ║" << cor(U[1][0]) << cor(U[1][1]) << "║" << endl; cout << "╔════╬════╬════╦════╗" << endl;
        cout << "║" << cor(L[0][0]) << cor(L[0][1]) << "║" << cor(F[0][0]) << cor(F[0][1]) << "║" << cor(R[0][0]) << cor(R[0][1]) << "║" << cor(B[0][0]) << cor(B[0][1]) << "║" << endl;
        cout << "║" << cor(L[1][0]) << cor(L[1][1]) << "║" << cor(F[1][0]) << cor(F[1][1]) << "║" << cor(R[1][0]) << cor(R[1][1]) << "║" << cor(B[1][0]) << cor(B[1][1]) << "║" << endl;
        cout << "╠════╩════╩════╩════╣" << endl; cout << "║(L) (F)  (R)  (B) ║" << endl;
        cout << "╚════╦════╦════╦════╝" << endl; cout << "      ║" << cor(D[0][0]) << cor(D[0][1]) << "║" << endl;
        cout << "      ║" << cor(D[1][0]) << cor(D[1][1]) << "║  (D - Base)" << endl; cout << "      ╚════╝" << endl;
    }
    
    void moveU() { GirarFaceHor(U); vector<int> temp = F[0]; F[0] = R[0]; R[0] = B[0]; B[0] = L[0]; L[0] = temp; }
    void moveU_antihor() { moveU(); moveU(); moveU(); }
    void moveD() { GirarFaceHor(D); vector<int> temp = F[1]; F[1] = L[1]; L[1] = B[1]; B[1] = R[1]; R[1] = temp; }
    void moveD_antihor() { moveD(); moveD(); moveD(); }
    void moveF() { GirarFaceHor(F); vector<int> tempU = {U[1][0], U[1][1]}; U[1][0] = L[0][1]; U[1][1] = L[1][1]; L[0][1] = D[0][1]; L[1][1] = D[0][0]; D[0][1] = R[1][0]; D[0][0] = R[0][0]; R[1][0] = tempU[0]; R[0][0] = tempU[1]; }
    void moveF_antihor() { moveF(); moveF(); moveF(); }
    void moveB() { GirarFaceHor(B); vector<int> tempU = {U[0][0], U[0][1]}; U[0][0] = R[0][1]; U[0][1] = R[1][1]; R[0][1] = D[1][1]; R[1][1] = D[1][0]; D[1][1] = L[1][0]; D[1][0] = L[0][0]; L[1][0] = tempU[0]; L[0][0] = tempU[1]; }
    void moveB_antihor() { moveB(); moveB(); moveB(); }
    void moveL() { GirarFaceHor(L); vector<int> tempU = {U[0][0], U[1][0]}; U[0][0] = B[1][0]; U[1][0] = B[0][0]; B[1][0] = D[0][0]; B[0][0] = D[1][0]; D[0][0] = F[0][0]; D[1][0] = F[1][0]; F[0][0] = tempU[0]; F[1][0] = tempU[1]; }
    void moveL_antihor() { moveL(); moveL(); moveL(); }
    void moveR() { GirarFaceHor(R); vector<int> tempU = {U[0][1], U[1][1]}; U[0][1] = F[0][1]; U[1][1] = F[1][1]; F[0][1] = D[0][1]; F[1][1] = D[1][1]; D[0][1] = B[1][1]; D[1][1] = B[0][1]; B[1][1] = tempU[0]; B[0][1] = tempU[1]; }
    void moveR_antihor() { moveR(); moveR(); moveR(); }

    bool estaResolvido() const {
        for (int i = 0; i < 2; ++i) { for (int j = 0; j < 2; ++j) { if (U[i][j] != U[0][0] || F[i][j] != F[0][0] || R[i][j] != R[0][0] || B[i][j] != B[0][0] || D[i][j] != D[0][0] || L[i][j] != L[0][0]) { return false; } } }
        return true;
    }

    void embaralhar() {
        limparTela(); unsigned seed = chrono::high_resolution_clock::now().time_since_epoch().count();
        mt19937 gerador(seed); uniform_int_distribution<int> distribuicao(1, 6);
        ll numMovimentos = 0; cout << "╔════════════════════════════════╗" << endl;
        cout << "║    NÍVEL DE EMBARALHAMENTO     ║" << endl; cout << "╠════════════════════════════════╣" << endl;
        cout << "║ 1. Fácil (5 movimentos)        ║" << endl; cout << "║ 2. Médio (15 movimentos)       ║" << endl;
        cout << "║ 3. Difícil (30 movimentos)     ║" << endl; cout << "╚════════════════════════════════╝" << endl;
        cout << "Escolha o nivel de embaralhamento (1-3): "; int nivel; cin >> nivel;
        switch (nivel) { case 1: numMovimentos = 5; break; case 2: numMovimentos = 15; break; case 3: numMovimentos = 30; break; default: cout << "Nivel invalido. Usando nivel facil por padrao." << endl; numMovimentos = 5; break; }
        for (int i = 0; i < numMovimentos; ++i) { int movembaralha = distribuicao(gerador); switch (movembaralha) { case 1: moveU(); break; case 2: moveD(); break; case 3: moveF(); break; case 4: moveB(); break; case 5: moveL(); break; case 6: moveR(); break; } }
        limparTela(); cout << "\nCubo embaralhado:" << endl; printCube();
    }
    
    void resolverComDFS() {
        for (int profundidade_max = 1; profundidade_max <= 14; ++profundidade_max) {
            limparTela(); cout << "Procurando solucao com ate " << profundidade_max << " movimentos..." << endl;
            vector<int> caminho; Cubo2x2 copia = *this;
            if (copia.dfs(profundidade_max, caminho)) {
                cout << "\n\033[1;32mSolucao encontrada!\033[0m" << endl;
                cout << "Numero de movimentos: " << caminho.size() << endl; cout << "Caminho: ";
                for (int moveId : caminho) { cout << getNomeMovimento(moveId) << " "; }
                cout << endl; return;
            }
        }
        cout << "\nNao foi possivel encontrar uma solucao em tempo habil." << endl;
    }
};

void jogador(Cubo2x2& cubo) {
    ll movimento = 0;
    while (movimento != -1) {
        limparTela(); cout << "---------------JOGADOR---------------\n" << endl; cubo.printCube();
        cout << "\n╔═══════════════ MOVIMENTOS ═══════════════╗" << endl;
        cout << "║ Horario:    1-U  2-D  3-F  4-B  5-L  6-R   ║" << endl;
        cout << "║ Anti-hor.:  7-U' 8-D' 9-F' 10-B' 11-L' 12-R' ║" << endl;
        cout << "╠════════════════════════════════════════════╣" << endl;
        cout << "║ Para SAIR, digite -1                       ║" << endl;
        cout << "╚════════════════════════════════════════════╝" << endl;
        cout << "Digite o movimento desejado: "; cin >> movimento;
        switch (movimento) {
            case 1: cubo.moveU(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 2: cubo.moveD(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 3: cubo.moveF(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 4: cubo.moveB(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 5: cubo.moveL(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 6: cubo.moveR(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 7: cubo.moveU_antihor(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 8: cubo.moveD_antihor(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 9: cubo.moveF_antihor(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 10: cubo.moveB_antihor(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 11: cubo.moveL_antihor(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case 12: cubo.moveR_antihor(); limparTela(); cubo.printCube(); if (cubo.estaResolvido()) { cout << "\n\033[1;32mCUBO RESOLVIDO\033[0m" << endl; } else { cout << "\nCUBO NAO RESOLVIDO" << endl; } break;
            case -1: cout << "\nSaindo do programa..." << endl; break;
            default: cout << "\nMovimento invalido!" << endl; break;
        }
        if (movimento != -1 && movimento > 0 && movimento <=12 && !cubo.estaResolvido()) {
             cout << "\nPressione Enter para o proximo movimento..."; cin.ignore(); cin.get();
        }
    }
}

int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul"); 
    #endif
    habilitarCores(); limparTela(); Cubo2x2 cubo;
    cout << "Cubo inicial (resolvido):" << endl; cubo.printCube();
    cout << "\n Deseja embaralhar o cubo?" << endl; cout << "1- Sim" << endl; cout << "2- Nao" << endl;
    int opcao; cin >> opcao;
    if (opcao == 1) { cubo.embaralhar(); } else { cout << "\nCubo nao embaralhado." << endl; }
    cout << "\n Como deseja resolver o cubo??" << endl; cout << "1- Jogador" << endl; cout << "2- IA (BFS)" << endl;
    cout << "3- IA (DFS)" << endl; cout << "4- IA (A*)" << endl; cout << "Escolha uma opcao: ";
    int escolha; cin >> escolha;
    switch (escolha) {
        case 1: jogador(cubo); break;
        case 2: cout << "Funcao BFS ainda nao implementada." << endl; break;
        case 3: cubo.resolverComDFS(); break;
        case 4: cout << "Funcao A* ainda nao implementada." << endl; break;
        default: cout << "Opcao invalida. Encerrando o programa." << endl; break;
    }
    return 0;
}