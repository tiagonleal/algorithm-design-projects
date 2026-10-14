#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <sstream>

using namespace std;

const array<array<int, 5>, 5> table = {{
    {1, 1, 1, 1, 1},
    {1, 1, 3, 1, 3},
    {1, 5, 1, 0, 1},
    {1, 0, 1, 0, 4},
    {1, 1, 3, 2, 3}
}};


void order_builder(int i, int j, vector<vector<int>>& last_to_remove, vector<unsigned long long>& solution){
    if (i == j - 1) return;

    int k = last_to_remove[i][j];
    if (k == 0) return;
    order_builder(i, k, last_to_remove, solution);
    order_builder(k, j, last_to_remove, solution);
    solution.push_back(k);
}


vector<unsigned long long> energy(int n, vector<unsigned long long> energy_aminoacidos, vector<int> aminoacidos){

    vector<unsigned long long> solution;

    vector<vector<unsigned long long>> energies(n + 2, vector<unsigned long long>(n + 2, 0));

    vector<vector<int>> last_to_remove(n + 2, vector<int>(n + 2, 0));

    for (int incremento = 2; incremento <= n + 1; incremento++) {

        for (int i = 0; i + incremento<= n + 1; i++) {

            int j = i + incremento;

            for (int k = i+1; k<j; k++) {

                unsigned long long energia_k = energy_aminoacidos[i] * table[aminoacidos[i]][aminoacidos[k]] * energy_aminoacidos[k] + 
                                      energy_aminoacidos[k] * table[aminoacidos[k]][aminoacidos[j]] * energy_aminoacidos[j];

                unsigned long long energia_total = energia_k + energies[i][k] + energies[k][j];

                if (energia_total > energies[i][j]) {
                
                    energies[i][j] = energia_total;
                    last_to_remove[i][j] = k;
                }

                else if (energia_total == energies[i][j] && k > last_to_remove[i][j]) {
                    last_to_remove[i][j] = k;
                }
            }
        }

    }

    solution.push_back(energies[0][n + 1]);
    order_builder(0, n+1, last_to_remove, solution);

    return solution;
}


int letter_to_number(char ch){
    if (ch =='P') return 1;
    if (ch =='N') return 2;
    if (ch =='A') return 3;
    if (ch =='B') return 4;
    return 0;
}


int main(){
    int n;
    cin >> n;
    cin.ignore();

    string en_aminoacidos;
    getline(cin, en_aminoacidos);
    istringstream iss(en_aminoacidos);
    vector<unsigned long long> energy_aminoacidos;
    energy_aminoacidos.push_back(1);
    unsigned long long num;
    for (int i = 0; i < n; i++) {
        iss >> num;
        energy_aminoacidos.push_back(num);
    }

    energy_aminoacidos.push_back(1);

    string str_aminoacidos;
    getline(cin, str_aminoacidos);
    vector<int> aminoacidos;
    aminoacidos.push_back(0);
    for (int i = 0; i < n; i++) {
        aminoacidos.push_back(letter_to_number(str_aminoacidos[i]));
    }
    aminoacidos.push_back(0);

    vector<unsigned long long> solution = energy(n, energy_aminoacidos, aminoacidos);

    cout << solution[0] << endl;
    for (int i = 1; i < n + 1; i++){
        cout << solution[i];
        if (i < n) cout << ' ';
        else cout << endl;
    }

    return 0;
}
