const int releEsq = 2; //rele 1
const int releDir = 3; // rele 2

void setup()
{
  pinMode(releEsq, OUTPUT);
  pinMode(releDir, OUTPUT);
  
  Serial.begin(9600);
  Serial.print(">>> Aproxime o Identificador ");
  Serial.println(" ");

  digitalWrite(releEsq, HIGH);
  digitalWrite(releDir, HIGH);
}

void loop()
{
  
  if(Serial.available() > 0){
    
    int ligaDesliga = Serial.parseFloat();

    if(ligaDesliga == 0){ // circuito desenergizado

      digitalWrite(releEsq, HIGH);
      digitalWrite(releDir, HIGH);
    }

    if(ligaDesliga == 2){ // abrir portao

      digitalWrite(releEsq, HIGH);
      digitalWrite(releDir, LOW);
      delay(1000);
      digitalWrite(releDir, HIGH);
    }
    if(ligaDesliga == 1){ // fechar portao

      digitalWrite(releEsq, LOW);
      digitalWrite(releDir, HIGH);
      delay(1000);
      digitalWrite(releEsq, HIGH);
    }
  }
  
}
