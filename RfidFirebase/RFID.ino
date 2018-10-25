//#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h> // 引用程式庫

/*
  http://asd9616132.pixnet.net/blog/post/459146521-rfid-nodemcu-rfid
  RFID讀卡機連接NodeMCU的接腳方式
  RST     = GPIO5 (D1)
  SDA(SS) = GPIO4(D2)
  MOSI    = GPIO13(D7)
  MISO    = GPIO12(D6)
  SCK     = GPIO14(D5)
  GND     = GND
  3.3V    = 3.3V
*/

#define RST_PIN D0 // 讀卡機的重置腳位
#define SS_PIN D8  // 晶片選擇腳位

MFRC522 mfrc522(SS_PIN, RST_PIN); // 建立MFRC522物件

void InitRfid()
{
  SPI.begin();
  mfrc522.PCD_Init(); // 初始化MFRC522讀卡機模組
}

bool CheckNewCard(String *idText)
{
  // 確認是否有新卡片
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  {
    String idTemp = "";
    byte *id = mfrc522.uid.uidByte;                                      // 取得卡片的UID
    byte idSize = mfrc522.uid.size;                                      // 取得UID的長度
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak); // 根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
    String TypeName = mfrc522.PICC_GetTypeName(piccType);
    for (int i = idSize - 1; i >= 0; i--)
    {
      //Serial.print(id[i], HEX);       // 逐一以16進位顯示UID值
      String segmentText = String(id[i], HEX); //想用sprintf( "%2X", id[i]); 克服前面補0，未果
      if (segmentText.length() == 1)           //如果是1位數
        segmentText = "0" + segmentText;       //在前面補0
      idTemp = idTemp + segmentText;
      //Serial.println(segmentText);
    }
    Serial.println(" Card type: " + TypeName + ", UID Size: " + idSize + ", ID: " + idTemp); // 顯示卡片類型、卡片的UID長度值、卡片號碼
    mfrc522.PICC_HaltA();                                                                    // 讓卡片進入停止模式
    *idText = idTemp;
    return true;
  }
  else
  {
    return false;
  }
}
