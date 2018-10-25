//20181016 Darrent
// 以Aduino相容板 D1 mini + RFID-RC522模組
// 模擬上班刷卡系統
// 刷卡時，會將卡號、刷卡時間寫至Firebase。

#define LcdLeft 0
#define LcdCenter 1
#define LcdRight 2
unsigned int secendCount;

void setup()
{
  Serial.begin(115200);
  InitIOPin();
  InitLcd();
  InitDateTime();
  InitTimer();
  InitRfid();
  InitWiFi();
  InitFirebase();
}

void loop()
{

  struct tm *timeinfo = GetLocalTime();
  String id, memberName;
  if (CheckNewCard(&id))
  { //如果有新的刷卡資料
    Serial.println(id);
    LcdBacklight(true); // 開啟背光
    LcdShow(0, GetTimeFormat(timeinfo, "%Y/%m/%d %H:%M"), LcdCenter);

    String memberName = GetFirebaseMember(id);
    if (memberName == "")
      memberName = id;
    LcdShow(1, memberName, LcdLeft);
    Push2DatabaseAttendance(timeinfo, id); //換資料庫資料結構
    delay(3000);
  }
  /*
    if (secendCount == 0)
    return;
    //以下每秒進來一次
    secendCount--;
  */

  timeinfo = GetLocalTime();
  ShowClock(timeinfo);
  LcdBacklight(false); // 關閉背光
}

void ShowClock(struct tm *timeinfo)
{
  LcdShow(0, GetTimeFormat(timeinfo, "%Y/%m/%d"), LcdCenter);
  LcdShow(1, GetTimeFormat(timeinfo, "%H:%M:%S"), LcdCenter);
}

void InitIOPin()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}
