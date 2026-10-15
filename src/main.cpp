#include <Arduino.h>

void leituraBloqueante();
void leituraNaoBloqueante();
void processarFrase(char* texto);
int lerProximoNumero(char*& cursor);
void AplicaCor();

int vermelho = 0;
int verde = 0;
int azul = 0;
int brilho = 0;

//! =================================================================
void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  //* readStringUntil espera por caractere no máximo esse tempo(ms) pelo '\n'
  //* O padrão é 1000ms

  Serial.println();
  Serial.println("Digite o seu comando e pressione ENTER: ");
}

//! =================================================================
void loop() 
{
  leituraNaoBloqueante();
}

//! =================================================================
void leituraNaoBloqueante()
{
  const int Tamanho_max = 50;
  //* tamanho do vetor (inclui '\0')
  static char frase[Tamanho_max];
  static int position;

  while(Serial.available() > 0)
  {
    char c = Serial.read();
    //* o continue pula o restante do while naquela volta e passa pra
    //* leitura do próximo caractere. O '\r' é lido e simplesmente descartado
    //* sem entrar no vetor
    if(c == '\r') continue;
    
    if(c == '\n')
    {
      frase[position] = '\0';
      if(position > 0) processarFrase(frase);
      position = 0;
    }
    else if (position < Tamanho_max -1)
    {
      frase[position] = c;
      position++;
    }
  }
}



//! =================================================================
//TODO  Vermelho, verde, azul, branco, desliga,
//TODO* RGB r g b (ex: RGB 200 125 98)
//TODO? BRILHO n (0 a 100%)
void processarFrase(char* texto)
{
  Serial.printf ("Recebido: [%s]\n\r", texto);

  char* comando = texto;
  char* parametro = NULL;
  char* espaco = strchr(texto, ' ');
  if(espaco != NULL)
  {
    parametro = espaco + 1;
    *espaco = '\0';
  }

  //? strcasecmp = string compare
  //? compara normal mas sem levar em consideracão maiuscula e minuscula
  if(strcasecmp(comando, "VERMELHO") == 0)
  {
    vermelho = 255;
    verde = 0;
    azul = 0;
    brilho = 100;
  }
  else if(strcasecmp(comando, "VERDE") == 0)
  {
    vermelho = 0;
    verde = 255;
    azul = 0;
    brilho = 100;
  }
  else if(strcasecmp(comando, "AZUL") == 0)
  {
    vermelho = 0;
    verde = 0;
    azul = 255;
    brilho = 100;
  }
  else if(strcasecmp(comando, "BRANCO") == 0)
  {
    vermelho = 255;
    verde = 255;
    azul = 255;
    brilho = 100;
  }
  else if(strcasecmp(comando, "DESLIGA") == 0)
  {
    vermelho = 0;
    verde = 0;
    azul = 0;
    brilho = 0;
  }
  else if(strcasecmp(comando, "RGB") == 0)
  {
    if (parametro == NULL)
    {
      Serial.println("Use o comando: RGB r g b  (0 a 255)");
      return;
    }

    char* cursor = parametro;

    int r = lerProximoNumero(cursor);
    int g = lerProximoNumero(cursor);
    int b = lerProximoNumero(cursor);
    vermelho = constrain(r, 0, 255);
    verde = constrain(g, 0, 255);
    azul = constrain(b, 0, 255);
  }
  else if(strcasecmp(comando, "BRILHO") == 0)
  {
    if (parametro == NULL)
    {
      Serial.println("Use o comando: BRILHO n  (0 a 100)");
      return;
    }

    brilho = constrain(atoi(parametro), 0, 100);
  }
  else
  {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Comando: [%s] não existe", comando);
    Serial.println(buffer);
    return;
  }
  AplicaCor();
}

//! =================================================================
void AplicaCor()
{
  int r = vermelho * brilho / 100;
  int g = verde * brilho / 100;
  int b = azul * brilho / 100;
  rgbLedWrite(PIN_RGB_LED, r, g, b);

  char bufferMensagem[80];
  snprintf(bufferMensagem, sizeof(bufferMensagem), "Cor: R%3d G%3d B%3d | Brilho: %3d %% | HEX: #%02X%02X%02X", 
  vermelho, verde, azul, brilho, r, g, b);
  Serial.println(bufferMensagem);
}

//! =================================================================

//* cursor é uma referencia (&) para um ponteiro char (char*)
//* a funcão recebe a própria variável de quem chamou e não uma cópia

//* PARAMETRO     | ALTERA O TEXTO APONTADO | ALTERA PARA ONDE O PONTEIRO DE FORA APONTA
//* char * p      |    sim (*p = '\0')      |         não porque ele é uma cópia
//* const char* p |           não           |                   não
//* char* &p      |           sim           | sim, é a própria variável passada como parametro

int lerProximoNumero(char*& cursor)
{
  if(cursor == NULL) return 0; //? proteje o estouro de memória no caso atoi(NULL)
  int valor = atoi(cursor);
  char* espaco = strchr(cursor, ' ');
  cursor = (espaco != NULL)? espaco + 1 : NULL;
  return valor;
}



//! =================================================================
void leituraBloqueante()
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