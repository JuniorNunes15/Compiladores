#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Bloco {
    int index; //numero do bloco
    int val = 0; //numero de quantas linhas de codigo do bloco
    vector<string> codigo; //a linha de codigo
    vector<int> aponta; //vai guardar os valores para onde esta apontando
    vector<int> recebe; //vai guardar os valores de quem esta apontando para ele
};

struct longevidade {
    string in = "";
    string def = "";
    string use = "";
    string out = "";
};

/***************ANALISE DE LONGEVIDADE*********************/
string Use(string u, string b, string check) {
    string aux = "", xx;
    bool pegar = false;
    char a;

    xx = b;
    a = xx[0];
    for(int j = 0; j < xx.size(); j++) {
        if(xx[j] == ' ') {} //do  nothing
        else if(xx[j] == '=') {
            pegar = true;
        }
        else if(pegar && (xx[j] >= 'a' && xx[j] <= 'z')) {
            if(aux.find(xx[j]) != std::string::npos) {}
            else {
                if(check.find(xx[j]) != std::string::npos) {}
                else {
                    aux += xx[j];
                    aux += ',';
                }
            }
        }
    }
    if(!pegar) {
        for(int j = 1; j < xx.size(); j++) {
            if((j+1 == xx.size() && (xx[j] >= 'a' && xx[j] <= 'z')) || (xx[j-1] == ' ' && xx[j+1] == ' ' && (xx[j] >= 'a' && xx[j] <= 'z'))) {
                if(check.find(xx[j]) == std::string::npos) {
                    aux += xx[j];
                    aux += ',';
                }
            }
        }
    }
    return aux;
}

string define(string use, string check) {
    string aux = "";
    for(int i = 0; i < check.size(); i++) {
        if(check[i] == '=') {
            if(use.find(check[0]) == std::string::npos) {
                aux += check[0];
                aux += ',';
            }
            break;
        }
    }
    return aux;
}

string out(Bloco *bloco, int N, longevidade* log, int x) {
    string aux = "";
    string aa;
    string nova_aux = log[x].def;
    nova_aux += log[x].in;
    int j = x;
    while(x < N) {
        for(int i = 0; i < bloco[x].aponta.size(); i++) {
            aa = log[bloco[x].aponta[i]-1].use;
            for(int k = 0; k < aa.size(); k++) {
                if(nova_aux.find(aa[k]) != std::string::npos) {
                    if(aux.find(aa[k]) == std::string::npos) {
                        aux += aa[k];
                    }
                }
            }
        }
        x++;
    }
    x = j;
    return aux;
}

string in(Bloco *bloco, int N, longevidade* log, int x) {
    string aux = "";
    string aa;
    string nova_aux = log[x].def;
    int j = x;
    while(x < N) {
        for(int i = 0; i < bloco[x].aponta.size(); i++) {
            aa = log[bloco[x].aponta[i]-1].use;
            for(int k = 0; k < aa.size(); k++) {
                if(log[x].def.find(aa[k]) == std::string::npos) {
                    if(aa[k] != ',') {
                        if(aux.find(aa[k]) == std::string::npos) {
                            if(nova_aux.find(aa[k]) == std::string::npos) {
                                aux += aa[k];
                                aux += ',';
                            }
                        }
                    }
                }
            }
        }
        x++;
    }
    x = j;
    for(int i = 0; i < log[x].use.size(); i++) {
        aa = log[x].use;
        if(aux.find(aa[i]) == std::string::npos) {
            if(aa[i] != ',') {
                aux += aa[i];
                aux += ',';
            }
        }
    }
    return aux;
}

void analise_longevidade(Bloco *bloco, int N, longevidade* log) {
    string u;
    
    for(int i = 0; i < N; i++) {
        string uti_aux = "";
        for(int j = 0; j < bloco[i].val; j++) {
            u = Use(u, bloco[i].codigo[j], uti_aux);
            uti_aux += bloco[i].codigo[j][0];
            uti_aux += u;
            log[i].use += u;

            log[i].def += define(log[i].use, bloco[i].codigo[j]);
        }
    }
    for(int i = 0; i < N; i++) {
        log[i].in += in(bloco, N, log, i);
    }
    for(int i = 0; i < N; i++) {
        log[i].out += out(bloco, N, log, i);
    }
}
/************************************************************/

