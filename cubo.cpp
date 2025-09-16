#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
// oi
using namespace std;
#define ll long long
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr);

using Face = vector<vector<int>>;

class Cubo2x2 {
private:
    //faces do cubo
    Face U, D, F, B, L, R;

    //função auxiliar para girar uma face 2x2 90 graus no sentido horário
    void GirarFaceHor(Face& face) {
        int temp = face[0][0];
        face[0][0] = face[1][0];
        face[1][0] = face[1][1];
        face[1][1] = face[0][1];
        face[0][1] = temp;
    }

    char cor(int color) const {
        switch (color) {
            case 0: return 'W'; //branco
            case 1: return 'Y'; //amarelo
            case 2: return 'B'; //azul
            case 3: return 'O'; //laranja
            case 4: return 'G'; //verde
            case 5: return 'R'; //vermelho
            default: return '?'; //cor desconhecida
        }
    }

public:
    //construtor: inicializa o cubo resolvido
    Cubo2x2() {
        //inicializa as faces com cores correspondentes
        //0: Branco, 1: Amarelo, 2: Azul, 3: Laranja, 4: Verde, 5: Vermelho

        //face U (branca)
        U = {{0, 0}, {0, 0}};
        //face D (amarela)
        D = {{1, 1}, {1, 1}};
        //face F (azul)
        F = {{2, 2}, {2, 2}};
        //face B (verde)
        B = {{4, 4}, {4, 4}};
        //face L (vermelha)
        L = {{5, 5}, {5, 5}};
        //face R (laranja)
        R = {{3, 3}, {3, 3}};
    }

    //função para imprimir o estado atual do cubo
    void printCube() const {
        //exibição da face U
        cout << "    " << cor(U[0][0]) << " " << cor(U[0][1]) << endl;
        cout << "    " << cor(U[1][0]) << " " << cor(U[1][1]) << endl;

        //exibição das faces L, F, R, B na mesma linha
        for (int i = 0; i < 2; ++i) {
            cout << cor(L[i][0]) << " " << cor(L[i][1]) << " "
                      << cor(F[i][0]) << " " << cor(F[i][1]) << " "
                      << cor(R[i][0]) << " " << cor(R[i][1]) << " "
                      << cor(B[i][0]) << " " << cor(B[i][1]) << endl;
        }

        //exibição da face D
        cout << "    " << cor(D[0][0]) << " " << cor(D[0][1]) << endl;
        cout << "    " << cor(D[1][0]) << " " << cor(D[1][1]) << endl;
    }
    
    //movimento U
    void moveU() {
        GirarFaceHor(U);
        vector<int> temp = F[0];
        F[0] = R[0];
        R[0] = B[0];
        B[0] = L[0];
        L[0] = temp;
    }
    
    //movimento U'
    void moveU_antihor() {
        moveU(); moveU(); moveU();
    }

    //movimento D
    void moveD() {
        GirarFaceHor(D);
        vector<int> temp = F[1];
        F[1] = L[1];
        L[1] = B[1];
        B[1] = R[1];
        R[1] = temp;
    }

    //movimento D'
    void moveD_antihor() {
        moveD(); moveD(); moveD();
    }

    //movimento F
    void moveF() {
        GirarFaceHor(F);
        vector<int> tempU = {U[1][0], U[1][1]};
        U[1][0] = L[0][1];
        U[1][1] = L[1][1];
        L[0][1] = D[0][1];
        L[1][1] = D[0][0];
        D[0][1] = R[1][0];
        D[0][0] = R[0][0];
        R[1][0] = tempU[0];
        R[0][0] = tempU[1];
    }

    //movimento F'
    void moveF_antihor() {
        moveF(); moveF(); moveF();
    }

    //movimento B
    void moveB() {
        GirarFaceHor(B);
        vector<int> tempU = {U[0][0], U[0][1]};
        U[0][0] = R[0][1];
        U[0][1] = R[1][1];
        R[0][1] = D[1][1];
        R[1][1] = D[1][0];
        D[1][1] = L[1][0];
        D[1][0] = L[0][0];
        L[1][0] = tempU[0];
        L[0][0] = tempU[1];
    }

    //movimento B'
    void moveB_antihor() {
        moveB(); moveB(); moveB();
    }

    //movimento L
    void moveL() {
        GirarFaceHor(L);
        vector<int> tempU = {U[0][0], U[1][0]};
        U[0][0] = B[1][0];
        U[1][0] = B[0][0];
        B[1][0] = D[0][0];
        B[0][0] = D[1][0];
        D[0][0] = F[0][0];
        D[1][0] = F[1][0];
        F[0][0] = tempU[0];
        F[1][0] = tempU[1];
    }
    
    //movimento L'
    void moveL_antihor() {
        moveL(); moveL(); moveL();
    }

