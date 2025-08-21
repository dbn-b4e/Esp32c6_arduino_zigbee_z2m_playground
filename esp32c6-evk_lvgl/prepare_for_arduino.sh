#!/bin/bash

# Dossier cible (à créer ou vider)
TARGET="arduino_flat"

# Créer/vider le dossier
rm -rf "$TARGET"
mkdir "$TARGET"

# Copier tous les .ino, .cpp, .h de src et include
find src -type f \( -name "*.ino" -o -name "*.cpp" -o -name "*.c" \) -exec cp {} "$TARGET/" \;
find include -type f -name "*.h" -exec cp {} "$TARGET/" \;

# Copier aussi les fichiers générés par EEZ Studio si besoin
if [ -f src/ui.h ]; then cp src/ui.h "$TARGET/"; fi
if [ -f src/ui.c ]; then cp src/ui.c "$TARGET/"; fi

echo "Tous les fichiers sont dans $TARGET/"