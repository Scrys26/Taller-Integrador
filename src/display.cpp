// display.cpp — Dashboard OLED SSD1306 128x64

#include "display.h"
#include "config.h"

#if OLED_ENABLED
  #include <Wire.h>
  #include <WiFi.h>
  #include "aprs_is_client.h"
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  static Adafruit_SSD1306 oled(128, 64, &Wire, OLED_RST);
  static bool oledOk = false;

  static SystemState gState     = STATE_START;
  static uint32_t   gRxCount   = 0;
  static uint32_t   gTxCount   = 0;
  static String     gLastSource = "---";
  static String     gStatus    = "BOOT";
  static uint8_t    gErrRetries = 0;

  static void clear() { oled.clearDisplay(); }
  static void show()  { oled.display(); }

  static String extractSource(const String& packet) {
      int gt = packet.indexOf('>');
      if (gt > 0) return packet.substring(0, gt);
      return "---";
  }

  static String trimTo(const String& s, size_t maxLen) {
      if (s.length() <= maxLen) return s;
      if (maxLen < 4) return s.substring(0, maxLen);
      return s.substring(0, maxLen - 3) + "...";
  }

  static int centerX(const String& text, uint8_t size) {
      int16_t x1, y1;
      uint16_t w, h;
      oled.setTextSize(size);
      oled.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
      int x = (128 - (int)w) / 2;
      return (x < 0) ? 0 : x;
  }

  static String wifiInfo() {
      if (WiFi.status() != WL_CONNECTED) return "OFF";
      String ssid = String(WIFI_SSID);
      int spacePos = ssid.indexOf(' ');
      if (spacePos > 0) ssid = ssid.substring(0, spacePos);
      return ssid;
  }

  static String ipInfo() {
      if (WiFi.status() != WL_CONNECTED) return "--";
      return WiFi.localIP().toString();
  }

  static void renderDashboard() {
      if (!oledOk) return;

      clear();
      oled.setTextWrap(false);
      oled.setTextColor(SSD1306_WHITE);

      // Título
      oled.setTextSize(2);
      String title = trimTo(String(CALLSIGN), 10);
      oled.setCursor(centerX(title, 2), 0);
      oled.println(title);

      oled.drawLine(0, 18, 127, 18, SSD1306_WHITE);
      oled.setTextSize(1);

      // WiFi y estado APRS
      oled.setCursor(0, 21);
      oled.print("WiFi: ");
      oled.print(trimTo(wifiInfo(), 10));
      oled.setCursor(86, 21);
      oled.print("APRS:");
      oled.print(APRSISClient::isConnected() ? "ON" : "OFF");

      // IP
      oled.setCursor(0, 30);
      oled.print("IP: ");
      oled.print(trimTo(ipInfo(), 15));

      // LoRa y contadores
      oled.setCursor(0, 39);
      oled.print("LoRa: OK");
      oled.setCursor(58, 39);
      oled.print("RX:");
      oled.print(gRxCount);
      oled.setCursor(96, 39);
      oled.print("TX:");
      oled.print(gTxCount);

      // Última estación recibida
      oled.setCursor(0, 48);
      oled.print("Lst: ");
      oled.print(trimTo(gLastSource, 12));

      // Barra de estado
      oled.fillRect(0, 56, 128, 8, SSD1306_WHITE);
      oled.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      oled.setTextSize(1);
      String bar = trimTo(gStatus, 20);
      oled.setCursor(centerX(bar, 1), 57);
      oled.print(bar);

      oled.setTextColor(SSD1306_WHITE);
      show();
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

    oled.clearDisplay();
    oled.setTextWrap(false);
    oled.display();
    Serial.println(F("[Display] OLED OK"));
#endif
}

void showBoot() {
#if OLED_ENABLED
    if (!oledOk) return;

    clear();
    oled.setTextColor(SSD1306_WHITE);

    oled.setTextSize(2);
    oled.setCursor(centerX("iGate", 2), 8);
    oled.println("iGate");

    oled.setTextSize(1);
    oled.setCursor(centerX("LoRa -> APRS-IS", 1), 34);
    oled.println("LoRa -> APRS-IS");
    oled.setCursor(centerX(String(CALLSIGN), 1), 50);
    oled.println(CALLSIGN);

    show();
    delay(1200);

    gStatus = "LISTENING...";
    renderDashboard();
#endif
}

void showState(SystemState s) {
#if OLED_ENABLED
    if (!oledOk) return;
    gState = s;
    switch (s) {
        case STATE_START:     gStatus = "STARTING";     break;
        case STATE_LISTEN:    gStatus = "LISTENING..."; break;
        case STATE_DECODE:    gStatus = "DECODING";     break;
        case STATE_UPLINK:    gStatus = "UPLINK";       break;
        case STATE_BEACON_TX: gStatus = "BEACON TX";    break;
        case STATE_ERROR:     gStatus = "ERROR";        break;
        default:              gStatus = "UNKNOWN";      break;
    }
    renderDashboard();
#endif
}

void showRx(const String& packet, bool valid) {
#if OLED_ENABLED
    if (!oledOk) return;
    gLastSource = extractSource(packet);
    if (valid) {
        gRxCount++;
        gStatus = "RX VALID";
    } else {
        gStatus = "RX INVALID";
    }
    renderDashboard();
#endif
}

void showUplink(const String& pkt) {
#if OLED_ENABLED
    if (!oledOk) return;
    gLastSource = extractSource(pkt);
    gTxCount++;
    gStatus = "APRS-IS TX OK";
    renderDashboard();
#endif
}

void showBeacon() {
#if OLED_ENABLED
    if (!oledOk) return;
    gStatus = "BEACON SENT";
    renderDashboard();
#endif
}

void showError(uint8_t retries) {
#if OLED_ENABLED
    if (!oledOk) return;
    gErrRetries = retries;
    gStatus = "ERROR #" + String(gErrRetries);
    renderDashboard();
#endif
}

} // namespace Display
