/***************************************************** 
* Universidade de Fortaleza 
* Engenharia de Controle e Automaçao
* TCC2- Prototipo de Medidor Inteligente para uso Residencial
* Autor: Jefferson Acioly França Jorge
* 
* Sensore e módulos
*   Sensor de Corrente não invasivo SCT-013-030
*   Circuito para medição te Tensão
*   Módulo RTC DS3231
*   
* FREEZE_LED:       ==> Pin 13 (ESP-01 Freezing and Comm errors)
* HW RESET          ==> Pin 08
*     
*****************************************************/
#include <SoftwareSerial.h>  //Biblioteca para criação de interface serial
#include "EmonLib.h"         //Biblioteca para manipulaçao do sensor de corrente
#include <Wire.h>            //Biblioteca para manipulação do protocolo I2C
#include <DS3231.h>          //Biblioteca para manipulação do DS3231


// Thingspeak  
String Chave_de_Escrita = "9SIMK6M2YV38E789";  // Status Channel id: 385184
// SoftwareSerial
SoftwareSerial ESP(4, 3); // Rx,  Tx
EnergyMonitor emon1;     //Criaçao do objeto do tipo EnergyMonitor
DS3231 rtc;              //Criação do objeto do tipo DS3231
RTCDateTime dataehora;   //Criação do objeto do tipo RTCDateTime


// HW Pins
#define FREEZE_LED 13
#define HARDWARE_RESET 8

//Variaveis de tensao e corrente.
int pin_ten = 0; 
int pin_sct = 2;
float Irms;
float realPower;
float apparentPower;    //extract Apparent Power into variable
//double powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
float supplyVoltage; 

//Variaveis de jogo de led's para inicializaçao do programa.
int led_1=5;
int led_2=6;
int led_3=7;

//Variaveis do RTC.
unsigned char hora_1 = 6;
unsigned char minuto_1 = 00;
unsigned char hora_2 = 15;
unsigned char minuto_2 = 30;
int alerta;

// Variables to be used with timers
long Tempo_Entre_Escrita = 15000; // ==> Tempo entre escrita no Thing Speak
long Tempo_Inicial_Escrita = 0;
long Tempo_Decorrido_Escrita = 0;
int A;
int spare = 0;
boolean error;
//Variavies de consumo  e total.
float consumo=0;
float total=0;
void setup()
{
  Serial.begin(9600); 
  digitalWrite(FREEZE_LED, LOW);
  digitalWrite(HARDWARE_RESET, HIGH);
  
  ESP.begin(9600); // Comunicacao com Modulo WiFi
  ESP_Resete(); //Resete do Modulo WiFi
  
  connectWiFi();
  rtc.begin();//Inicialização do RTC DS3231    
  emon1.current(pin_sct, 29);//Pino, Calibração - Corrente Const= Ratio/Res. Burder. 1800/62 = 29.
  emon1.voltage(pin_ten,219.25, 1.7);// Pino, Calibração - phase_shift
  Tempo_Inicial_Escrita = millis(); // inicia a contagem do tempo. 
  inicializacao();//Funçao que chama jogo de led's para iniciar o programa.
  }

void loop()
{
  start: //label 
  error=0;
  
  
  Tempo_Decorrido_Escrita = millis()-Tempo_Inicial_Escrita; 
  
  if (Tempo_Decorrido_Escrita > (Tempo_Entre_Escrita)) 
  {
    ESPcheck();//executar antes de qualquer leitura ou gravação
    Leitura_Sensores();
    Escrever_ThingSpeak();
    Tempo_Inicial_Escrita = millis();   
  }
  
  if (error==1) //Renicia a transmissão se não for completada  
  {       
    Serial.println(" <<<< ERROR >>>>");
    digitalWrite(FREEZE_LED, HIGH);
    delay (2000);  
    goto start; 
  }
}

/********* Leitura de Sensores *************/
void Leitura_Sensores(void)
{
  sensor_corrente();
  sensor_tensao();
  apparentPower = emon1.apparentPower;
  custo();
  RTC();
}

/********* Conexao com TCP com Thingspeak *******/
void Escrever_ThingSpeak(void)
{

  Inicia_Cmd_ThingSpeak();

  // preparacao da string GET
  String getStr = "GET /update?api_key=";
  getStr += Chave_de_Escrita;
  getStr +="&field1=";
  getStr += apparentPower ;
  getStr +="&field2=";
  getStr += supplyVoltage;
  getStr +="&field3=";
  getStr += Irms;
  getStr +="&field4=";
  getStr += alerta;
  getStr +="&field5=";
  getStr += consumo;
  getStr +="&field6=";
  getStr += total;
  getStr += "\r\n\r\n";  

 Enviar_Cmd_ThingSpeak(getStr);
}

