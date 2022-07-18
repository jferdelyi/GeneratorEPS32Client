#include <Adafruit_SSD1306.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>

// The BOOT button
#define BTN_BOOT 0

// Secrets
#define SECRET_SSID "xxxx"
#define SECRET_PASS "xxxx"

// The display
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32);

// Server data client
String IP = String("x.x.x.x");
String PORT = String("4242");
String VERSION = String("v1");
String GENERATE = String("generate");

// Connection data
WiFiClient wifi;
HttpClient client = HttpClient(wifi, IP, 4242);
bool requested = false;

void initScreen() {
	// Address 0x3C for 128x32
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0,0);
	display.clearDisplay();
}

void initWiFi() {
	// Set mode and disconnect previous connection
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	// New connection
	WiFi.begin(SECRET_SSID, SECRET_PASS);
	display.print("Conection to ");
	display.println(SECRET_SSID);
	display.display();
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
	}
	display.println(WiFi.localIP());
	display.display();
}

void setup() {
	// Init screen
	initScreen();

	// Init WiFi
	initWiFi();
}

void loop() {
	// If the button BOOT is pressed
	if(!requested && !digitalRead(BTN_BOOT)) {
		// Block requests
		requested=true;

		// Init screen
		display.clearDisplay();
		display.setCursor(0,0);

		// GET
  		client.get("/" + VERSION + "/" + GENERATE);
		int statusCode = client.responseStatusCode();
		if(statusCode == 200) {
			String response = client.responseBody();
			display.println(response);
		} else {
			display.print("Error code ");
			display.println(statusCode);
		}

		// Release
		requested=false;
	}

	// Wait a bit
	delay(10);
  	yield();
  	display.display();
}
