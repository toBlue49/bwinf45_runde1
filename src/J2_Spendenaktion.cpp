#include "bColor.hpp"
#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <vector>
#include <array>

using namespace std;

#define ERR_INVALIDARGS 1;
#define ERR_COULDNTOPENFILE 2;

bColor bcolor = bColor(); // ANSI Codes für Farbigen Text
string newl = bcolor.reset + "\n"; // newl "new line" setzt Farbe zurück und erstellt eine neue Zeile

//Struct für ein Projekt
struct projekt {
    string name; //Name (A, B, C, etc.) des Projektes
    int original_stimmen; //Anzahl der Stimmen vor dem Tauschen
    int stimmen; //Anzahl der Stimmen
    int geld; //Wie viel Geld mit den Stimmen eingenommen wird
    int schenken_amount = 0; //Wie viel das Projekt schenken wird
    string schenken_name = ""; //Name des Projektes an das dieses Stimmen schenken will.
};

//Aus J1
vector<string> split_string_by_space(string str_in) {
    stringstream str_in_stream(str_in);
    string word;
    
    vector<string> out_vector;
    
    while (str_in_stream >> word) {
        out_vector.insert(out_vector.end(), word);
    }
    
    return out_vector;
}

//Gibt zurück wie oft num durch 2 ganz geteilt werden kann.
int get_divide_count(int num) {
    int div_counter = 0; //Zählt Teilungen
    while (num % 2 == 0) { //Teile current durch 2 und erhöhe div_counter, bis current mod 2 nicht mehr 0 ergibt.
        num /= 2;
        div_counter++;
    }
    
    return div_counter;
}

//Wendet schenken_amount auf das gegebene Projekt an. Setzt auch schenken_name zurück und gibt immer 0 zurück
int schenken_anwenden(projekt* besch_proj_ptr, projekt* schnk_proj_ptr) {
    //vom schenkenden Projekt Stimmen abziehen
    schnk_proj_ptr->stimmen -= schnk_proj_ptr->schenken_amount;
    
    //zum beschenkten Projekt Stimmen hinzufügen
    besch_proj_ptr->stimmen += schnk_proj_ptr->schenken_amount;
    
    std::cout << "| Schenkendes Projekt: " << schnk_proj_ptr->name << " schenken_amount: " << schnk_proj_ptr->schenken_amount << endl;
    
    //schenken_amount zurücksetzten
    schnk_proj_ptr->schenken_amount = 0;
    
    //schenken_name zurücksetzten
    schnk_proj_ptr->schenken_name = "";
    
    //Geld neu berechnen (ich hab hier 2 tage gehangen, bis mir eingefallen ist, dass ich das geld auch neuberechnen muss)
    besch_proj_ptr->geld = get_divide_count(besch_proj_ptr->stimmen) * 100;
    schnk_proj_ptr->geld = get_divide_count(schnk_proj_ptr->stimmen) * 100;
    return 0;
}

//Gibt true zurück wenn sich die Geldwerte von beiden Projekten erhöht hat mit den neuen Stimmen, verglichen mit den Alten.
//Gibt false zurück wenn dies nicht zutrifft (also kleiner oder gleich)
bool haben_mit_spenden_mehr_geld(projekt besch_proj, projekt schnk_proj, int besch_neue_stimmen, int schnk_neue_stimmen) {
    int besch_geld = get_divide_count(besch_neue_stimmen) * 100;
    int schnk_geld = get_divide_count(schnk_neue_stimmen) * 100;
    
    if (besch_geld <= besch_proj.geld) { //Beschenktes Projekt; Neuer Wert kleiner oder gleich.
        return false;
    }
    if (schnk_geld <= schnk_proj.geld) {  //Schenkendes Projekt; Neuer Wert kleiner oder gleich.
        return false;
    }
    
    //Sonst muss ja beides größer sein, als true
    return true;
}

