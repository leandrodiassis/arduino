#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
int c = 0; 
char st[20];

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//entrada digitais
const int releEsq = 2; //rele 1
const int releDir = 3; // rele 2
const int pinSensor = 4; // pino do sensor
const int pinFonte = 5; // pino da fonte do sensor
const int buzzer = 6;
int comumSensor = A5;
int receptorAbertura = 7;
int receptorFechamento = 8;
 
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
  
  pinMode(comumSensor, INPUT);
  pinMode(receptorAbertura, OUTPUT);
  pinMode(receptorFechamento, OUTPUT);
  
  
  
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();

}


 
void loop() 
{
  inicio: ;
  digitalWrite(pinFonte, LOW); // sensor desligado por padrao
  digitalWrite(releEsq, HIGH); // rele esquerdo desligado por padrao
  digitalWrite(releDir, HIGH); // rele direito desligado por padrao
  digitalWrite(receptorAbertura, HIGH);
  digitalWrite(receptorFechamento, HIGH);
  //int test = analogRead(comumSensor);
  //Serial.println(test);
  
  
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
    Serial.println("voltou para o loop");
    goto inicio;
}


 

int abrirPortao(){
  
  Serial.println("Abrindo Portao");
  int estadoPortao;
  digitalWrite(releDir, LOW);
  //delay(200);
  
  digitalWrite(releDir, LOW);
  delay(100);
  estadoPortao = analogRead(comumSensor);
  
  /*if(estadoPortao <2){
      digitalWrite(releDir, LOW);
      delay(500);
  }*/
  do{
    digitalWrite(releDir, LOW);
    delay(100);
    estadoPortao = analogRead(comumSensor);
    }while(estadoPortao < 2);
  
  
  digitalWrite(releDir, HIGH);  
  return 0;
}

int fecharPortao(){
   
   int j = 0; 
   int tempoPortao = 40;
   int estadoPortao;
     
   
  Serial.println("Processando fechamento");
  bool estadoSensor = digitalRead(pinSensor); // ler estado do sensor

  do{ 
    Serial.println("aguardando passagem do veiculo");
    estadoSensor = digitalRead(pinSensor); // ler estado do sensor
  } while(estadoSensor);// enquanto nao ha objeto
  
    
    do{
        Serial.println("Algo esta passando");
         
        
         if(c==1){
            tone(buzzer,800,250);          
         } else{
            digitalWrite(buzzer,LOW);
          }
   
        c++;    
        estadoSensor = digitalRead(pinSensor); // ler estado do sensor
      } while(!estadoSensor); // quando sensor detectar algo
    
    c = 0;
    
    
      
    if(estadoSensor){ // quando sensor nao detectar
      
      Serial.println("Objeto passou pelo portao. Fechando...");
      tone(buzzer,1200,200);
      delay(400);
      tone(buzzer,1200,200);
      delay(800);
      
      // impulso para o sensor desencostar
      digitalWrite(releEsq,LOW);
      delay(100);

      loopSensor: ;
     do{            
        
        estadoPortao = analogRead(comumSensor);
    
        if(estadoPortao <2){
            digitalWrite(releEsq,LOW);
            delay(100);
            } else{
              goto terminar;
            }
            
    
         /*if(j <= tempoPortao){ // quando o portao fechar completamente
                   
            Serial.println("loop enquanto nao detecta nada");
            digitalWrite(releEsq,LOW);         
          
          }else{
            digitalWrite(releEsq,HIGH);
            Serial.println("aqui ele volta");
            goto terminar;
            }
          
        j++;*/
         estadoSensor = digitalRead(pinSensor); 
      }while(estadoSensor); // enquanto nao detectar      
    }




     
      if(!estadoSensor){ // sensor detectou algo enquanto fechava
       
        Serial.println("algo foi detectado enquanto a porta fechava");
        
          do{
              digitalWrite(releEsq,HIGH);
              estadoSensor = digitalRead(pinSensor);
              Serial.println("loop para manter a porta parada enquanto algo passa");
              
            }while(!estadoSensor); // mantenha porta parada enquanto algo estiver passado.
            
              
                goto loopSensor;
                
                      
        }
      
      terminar: ;
      Serial.println("chegou no fim");
      return 0;
  }