/*******************Reaching Definitions*********************/
void gen(Bloco* bloco, int N, longevidade* log) {
    int x = 1;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < bloco[i].codigo.size(); j++) {
            if(bloco[i].codigo[j].find('=') != std::string::npos) {
                log[i].def += to_string(x);
                x++;
            }
        }
    }
}

void kill(Bloco* bloco, int N, int x, int b, char a, longevidade* log) {
    string aux = "";
    char compare;
    int d = 0;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < bloco[i].codigo.size(); j++) {
            aux = bloco[i].codigo[j];
            if(aux[0] == a) {
                if(x == i && b == j) {}
                else {
                    log[x].use += log[i].def[d];
                }
            }
            d += 1;
        }
        d = 0;
    }
}

void Reaching_in(Bloco* bloco, int N, int x, longevidade* log) {
    string aux = "", aa = "";

    for(int i = 0; i < bloco[x].recebe.size(); i++) {
        aa += log[bloco[x].recebe[i]-1].out;
    }
    log[x].in += aa;

    for(int i = 0; i < log[x].in.size(); i++) {
        if(log[x].use.find(aa[i]) == std::string::npos) {
            if(log[x].out.find(aa[i]) == std::string::npos) {
                aux += aa[i];
            }
        }
    }
    log[x].out += aux; 
}

string arruma(string x) {
    string aux = "";
    for(int i = 0; i < x.size(); i++) {
        if(i+1 == x.size()) {
            aux += "d";
            aux += x[i];
        }
        else {
            aux += "d";
            aux += x[i];
            aux += ",";
        }
    }
    return aux;
}

void Reaching_Definitions(Bloco *bloco, int N, longevidade* log) {

    gen(bloco, N, log);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < bloco[i].codigo.size(); j++) {
            kill(bloco, N, i, j, bloco[i].codigo[j][0], log);
        }
        log[i].out += log[i].def;
    }
    for(int i = 0; i < N; i++) {
        Reaching_in(bloco, N, i, log);
    }
    for(int i = 0; i < N; i++) {
        log[i].in = arruma(log[i].in);
        log[i].out = arruma(log[i].out);
        log[i].def = arruma(log[i].def);
        log[i].use = arruma(log[i].use);
    }
}
/*************************************************************/

/**************************Avaliable Expressions*******************/
string def_avaliable(Bloco bloco) {
    string aux = "";
    bool x = false;
    for(int i = 0; i < bloco.codigo.size(); i++) {
        for(int j = 0; j < bloco.codigo[i].size(); j++) {
            if(bloco.codigo[i][j] == '=') {
                x = true;
            }
            else if(x && bloco.codigo[i][j] != ' '){
                aux += bloco.codigo[i][j];
            }
        }
        x = false;
        aux += ",";
    }
    return aux;
}

string kill_expre(Bloco bloco) {
    string aux = "";
    for(int i = 0; i < bloco.codigo.size(); i++) {
        if(bloco.codigo[i].find('=') != std::string::npos) {
            aux += bloco.codigo[i][0];
        }
    }
    return aux;
}

void in_avaliable(Bloco* bloco, int N, int x, longevidade* log) {
    string aux = "", aa = "";
    bool findi = false;
    string nova_aux = "";
    string nova_aux_d = "";
    int d = 0;

    for(int i = 0; i < bloco[x].recebe.size(); i++) {
        aa += log[bloco[x].recebe[i]-1].out;
    }
    log[x].in += aa;

    nova_aux = log[x].def + log[x].in;

    for(int i = log[x].use.size()-1; i >= 0; i--) {
        d = 0;
        if(nova_aux.find(log[x].use[i]) != std::string::npos) {
            for(int j = 0; (j < nova_aux.size()  && d < i+1); j++) {
                if(nova_aux[j] == ',' && i == 0) {
                    d++;
                    aux = nova_aux;
                }
                if(nova_aux[j] == ',' && !findi) {
                    nova_aux_d += aux;
                    nova_aux_d += ",";
                }
                else if(nova_aux[j] == ',' && findi) {
                    aux = ",";
                    findi = false;
                }
                else if(log[x].use[i] == nova_aux[j]) {
                    findi = true;
                }
                else {
                    aux += nova_aux[j];
                }
            }
            nova_aux = nova_aux_d;
            nova_aux_d = "";
        }
        else {
            //nova_aux += log[x].def;
        }
    }
    log[x].out += nova_aux; 
}


