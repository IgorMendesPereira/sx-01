//Função que envia os valores das variaveis para a pagina web

/*sintaxe:

    if (var == "(placeholder da pagina web)" {
     return (variavel para aparecer na pagina web);
   }

*/

String processor(const String& var) {
  //Serial.println(var);
  if (var == "SENTIDO") {
    if (sentido == "Avanco") {
      sentidow = "Avanco";
    } else if (sentido == "Reverso") {
      sentidow = "Reverso";
    } else {
      sentidow = "";
    }
    //Serial2. print(ledState);
    return sentidow;
  }
    if (var == "SECO") {
      if (seco == "Seco") {
        secow = "Seco";
      } else if (seco == "Molhado") {
        secow = "Molhado";
      }
      if (EstadoAtual[2] == '2') {
        secow = "";
      }
  
      return secow;
    }
    if (var == "LIGADO") {
      if ( EstadoAtual[2] == '2') {
        ligadow = "Desligado";
      } else {
        ligadow = "Ligado";
      }
      return ligadow;
    }
  if (var == "PERCENTIMETRO") {
    return inputMessage;
  }
  if (var == "PERC") {
    return STRperc;
  }
  if (var == "HORA") {
    return String(hora);
  }
  if (var == "ANG") {
    return String(angulo.toInt());
  }
  if (var == "RSSI") {
    return String(rssi);
  }


  if (var == "AGENDA1") {
    String reg1 = AgFS.findRecord(0);
    if (reg1.substring(0, 3) == "351") {
      String sreg1 = String("Avanco Seco Ligado - " + reg1.substring(4, reg1.indexOf("\n")));
      return sreg1;
    }
    if (reg1.substring(0, 3) == "361") {
      String sreg1 = String("Avanco Molhado Ligado - " + reg1.substring(4, reg1.indexOf("\n")));
      return sreg1;
    }
    if (reg1.substring(0, 3) == "451") {
      String sreg1 = String("Reverso Seco Ligado - " + reg1.substring(4, reg1.indexOf("\n")));
      return sreg1;
    }
    if (reg1.substring(0, 3) == "461") {
      String sreg1 = String("Reverso Molhado Ligado - " + reg1.substring(4, reg1.indexOf("\n")));
      return sreg1;
    }
    if (reg1.substring(0, 3) == "002") {
      String sreg1 = String("Desligado - " + reg1.substring(4, reg1.indexOf("\n")));
      return sreg1;
    }

  }
  if (var == "AGENDA2") {
    String reg2 = AgFS.findRecord(1);
    if (reg2.substring(0, 3) == "351") {
      String sreg2 = String("Avanco Seco Ligado - " + reg2.substring(4, reg2.indexOf("\n")));
      return sreg2;
    }
    if (reg2.substring(0, 3) == "361") {
      String sreg2 = String("Avanco Molhado Ligado - " + reg2.substring(4, reg2.indexOf("\n")));
      return sreg2;
    }
    if (reg2.substring(0, 3) == "451") {
      String sreg2 = String("Reverso Seco Ligado - " + reg2.substring(4, reg2.indexOf("\n")));
      return sreg2;
    }
    if (reg2.substring(0, 3) == "461") {
      String sreg2 = String("Reverso Molhado Ligado - " + reg2.substring(4, reg2.indexOf("\n")));
      return sreg2;
    }
    if (reg2.substring(0, 3) == "002") {
      String sreg2 = String("Desligado - " + reg2.substring(4, reg2.indexOf("\n")));
      return sreg2;
    }

  }
  if (var == "AGENDA3") {
    String reg3 = AgFS.findRecord(2);
    if (reg3.substring(0, 3) == "351") {
      String sreg3 = String("Avanco Seco Ligado - " + reg3.substring(4, reg3.indexOf("\n")));
      return sreg3;
    }
    if (reg3.substring(0, 3) == "361") {
      String sreg3 = String("Avanco Molhado Ligado - " + reg3.substring(4, reg3.indexOf("\n")));
      return sreg3;
    }
    if (reg3.substring(0, 3) == "451") {
      String sreg3 = String("Reverso Seco Ligado - " + reg3.substring(4, reg3.indexOf("\n")));
      return sreg3;
    }
    if (reg3.substring(0, 3) == "461") {
      String sreg3 = String("Reverso Molhado Ligado - " + reg3.substring(4, reg3.indexOf("\n")));
      return sreg3;
    }
    if (reg3.substring(0, 3) == "002") {
      String sreg3 = String("Desligado - " + reg3.substring(4, reg3.indexOf("\n")));
      return sreg3;
    }

  }
  if (var == "AGENDA4") {
    String reg4 = AgFS.findRecord(3);
    if (reg4 == "Seek error") {
      String sreg4 = "";
      return sreg4;
    }
    if (reg4.substring(0, 3) == "351") {
      String sreg4 = String("Avanco Seco Ligado - " + reg4.substring(4, reg4.indexOf("\n")));
      return sreg4;
    }
    if (reg4.substring(0, 3) == "361") {
      String sreg4 = String("Avanco Molhado Ligado - " + reg4.substring(4, reg4.indexOf("\n")));
      return sreg4;
    }
    if (reg4.substring(0, 3) == "451") {
      String sreg4 = String("Reverso Seco Ligado - " + reg4.substring(4, reg4.indexOf("\n")));
      return sreg4;
    }
    if (reg4.substring(0, 3) == "461") {
      String sreg4 = String("Reverso Molhado Ligado - " + reg4.substring(4, reg4.indexOf("\n")));
      return sreg4;
    }
    if (reg4.substring(0, 3) == "002") {
      String sreg4 = String("Desligado - " + reg4.substring(4, reg4.indexOf("\n")));
      return sreg4;
    }

  }

  if (var == "AGENDAPOS1") {
    String reg5 = PosFS.findRecord(0);
    if (reg5 == "Seek error") {
      String sreg5 = "";
      return sreg5;
    }
    if (reg5.substring(0, 3) == "002") {
      String sreg5 = String("Desligar - " + reg5.substring(4, reg5.indexOf("\n")));
      return sreg5;
    }
    if (reg5.substring(0, 3) == "RET") {
      String sreg5 = String("Retornar - " + reg5.substring(4, reg5.indexOf("\n")));
      return sreg5;
    }


  }

  if (var == "AGENDAPOS2") {
    String reg6 = PosFS.findRecord(1);
    if (reg6 == "Seek error") {
      String sreg6 = "";
      return sreg6;
    }
    if (reg6.substring(0, 3) == "002") {
      String sreg6 = String("Desligar - " + reg6.substring(4, reg6.indexOf("\n")));
      return sreg6;
    }
    if (reg6.substring(0, 3) == "RET") {
      String sreg6 = String("Retornar - " + reg6.substring(4, reg6.indexOf("\n")));
      return sreg6;
    }


  }

  if (var == "CONTATOR") {
    return TIPO;
  }
  if (var == "DELAY") {
    return String((espera / 1000));
  }
  if (var == "NOME") {
    return nome;
  }
  if (var == "SW") {
    return String(LoRaAdress);
  }
  if (var == "JSONhora") {
    return JSONhora;
  }

  return String();
}
