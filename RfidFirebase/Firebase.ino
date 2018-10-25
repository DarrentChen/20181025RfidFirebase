#include <FirebaseArduino.h>

//#define FIREBASE_HOST "你自己在Firebase上專案的網址"
//#define FIREBASE_AUTH "你自己在Firebase上專案的授權碼"

String FirebasePath = "iot0901";
String FirebaseHumidity = FirebasePath + "/humidity";
String FirebaseTemperature = FirebasePath + "/temperature";
String FirebaseRelay = FirebasePath + "/relay";
String FirebaseTouchkey = FirebasePath + "/touchKey";
String FirebasePowerControl = FirebasePath + "/powerControl";
String FirebaseLog = FirebasePath + "/Log";
String FirebaseRfid = FirebasePath + "/Rfid";
String FirebaseMember = FirebasePath + "/member";
String FirebaseAttendance = FirebasePath + "/Attendance";
String FirebaseAttendanceRecords = FirebaseAttendance + "/Records";

int firebaseErrorCount = 0;

void InitFirebase()
{
  Firebase.begin(FIREBASE_HOST); // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.print("Firebase host:");
  Serial.println(FIREBASE_HOST);
}

void ReconnectFirebase()
{ //重新連接
  Serial.println("Trying to reconnect");
  //Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  InitFirebase();
}

bool GetFirebasePowerControl(int *value)
{
  return GetFirebaseData(FirebasePowerControl, value);
}

String GetFirebaseMember(String cardId)
{
  String path = FirebaseMember + "/" + cardId;
  Serial.print("GetFirebaseMember:" + path);
  String memberName = Firebase.getString(path);
  Serial.println(" - " + memberName);
  if (Firebase.success())
    return memberName;
  else
    return "";
}

bool GetFirebaseData(String path, int *value)
{
  int oldValue = *value;
  digitalWrite(LED_BUILTIN, HIGH);
  int nowValue = Firebase.getInt(path);
  bool result = CheckFirebase("Get", path, String(nowValue));
  if (result && oldValue != nowValue)
  {
    *value = nowValue;
    String message = "Get:" + path + "/" + String(nowValue);
    Serial.println(message);
    Firebase.pushString(FirebaseLog, message);
  }
  return result;
}

bool GetFirebaseRelay(bool *value)
{
  return GetFirebaseData(FirebaseRelay, value);
}

bool GetFirebaseData(String path, bool *value)
{
  bool oldValue = *value;
  digitalWrite(LED_BUILTIN, HIGH);
  bool nowValue = Firebase.getBool(path);
  bool result = CheckFirebase("Get", path, String(nowValue));
  if (result && oldValue != nowValue)
  {
    *value = nowValue;
    String message = "Get:" + path + "/" + String(nowValue);
    Serial.println(message);
    Firebase.pushString(FirebaseLog, message);
  }
  return result;
}

bool Set2DatabaseRelay(bool value)
{
  return SetFirebaseData(FirebaseRelay, value);
}

bool Set2DatabaseTouchkey(bool touchKeyNow)
{
  return SetFirebaseData(FirebaseTouchkey, touchKeyNow);
}

bool Set2DatabaseHumidity(float humidity)
{
  return SetFirebaseData(FirebaseHumidity, humidity);
}

bool Set2DatabaseTemperature(float temperature)
{
  return SetFirebaseData(FirebaseTemperature, temperature);
}

bool Set2DatabaseRfid(String id)
{
  return SetFirebaseData(FirebaseRfid, id);
}

bool Push2DatabaseAttendance(struct tm *timeinfo, String id)
{
  //  Set2DatabaseAttendance(timeinfo, id); //把資料寫至Firebase
  PushJsonObject(FirebaseAttendanceRecords, id);
}

bool Set2DatabaseAttendance(struct tm *timeinfo, String id)
{
  String dateToday = GetTimeFormat(timeinfo, "%Y%m%d");
  String timeNow = GetTimeFormat(timeinfo, "%H%M%S");
  String path = FirebaseAttendance + "/" + dateToday + "/" + id;
  Push2Database(path, timeNow);
}

bool Push2Database(String path, String value)
{
  digitalWrite(LED_BUILTIN, HIGH);
  Firebase.pushString(path, value);
  return CheckFirebase("Push", path, value);
}

bool SetFirebaseData(String path, bool value)
{
  digitalWrite(LED_BUILTIN, HIGH);
  Firebase.setBool(path, value);
  return CheckFirebase("Set", path, String(value));
}

bool SetFirebaseData(String path, float value)
{
  digitalWrite(LED_BUILTIN, HIGH);
  Firebase.setFloat(path, value);
  return CheckFirebase("Set", path, String(value));
}

bool SetFirebaseData(String path, String value)
{
  Firebase.setString(path, value);
  return CheckFirebase("Set", path, String(value));
}

bool CheckFirebase(String command, String path, String value)
{
  bool result = false;
  if (Firebase.success())
  {
    String message = command + ":" + path + "/" + value;
    result = true;
    firebaseErrorCount = 0;
    if (command == "Set")
      Firebase.pushString(FirebaseLog, message);
  }
  if (Firebase.failed())
  {
    if (firebaseErrorCount++ > 10)
    {
      ESP.restart();
    }
    else
    {
      Serial.println(command + " Error!\r\n" + Firebase.error());
      ReconnectFirebase();
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
  return result;
}
