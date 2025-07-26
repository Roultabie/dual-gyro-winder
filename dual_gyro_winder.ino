#include <Stepper.h> //Inclure la librairie stepper.h
#define STEPS 100

const int selecteurLeft = 2; // Numéro de la broche à laquelle est connecté le bouton poussoir 1
const int selecteurRight = 3; // Numéro de la broche à laquelle est connectée le bouton poussoir 2
const int ledSwitch = 4; // Numéro de la broche à laquelle est connecté la led du bouton pour allumer / éteindre le système
const int nbPas = 4096; // Nombre de pas demandés au moteur, une rotation complète avec 2048 pas (1 tour environ 4.5sec)
const int nbClignLed = 3; // Nombre de clignotement de la led après deux rotations (permet d'arrêter le système moteurs bien placés)
const int attenteLed = 1000; // Durée en ms entre les deux clignotements de la led.
const int series = 30; // Nombre de révolutions faites avant une pause
const int speed = 300; // Vitesse de 300 (max) réduire ce chiffre pour un mouvement plus lent du moteur pas à pas
//100 permet d'avoir un couple élevé >300 le moteur vibre sans tourner
const unsigned long pause = 300; // Pause entre deux séries (en secondes)
int sens = 2; // Sens de rotation des moteurs : 0 horaire, 1 anti horaire, 2 aléatoire, 3+ horaire puis anti horaire

//Créer une instance de la classe stepper
//Le moteur (fils 1 2 3 4) est branché sur les sorties 9 10 11 12 de l'Arduino (et sur GND, +V)
Stepper motor1(STEPS, 9, 11, 10, 12);  // Sens horaire
Stepper motor2(STEPS, 5, 7, 6, 8); // Sens horaire

int compteur = 0;

void setup() {
  Serial.begin(9600);  // 9600 bps
  pinMode(ledSwitch, OUTPUT);  // Declare le Pin 5 comme sortie pour la LED et indique que la broche led est une sortie :
  pinMode(selecteurLeft, INPUT); // indique que la broche bouton est une entrée :
  pinMode(selecteurRight, INPUT); // indique que la broche bouton est une entrée :
}

// Démarrage de la boucle
void loop() {
  digitalWrite(ledSwitch, HIGH);  // On allume la led
  // lit l'état du sélecteur 3 positions et extrapole le ou les moteurs allumés dans la variable motors (0 -> les deux moteurs, 1 gauche, 2 droite)
  int motors = (digitalRead(selecteurLeft) == LOW && digitalRead(selecteurRight) == LOW) ? 0 : (digitalRead(selecteurLeft) == HIGH) ? 1 : 2;

  motor1.setSpeed(speed);
  motor2.setSpeed(speed);

  // Lancement d'une série de rotations
  // Sens : 0 horaire, 1 anti horaire, 2 aléatoire, 3+ horaire puis anti horaire
  int direction = (sens == 0) ? -1 : (sens == 1) ? 1 : (sens == 2) ? (random(0, 2) == 0) ? -1 : 1 : (compteur % 2) ? 1 : -1;
  for (int i = 1; i <= nbPas; i++) {
    if (motors == 0 || motors == 1) motor1.step(direction);
    if (motors == 0 || motors == 2) motor2.step(direction);
  }

  // Pause après une rotation horaire / anti horaire permettant d'arrêter physiquement le système, moteurs bien placés
  int nbClign = (compteur >= series * 2) ? pause / 2 : (compteur % 2) ? nbClignLed : 1; // Soit longue pause, soit pose entre 2 révolution, sinon micro pause
  for (int i = 0; i < nbClign * 2; i++) { // Gestion du clignotement, on multiplie par 2 pour respecter le nombre souhaité
    digitalWrite(ledSwitch, (i % 2) ? LOW : HIGH); // Si pair, led éteinte, si impair, led allumée
    delay(attenteLed);
  }
  compteur = (compteur < series * 2) ? compteur += 1 : 0;  //Ajoute 1 au Compteur si < à série, sinon, on le réinitialise
}