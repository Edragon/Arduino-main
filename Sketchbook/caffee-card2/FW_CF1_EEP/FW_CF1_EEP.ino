
#include <EEPROM.h>

void setup()
{
  EEPROM_WriteStr(0, "<ERASE_MEM>");  // da usare solo 1 volta come Reset completo con perdita di tutti i dati !
  
}
// -----------------------------------------------------------------------------------------------
void loop()
{
  
}
// -----------------------------------------------------------------------------------------------
void EEPROM_WriteStr(char *mAddress, char *Data)
{
  int DataSize = strlen(Data);

  if(strcmp(Data,"<ERASE_MEM>") == 0){  // Comando speciale di 'clear' memory 4(l'address viene ignorato)
    for (int i=0; i<EEPROM.length(); i++) {
      EEPROM.write(i, 0);
    }
  }else{
    for(int i=0; i<DataSize; i++){
      EEPROM.write(mAddress+i, Data[i]);
    }
    EEPROM.write(mAddress+DataSize,'\0');   //Add termination null character for String Data
  }
    
  EEPROM.end();
}
