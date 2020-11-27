/************** lora_boilerplate ***************
 *    Simple skeleton to interface with a 
 *    Reyax RYLR896 Serial LoRa module
 *          Jon Wallace 2020
 */

#include "secrets.h"
#include "lora.h"

void setup() {
  Serial.begin(115200);
  lora_reset();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  char next = Serial.read();
  process_character(next);
}

void process_response() {
  switch (parse_response()) {
    case RES_READY:
      Serial.print("AT+CPIN=" SECRET "\r\n");
      lora_initialized = true;
      break;
      
    case RES_ERR: print_error(); break;
    case RES_OK: Serial.println("RES_OK"); break;
    
    case RES_RCV:
      process_rcv();

      Serial.print("Incoming: ");
      Serial.print(res_buff);
      Serial.print("\r\n");

      send("AT+SEND=0,4,PONG\r\n");
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      break;
      
    case UNKNOWN_RESPONSE:
      Serial.print("UNKNOWN_RESPONSE: ");
      Serial.print(res_buff);
      break;
      
    default: break;
  }
}