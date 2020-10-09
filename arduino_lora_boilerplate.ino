/************** lora_boilerplate ***************
 *    Simple skeleton to interface with a 
 *    Reyax RYLR896 Serial LoRa module
 *          Jon Wallace 2020
 */

#define MAX_CMD (8+5+1+3+1+240+1+3+1+2+2)
char cmd_buff[MAX_CMD]; // Accomodate a maximum length command (Recieve 240 bytes)
int  cmd_i = 0;

void setup() {
  Serial.begin(115200); 
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  char next = Serial.read();
  switch (next) {
  case -1: break; // Nothing to read
  case 10: // Line feed
    cmd_buff[cmd_i++] = next;
    cmd_buff[cmd_i++] = '\0'; // Add a null termination for easy printing if need be
    process_response();
    break;
  case '+':
    cmd_i = 0;
    // fall through
  default:
    cmd_buff[cmd_i++] = next;
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

void process_response() {
  switch (parse_response()) {
    case RES_READY:
      Serial.print("AT+SEND=0,6,ONLINE\r\n");
      break;
    case RES_ERR:
      print_error();
      break;
    case RES_OK:
      Serial.println("RES_OK");
      break;
    case RES_RCV:
      Serial.print("AT+SEND=0,4,PONG\r\n");
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      break;
    case UNKNOWN_RESPONSE:
      Serial.print("UNKNOWN_RESPONSE: ");
      Serial.print(cmd_buff);
      break;
    default:
      break;
  }
}

int parse_response() {
  int res;
  for (res = 0; res < N_RES; res++) { // for each known response string
    for (int i = 0; i < cmd_i; i++) { // for each character in the response buffer
      if (RES_STRINGS[res][i] == '\0') {
        // Got to end of a known response string, must be a match
        goto parse_response_match;
      }
      if (RES_STRINGS[res][i] != cmd_buff[i]) {
        //This aint it chief!
        break; // go to next response string
      }
    }
  }
  // Went through all response strings without a match
  return UNKNOWN_RESPONSE;
  
parse_response_match:
  return res;
}

void print_error() {
  int err;
  char a = cmd_buff[5];
  if (a >= '0' && a <= '9') err = a - '0';
  else err = -1;
  a = cmd_buff[6];
  if (a >= '0' && a <= '9') err = 10*err + (a - '0');
  switch(err) {
    default:
    case -1: // Couldn't parse the error number
      Serial.print("Error: ");
      Serial.print(cmd_buff); // Just throw it back
      break;
    case 1:
      // Printing this one generates itself :)
      // Serial.println("There is not \"enter\" or 0x0D 0x0A in the end of the AT Command.");
      break;
    case 2:
      // Good idea to suppress this one since everything you print generates it
      // Serial.println("The head of AT command is not \"AT\" string.");
      break;
    case 3: Serial.println("Error: There is not \"=\" symbol in the AT command."); break;
    case 4: Serial.println("Error: Unknow command."); break;
    case 10: Serial.println("Error: TX is over times."); break;
    case 11: Serial.println("Error: RX is over times."); break;
    case 12: Serial.println("Error: CRC error."); break;
    case 13: Serial.println("Error: TX data more than 240bytes."); break;
    case 15: Serial.println("Error: Unknow error."); break;
  }
}
