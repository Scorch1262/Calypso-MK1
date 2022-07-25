/** Includes **************************************************/
#define FCY 32000000UL

#include <xc.h>
#include <assert.h>
#include <stdbool.h>
#include <libpic30.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/fatfs/fatfs.h"
#include <string.h>
#include <stdio.h>
#include "calypso.h"

#if defined(__dsPIC33E__)
	#include <p33exxxx.h>
#elif defined(__dsPIC33F__)
	#include <p33fxxxx.h>
#elif defined(__dsPIC30F__)
	#include <p30fxxxx.h>
#elif defined(__PIC24E__)
	#include <p24exxxx.h>
#elif defined(__PIC24H__)
	#include <p24hxxxx.h>
#elif defined(__PIC24F__) || defined(__PIC24FK__)
	#include <p24fxxxx.h>
#endif

int usart_index = 0;                                                // definiert "usart_index"
char MQTT_Topic2[50];                                               // defienert "MQTT_Topic2"
char MQTT_Topic3[50];                                               // defienert "MQTT_Topic3"
char MQTT_Topic4[50];                                               // defienert "MQTT_Topic4"
char MQTT_Topic5[50];                                               // defienert "MQTT_Topic5"
char MQTT_TopicSys[50];                                             // defienert "MQTT_TopicSys"
char MQTT_TopicRaum[50];                                            // defienert "MQTT_TopicRaum"
char MQTT_TopicSend[50];                                            // defienert "MQTT_TopicSend"
char MQTT_Port[6] = "1883";                                         // definiert "MQTT_Port"
char AT_Data[500];                                                  // definiert "AT_Data"
char Sub_Read[100];                                                 // drfiniert "Sub_Read"
extern char WLAN_ssid[50];                                          // defienert "WLAN_ssid"
extern char WLAN_passw[100];                                        // defienert "WLAN_passw"
extern char MQTT_Address[20];                                       // defienert "MQTT_Address"
extern char MQTT_Topic1[50];                                        // defienert "MQTT_Topic1"
extern char MQTT_Device_Name[50];                                   // definiert "MQTT_Device_Name"

/** Programm **************************************************/

void Calypso_Init(void){                                                // "Calypso_Init"
    Mode0_SetLow();                                                     // setzt "Mode0" auf 0
    Mode1_SetLow();                                                     // setzt "Mode1" auf 0
    Reset_SetHigh();                                                    // setzt "Reset" auf 1
    Wakeup_SetLow();                                                    // setzt "Wakeup" auf 0
    __delay_ms(1000);                                                   // warte 1000ms
    sprintf(AT_Data, "AT+test\r\n");                                    // schreibt "AT+test/r/n" in "AT_Data"
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(100);                                                    // warte 100ms
}                                                                       //

void Calypso_WLAN_init(void){                                           // "Calypso_WLAN_init"
    sprintf(AT_Data, "AT+wlansetmode=STA\r\n");                         // schreibt "AT+wlansetmode=STA/r/n" in "AT_Data"
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(500);                                                    // warte 500ms
    
    sprintf(AT_Data, "AT+wlanset=general,country_code,EU\r\n");         // schreibt "AT+wlanset=general,country_code,EU/r/n" in "AT_Data"
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(500);                                                    // warte 500ms
    
    sprintf(AT_Data, "AT+wlanpolicyset=P2P,client,rand_backoff\r\n");   // schreibt "AT+wlanpolicyset=P2P,client,rand_backoff/r/n" in "AT_Data"
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(500);                                                    // warte 500ms
    
    sprintf(AT_Data, "AT+wlanconnect=%s,,WPA_WPA2,%s,,,\r\n",           // schreibt "AT+wlanconnect=%s,,WPA_WPA2,%s,,,/r/n" in "AT_Data"
            WLAN_ssid,                                                  // 
            WLAN_passw);                                                // 
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(2000);                                                   // warte 2000ms
}                                                                       //

