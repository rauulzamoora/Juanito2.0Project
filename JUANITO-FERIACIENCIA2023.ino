// Juanito FERIA DE LA CIENCIA 2023

// Infrarrojos
const int pinSensorDerecha = 8;
const int pinSensorIzquierda = 9;

// Ultrasonico
const int triggerPin = 7;
const int echoPin = 6;

// Motores
const int motorDerechaA = 2;
const int motorDerechaB = 3;
const int motorIzquierdaA = 4;
const int motorIzquierdaB = 5;
const int enableA = 10; // Controlar las RPM del motor IZQ
const int enableB = 11; // Controlar las RPM del motor DER

// Constantes
const int distanciaUmbral = 60;
const int vSonido = 58;
const int tiempoGiro = 800;

int valorSensorDerecha;
int valorSensorIzquierda;
float distancia;

void setup() {
  Serial.begin(9600);
  pinMode(pinSensorDerecha, INPUT);
  pinMode(pinSensorIzquierda, INPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorDerechaA, OUTPUT);
  pinMode(motorDerechaB, OUTPUT);
  pinMode(motorIzquierdaA, OUTPUT);
  pinMode(motorIzquierdaB, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);
  delay(5000);
}

void buscar() {
  analogWrite(enableA, 40);
  analogWrite(enableB, 40);
  digitalWrite(motorDerechaA, HIGH);
  digitalWrite(motorDerechaB, LOW);
  digitalWrite(motorIzquierdaA, LOW);
  digitalWrite(motorIzquierdaB, HIGH);
}

void atacar() {
  analogWrite(enableA, 240);
  analogWrite(enableB, 240);
  digitalWrite(motorDerechaA, HIGH);
  digitalWrite(motorDerechaB, LOW);
  digitalWrite(motorIzquierdaA, HIGH);
  digitalWrite(motorIzquierdaB, LOW);
}

void girarDerecha() {
  analogWrite(enableA, 40);
  analogWrite(enableB, 40);
  digitalWrite(motorDerechaA, LOW);
  digitalWrite(motorDerechaB, HIGH);
  digitalWrite(motorIzquierdaA, HIGH);
  digitalWrite(motorIzquierdaB, LOW);
  delay(tiempoGiro);
}

void girarIzquierda() {
  analogWrite(enableA, 40);
  analogWrite(enableB, 40);
  digitalWrite(motorDerechaA, HIGH);
  digitalWrite(motorDerechaB, LOW);
  digitalWrite(motorIzquierdaA, LOW);
  digitalWrite(motorIzquierdaB, HIGH);
  delay(tiempoGiro);
}

void medirDistancia() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  int duracion = pulseIn(echoPin, HIGH);
  distancia = duracion / vSonido;
}

void retroceder() {
  analogWrite(enableA, 100);
  analogWrite(enableB, 100);
  digitalWrite(motorDerechaA, LOW);
  digitalWrite(motorDerechaB, HIGH);
  digitalWrite(motorIzquierdaA, LOW);
  digitalWrite(motorIzquierdaB, HIGH);
}

bool enModoBuscar = true;  // Variable para controlar el modo del robot (que interrumpa una función para hacer otra)

void loop() {
  // Leer valores de los infrarrojos
  valorSensorDerecha = digitalRead(pinSensorDerecha);
  valorSensorIzquierda = digitalRead(pinSensorIzquierda);

  // Valores del ultrasonico
  medirDistancia();

  //Monitor serial
  Serial.print(distancia);
  Serial.println("cm - ultrasonico");
  Serial.print(valorSensorDerecha);
  Serial.println(" - derecha");
  Serial.print(valorSensorIzquierda);
  Serial.println(" - izquierda");

  //atacar();
  //delay(500);

  // Control de modo
  if (enModoBuscar) {
    if (distancia > distanciaUmbral) {
      buscar();
    } else {
      enModoBuscar = false;
      atacar();
    }
  } else {
    if (distancia <= distanciaUmbral) {
      enModoBuscar = true;
      atacar();
    } else {
      buscar();
    }
  }

  // Lógica de los sensores
  if (valorSensorDerecha == 1 && valorSensorIzquierda == 0) {
    girarIzquierda();
  } else if (valorSensorIzquierda == 1 && valorSensorDerecha == 0) {
    girarDerecha();
  } else if (valorSensorIzquierda == 0 && valorSensorDerecha == 0) {
    retroceder();
  }
}
