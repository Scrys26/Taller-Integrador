/**
 * display.cpp — Implementación Display (Adafruit SSD1306 128x64)
 */

#include "display.h"
#include "config.h"

#if OLED_ENABLED
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  static Adafruit_SSD1306 oled(128, 64, &Wire, OLED_RST);
  static bool oledOk = false;

  static void clear() { oled.clearDisplay(); }
  static void show()  { oled.display(); }
  static void header(const char* title) {
      oled.setTextSize(1);
      oled.setTextColor(SSD1306_WHITE);
      oled.setCursor(0, 0);
      oled.println(title);
      oled.drawLine(0, 9, 127, 9, SSD1306_WHITE);
      oled.setCursor(0, 12);
  }
#endif

namespace Display {

void init() {
#if OLED_ENABLED
    if (OLED_RST >= 0) {
        pinMode(OLED_RST, OUTPUT);
        digitalWrite(OLED_RST, LOW);
        delay(20);
        digitalWrite(OLED_RST, HIGH);
    }
    Wire.begin(OLED_SDA, OLED_SCL);
    oledOk = oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    if (!oledOk) {
        Serial.println(F("[Display] OLED no encontrado — continuando sin pantalla"));
        return;
    }
    oled.setTextWrap(true);
    Serial.println(F("[Display] OLED OK"));
#endif
}

void showBoot() {
#if OLED_ENABLED
    if (!oledOk) return;
    clear();
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(10, 8);  oled.println("iGate");
    oled.setCursor(4, 30);  oled.println("APRS-IS");
    oled.setTextSize(1);
    oled.setCursor(0, 54);  oled.println("TEC — Firmware Propio");
    show();
    delay(2000);
#endif
}

void showState(SystemState s) {
#if OLED_ENABLED
    if (!oledOk) return;
    clear();
    header("== iGate FSM ==");
    oled.setTextSize(2);
    oled.setCursor(0, 14);
    oled.println(stateName(s));
    show();
#endif
}

void showRx(const String& packet, bool valid) {
#if OLED_ENABLED
    if (!oledOk) return;
    clear();
    header(valid ? "RX VALID" : "RX INVALID");
    oled.setTextSize(1);
    // Mostrar primeros 64 chars del paquete
    oled.println(packet.substring(0, 21));
    oled.println(packet.substring(21, 42));
    oled.println(packet.substring(42, 63));
    show();
#endif
}

void showUplink(const String& pkt) {
#if OLED_ENABLED
    if (!oledOk) return;
    clear();
    header("TX → APRS-IS");
    oled.setTextSize(1);
    oled.println(pkt.substring(0, 21));
    oled.println(pkt.substring(21, 42));
    oled.println("OK");
    show();
#endif
}

void showBeacon() {
#if OLED_ENABLED
    if (!oledOk) return;
    clear();
    header("BEACON TX");
    oled.setTextSize(1);
    oled.println("Beacon enviado");
    oled.println("al servidor APRS-IS");
    show();
#endif
}

void showError(uint8_t retries) {
#if OLED_ENABLED
    if (!oledOk) return;
    clear();
    header("!! ERROR !!");
    oled.setTextSize(1);
    oled.print("Intento: ");
    oled.println(retries);
    oled.println("Recuperando...");
    show();
#endif
}

} // namespace Display
