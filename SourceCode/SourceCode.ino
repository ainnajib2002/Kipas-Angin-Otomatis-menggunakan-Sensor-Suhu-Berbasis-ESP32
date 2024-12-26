#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Definisi pin dan komponen
#define DHTPIN 5               // Pin sensor DHT pada ESP32 (GPIO5)
#define DHTTYPE DHT11          // Jenis sensor DHT (DHT11 atau DHT22)
#define RELAY_PIN 23           // Relay control pin

DHT dht(DHTPIN, DHTTYPE);      // Inisialisasi objek DHT
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows

void setup() {
  Serial.begin(9600);          // Inisialisasi komunikasi serial

  // Inisialisasi DHT
  dht.begin();

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Inisialisasi pin relay sebagai output
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Pastikan relay off pada awalnya
}

void loop() {
  // Baca suhu dari sensor DHT
  float temperature = dht.readTemperature(); // Membaca suhu dalam Celsius

  // Cek apakah pembacaan berhasil
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error  ");  // Tampilkan error jika pembacaan gagal
    delay(2000);  // Tunggu 2 detik sebelum mencoba membaca lagi
    return;
  }

  // Cetak nilai suhu ke Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.println(" C");

  // Perbarui tampilan LCD dan kontrol relay
  updateLCDAndRelay(temperature);

  delay(1000);  // Tunggu 1 detik sebelum pembacaan berikutnya
}

void updateLCDAndRelay(float temperature) {
  // Tampilkan suhu pada baris pertama LCD
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(temperature);
  lcd.print(" C  ");  // Tambahkan spasi untuk menghapus karakter sebelumnya

  // Logika kontrol relay berdasarkan suhu
  if (temperature >= 27) {  // Jika suhu di atas 30°C
    digitalWrite(RELAY_PIN, LOW); // Hidupkan relay (misalnya pompa aktif)
    lcd.setCursor(0, 1);
    lcd.print("Kipas: ON     "); // Tampilkan status pompa "ON" dengan spasi untuk menghapus karakter sebelumnya
    Serial.println("Relay State: ON"); // Cetak status relay ke Serial Monitor
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Matikan relay (misalnya pompa mati)
    lcd.setCursor(0, 1);
    lcd.print("Kipas: OFF    "); // Tampilkan status pompa "OFF" dengan spasi untuk menghapus karakter sebelumnya
    Serial.println("Relay State: OFF"); // Cetak status relay ke Serial Monitor
  }
}
