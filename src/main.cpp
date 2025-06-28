
#define BLYNK_TEMPLATE_ID "TMPL4AoCAYy4H"
#define BLYNK_TEMPLATE_NAME "VFD"
#define BLYNK_AUTH_TOKEN "wr_lhdUifRsvWRCYoDbCVqFrtvgyrC7u"

#define TXD1 33  // OK!
#define RXD1 35  // OK!

#include <Arduino.h>
#include <BlynkSimpleEsp32.h> //You need to add it by searching "Blynk" in libraries and install it
#define WIFI_SSID             "Dykonst"
#define WIFI_PASS             "dykonst1991"
#define NUM_OF_PARAMS 5
typedef struct  
{
  int Par_F_Start_Slope;
  int Par_F_End_Slope;
  int Par_V_Start;
  int Par_V_Final;
  int Par_F_Run;
  int Par_Slope_T;
  uint8_t Par_i; //index of param to set/write
  bool RUN_STOP;
  bool init_done;
  bool Set_Settings; //Will Write FLASH by MEM command
  bool Config; //Will set params with SP command
  uint8_t Initialize_Blocker; //index of param to set/write
  //uint32_t Timer;
}tpBly_Set;
tpBly_Set Bly_Set;
bool COM_block=false;
typedef struct  
{
  bool ReadSTM; //Will read STM params
  int Par_F_Start_Slope;
  int Par_F_End_Slope;
  int Par_V_Start;
  int Par_V_Final;
  int Par_F_Run;
  int Par_Slope_T;
  bool Comm_OK;
  uint8_t Par_i; //index of param to set/write
}tpST32_Set;
tpST32_Set ST32_Set;

String receivedMessage = "";
String  SendMessage;
#pragma region Tasks hande
TaskHandle_t TaskHandle_Config;

// TaskHandle_t TaskHandle_Safe;
// TaskHandle_t TaskHandle_4;
#pragma endregion
// put function declarations here:
#pragma region BLYNK_WRITE
BLYNK_WRITE(V0)
{
  Bly_Set.Par_F_Start_Slope=param.asInt();
 if( Bly_Set.init_done==true)
 {
  Serial1.printf("SP 0 %d\r",Bly_Set.Par_F_Start_Slope);
 }
}
BLYNK_WRITE(V1)
{
  Bly_Set.Par_V_Start=param.asInt();
   if( Bly_Set.init_done==true)
 {
  Serial1.printf("SP 1 %d\r",Bly_Set.Par_V_Start);
 }
}
BLYNK_WRITE(V2)
{
  Bly_Set.Par_F_End_Slope=param.asInt();
     if( Bly_Set.init_done==true)
 {
   Serial1.printf("SP 2 %d\r",Bly_Set.Par_F_End_Slope);
 }
}
BLYNK_WRITE(V3)
{
  Bly_Set.Par_V_Final=param.asInt();
      if( Bly_Set.init_done==true)
 {
   Serial1.printf("SP 3 %d\r",Bly_Set.Par_V_Final);
 }
}
BLYNK_WRITE(V4)
{
  Bly_Set.Par_F_Run=param.asInt();
  if( Bly_Set.init_done==true)
 {
   Serial1.printf("SP 4 %d\r",Bly_Set.Par_F_Run);
 }

}
BLYNK_WRITE(V5)
{
  Bly_Set.Par_Slope_T=param.asInt();
    if( Bly_Set.init_done==true)
 {
   Serial1.printf("SP 5 %d\r",Bly_Set.Par_Slope_T);
 }
}
BLYNK_WRITE(V6)
{
      if( Bly_Set.init_done==true)
 {
  if(param.asInt()==0)
  {
  Serial1.println("STP");
  Bly_Set.RUN_STOP=false;
  }
  if(param.asInt()==1)
  {
  Serial1.println("FWD");
  Bly_Set.RUN_STOP=true;
  }
}
}
BLYNK_WRITE(V7)
{
      if( Bly_Set.init_done==true&&Bly_Set.Config==false)
 {
  if(param.asInt()==1)
  {
  Serial.println("READ");
  receivedMessage = ""; 
  ST32_Set.ReadSTM=true;
  ST32_Set.Par_i=0;
  }
}
}
BLYNK_WRITE(V8)
{
      if( Bly_Set.init_done==true)
 {
  if(param.asInt()==1)
  {
  Serial.println("MEM");
  Bly_Set.Set_Settings=true;
  Bly_Set.Config=true;
  Bly_Set.Par_i=0;
  }
}
}
#pragma endregion
 BLYNK_CONNECTED() 
 {     
        Blynk.syncVirtual(V0);
        Blynk.syncVirtual(V1);
        Blynk.syncVirtual(V2);
        Blynk.syncVirtual(V3);
        Blynk.syncVirtual(V4);
        Blynk.syncVirtual(V5);
        Blynk.syncVirtual(V6);
        Bly_Set.Config=true;
        Bly_Set.Par_i=0;
 }
  
