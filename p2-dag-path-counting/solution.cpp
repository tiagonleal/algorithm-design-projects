#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

void countingPaths(unsigned int n, unsigned int a, long long m,
    vector<vector<unsigned int>> &anteriores,
    vector<vector<unsigned int>> &seguintes,
    vector<long long> &num_paths, const vector<unsigned int> &ordem_topologica,
    vector<unsigned int> &visited, unsigned int cur_index, vector<unsigned int> &pilha, vector<unsigned int> &alcancaveis_lista){
        
    // Limpa as estruturas auxiliares para o próximo nó
    pilha.clear();
    alcancaveis_lista.clear();
    
    pilha.push_back(a);
    visited[a] = cur_index;

    
    while (!pilha.empty()){
        unsigned int u = pilha.back();
        pilha.pop_back();
        
        for (unsigned int v : seguintes[u]){
            if (visited[v] != cur_index){
                visited[v] = cur_index;
                pilha.push_back(v);
            }
        }
    }

    //Filtra os nós alcançáveis mantendo a ordem
    for (unsigned int u : ordem_topologica){
        if (visited[u] == cur_index) {
            num_paths[u] = 0;
            alcancaveis_lista.push_back(u);
        }
    }
    
    num_paths[a] = 1;
    

    //DP
    for (unsigned int u : alcancaveis_lista){
        if (num_paths[u] == 0) continue;
        
        for (unsigned int v : seguintes[u]){
            //Ignora nós fora do subgrafo alcançável
            if (visited[v] != cur_index) continue;

            //Acumula o número de caminhos módulo m
            num_paths[v] += num_paths[u];
            if (num_paths[v] >= m) {
                num_paths[v] -= m;
            }
        }      
    }
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long m;
    unsigned int n, k;
    unsigned int first_truck, last_truck;
    cin >> n >> m >> first_truck >> last_truck >> k;
    cin.ignore();
    
    vector<vector<unsigned int>> nos_alcancaveis_por_fonte(n + 1);
    vector<vector<unsigned int>> anteriores(n + 1);
    vector<vector<unsigned int>> seguintes(n + 1);
    vector<unsigned int> visited(n + 1, 0);
    unsigned int cur_index = 0;
    vector<unsigned int> pilha;
    vector<unsigned int> alcancaveis_lista;


    pilha.reserve(n);
    alcancaveis_lista.reserve(n);

    //Construção do grafo
    for (unsigned int i = 0; i < k; i++){
        unsigned int a, b;
        cin >> a >> b;
        anteriores[b].push_back(a);
        seguintes[a].push_back(b);

    }

    unsigned int evaluated_trucks_nbr = last_truck - first_truck + 1;

    vector<vector<pair<unsigned int, unsigned int>>> solutions(evaluated_trucks_nbr);
    vector<long long> num_paths(n + 1, 0);   
    queue<unsigned int> fila_topo;
    vector<unsigned int> graus_in(n + 1, 0);
    
    //Cálculo do grau de entrada de cada nó
    for (unsigned int i = 1; i <= n; i++){
        graus_in[i] = anteriores[i].size();
        if (graus_in[i] == 0)
            fila_topo.push(i);
    }
    

    vector<unsigned int> ordem_topologica;
    ordem_topologica.reserve(n);
    

    while (!fila_topo.empty()){
        unsigned int u = fila_topo.front();
        fila_topo.pop();
        ordem_topologica.push_back(u);
        
        for (unsigned int v : seguintes[u]){
            graus_in[v]--;
            if (graus_in[v] == 0)
                fila_topo.push(v);
        }
    }


    //Calcula os caminhos simulando cada nó como origem
    for (unsigned int a = 1; a <= n; a++){
        cur_index++;
        countingPaths(n, a, m, anteriores, seguintes,
            num_paths, ordem_topologica, visited,
            cur_index, pilha, alcancaveis_lista);

        for (unsigned int b : alcancaveis_lista){
            if (a == b) continue;

                //Aplica a fórmula dada para calcular o camião
                long long truck_nbr = 1 + num_paths[b];

                if (truck_nbr >= first_truck && truck_nbr <= last_truck) 
                    solutions[truck_nbr - first_truck].push_back(make_pair(a, b));
            
        }
    }

    for (unsigned int i = 0; i < evaluated_trucks_nbr; i++){
        sort(solutions[i].begin(), solutions[i].end());
        cout << 'C' << first_truck + i;

        for (long unsigned j = 0; j < solutions[i].size(); j++)
            cout << ' ' << solutions[i][j].first << "," << solutions[i][j].second;

        cout << '\n';
    }
    return 0;
}