#include <iostream>
#include <vector>
#include <set>


class Chain{
private:
    std::vector<int> chain;

public:
    Chain(){}

    const int operator [](int i){
        return chain[i];
    }

    Chain(Chain& c){
        for(int i = 0; i < c.getSize(); ++i){
            chain.push_back(c[i]);
        }
    }

    void addNode(int n){
        chain.push_back(n);
    }

    int getSize(){
        return chain.size();
    }

    void print(){
        for (int i = 0; i < getSize(); ++i) {
            std::cout << chain[i] << " ";
        }
        std::cout << std::endl;
    }


};


class Face{
    std::vector<int> face;
public:

    Face(){}

    Face(Chain c){
        for(int i = 0; i < c.getSize(); ++i){
            face.push_back(c[i]);
        }
    }

    const int operator [](int i){
        return face[i];
    }

    int getSize(){
        return face.size();
    }

    int next(int n){
        return face[(n + 1) % getSize()];
    }

    int prev(int n){
        return face[(n + getSize() - 1) % getSize()];
    }

    std::pair<Face, Face> split(Chain c){
        Chain first(c);
        Chain second(c);
        int start, end;
        for(int i = 0; i < getSize(); ++i){
            if(face[i] == c[0])
                start = i;
            if(face[i] == c[c.getSize() - 1])
                end = i;
        }
        for(int i = (end + 1) % getSize(); i != start; i = (i + 1) % getSize()){
            std::set<int> help = Face(first).toSet();
            //fixing
            if(help.find(face[i]) == help.end())
                first.addNode(face[i]);
        }
        for(int i = (end - 1) % getSize(); i != start; i = (i + getSize() - 1) % getSize()){
            std::set<int> help = Face(first).toSet();
            if(help.find(face[i]) == help.end())
                second.addNode(face[i]);
        }

        return std::make_pair(Face(first), Face(second));
    }

    void print(){
        for (int i = 0; i < getSize(); ++i) {
            std::cout << face[i] << " ";
        }
        std::cout << std::endl;
    }

    std::set<int> toSet(){
        if(getSize() > 0){
            std::set<int> s(face.begin(), face.end());
            return s;
        }
    }

};

const bool operator==(Face a, Face b) {
    if(a.getSize() != b.getSize()) return false;
    for (int i = 0; i < a.getSize(); ++i) {
        if(a[i] != b[i]) return false;
    }
    return true;
}

class Graph{
private:
    int size = 0;
    std::vector<int> color;
    std::vector<bool> visited;
    std::vector<int> parent;
    int cycleEnd;

    void dfs(int v, std::set<int> outsideNodes, Chain& chain){
        visited[v] = 1;
        chain.addNode(v);
        for(auto i = matrix[v].begin(); i != matrix[v].end(); ++i){
            if(!visited[*i]){
                if(outsideNodes.find(*i) == outsideNodes.end()){
                    dfs(*i, outsideNodes, chain);
                }else{
                    chain.addNode(*i);
                }
                return;
            }
        }
    }

    void dfs(int v, Graph& g, std::set<int> outNodes){
        if(outNodes.find(v) == outNodes.end()){
            visited[v] = 1;
            for(auto i = matrix[v].begin(); i != matrix[v].end(); ++i){
                //fix add edge
                g.addEdge(v, *i);
                if(!visited[*i])
                    dfs(*i, g, outNodes);
            }
        }
    }

    void dfs(int v, Graph& g){
            visited[v] = 1;
            for(auto i = matrix[v].begin(); i != matrix[v].end(); ++i){
                //fix add edge
                g.addEdge(v, *i);
                if(!visited[*i])
                    dfs(*i, g);
            }
    }

    void dfs(int v, int& time, std::vector<int>& enter, std::vector<int>& ret){
        enter[v] = time;
        ret[v] = time;
        ++time;
        for(auto i = matrix[v].begin(); i != matrix[v].end(); ++i){
            if(enter[*i] == -1){
                parent[*i] = v;
                dfs(*i, time, enter, ret);
                ret[v] = std::min(ret[v], ret[*i]);
            } else if(parent[v] != *i) {
                ret[v] = std::min(ret[v], enter[*i]);
            }
        }
    }

