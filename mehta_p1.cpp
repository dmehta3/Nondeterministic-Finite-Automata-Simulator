#include <bits/stdc++.h>
using namespace std;

class state;    //forward declaration
class transition{
    public:  
        state* from;
        char symbol;
        state* to;      
        transition(state* from, char symbol, state* to) //constructor
        {
            this->to = to;
            this->from = from;
            this->symbol = symbol;
        }
};
class state {
    public:
        vector<transition*> transitions;
        int id;
        bool start;
        bool accept;
        state(int id, bool start, bool accept) //constructor
        {
            this->id = id;
            this->start = start;
            this->accept = accept;
        }
};
int main(int argc, char* argv[]) {
    ifstream in(argv[1]);
    vector<transition*> Transitions;    //main list of transitions which will be used to make transition vectors for each state
    vector<state*> States;  //main list of all the states
    string line;    //each line from the input file
    while (getline(in, line)) {
        istringstream iss(line);
        string firstWord;
        iss >> firstWord;
        if (firstWord == "state") {     //create an accept, start, or (accept and start) state
            int num;
            string specialty;
            string specialty2;  //will be used if accept and start
            iss >> num >> specialty >> specialty2;
            if (specialty2 == "start" || specialty2 == "accept") {
                state* s = new state(num, true, true);
                States.push_back(s);
            }
            else if (specialty == "start") {
                state* s = new state(num, true, false);
                States.push_back(s);
            }
            else {
                state* s = new state(num, false, true);
                States.push_back(s);
            }
        }
        else {  //transition line
            int p;
            char x;
            int q;
            iss >> p >> x >> q;
            bool p_flag = false;
            bool q_flag = false;
            for (state* s : States) {   //check if either p or q is already existing
                if (s->id == p) {
                    p_flag = true;        
                }
                if (s->id == q) {
                    q_flag = true;
                }
            }
            if(!p_flag) {       //doesn't exist? create a state 
                state* s = new state(p, false, false);
                States.push_back(s);
            }
            if(!q_flag && (p != q)) {
                state* s = new state(q, false, false);
                States.push_back(s);
            }
            state* pstate;
            state* qstate;
            for (state* s : States) {   //assign pointers to p and q
                if (s->id == p) {
                    pstate = s;     
                }
                if (s->id == q) {
                    qstate = s;
                }
            }
            transition* t = new transition(pstate, x, qstate);  //create a transition
            Transitions.push_back(t);
        }
    }
    in.close();
    state* start;
    vector<state*> accept;
    for (state* s : States) {   //for each state, loop through all transitions and add them to the state transitions if p matches and find the start state
        for (transition* t : Transitions) {
            if (t->from->id == s->id) {
                s->transitions.push_back(t);
            }
        }
        if (s->start == true) {
            start = s;
        }
        if(s->accept == true) {
            accept.push_back(s);
        }
    }
    string input = argv[2];
    vector<int> configurations;
    configurations.push_back(start->id);
    int i = 0;
    while (i < input.length()) {
        char c = input[i];
        int limit = configurations.size();
        for (int j = 0; j < limit; ++j) {
            if (configurations[j] != -1) {
                int curr = configurations[j];
                state* temp;
                for (state* s : States) {
                    if (s->id == curr) {
                        temp = s;
                    }
                }
                bool working = false;
                for (transition* t : temp->transitions) {
                    if (t->symbol == c && working == false) {
                        configurations[j] = t->to->id;
                        working = true;
                        continue;
                    }
                    if (t->symbol == c && working == true) {
                        configurations.push_back(t->to->id);
                    }
                }
                if (working == false) {
                    configurations[j] = -1;
                }
            }
        }
        i++;
    }
    cout << configurations.size() << endl;

    bool y = false;
    for (int i = 0; i < configurations.size(); i++) {
        for (int j = 0; j < accept.size(); j++) {
            if (configurations[i] == accept[j]->id && y == false) {
                cout << "accept" << configurations[i];
                y = true;
                continue;
            }
            if (configurations[i] == accept[j]->id && y == true) {
                cout << " " << configurations[i];
            }
        }
    }
    if (y == false) {
        cout << "reject" << endl;
    }
}