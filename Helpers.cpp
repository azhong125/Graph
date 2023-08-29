#include "Helpers.h"

Graph::Graph() {
}

Graph::~Graph() {
    for (Node* n : nodes) {
        delete n;
    }
    for (Edge* e: edges ) {
        delete e;
    }
}

bool Graph::containsNode(string name) {
    if (maps.find(name) == maps.end()) return false;
    return true;
}
Node* Graph::findNode(string name) {
    return maps[name];
}

void Graph::insertNode(string &name) {
    if (maps.find(name) == maps.end()) {
        //not in map
        //create new node
        Node* newNode = new Node;
        newNode->name = name;
        maps[name] = newNode;
        nodes.insert(newNode);
    }
}

map<string, Node*> Graph::getMap() {
    return maps;
}

void Graph::insertEdge(string prev, string curr, int cost, string type, string line) {
    Edge* edge = new Edge;
//    edge.first = prev;
//    edge.next = curr;
    edge->cost = cost;
    edge->type = type;
    edge->line = line;
    edges.insert(edge);

    Node *node1 = maps[prev];
    Node *node2 = maps[curr];

    pair<Node*, Edge*> pair1;
    pair1.first = maps[curr];
    pair1.second = edge;
    node1->connections.push_back(pair1);

    pair<Node*, Edge*> pair2;
    pair2.first = maps[prev];
    pair2.second = edge;
    node2->connections.push_back(pair2);

}