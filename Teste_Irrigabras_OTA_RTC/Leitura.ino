void Leitura() {
  for (int i = 0; i <= 5; i++) {
    
    stats[i] = Serial.read();                //le a entrada Serial, byte por byte
  }
  if(webflag == 1){
    stats[0] = INWEB[0];
    stats[1] = INWEB[1];
    stats[2] = INWEB[2];
    stats[3] = INWEB[3];
    stats[4] = INWEB[4];
    stats[5] = INWEB[5];
  }
}