void run_blynk(void *pvParameters)
{
	while (true)
	{
		if (WiFi.status() != WL_CONNECTED) 
		{
			Blynk.connectWiFi(WIFI_SSID, WIFI_PASS);
		}
		Blynk.run();

  }
  vTaskDelay(1747);
}

void checker_task(void *pvParameters)
{
	while (true)
	{

    if(Bly_Set.Config==false&&Bly_Set.init_done==true&&ST32_Set.ReadSTM==false)
    {
    Serial1.println("CHK");
    vTaskDelay(200);
    if(Serial1.available())
    {
     while (Serial1.available())
          {
            char incomingChar = Serial1.read();  // Read each character from the buffer
            if (incomingChar == '\n') 
            {  
            //Serial.print("CHK: ");
            Serial.print(receivedMessage.toInt());
            if(receivedMessage.toInt()==83)
            {
            ST32_Set.Comm_OK=true;
            //Serial.println(" PASS");
            }
            else
            {
            ST32_Set.Comm_OK=false;
            }
            receivedMessage = "";   
            } 
            else 
             {
             receivedMessage += incomingChar;
             }
          }
      }
      else
      { 
        ST32_Set.Comm_OK=false;
      }
    Serial.print("CHK ");
    Serial.println(ST32_Set.Comm_OK);
    }
    
     vTaskDelay(1000);
  }
  
}

void send_data_to_blynk(void *pvParameters)
{
	while (true)
	{
		Blynk.virtualWrite(V9, ST32_Set.Comm_OK);
		vTaskDelay(3000);
	}
}


