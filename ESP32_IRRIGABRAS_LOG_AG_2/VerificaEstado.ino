void VerificaEstado() {
  aux = 0;
  if (EstadoAtual[2] == '2') {
    delay(150);
    EnviaStatus();
    aux = 1;

    return;
  }
  if (stats[0] == EstadoAtual[0] && stats[2] == EstadoAtual[2]) {

    if (aux == 0) {
      EnviaStatus();
      aux = 1;
    }

  } else if (stats[0] == '0' && stats[1] == '0' && stats[2] == '0') {
    if (aux == 0) {
      EnviaStatus();
      aux = 1;
    }

  }
  else {
    LeEntrada();
    EnviaStatus();
  }
  aux = 0;
}