int main(int argc, char* argv[]) {
    // Einlesen der Datei kopiert aus J1
    
    //Hat falsche Anzahl an Argumenten
    if (argc != 2) {
        std::cout << bcolor.red << "[1] Falsche Anzahl an Argumenten.\n Syntax: [PROGRAM] [DATEIPFAD]" << newl;
        return ERR_INVALIDARGS;
    }
    
    string filepath = argv[1]; // Lese filepath von argv
    std::cout << "Dateipfad: " << bcolor.cyan << filepath << newl; //Print
    
    // Lese Datei ein.
    ifstream file(filepath);
    
    //Konnte Datei nicht öffnen
    if (!file.is_open()) {
        std::cout << bcolor.red << "[2] Datei konnte nicht geöffnet werden." << newl;
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
    
    //Erstelle Projekt Array
    int projekt_amount = stoi(lines_split[0][0]); //Projekte Anzahl
    projekt projekt_array[projekt_amount]; //Array mit "Projekten"
    
    //Loop über alle Projekte
    for (int i = 0; i < projekt_amount; i++) {
        vector<string> this_line = lines_split[1+i]; //Zeile dieser Iteration
        
        projekt_array[i].name = this_line[0];
        projekt_array[i].stimmen = stoi(this_line[1]);
        
        //Setzte Geld
        projekt_array[i].geld = get_divide_count(projekt_array[i].stimmen) * 100; //Setzte Geld auf die Anzahl die geteilt werden konnte * 100
        std::cout << projekt_array[i].name << "; " << projekt_array[i].geld << newl;
    }
    
    int geld_alle_projekte = 0; //Geld aller Projekte
    int letzte_geld_alle_projekte = -1; //Geld aller Projekte der vorherigen Iteration
    bool break_this_iteration = false; //Soll diese Iteration übersprungen werden?
    for (int i = 0; i < (sizeof(projekt_array)/sizeof(*projekt_array)); i++) {
        geld_alle_projekte += projekt_array[i].geld;
        
        projekt_array[i].original_stimmen = projekt_array[i].stimmen; //Speichere Stimmen vor dem Schenken 
    }
    
    //Stimmen Algorithmus
    //For Loop verwenden Index, um auf die original Variable zugreifen zu können. Ich bin mir sicher, man kann das auch mit einem Pointer machen, ich bin mit denen aber noch zu unsicher.
    while (geld_alle_projekte > letzte_geld_alle_projekte) { //Wenn das Geld gleich ist, wie bei der vorherigen Iteration, dann sind wir fertig.
        for (int schenkendes_projekt_index = 0; schenkendes_projekt_index < (sizeof(projekt_array)/sizeof(*projekt_array)); schenkendes_projekt_index++) { //Für alle Projekte
            projekt schenkendes_projekt = projekt_array[schenkendes_projekt_index];
            
            //iterate_over (vector) soll der projekt_array (array) außer schenkendes_projekt sein
            vector<projekt> iterate_over;
            for (auto element: projekt_array) {
                if (element.name != schenkendes_projekt.name) { //Name kann auch verwendet werden da diese nicht gleich sind.
                    iterate_over.insert(iterate_over.end(), element);
                }
            }
            
            for (int beschenktes_projekt_index = 0; beschenktes_projekt_index < iterate_over.size(); beschenktes_projekt_index++) { //Für alle Projekte außer das eigene.
                projekt beschenktes_projekt = iterate_over[beschenktes_projekt_index];
                //beschenktes_projekt = Das Projekt dem hier gespendet wird
                
                
                //Es werden "s" Stimmen von "schenkendes_projekt" and "beschenktes_projekt" gespendet
                for (int s = 1; s < schenkendes_projekt.stimmen; s++) { //Für alle verfügbaren Stimmen von dem Projekt was spendet (schenkendes_projekt).
                    // std::cout << schenkendes_projekt.name << "->" << beschenktes_projekt.name << " ";
                    // std::cout << bcolor.yellow << to_string(s) << newl;
                    
                    int beschenktes_projekt_neue_stimmen = beschenktes_projekt.stimmen + s; //Beschenktes Projekt kriegt plus
                    int schenkendes_projekt_neue_stimmen = schenkendes_projekt.stimmen - s; //Schenkendes Projekt kriegt minus
                    
                    bool mehr_geld = haben_mit_spenden_mehr_geld(beschenktes_projekt, schenkendes_projekt,
                                                                beschenktes_projekt_neue_stimmen, schenkendes_projekt_neue_stimmen);
                    
                    if (mehr_geld) {
                        //Speichere in schenken_amount und schenken_name
                        projekt_array[schenkendes_projekt_index].schenken_amount = s;
                        projekt_array[schenkendes_projekt_index].schenken_name = beschenktes_projekt.name;
                        break_this_iteration = true;
                    }
                    
                }
                
                if (break_this_iteration) {
                    break;
                }
            }
            
            if (break_this_iteration) {
                break;
            }
        }
        
        //Scheinbar wird das schenken nicht richtig gesetzt. bestimmt weil ich einen pointer brauche. mal schauen
        //Anwenden des Schenkens erst nach allen Loops
        projekt* projekt_ptr;
        for (int _index = 0; _index < (sizeof(projekt_array)/sizeof(*projekt_array)); _index++) {
            std::cout << projekt_array[_index].schenken_name << endl;
            //Wende gespeicherte schenken_amount und schenken_name an.
            //Wahrscheinlich muss erst geschaut werden welcher Name (buchstabe) zu welchem index bzw. welchem Projekt eigentlich passt.
            if (projekt_array[_index].schenken_name != "") {
                for (int i = 0;  i < (sizeof(projekt_array)/sizeof(*projekt_array)); i++) {
                    if (projekt_array[i].name == projekt_array[_index].schenken_name) {
                        projekt_ptr = &projekt_array[i];
                    }
                }
                
                schenken_anwenden(projekt_ptr, &projekt_array[_index]);
            }
        }
        
        letzte_geld_alle_projekte = geld_alle_projekte;
        geld_alle_projekte = 0;
        for (auto i: projekt_array) {
            geld_alle_projekte += i.geld;
        }
        
        std::cout << "##### ITERATION RESULTS #####" << endl;
        std::cout << projekt_array[0].stimmen << endl;
        std::cout << projekt_array[1].stimmen << endl;
        std::cout << projekt_array[2].stimmen << endl;
        std::cout << "letzte geld " << letzte_geld_alle_projekte << endl;
        std::cout << "       geld " << geld_alle_projekte << endl;
        std::cout << "##### ITERATION RESULTS #####" << endl;
        
        break_this_iteration = false;
    }
    
    //Muss noch Datei ausgeben
}