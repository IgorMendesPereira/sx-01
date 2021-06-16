//Executa a leitura dos agendamentos por horario na memoria e grava os valores em strings

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
    if (i < 4) {
      AgFS.readFileNextRecord(&linha, &errorMsg);
      String horarioag = linha.substring(linha.indexOf('-') + 1, linha.indexOf('\n'));
      int ano = (horarioag.substring(14, 18)).toInt();
      int mes = (horarioag.substring(11, 13)).toInt();
      int dia = (horarioag.substring(8, 10)).toInt();
      int hora = (horarioag.substring(0, 2)).toInt();
      int minuto = (horarioag.substring(3, 5)).toInt();

      //transforma data legivel para timestamp

      struct tm t;
      time_t t_of_day;

      t.tm_year = ano - 1900; // Year - 1900
      t.tm_mon = (mes - 1 );           // Month, where 0 = jan
      t.tm_mday = dia;          // Day of the month
      t.tm_hour = hora;
      t.tm_min = minuto;
      t.tm_sec = 0;
      t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
      t_of_day = mktime(&t);
      //Serial2.println(t_of_day);
      horarioag = String(t_of_day);
      horaag[i] = horarioag.toInt();
      atuaag[i] = linha.substring(0, linha.indexOf('/'));
      percag[i] = linha.substring(linha.indexOf('/') + 1, linha.indexOf('-')).toInt();
      //    Serial2.println(horaag[i]);
      //    Serial2.println(atuaag[i]);
      //    Serial2.println(percag[i]);
    }
  }



}

//Atuação do agendamento por horario

void AtuaAg() {
  for (int i = 0; i < registros2; i++) {
    if (epoch <= horaag[i] && horaag[i] < epoch + 5) {
      contag++;

      if (atuaag[i] == "351") {

        digitalWrite(LIGA, LOW);
        digitalWrite(RAUX, LOW);
        digitalWrite(AVANCO, LOW);
        digitalWrite(REVERSO, HIGH);
        digitalWrite(MOLHADO, HIGH);
        delay(espera);
        epoch = epoch + espera / 1000;
        digitalWrite(LIGA, HIGH);


      }
      if (atuaag[i] == "451") {
        digitalWrite(LIGA, LOW);
        digitalWrite(RAUX, LOW);
        digitalWrite(REVERSO, LOW);
        digitalWrite(AVANCO, HIGH);
        digitalWrite(MOLHADO, HIGH);
        delay(espera);
        epoch = epoch + espera / 1000;
        digitalWrite(LIGA, HIGH);
      }
      if (atuaag[i] == "361") {
        digitalWrite(RAUX, LOW);
        digitalWrite(AVANCO, LOW);
        digitalWrite(REVERSO, HIGH);
        digitalWrite(MOLHADO, LOW);
        delay(espera);
        epoch = epoch + espera / 1000;
        digitalWrite(LIGA, HIGH);
      }
      if (atuaag[i] == "461") {
        digitalWrite(LIGA, LOW);
        digitalWrite(RAUX, LOW);
        digitalWrite(REVERSO, LOW);
        digitalWrite(AVANCO, HIGH);
        digitalWrite(MOLHADO, LOW);
        delay(espera);
        epoch = epoch + espera / 1000;
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
        delay(espera);
        epoch = epoch + espera / 1000;
        digitalWrite(DESLIGA, HIGH);
        perc = 0;
        auxP = 0;
        num = 0;
        numw = 0;
        aux2 = 0;
      }
      num = percag[i];
      //Serial2.println(num);
      AtuaPercentimetro();
      percag[i] = 0;
      horaag[i] = 9999999999;
      atuaag[i] = " ";
      LeEntrada();
      EnviaStatus();

      //apaga os arquivos de agendamento quando todos sao executados

      if (contag == (registros2)) {
        AgFS.destroyFile();
        SPIFFS.remove("/agenda.bin");
        contag = 0;
      }
    }

  }
}


//Faz a leitura dos arquivos de agendamento e grava em String
void AgendaPOS() {
  dados = "";
  int count = 0;
  String linha = "";
  registros3 = 0;
  PosFS.rewind();
  while (PosFS.readFileNextRecord(&linha, &errorMsg) && linha != "")
  {
    dados += linha;
    count++;
  }
  registros3 = count;
  PosFS.rewind();
  for (int i = 0; i < registros3; i++) {
    PosFS.readFileNextRecord(&linha, &errorMsg);
    atuaP[i] = linha.substring(0, linha.indexOf('-'));
    pos[i] = linha.substring(linha.indexOf('-') + 1, linha.indexOf('\n')).toInt();

  }
}

//Atuação do agendamento por posicao


void AtuaPOS() {
  int anguloint = angulo.toInt();
  for (int i = 0; i < registros3; i++) {
    if (anguloint >=  pos[i] - 2 && anguloint <= pos[i] + 2) {

      contpos++;

      if (atuaP[i] == "002" && RETflag == 1) {
        digitalWrite(DESLIGA, LOW);
        digitalWrite(RAUX, HIGH);
        digitalWrite(LIGA, HIGH);
        digitalWrite(AVANCO, HIGH);
        digitalWrite(REVERSO, HIGH);
        digitalWrite(MOLHADO, HIGH);
        digitalWrite(RAUXP, HIGH);
        digitalWrite(PERCAT, HIGH);
        delay(espera);
        epoch = epoch + espera / 1000;
        digitalWrite(DESLIGA, HIGH);
        perc = 0;
        auxP = 0;
        num = 0;
        numw = 0;
        aux2 = 0;

        pos[i] = 9999999999;
        atuaP[i] = " ";
        //delay(1000);
        LeEntrada();
        EnviaStatus();
        RETflag = 0;
      }

      //Faz o retorno automatico por posicao

      if (atuaP[i] == "RET") {
        LeEntrada();
        digitalWrite(DESLIGA, LOW);
        digitalWrite(RAUX, HIGH);
        digitalWrite(LIGA, HIGH);
        digitalWrite(AVANCO, HIGH);
        digitalWrite(REVERSO, HIGH);
        digitalWrite(MOLHADO, HIGH);
        digitalWrite(RAUXP, HIGH);
        digitalWrite(PERCAT, HIGH);
        delay(espera);
        epoch = epoch + espera / 1000;
        digitalWrite(DESLIGA, HIGH);
        perc = 0;
        auxP = 0;
        num = 0;
        numw = 0;
        aux2 = 0;
        if (EstadoAtual[0] == '4') {
          num = 100;
          digitalWrite(LIGA, LOW);
          digitalWrite(RAUX, LOW);
          digitalWrite(AVANCO, LOW);
          digitalWrite(REVERSO, HIGH);
          digitalWrite(MOLHADO, HIGH);
          delay(espera);
          epoch = epoch + espera / 1000;
          digitalWrite(LIGA, HIGH);
        }
        if (EstadoAtual[0] == '3') {
          num = 100;
          digitalWrite(LIGA, LOW);
          digitalWrite(RAUX, LOW);
          digitalWrite(REVERSO, LOW);
          digitalWrite(AVANCO, HIGH);
          digitalWrite(MOLHADO, HIGH);
          delay(espera);
          epoch = epoch + espera / 1000;
          digitalWrite(LIGA, HIGH);
        }

        pos[i] = 9999999999;
        atuaP[i] = " ";
        //delay(1000);
        LeEntrada();
        EnviaStatus();
        RETflag = 1;
      }

      if (contpos == (registros3)) {
        //Serial2.println("apaga");
        PosFS.destroyFile();
        SPIFFS.remove("/agendaPOS.bin");
        contpos = 0;
      }

    }
  }
}
