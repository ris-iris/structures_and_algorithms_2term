#include <iostream>
#include <vector>

void dfs(int v, bool** visited, std::vector<int> &outTime, std::vector<std::vector<int> > &graph){
    (*visited)[v] = true;
    for (int i = 0; i < graph[v].size(); ++i) {
        int to = graph[v][i];
        if(!(*visited)[to]){
            dfs(to, visited, outTime, graph);
        }
    }
    outTime.push_back(v);
}

void dfs2(int v, int col, bool** visited, int** color, std::vector<std::vector<int> > &graph){
    (*visited)[v] = true;
    (*color)[v] = col;
    for (int i = 0; i < graph[v].size(); ++i) {
        int to = graph[v][i];
        if(!(*visited)[to]){
            dfs2(to, col, visited, color, graph);
        }
    }
}


int main(){
    int n, m, a, b;
    std::cin >> n >> m;
    std::vector<std::vector<int> > graph(n);
    std::vector<std::vector<int> > invGraph(n);
    bool* visited = new bool[n];
    std::vector<int> outTime;
    for (int i = 0; i < n; ++i) {
        visited[i] = false;
    }
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b;
        --a;
        --b;
        graph[a].push_back(b);
        invGraph[b].push_back(a);
    }

    for (int i = 0; i < n; ++i) {
        if(!visited[i]) {
            dfs(i, &visited, outTime, invGraph);
        }
    }

    int* color = new int[n];
    int numOfComp = 0;
    for (int i = 0; i < n; ++i) {
        visited[i] = false;
    }
    for(int i = n - 1; i > -1; --i){
        if(!visited[outTime[i]]){
            dfs2(outTime[i], numOfComp, &visited, &color, graph);
            ++numOfComp;
        }
    }

    bool* row = new bool[numOfComp];
    bool* col = new bool[numOfComp];
    for (int k = 0; k < numOfComp; ++k) {
        row[k] = false;
        col[k] = false;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < graph[i].size(); ++j) {
            int to = graph[i][j];
            if(color[i] != color[to]){
                row[color[i]] = true;
                col[color[to]] = true;
            }
        }
    }

    if(numOfComp != 1){
        int ansRow = numOfComp;
        int ansCol = numOfComp;

        for (int i = 0; i < numOfComp; ++i) {
            if(row[i])
                --ansRow;
            if(col[i])
                --ansCol;
        }

        std::cout << std::max(ansCol, ansRow);
    } else {
        std::cout << 0;
    }

    delete[] color;
    delete[] visited;
    delete[] row;
    delete[] col;

    return 0;
}
