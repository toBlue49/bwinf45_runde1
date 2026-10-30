#include "raylib.h"
#include <vector>
#include <format>
#include <bits/stdc++.h>
#include "bColor.hpp"

#define CELL_EMPTY 0
#define CELL_PLAYER1 1
#define CELL_PLAYER2 2
#define CELL_ZIELKUGEL 3
#define CELL_FIELD_SIZE 13

#define PLAYER_1 1
#define PLAYER_2 2

bColor bcolor = bColor(); // ANSI Codes für Farbigen Text
string newl = bcolor.reset + "\n"; // newl "new line" setzt Farbe zurück und erstellt eine neue Zeile

std::vector<std::vector<int>> cells = { //Alle Zellen in einem 13x13 Feld
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

//Gibt den Zellenwert von (x, y) zurück
int get_cell_state(int x, int y) {
    if (x >= 0 && x <= 12 && y >= 0 && y <= 12) {
        return cells[y][x];
    }
    
    return CELL_EMPTY;
}

//Setzt den Zellenwert von (x, y)
void set_cell_state(int x, int y, int value) {
    cells[y][x] = value;
}

void spawn_zielkugel() {
    int x = rand() % 13;
    int y = rand() % 13;
    
    std::cout << bcolor.cyan << "Zielkugel Pos: " << x << ", " << y << newl;
    
    set_cell_state(x, y, CELL_ZIELKUGEL);
}

//Gibt die Feld Koordinate der mouse_pos zurück
//Gibt (-1, -1) zurück, wenn du Maus außerhalb des Feldes ist
Vector2 mouse_to_cell_pos(Vector2 mouse_pos) {
    if (mouse_pos.x >= 10 && mouse_pos.y >= 10 && mouse_pos.x <= 660 && mouse_pos.y <= 660){
        Vector2 result = mouse_pos;
        result.x -= 10;
        result.y -= 10;
        result.x /= 50;
        result.y /= 50;
        result.x = floor(result.x);
        result.y = floor(result.y);
        
        return result;
    } else {
        Vector2 result = Vector2();
        result.x = -1;
        result.y = -1;
        return result;
    }
}

//Gibt ein bool zurück, ob ein Zug auf diesem Feld gemacht werden darf.
bool is_valid_cell(int x, int y, bool first_move) {
    if (get_cell_state(x, y) != CELL_EMPTY) {
        return false;
    }
    
    
    if (first_move) { //falls first_move
        if (x == 0 || x == 12 || y == 0 || y == 12) { //Falls Feld am Rand ist
            return true;
        } else { //Wenn nicht
            return false;
        }
    } else { //falls nicht first_move
        if (x == -1 || y == -1) { //Wenn x oder y -1 ist 
            return false;
        } else { //Sonst
            return true;
        }
    }
    
    //Falls aus irgendeinem Grund das oben nichts macht wird einfach false zurückgegeben
    return false;
}

void draw_highlight_cell_hover(int x, int y, int player, bool first_move) {
    if (is_valid_cell(x, y, first_move)) { //Ist ein valides Feld?
        Color col = BLACK; //Zellfarbe (BLACK als Fallback)
        if (player == PLAYER_1) {
            col = RED;
        }
        if (player == PLAYER_2) {
            col = GOLD;
        }
        
        DrawRectangle(10 + 50*x, 10 + 50*y, 50, 50, col);
    }
}

void draw_cell(int x, int y){
    // Feld
    Color col = {220, 220, 220, 255};
    if ((x + y*13) % 2 == 0) {
        col = {240, 240, 240, 255};
    }
    DrawRectangle(10 + 50*x, 10 + 50*y, 50, 50, col);
    
    // Kugel
    int this_cell = get_cell_state(x, y); //Diese Zelle
    if (this_cell == CELL_PLAYER1) {
        DrawCircle(60 + 50*x - 25, 60 + 50*y - 25, 20, MAROON);
    }
    if (this_cell == CELL_PLAYER2) {
        DrawCircle(60 + 50*x - 25, 60 + 50*y - 25, 20, ORANGE);
    }
    if (this_cell == CELL_ZIELKUGEL) {
        DrawCircle(60 + 50*x - 25, 60 + 50*y - 25, 20, BLUE);
    }
}

int main(void)
{
    // Init
    srand(time(0)); //Set Seed von rand() auf die Systemzeit
    const int screenWidth = 670; //Screen Breite
    const int screenHeight = 700; //Screen Höhe
    
    int active_player = PLAYER_1; //Spieler, der gerade dran ist
    bool first_move = true; //Ist das der erste Zug. (Dann erlaube nur Zügen am Rand)
    
    spawn_zielkugel();
    
    InitWindow(screenWidth, screenHeight, "BWINF A4");
    SetTargetFPS(100);
    
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update //
        Vector2 mouse_pos = GetMousePosition(); //Maus Position
        Vector2 cell_m_pos = mouse_to_cell_pos(mouse_pos); //"Cell Mouse Position" Zellen Position
        int clicked_cell_state = -1; //Minus eins, falls kein Click
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            clicked_cell_state = get_cell_state(cell_m_pos.x, cell_m_pos.y);
        }
        
        // Draw //
        BeginDrawing();

            ClearBackground(DARKGRAY);
            
            DrawText(TextFormat("Mouse: %.02f, %.02f", cell_m_pos.x, cell_m_pos.y), 10, 670, 20, RAYWHITE);
            
            if (clicked_cell_state != -1 && is_valid_cell(cell_m_pos.x, cell_m_pos.y, first_move)){
                cout << "clicked_cell_state: " << clicked_cell_state << newl;
                set_cell_state(cell_m_pos.x, cell_m_pos.y, active_player);
                
                if (active_player == PLAYER_1) { //Switch active player.
                    active_player = PLAYER_2;
                } else {
                    active_player = PLAYER_1;
                    first_move = false; //Erst wenn der erste Spieler zum zweiten mal dran ist.
                }
            }
            
            
            if (active_player == PLAYER_1) {
                DrawText("Spieler 1", 280, 670, 20, RED);
            } else if (active_player == PLAYER_2) {
                DrawText("Spieler 2", 280, 670, 20, ORANGE);
            }
            
            for (int x = 0; x < CELL_FIELD_SIZE; x++) {
                for (int y = 0; y < CELL_FIELD_SIZE; y++) { //Für alle Zellen im Feld
                    draw_cell(x, y);
                }
            }
            
            draw_highlight_cell_hover(cell_m_pos.x, cell_m_pos.y, active_player, first_move);
            
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); //Schließen

    return 0;
}