    std::vector<Graph> split(std::set<int> outNodes){
        for (int i = 0; i < matrix.size(); ++i) {
            visited[i] = 0;
        }
        std::vector<Graph> components;
        for (int i = 0; i < matrix.size(); ++i) {
            if(!visited[i] && outNodes.find(i) == outNodes.end()){
                Graph g(matrix.size());
                dfs(i, g, outNodes);
                components.push_back(g);
            }
        }
        return components;
    }

    std::vector<Graph> split(){
        for (int i = 0; i < getSize(); ++i) {
            visited[i] = 0;
        }
        std::vector<Graph> components;
        for (int i = 0; i < getSize(); ++i) {
            if(!visited[i]){
                Graph g(size);
                dfs(i, g);
                components.push_back(g);
            }
        }
        return components;
    }

    std::vector<Graph> deleteBriges(){
        std::vector<int> enter;
        enter.assign(matrix.size(), -1);
        std::vector<int> ret(matrix.size());
        int t = 0;
        dfs(0, t, enter, ret);
        for(int i = 1; i < matrix.size(); ++i){
            if(enter[parent[i]] < ret[i]){
                deleteEdge(parent[i], i);
            }
        }
        return split();
    }

public:
    std::vector<std::vector<int> > matrix;

    Graph(){}

    Graph(int n){
        matrix.resize(n);
        for (int i = 0; i < n; ++i) {
            color.push_back(0);
            visited.push_back(0);
            parent.push_back(-1);

        }
    }

    void addEdge(int a, int b){
        if(a >= matrix.size()){
            color.resize(a + 1);
            visited.resize(a + 1);
            parent.assign(a + 1, -1);
            matrix.resize(a + 1);
        }
        matrix[a].push_back(b);
    }

    void deleteEdge(int v, int u){
        for(auto j = matrix[v].begin(); j != matrix[v].end(); ++j){
            if(*j == u){
                matrix[v].erase(j);
                break;
            }
        }
        for(auto j = matrix[u].begin(); j != matrix[u].end(); ++j){
            if(*j == v){
                matrix[u].erase(j);
                break;
            }
        }
    }

    int getSize(){
        size = 0;
        for(auto i = matrix.begin(); i != matrix.end(); ++i){
            if((*i).size() > 0)
                ++size;
        }
        return size;
    }

    bool hasCycle(int v){
        color[v] = 1;
        for(auto i = matrix[v].begin(); i != matrix[v].end(); ++i){
            if(color[*i] == 0){
                parent[*i] = v;
                if(hasCycle(*i))
                    return true;
            }
            else if ((color[*i] == 1) && (parent[v] != *i)){
                parent[*i] = v;
                cycleEnd = *i;
                return true;
            }
        }
        color[v] = 2;
        return false;
    }

    Chain getCycle(){
        for (int i = 0; i < size; ++i) {
            visited[i] = 0;
        }
        int v = cycleEnd;
        Chain chain;
        do{
            chain.addNode(v);
            v = parent[v];
        } while (v != cycleEnd);
        return chain;
    }

    Chain getChain(Face outNodes){
        for (int i = 0; i < matrix.size(); ++i) {
            visited[i] = 0;
        }
        Chain chain;
        for(int i = 0; i < outNodes.getSize(); ++i){
            if(outNodes[i] < matrix.size() && matrix[outNodes[i]].size() > 0){
                dfs(outNodes[i], outNodes.toSet(), chain);
                break;
            }
        }
        return chain;
    }

    std::vector<Graph> getSegments(std::vector<Face> faces, std::set<int> outNodes){
        std::vector<Graph> g = split(outNodes);
        std::vector<Graph> res;
        int resSize = 0;
        for(auto graph = g.begin(); graph != g.end(); ++graph){
            res.push_back(*graph);
            for(int v = 0; v < matrix.size(); ++v){
                for(int i = 0; i < (graph->matrix)[v].size(); ++i){
                    if(outNodes.find((graph->matrix)[v][i]) != outNodes.end()){
                        res[resSize].addEdge((graph->matrix)[v][i], v);
                    }
                }
            }
            ++resSize;
        }
        for(auto face = faces.begin(); face != faces.end(); ++face){
            for(int i = 0; i < face->getSize(); ++i){
                for(auto j = matrix[(*face)[i]].begin(); j != matrix[(*face)[i]].end(); ++j){
                    std::set<int> helpSet = face->toSet();
                    if((*j != face->next(i)) && (*j != face->prev(i)) && (helpSet.find(*j) != helpSet.end())){
                        Graph graph;
                        graph.addEdge((*face)[i], *j);
                        graph.addEdge(*j, (*face)[i]);
                        res.push_back(graph);
                        ++resSize;
                    }
                }
            }
        }
        return res;
    }

