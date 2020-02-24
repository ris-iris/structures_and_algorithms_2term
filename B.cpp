#include <iostream>
#include <vector>

void dfs(int v, std::vector<std::vector<int> > &graph, std::vector<int> &color, std::vector<int> &sorted, bool &hasCycle){
    color[v] = 1;
    for(int i = 0; i < graph[v].size(); ++i){
        int to = graph[v][i];
        if(color[to] == 0){
            dfs(to, graph, color, sorted, hasCycle);
        } else if(color[to] == 1){
            hasCycle = true;
        }
    }
    color[v] = 2;
    sorted.push_back(v);
}

int main(){
    int n, m, a, b;
    std::cin >> n >> m;
    std::vector<std::vector<int> > graph(n);
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b;
        graph[a].push_back(b);
    }
    std::vector<int> color(n);
    for (int i = 0; i < n; ++i) {
        color[i] = 0;
    }
    std::vector<int> sorted;

    bool hasCycle = false;
    for (int i = 0; i < n; ++i) {
        if(hasCycle)
            break;
        if(color[i] == 0)
            dfs(i, graph, color, sorted, hasCycle);
    }

    if(hasCycle){
        std::cout << "NO";
    }else{
        std::cout << "YES" << std::endl;
        for(int i = n -1; i > -1; --i){
            std::cout << sorted[i] << " ";
        }
    }

    return 0;
}
