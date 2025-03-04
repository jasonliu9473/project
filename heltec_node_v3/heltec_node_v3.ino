#include "LoRaWan_APP.h"

/* OTAA para*/
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0x53, 0xC8 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x74, 0xD6, 0x6E, 0x63, 0x45, 0x82, 0x48, 0x27, 0xFE, 0xC5, 0xB7, 0x70, 0xBA, 0x2B, 0x50, 0x45 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x91,0x5A,0x14,0x90,0x72,0x7B,0x89,0xE6,0x5F,0x34,0x5D,0x4B,0xF1,0x43,0xE3,0xB6 };
uint8_t appSKey[] = { 0xE7,0xC7,0x3A,0xF1,0xB9,0xEF,0xE7,0x00,0x3D,0xE4,0x86,0xA6,0xDE,0xAE,0x6F,0x08 };
uint32_t devAddr =  ( uint32_t )0x260DFA7F;

/*MQTT API-KEY*/
//NNSXS.EJQKCXFBXQYBX3PEN4OCAF2EANKF3LDCNRC3LKA.5MUCJMSGZMOTA5EKVSWBRADNEEDWSZFEOMDNZ2KVAT7IPUR7LNXA

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

//Communication mode
//true  := OTAA
//false := ABP
bool overTheAirActivation = false;

/*ADR enable*/
bool loraWanAdr = true;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = true;

/* Application port */
uint8_t appPort = 1700;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment 
*/
uint8_t confirmedNbTrials = 4;

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
    appDataSize = 4;
    appData[0] = 'C';
    appData[1] = 0x41;
    appData[2] = 0x02;
    appData[3] = 0x03;
}

void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("+REV DATA:%s,RXSIZE %d,PORT %d, RSSI: %d,\n\t SNR: %d,\n\t DATA_RATE:%d,\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port, mcpsIndication->Rssi, (int)mcpsIndication->Snr, (int)mcpsIndication->RxDoneDatarate);
  Serial.print("+REV DATA:");
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++)
  {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
  uint32_t color=mcpsIndication->Buffer[0]<<16|mcpsIndication->Buffer[1]<<8|mcpsIndication->Buffer[2];
#if(LoraWan_RGB==1)
  turnOnRGB(color,5000);
  turnOffRGB();
#endif
}

//if true, next uplink will add MOTE_MAC_DEVICE_TIME_REQ 

RTC_DATA_ATTR bool firstrun = true;

void setup() {
  Serial.begin(115200);
  Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);

  if(firstrun)
  {
    LoRaWAN.displayMcuInit();
    firstrun = false;
  }
}

void loop()
{
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
#if(LORAWAN_DEVEUI_AUTO)
      LoRaWAN.generateDeveuiByChipID();
#endif
      LoRaWAN.init(loraWanClass,loraWanRegion);
      //both set join DR and DR when ADR off 
      LoRaWAN.setDefaultDR(3);
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.displayJoining();
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      LoRaWAN.displaySending();
      prepareTxFrame( appPort );
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.displayAck();
      LoRaWAN.sleep(loraWanClass);
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}