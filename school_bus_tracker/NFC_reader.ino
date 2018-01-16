void processNFC() {
  uint8_t keyA[6] = { 0xBC, 0x76, 0xEB, 0x3F, 0x58, 0x9D };
  uint8_t blockNum = 6;

  if (nfc.tagPresent()) {
    String msg = readTag(blockNum, keyA);

    if (msg == "enIN") {
      AddStdntNo(false);
      writeTag("OUT", blockNum, keyA);
      neoColor('B');
      Serial.println("student OUT");
    }
    else if (msg == "enOUT") {
      AddStdntNo(true);
      writeTag("IN", blockNum, keyA);
      neoColor('G');
      Serial.println("student IN");
    }
    else {
      writeTag("IN", blockNum, keyA);
      neoColor('R');
    }
  }
  smartDelay(200);
}

String readTag(int block, uint8_t key[6]) { //reads the TAGs using the defined keys on a specific block.

  String Message;

  NfcTag tag = nfc.read(block, key);
  if (tag.hasNdefMessage()) {
    Uid = tag.getUidString();
    NdefMessage message = tag.getNdefMessage();
    int recordCount = message.getRecordCount();
    for (int i = 0; i < recordCount; i++) {
      NdefRecord record = message.getRecord(i);
      int payloadLength = record.getPayloadLength();
      byte payload[payloadLength];
      record.getPayload(payload);
      for (int c = 0; c < payloadLength; c++) {
        Message += (char)payload[c];
      }
    }
    return Message;
  }
  return "";
}

void writeTag(String text, int block, uint8_t key[6]) { //writes to the TAG specified text using keys of that block

  NdefMessage message = NdefMessage();
  message.addTextRecord(text);
  bool success = nfc.write(message, block, key);
  if (success){
    Serial.println("Message written correctly");
    return;
  }
  Serial.println("Message not written!?");
}
