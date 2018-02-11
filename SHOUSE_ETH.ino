#include <SPI.h>
#include <Ethernet.h>

//////////////////////////////////////////////////////

// Arduino ID also used as an user to login to website
char MyID[] = "aaaa";                       //id of the station

//////////////////////////////////////////////////////
int volts1 = A1;                     // Volts pin 1         
int volts2 = A0;                     // Volts pin 0
int ledPin = 13;
int fan1 = 5;                           // fan control if needed
float tempC1;                           //Temperature sensor 1
float tempC2;                           //Temperature sensor 1


byte mac[] = {  0x00, 0xAB, 0xCB, 0xCD, 0xDE, 0x02 };
IPAddress ip(192, 168, 0, 140);
//IPAddress server(82, 28, 186, 219);// numeric IP (no DNS)
char server[] = "www.servers.vxm.pl";    // (using DNS)

EthernetClient client;

void connect() {
  tempC1 = analogRead(volts1);
  tempC1 = (tempC1 * 100.0 / 1023) + 5;           //  tempC1 = (tempC1 - 0.5 )* 100.0 / 1024.0; 


  tempC2 = analogRead(volts2);
  tempC2 = (tempC2 * 100.0 / 1023) + 5 ;          // tempC2 = (tempC2 - 0.5 )* 100.0 / 1024.0

  if (client.connect(server, 80)) {
    client.print("GET /sh/recive_data.php");
    client.print("?un=");
    client.print(MyID);                               //Post The Serial Number
    client.print("&ti=");
    client.print(tempC1);                           //Post The Temperature
    client.print("&to=");
    client.print(tempC2);
    client.println(" HTTP/1.0");
    client.println("HOST: www.servers.vxm.pl");
    client.println();
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }

}

void setup() {
  Serial.begin(9600);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

}

void loop() {
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
    // Serial.println("");
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    client.stop();
    delay(6000);
    connect();
  }

}

