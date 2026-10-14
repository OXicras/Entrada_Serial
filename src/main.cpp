#include <Arduino.h>

void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(2000);
  //* readStringUntil espera no máximo esse tempo(ms) pelo '\n'
  //* O padrão é 1000ms

  Serial.println();
  Serial.println("Digite o seu nome e pressione ENTER: ");
}

void loop() 
{
  //* available() diz quantos bytes já chegaram e estão esperando leitura
  if(Serial.available() > 0)
  {
    String texto = Serial.readStringUntil('\n');
    Serial.printf("Seu Nome é %s \n\r", texto.c_str());
  }
}