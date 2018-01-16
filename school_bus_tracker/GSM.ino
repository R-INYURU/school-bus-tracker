/*boolean mqttConnect() {
  Serial.print("Connecting to ");
  Serial.print(broker);
  if (!mqtt.connect(deviceID)) {
    Serial.println(" fail");
    return false;
  }
  Serial.println(" OK");

  mqtt.subscribe(topicgps);
  mqtt.subscribe(topicstdntNum);
  mqtt.subscribe(topiccards);
  mqtt.subscribe(topicjourney);
  mqtt.subscribe(topicsettings);
  return mqtt.connected();
}*/

/*void mqttCallback(char* topic, byte* payload, unsigned int len) {
  String journey;
  for (unsigned int c = 0; c < len; c++) {
    journey += (char)payload[c];
  }
  Serial.print("topic data= ");
  Serial.println(journey);
}*/

JsonObject& createJson(JsonBuffer& jsonBuffer){
  JsonObject& Object = jsonBuffer.createObject();
  Object["deviceID"] = deviceID;
  refreshGPS();
  JsonArray& GPS = Object.createNestedArray("gpsData");
  GPS.add(gps.location.lat());
  GPS.add(gps.location.lng());
  GPS.add(gps.speed.kmph());
  Object["cardID"] = Uid;
  Object["student#"] = stdntNo;

  Uid = "";
  
  return Object;
}

