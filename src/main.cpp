
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
  //uint32_t Timer;
}tpBly_Set;
tpBly_Set Bly_Set;

typedef struct  
{
  int Par_F_Start_Slope;
  int Par_F_End_Slope;
  int Par_V_Start;
  int Par_V_Final;
  int Par_F_Run;
  int Par_Slope_T;
  // uint8_t Par_i; //index of param to set/write
  // bool RUN_STOP;
  // bool Set_Settings; //Will Write FLASH by MEM command
  // bool Config; //Will set params with SP command
  //uint32_t Timer;
}tpST32_Set;
tpST32_Set ST32_Set;

String receivedMessage = "";

#pragma region Tasks hande
TaskHandle_t TaskHandle_Config;

// TaskHandle_t TaskHandle_Safe;
// TaskHandle_t TaskHandle_4;
#pragma endregion
// put function declarations here:
#pragma region BLYNK_WRITE
	// "0:Start Slope Freqency",
	// "1:Start Slope Voltage",
	// "2:End Slope Frequency",
	// "3:End Slope Voltage",
	// "4:Final Run Frequency",
	// "5:Slope Time",

BLYNK_WRITE(V0)
{
  Bly_Set.Par_F_Start_Slope=param.asInt();
 if( Bly_Set.init_done==true)
 {
   Serial1.print("SP 0 ");
   Serial1.println(Bly_Set.Par_F_Start_Slope);
 }
}
BLYNK_WRITE(V1)
{
  Bly_Set.Par_V_Start=param.asInt();
   if( Bly_Set.init_done==true)
 {
        Serial1.print("SP 1 ");
        Serial1.println(Bly_Set.Par_V_Start);
 }
}
BLYNK_WRITE(V2)
{
  Bly_Set.Par_F_End_Slope=param.asInt();
     if( Bly_Set.init_done==true)
 {
   Serial1.print("SP 2 ");
   Serial1.println(Bly_Set.Par_F_End_Slope);
 }
}
BLYNK_WRITE(V3)
{
  Bly_Set.Par_V_Final=param.asInt();
      if( Bly_Set.init_done==true)
 {
   Serial1.print("SP 3 ");
   Serial1.println(Bly_Set.Par_V_Final);
 }
}
BLYNK_WRITE(V4)
{
  Bly_Set.Par_F_Run=param.asInt();
  if( Bly_Set.init_done==true)
 {
   Serial1.print("SP 4 ");
   Serial1.println(Bly_Set.Par_F_Run);
 }

}
BLYNK_WRITE(V5)
{
  Bly_Set.Par_Slope_T=param.asInt();
    if( Bly_Set.init_done==true)
 {
   Serial1.print("SP 5 ");
   Serial1.println(Bly_Set.Par_Slope_T);
 }
}
BLYNK_WRITE(V7)
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
        Blynk.syncVirtual(V7);
        Bly_Set.Config=true;
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
	  //Serial1.println("IDLE");
    while (Serial1.available()) {
    char incomingChar = Serial1.read();  // Read each character from the buffer
    if (incomingChar == '\n') {  // Check if the user pressed Enter (new line character)
      // Print the message
      Serial1.print("You sent: ");
      Serial1.println(receivedMessage);
      // Clear the message buffer for the next input
      receivedMessage = "";
    } else 
    {
      // Append the character to the message string
      receivedMessage += incomingChar;
    }
  }
  vTaskDelay(1747);
  }
}

void send_data_to_blynk(void *pvParameters)
{
	while (true)
	{
		Blynk.virtualWrite(V8, receivedMessage);
		vTaskDelay(1000);
	}
}


void Task_Config(void *pvParameters)
{
   // (void)pvParameters;
    while (1) 
    { 
      // Serial1.print("CONF");
      // Serial1.println(Bly_Set.Config);
      // Serial1.println(Bly_Set.Par_i);
      if(Bly_Set.Config==true&&Bly_Set.Par_i<=NUM_OF_PARAMS)
      {
      switch(Bly_Set.Par_i)
      {
      case 0:
        Serial1.print("SP 0 ");
        Serial1.println(Bly_Set.Par_F_Start_Slope);
      break;
      case 1:
        Serial1.print("SP 1 ");
        Serial1.println(Bly_Set.Par_V_Start);
      break;
      case 2:
        Serial1.print("SP 2 ");
        Serial1.println(Bly_Set.Par_F_End_Slope);
      break;
       case 3:
        Serial1.print("SP 3 ");
        Serial1.println(Bly_Set.Par_V_Final);
      break;
       case 4:
        Serial1.print("SP 4 ");
        Serial1.println(Bly_Set.Par_F_Run);
      break;
       case 5:
        Serial1.print("SP 5 ");
        Serial1.println(Bly_Set.Par_Slope_T);
      break;
      }
      Bly_Set.Par_i++;
      }
      if(Bly_Set.Config==true&&Bly_Set.Par_i>NUM_OF_PARAMS)
      {
        Bly_Set.Config=false;
        Bly_Set.init_done=true;
        Bly_Set.Par_i=0;
      }
        vTaskDelay(666);
    }
}

void setup() {
  Serial.begin(57600);
  Serial1.begin(57600, SERIAL_8N1, RXD1, TXD1);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  Bly_Set.Par_i=0;
  #pragma region TasksCreate
  xTaskCreate(run_blynk, "run_blynk", 8192, NULL, 1, NULL);
  //xTaskCreate(send_data_to_blynk, "send_data_to_blynk", 8192, NULL, 1, NULL);
  xTaskCreate(Task_Config, "Task_Config", 8192, NULL, 1, NULL);
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