void Avaliabre_Expressions(Bloco *bloco, int N, longevidade* log) {
    for(int i = 0; i < N; i++) {
        log[i].def += def_avaliable(bloco[i]);
        log[i].use += kill_expre(bloco[i]);
    }
    for(int i = 0; i < N; i++) {
        in_avaliable(bloco, N, i, log);
    }
}
/*****************************************************************************/

int main() {

    int N,M;
    int index_aux, val_aux;
    string codigo_aux;

    cout << "digite N: "; 
    cin >> N;
    
    Bloco bloco[N];
    longevidade* long_resposta = new longevidade[N];
    longevidade* long_definitions = new longevidade[N];
    longevidade* long_avaliable_expre = new longevidade[N];

    for(int i = 0; i < N; i++) {
        cout << "digite M: ";
        cin >> M;
        cin.ignore();
        for(int j = 0; j < M; j++) {
            cout << "digite o codigo:";
            getline(cin, codigo_aux);
            bloco[i].index = i+1;
            bloco[i].val++;
            bloco[i].codigo.push_back(codigo_aux);
        }
        cout << "digite o valor de quantos ponteiros apontam:";
        cin >> index_aux;    
        while (index_aux > 0) {
            cout << "digite para onde ele aponta: ";
            cin >> val_aux;
            bloco[i].aponta.push_back(val_aux);
            bloco[val_aux-1].recebe.push_back(i+1);
            index_aux--;
        }
    }

    analise_longevidade(bloco, N, long_resposta);
    Reaching_Definitions(bloco, N, long_definitions);
    Avaliabre_Expressions(bloco, N, long_avaliable_expre);
    
    cout << endl << "Analise de Longevidade!" << endl;
    for(int i = 0; i < N; i++) {
        cout << "B" << bloco[i].index << ": in = {" << long_resposta[i].in << "}" << endl;
        cout << "B" << bloco[i].index << ": def = {" << long_resposta[i].def << "}" << endl;
        cout << "B" << bloco[i].index << ": use = {" << long_resposta[i].use << "}" << endl;
        cout << "B" << bloco[i].index << ": out = {" << long_resposta[i].out << "}" << endl;
    }
    cout << "Reaching Definitions!" << endl;
    for(int i = 0; i < N; i++) {
        cout << "B" << bloco[i].index << ": in = {" << long_definitions[i].in << "}" << endl;
        cout << "B" << bloco[i].index << ": gen = {" << long_definitions[i].def << "}" << endl;
        cout << "B" << bloco[i].index << ": kill = {" << long_definitions[i].use << "}" << endl;
        cout << "B" << bloco[i].index << ": out = {" << long_definitions[i].out << "}" << endl;
    }
    cout << "Avaliable Expressions!" << endl;
    for(int i = 0; i < N; i++) {
        cout << "B" << bloco[i].index << ": in = {" << long_avaliable_expre[i].in << "}" << endl;
        cout << "B" << bloco[i].index << ": gen = {" << long_avaliable_expre[i].def << "}" << endl;
        cout << "B" << bloco[i].index << ": kill = {" << long_avaliable_expre[i].use << "}" << endl;
        cout << "B" << bloco[i].index << ": out = {" << long_avaliable_expre[i].out << "}" << endl;
    }
    /*for(int i = 0; i < N; i++) {
        for(int j = 0; j < bloco[i].aponta.size(); j++) {
        cout << "b" << bloco[i].index << ": aponta = {" << bloco[i].aponta[j] << "}" << endl;
        cout << "b" << bloco[i].index << ": recebe = {" << bloco[i].recebe[j] << "}" << endl;
        }
    }*/

    return 0;
}
