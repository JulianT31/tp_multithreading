# TP Multithreading

## Introduction

Ce dépôt est un projet scolaire inscrit dans le module de multithreading de la 3ème année de l'UPSSITECH.

Les auteurs de ce projet sont Julian TRANI & Pauline JOBERT.

**DISCLAIMER** : Ce projet a été réalisé sous Windows, les commandes pour le C++ n'ont pas été testées.

## Fonctionnement

### Partie Python

Pour lancer la partie Python, vous devez exécuter le fichier `manager.py`, le fichier `proxy.py`, puis `boss.py` et `minion.py` si vous souhaitez que la tâche soit exécutée en Python ou C++.

Dès son lancement, le fichier `boss.py` va ajouter 15 tâches dans la file d'attente.

Quant au `minion`, il va récupérer une tâche dans la `queue` afin de l'exécuter, puis renvoyer le résultat dans une `queue`.

Voici un exemple d'exécution :

![Running](images/running_python.png)

### Partie C++

Vous pouvez lancer l'équivalent d'un minion Python, mais en C++, qui va vérifier de manière périodique si une tâche est disponible afin de la réaliser.

Pour cela, **assurez-vous que le `proxy` Python soit lancé** avant d'exécuter le code C++ avec les commandes suivantes :

```
cmake -B build -S .
cmake --build build
./build/low_level
```

Voici un exemple d'exécution du côté C++ :

![Running c++](images/running_cpp.png)

Une fois l'exécution de la tâche terminée, cette tâche est renvoyée au manager via une requête `POST` (proxy).

Du côté du code Python, nous récupérons les informations sur l'exécution de la tâche :

![Running python c++](images/running_python_cpp.png)

## Comparaison Python/C++

Voici un tableau comparatif des différents tests réalisés.
**NB :** En C++, les tests sont réalisés en mode `RELEASE` afin d'obtenir de meilleures performances.

| Taille             |                3000                |
| :----------------- | :--------------------------------: |
| Python             | Task ended in en 0.3022408 seconds |
| C++ (Piv)        |  Task ended in en 10.0547 seconds  |
| C++ (Lu) 1 thread  |  Task ended in en 1.34219 seconds  |
| C++ (Lu) 2 threads | Task ended in en 0.884321 seconds  |
| C++ (Lu) 4 threads | Task ended in en 0.627631 seconds  |
