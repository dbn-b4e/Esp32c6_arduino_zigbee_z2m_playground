# esp32c6-evk_lvgl

## Présentation

Ce projet fournit une architecture modulaire prête à l'emploi pour développer une interface graphique LVGL sur une carte ESP32-C6 avec écran SPI, tactile I2C et gestion multi-plateforme (Arduino IDE **et** PlatformIO).

- **Organisation claire** : séparation écran, tactile, gestion GUI, callbacks personnalisés compatibles EEZ Studio.
- **Compatible** : Arduino IDE, PlatformIO (pioarduino), EEZ Studio (callbacks `action_`).
- **Utilisation facilitée** : script Python pour générer un dossier à plat pour l'IDE Arduino.

---

## Arborescence

```
esp32c6-evk_lvgl/
├── src/
│   ├── esp32c6-evk_lvgl.ino
│   ├── ScreenGFX.cpp
│   ├── TouchInput.cpp
│   ├── GuiManager.cpp
│   ├── custom_actions.cpp
│   ├── ui.h         # généré par EEZ Studio
│   └── ui.c         # généré par EEZ Studio
├── include/
│   ├── ScreenGFX.h
│   ├── TouchInput.h
│   ├── GuiManager.h
│   ├── custom_actions.h
│   └── targ_def.h
├── platformio.ini
├── prepare_for_arduino.py
```

---

## Utilisation sous PlatformIO

1. **Ouvrir le dossier dans VSCode avec PlatformIO.**
2. Vérifier/adapter `platformio.ini` selon votre carte & librairies.
3. Compiler, flasher, monitorer : tout fonctionne directement.

---

## Utilisation sous Arduino IDE

1. **Exécuter le script** (nécessite Python 3) :
   ```sh
   python3 prepare_for_arduino.py
   ```
   → Cela crée un dossier `arduino_flat/` avec tous les fichiers nécessaires à plat.

2. **Ouvrir le dossier `arduino_flat/` dans l’IDE Arduino.**
3. Compiler, flasher normalement.

---

## Fichiers clés

- `esp32c6-evk_lvgl.ino` : point d'entrée Arduino.
- `ScreenGFX.*` : gestion écran (Arduino_GFX).
- `TouchInput.*` : gestion tactile.
- `GuiManager.*` : initialisation LVGL, gestion des callbacks statiques, cycle principal.
- `custom_actions.*` : vos callbacks EEZ Studio `action_...` (à ne pas renommer).
- `ui.h/ui.c` : générés par EEZ Studio, à placer dans `src/`.

---

## Générer le dossier compatible Arduino IDE

Lancez simplement :

```sh
python3 prepare_for_arduino.py
```

Le dossier `arduino_flat/` contiendra tous les fichiers nécessaires pour le sketch Arduino.

---

## Dépendances

À adapter dans `platformio.ini` :

- [lvgl/lvgl](https://platformio.org/lib/show/1041/lvgl)
- [moononournation/Arduino_GFX](https://platformio.org/lib/show/9424/Arduino_GFX)
- [Wire](https://platformio.org/lib/show/355/Wire)
- Librairie tactile adaptée (`esp_lcd_touch_axs5106l` ou autre selon écran)

---

## Notes EEZ Studio

- Les callbacks générés par EEZ Studio doivent conserver leur nom (`action_...`) pour rester compatibles.
- Placez les fichiers `ui.h` et `ui.c` générés dans le dossier `src/`.

---

## Script Python de préparation

Ce script copie tous les fichiers nécessaires dans un dossier à plat pour l’IDE Arduino.

---

## Contact

Projet maintenu par dbn-b4e – toute contribution ou suggestion est bienvenue.
