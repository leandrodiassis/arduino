#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 9;     // pino RST do RFID
constexpr uint8_t SS_PIN = 10;     // pino SS do RFID
const int releEsq = 2; //rele 1
const int releDir = 3; // rele 2
const int pinSensor = 4; // pino do sensor
const int pinFonte = 5; // pino da fonte do sensor
const int buzzer = 6;
int i = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN); 

//*****************************************************************************************//
void setup() {
  Serial.begin(9600);                                           // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F(">>> Ler Informacoes Pessoais:"));    //shows in serial that it is ready to read

  pinMode(releEsq, OUTPUT);
  pinMode(releDir, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pinSensor, INPUT);
  pinMode(pinFonte, OUTPUT);
  //mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
}

//*****************************************************************************************//
void loop() {
  digitalWrite(pinFonte, LOW); // sensor desligado por padrao
  digitalWrite(releEsq, HIGH); // rele esquerdo desligado por padrao
  digitalWrite(releDir, HIGH); // rele direito desligado por padrao

  
  // Preparar chave- todas as chaves sao configuradas como FFFFFFFFFFFFh configuracao de fabrica.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Buscar novos cartoes
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F(">>> Cartao Detectado:**"));

  //-------------------------------------------

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //informar detalhes sobre o cartao
  
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //ver blocos hex

  //-------------------------------------------

  Serial.print(F("Nome: "));

  byte buffer1[18];

  block = 4;
  len = 18;

  //------------------------------------------- Pegar Nome
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //linha 834 do arquivo MFRC522.cpp
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Falha na Autentificacao: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Erro de Leitura: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  //Imprimir Nome
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]);
    }
  }
  Serial.print(" ");

  //---------------------------------------- Pegar Ultimo Nome

  byte buffer2[18];
  block = 1;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //linha 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Falha de Autentificacao: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Erro de Leitura: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  //Imprimir Ultimo Nome
  for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer2[i] );
  }


  //----------------------------------------

  Serial.println(F("\n**Leitura Realizada**\n"));
 
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  abrirPortao();
  
  digitalWrite(pinFonte, HIGH); // ligar o sensor
  
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
          digitalWrite(buzzer,HIGH);
          delay(250);
          digitalWrite(buzzer,LOW);
          delay(250);
         } else{
            digitalWrite(buzzer,LOW);
          }
   
        i++;    
        estadoSensor = digitalRead(pinSensor); // ler estado do sensor
      } while(!estadoSensor); // quando sensor detectar algo
    
    i = 0;    
      
    if(estadoSensor){ // quando sensor nao detectar
     
      digitalWrite(releEsq,LOW);
      
      
      digitalWrite(buzzer,HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer,HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(400);
      digitalWrite(releEsq,HIGH);
  }
  return 0;
}



//*****************************************************************************************//
