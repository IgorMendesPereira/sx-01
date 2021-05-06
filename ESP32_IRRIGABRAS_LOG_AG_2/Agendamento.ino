void Agendamento() {
  dados = "";
  int count = 0;
  String linha = "";
  registros2 = 0;
  AgFS.rewind();
  while (AgFS.readFileNextRecord(&linha, &errorMsg) && linha != "")
  {
    dados += linha;
    count++;
  }
  registros2 = count;
  AgFS.rewind();
  for (int i = 0; i < registros2; i++) {
    AgFS.readFileNextRecord(&linha, &errorMsg);
    String horarioag = linha.substring(linha.indexOf('-') + 1, linha.indexOf('\n'));
    int ano = (horarioag.substring(14,18)).toInt();
    int mes = (horarioag.substring(11, 13)).toInt();
    int dia = (horarioag.substring(8, 10)).toInt();
    int hora = (horarioag.substring(0, 2)).toInt();
    int minuto = (horarioag.substring(3, 5)).toInt();
    horarioag = String((ano - 1970) * 31556926 + mes * 2629743 + dia * 86400 + hora * 3600 + minuto * 60 - 2811141);
    horaag[i] = horarioag.toInt();
    //Serial2.println(horaag[i]);
    //    Serial.println("ok");
    atuaag[i] = linha.substring(0, linha.indexOf('-'));
    //Serial2.println(atuaag[i]);
  }
  


}

void AtuaAg(){
  for (int i = 0; i < registros2; i++) {
      if (epoch <= horaag[i] && horaag[i] < epoch + 5) {
        // Serial.println();
        //      Serial.println(atuaag[i]);
        contag++;
        if (atuaag[i] == "351") {
          digitalWrite(LIGA, LOW);
          digitalWrite(RAUX, LOW);
          digitalWrite(AVANCO, LOW);
          digitalWrite(REVERSO, HIGH);
          digitalWrite(MOLHADO, HIGH);
          delay(1000);
          digitalWrite(LIGA, HIGH);


        }
        if (atuaag[i] == "451") {
          digitalWrite(LIGA, LOW);
          digitalWrite(RAUX, LOW);
          digitalWrite(REVERSO, LOW);
          digitalWrite(AVANCO, HIGH);
          digitalWrite(MOLHADO, HIGH);
          delay(1000);
          digitalWrite(LIGA, HIGH);
        }
        if (atuaag[i] == "361") {
          digitalWrite(RAUX, LOW);
          digitalWrite(AVANCO, LOW);
          digitalWrite(REVERSO, HIGH);
          digitalWrite(MOLHADO, LOW);
          delay(1000);
          digitalWrite(LIGA, HIGH);
        }
        if (atuaag[i] == "461") {
          digitalWrite(LIGA, LOW);
          digitalWrite(RAUX, LOW);
          digitalWrite(REVERSO, LOW);
          digitalWrite(AVANCO, HIGH);
          digitalWrite(MOLHADO, LOW);
          delay(1000);
          digitalWrite(LIGA, HIGH);
        }
        if (atuaag[i] == "002") {
          digitalWrite(DESLIGA, LOW);
          digitalWrite(RAUX, HIGH);
          digitalWrite(LIGA, HIGH);
          digitalWrite(AVANCO, HIGH);
          digitalWrite(REVERSO, HIGH);
          digitalWrite(MOLHADO, HIGH);
          digitalWrite(RAUXP, HIGH);
          digitalWrite(PERCAT, HIGH);
          delay(1000);
          digitalWrite(DESLIGA, HIGH);
          perc = 0;
          auxP = 0;
          num = 0;
          numw = 0;
          aux2 = 0;
        }
        horaag[i] = 9999999999;
        atuaag[i] = " ";
        //delay(1000);
        LeEntrada();
        EnviaStatus();
//        Serial2.println(contag);
//        Serial2.println(registros2);
        if (contag == (registros2)) {
          SPIFFS.remove("/agenda.bin");
          contag = 0;
        }
      }
      
    }
}
  
