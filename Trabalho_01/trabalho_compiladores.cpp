#include <iostream>
#include <string>
#include <vector>

using namespace std;

/******funcoes auxiliares*******/
string separa(string linguagem, int* aux) {
    string x = "";
    cout << *aux;
    while(linguagem[*aux] != ' ' && linguagem[*aux] != '-') {
        x += linguagem[*aux];
    }
    return x;
}

string separa_terminais(string linguagem, int num) {
    string aux = "";
    for(int i = num; i < linguagem.length(); i++) {
        if(linguagem[i] == ' ') {} //nao fazer nada pois é apenas espaco em branco e tem que ser ignorado
        else if(linguagem[i] == '-' && linguagem[i+1] == '>' ) {
            i++;
        } //aumentar o i em 1, apenas para ignorar a seta
        else {
            aux += linguagem[i];
        }
    }
    return aux;
}

string unir_firsts(vector<string> naoTerminais, vector<string> val_first, int k) {
    string nova = "";
    string x = naoTerminais[k];
    bool existe = false;
    int i = k;
    for(int j = i-1; j >= 0; j--) {
            if(naoTerminais[i]==naoTerminais[j]) {
                existe = true;
            break;
        }
    }
    if(existe) {
        return "";
    }
    else {
        for(i = k; i < naoTerminais.size(); i++) {
            if(x == naoTerminais[i]) {
                if(nova.find(val_first[i]) != std::string::npos) {}
                else {
                    nova += val_first[i];
                }
            }
        }
    return "First("+x+") = "+nova;
    }
}

string unir_follows(vector<string> naoTerminais, vector<string> val_follows, int k) {
    string nova = "";
    string x = val_follows[k];
    bool existe = false;
    int i = k;
    for(int j = i-1; j >= 0; j--) {
            if(naoTerminais[i]==naoTerminais[j]) {
                existe = true;
            break;
        }
    }
    if(existe) {
        return "";
    }
    else {
        for(i = 0; i < x.size(); i++) {
            //cout << nova << endl;
            if(nova.find(x[i]) != std::string::npos) {}
            else {
                nova += x[i];
            }
        }
    return "Follow("+naoTerminais[k]+") = "+nova;
    }
}
/*********************************************/

/**********First e Follows************/
string First(vector<string> naoTerminais, vector<string> terminais, int val) {
    string first = "";
    string saux = "";
    size_t aux = 0;
    string n = terminais[val];

    if((n[0] >= 'A') && (n[0]) <= 'Z') {
        saux += n[0];
        while(true) {
            if(n[aux+1] == '\'') {
                saux += '\'';
            }
            else{
                break;
            }
            aux++;
        }
        for(int j = 0; j < naoTerminais.size(); j++) {
            if(j == val) {}
            else if(saux == naoTerminais[j]) {
                first += First(naoTerminais, terminais, j);
            }
        }
    }
    else {
        first += n[0];
    }
    return first;
}

string Follow(vector<string> naoTerminais, vector<string> terminais, int val) {
    string follow = "";
    string saux = "";
    int aux = 0;
    string n;
    string s = naoTerminais[val];
    string contem = "";
    int v_contem[naoTerminais.size()];

    for(int i = 1; i < naoTerminais.size(); i++) {
        if(terminais[i].find(s) != std::string::npos) {
            contem += naoTerminais[i];
            v_contem[aux] = i;
            aux++;
        } 
    }
    for(int i = 0; i < terminais.size(); i++) {
        n = terminais[i];
    //if((n.find(naoTerminais[val]) != std::string::npos)) {
        for(int j = 0; j < n.size(); j++) {
            if(n[j] != s[0]) {}
            else {
                follow += n[j+s.size()];
            }
        }
        for(int j = 0; j < contem.size(); j++) {
            size_t f = n.find(contem[j]); 
            if(f != std::string::npos) {
                n.insert(f+1, terminais[v_contem[j]]);
                for(int k = 0; k <= n.size(); k++) {
                    if(n[k] != s[0]) {}
                    else {
                        if(n[k+s.size()] == s[0]) {}
                        else {
                            follow += n[k+s.size()];
                        }
                        break;
                    }   
                }
                break;
            }
        }
    //}
    //else { //}
    }
    return follow;
}
/***********************************************/


