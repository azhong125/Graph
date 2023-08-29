#include "Atlas.h"

Atlas::Atlas(std::istream& stream) {
    vector<Data> points;
    string type;
    string line;
    for (string s;getline(stream, s, '\n');) {
        //loops through every line in the file
        istringstream str(s);

        if (s.substr(0,6) == "TRAIN:" || s.substr(0,4) == "BUS:") {
            //this line has the type and line
            int num;
            if (s[0] == 'T') {
                type = "TRAIN";
                num = 7;
            }
            else {
                type = "BUS";
                num = 5;
            }
            line = s.substr(num, s.length() - num);
        }

        if (s[0] == '-') {
            string time = "";
            //line represents a station
            int num = 2;
            //since each line starts with a dash then a space, skip to the number at position 2
            while(isdigit(s[num])) {
                time += s[num];
                num++;
            }
            num++;

            //now the number is saved into the string time
            //get the rest of the line to be the station name
            string station = s.substr(num, s.length() - num);
            int length = station.length();
            int count = 0;
            //trim white space from the beginning of strings
            while (isspace(station[count])) {
                station = station.substr(count + 1, length - count);
            }

            Data data;
            data.name = station;
            data.num = stoi(time);
            data.type = type;
            data.line = line;
            points.push_back(data);

            graph.insertNode(data.name);
        }
    }

    int size = points.size();
    for (int i = 0; i < size - 1; ++i) {
        Data first = points[i];
        Data next = points[i + 1];

        if (first.line == next.line) {
            graph.insertEdge(first.name, next.name, next.num - first.num, first.type, first.line);
        }
    }

//    Node* node = graph.findNode("Sunday");
//    for (int i = 0; i < node->connections.size(); ++i) {
//        Node* next = node->connections[i].first;
//        cout << next->name << endl;
//    }
}

Atlas::~Atlas() {
}

void printQueue(priority_queue<Station*> queue) {
    priority_queue<Station*> copy = queue;
    while (!copy.empty()) {
        cout << "Queue: " << copy.top()->node->name << ", " << copy.top()->dist << endl;
        copy.pop();
    }
}

Trip Atlas::route(const std::string& src, const std::string& dst) {
    set<Station*> toDelete;
    Station* current = new Station;
    current->node = graph.findNode(src);

    current->edge = NULL;
    current->dist = 0;

    priority_queue<Station*, std::vector<Station*>, CMP> queue;
    //min heap
    set<string> checked;
    //store all the nodes that have been checked
    map<string, int> distances;
    //store the routes that you pop off with minimum distances

    queue.push(current);
    //add source node to the queue
    distances[current->node->name] = 0;
    toDelete.insert(current);


    while (current->node->name != dst && !queue.empty()) {
        //keep checking until you find the destination or if the queue is empty which means the destination is not in the graph
        //printQueue(queue);
        current = queue.top();
        queue.pop();

       // cout << "Current: " << current->node->name << ", " << current->dist << endl;
        if (checked.find(current->node->name) == checked.end()) {
            distances[current->node->name] = current->dist;
            for (int i = 0; i < (int) current->node->connections.size(); ++i) {
                if (distances.find(current->node->connections[i].first->name) == distances.end() && checked.find(current->node->connections[i].first->name) == checked.end()) {
                    Station* neighbor = new Station;
                    neighbor->node = current->node->connections[i].first;
                    neighbor->edge = current->node->connections[i].second;
                    neighbor->prev = current;
                    toDelete.insert(neighbor);
                    //cout << "neighbor: " << neighbor->node->name << endl;

                    //potential distance, if shorter than distance then replace the one you have
                    int potential = current->dist;
                    if (neighbor->edge->type == "TRAIN") potential += neighbor->edge->cost;
                    //if the node is not checked yet, add it to the queue
                    neighbor->dist = potential;
                    //distances[neighbor->node->name] = neighbor->dist;
                    queue.push(neighbor);
                    //cout << "Neighbor: " << neighbor->node->name << ", " << neighbor->dist << endl;
                    }
            }
            checked.insert(current->node->name);
        }
    }

    if (current->node->name != dst) {
        for (Station* s : toDelete) {
            delete s;
        }
        throw runtime_error("No route.");
    }

    Trip backwards;
    backwards.start = dst;
    Station* bCurr = current;
    string prevLine = "";
    while (bCurr->node->name != src) {
        Trip::Leg leg;
        leg.line = bCurr->edge->line;
        prevLine = leg.line;
        leg.stop = bCurr->node->name;
        backwards.legs.push_back(leg);
        bCurr = bCurr->prev;
    }
    Trip::Leg leg;
    leg.line = prevLine;
    leg.stop = bCurr->node->name;
    backwards.legs.push_back(leg);
    //add line to source

    Trip forwards;
    forwards.start = src;
    string currentLine = backwards.legs[backwards.legs.size() - 1].line;
    for (int i = (int) backwards.legs.size() - 1; i >= 0; --i) {
        if (currentLine != backwards.legs[i].line) {
            forwards.legs.push_back(backwards.legs[i + 1]);
            currentLine = backwards.legs[i].line;
        }
        if (backwards.legs[i].stop == dst) {
            forwards.legs.push_back(backwards.legs[i]);
            break;
        }
    }

    for (Station* s : toDelete) {
        delete s;
    }

    return forwards;
}

Atlas* Atlas::create(std::istream& stream) {
  // This default implementation will probably do what you want.
  // If you use a different constructor, you'll need to change it.
  return new Atlas(stream);
}
