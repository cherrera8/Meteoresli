#ifndef MicroSD_h
#define MicroSD_h
/* Librerías fabricante necesarias para utilizar la MicroSD */
#include <SD.h>
#include <SPI.h>

#define MICROSD_PIN 15    /* Pin de control MicroSD */  
File MicroSD_File;       /* Archivo para escribir  o leer en la MicroSD */
class MicroSD  {
  // VARIABLES
  public:
    String filename = "/prieto123.txt";   /* Nombre de archivo que incluye "/" que indica la ubicación en raíz */
  // MÉTODOS
  public:
   void MicroSD_init ( void );
   void writeFile ( String );
   String readFile ( void );

};

void MicroSD::MicroSD_init ( void ){

   while ( !SD.begin ( MICROSD_PIN ) ) {
        Serial.println ( F ( "Falló la inicialización del módulo MicroSD"  ));
        delay(1000);
    }
    //Serial.println ( F ( "La librería MicroSD ha inicializado con éxito" ) );
}

String MicroSD::readFile()
{
  String text;
  File textFile = SD.open("/prieto.txt");
  if (textFile)
  {
    while (textFile.available())
    {
      text += (char)textFile.read();
    }
    textFile.close();
  } 
  else
  {
    // Si no existe el archivo
    //serializeJson(doc, text);
    writeFile(text);
  }
  Serial.println(text);
  return text;
}
void MicroSD::writeFile(String text)
{
  File textFile = SD.open("/prieto.txt", FILE_WRITE);
  if (textFile)
  {
    textFile.print(text);
    textFile.close();
    Serial.println(text); //readFile()
  }
  else
  {
    Serial.println("Caca");
  }
}

#endif