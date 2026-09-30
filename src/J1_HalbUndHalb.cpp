#include <iostream>
#include <fstream>
#include "bColor.hpp"
#include <vector>
#include <string>

using namespace std;

struct point{ //2D Punkt
    int x;
    int y;
};

struct pizza{
    int radius;
    int amount_belag1; // Anzahl der Zeilen vom ersten Belag
    int amount_belag2; // Anzahl der Zeilen vom zweiten Belag
    vector<point> points_belag1; // Vector mit Punkten des ersten Belags
    vector<point> points_belag2; // Vector mit Punkten des zweiten Belags
};

#define ERR_INVALIDARGS 1;
#define ERR_COULDNTOPENFILE 2;

int main(int argc, char* argv[]) {
    bColor bcolor = bColor(); // ANSI Codes für Farbigen Text
    string newl = bcolor.reset + "\n"; // newl "new line" setzt Farbe zurück und erstellt eine neue Zeile
    
    //Hat falsche Anzahl an Argumenten
    if (argc != 2) {
        cout << bcolor.red << "[1] Falsche Anzahl an Argumenten.\n Syntax: [PROGRAM] [DATEIPFAD]" << newl;
        return ERR_INVALIDARGS;
    }
    
    string filepath = argv[1]; // Lese filepath von argv
    cout << "Dateipfad: " << bcolor.cyan << filepath << newl; //Print
    
    // Lese Datei ein.
    ifstream file(filepath);
    
    //Konnte Datei nicht öffnen
    if (!file.is_open()) {
        cout << bcolor.red << "[2] Datei konnte nicht geöffnet werden." << newl;
        return ERR_COULDNTOPENFILE;
    }
    
    // Speichert den Dateiinhalt in ein Vector; Ein Element pro Zeile.
    string this_line;
    vector<string> lines; //Linen speichernder Vector
    while (getline(file, this_line)) {
        lines.insert(lines.end(), this_line);
    }
    this_line = ""; //Clear
    
    //Loop over vector
    
    return 0;
}