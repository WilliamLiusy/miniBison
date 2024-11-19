#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

typedef string token;
struct rule {
    token Left;
    vector<token> Right;
};

vector<token> TerminateToken;
vector<token> NonTerminateToken; // 0 is starting token
vector<rule> Production;

unordered_map<token, unordered_set<token>> First, Follow;


void ReadGrammar() {
    cout << "Input number of NonTerminate Token: " << endl;
    int num_nt;
    cin >> num_nt;
    cout << "Input NonTerminate Token: " << endl;
    for (int i = 0; i < num_nt; i++) {
        token NT;
        cin >> NT;
        NonTerminateToken.push_back(NT);
    }
    cout << "Input number of Terminate Token: " << endl;
    int num_t;
    cin >> num_t;
    cout << "Input Terminate Token: " << endl;
    for (int i = 0; i < num_t; i++) {
        token T;
        cin >> T;
        TerminateToken.push_back(T);
    }
    cout << "Input number of production rules: " << endl;
    int num_rule;
    cin >> num_rule;
    getchar();
    cout << "Input production rules (change line): " << endl;
    for(int i = 0; i < num_rule; i++) {
        string production;
        getline(cin, production);
        // cout << " !!!" << production << endl;
        int l = production.size();
        stringstream ss;
        ss << production;
        rule r;
        ss >> r.Left;
        for(int j = 0; j < l - 1; j++) {
            token tmp;
            ss >> tmp;
            r.Right.push_back(tmp);
        }
        Production.push_back(r);
    }
}

void getTestGrammar() {
    NonTerminateToken.push_back("E");
    NonTerminateToken.push_back("L");
    TerminateToken.push_back("ID");
    TerminateToken.push_back("(");
    TerminateToken.push_back(")");
    TerminateToken.push_back("TN");
    TerminateToken.push_back("NAT");
    TerminateToken.push_back(",");
    Production.push_back({"L", {"E"}});
    Production.push_back({"L", {"L", ",", "E"}});
    Production.push_back({"E", {"ID", "(", ")"}});
    Production.push_back({"E", {"ID", "(", "L", ")"}});
    Production.push_back({"E", {"(", "TN", ")", "E"}});
    Production.push_back({"E", {"NAT"}});
    Production.push_back({"E", {"ID"}});
}

void getFirst() {
    for(auto T: TerminateToken) {
        First[T].insert(T);
    }
    bool changed = false;
    do {
        changed = false;
        for(auto r: Production) {
            token Y = r.Left;
            token Z = r.Right[0];
            for(auto u: First[Z]) {
                if(First[Y].find(u) == First[Y].end()) {
                    First[Y].insert(u);
                    changed = true;
                }
            }
        }
    } while(changed);
}

void getFollow() {
    for(auto r: Production) {
        for(int i = 0; i + 1 < r.Right.size(); i++) {
            for(auto u: First[r.Right[i + 1]]) {
                if(Follow[r.Right[i]].find(u) == Follow[r.Right[i]].end()) {
                    Follow[r.Right[i]].insert(u);
                }
            }
        }
    }
    bool changed = false;
    do {
        changed = false;
        for(auto r: Production) {
            token Z = r.Left;
            token Y = r.Right[r.Right.size() - 1];
            for(auto u: Follow[Z]) {
                if(Follow[Y].find(u) == Follow[Y].end()) {
                    Follow[Y].insert(u);
                    changed = true;
                }
            }
        }
    } while(changed);
}

void getNFA() {}

void judge_MoveinReduction() {
    for(auto s: NonTerminateToken) {

    }
}

int main() {
    getTestGrammar();
    getFirst();
    getFollow();
    return 0;
}
