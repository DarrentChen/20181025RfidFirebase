#include <ESP8266WiFi.h>
//#define WIFI_SSID0 "你自己家裡的WiFi AP SSID"
//#define WIFI_PASSWORD0 "你自己家裡的AP密碼"
//#define WIFI_SSID1 "你自己手機的WiFi AP SSID"
//#define WIFI_PASSWORD1 "你自己手機的AP密碼"

void InitWiFi()
{
  int indexAp, sec = 5;
  String message = "WiFi connecting...";
  Serial.print(message);
  LcdShow(1, message);

  while (true)
  {
    if (sec < 60)
      sec++;
    int networks = ScanWiFiAp();
    if (networks == 0)
    { //沒有可連線的AP
      delay(sec * 1000);
      continue;
    }
    indexAp = -1;
    for (int i = 0; i < networks; i++)
    {
      if (WiFi.SSID(i) == WIFI_SSID0)
      { //SSID符合第0組AP
        indexAp = 0;
        break;
      }
      if (WiFi.SSID(i) == WIFI_SSID1)
      { //SSID符合第組AP
        indexAp = 1;
        break;
      }
    }
    if (indexAp < 0) //現有的AP都不符合
      continue;
    if (ConnectWiFi(indexAp, sec)) //成功連線
      break;
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println();
  message = WiFi.localIP().toString();
  Serial.println("connected: " + message);
  LcdShow(1, message, LcdLeft);
  delay(2000);
}

//連接WiFi AP
//i:用兩組SSID與password的哪一組
//c:等候幾秒
bool ConnectWiFi(int i, int c)
{
  switch (i)
  {
    case 0:
      Serial.println("");
      Serial.print(WIFI_SSID0);
      LcdShow(0, WIFI_SSID0, LcdLeft);
      WiFi.begin(WIFI_SSID0, WIFI_PASSWORD0);
      break;
    case 1:
      Serial.println("");
      Serial.print(WIFI_SSID1);
      LcdShow(0, WIFI_SSID1, LcdLeft);
      WiFi.begin(WIFI_SSID1, WIFI_PASSWORD1);
      break;
  }
  for (int i = 0; i < c; i++)
  {
    int statusValue = WiFi.status();
    //PrintWiFiStatus(statusValue);
    if (statusValue == WL_CONNECTED)
      return true;
    bool point = digitalRead(LED_BUILTIN);
    digitalWrite(LED_BUILTIN, !point);
    String pointText = point ? "." : " ";
    LcdShow(15, 1, pointText);
    Serial.print(".");
    delay(1000);
  }
  return false;
}

int ScanWiFiAp()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int networks = WiFi.scanNetworks();
  Serial.println("scan done");
  if (networks == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(networks);
    Serial.println(" networks found");
    for (int i = 0; i < networks; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  return networks;
}

void PrintWiFiStatus(int statusValue)
{
  switch (statusValue)
  {
    case WL_CONNECTED:
      Serial.println("WL_CONNECTED");
      break;
    case WL_NO_SHIELD:
      Serial.println("WL_NO_SHIELD");
      break;
    case WL_IDLE_STATUS:
      Serial.println("WL_IDLE_STATUS");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("WL_NO_SSID_AVAIL");
      break;
    case WL_SCAN_COMPLETED:
      Serial.println("WL_SCAN_COMPLETED");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("WL_CONNECT_FAILED");
      break;
    case WL_CONNECTION_LOST:
      Serial.println("WL_CONNECTION_LOST");
      break;
    case WL_DISCONNECTED:
      Serial.println("WL_DISCONNECTED");
      break;
  }
}
