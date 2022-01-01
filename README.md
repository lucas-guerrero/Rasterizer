# Rasterizer

Create by Lucas GUERRERO

## Changement de fichier obj

Pour modifier les transformations de l'objet, modifier la ligne 63 du fichier src/scene.cpp

## instruction

To compile Rasterizer

```
  make
```

After that, add the path to the librairy to the 'LD_LIBRARY_PATH':  

```
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:<rasterizer>/minwin/bin
```

To launch Rasterizer

```
  bin/rasterize
```


To clean up the project:

```
  make clean
```

## Déplacement de la caméra

Translation:
* Z -> Avancer
* S -> Reculer
* Q -> Gauche
* D -> Droite
* A -> Monter
* E -> Descendre

Rotation:
* I -> Axe X ACW
* K -> Axe X CW
* J -> Axe Y ACW
* L -> Axe Y CW
* U -> Axe Z ACW
* O -> Axe Z CW


## Problème Actuel

Il y a une déformation des objets avec les rotations de la caméra si on utilise différent axe.