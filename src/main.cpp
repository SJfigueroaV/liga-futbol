#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
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
    if (linea.empty()) continue;
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
// Actualiza las estadísticas de un equipo según el resultado de un partido
void actualizarEstadisticas(Team* t, int golesAFavor, int golesEnContra, ConfigLiga config){
  t->tgames++;
  t->sgoals += golesAFavor;
  t->agoals += golesEnContra;
  t->dgoals += golesAFavor - golesEnContra;
  if ( golesAFavor > golesEnContra ) {
    t->wgames++;
    t->points += config.wpoints;
  } else if ( golesAFavor == golesEnContra ) {
    t->dgames++;
    t->points += config.dpoints;
  } else {
    t->lgames++;
    t->points += config.lpoints;
  }

}

vector<Team> construirTabla(vector<Game> partidos, ConfigLiga config) {
  vector<Team> equipos;
  for (int i = 0; i < config.teams.size(); i++) {
    Team t;
    t.name = config.teams[i];
    t.tgames = 0;
    t.wgames = 0;
    t.dgames = 0;
    t.lgames = 0;
    t.sgoals = 0;
    t.agoals = 0;
    t.dgoals = 0;
    t.points = 0;
    equipos.push_back(t);
  }
  for (int i = 0; i < partidos.size(); i++){
    //buscar equipo local en equipos
    for (int j = 0; j < equipos.size(); j++){
      if (equipos[j].name == partidos[i].local){
        actualizarEstadisticas(&equipos[j], partidos[i].lgoals, partidos[i].vgoals, config);
      }
    //buscar equipo visitante en equipos
      if (equipos[j].name == partidos[i].visiting) {
        actualizarEstadisticas(&equipos[j], partidos[i].vgoals, partidos[i].lgoals, config);
        }
    }
  }
  return equipos;
}
bool compararEquipos(Team a, Team b){
  if ( a.points != b.points ){
    return a.points > b.points;
  } else if( a.dgoals != b.dgoals ){
    return a.dgoals > b.dgoals;
  } else {
    return a.sgoals > b.sgoals;
  }
}
void ordenarTabla(vector<Team>& equipos) {
  sort(equipos.begin(), equipos.end(), compararEquipos);
}

// Muestra la tabla de posiciones en consola con formato de columnas
void mostrarTabla(vector<Team> tabla) {
    // imprimir encabezado
    cout << left << setw(4) << "#"
         << setw(22) << "Equipo"
         << right << setw(4) << "PJ"
         << setw(4) << "PG"
         << setw(4) << "PE"
         << setw(4) << "PP"
         << setw(4) << "GF"
         << setw(4) << "GC"
         << setw(4) << "DG"
         << setw(5) << "PTS" << endl;

    // imprimir cada equipo
    for (int i = 0; i < tabla.size(); i++) {

      cout << left << setw(4) << i+1
           << setw(22) << tabla[i].name
           << right << setw(4) << tabla[i].tgames
           << setw(4) << tabla[i].wgames
           << setw(4) << tabla[i].dgames
           << setw(4) << tabla[i].lgames
           << setw(4) << tabla[i].sgoals
           << setw(4) << tabla[i].agoals
           << setw(4) << tabla[i].dgoals
           << setw(5) << tabla[i].points << endl;
    }
}

// Guarda la tabla de posiciones en tabla.txt
void exportarTabla(vector<Team> tabla) {

    ofstream archivo("data/tabla.txt");
    if (!archivo.is_open()) {
        cout << "Error: no se pudo crear tabla.txt" << endl;
        return;
    }
    
    archivo << left << setw(4) << "#"
         << setw(22) << "Equipo"
         << right << setw(4) << "PJ"
         << setw(4) << "PG"
         << setw(4) << "PE"
         << setw(4) << "PP"
         << setw(4) << "GF"
         << setw(4) << "GC"
         << setw(4) << "DG"
         << setw(5) << "PTS" << endl;

    // imprimir cada equipo
    for (int i = 0; i < tabla.size(); i++) {

      archivo << left << setw(4) << i+1
           << setw(22) << tabla[i].name
           << right << setw(4) << tabla[i].tgames
           << setw(4) << tabla[i].wgames
           << setw(4) << tabla[i].dgames
           << setw(4) << tabla[i].lgames
           << setw(4) << tabla[i].sgoals
           << setw(4) << tabla[i].agoals
           << setw(4) << tabla[i].dgoals
           << setw(5) << tabla[i].points << endl;
    }
    archivo.close();
}

int main() {
    ConfigLiga config = leerConfig("data/config.txt");
    vector<Game> partidos = leerPartidos();
    vector<Team> tabla = construirTabla(partidos, config);
    ordenarTabla(tabla);
    mostrarTabla(tabla);
    exportarTabla(tabla);
    return 0;
}
