#include <bits/stdc++.h>

#include <iostream>
#include <fstream>
#include "bColor.hpp"
#include <vector>
#include <string>

using namespace std;

struct point{ //2D Punkt
    double x;
    double y;
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

bColor bcolor = bColor(); // ANSI Codes für Farbigen Text
string newl = bcolor.reset + "\n"; // newl "new line" setzt Farbe zurück und erstellt eine neue Zeile

vector<string> split_string_by_space(string str_in) {
    stringstream str_in_stream(str_in);
    string word;
    
    vector<string> out_vector;
    
    while (str_in_stream >> word) {
        out_vector.insert(out_vector.end(), word);
    }
    
    return out_vector;
}

//Nimmt zwei Strings und convertiert diese zu einem point
point string_to_point(string x, string y) {
    point out;
    out.x = stod(x);
    out.y = stod(y);
    
    return out;
}

int main(int argc, char* argv[]) {
    
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
    
    //Splittet für jedes Leerzeichen die Zeilen auf
    vector<vector<string>> lines_split;
    for (auto &i: lines) {
        lines_split.insert(lines_split.end(), split_string_by_space(i));
    }
    lines = {}; //Clear
    
    //Pizza Struct
    pizza pizza;
    pizza.radius = stoi(lines_split[0][0]);
    cout << "Pizza radius: " << pizza.radius << newl;
    pizza.amount_belag1 = stoi(lines_split[1][0]);
    
    //Belag 1
    for (int i = 0; i < pizza.amount_belag1; i++) {
        int line = 2 + i; //"n" in dem Material
        int v_size = lines_split[line].size(); //Größe des Vectors der Zeile
        
        for (int j = 1; j < v_size; j += 2) {
            string str_x = lines_split[line][j];
            string str_y = lines_split[line][j+1];
            
            pizza.points_belag1.insert(pizza.points_belag1.end(), string_to_point(str_x, str_y));
        }
    }
    
    //debug
    for (auto &i: pizza.points_belag1) {
        cout << "(" << i.x << ", " <<i.y << ")" << newl;
    }
    
    
    return 0;
};