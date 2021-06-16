void Leitura(int buffersize) {
  for (int i = 0; i < 50; i++) {
    if ( i < buffersize) {
      stats[i] =  Serial2.read();
    } else {
      stats[i] = '#';
      i = 51;
    }
  }
  if ((stats[0] == '3' || stats[0] == '4')  && webflag == 0) {
    Snum = String((stats[3] - '0') * 100 + (stats[4] - '0') * 10 + (stats[5] - '0'));
    num = Snum.toInt();
    aux2 = 0;
  }
  if (webflag == 1) {
    stats[0] = INWEB[0];
    stats[1] = INWEB[1];
    stats[2] = INWEB[2];
  }

}
