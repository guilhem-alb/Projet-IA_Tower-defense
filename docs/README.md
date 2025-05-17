# Ballon Laby Defense

Un jeu de type tower defense dans un labyrinthe généré aléatoirement, développé en C avec la bibliothèque raylib.

![Screenshot du jeu](screenshots/gameplay.png)

## Description

Ballon Laby Defense est un jeu où vous devez défendre un labyrinthe généré procéduralement contre des vagues de ballons. Le jeu utilise l'algorithme de Prim pour générer un labyrinthe unique à chaque partie, et l'algorithme A* pour calculer les chemins des ballons.

### Fonctionnalités
- Génération procédurale de labyrinthes
- Algorithme A* pour le pathfinding des ballons
- Placement stratégique de tours de défense
- Multiples vagues d'ennemis
- Projectiles avec ciblage automatique

## Captures d'écran

<table>
  <tr>
    <td><img src="screenshots/start_game.png" alt="Début de partie" width="400"/></td>
    <td><img src="screenshots/mid_game.png" alt="Milieu de partie" width="400"/></td>
  </tr>
  <tr>
    <td><img src="screenshots/tower_placement.png" alt="Placement de tours" width="400"/></td>
    <td><img src="screenshots/game_over.png" alt="Fin de partie" width="400"/></td>
  </tr>
</table>

## Guide d'installation

### Prérequis
- GCC ou Clang (compilateur C)
- [raylib](https://www.raylib.com/) (bibliothèque graphique)
- Make (pour la compilation)

### Installation sur Linux
```bash
# Installer les dépendances
sudo apt update
sudo apt install build-essential git
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev

# Installer raylib
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
cd ../..

# Cloner le dépôt
git clone https://github.com/votreusername/ballon-laby-defense.git
cd ballon-laby-defense

# Compiler le jeu
make