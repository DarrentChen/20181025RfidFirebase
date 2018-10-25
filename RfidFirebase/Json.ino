#include <ArduinoJson.h>

StaticJsonBuffer<200> jsonBuffer;

void PushJsonObject(String path, String cardID)
{
  jsonBuffer.clear();
  JsonObject& record = jsonBuffer.createObject();
  record["cardID"] = cardID;
  JsonObject& timeStampObject = jsonBuffer.createObject();
  timeStampObject[".sv"] = "timestamp";
  record["timeStamp"] = timeStampObject;
  Firebase.push(path, record);
}

/*
  JsonObject& cardIdObject = jsonBuffer.createObject();
  cardIdObject["CardId"] = cardId;
  JsonObject& timeStampObject = jsonBuffer.createObject();
  timeStampObject[".sv"] = "timestamp";
  cardIdObject["timeStamp"] = timeStampObject;
  Firebase.push("iot0901/RfidRecords", timeStampObject);
  }
*/

/*
  //create Json
  JsonObject& cardIdObject = jsonBuffer.createObject();
  cardIdObject["cardID"] = cardID;
  JsonObject& timeStampObject = jsonBuffer.createObject();
  timeStampObject[".sv"] = "timestamp";
  cardIdObject["timeStamp"] = timeStampObject;
  Firebase.push("iot0901/rfid/records/", cardIdObject);
*/

/*
  String GetJsonAttendance(String id){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["Date"] = GetLocalTime();
  root["ID"] = id;
  String output;
  root.printTo(output);
  return output;
  }
*/
