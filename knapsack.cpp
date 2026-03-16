#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

class Task {
public:
    vector<int> w;
    vector<int> v;
    int c;
    int n;
    
    int count(vector<bool>& sol) {
        int general_w = 0, general_val = 0;
        for (int i = 0; i < n; i++) {
            if (sol[i]) {
                general_w += w[i];
                general_val += v[i];
            }
        }
        if (general_w > c) return 0;
        return general_val;
    }
};

class Knapsack : public Task {
public:
    void load(vector<int>& w1, vector<int>& v1, int c1) {
        w = w1;
        v = v1;
        c = c1;
        n = w.size();
    }
    
    vector<bool> get_init_sol() {
        vector<bool> sol(n, false);
        int weight = 0;
        
        for (int i = 0; i < n; i++) {
            if (weight + w[i] <= c) {
                sol[i] = true;
                weight += w[i];
            }
        }
        
        return sol;
    }
};

class Solver {
private:
    Knapsack& knapsack;
    
    vector<vector<bool>> get_neighbours(vector<bool>& sol) {
        vector<vector<bool>> neighbours;
        
        for (int i = 0; i < knapsack.n; i++) {
            vector<bool> neighbour = sol;
            neighbour[i] = !neighbour[i];
            neighbours.push_back(neighbour);
        }
        
        return neighbours;
    }
    
public:
    Solver(Knapsack& ks) : knapsack(ks) {
        srand(time(0));
    }
    
    vector<bool> solve(int max_i = 1000) {
        vector<bool> curr = knapsack.get_init_sol();
        int curr_value = knapsack.count(curr);
        
        vector<bool> best = curr;
        int best_value = curr_value;
        
        double T = 100.0;
        double alpha = 0.99;
        
        for (int i = 0; i< max_i; i++) {
            vector<vector<bool>> neighbours = get_neighbours(curr);
            
            bool found = false;
            vector<bool> best_neighbour;
            int best_neighbour_value = curr_value;
            
            for (int j = 0; j < neighbours.size(); j++) {
                vector<bool> neighbour = neighbours[j];
                int value = knapsack.count(neighbour);
                if (value > best_neighbour_value) {
                    best_neighbour = neighbour;
                    best_neighbour_value = value;
                    found = true;
                }
            }
            
            if (found) {
                curr = best_neighbour;
                curr_value = best_neighbour_value;
                
                if (curr_value > best_value) {
                    best = curr;
                    best_value = curr_value;
                }
            }
            else {
                int ind = rand() % neighbours.size();
                int rval = knapsack.count(neighbours[ind]);
                                
                if (exp((rval - curr_value) / T) > (double)rand() / RAND_MAX) {
                    curr = neighbours[ind];
                    curr_value = rval;
                }
            }     
            
            T *= alpha;
        }
        
        return best;
    }
};

int main() {
    int n, c;
    cin >> n >> c;
    
    vector<int> v(n), w(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i] >> w[i];
    }
    
    Knapsack kns;
    kns.load(w, v, c);
    
    Solver solver(kns);
    vector<bool> result = solver.solve();
    
    int gen_weight = 0;
    for (int i = 0; i < n; i++) {
        if (result[i]) {
            gen_weight += w[i];
        }
    }
    
    cout << gen_weight << endl;
    
    for (int i = 0; i < n; i++) {
        cout << result[i] << " ";
    }
    cout << endl;
    
    return 0;
}