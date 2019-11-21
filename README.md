Projet_Moteur_de_jeu_Imagerie_3D_Sons_et_musique
===
# HMIN 323 Moteur de jeux
### Génération procédurale
# HMIN318M Imagerie Médicale et 3D
### Moteur Voxel
# HMIN 316 Sons et musique
### Ben la musique
Moi : musique milieu et fin, guillaume : début et combat
# Projet :
- Moteur voxel
- Map générer procéduralement
- Carte en longueur. Objectif : traverser le plus rapidement possible sans mourrir
- c++ & sfml
- Unité de base : 1/512 de blocs,
- un personnage : 2 bloc
- definir des formes fixe (pour le moment )
- github
- modèle de phong
- matrice MVA pour les objets  

# PROG

- ECS (ECS guillaume)
- Personnages :
  - sauter -> moteur physique
    - position (vec3)
    - vitesse (vec3 )
    - accélération (vec3)
    - collisions par cubes ()
  - avancer
  - tirer avec une arme
    - minigun
    - bazooka
    - grenade
    - revolver silencieux
- Projectile : vitesse (pas de gravité sauf grenade)
- changer d'arme
- courrir
- C.A.C MACHETTE
- génération de terrain
  - générer sturcture
  - génération énnemies
- algo difficulté
- ennemies :
  - IA
  - FSM
  - repere joueur selon son et vues
- son : FMOD

- UI : menu
  - jouer
  - options
  - quitter
- HUD
- écran victoire