    //movimento R
    void moveR() {
        GirarFaceHor(R);
        vector<int> tempU = {U[0][1], U[1][1]};
        U[0][1] = F[0][1];
        U[1][1] = F[1][1];
        F[0][1] = D[0][1];
        F[1][1] = D[1][1];
        D[0][1] = B[1][1];
        D[1][1] = B[0][1];
        B[1][1] = tempU[0];
        B[0][1] = tempU[1];
    }

    //movimento R'
    void moveR_antihor() {
        moveR(); moveR(); moveR();
    }


    bool estaResolvido() const {
        //verifica se todas as faces têm a mesma cor
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                if (U[i][j] != U[0][0] || F[i][j] != F[0][0] ||
                    R[i][j] != R[0][0] || B[i][j] != B[0][0] ||
                    D[i][j] != D[0][0] || L[i][j] != L[0][0]) {
                    return false;
                }
            }
        }
        return true;
    }

    void embaralhar() {
        unsigned seed = chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count();

        mt19937 gerador(seed);

        uniform_int_distribution<int> distribuicao(1, 6);

        ll numMovimentos = 0;

        cout << "\n---------------EMBARALHAMENTO---------------\n" << endl;
        cout << "1- Nivel facil: 5 movimentos" << endl;
        cout << "2- Nivel medio: 15 movimentos" << endl;
        cout << "3- Nivel dificil: 30 movimentos" << endl;

        cout << "Escolha o nivel de embaralhamento (1-3): ";
        int nivel;
        cin >> nivel;

        switch (nivel) {
            case 1:
                numMovimentos = 5;
                break;
            case 2:
                numMovimentos = 15;
                break;
            case 3:
                numMovimentos = 30;
                break;
            default:
                cout << "Nivel invalido. Usando nivel facil por padrao." << endl;
                numMovimentos = 5;
                break;
        }
        for (int i = 0; i < numMovimentos; ++i) {
            int movembaralha = distribuicao(gerador);
            switch (movembaralha) {
                case 1: moveU(); break;
                case 2: moveD(); break;
                case 3: moveF(); break;
                case 4: moveB(); break;
                case 5: moveL(); break;
                case 6: moveR(); break;
            }
        }

        cout << "\nCubo embaralhado:" << endl;
        printCube();
    }

    
};

void jogador(Cubo2x2& cubo) {
    ll movimento=0;

    while(movimento != -1) {

        cout << "\n---------------JOGADOR---------------\n" << endl;
        cubo.printCube();

        cout << "\n---------------MOVIMENTOS---------------\n" << endl;
        cout << "1-U, 2-D, 3-F, 4-B, 5-L, 6-R" << endl;
        cout << "7-U', 8-D', 9-F', 10-B', 11-L', 12-R'" << endl;
        cout << "SAIR: -1" << endl;
        cout << "Digite o movimento desejado: ";
        cin >> movimento;

        switch (movimento)
        {
        case 1:
            cubo.moveU();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 2:
            cubo.moveD();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 3:
            cubo.moveF();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 4:
            cubo.moveB();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 5:
            cubo.moveL();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 6:
            cubo.moveR();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 7:
            cubo.moveU_antihor();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 8:
            cubo.moveD_antihor();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 9:
            cubo.moveF_antihor();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 10:
            cubo.moveB_antihor();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 11:
            cubo.moveL_antihor();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case 12:
            cubo.moveR_antihor();
            cubo.printCube();
            if (cubo.estaResolvido()) {
                cout << "\nCUBO RESOLVIDO" << endl;
            }else {
                cout << "\nCUBO NAO RESOLVIDO" << endl;
            }
            break;
        case -1:
            cout << "\nSaindo do programa..." << endl;
            break;
        default:
            cout << "\nMovimento invalido!" << endl;
            break;
        }
    }
}

int main() {
    Cubo2x2 cubo;
    Cubo2x2 cuboInicial; //cubo resolvido para referencia
    cuboInicial = cubo;

    cout << "Cubo inicial (resolvido):" << endl;
    cubo.printCube();

    cout << "\n Deseja embaralhar o cubo?" << endl;
    cout << "1- Sim" << endl;
    cout << "2- Nao" << endl;
    int opcao;
    cin >> opcao;

    if (opcao == 1) {
        cubo.embaralhar();
    } else {
        cout << "\nCubo nao embaralhado." << endl;
    }
    

    cout << "\n Como deseja resolver o cubo??" << endl;
    cout << "1- Jogador" << endl;
    cout << "2- IA (BFS)" << endl;
    cout << "3- IA (DFS)" << endl;
    cout << "4- IA (A*)" << endl;
    cout << "Escolha uma opcao: ";
    int escolha;
    cin >> escolha;
    switch (escolha) {
        case 1:
            jogador(cubo);
            break;
        case 2:
            cout << "Funcao BFS ainda nao implementada." << endl;
            break;
        case 3:
            cout << "Funcao DFS ainda nao implementada." << endl;
            break;
        case 4:
            cout << "Funcao A* ainda nao implementada." << endl;
            break;
        default:
            cout << "Opcao invalida. Encerrando o programa." << endl;
            break;
    }

    return 0;
}