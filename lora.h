#ifndef LORA_H
#define LORA_H

// Accomodate a maximum length response (Recieve 240 bytes)
#define MAX_RES_SIZE (270)

// Response buffer, holds characters coming from the LoRa modules
char res_buff[MAX_RES_SIZE];

// global index for the response buffer is needed since input is processed one byte per loop()
int  res_i = 0;

// True if the LoRa module is initialized
bool lora_initialized;

// Populated by process_rcv() in the RES_RCV case
int rcv_addr;
int rcv_data_len;
char *rcv_data; // Warning: Not null terminated! Use rcv_data_len!
int rcv_rssi;
int rcv_snr;

// User funtions
void lora_reset();
void send(char* data);
// Boilerplate functions
void process_character(char next);
void process_response();
int parse_response();
void process_rcv();
void print_error();

void lora_reset() {
  Serial.print("AT+RESET\r\n");
  lora_initialized = false;
}

void send(char* data) {
  if (lora_initialized) {
    Serial.print(data);
  }
}

void process_character(char next) {
  switch (next) {
  case -1: break; // Nothing to read
  
  case 10: // Line feed
    res_buff[res_i++] = next;
    res_buff[res_i++] = '\0'; // Add a null termination for easy printing if need be
    process_response();
    break;
    
  case '+': res_i = 0;
    // fall through
  default: res_buff[res_i++] = next;
    break;
  }
}

#define UNKNOWN_RESPONSE -1
#define RES_READY 0
#define RES_ERR 1
#define RES_OK 2
#define RES_RCV 3
#define N_RES 4
const char *RES_STRINGS[N_RES] = {"+READY", "+ERR=", "+OK", "+RCV="};
#define RES_ERR_OFFSET 5
#define RES_RCV_OFFSET 5

int parse_response() {
  int res;
  for (res = 0; res < N_RES; res++) { // for each known response string
    for (int i = 0; i < res_i; i++) { // for each character in the response buffer
      if (RES_STRINGS[res][i] == '\0') goto parse_response_match;
      if (RES_STRINGS[res][i] != res_buff[i]) break;
    }
  }
  // Went through all response strings without a match
  return UNKNOWN_RESPONSE;
parse_response_match:
  return res;
}

void process_rcv() {
  int rcv_data_offset;
  sscanf(res_buff + RES_RCV_OFFSET, "%d,%d,%n", &rcv_addr, &rcv_data_len, &rcv_data_offset);
  rcv_data_offset += RES_RCV_OFFSET;
  sscanf(res_buff + rcv_data_offset + rcv_data_len, ",%d,%d", &rcv_rssi, &rcv_snr);
  rcv_data = res_buff + rcv_data_offset;
}

void print_error() {
  int err;
  sscanf(res_buff + RES_ERR_OFFSET, "%d", &err);
  switch(err) {
    default:
    case -1: // Couldn't parse the error number
      Serial.print("Error: ");
      Serial.print(res_buff); // Just throw it back
      break;
    // Errors 1&2 must be supressed since they are sent back any time Serial.println is used
    case 1: break; // Serial.println("There is not \"enter\" or 0x0D 0x0A in the end of the AT Command.");
    case 2: break; // Serial.println("The head of AT command is not \"AT\" string.");
    case 3: Serial.println("Error: There is not \"=\" symbol in the AT command."); break;
    case 4: Serial.println("Error: Unknow command."); break;
    case 10: Serial.println("Error: TX is over times."); break;
    case 11: Serial.println("Error: RX is over times."); break;
    case 12: Serial.println("Error: CRC error."); break;
    case 13: Serial.println("Error: TX data more than 240bytes."); break;
    case 15: Serial.println("Error: Unknow error."); break;
  }
}

#endif