#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>

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
    vector<int> ind;
    void load(vector<int>& w1, vector<int>& v1, int c1) {
        w = w1;
        v = v1;
        c = c1;
        n = w.size();
        
        ind.resize(n);
        for (int i = 0; i < n; i++) ind[i] = i;
        
        sort(ind.begin(), ind.end(), [this](int a, int b) {
            return (double)v[a] / w[a] > (double)v[b] / w[b];
        });
    }
    
    double ub_count(int i_ind, int wi, int vi) {
        if (wi > c) return 0;
        
        double curr = vi;
        int ub = c - wi;
        int fl = 0;
        
        for (int i = i_ind; i < n && fl == 0; i++) {
            int idx = ind[i];
            if (w[idx] <= ub) {
                curr += v[idx];
                ub -= w[idx];
            } 
            else {
                curr += (double)v[idx] * ub / w[idx];
                fl = 1;
            }
        }
        
        return curr;
    }
    
    vector<bool> get_init_sol() {
        vector<bool> sol(n, false);
        int weight = 0;
       
        for (int i = 0; i < n; i++) {
            int idx = ind[i];  
            if (weight + w[idx] <= c) {
                sol[idx] = true;
                weight += w[idx];
            }
        }
        
        return sol;
    }
};

class Solver {
private:
    Knapsack& knapsack;
    vector<bool> res_sol;
    int res_val;
    
    struct Node {
        int i_ind;
        int wi;
        int vi;
        vector<bool> d;
        double sup;
    };
    
    void branch_and_bound() {
        Node root;
        root.i_ind = 0;
        root.wi = 0;
        root.vi = 0;
        root.d = vector<bool>(knapsack.n, false);
        root.sup = knapsack.ub_count(0, 0, 0);
        
        struct Compare {
            bool operator()(const Node& a, const Node& b) {
                return a.sup < b.sup;
            }
        };
        
        priority_queue<Node, vector<Node>, Compare> pq;
        pq.push(root);
        
        while (!pq.empty()) {
            Node current = pq.top();
            pq.pop();
            
            if (!(current.sup <= res_val)) {
                if (current.i_ind == knapsack.n) {
                    if (current.vi > res_val) {
                        res_val = current.vi;
                        res_sol = current.d;
                    }
                } 
                else {
                    int idx = knapsack.ind[current.i_ind];
                    
                    if (current.wi + knapsack.w[idx] <= knapsack.c) {
                        Node items_ks = current;
                        items_ks.d[idx] = true;
                        items_ks.wi += knapsack.w[idx];
                        items_ks.vi += knapsack.v[idx];
                        items_ks.i_ind++;
                        items_ks.sup = knapsack.ub_count(items_ks.i_ind, items_ks.wi, items_ks.vi);
                        
                        if (items_ks.sup > res_val) {
                            pq.push(items_ks);
                        }
                    }
                    
                    Node notneed = current;
                    notneed.i_ind++;
                    notneed.sup = knapsack.ub_count(notneed.i_ind, notneed.wi, notneed.vi);
                    
                    if (notneed.sup > res_val) {
                        pq.push(notneed);
                    }
                }
            }
        }
    }
    
public:
    Solver(Knapsack& ks) : knapsack(ks) {
        res_val = 0;
        res_sol = vector<bool>(knapsack.n, false);
    }
    
    vector<bool> solve(int max_i = 1000) {
        res_sol = knapsack.get_init_sol();
        res_val = knapsack.count(res_sol);
        
        branch_and_bound();
        
        return res_sol;
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