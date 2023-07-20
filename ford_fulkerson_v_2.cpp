#include <iostream>
const int INT_MAX = 1073741823;

struct Brid {
    int v;
    int tok;
    int kapacitet;
    int obratan;

    Brid(int v, int kapacitet, int obratan) : v(v), tok(0), kapacitet(kapacitet), obratan(obratan) {}
};

class FordFulkerson {
    int V;
    int** graph;
    bool* posjecen;

    bool dfs(int u, int sliv, int* roditelj) {
        posjecen[u] = true;

        if (u == sliv) {
            return true;
        }

        for (int v = 0; v < V; v++) {
            if (!posjecen[v] && graph[u][v] > 0) {
                roditelj[v] = u;
                if (dfs(v, sliv, roditelj)) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    FordFulkerson(int V) : V(V), graph(new int*[V]), posjecen(new bool[V]) {
        for (int i = 0; i < V; i++) {
            graph[i] = new int[V];
            std::fill(graph[i], graph[i] + V, 0);
            posjecen[i] = false;
        }
    }

    void dodajBrid(int u, int v, int kapacitet) {
        graph[u][v] = kapacitet;
        graph[v][u] = kapacitet;
    }

    int max_tok(int izvor, int sliv) {
        int* roditelj = new int[V];
        int max_tok = 0;

        while (dfs(izvor, sliv, roditelj)) {
            int put_tok = INT_MAX;

            for (int v = sliv; v != izvor; v = roditelj[v]) {
                int u = roditelj[v];
                if (graph[u][v] > 0) {
                    put_tok = std::min(put_tok, graph[u][v]);
                } else {
                    put_tok = std::min(put_tok, -graph[v][u]);
                }
            }

            for (int v = sliv; v != izvor; v = roditelj[v]) {
                int u = roditelj[v];
                if (graph[u][v] > 0) {
                    graph[u][v] -= put_tok;
                } else {
                    graph[v][u] += put_tok;
                }
            }
            max_tok += put_tok;

            std::fill(posjecen, posjecen + V, false);
        }
        delete[] roditelj;
        return max_tok;
    }

    ~FordFulkerson() {
        for (int i = 0; i < V; i++) {
            delete[] graph[i];
        }
        delete[] graph;
        delete[] posjecen;
    }
};

int main() {
    int V = 6;

    FordFulkerson graph(V);

    graph.dodajBrid(0, 1, 9);
    graph.dodajBrid(0, 2, 7);
    graph.dodajBrid(1, 3, 8);
    graph.dodajBrid(2, 3, 5);
    graph.dodajBrid(2, 4, 3);
    graph.dodajBrid(3, 4, 7);
    graph.dodajBrid(4, 0, 6);

    int izvor = 0;
    int sliv = 3;

    int max_tok = graph.max_tok(izvor, sliv);
    std::cout << "Maksimalni protok mreze: " << max_tok << std::endl;

    return 0;
}