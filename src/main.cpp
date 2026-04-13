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
    vector<string> teams; //crea un vector en el que se guardan los nombres de los equipos
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
        ConfigLiga config; // Crea la variable (copia) config de tipo ConfigLiga(struct) en donde guardar las configuraciones para utilizar posteriormente
        string linea;
        while (getline(archivo, linea)){
            if (linea[0] == '#') continue;
            if (linea.empty()) continue;
            stringstream ss(linea);
            string clave, valor; //crea las dos variables necesarias para parsear y dar valor (llenar el structure config)
            getline(ss, clave, '='); //lo que este antes del = va a ser la clave
            getline(ss, valor); // lo que este despues va a ser el valor
            //asignacion del valor dependiendo de la clave
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
        archivo.close(); //despues de abrir un archivo muy importante cerrarlo
        return config; //devolvemos la copia de tipo ConfigLiga que creamos al inicio para guardar las claves y luego retornarlo con los datos leidos del config.txt
    } else {
        cout << "Error: no se pudo abrir " << ruta << endl;
        exit(1);
    }
}

bool partidoDuplicado(Game nuevo, vector<Game> partidos) { //Funcion de tipo verdadero/falso que verifica si un partido nuevo (Game nuevo)  ya esta registrado a partir del vector de partidos (todos los partidos) que se le pase (vector<Game> partidos)
    for (int i = 0; i < partidos.size(); i++) {
        if (partidos[i].local == nuevo.local &&  
            partidos[i].visiting == nuevo.visiting &&
            partidos[i].date == nuevo.date  
            ) {
            return true; // si la condicion se cumple la funcion retorna verdadero
        }
    }
    return false; // si no se cumple retorna falso
}

