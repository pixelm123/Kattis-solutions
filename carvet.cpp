//carvet
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,fma")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;

int h, w;
int dr[] = {1, -1, 0, 0};
int dc[] = {0, 0, 1, -1};
int tr, tc;
int grid[250][250];
set<pair<int, int>> visited;
vector<int> current_path;
vector<vector<int>> solutions;
int min_moves = INT_MAX;

void backtrack(int r, int c) {
    visited.insert({r, c});
    if (r == tr && c == tc) {
        if ((int)current_path.size() < min_moves) {
            min_moves = current_path.size();
            solutions.clear();
            solutions.push_back(current_path);
        } else if ((int)current_path.size() == min_moves) {
            solutions.push_back(current_path);
        }
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (r + 2 * dr[i] < 0 || r + 2 * dr[i] >= h || c + 2 * dc[i] < 0 || c + 2 * dc[i] >= w)
            continue;  // out of grid
        if (grid[r + dr[i]][c + dc[i]] != grid[r + 2 * dr[i]][c + 2 * dc[i]])
            continue;  // not 1 car
        if (grid[r + dr[i]][c + dc[i]] == -1) continue;  // blocked
        if (visited.count({r + 2 * dr[i], c + 2 * dc[i]})) continue;  // visited

        swap(grid[r][c], grid[r + 2 * dr[i]][c + 2 * dc[i]]);
        current_path.push_back(grid[r][c]);
        backtrack(r + 2 * dr[i], c + 2 * dc[i]);
        current_path.pop_back();
        swap(grid[r][c], grid[r + 2 * dr[i]][c + 2 * dc[i]]);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> h >> w;
    int er = 0, ec = 0;
    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            cin >> grid[r][c];
            if (grid[r][c] == -1) {
                er = r;
                ec = c;
            }
        }
    }
    cin >> tr >> tc;
    tr--;
    tc--;
    
    backtrack(er, ec);
    sort(solutions.begin(), solutions.end());

    if (solutions.empty()) {
        cout << "impossible" << endl;
    } else {
        for (auto &path : solutions[0]) {
            cout << path << " ";
        }
        cout << endl;
    }

    return 0;
}
