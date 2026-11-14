#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <format>
#include <bits/stdc++.h>
#include "bColor.hpp"
#include "SimpleIni.h" //Siehe [5] (Verwendete Resourcen.txt)

#define CELL_EMPTY 0
#define CELL_PLAYER1 1
#define CELL_PLAYER2 2
#define CELL_ZIELKUGEL 3
#define CELL_FIELD_SIZE 13

#define PLAYER_1 1
#define PLAYER_2 2
#define PLAYER_NONE 3

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

bColor bcolor = bColor(); // ANSI Codes für Farbigen Text
string newl = bcolor.reset + "\n"; // newl "new line" setzt Farbe zurück und erstellt eine neue Zeile
CSimpleIniA ini;

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

//Gibt einen Vector2 mit den Koordinaten der Zielkugel wieder.
//Gibt (-1, -1) zurück, falls die Zielkugel nicht gefunden werden konnte
Vector2 get_zielkugel_position() {
    Vector2 result;
    result.x = -1;
    result.y = -1;
    
    for (int cell_y = 0; cell_y < CELL_FIELD_SIZE; cell_y++) {
        for (int cell_x = 0; cell_x < CELL_FIELD_SIZE; cell_x++) {
            if (get_cell_state(cell_x, cell_y) == CELL_ZIELKUGEL) { //Hier ist die Zielkugel
                result.x = cell_x;
                result.y = cell_y;
                return result; //Position zurückgeben
            }
        }
    }
    
    return result; //Sonst default (0, 0) zurückgeben
}

