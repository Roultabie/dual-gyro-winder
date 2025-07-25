//**************************************************************
// Commande de moteur pas-à-pas unipolaire 4 fils
// tiptopboards.com 05/12/2013
// Mise à jour du 28/07/2025
// Refactorisation du code
// Ajout de la gestion de la rotation des moteurs simultanés
// Ajout d'un section dédiée à la personnalisation des variables
// Ajout de documentation
// Driver ULN2003 et moteur réducté au 1:64 de type 28BYJ-48
// 64 pas par tour, 4 phases, angle de 5.625° selon les spécifications du moteur
// Démultiplication 1:64 pour ce moteur réducté mécaniquement
// 360° / 5.625° * 64 = 4096 angles avec la démultiplication
// 360° / 5.625° * 64  * 4 bobines / 2 bipolaire = 2048 step / tour
//
//**************************************************************
//Inclure la librairie stepper.h
#include <Stepper.h>
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

//Créer une instance de la classe stepper
//Le moteur (fils 1 2 3 4) est branché sur les sorties 9 10 11 12 de l'Arduino (et sur GND, +V)
Stepper small_stepper1(STEPS, 9, 11, 10, 12);  // Sens horaire
Stepper small_stepper2(STEPS, 5, 7, 6, 8); // Sens horaire

int compteur = 0;

void setup() {
  Serial.begin(9600);  // 9600 bps
  Serial.println("Lancement du mouvement gyroscopique");
  pinMode(ledSwitch, OUTPUT);  // Declare le Pin 5 comme sortie pour la LED et indique que la broche led est une sortie :
  pinMode(selecteurLeft, INPUT); // indique que la broche bouton est une entrée :
  pinMode(selecteurRight, INPUT); // indique que la broche bouton est une entrée :
}

// Démarrage de la boucle
void loop() {
  digitalWrite(ledSwitch, HIGH);  // On allume la led
  // lit l'état du sélecteur 3 positions et extrapole le ou les moteurs allumés dans la variable motors (0 -> les deux moteurs, 1 gauche, 2 droite)
  int motors = (digitalRead(selecteurLeft) == LOW && digitalRead(selecteurRight) == LOW) ? 0 : (digitalRead(selecteurLeft) == HIGH) ? 1 : 2;

  // On définit la vitesse des moteurs
  Serial.println("Réglage de la vitesse des moteurs");
  small_stepper1.setSpeed(speed);
  small_stepper2.setSpeed(speed);  //Vitesse de 300 (max) réduire ce chiffre pour un mouvement plus lent
                                   //100 permet d'avoir un couple élevé >300 le moteur vibre sans tourner

  // Lancement d'un série de rotations
  // Rotation simultanée des moteurs quand le switch est au centre
  Serial.println("Rotation des moteurs");
  int step = (compteur % 2) ? 1 : -1; // On vérifie si pair ou impair pour lancer la rotation dans les deux sens
  for (int st = 1; st <= nbPas; st++) {
    if (motors == 0 || motors == 1) {
      small_stepper1.step(step);
    }
    if (motors == 0 || motors == 2) {
      small_stepper2.step(step);
    }
  }

  // Pause après une rotation horaire / anti horaire permettant d'arrêter physiquement le système, moteurs bien placés
  Serial.println("Pause entre deux révolutions");
  int nbClign = (compteur >= series * 2) ? pause / 2 : (step == 1) ? nbClignLed : 1;
  for (int led = 0; led < nbClign * 2; led++) { // Gestion du clignotement, on multiplie par 2 pour respecter le nombre souhaité
    int ledState = (led % 2) ? LOW : HIGH; // Si pair, led éteinte, si impair, led allumée
    // Clignotement de la led
    digitalWrite(ledSwitch, ledState);
    delay(attenteLed);
  }
  compteur = (compteur < series * 2) ? compteur += 1 : 0;  //Ajoute 1 au Compteur si < à série, sinon, on le réinitialise
}