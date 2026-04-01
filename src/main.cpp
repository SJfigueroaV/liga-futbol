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

bool partidoDuplicado(Game nuevo, vector<Game> partidos) {
    for (int i = 0; i < partidos.size(); i++) {
        if (partidos[i].local == nuevo.local && 
            partidos[i].visiting == nuevo.visiting) {
            return true;
        }
    }
    return false;
}

void guardarPartido(Game g, vector<Game> partidos ){
  if (partidoDuplicado(g, partidos)){
  cout << "Error: ese partido ya fue registrado" << endl;
  return;
  }
  ofstream archivo("data/partidos.txt", ios::app);
  if (archivo.is_open()){
    archivo << g.date << "|";
    archivo << g.local << "|";
    archivo << g.visiting << "|";
    archivo << g.lgoals << "|" ;
    archivo << g.vgoals << "\n";
    archivo.close();
  } else {
    cout << "No se pudo guardar el archivo";
    exit(1);
  }
}

vector<Game> leerPartidos() {
    vector<Game> partidos;
    ifstream archivo("data/partidos.txt");
    if (!archivo.is_open()) return partidos; // si no existe, retorna vacío
    string linea;
    while (getline(archivo, linea)) {
    stringstream ss(linea);
    string date, local, visiting, lgoals, vgoals;
    getline(ss, date, '|');
    getline(ss, local, '|');
    getline(ss, visiting, '|');
    getline(ss, lgoals, '|');
    getline(ss, vgoals);
    Game g;
    g.date = date;
    g.local = local;
    g.visiting = visiting;
    g.lgoals = stoi(lgoals);
    g.vgoals = stoi(vgoals);
    partidos.push_back(g);
    
    }
    archivo.close();
    return partidos;
}

int main() {
    ConfigLiga config = leerConfig("data/config.txt");
    cout << "Liga: " << config.name << endl;

    // Prueba guardar un partido
    Game g;
    g.date = "2024-01-15";
    g.local = "Real Madrid";
    g.visiting = "Barcelona";
    g.lgoals = 2;
    g.vgoals = 1;
    vector<Game> partidos = leerPartidos();
    guardarPartido(g, partidos);

    // Prueba leer partidos
    cout << "Partidos guardados: " << partidos.size() << endl;

    return 0;
}
