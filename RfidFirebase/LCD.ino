#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define HelloText "Hello, world!"
#define LcdCharacters 16 //一行 16 字元
#define LcdLines 2       //共 2 行

// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // 設定 LCD I2C 位址  //LiquidCrystal_I2C lcd(0x27, D1, D2);
String LcdlineText[2] = {"", ""};                              //儲存LCD顯示內容

void InitLcd()
{
  lcd.begin(LcdCharacters, LcdLines); // 初始化 LCD，一行 16 的字元，共 2 行，預設開啟背光
  lcd.clear();
  LcdBacklight(true);
  LcdShow(0, 0, HelloText);
}

void LcdShow(int y, String text, int location)
{
  unsigned int lineLength = text.length();
  if (lineLength < LcdCharacters)
  {
    String spaceText;
    int firstSpace, lastSpace;
    switch (location)
    {
      case LcdLeft:
        text = text + Repeat(' ', LcdCharacters - lineLength);
        break;
      case LcdCenter:
        firstSpace = (LcdCharacters - lineLength) / 2;
        lastSpace = LcdCharacters - lineLength - firstSpace;
        text = Repeat(' ', firstSpace) + text + Repeat(' ', lastSpace);
        break;
      case LcdRight:
        text = Repeat(' ', LcdCharacters - lineLength) + text;
        break;
    }
  }
  if (LcdlineText[y] == text) //若內容一樣就無需再重複(例如時鐘的日期)
    return;
  LcdlineText[y] = text;
  lcd.setCursor(0, y);
  lcd.print(text);
  //Serial.println((String)y + " - " + text);
}

void LcdShow(int x, int y, String text)
{
  lcd.setCursor(x, y);
  lcd.print(text);
}

void LcdShow(int y, String text)
{
  lcd.setCursor(0, y);
  lcd.print(text);
}

String Repeat(char repeatChar, unsigned int count)
{
  String repeatString = "";
  for (unsigned int i = 0; i < count; i++)
  {
    repeatString = repeatString + (String)repeatChar;
  }
  return repeatString;
}

void Clear()
{
  lcd.clear();
}

void LcdBacklight(bool onOff)
{
  if (onOff)
    lcd.backlight(); // 開啟背光
  else
    lcd.noBacklight(); // 關閉背光
}
