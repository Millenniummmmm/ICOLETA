
#include <ESP32Servo.h>
 // Biblioteca do servo motor

// Definir pino do servo motor
#define SERVO_PIN 15

Servo servoMotor; // Objeto para controlar o servo

void setup() {
  // Iniciar o servo e definir o pino de controle
  servoMotor.attach(SERVO_PIN);
  
  // Inicializar o servo na posição 0 graus
  servoMotor.write(0);  
}

void loop() {

  servoMotor.write(60);  
  delay(5000);  // Aguarde 5 segundos

 
  servoMotor.write(0);   
  delay(2000);  // Aguarde 2 segundos
}
