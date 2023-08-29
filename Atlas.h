#ifndef ATLAS_H
#define ATLAS_H

#include <istream>
#include <sstream>
#include <string>

#include "Trip.h"
#include "Helpers.h"
#include "map"
#include <iostream>
#include <queue>
#include <set>
#include <stdexcept>
#include <climits>
using namespace std;

struct Data {
    int num;
    string name;
    string type;
    string line;
};

struct Station {
    Node* node;
    Edge* edge;

    int dist;
    struct Station* prev;
    //edge is the edge along prev to node

    bool operator < (const Station& other) const {
        return this->dist < other.dist;
    }
};

struct CMP {
    bool operator () (Station* lhs, Station* rhs) const {
        return lhs->dist > rhs->dist;
    }
};

class Atlas {
public:
  // Required Class Function
  static Atlas* create(std::istream& stream);

private:
  // Member Variables
  Graph graph;

public:
  // Constructor & Destructor
  Atlas(std::istream& stream);
  ~Atlas();

  // Required Member Function
  Trip route(const std::string& src, const std::string& dst);
};

#endif
