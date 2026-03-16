#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>


using namespace std;

class Task {
public:
    int n;
    
    double count_dist(vector<int>& route) {
        double length = 0;
        for (int i = 0; i < n - 1; i++) length += dist(route[i], route[i + 1]);
        length += dist(route[n - 1], route[0]);
        return length;
    }
    
    virtual double dist(int i, int j) = 0;
};

class TSP : public Task {
private:
    vector<double> x, y;
    
public:
    double dist(int i, int j) override {
        double dx = x[i] - x[j], dy = y[i] - y[j];
        return sqrt(dx*dx + dy*dy);
    }
    
    vector<int> get_init_sol() {
        vector<int> start;
        vector<bool> visited(n, false);
        
        start.push_back(0);
        visited[0] = true;
        
        for (int i = 1; i < n; i++) {
            int last = start.back();
            int best_city = -1;
            double best_dist = 1e9;
            
            for (int j = 0; j < n; j++) {
                if (!visited[j]) {
                    double d = dist(last, j);
                    if (d < best_dist) {
                        best_dist = d;
                        best_city = j;
                    }
                }
            }
            
            start.push_back(best_city);
            visited[best_city] = true;
        }
        
        return start;
    }
    
    void coordinates(vector<double>& x_i, vector<double>& y_i) {
        x = x_i;
        y = y_i;
        n = x.size();
    }
};

class Solver {
private:
    TSP& tsp;
    mt19937 rng;
    
public:
    Solver(TSP& tsp1) : tsp(tsp1) {
        random_device rd;
        rng.seed(rd());
    }
    
    vector<int> solve(int max_iter = 5000) {
        uniform_real_distribution<double> dist01(0.0, 1.0);
        
        vector<int> current = tsp.get_init_sol();
        double current_path = tsp.count_dist(current);
        vector<int> best = current;
        double best_path = current_path;
        
        double T = 100.0;
        double alpha = 0.995;
        
        for (int iter = 0; iter < max_iter; iter++) {
            vector<int> neighbour = current;
            
            int r = rand() % 3;
            
            if (r == 0) {
                int i = rand() % (tsp.n - 2);
                int j = i + 2 + rand() % (tsp.n - i - 2);
                reverse(neighbour.begin() + i, neighbour.begin() + j + 1);
            } 
            else if (r == 1) {
                int i = rand() % (tsp.n - 3);
                int j = i + 1 + rand() % (tsp.n - i - 2);
                int k = j + 1 + rand() % (tsp.n - j - 1);
                reverse(neighbour.begin() + j, neighbour.begin() + k + 1);
            } 
            else {
                int i = rand() % (tsp.n - 3);
                int j = i + 1 + rand() % (tsp.n - i - 2);
                int k = j + 1 + rand() % (tsp.n - j - 1);
                reverse(neighbour.begin() + i, neighbour.begin() + j + 1);
                reverse(neighbour.begin() + j, neighbour.begin() + k + 1);
            }
            
            double neighbour_path = tsp.count_dist(neighbour);
            
            if (neighbour_path < current_path || exp((current_path - neighbour_path) / T) > dist01(rng)) {
                current = neighbour;
                current_path = neighbour_path;
                
                if (current_path < best_path) {
                    best = current;
                    best_path = current_path;
                }
            }
            
            T *= alpha;
        }
        
        cout << best_path << endl;
        return best;
    }
};

int main() {
    int n;
    cin >> n;
    
    vector<double> x(n), y(n);
    for (int i = 0; i < n; i++) cin >> x[i] >> y[i];
    
    TSP tsp;
    tsp.coordinates(x, y);
    
    Solver solver(tsp);
    vector<int> res = solver.solve();
    
    for (int i = 0; i < res.size(); i++) {
    cout << res[i] << " ";
}
    cout << res[0] << endl;
    
    return 0;
}
