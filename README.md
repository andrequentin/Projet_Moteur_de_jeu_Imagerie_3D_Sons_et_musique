Projet_Moteur_de_jeu_Imagerie_3D_Sons_et_musique
===
# HMIN 323 Moteur de jeux
### Génération procédurale
# HMIN318M Imagerie Médicale et 3D
### Moteur Voxel
# HMIN 316 Sons et musique
### Ben la musique

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

Voxel :
- Position X,Y,Z
- Couleur
- Echelle


VoxelSet :
  - Ensemble De Voxel qui défini une entité

Bloc :
  - VoxelSet de 8x8x8 Voxel, Traite l'affiche

Player :
    - VoxelSet
    - Caméra
    - GUN

    