//Gibt die Richtung der Zielkugel von (x, y) als UP, RIGHT, DOWN oder LEFT zurück
//UP und DOWN werden vor LEFT und RIGHT getestet, bedeutet diese haben Vorrang
//Falls nichts zutrifft wird UP zurückgegeben
int get_zielkugel_direction(int x, int y) {
    Vector2 diff_pos = Vector2Subtract((Vector2){(float)x ,(float)y}, (Vector2)get_zielkugel_position()); //Differenz der Positionen
    
    if (abs(diff_pos.y) >= abs(diff_pos.x)) { //Die vertikale Differenz ist größer
        if (diff_pos.y > 0) {
            return DOWN;
        } else if (diff_pos.y < 0) { 
            return UP;
        }
    } else { //Die horizontale Differenz ist größer
        if (diff_pos.x > 0) {
            return RIGHT;
        } else if (diff_pos.x < 0) { 
            return LEFT;
        }
    }
    
    //Fallback
    cout << "Zielkugel Direction Fallback" << newl;
    return UP;
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

//Gibt ein bool zurück, ob ein Zug auf diesem Feld gemacht werden darf. (nicht zu verwechseln mit is_on_field)
bool is_valid_cell(int x, int y, bool first_move) {
    if (get_cell_state(x, y) != CELL_EMPTY) {
        return false;
    }
    
    if (first_move) { //falls first_move
        if (x == 0 || x == CELL_FIELD_SIZE-1 || y == 0 || y == CELL_FIELD_SIZE-1) { //Falls Feld am Rand ist
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

//Gibt ein bool zurück, ob die Koordinate im Feld ist, oder nicht. (nicht zu verwechseln mit is_valid_cell)
bool is_on_field(int x, int y) {
    if (x >= 0 && x <= CELL_FIELD_SIZE-1 && y >= 0 && y <= CELL_FIELD_SIZE-1) { //Falls Feld am Rand ist
        return true;
    } else { //Wenn nicht
        return false;
    }
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

//Gibt die Position der Zelle zurück, wo die Kugel laden soll, nach dem zufälligen Offset
Vector2 get_rand_cell_position(int x, int y, int direction) {
    Vector2 this_pos = {(float)x, (float)y};
    int offset_parallel = rand() % 6; //Zwischen 0, 5
    int offset_orthogonal = rand() % 3 - 1; //Zwischen -1, 1
    
    
    if (direction == UP) {
        this_pos.y += offset_parallel;
        this_pos.x += offset_orthogonal;
    }
    if (direction == RIGHT) {
        this_pos.x -= offset_parallel;
        this_pos.y += offset_orthogonal;
    }
    if (direction == DOWN) {
        this_pos.y -= offset_parallel;
        this_pos.x += offset_orthogonal;
    }
    if (direction == LEFT) {
        this_pos.x += offset_parallel;
        this_pos.y += offset_orthogonal;
    }
    
    this_pos.x = clamp((int)this_pos.x, 0, CELL_FIELD_SIZE-1);
    this_pos.y = clamp((int)this_pos.y, 0, CELL_FIELD_SIZE-1);
    
    std::cout << bcolor.cyan << "Random Cell Position: " << this_pos.x << " " << this_pos.y << " Direction: " << direction << newl;
    
    return this_pos;
}

//Platziert den cell_type an der Position unter Beachtung vom Verschieben der Kugeln sind bereits da sind.
void place_cell(int x, int y, int cell_type, int direction) {
    if (get_cell_state(x, y) != CELL_EMPTY) { //Falls die Zielzelle nicht leer ist
        Vector2 add_vector = {0, 0}; //Richtung in die geschoben werden soll
        if (direction == UP) {
            add_vector =  {0, 1};
        }
        if (direction == DOWN) {
            add_vector = {0, -1};
        }
        if (direction == LEFT) {
            add_vector = {1, 0};
        }
        if (direction == RIGHT) {
            add_vector = {-1, 0};
        }
        
        Vector2 position = {x+add_vector.x, y+add_vector.y}; //Finale Position, in welche die Zelle (x, y) hingeschoben werden soll
        
        if (is_on_field(position.x, position.y) == false) {
            cout << bcolor.yellow << "Zelle (" << position.x << ", " << position.y << ") ist außerhalb des Spielfeldes. Ignoriere" << newl;
        } else {
            place_cell(position.x, position.y, get_cell_state(x, y), direction); //Führe diese Funktion aus, wegen Rekursion. ("Falls dort auch schon eine Kugel liegt, wird diese ebenfalls verschoben, usw.")
        }
    }
    
    set_cell_state(x, y, cell_type);
}

//Gibt den Spieler zurück, der dieses Spiel gewonnen hat. (PLAYER_NONE wenn unentschieden)
int get_winning_player() {
    Vector2 zielkugel_pos = get_zielkugel_position();
    int player1_count = 0; //Spieler 1 Kugeln neben Zielkugel
    int player2_count = 0; //Spieler 2 Kugeln neben Zielkugel
    
    //Spieler 1
    if (get_cell_state(zielkugel_pos.x, zielkugel_pos.y+1) == CELL_PLAYER1) {
        player1_count += 1;
    }
    if (get_cell_state(zielkugel_pos.x, zielkugel_pos.y-1) == CELL_PLAYER1) {
        player1_count += 1;
    }
    if (get_cell_state(zielkugel_pos.x+1, zielkugel_pos.y) == CELL_PLAYER1) {
        player1_count += 1;
    }
    if (get_cell_state(zielkugel_pos.x-1, zielkugel_pos.y) == CELL_PLAYER1) {
        player1_count += 1;
    }
    
    //Spieler 2
    if (get_cell_state(zielkugel_pos.x, zielkugel_pos.y+1) == CELL_PLAYER2) {
        player2_count += 1;
    }
    if (get_cell_state(zielkugel_pos.x, zielkugel_pos.y-1) == CELL_PLAYER2) {
        player2_count += 1;
    }
    if (get_cell_state(zielkugel_pos.x+1, zielkugel_pos.y) == CELL_PLAYER2) {
        player2_count += 1;
    }
    if (get_cell_state(zielkugel_pos.x-1, zielkugel_pos.y) == CELL_PLAYER2) {
        player2_count += 1;
    }
    
    std::cout << bcolor.cyan << "Spieler 1 Kugeln: " << player1_count << ", Spieler 2 Kugeln: " << player2_count << newl;
    
    if (player1_count == player2_count) {
        return PLAYER_NONE;
    } else if (player1_count > player2_count) {
        return PLAYER_1;
    } else {
        return PLAYER_2;
    }
}

//Zelle wurde angeklickt
void cell_clicked(int x, int y, int active_player) {
    int zielkugel_direction = get_zielkugel_direction(x, y); //Zielkugel Richtung
    Vector2 cell_place = get_rand_cell_position(x, y, zielkugel_direction); //Wo die Kugel platziert werden soll
    place_cell(cell_place.x, cell_place.y, active_player, zielkugel_direction); //Platziere die Kugel wow
}

void draw_game_over(int player_won) {
    DrawRectangle(100, 200, 670-100*2, 200, WHITE);
    DrawRectangleLines(100, 200, 670-100*2, 200, GRAY);
    
    DrawText("Spiel vorbei!", 110, 210, 30, GRAY);
    DrawLine(110, 245, (670-100*2)+90, 245, GRAY);
    
    if (player_won == PLAYER_1) {
        DrawText("Spieler 1 hat gewonnen!", 110, 260, 20, RED);
    } else if (player_won == PLAYER_2) {
        DrawText("Spieler 2 hat gewonnen!", 110, 260, 20, ORANGE);
    } else { //Unentschieden
        DrawText("Unentschieden!", 110, 260, 20, DARKGREEN);
    }
    
    DrawText("x", (670-100*2)+80, 210, 20, GRAY);
}

int main(void)
{ //Basiert auf dem Code von [1] (siehe Verwendete Resourcen.txt)
    // Init
    srand(time(0)); //Setze Seed von rand() auf die Systemzeit
    const int screenWidth = 670; //Screen Breite
    const int screenHeight = 700; //Screen Höhe
    bool is_game_over = false; //Ist das Spiel vorbei?
    
    int active_player = PLAYER_1; //Spieler, der gerade dran ist
    bool only_on_edge = true; //Erlaube nur Züge am Rand
    int player_won = -1; //Spieler der gewonnen hat.
    
    spawn_zielkugel();
    
    InitWindow(screenWidth, screenHeight, "BWINF A4");
    SetTargetFPS(100);
    
    //Lade Settings von .ini Datei
    ini.LoadFile("A4.ini");
    const char *runden_char = ini.GetValue("settings", "rundenanzahl");
    int runden = stoi(runden_char);
    
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update //
        Vector2 mouse_pos = GetMousePosition(); //Maus Position
        Vector2 cell_m_pos = mouse_to_cell_pos(mouse_pos); //"Cell Mouse Position" Zellen Position
        int clicked_cell_state = -1; //Min us eins, falls kein Click
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            clicked_cell_state = get_cell_state(cell_m_pos.x, cell_m_pos.y);
            
            if (is_game_over) { if (mouse_pos.x > 550 && mouse_pos.x < 550+20 && mouse_pos.y > 210 && mouse_pos.y < 210+20) {
                is_game_over = false; //Schließt das Game Over Fenster
            }}
        }
        
        // Draw //
        BeginDrawing();

            ClearBackground(DARKGRAY);
            
            DrawText(TextFormat("Runden übrig: %i", runden), 10, 670, 20, RAYWHITE);
            
            if (clicked_cell_state != -1 && is_valid_cell(cell_m_pos.x, cell_m_pos.y, only_on_edge)){
                cell_clicked(cell_m_pos.x, cell_m_pos.y, active_player);
                
                if (active_player == PLAYER_1) { //Switch active player.
                    active_player = PLAYER_2;
                } else if (active_player == PLAYER_2) { //Switch active player.
                    active_player = PLAYER_1;
                    runden--; //nachdem beide Spieler dran waren, nächste Runde
                    
                    if (runden == 0) {
                        is_game_over = true;
                        player_won = get_winning_player();
                    }
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
            
            draw_highlight_cell_hover(cell_m_pos.x, cell_m_pos.y, active_player, only_on_edge);
            
            if (is_game_over) {
                draw_game_over(player_won);
            }
            
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); //Schließen

    return 0;
}