void Calypso_MQTT_init(void){                                           // "Calypso_MQTT_init"
    /*
    sprintf(AT_Data, "AT+MqttDelete=0\r\n");                            // schreibt "AT+MqttDelete=0\r\n" in "AT_Data"
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(500);                                                    // warte 500ms
    */
    sprintf(AT_Data, "AT+MqttCreate=%s,ip4,%s,%s,,,,,,,v3_1,0,0\r\n",   // schreibt "AT+MqttCreate=%s,ip4,%s,%s,,,,,,,v3_1,0,0\r\n" in "AT_Data"
            MQTT_Device_Name,                                           // 
            MQTT_Address,                                               //     
            MQTT_Port);                                                 // 
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(1000);                                                   // warte 1000ms

    sprintf(AT_Data, "AT+MqttDisconnect=0\r\n");                        // schreibt "AT+MqttDisconnect=0\r\n" in "AT_Data"
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(500);                                                    // warte 500ms
 
    sprintf(AT_Data, "AT+MqttConnect=0\r\n");                           // schreibt "AT+MqttConnect=0\r\n" in "AT_Data"
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(5000);                                                   // warte 5000ms
    
}                                                                       // 

void Calypso_MQTT_Read(void){                                           // "Calypso_MQTT_Read"
    uint8_t tempData = 0;                                               // definiert "tempData"

    //TMR1_Stop();                                                        // Stoppt "Timer1"
    /*
    if(UART1_IsRxReady() == true){                                      // wenn "UART1_IsRxReady" gleich "false" ist
        tempData = UART1_Read();                                        // kopiere "UART1_Read" in "tempData"
        if(tempData == '\n'){                                           // wenn "tempData" gleich "\n" ist
            Sub_Read[usart_index] = '\0';                               // schreibt "\0" in "Sub_Read" position "usart_index"
            usart_index = 0;                                            // setzt "usart_index" auf "0"
            Calypso_MQTT_Auswertung();                                  // ruft "Calypso_MQTT_Auswertung" auf
        }else if(tempData != '\r'){                                     // sonst wenn "tempData" nicht gleich "\r" ist
            Sub_Read[usart_index] = tempData;                           // kopiere "tempData" in "Sub_Read" poisition "usart_index"
            usart_index = usart_index + 1;                              // "usart_index" + 1
        }                                                               // 
        if(usart_index == sizeof(Sub_Read)){                            // wenn "usart_index" gleich die größe von "Sub_Read" ist
            usart_index = 0;                                            // schreibt "0" in "usart_index"
        }                                                               //  
    }                                                                   // 
    */
    if(UART2_IsRxReady() == true){                                      // wenn "UART2_IsRxReady" gleich "false" ist
        tempData = UART2_Read();                                        // kopiere "UART2_Read" in "tempData"
        if(tempData == '\n'){                                           // wenn "tempData" gleich "\n" ist
            Sub_Read[usart_index] = '\0';                               // schreibt "\0" in "Sub_Read" position "usart_index"
            usart_index = 0;                                            // setzt "usart_index" auf "0"
            Calypso_MQTT_Auswertung();                                  // ruft "Calypso_MQTT_Auswertung" auf
        }else if(tempData != '\r'){                                     // sonst wenn "tempData" nicht gleich "\r" ist
            Sub_Read[usart_index] = tempData;                           // kopiere "tempData" in "Sub_Read" poisition "usart_index"
            usart_index = usart_index + 1;                              // "usart_index" + 1
        }                                                               // 
        if(usart_index == sizeof(Sub_Read)){                            // wenn "usart_index" gleich die größe von "Sub_Read" ist
            usart_index = 0;                                            // schreibt "0" in "usart_index"
        }                                                               //  
    }                                                                   // 
    //TMR1_Start();                                                       // Startet "Timer1" wieder
}                                                                       // 
    
