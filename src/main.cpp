#include <Arduino.h>

void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  //* readStringUntil espera por caractere no máximo esse tempo(ms) pelo '\n'
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
    texto.trim();
    //? tratamento de string: trim() remove do fim do texto os espacos, '\r', '\n', '\t' que alguns terminais enviam
    if (texto.length() == 0) 
    {
      Serial.println("Erro! Escreva um nome válido.");
      return;
    }

    Serial.printf("Seu Nome é %s\n\r", texto.c_str());
    Serial.printf("Quantidade de caracteres: %u\n\r", texto.length());
    Serial.printf("Primeira letra: %c\n\r", texto.charAt(0));
    Serial.printf("Ultima letra: %c\n\r", texto[texto.length() - 1]);

    String maiusculo = texto;
    maiusculo.toUpperCase();
    Serial.printf("Em maiuscúlas: %s\n\r", maiusculo);

    if(maiusculo == "BRUNO")
      Serial.println("Esse é seu nome correto");
  }
}