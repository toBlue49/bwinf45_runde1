#include <iostream>
#include <fstream>
#include "bColor.hpp"
#include <vector>

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

int main(int argc, char* argv[]) {
    bColor bcolor = bColor(); // ANSI Codes für Farbigen Text
    string newl = bcolor.reset + "\n"; // newl "new line" setzt Farbe zurück und erstellt eine neue Zeile
    
    //Hat falsche Anzahl an Argumenten
    if (argc != 2) {
        cout << bcolor.red << "[1] Falsche Anzahl an Argumenten.\n Syntax: [PROGRAM] [DATEIPFAD]" << newl;
        return ERR_INVALIDARGS;
    }
    
    string filepath = argv[1] + newl; // Lese filepath von argv
    cout << "Dateipfad: " << bcolor.cyan << filepath << newl; //Print
    
    // Speichert den Dateiinhalt in ein Array; Ein Element pro Zeile.
    ifstream File(filepath);
    
    while (getline(MyReadFile, myText)) {
        // Output the text from the file
        cout << myText;
    }
    
    return 0;
}