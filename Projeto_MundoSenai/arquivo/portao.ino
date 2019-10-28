const int sentidoHorario = 8; //rele 1
const int sentidoAntiHorario = 9; // rele 2

void setup()
{
  pinMode(sentidoHorario, OUTPUT);
  pinMode(sentidoAntiHorario, OUTPUT);
  Serial.begin(9600);
  Serial.print(">>> Defina o estado do circuito: ");
}

void loop()
{
  
  if(Serial.available() > 0){
    
    int ligaDesliga = Serial.parseFloat();

    if(ligaDesliga == 0){ // circuito desenergizado

      digitalWrite(sentidoHorario, LOW);
      digitalWrite(sentidoAntiHorario, LOW);
    }

    if(ligaDesliga == 1){ // ligar o rele 1

      digitalWrite(sentidoHorario, HIGH);
      digitalWrite(sentidoAntiHorario, LOW);
    }
    if(ligaDesliga == 2){ // ligar o rele 2

      digitalWrite(sentidoHorario, LOW);
      digitalWrite(sentidoAntiHorario, HIGH);
    }

  }
  
}