/********* Reset ESP *************/
void ESP_Resete(void)
{
  Serial.println("Reseting......."); 
  digitalWrite(HARDWARE_RESET, LOW); 
  delay(500);
  digitalWrite(HARDWARE_RESET, HIGH);
  delay(2000);//Tempo necessário para começar a ler 
  Serial.println("RESET"); 
}

/********* Check ESP *************/
boolean ESPcheck(void)
{
  ESP.println("AT"); 
   
  if (ESP.find("OK")) 
  {
    Serial.println("ESP ok");
    digitalWrite(FREEZE_LED, LOW);
    return true; 
  }
  else //Freeze ou Busy
  {
    Serial.println("ESP Freeze ***********");
    digitalWrite(FREEZE_LED, HIGH);
    ESP_Resete();
    return false;  
  }
}

/********* Start communication with ThingSpeak*************/
void Inicia_Cmd_ThingSpeak(void)
{
  ESP.flush();
  
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149";
  cmd += "\",80";
  ESP.println(cmd);
  Serial.print("enviado ==> Start cmd: ");
  Serial.println(cmd);

  if(ESP.find("ERROR"))
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
}

/********* send a GET cmd to ThingSpeak *************/
String Enviar_Cmd_ThingSpeak(String getStr)
{
  String cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ESP.println(cmd);
  Serial.print("enviado ==> lenght cmd: ");
  Serial.println(cmd);

  if(ESP.find((char *)">"))
  {
    ESP.print(getStr);
    Serial.print("enviado ==> getStr: ");
    Serial.println(getStr);
    delay(500);//tempo para processar o GET, sem este delay apresenta busy no próximo comando

    String mensagem = "";
    while (ESP.available()) 
    {
      String line = ESP.readStringUntil('\n');
      if (line.length() == 1) 
     {  //actual content starts after empty line (that has length 1)
        mensagem = ESP.readStringUntil('\n');
      }
    }
    Serial.print("mensagem recebida: ");
    Serial.println(mensagem);
    return mensagem;
  }
  else
  {
    ESP.println("AT+CIPCLOSE");     // alert user
    Serial.println("ESP8266 CIPSEND ERROR: RESENDING"); //Resend...
    spare = spare + 1;
    error=1;
    return "error";
  } 
}

/***************************************************
* Connect WiFi
****************************************************/
void connectWiFi(void)
{
  sendData("AT+RST\r\n", 2000, 0);
  sendData("AT+CWJAP=\"TAVARES\",\"yoshi123\"\r\n", 2000, 0);
  delay(3000);
  sendData("AT+CWMODE=1\r\n", 1000, 0);
  sendData("AT+CIPMUX=0\r\n", 1000, 0);
  sendData("AT+CIFSR\r\n", 1000, 0);
  Serial.println("8266 Connected");
}

/***************************************************
* Send AT commands to module
****************************************************/

String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  ESP.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (ESP.available())
    {
      // The esp has data so display its output to the serial window
      char c = ESP.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}

void sensor_corrente(){
  Irms = emon1.calcIrms(1480);
   
  if ( Irms <0.10||Irms==0.10){
  Irms = 0;
  return;
  }
}
void sensor_tensao(){
 emon1.calcVI(20,2000);
 emon1.serialprint(); 
 supplyVoltage = emon1.Vrms;   
}
void custo(){
  
   consumo += ((apparentPower*15)/3600);
    total+=(consumo/1000)*0.70163;   
  }
void inicializacao(){
  pinMode(led_1,OUTPUT); 
  pinMode(led_2,OUTPUT);
  pinMode(led_3,OUTPUT);
  Serial.println("Aguarde Calibração dos Sensores.");
  digitalWrite(led_1,HIGH);
  delay(7500);
  digitalWrite(led_2,HIGH);
  delay(7500);
  digitalWrite(led_3,HIGH);
  int somador=0;
  while(somador < 3){
    Irms = emon1.calcIrms(1480);
    somador++;
  }
}
void RTC(){
  dataehora = rtc.getDateTime();
  Serial.print(dataehora.year);     //Imprimindo o Ano   
  Serial.print("-");
  Serial.print(dataehora.month);    //Imprimindo o Mês
  Serial.print("-");
  Serial.print(dataehora.day);      //Imprimindo o Dia
  Serial.print(" ");
  Serial.print(dataehora.hour);     //Imprimindo a Hora
  Serial.print(":");
  Serial.print(dataehora.minute);   //Imprimindo o Minuto
  Serial.print(":");
  Serial.print(dataehora.second);   //Imprimindo o Segundo
  Serial.println("");
  if ((hora_1 == dataehora.hour)&&(minuto_1 == dataehora.minute)){
  alerta=1;
  }else{
    alerta=0;
  }
  if ((hora_2 == dataehora.hour)&&(minuto_2 == dataehora.minute)){
  alerta=1;
  }else{
    alerta=0;
  }
}