/************Algoritmo para fazer o automato*********************/
void print(vector<string> naoTerminais, vector<string> terminais, int val) {
    for(int i = val; i < naoTerminais.size(); i++) {
        cout << naoTerminais[i] << terminais[i] << endl;
    }
}

void fazerAutomato(vector<string> naoTerminais, vector<string> terminais, vector<string> novosNaoTerminais, vector<string> novosTerminais) {
    char t;
    string aux3 = "";
    int tam = novosNaoTerminais.size();
    for(int k = 0; k < tam; k++) {
        string aux = novosTerminais[k];
        string aux2 = novosNaoTerminais[k];
        for(int i = 1; i < aux.size(); i++) {
            t = aux[i];
            for(int j = 0; j < tam; j++) {
                string v = terminais[j];
                string v2 = novosTerminais[j];
                if((v.find(t) != std::string::npos) && (v2[1] == t) ) {
                    cout << "Deriva " << t << endl;
                    bool entrar = true;
                    string a = novosNaoTerminais[j];
                    string b = novosTerminais[j];
                    string c = ".";
                    a += b[1];
                    for(int w = 2; w <= b.size(); w++) {
                        c += b[w];
                        novosNaoTerminais[j] = a;
                        novosTerminais[j] = c;
                        if(entrar) {
                            cout << novosNaoTerminais[j] << novosTerminais[j] << endl;
                            entrar = false;
                        }
                    }
                    for(int w = 1; w < naoTerminais.size(); w++) {
                        string z = naoTerminais[w];
                        string y = terminais[w];
                        if(aux[i+1] == z[0]) {
                            cout << naoTerminais[w] << " -> ." << terminais[w] << endl;
                            for(int m = 1; m < naoTerminais.size(); m++) {
                                string l = naoTerminais[m];
                                if(aux[i+1] != l[0]) {
                                    if(l[0] == y[0]) {
                                        cout << naoTerminais[m] << " -> ." << terminais[m] << endl;
                                    }
                                }
                            }
                        }
                        
                    }
                }
            }
            aux3 += t;
        }
    }
}

void automato(vector<string> naoTerminais, vector<string> terminais, int val, string x) {
    vector<string> novosNaoTerminais;
    vector<string> novosTerminais;
    vector<string> naoTerminaisAux;
    vector<string> terminaisAux;
    for(int i = 0; i < naoTerminais.size(); i++) {
        string x = naoTerminais[i];
        string y = terminais[i];
        x += " -> ";
        y = "." + y;
        novosNaoTerminais.push_back(x);
        novosTerminais.push_back(y);
    }
    print(novosNaoTerminais, novosTerminais, 0);
    fazerAutomato(naoTerminais, terminais, novosNaoTerminais, novosTerminais);
}
/*************************************************************/


int main() {

    vector<string> naoTerminais;
    vector<string> terminais;
    vector<string> val_first;
    vector<string> val_follow;
    string aux = "";
    int i;

    cout << "Adicione a linguagem:<aperte enter para parar de adicionar>" << endl;
    string linguagem = "naoVazia"; //string iniciada com algo apenas para entrar no while
    while(linguagem != "") {
        getline(cin, linguagem);
        if(linguagem != "") {
            aux = "";
            for(i = 0; linguagem[i] != ' ' && linguagem[i] != '-'; i++) {
                aux += linguagem[i];
            }
            naoTerminais.push_back(aux);
            terminais.push_back(separa_terminais(linguagem, i));
        }
    }

    cout << "Calculando First e Follows: " << endl;
    for(int i = 0; i < naoTerminais.size(); i++) {
        val_first.push_back(First(naoTerminais, terminais, i));
    }
    for(int i = 0; i < naoTerminais.size(); i++) {
        cout << unir_firsts(naoTerminais, val_first, i) << endl; 
    }
    for(int i = 1; i < naoTerminais.size(); i++) {
        val_follow.push_back(Follow(naoTerminais, terminais, i));
    }
    for(int i = 1; i < naoTerminais.size()-1; i++) {
        cout << unir_follows(naoTerminais, val_follow, i) << endl; 
    }

    cout << "Fazendo o automato: " << endl;
    automato(naoTerminais, terminais, 0, "");

    
    /*for(const auto& str : val_first) {
        cout << str << endl;
    }/*
    for(const auto& str : terminais) {
        cout << str << endl;
    }
    for(const auto& str : val_first) {
        cout << str << endl;
    }*/


    return 0;
}