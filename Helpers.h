#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <set>
using namespace std;

struct Edge{
    int cost;
    string type;
    string line;
};

struct Node {
    string name;
    //vector<Edge> edges;
    vector<pair<Node*, Edge*>> connections;
    //make connections a vector of pairs between node* to the next node and edge
    //int currDist;

//    bool operator < (const Node& other) const {
//        return this->currDist < other.currDist;
//    }
};

class Graph {
    map<string, Node*> maps;
    set<Node*> nodes;
    set<Edge*> edges;
public:
    Graph();
    ~Graph();
    bool containsNode(string name);
    map<string, Node*> getMap();
    Node* findNode(string name);
    void insertNode(string &name);
    void insertEdge(string prev, string curr, int cost, string type, string line);
};

#endif