void Calypso_MQTT_Auswertung(void){                                     // "Calypso_MQTT_Auswertung"
    int position = 0;                                                   // definiert "position"
    int i;                                                              // definiert "i"
    uint8_t ReturnValue = 0;                                            // definiert "ReturnValue"

    if(strstr(Sub_Read, MQTT_Topic1)){                                  // wenn in "MQTT_Topic1" "Sub_Read" steht dann
        position = strlen(Sub_Read) -1;                                 // sucht ende von Sub_Read" und schreibt die Position in "position" 
        ReturnValue = Sub_Read[position];                               // kopiert "strcspn" position "Position" in "ReturnValue"
        switch(ReturnValue){                                            // fragt "ReturnValue" ab                                               |
            case 'A':                                                   // wenn 'A'                                                             |
                LED1_SetHigh();                                         // setzt "LED1" auf 1                                                   | Beispiel
                LED2_SetLow();                                          // setzt "LED2" auf 0                                                   | Anwendung
                break;                                                  // Fertig                                                               |
            case 'B':                                                   // wenn 'B'                                                             |
                LED1_SetLow();                                          // setzt "LED1" auf 0                                                   |
                LED2_SetHigh();                                         // setzt "LED2" auf 1                                                   |
                break;                                                  // Fertig                                                               |
            case 'C':                                                   // wenn 'B'                                                             |
                LED1_SetLow();                                          // setzt "LED1" auf 0                                                   |
                LED2_SetLow();                                          // setzt "LED2" auf 0                                                   |
                break;                                                  // Fertig                                                               |
        }                                                               // 
    }                                                                   // 
    else if(strstr(Sub_Read, MQTT_TopicSys)){                           // wenn in "MQTT_TopicSys" "Sub_Read" steht dann
        position = strlen(Sub_Read) -1;                                 // sucht ende von Sub_Read" und schreibt die Position in "position" 
        ReturnValue = Sub_Read[position];                               // kopiert "strcspn" position "Position" in "ReturnValue"
        if(ReturnValue == '1'){                                         // wenn "ReturnValue" gleich "1" ist
            
        }                                                               // 
    }                                                                   // 
    for(i = 0; i < (sizeof(AT_Data)); i++){                             // solange größe von "AT_Data" kleiner ist als "i"
        AT_Data[i] = '\0';                                              // schreibt "\0" in "AT_Data" positin "i"
    }                                                                   // 
}                                                                       // 

void Calypso_MQTT_write(char MQTT_Topic[50], char MQTT_payload[100]){   // "Calypso_MQTT_write"
    sprintf(AT_Data, "AT+MqttPublish=0,%s,QoS0,1,1,%s\r\n",             // schreibt "AT+MqttPublish=0,%s,QoS0,2,1,%s" in "AT_Data"
            MQTT_Topic,                                                 // 
            MQTT_payload);                                              // 
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(500);                                                    // warte 500ms
}                                                                       // 

void Calypso_MQTT_Sub(char Topic[50]){                                  // "Calypso_MQTT_Sub"
    sprintf(AT_Data, "AT+mqttSubscribe=0,1,%s,QoS0,\r\n",               // schreibt "AT+MqttSubscribe=0,1,%s,QoS0," in "AT_Data"
            Topic);                                                     // 
    Calypso_AT_Send();                                                  // ruft "Calypso_AT_Send" auf
    while(UART2_IsRxReady() == false);                                  // warte solange "UART2_IsRxReady" gleich "false" ist
    __delay_ms(500);                                                    // warte 500ms
}                                                                       // 

void Calypso_AT_Send(void){                                             // "Calypso_AT_Send"    
    int i;                                                              // definiert "i"
    
    for(i = 0; i < (strlen(AT_Data)); i++){                             // solange länge von "AT_Data" kleiner ist als "i"
        UART1_Write(AT_Data[i]);                                        // schreibt "AT_Data" an "UART1" 
        UART2_Write(AT_Data[i]);                                        // schreibt "AT_Data" an "UART2" 
        //UART3_Write(AT_Data[i]);                                        // schreibt "AT_Data" an "UART3" 
    }                                                                   // 
    for(i = 0; i < (sizeof(AT_Data)); i++){                             // solange größe von "AT_Data" kleiner ist als "i"
        AT_Data[i] = '\0';                                              // schreibt "\0" in "AT_Data" positin "i"
    }                                                                   // 
}                                                                       // 