    bool isInFace(Face face, std::set<int> outNodes){
        std::set<int> faceNodes = face.toSet();
        bool res = true;
        for(int i = 0; i < matrix.size(); ++i){
            if((matrix[i].size() > 0) && (outNodes.find(i) != outNodes.end()) && (faceNodes.find(i) == faceNodes.end())){
                res = false;
                break;
            }
        }
        return res;
    }

    void gErace(Face f){
        for (int i = 0; i < f.getSize(); ++i) {
            deleteEdge(f[i], f.next(i));
        }
    }

    std::vector<Graph> getComps(){
        std::vector<Graph> comps = split();
        std::vector<Graph> finComps;
        for(auto graph = comps.begin(); graph != comps.end(); ++graph) {
            std::vector<Graph> subComps = deleteBriges();
            for (auto subGraph = subComps.begin(); subGraph != subComps.end(); ++subGraph)
                finComps.push_back(*subGraph);
        }
        return finComps;
    }

};

bool getMinFaced(std::vector<Graph> segments, std::vector<Face> faces, Graph& res, Face& faceRes, std::set<int> outNodes){
    std::vector<int> gs(segments.size());
    std::vector<Face> ansfaces(segments.size());
    for(int i = 0; i < segments.size(); ++i){
        gs[i] = 0;
        for(auto j = faces.begin(); j != faces.end(); ++j){
            if(segments[i].isInFace(*j, outNodes)){
                ++gs[i];
                ansfaces[i] = *j;
            }
        }
    }
    int minInd = 0;
    for(int i = 0; i < segments.size(); ++i){
        if(gs[i] < gs[minInd])
            minInd = i;
    }
    if (gs[minInd] == 0)
        return false;
    res = segments[minInd];
    faceRes = ansfaces[minInd];
    return true;
}

bool gammaAlg(Graph g){
    if( !g.hasCycle(0)){
        return true;
    }
    std::vector<Face> faces;
    int fSize = 0;
    Chain c;
    c = g.getCycle();
    faces.push_back(c);
    faces.push_back(c);
    std::set<int> faceNodes;
    for(int j = 0; j < c.getSize(); ++j){
        faceNodes.insert(c[j]);
    }
    g.gErace(faces[fSize]);
    while(g.getSize() > 0){
        std::vector<Graph> segm = g.getSegments(faces, faceNodes);
        Face faceToPut;
        Graph segmToLay;
        if(segm.size() == 0 || !getMinFaced(segm, faces, segmToLay, faceToPut, faceNodes))
            return false;
        Chain toSplit;
        toSplit = segmToLay.getChain(faceToPut);
        std::pair<Face, Face> p = faceToPut.split(toSplit);

        for(int i = 0; i < p.first.getSize(); ++i){
            faceNodes.insert(p.first[i]);
        }
        for(int i = 0; i < p.second.getSize(); ++i){
            faceNodes.insert(p.second[i]);
        }

        for (int k = 0; k < faces.size(); ++k) {
            if(faces[k] == faceToPut){
                faces[k] = p.first;
                faces.push_back(p.second);
                break;
            }
        }
        ++fSize;
        g.gErace(p.first);
        g.gErace(p.second);
    }
    return true;
}


int main() {
    int n, m, a, b;
    std::cin >> n >> m;
    Graph g = Graph(n);
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b;
        g.addEdge(a, b);
        g.addEdge(b, a);
    }

    std::vector<Graph> components = g.getComps();
    bool isPlane = true;


    for(auto graph = components.begin(); graph != components.end(); ++graph){
        if(!gammaAlg(*graph)){
            isPlane = false;
            break;
        }
    }

    if(isPlane)
        std::cout << "YES";
    else
        std::cout << "NO";


    return 0;
}
