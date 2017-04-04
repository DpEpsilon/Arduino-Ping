#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>

#define LED_PIN 2
#define BUZZER_PIN 3
#define INFLATABLE_PIN 4

byte mac[] = { 0xBA, 0xDC, 0x0D, 0xED, 0xEE, 0xE6 };

IPAddress ping_addr(8,8,8,8);
IPAddress ip;
SOCKET ping_socket = 0;

char buffer [256];
ICMPPing ping(ping_socket, (uint16_t)random(0, 255));

unsigned long missed_pings = 0;

void setup() {
  Serial.begin(9600);

  // Start the Ethernet connection:
  Serial.println("Trying to get an IP address using DHCP");
  while (Ethernet.begin(mac) == 0) {}

  // Print your local IP address:
  Serial.print("IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // Print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Pinging: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // Print the value of each byte of the IP address:
    Serial.print(ping_addr[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(INFLATABLE_PIN, OUTPUT);
  digitalWrite(INFLATABLE_PIN, LOW);
}

void loop() {
  ICMPEchoReply echo_reply = ping(ping_addr, 1);
  if (echo_reply.status == SUCCESS) {
    if (missed_pings >= 3) {
      Serial.println("Back online");
      digitalWrite(INFLATABLE_PIN, LOW);
      tone(BUZZER_PIN, 880, 250);
      missed_pings = 0;
    } else {
      Serial.println("Online");
    }
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.print("Offline: ");
    Serial.println(missed_pings+1);
    digitalWrite(LED_PIN, HIGH);
    if (missed_pings == 2) {
      tone(BUZZER_PIN, 440, 250);
      digitalWrite(INFLATABLE_PIN, HIGH);
    }
    missed_pings += 1;
  }
  delay(1000);
}
