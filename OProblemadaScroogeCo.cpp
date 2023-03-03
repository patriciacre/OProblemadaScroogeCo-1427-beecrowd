#include <iostream>
#include <vector>
#include <climits> // para utilizar INT_MAX e representar a distância infinita
#include <map> // para mapear o nome das localizações com seus índices de vértices correspondentes

using namespace std;

class grafo{
	public:
        grafo(vector<vector<int>> &d, int v, vector<vector<int>> &pred);
        virtual ~grafo() {};
        int getD(int i, int j) const {return this->D[i][j];};
        int getPred(int i, int j) const {return this->matrizPi[i][j];};
        void setD(vector<vector<int>> &d) {this->D = d;}
        void setPred(vector<vector<int>> &pi) {this->matrizPi = pi;}
        int getNumV() const {return numV;}
        void setNumV(int num) {this->numV = num;}
        void floyd_Warshall();
	private:
        vector<vector<int>> D; // matriz de distâncias
        vector<vector<int>> matrizPi;  // é a matriz de predecessores
        int numV; // número de vértices
};

grafo::grafo(vector<vector<int>> &d, int v, vector<vector<int>> &pred){
	setNumV(v);
	setD(d);
	setPred(pred);
}

void grafo::floyd_Warshall(){
    for (int k = 0; k<numV; k++){ // executa as comparações k vezes
        for (int l = 0; l<numV; l++ ){ // percorre a linha
            for (int c = 0; c<numV; c++){ // percorre a coluna
            // verifico se os valores para somar nao sao infinitos, pq caso sejam nao sera uma opção de caminho melhor, então nem preciso testar
                if ((D[k][c] != INT_MAX) && (D[l][k] != INT_MAX)){
                    if (D[l][c]>D[k][c]+D[l][k]){ // verifico se será melhor chegar em c passando por k
                        D[l][c] = D[k][c]+D[l][k]; // atualizo o valor da distância
                        matrizPi[l][c] = getPred(l, k); // atualizo o predecessor de c
                    }
                }
            }
        }
    }
}

int main() {
    int C, P, custo, R, origem, destino, pesoFinal;
    string O, D, local, func;
    map<string, int> nomeDoVert; // para mapear a partir do nome do local o indice correspondente
    map<int, string> indDoVert; // para mapear a partir do indice o nome do local correspondente
    cin >> C; // lê quantos serão os casos de teste
	for (int i = 0; i<C; i++){ // lendo os testes
        cin >> P; // quantidade de localizaçoes
    	vector<vector<int>> d(P, vector<int> (P, 0)); // matriz de distâncias
    	vector<vector<int>> pred(P, vector<int> (P, 0)); // matriz de predecessores
        for (int j = 0; j<P; j++){ // leio o nome dos locais (vertices)
            cin >> local;
            nomeDoVert[local] = j; // armazeno o local com seu índice
            indDoVert[j] = local; // armazeno o índice com o nome do local
        }
        for (int l = 0; l<P;l++){
            for (int c = 0; c<P; c++){
                cin >> custo; // lendo o custo de um local para outro
                if (custo == -1){ // caso em que não pode-se realizar a viagem por essa rota
                    d[l][c] = INT_MAX; // inicialmente a matriz de distâncias é igual a matriz de pesos
                    pred[l][c] = -1; // significa que não é um predecessor válido
                } else {
                    d[l][c] = custo;
                    pred[l][c] = c; // inicialmente o predecessor de c é ele próprio, uso essa abordagem pois irei usar matrizes bidimensionais no Floyd_Warshall
                }
            }
        }
        grafo g(d, P, pred); // matriz de distâncias e predecessores alem do numero de localizações
    	g.floyd_Warshall(); // realizo a busca para encontrar o menor caminho
        cin >> R;
        for (int i = 0; i < R; i++){ // lendo as entregas
        	cin >> func >> O >> D; // nome do funcionario, origem e destino
        	origem = nomeDoVert[O]; destino = nomeDoVert[D]; //pego os indices correspondetes a origem e o destino
        	// mostrando o resultado
        	pesoFinal = g.getD(origem, destino); // o custo total do caminho
            if (pesoFinal != INT_MAX) { // verifico se e possivel realizar a entrega
                cout << "Mr " << func << " to go from " << O << " to " << D << ", you will receive " << pesoFinal << " euros" << endl;
                cout << "Path:" << O;
                if (O == D){
                    cout << " " << D;
                } else {
                     while (origem != destino){ // apresentando o nome dos locais que estao no caminho
                        origem = g.getPred(origem, destino);
                        cout << " " << indDoVert[origem]; // indicando o indice do local para map para mostrar o nome
                    }
                }
                cout << endl;
            } else { // caso em que não e possivel realizar a entrega
                cout << "Sorry Mr " << func << " you can not go from " << O << " to " << D << endl;
            }
        }
        nomeDoVert.clear(); // limpo os maps para realização do proximo teste
        indDoVert.clear();
    }
	return 0;
}
