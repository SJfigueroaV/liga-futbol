#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// Almacena la configuracion leida desde config.txt
struct ConfigLiga {
    string name;
    int wpoints;
    int dpoints;
    int lpoints;
    vector<string> teams;
};

// Representa un partido jugado
struct Game {
    string date;
    string local;
    string visiting;
    int lgoals;
    int vgoals;
};

// Representa un equipo en la tabla de posiciones
struct Team {
    string name;
    int tgames;
    int wgames;
    int dgames;
    int lgames;
    int sgoals;
    int agoals;
    int dgoals;
    int points;
};

// Leer la configuracion y asignar el valor al campo correspondiente del struct
ConfigLiga leerConfig(string ruta){
    ifstream archivo(ruta);
    if (archivo.is_open()){
        ConfigLiga config;
        string linea;
        while (getline(archivo, linea)){
            if (linea[0] == '#') continue;
            if (linea.empty()) continue;
            stringstream ss(linea);
            string clave, valor;
            getline(ss, clave, '=');
            getline(ss, valor);
            if (clave == "liga") {
                config.name = valor;
            } else if (clave == "puntos_victoria"){
                config.wpoints = stoi(valor);
            } else if (clave == "puntos_derrota"){
                config.lpoints = stoi(valor);
            } else if (clave == "puntos_empate"){
                config.dpoints = stoi(valor);
            } else if (clave == "equipo"){
                config.teams.push_back(valor);
            }
        }
        archivo.close();
        return config;
    } else {
        cout << "Error: no se pudo abrir " << ruta << endl;
        exit(1);
    }
}

int main() {
    ConfigLiga config = leerConfig("data/config.txt");
    cout << "Liga: " << config.name << endl;
    cout << "Equipos: " << config.teams.size() << endl;
    return 0;
}
