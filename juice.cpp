#include <bits/stdc++.h>
using namespace std;

struct House {
    int demand;
    int capacity;
    vector<int> children;
};

vector<House> houses;
vector<int> dp; // To store the total power demand in each subtree
int N;

int dfs(int node) {
    int total_demand = houses[node].demand;
    int powered_houses = 0;

    for (int child : houses[node].children) {
        int child_powered = dfs(child);
        if (child_powered != -1) {
            total_demand += dp[child];
            powered_houses += child_powered;
        }
    }

    if (node != 0 && total_demand > houses[node].capacity) {
        dp[node] = 0;
        return 0;
    } else {
        dp[node] = total_demand;
        return powered_houses + 1;
    }
}

int main() {
    cin >> N;
    houses.resize(N + 1);
    dp.resize(N + 1, 0);

    for (int i = 1; i <= N; ++i) {
        int p, r, c;
        cin >> p >> r >> c;
        houses[i].demand = r;
        houses[i].capacity = c;
        houses[p].children.push_back(i);
    }

    int max_powered_houses = 0;
    for (int child : houses[0].children) {
        max_powered_houses += dfs(child);
    }

    cout << max_powered_houses << endl;

    return 0;
}
