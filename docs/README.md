# Ballon Laby Defense

Un jeu de type tower defense dans un labyrinthe généré aléatoirement, développé en C avec la bibliothèque raylib.

![Screenshot du jeu](screenshots/gameplay.png)

## Description

Ballon Laby Defense est un jeu où vous devez défendre un labyrinthe généré procéduralement contre des vagues de ballons. Le jeu utilise l'algorithme de Prim pour générer un labyrinthe unique à chaque partie, et l'algorithme A* pour calculer les chemins des ballons.

### Fonctionnalités
- Génération procédurale de labyrinthes
- Pathfinding intelligent des ballons pour éviter les tours basé sur A*
- Placement stratégique de tours de défense
- Multiples vagues d'ennemis
- Projectiles avec ciblage automatique
- Système de points de vie et de monnaie pour poser les tours

## Captures d'écran

<table>
  <tr>
    <td><img src="docs/Screen_debut.png" alt="Début de partie" width="400"/></td>
    <td><img src="docs/Screen_partie.png" alt="Milieu de partie" width="400"/></td>
  </tr>
  <tr>
    <td><img src="docs/Screen_win.png" alt="Win" width="400"/></td>
    <td><img src="docs/Screen_gameover.png" alt="Fin de partie" width="400"/></td>
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

# Lancer l'exécutable
./bloons_maze_battle