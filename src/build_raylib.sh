#!/bin/bash

#
# Wird verwendet für A4_Computer-Pétanque.cpp, weil die Library "raylib" einen anderen Build-Befehl benötigt. Raylib wird für die Graphische Benutzeroberfläche verwendet.
#

g++ A4_Computer-Pétanque.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o ../bin/A4_Computer-Pétanque
