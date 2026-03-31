#include <iostream>
#include <string>
#include <vector>
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
  int dgames;
  int wgames;
  int lgames;
  int sgoals;
  int agoals;
  int dgoals;
  int points;
};

int main() {
    cout << "Liga de Futbol - en construccion" << endl;
    return 0;
}