void guardarPartido(Game g, vector<Game> partidos ){ //Funcion que no espera ningun valor sino que ejecuta la accion de guardar un partido que se le pase(g) y guardarlo en data/partidos.txt y tambien verifica si ese partido ya esta registrado gracias a la funcion partidoDuplicado, por eso se le pasa el vector de todos los partidos como parametro
  if (partidoDuplicado(g, partidos)){
  cout << "Error: ese partido ya fue registrado" << endl;
  return;
  }
  ofstream archivo("data/partidos.txt", ios::app); //abre un archivo con el tipo output(escritura) y con la caracteristica app que permite dejar el contenido que tenia y escribir al final en vez de sobre escribir
  if (archivo.is_open()){ // verifica que se haya abierto el archivo y procede a guardar los datos del partido nuevo que se le paso en los parametros al archivo .txt
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

vector<Game> leerPartidos() { //Es una funcion que espera(retorna) un vector formado por structures de tipo Game que definimos al inicio del codigo (lee todos los partidos y los devuelve en un vector)
    vector<Game> partidos; //crea la variable (copia) partidos de tipo vector formado por structure Game, esta copia es importante ya que es la estructura final que espera la funcion que creamos y asi podemos retornar facilmente la lectura de todos los partidos
    ifstream archivo("data/partidos.txt");
    if (!archivo.is_open()) return partidos; // si no existe, retorna vacío
    string linea; //definimos la variable en la que vamos a guardar el valor de cada linea que vayamos leyendo del archivo partido.txt
    while (getline(archivo, linea)) {
    if (linea.empty()) continue;
    stringstream ss(linea); //toma el string (linea) y lo trata como con stream que puede leer
    string date, local, visiting, lgoals, vgoals; //crea los valores clave para parsear y luego asignarlos a las propiedades del structure individual (partido)
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
    partidos.push_back(g); //envia un nuevo partido al final del vector partidos
    
    }
    archivo.close();
    return partidos;
}
// Actualiza las estadísticas de un equipo según el resultado de un partido
void actualizarEstadisticas(Team* t, int golesAFavor, int golesEnContra, ConfigLiga config){ //define la variable t como un puntero, es decir espera una direccion, luego define los demas valores necesarios para actualizar las estadisticas teniendo en cuenta la configuracion previamente echa. esta funcion no espera nada como retorno
  t->tgames++; // -> es el operador que se usa para acceder a las propiedades de un puntero - aumenta la cantidad de partidos totales
  t->sgoals += golesAFavor; // aumenta los goales a favor en caso de que hayan
  t->agoals += golesEnContra; // lo mismo pero con los goles en contra
  t->dgoals += golesAFavor - golesEnContra; //diferencia de goles
  if ( golesAFavor > golesEnContra ) { //Definir si aumentar lo partidos ganados dependiendo de los goles en contra y a favor
    t->wgames++;
    t->points += config.wpoints;
  } else if ( golesAFavor == golesEnContra ) { //Definir si aumentar los partidos empatados
    t->dgames++;
    t->points += config.dpoints;
  } else { //Aumenta los partidos perdidos como ultima instancia
    t->lgames++;
    t->points += config.lpoints;
  }

}

vector<Team> construirTabla(vector<Game> partidos, ConfigLiga config) { //Funcion que permite crear la tabla de los equipos
  vector<Team> equipos; //defina la variable que vamos a retornar al final de la funcion
  for (int i = 0; i < config.teams.size(); i++) { //iteramos sobre la cantidad de equipos que se hayan definido en la configuracion y le damos el valor inicial de 0 en todas las propiedades de cada equipo por separado
    Team t; //creamos la variable t(objeto) que es la estructura de cada equipo para modificar sus propiedades 
    t.name = config.teams[i];
    t.tgames = 0;
    t.wgames = 0;
    t.dgames = 0;
    t.lgames = 0;
    t.sgoals = 0;
    t.agoals = 0;
    t.dgoals = 0;
    t.points = 0;
    equipos.push_back(t); //enviamos al final del vector equipos cada equipo correspondiente
  }
  //Actualizamos los datos de cada equipo a partir de los partidos que tenemos y dependiendo si es local o no se le pasan los los goles a favor o en contra respectivamente
  for (int i = 0; i < partidos.size(); i++){
    //Verificar dentro del partido si el equipo es local para pasarle correctamente los parametro de los goles que anoto a favor y los goles en contra
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
  return equipos; //Devuelve la tabla de equipos actualizada
}
bool compararEquipos(Team a, Team b){ //Es la regla que vamos a utilizar (sort()) para ordenar la tabla de mayor a menor dependiendo de sus puntos en caso de que no sean iguales o Diferencia de goles o goles a favor en ese orden
  if ( a.points != b.points ){
    return a.points > b.points;
  } else if( a.dgoals != b.dgoals ){
    return a.dgoals > b.dgoals;
  } else {
    return a.sgoals > b.sgoals;
  }
}
void ordenarTabla(vector<Team>& equipos) { //Es la funcion que hace uso de sort() teniendo en cuenta la funcion compararEquipos 
  sort(equipos.begin(), equipos.end(), compararEquipos); //Le decimos que empieze desde el inicio del vector hasta el final y haga uso de la funcion compararEquipos para ordenar
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
void exportarTabla(vector<Team> tabla) {//Esta funcion no espara nada como retorno

    ofstream archivo("data/tabla.txt"); //Abre el archivo en modo escritura
    if (!archivo.is_open()) {
        cout << "Error: no se pudo crear tabla.txt" << endl;
        return;
    }
    //Vuelve a repetir lo mismo que en la funcion mostrarTabla solo que escribe en un archivo y no en consola
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

// Guarda los partidos de una jornada en fechas.txt en modo append
void guardarJornada(int numero, vector<Game> partidos) {
    ofstream archivo("data/fechas.txt", ios::app); //Abre el archivo fechas.txt con el modo especial app
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir fechas.txt" << endl;
        return;
    }
    //Escribe la estructura de la jornada junto con los partidos correspondientes a esa jornada
    archivo << "JORNADA=" << numero << "\n";
    for (int i = 0; i < partidos.size(); i++) {
      archivo << partidos[i].date << "|";
      archivo << partidos[i].local << "|";
      archivo << partidos[i].visiting << "|";
      archivo << partidos[i].lgoals << "|" ;
      archivo << partidos[i].vgoals << "\n";
    }
    archivo << "FIN_JORNADA\n";
    archivo.close();
}

// Lee fechas.txt y muestra todas las jornadas con sus partidos
void mostrarJornadas() {
    ifstream archivo("data/fechas.txt");
    if (!archivo.is_open()) {
        cout << "No hay jornadas registradas." << endl;
        return;
    }
    string linea;
    while (getline(archivo, linea)) {
        // si la línea empieza con "JORNADA=" → imprimir encabezado
        if ( linea.substr(0, 8) == "JORNADA="){
          cout << "--" << " Jornada " << linea.substr(8) << " --" << endl;
        } else if (linea == "FIN_JORNADA") {
          cout << endl;
        } else {
          if (linea.empty()) continue;
          stringstream ss(linea);
          string date, local, visiting, lgoals, vgoals;
          getline(ss, date, '|');
          getline(ss, local, '|');
          getline(ss, visiting, '|');
          getline(ss, lgoals, '|');
          getline(ss, vgoals);
          cout << local << " " << lgoals << " - " << vgoals << " " << visiting << endl;

        }
    }
    archivo.close();
}

// Muestra todos los partidos entre dos equipos seleccionados, esta funcion no espera nada como retorno y los parametros son los dos equipos mas el total de partidos
void historialEnfrentamientos(string equipo1, string equipo2, vector<Game> partidos) {
    cout << "-- Enfrentamientos: " << equipo1 << " vs " << equipo2 << " --" << endl;
    bool encontrado = false;
    //recorre el vector de los partidos para encontrar los partidos en los que se enfrentan los dos equipos
    for (int i = 0; i < partidos.size(); i++) {
      //Verfica que si coincidan los dos equipos ya sea como local - visitante o al reves
      if ((partidos[i].local == equipo1 && partidos[i].visiting == equipo2) ||
          (partidos[i].local == equipo2 && partidos[i].visiting == equipo1)) {
        //imprime en consola los partidos en los que si coinciden los dos equipos
              cout << partidos[i].date << " | " << partidos[i].local 
                   << " " << partidos[i].lgoals << " - " 
                   << partidos[i].vgoals << " " << partidos[i].visiting << endl;
       encontrado = true;
      }

    }
    if (!encontrado) cout << "No hay enfrentamientos registrados." << endl;
}
// Muestra el menu y retorna la opcion elegida
int menu(string liga) {
    int opcion;
    cout << "\n=== " << liga << " ===" << endl;
    cout << "1. Ver tabla de posiciones" << endl;
    cout << "2. Registrar resultado de un partido" << endl;
    cout << "3. Ver historial de jornadas" << endl;
    cout << "4. Ver todos los partidos jugados" << endl;
    cout << "5. Exit" << endl;
    cout << "6. Editar resultado de un partido" << endl; 
    cout << "Elige una opcion: ";
    cin >> opcion;
    return opcion;
}

// Guía al usuario para ingresar un nuevo partido con validación
void ingresarPartido(ConfigLiga config, vector<Game>& partidos) { 
    for (int i = 0; i < config.teams.size(); i++){ //imprime en una lista enumerada los equipos que se definieron en config
      cout << i+1 << " " << config.teams[i] << endl;
    }
    int nl = 0; // numero del equipo local
    int nv = 0; // numero del equipo visitante
    //ejecuta al menos una vez el menu que le pregunta por el equipo local y visitante hasta que sean validas las entradas
    do {
      cout << "Numero de equipo local: ";
      cin >> nl;
      cout << "Numero de equipo visitante: ";
      cin >> nv;
      //verifica que el valor de entrada sea valido
      if (nl < 1 || nl > config.teams.size() || 
      nv < 1 || nv > config.teams.size()) {
      cout << "Número de equipo inválido." << endl;
      nl = nv = 0;
      continue;
      }
      if ( nl == nv ) {
        cout << "Equipos iguales intente de nuevo" << endl;
      }
    }
    while ( nl == nv || nl == 0 ); //no para hasta que se intruduzcan valores validos
    int gl = 0; //se definen los goles del quipo local
    int gv = 0; // se definen los goles del equipo visitante
    
    string date;
    cout << "Goles del " << config.teams[nl-1] << ": ";
    cin >> gl;
    cout << "Goles del " << config.teams[nv-1] << ": " ;
    cin >> gv;
    cout << "Fecha: ";
    cin >> date;
    //apartir de los datos que puso el usuario se crea un nuevo partido y se envia al final del vector de partidos
    Game g;
    g.date = date;
    g.local = config.teams[nl-1];
    g.visiting = config.teams[nv-1];
    g.lgoals = gl;
    g.vgoals = gv;
    guardarPartido(g, partidos); //guarda el partido en partidos.txt para generar persistencia
    partidos.push_back(g);

}

// Reescribe partidos.txt completo con el vector actualizado
void reescribirPartidos(vector<Game> partidos) {
    ofstream archivo("data/partidos.txt");  // sin ios::app → sobreescribe
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir partidos.txt" << endl;
        return;
    }
    for (int i = 0; i < partidos.size(); i++) {
        archivo << partidos[i].date << "|";
        archivo << partidos[i].local << "|";
        archivo << partidos[i].visiting << "|";
        archivo << partidos[i].lgoals << "|";
        archivo << partidos[i].vgoals << "\n";
    }
    archivo.close();
}
// Permite corregir el marcador de un partido ya registrado
void editarResultado(vector<Game>& partidos) {
    int num = 0;
    // 1. mostrar todos los partidos numerados
    for ( int i = 0; i < partidos.size(); i++ ){
      cout << i+1 << ". " << partidos[i].date << " | " << partidos[i].local << " | " << partidos[i].visiting << " | " << partidos[i].lgoals << " | " << partidos[i].vgoals << endl;
    }
    // 2. pedir al usuario que elija un número
    do {
    cout << "Elije el numero: ";
    cin >> num;
    if ( num < 1 || num > partidos.size() ){
      cout << "Valor invalido" << endl;
    }
    } while( num < 1 || num > partidos.size());
    int gl = 0;
    int gv = 0;
    cout << "Goles equipo local: ";
    cin >> gl;
    cout << "Goles equipo visitante";
    cin >> gv;
    partidos[num-1].lgoals = gl;
    partidos[num-1].vgoals = gv;
    reescribirPartidos(partidos);
    cout << "Partidos modificados" << endl;
}
void run(){ //funcion inicial que llama main
    ConfigLiga config = leerConfig("data/config.txt"); //definimos config como la variable que guarda lo que nos retorna leerConfig()
    vector<Game> partidos = leerPartidos(); //definimos partidos como el vector de salida que retorna leerPartidos()
    int opcion;
    do {
        opcion = menu(config.name); //guarda la opcion que el usuario pone cuando el menu se ejecuta, el menu retorna un entero(opcion que elijio)
        if (opcion == 1) {
            vector<Team> tabla = construirTabla(partidos, config);
            ordenarTabla(tabla);
            mostrarTabla(tabla);
        } else if (opcion == 2) {
            ingresarPartido(config, partidos);
        } else if (opcion == 3) {
            mostrarJornadas();
        } else if (opcion == 4) {
            vector<Game> todosLosPartidos = leerPartidos();
            for (int i = 0; i < todosLosPartidos.size(); i++) {
            cout << todosLosPartidos[i].date << " | "
                 << todosLosPartidos[i].local << " "
                 << todosLosPartidos[i].lgoals << " - "
                 << todosLosPartidos[i].vgoals << " "
                 << todosLosPartidos[i].visiting << endl;
    }
        } else if (opcion == 5) {
            cout << "Exit" << endl;
        } else if (opcion == 6) {
            editarResultado(partidos);
        } else {
            cout << "Opción inválida." << endl;
        }
    } while (opcion != 5);
}


int main() {
    run();
    return 0;
}
