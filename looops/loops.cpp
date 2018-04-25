#include <bits/stdc++.h>

using namespace std;

struct Vertex {
    string type;
    int id, time;
    Vertex *parent, *sdom, *label, *anc, *dom;
    vector<Vertex *> to, from, bucket;

    Vertex(int id, const string &type) : time(0), id(id), type(type),
                                         sdom(this), label(this), parent(nullptr),
                                         anc(nullptr), dom(nullptr) {}

    void dfs(deque<Vertex *> &dst, int &time);

    Vertex *find();
};

void Vertex::dfs(deque<Vertex *> &dst, int &time) {
    this->time = time++;
    dst.emplace_front(this);
    for (auto u : this->to) {
        u->from.emplace_back(this);
        if (u->time == 0) {
            u->parent = this;
            u->dfs(dst, time);
        }
    }
}

Vertex *Vertex::find() {
    if (this->anc == nullptr) {
        return this;
    }
    stack<Vertex *> s;
    Vertex *u = this;
    while (u->anc->anc != nullptr) {
        s.push(u);
        u = u->anc;
    }
    while (!s.empty()) {
        Vertex *v = s.top();
        s.pop();
        if (v->anc->label->sdom->time < v->label->sdom->time) {
            v->label = v->anc->label;
        }
        v->anc = u->anc;
    }
    return this->label;
}

void dominators(deque<Vertex *> &graph) {
    for (auto w = graph.begin(); w != graph.end() - 1; w++) {
        for (auto v : (*w)->from) {
            auto u = v->find();
            if (u->sdom->time < (*w)->sdom->time) {
                (*w)->sdom = u->sdom;
            }
        }
        (*w)->anc = (*w)->parent;
        (*w)->sdom->bucket.emplace_back(*w);
        for (auto v : (*w)->parent->bucket) {
            auto u = v->find();
            if (u->sdom == v->sdom) {
                v->dom = (*w)->parent;
            } else {
                v->dom = u;
            }
            (*w)->parent->bucket.clear();
        }
    }
    for (auto w = graph.rbegin() + 1; w != graph.rend(); w++) {
        if ((*w)->dom != (*w)->sdom) {
            (*w)->dom = (*w)->dom->dom;
        }
    }
}

int main() {
    int n;
    cin >> n;
    Vertex *g[n];
    unordered_map<int, pair<Vertex *, vector<int>>> h;
    for (int i = 0; i < n; i++) {
        int id;
        string type;
        cin >> id >> type;
        g[i] = new Vertex(id, type);
        h.emplace(id, make_pair(g[i], vector<int>(1)));
        if (type != "ACTION") {
            cin >> h[id].second[0];
        }
    }
    for (int i = 0; i < n; i++) {
        if (g[i]->type != "JUMP" && i != n - 1) {
            g[i]->to.emplace_back(g[i + 1]);
        }
        if (g[i]->type != "ACTION") {
            g[i]->to.emplace_back(h[h[g[i]->id].second[0]].first);
        }
    }
    deque<Vertex *> graph;
    int time = 1;
    g[0]->dfs(graph, time);
    dominators(graph);
    int ans = 0;
    for (auto v : g) {
        for (auto u : v->from) {
            Vertex *x = u->dom;
            if (x == nullptr || x->time < v->time) {
                continue;
            }
            while (x != nullptr && x != v) {
                x = x->dom;
            }
            if (x != nullptr) {
                ans++;
                break;
            }
        }
    }
    cout << ans << endl;
    for (auto v : g) {
        delete v;
    }
    return 0;
}
