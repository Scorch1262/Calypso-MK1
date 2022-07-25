/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ128GB204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#define FCY 32000000UL 
#include <assert.h>
#include <stdbool.h>
#include "mcc_generated_files/system.h"
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include <xc.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/fatfs/fatfs.h"
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

char WLAN_ssid[50] = "ssid";                // defienert "WLAN_ssid"
char WLAN_passw[100] = "kennwort";                 // defienert "WLAN_passw"
char MQTT_Address[20] = "192.168.178.20";                // defienert "MQTT_Address"
char MQTT_Topic1[50] = "test/versuch";                   // defienert "MQTT_Topic1"
char MQTT_Device_Name[50] = "Calypso";     // definiert "MQTT_Device_Name"

/*
                         Main application
 */
int main(void){                                                 // "main"
    SYSTEM_Initialize();                                        // initialize the device
    __delay_ms(500);                                            // warte 500ms
    Calypso_Init();                                             // ruft "Calypso_Init" auf
    Calypso_WLAN_init();                                        // ruft "Calypso_WLAN_init" auf
    
    Calypso_MQTT_init();                                        // ruft "Calypso_MQTT_init" auf
    Calypso_MQTT_Sub(MQTT_Topic1);                              // ruft "Calypso_MQTT_Sub" mit "MQTT_Topic1" auf
    
    while(1){                                                   // Endlosschleife
        Calypso_MQTT_Read();                                    // ruft "Calypso_MQTT_Read" auf
        if(IN1_GetValue()){                                     // fragt "IN1" ab
            Calypso_MQTT_write(MQTT_Topic1, "a");
        }else if(IN2_GetValue()){                               // fragt "IN2" ab
            Calypso_MQTT_write(MQTT_Topic1, "b");
        }else if(IN3_GetValue()){                               // fragt "IN3" ab
            Calypso_MQTT_write(MQTT_Topic1, "c");
        }                                                       //
    }                                                           // 
    return 1;                                                   // 
}                                                               // 
/**
 End of File
*/