void Task_Config(void *pvParameters)
{ 
    while (1) 
    { 

      if(Bly_Set.Config==true&&Bly_Set.Par_i<=NUM_OF_PARAMS)
      {

        switch(Bly_Set.Par_i)
      {
      case 0:
      if(Bly_Set.Set_Settings==false)
      Serial1.printf("SP 0 %d\r",Bly_Set.Par_F_Start_Slope);
      if(Bly_Set.Set_Settings==true)
      Serial1.printf("MEM 0 %d\r",Bly_Set.Par_F_Start_Slope);
      break;
      case 1:
      if(Bly_Set.Set_Settings==false)
      Serial1.printf("SP 1 %d\r",Bly_Set.Par_V_Start);
      if(Bly_Set.Set_Settings==true)
      Serial1.printf("MEM 1 %d\r",Bly_Set.Par_V_Start);
      break;
      case 2:
      if(Bly_Set.Set_Settings==false)
      Serial1.printf("SP 2 %d\r",Bly_Set.Par_F_End_Slope);
      if(Bly_Set.Set_Settings==true)
      Serial1.printf("MEM 2 %d\r",Bly_Set.Par_F_End_Slope);
      break;
       case 3:
       if(Bly_Set.Set_Settings==false)
      Serial1.printf("SP 3 %d\r",Bly_Set.Par_V_Final);
      if(Bly_Set.Set_Settings==true)
      Serial1.printf("MEM 3 %d\r",Bly_Set.Par_V_Final);
      break;
       case 4:
       if(Bly_Set.Set_Settings==false)
      Serial1.printf("SP 4 %d\r",Bly_Set.Par_F_Run);
      if(Bly_Set.Set_Settings==true)
      Serial1.printf("MEM 4 %d\r",Bly_Set.Par_F_Run);
      
      break;
       case 5:
       if(Bly_Set.Set_Settings==false)
      Serial1.printf("SP 5 %d\r",Bly_Set.Par_Slope_T);
      if(Bly_Set.Set_Settings==true)
      Serial1.printf("MEM 5 %d\r",Bly_Set.Par_Slope_T);
      break;
      }
      Bly_Set.Par_i++;
      }
      if(Bly_Set.Config==true&&Bly_Set.Par_i>NUM_OF_PARAMS)
      {
        Bly_Set.Config=false;
        Bly_Set.Set_Settings=false;
        Bly_Set.init_done=true;
        Bly_Set.Par_i=0;
      }
        vTaskDelay(100);
    }
}
void Task_ReadSTM(void *pvParameters)
{
 // Serial.print("read idle");
    while (1) 
    { 
    //  Serial.print("read run");
      //Serial.println("C0");
      if(ST32_Set.ReadSTM==true&&ST32_Set.Par_i<=NUM_OF_PARAMS)
      { 
      switch(ST32_Set.Par_i)
      {
      case 0:
        Serial1.print("RP 0 0");
        vTaskDelay(200);
        while (Serial1.available())
          {
            char incomingChar = Serial1.read();  // Read each character from the buffer
            if (incomingChar == '\n') 
            {  
            ST32_Set.Par_F_Start_Slope=(receivedMessage.toInt());
            receivedMessage = "";   
            } 
            else 
             {
             receivedMessage += incomingChar;
             }
          }
      break;
      case 1:
        Serial1.print("RP 1 0"); 
        vTaskDelay(200);      
        while (Serial1.available())
          {
            char incomingChar = Serial1.read();  // Read each character from the buffer
            if (incomingChar == '\n') 
            {  
            ST32_Set.Par_V_Start=(receivedMessage.toInt());
            Serial.print("INT1:");
            Serial.println(ST32_Set.Par_V_Start);
            receivedMessage = "";   
            } 
            else 
             {
             receivedMessage += incomingChar;
             }
          }
      break;
      case 2:
        Serial1.print("RP 2 0"); 
        vTaskDelay(200);
        while (Serial1.available())
          {
            char incomingChar = Serial1.read();  // Read each character from the buffer
            if (incomingChar == '\n') 
            {  
            ST32_Set.Par_F_End_Slope=(receivedMessage.toInt());
            Serial.print("INT2:");
            Serial.println(ST32_Set.Par_F_End_Slope);
            receivedMessage = "";   
            } 
            else 
             {
             receivedMessage += incomingChar;
             }
          }
      break;
      case 3:
        Serial1.print("RP 3 0");
        vTaskDelay(200);        
        while (Serial1.available())
          {
            char incomingChar = Serial1.read();  // Read each character from the buffer
            if (incomingChar == '\n') 
            {  
            ST32_Set.Par_V_Final=(receivedMessage.toInt());
            Serial.print("INT3:");
            Serial.println(ST32_Set.Par_V_Final);
            receivedMessage = "";   
            } 
            else 
             {
             receivedMessage += incomingChar;
             }
          }
      break;
      case 4:
        Serial1.print("RP 4 0");  
        vTaskDelay(200);
        while (Serial1.available())
          {
            char incomingChar = Serial1.read();  // Read each character from the buffer
            if (incomingChar == '\n') 
            {  
            ST32_Set.Par_F_Run=(receivedMessage.toInt());
            Serial.print("INT4:");
            Serial.println(ST32_Set.Par_F_Run);
            receivedMessage = "";   
            } 
            else 
             {
             receivedMessage += incomingChar;
             }
          }
      break;
      case 5:
        Serial1.print("RP 5 0");  
        vTaskDelay(200);    
        while (Serial1.available())
          {
            char incomingChar = Serial1.read();  // Read each character from the buffer
            if (incomingChar == '\n') 
            {  
            ST32_Set.Par_Slope_T=(receivedMessage.toInt());
            Serial.print("INT5:");
            Serial.println(ST32_Set.Par_Slope_T);
            receivedMessage = "";   
            } 
            else 
             {
             receivedMessage += incomingChar;
             }
          }
      break;
      }
      ST32_Set.Par_i++;
      }
      if(ST32_Set.ReadSTM==true&&ST32_Set.Par_i>NUM_OF_PARAMS)
      {
        ST32_Set.ReadSTM=false;
        ST32_Set.Par_i=0;
      }
      if(ST32_Set.Par_i==0) vTaskDelay(333);
    }
}

void setup() {
  Serial.begin(57600);
  Serial1.begin(57600, SERIAL_8N1, RXD1, TXD1);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  //Bly_Set.Initialize_Blocker=0;
  #pragma region TasksCreate
  xTaskCreate(send_data_to_blynk, "send_data_to_blynk", 8192, NULL, 1, NULL);
  xTaskCreate(run_blynk, "run_blynk", 8192, NULL, 1, NULL);
  xTaskCreate(checker_task, "checker_task", 1024, NULL, 1, NULL);
  xTaskCreate(Task_Config, "Task_Config", 8192, NULL, 1, NULL);
  xTaskCreate(Task_ReadSTM, "Task_ReadSTM", 8192, NULL, 1, NULL);
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
