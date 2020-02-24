#include <iostream>
#include <vector>
#include <queue>

void bfs(int v, int** timeFromV, std::vector<std::vector<int> > graph){
    std::queue<int> q;
    int temp;
    int to;
    q.push(v);
    (*timeFromV)[v] = 0;
    while(!q.empty()){
        temp = q.front();
        q.pop();
        for(int i = 0; i < graph[temp].size(); ++i){
            to = graph[temp][i];
            if((*timeFromV)[to] == -1){
                (*timeFromV)[to] = (*timeFromV)[temp] + 1;
                q.push(to);
            }
        }
    }
}


int main() {
    int n, m, leon, matilda, moloko, a, b;

    std::cin >>  n >> m >> leon >> matilda >> moloko;
    leon--;
    matilda--;
    moloko--;
    std::vector< std::vector<int> > graph(n);
    int* timeFromLeon = new int[n];
    int* timeFromMatilda = new int[n];
    int* timeFromMilk = new int[n];

    for(int i = 0; i < n; ++i){
        timeFromLeon[i] = -1;
        timeFromMatilda[i] = -1;
        timeFromMilk[i] = -1;
    }
    for(int i = 0; i < m; ++i){
        std::cin >> a >> b;
        a--;
        b--;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    bfs(leon, &timeFromLeon, graph);
    bfs(matilda, &timeFromMatilda, graph);
    bfs(moloko, &timeFromMilk, graph);

    int lenght = 1000000;

    for(int i = 0; i < n; ++i){
        lenght = std::min(timeFromLeon[i] + timeFromMatilda[i] + timeFromMilk[i], lenght);
    }

    std::cout << lenght;

    delete[] timeFromLeon;
    delete[] timeFromMatilda;
    delete[] timeFromMilk;

    return 0;

}
