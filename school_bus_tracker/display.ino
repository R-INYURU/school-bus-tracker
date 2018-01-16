void processDisplay() {
  u8g.firstPage();
  do {
    char journey[] = "KABUYE-NYABUGOGO-KACYIRU";
    String curTm = printTime('T');
    String curDt = printTime('D');
    String curStd = String(stdntNo);
    if (stdntNo < 10) {
      curStd = "0" + curStd;
    }
    uint8_t len = sizeof(journey);
    uint8_t ttlChar = (len - 1) * 5;
    uint8_t scrWidth = u8g.getWidth();
    uint8_t rem = scrWidth - ttlChar;
    uint8_t Xjrny = rem / 2;
    uint8_t jrny = rem - Xjrny;

    uint8_t tmLen = curTm.length() + 1;
    uint8_t dtLen = curDt.length() + 1;
    uint8_t stdLen = curStd.length() + 1;
    char Tm[tmLen + 5];
    char Dt[dtLen + 5];
    char Std[stdLen + 5];
    curTm.toCharArray(Tm, tmLen);
    curDt.toCharArray(Dt, dtLen);
    curStd.toCharArray(Std, stdLen);

    u8g.setFont(u8g_font_5x7);
    u8g.drawStr( Xjrny, 28, journey);

    u8g.setFont(u8g_font_8x13B);
    u8g.drawStr( 45, 11, Tm);
    u8g.setFont(u8g_font_5x7);
    u8g.drawStr( 40, 19, Dt);
    u8g.setFont(u8g_font_fub11);
    u8g.drawStr( 108, 62, Std);
  } while ( u8g.nextPage() );
  smartDelay(100);
}

String printTime(char HrDt) {   //convert time and date into string
  String Time;
  if (HrDt == 'D') {
    if (day() < 10)
      Time = '0' + String(day()) + '-';
    else
      Time = String(day()) + '-';
    if (month() < 10)
      Time += '0' + String(month());
    else
      Time += String(month());
    Time += '-' + String(year());
  }
  else if (HrDt == 'T') {
    if (hour() < 10)
      Time = '0' + String(hour()) + ':';
    else
      Time = String(hour()) + ':';
    if (minute() < 10)
      Time += '0' + String(minute());
    else
      Time += String(minute());
//    if (second() < 10)
//      Time += ':' + '0' + String(second());
//    else
//      Time += ':' + String(second());
  }
  return Time;
}

void AddStdntNo(bool Add){
  if (Add == true){
    if (stdntNo >= 99){
      stdntNo = 99;
      return;
    }
    stdntNo ++;
    return;
  }
  else{
    if (stdntNo == 0){
      stdntNo = 0;
      return;
    }
    stdntNo --;
    return;
  }
}

