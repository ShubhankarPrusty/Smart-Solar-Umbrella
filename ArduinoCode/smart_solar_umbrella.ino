#define BLYNK_TEMPLATE_ID "TMPL3gbs5a2g-"
#define BLYNK_TEMPLATE_NAME "Temperature and Humidity Monitor"
#define BLYNK_AUTH_TOKEN "cQRFTbm4gBY6qUbl7OlbNbiGkU1YdBk4"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>





// Define Sensor Pins
#define DHTPIN 4       // DHT22 Data pin
#define DHTTYPE DHT22  // DHT22 sensor type
#define MQ135_PIN 34
#define BATTERY_PIN 35 // ADC GPIO Pin


// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C  // Default OLED I2C Address


// WiFi credentials
char ssid[] = "amit";
char pass[] = "123456789x";



// Initialize OLED Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize DHT Sensor
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN , ssid, pass);
  dht.begin();
  delay(2000);

  // Initialize OLED Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 OLED allocation failed");
    for (;;); // Stop execution if OLED fails
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}



void loop() {
  // Read Temperature and Humidity from DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read Air Quality from MQ135
  int airQuality=analogRead(MQ135_PIN)/15 + 97;
//   int adcValue = analogRead(BATTERY_PIN); // Read ADC
//   float voltage = (adcValue / 4095.0) * vRef; // Convert ADC to voltage
//   float batteryVoltage = voltage * ((R1 + R2) / R2); // Reverse voltage divider

//   // Convert to battery percentage
// int batteryPercentage = map(batteryVoltage * 100, 300, 420, 0, 100);
// batteryPercentage = constrain(batteryPercentage, 0, 100);

int adcReading = analogRead(BATTERY_PIN);
  
  // Convert ADC value to voltage
  float vOut = adcReading * (3.3 / 4095.0);  // ESP32 ADC reference voltage is 3.3V
  float batteryVoltage = vOut * (751/549) ;  // Reverse voltage divider

  // Print battery voltage
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.println("V");

  // Estimate battery percentage (Assuming 3.0V = 0% and 4.0V = 100%)
  int batteryPercentage = map(batteryVoltage * 100, 300, 420, 0, 100);
  batteryPercentage = constrain(batteryPercentage, 0, 100);

  Serial.print("Battery Charge: ");
  Serial.print(batteryPercentage);
  Serial.println("%");

Serial.print("Battery Voltage: ");
Serial.print(batteryVoltage);
Serial.print("V | Charge: ");
Serial.print(batteryPercentage);
Serial.println("%");

// Clear display before updating new values
display.clearDisplay();
  
  // Display Temperature
  display.setCursor(10, 10);
  display.print("Temperature:");
  display.print(temperature);
  display.print("`C");

  // Display Humidity
  display.setCursor(10, 25);
  display.print("Humidity: ");
  display.print(humidity);
  display.print(" %");

  // Display Air Quality
  display.setCursor(10, 40);
  display.print("Air Quality: ");
  display.print(airQuality);

  // Displat Battery Percentage
  display.setCursor(10,55);
  display.print("Charge: ");
  display.print(batteryPercentage);
  display.print("%");

  // Update OLED Display
  display.display();

  // Print values to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C  Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Air Quality: ");
  Serial.println(airQuality);
  

  Blynk.run();
  
  

  // Send data to Blynk
  Blynk.virtualWrite(V0, temperature); // Temperature to V0
  Blynk.virtualWrite(V1, humidity);  // Humidity to V1
  Blynk.virtualWrite(V2, airQuality); // Air Quality to V2
  Blynk.virtualWrite(V3, batteryPercentage);
  


  delay(2000); // Wait before next update
}
