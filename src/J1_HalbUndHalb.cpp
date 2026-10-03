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
#define ERR_NOSOLUTION 3;

//Punkt relativ zur Funktion, bzw. dem Schnitt der Pizza
enum point_relative{
    UNDER_CUT, //Punkt ist unter dem Schnitt
    OVER_CUT, //Punkt ist über dem Schnitt
    ON_CUT //Punkt ist auf dem Schnitt
};

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

double rad_to_deg(double rad) {
    return rad * (3.1415926/180.0);
}

// Returns value for tan(deg°)*x = y
double eval_y_linear_func(double x, int deg) {
    return tan(rad_to_deg(deg)) * x;
}

//Gibt eine point_relative variable zurück, basiert wo sich der Punkt befindet und was eval_y_linear_func zurück gibt
point_relative get_relative_position_over_cut(point pnt, int deg) {
    double point_y = pnt.y;
    double func_y = eval_y_linear_func(pnt.x, deg);
    
    if (point_y > func_y) {
        return point_relative::OVER_CUT;
    } else if (point_y < func_y) {
        return point_relative::UNDER_CUT;
    } else {
        return point_relative::ON_CUT;
    }
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
    pizza.amount_belag1 = stoi(lines_split[1][0]);
    pizza.amount_belag2 = stoi(lines_split[2+pizza.amount_belag1][0]);
    int belag1_line = 2; //Zeile der Punkte vom ersten Belag
    int belag2_line = 3 + pizza.amount_belag1; //Zeile der Punkte vom zweiten Belag
    
    //Belag 1
    for (int i = 0; i < pizza.amount_belag1; i++) {
        int line = belag1_line + i; //"n" in dem Material
        int v_size = lines_split[line].size(); //Größe des Vectors der Zeile
        
        for (int j = 1; j < v_size; j += 2) {
            string str_x = lines_split[line][j];
            string str_y = lines_split[line][j+1];
            
            pizza.points_belag1.insert(pizza.points_belag1.end(), string_to_point(str_x, str_y));
        }
    }
    
    //Belag 2 (duplizierter Code, aber funktioniert gut genug)
    for (int i = 0; i < pizza.amount_belag2; i++) {
        int line = belag2_line + i; //"n" in dem Material
        int v_size = lines_split[line].size(); //Größe des Vectors der Zeile
        
        for (int j = 1; j < v_size; j += 2) {
            string str_x = lines_split[line][j];
            string str_y = lines_split[line][j+1];
            
            pizza.points_belag2.insert(pizza.points_belag2.end(), string_to_point(str_x, str_y));
        }
    }
    
    //Teste ob Punkte über/unter/auf dem Schnitt sind, für 0° bis 180°
    bool is_possible = false; //Ist es möglich, mit einem Schnitt beide Beläge aufzuteilen
    int possible_deg = 0;
    for (int this_deg = 0; this_deg <= 180; this_deg++) { //this_deg ist die Rotation des Schnittes
        bool skip_iteration = false;
        point_relative first_belag1_relative_pos; //Erster point_relative vom ersten Belag zum Vergleichen
        point_relative first_belag2_relative_pos; //Erster point_relative vom zweiten Belag zum Vergleichen
        
        //Wo sind Punkte von Belag 1 relativ zu der Funktion
        for (int i = 0; i < pizza.points_belag1.size(); i++) { //Loopt über jeden Punkt mit dem Index i
            point this_point = pizza.points_belag1[i]; //Punkt dieser Interation
            point_relative this_point_relative = get_relative_position_over_cut(this_point, this_deg);
            
            if (i == 0) first_belag1_relative_pos = this_point_relative;
            
            //Ist dieser Punkt anders zu dem ersten. Wenn ja, überspringe diese Iteration sofort, um Zeit zu sparen.
            if (this_point_relative != first_belag1_relative_pos) {
                cout << "deg: " << this_deg << "; Relativer Punkt anders. Überspringe Iteration..." << endl;
                i = pizza.points_belag1.size() + 1; //i wird hoch gesetzt, damit diese Iteration sofort übersprungen wird.
                skip_iteration = true;
            }
        }
        
        //Überspringe Iteration?
        if (skip_iteration) continue;
        
        //Wo sind Punkte von Belag 2 relativ zu der Funktion
        for (int i = 0; i < pizza.points_belag2.size(); i++) { //Loopt über jeden Punkt mit dem Index i
            point this_point = pizza.points_belag2[i]; //Punkt dieser Interation
            point_relative this_point_relative = get_relative_position_over_cut(this_point, this_deg);
            
            if (i == 0) first_belag2_relative_pos = this_point_relative;
            
            //Ist dieser Punkt anders zu dem ersten. Wenn ja, überspringe diese Iteration sofort, um Zeit zu sparen.
            if (this_point_relative != first_belag2_relative_pos) {
                cout << "deg: " << this_deg << "; Relativer Punkt anders. Überspringe Iteration..." << endl;
                i = pizza.points_belag2.size() + 1; //i wird hoch gesetzt, damit diese Iteration sofort übersprungen wird.
                skip_iteration = true;
            }
        }
        
        //Überspringe Iteration?
        if (skip_iteration) continue;
            
        //point_relative von beiden Belägen dürfen nicht gleich sein.
        if (first_belag1_relative_pos == first_belag2_relative_pos) {
            cout << "deg: " << this_deg << "; Relative Belag Position gleich. Überspringe Iteration..." << endl;
            continue;
        }
        
        cout << "deg: " << this_deg << "; Iteration erfolgreich. Schnitt ist möglich!" << endl;
        is_possible = true;
        possible_deg = this_deg;
        break;
    }
    
    cout << newl << newl;
    
    //Gebe Lösung aus.
    if (is_possible) { //Es gibt eine Lösung
        cout << bcolor.green << "Lösung gefunden bei " << possible_deg << "°!" << newl;
    } else {//Es gibt keine Lösung
        cout << bcolor.red << "[3] Keine Lösung gefunden!" << newl;
        return ERR_NOSOLUTION;
    }
    
    return 0;
}