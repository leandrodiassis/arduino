#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
const int releEsq = 2; //rele 1
const int releDir = 3; // rele 2
const int pinSensor = 4; // pino do sensor
const int pinFonte = 5; // pino da fonte do sensor
const int buzzer = 6;
int i = 0;
 
char st[20];
 
void setup() 
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  pinMode(releEsq, OUTPUT);
  pinMode(releDir, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pinSensor, INPUT);
  pinMode(pinFonte, OUTPUT);
  
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  //Define o número de colunas e linhas do LCD:  

}
 
void loop() 
{
  inicio:
  digitalWrite(pinFonte, LOW); // sensor desligado por padrao
  digitalWrite(releEsq, HIGH); // rele esquerdo desligado por padrao
  digitalWrite(releDir, HIGH); // rele direito desligado por padrao
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

  
  if (conteudo.substring(1) == "F2 77 26 2C") //se o cartao inserido for o cadastrado
  {
    Serial.println("Acesso Garantido");
    tone(buzzer,12000,400);
    delay(500);
    tone(buzzer,12000,400);
    digitalWrite(pinFonte, HIGH); // ligar o sensor
        abrirPortao();  


  } else { //se o cartao inserido nao for o cadastrado
    Serial.println("Acesso Negado");
        tone(buzzer,800,800);
        goto inicio;
  }
 
  fecharPortao();
  
} 

int abrirPortao(){
  Serial.println("Abrindo Portao");
  digitalWrite(releDir, LOW);
  delay(1000);  
  digitalWrite(releDir, HIGH);
  return 0;
}

int fecharPortao(){

    Serial.println("Processando fechamento");
  bool estadoSensor = digitalRead(pinSensor); // ler estado do sensor

  do{ 
    Serial.println("aguardando passagem do veiculo");
    estadoSensor = digitalRead(pinSensor); // ler estado do sensor
  } while(estadoSensor);// enquanto nao ha objeto
  
    
    do{
        Serial.println("Algo esta passando");
         
        
         if(i==1){
            tone(buzzer,800,250);          
         } else{
            digitalWrite(buzzer,LOW);
          }
   
        i++;    
        estadoSensor = digitalRead(pinSensor); // ler estado do sensor
      } while(!estadoSensor); // quando sensor detectar algo
    
    i = 0;    
      
    if(estadoSensor){ // quando sensor nao detectar
     
      
      tone(buzzer,1200,200);
      delay(400);
      tone(buzzer,1200,200);
      delay(650);
      
      digitalWrite(releEsq,LOW);
      delay(1000);
      digitalWrite(releEsq,HIGH);
           
      
  }
  return 0;
}
