# Remontoir de montres à base d'Arduino

Ce projet est une réécriture du code du remontoir de montres automatiques "Dual Gyro Winder" de Nedalive disponible sur Cults3D, à base d’Arduino nano, permettant de faire tourner un ou deux moteurs pas-à-pas (type 28BYJ-48) via des boutons de sélection. Le système est conçu pour être **entièrement autonome**, avec indicateurs LED, contrôle des cycles, et alternance automatique des sens de rotation.

## Objectif du projet

L’objectif principal est de maintenir en mouvement des montres mécaniques automatiques en simulant un mouvement de poignet.

### Challenge personnel

> **Réduire le nombre de lignes de code au strict nécessaire**, sans sacrifier aucune fonctionnalité :
- réduction de la logique conditionnelle,
- nommage clair et concis,
- boucles et comportements simplifiés,
- optimisation de la gestion des moteurs, LED, et logique de cycle.

---

## Matériel utilisé

- arduino UNO ou Nano
- 2 moteurs pas-à-pas **28BYJ-48** avec drivers **ULN2003**,
- switch 3 positions pour sélectionner la rotation,
- 1 LED intégrée au bouton pour retour visuel,
- alimentation 5V externe,
- 2 résistances 100K.

---

## Fonctionnement

| Élément | Description |
|--------|-------------|
| `motor1`, `motor2` | Deux moteurs indépendants contrôlés par le programme |
| `selecteurLeft`, `selecteurRight` | Switch 3 positions pour choisir quel moteur est actif |
| `ledSwitch` | LED qui s’allume pendant la rotation et clignote en pause |
| `compteur` | Suit le nombre de cycles réalisés |
| `rotation alternée` | Change de sens à chaque rotation (horaire ↔ anti-horaire) |
| `pause intelligente` | Pause visuelle prolongée au bout de plusieurs cycles |

---

## Modes de sélection (via interrupteur à 3 positions)

| Position du switch | Moteur(s) activé(s) |
|----------------------|----------------------|
| Au centre | **Les deux moteurs tournent** |
| Côté gauche | **Seulement le moteur gauche (`motor1`)** |
| Côté droit | **Seulement le moteur droit (`motor2`)** |

---

## Personnalisation facile

Les variables modifiables sont regroupées en haut du code :

```cpp
const int nbPas = 4096;         // Pas par rotation
const int nbClignLed = 3;       // Nombre de clignotements
const int attenteLed = 1000;    // Temps entre clignotements
const int series = 30;          // Nombres de rotations avant pause longue
const int speed = 300;          // Vitesse moteur (max ~300)
const unsigned long pause = 300; // Pause longue (en secondes)
