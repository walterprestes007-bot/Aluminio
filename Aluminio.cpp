#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// ==========================================
// WI-FI DO PRÓPRIO ESP32
// ==========================================

const char* ssid = "RADAR-ESP32";
const char* senha = "12345678";

WebServer servidor(80);
Servo radarServo;


// ==========================================
// PINOS
// ==========================================

const int TRIG_PIN = 12;
const int ECHO_PIN = 14;
const int SERVO_PIN = 13;


// ==========================================
// VARIÁVEIS
// ==========================================

int anguloAtual = 90;

long distanciaAtual = 0;

int melhorAngulo = 90;

long menorDistancia = 400;


// ==========================================
// MEDIR DISTÂNCIA
// ==========================================

long medirDistancia() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duracao == 0) {
    return 400;
  }

  long distancia = duracao * 0.0343 / 2;

  return distancia;
}


// ==========================================
// PÁGINA DO RADAR
// ==========================================

void paginaInicial() {

  String html = R"rawliteral(


<!DOCTYPE html>
<html lang="pt-BR">

<head>

<meta charset="UTF-8">

<meta name="viewport"
      content="width=device-width, initial-scale=1.0">

<title>RADAR ESP32</title>


<style>

/* =====================================================
   CONFIGURAÇÕES GERAIS
===================================================== */

* {
  box-sizing: border-box;
}

body {

  margin: 0;

  min-height: 100vh;

  background:
    radial-gradient(
      circle at center,
      #062b18 0%,
      #03150d 35%,
      #010504 75%,
      #000000 100%
    );

  color: #00ff66;

  font-family:
    "Courier New",
    monospace;

  overflow-x: hidden;
}


/* =====================================================
   CABEÇALHO
===================================================== */

header {

  text-align: center;

  padding: 20px;

  border-bottom:
    1px solid rgba(0,255,102,0.25);

  background:
    rgba(0,0,0,0.35);

  box-shadow:
    0 0 25px rgba(0,255,102,0.08);

}

header h1 {

  margin: 0;

  font-size: clamp(26px, 5vw, 42px);

  letter-spacing: 5px;

  text-shadow:
    0 0 10px #00ff66,
    0 0 25px #00ff66;

}

header p {

  margin: 8px 0 0;

  color: #65ff9b;

  font-size: 13px;

  letter-spacing: 2px;

}


/* =====================================================
   LAYOUT
===================================================== */

.container {

  width: 95%;

  max-width: 1250px;

  margin: auto;

  display: grid;

  grid-template-columns:
    minmax(300px, 1fr)
    minmax(280px, 380px);

  gap: 25px;

  padding: 25px 0;

}


@media(max-width: 850px) {

  .container {

    grid-template-columns: 1fr;

  }

}


/* =====================================================
   RADAR
===================================================== */

.radar-area {

  display: flex;

  justify-content: center;

  align-items: center;

}


/* círculo principal */

.radar {

  position: relative;

  width: min(80vw, 600px);

  aspect-ratio: 1;

  border-radius: 50%;

  background:

    repeating-radial-gradient(
      circle,
      transparent 0px,
      transparent 48px,
      rgba(0,255,102,0.22) 50px,
      transparent 52px
    ),

    linear-gradient(
      rgba(0,255,102,0.08) 1px,
      transparent 1px
    ),

    linear-gradient(
      90deg,
      rgba(0,255,102,0.08) 1px,
      transparent 1px
    ),

    radial-gradient(
      circle,
      #063d20 0%,
      #032415 45%,
      #010b06 100%
    );

  background-size:
    auto,
    50px 50px,
    50px 50px,
    auto;

  border:
    3px solid rgba(0,255,102,0.65);

  box-shadow:

    0 0 20px rgba(0,255,102,0.3),

    inset 0 0 80px
    rgba(0,255,102,0.15);

  overflow: hidden;

}


/* linhas horizontais e verticais */

.radar::before {

  content: "";

  position: absolute;

  left: 0;

  right: 0;

  top: 50%;

  height: 1px;

  background:
    rgba(0,255,102,0.3);

}


.radar::after {

  content: "";

  position: absolute;

  top: 0;

  bottom: 0;

  left: 50%;

  width: 1px;

  background:
    rgba(0,255,102,0.3);

}


/* =====================================================
   LINHA DE VARREDURA
===================================================== */

.scan {

  position: absolute;

  width: 50%;

  height: 3px;

  left: 50%;

  top: 50%;

  transform-origin: left center;

  background:
    linear-gradient(
      90deg,
      rgba(0,255,102,1),
      rgba(0,255,102,0)
    );

  box-shadow:
    0 0 10px #00ff66,
    0 0 25px #00ff66;

  z-index: 5;

}


/* =====================================================
   CENTRO
===================================================== */

.center {

  position: absolute;

  width: 12px;

  height: 12px;

  background: #00ff66;

  border-radius: 50%;

  left: calc(50% - 6px);

  top: calc(50% - 6px);

  box-shadow:
    0 0 10px #00ff66,
    0 0 25px #00ff66;

  z-index: 10;

}


/* =====================================================
   OBJETO DETECTADO
===================================================== */

.target {

  position: absolute;

  width: 18px;

  height: 18px;

  border-radius: 50%;

  transform:
    translate(-50%, -50%);

  z-index: 20;

  display: none;

  box-shadow:
    0 0 8px currentColor,
    0 0 20px currentColor,
    0 0 35px currentColor;

  animation:
    pulse 0.8s infinite;

}


@keyframes pulse {

  0% {

    transform:
      translate(-50%, -50%)
      scale(0.8);

    opacity: 0.6;

  }

  50% {

    transform:
      translate(-50%, -50%)
      scale(1.3);

    opacity: 1;

  }

  100% {

    transform:
      translate(-50%, -50%)
      scale(0.8);

    opacity: 0.6;

  }

}


/* =====================================================
   CÍRCULOS DE DISTÂNCIA
===================================================== */

.range {

  position: absolute;

  border:
    1px dashed
    rgba(0,255,102,0.25);

  border-radius: 50%;

  left: 50%;

  top: 50%;

  transform:
    translate(-50%, -50%);

}

.range1 {

  width: 25%;

  height: 25%;

}

.range2 {

  width: 50%;

  height: 50%;

}

.range3 {

  width: 75%;

  height: 75%;

}


/* =====================================================
   PAINEL
===================================================== */

.panel {

  display: flex;

  flex-direction: column;

  gap: 15px;

}


/* =====================================================
   CARDS
===================================================== */

.card {

  background:
    rgba(0,20,10,0.75);

  border:
    1px solid
    rgba(0,255,102,0.35);

  border-radius: 12px;

  padding: 18px;

  box-shadow:
    0 0 20px rgba(0,255,102,0.05);

}


.card-title {

  font-size: 12px;

  color: #6cff9c;

  letter-spacing: 2px;

  margin-bottom: 8px;

}


.big-value {

  font-size: 38px;

  font-weight: bold;

  text-shadow:
    0 0 10px currentColor;

}


.unit {

  font-size: 15px;

  opacity: 0.7;

}


/* =====================================================
   STATUS
===================================================== */

.status {

  padding: 14px;

  border-radius: 8px;

  text-align: center;

  font-weight: bold;

  background:
    rgba(0,255,102,0.08);

  border:
    1px solid rgba(0,255,102,0.25);

}


/* =====================================================
   HISTÓRICO
===================================================== */

.history {

  max-height: 240px;

  overflow-y: auto;

  font-size: 13px;

}


.history-row {

  display: flex;

  justify-content: space-between;

  padding: 6px 0;

  border-bottom:
    1px solid
    rgba(0,255,102,0.08);

}


/* =====================================================
   BARRA DE DISTÂNCIA
===================================================== */

.distance-bar {

  width: 100%;

  height: 12px;

  background:
    #021008;

  border-radius: 20px;

  overflow: hidden;

  margin-top: 10px;

}


.distance-fill {

  height: 100%;

  width: 0%;

  transition:
    width 0.2s,
    background 0.2s;

  box-shadow:
    0 0 10px currentColor;

}


/* =====================================================
   LEGENDA
===================================================== */

.legend {

  display: grid;

  grid-template-columns:
    repeat(3,1fr);

  gap: 6px;

  font-size: 11px;

  text-align: center;

}


.legend div {

  padding: 6px;

  border-radius: 5px;

}


/* =====================================================
   STATUS DA CONEXÃO
===================================================== */

.connection {

  text-align: center;

  font-size: 12px;

  color: #00ff66;

}


.dot {

  display: inline-block;

  width: 8px;

  height: 8px;

  border-radius: 50%;

  background: #00ff66;

  box-shadow:
    0 0 10px #00ff66;

  margin-right: 5px;

}

</style>

</head>


<body>


<header>

<h1>📡 RADAR ESP32</h1>

<p>SISTEMA DE DETECÇÃO • MONITORAMENTO EM TEMPO REAL</p>

</header>



<div class="container">


<!-- ============================================
     RADAR
============================================= -->

<div class="radar-area">

<div class="radar" id="radar">


<div class="range range1"></div>

<div class="range range2"></div>

<div class="range range3"></div>


<div class="scan" id="scan"></div>


<div class="center"></div>


<div class="target" id="target"></div>


</div>

</div>



<!-- ============================================
     PAINEL
============================================= -->

<div class="panel">


<div class="connection">

<span class="dot"></span>

ESP32 ONLINE

</div>



<div class="card">

<div class="card-title">

ÂNGULO ATUAL

</div>

<div class="big-value">

<span id="angulo">0</span>

<span class="unit">°</span>

</div>

</div>



<div class="card">

<div class="card-title">

DISTÂNCIA

</div>

<div class="big-value" id="distancia">

0

<span class="unit">cm</span>

</div>


<div class="distance-bar">

<div
class="distance-fill"
id="distanceFill">
</div>

</div>

</div>



<div class="card">

<div class="card-title">

STATUS

</div>

<div
class="status"
id="status">

🔎 PROCURANDO...

</div>

</div>



<div class="card">

<div class="card-title">

HISTÓRICO DE LEITURAS

</div>

<div
class="history"
id="history">

Aguardando dados...

</div>

</div>



<div class="card">

<div class="card-title">

LEGENDA DE DISTÂNCIA

</div>

<div class="legend">

<div
style="background:#ff0033;color:white">

0–20cm

</div>

<div
style="background:#ff9900;color:black">

20–50cm

</div>

<div
style="background:#00ff66;color:black">

50cm+

</div>

</div>

</div>


</div>

</div>



<script>


// ==================================================
// ELEMENTOS
// ==================================================

const scan =
document.getElementById("scan");

const target =
document.getElementById("target");

const anguloElement =
document.getElementById("angulo");

const distanciaElement =
document.getElementById("distancia");

const statusElement =
document.getElementById("status");

const historyElement =
document.getElementById("history");

const distanceFill =
document.getElementById("distanceFill");

const radar =
document.getElementById("radar");


// ==================================================
// HISTÓRICO
// ==================================================

let historico = [];


// ==================================================
// COR DA DISTÂNCIA
// ==================================================

function getColor(distancia) {


  if (distancia <= 20) {

    return "#ff0033";

  }


  if (distancia <= 50) {

    return "#ff9900";

  }


  if (distancia <= 100) {

    return "#ffff00";

  }


  return "#00ff66";

}


// ==================================================
// POSICIONAR OBJETO
// ==================================================

function posicionarObjeto(
  angulo,
  distancia
) {


  if (
    distancia <= 0 ||
    distancia >= 200
  ) {

    target.style.display =
      "none";

    return;

  }


  target.style.display =
    "block";


  const rect =
    radar.getBoundingClientRect();


  const tamanho =
    rect.width;


  // distância máxima representada
  const distanciaMaxima =
    200;


  let raio =
    (distancia / distanciaMaxima)
    * (tamanho / 2);


  // limita dentro do radar

  raio =
    Math.min(
      raio,
      tamanho / 2 - 10
    );


  // converte ângulo
  // 0 = esquerda
  // 90 = cima
  // 180 = direita

  const rad =
    (180 - angulo)
    * Math.PI / 180;


  const x =
    tamanho / 2
    + Math.cos(rad)
    * raio;


  const y =
    tamanho / 2
    - Math.sin(rad)
    * raio;


  target.style.left =
    x + "px";


  target.style.top =
    y + "px";


  target.style.color =
    getColor(distancia);


  target.style.background =
    getColor(distancia);

}


// ==================================================
// ATUALIZAR LINHA
// ==================================================

function atualizarScan(
  angulo
) {

  scan.style.transform =
    "rotate(" +
    (-angulo) +
    "deg)";

}


// ==================================================
// HISTÓRICO
// ==================================================

function adicionarHistorico(
  angulo,
  distancia
) {


  historico.unshift({

    angulo: angulo,

    distancia: distancia,

    hora:
      new Date()
      .toLocaleTimeString()

  });


  // máximo 30 registros

  if (
    historico.length > 30
  ) {

    historico.pop();

  }


  historyElement.innerHTML =
    "";


  historico.forEach(
    item => {


      const row =
        document.createElement(
          "div"
        );


      row.className =
        "history-row";


      row.innerHTML = `

        <span>

          ${item.hora}

        </span>

        <span>

          ${item.angulo}°

        </span>

        <span
          style="
          color:${getColor(item.distancia)}
          ">

          ${item.distancia} cm

        </span>

      `;


      historyElement
        .appendChild(row);

    }
  );

}


// ==================================================
// ATUALIZAR DISTÂNCIA
// ==================================================

function atualizarDistancia(
  distancia
) {


  distanciaElement.innerHTML =

    distancia +

    ' <span class="unit">cm</span>';


  const color =
    getColor(distancia);


  distanciaElement.style.color =
    color;


  distanceFill.style.background =
    color;


  distanceFill.style.color =
    color;


  // 200cm = 0%
  // 0cm = 100%

  let porcentagem =

    100 -
    (distancia / 200 * 100);


  porcentagem =
    Math.max(
      0,
      Math.min(
        100,
        porcentagem
      )
    );


  distanceFill.style.width =
    porcentagem + "%";


  // STATUS

  if (
    distancia <= 20
  ) {

    statusElement.innerHTML =
      "🚨 PERIGO — OBJETO MUITO PRÓXIMO";

    statusElement.style.color =
      "#ff0033";

  }

  else if (
    distancia <= 50
  ) {

    statusElement.innerHTML =
      "⚠️ ATENÇÃO — OBJETO PRÓXIMO";

    statusElement.style.color =
      "#ff9900";

  }

  else {

    statusElement.innerHTML =
      "🟢 ÁREA SEGURA";

    statusElement.style.color =
      "#00ff66";

  }

}


// ==================================================
// BUSCAR DADOS DO ESP32
// ==================================================

function atualizarRadar() {


  fetch("/dados")

  .then(
    response =>
      response.json()
  )

  .then(
    data => {


      const angulo =
        Number(data.angulo);


      const distancia =
        Number(data.distancia);


      // valores

      anguloElement.innerText =
        angulo;


      // linha

      atualizarScan(
        angulo
      );


      // distância

      atualizarDistancia(
        distancia
      );


      // alvo

      posicionarObjeto(
        angulo,
        distancia
      );


      // histórico

      adicionarHistorico(
        angulo,
        distancia
      );


    }
  )

  .catch(
    error => {

      statusElement.innerHTML =
        "🔴 CONEXÃO PERDIDA";

      statusElement.style.color =
        "#ff0033";

    }
  );

}


// ==================================================
// ATUALIZAÇÃO
// ==================================================

setInterval(
  atualizarRadar,
  200
);


atualizarRadar();


</script>


</body>

</html>

)rawliteral";



  servidor.send(200, "text/html", html);
}


// ==========================================
// ENVIAR DADOS PARA O SITE
// ==========================================

void enviarDados() {

  String status;

  if (menorDistancia < 400) {

    status = "🎯 Objeto detectado";

  } else {

    status = "🔎 Procurando objeto";

  }


  String json = "{";

  json += "\"angulo\":";
  json += String(anguloAtual);

  json += ",";

  json += "\"distancia\":";
  json += String(distanciaAtual);

  json += ",";

  json += "\"status\":\"";
  json += status;
  json += "\"";

  json += "}";


  servidor.send(
    200,
    "application/json",
    json
  );
}


// ==========================================
// SETUP
// ==========================================

void setup() {

  Serial.begin(115200);


  // Sensor

  pinMode(TRIG_PIN, OUTPUT);

  pinMode(ECHO_PIN, INPUT);


  // Servo

  radarServo.attach(SERVO_PIN);

  radarServo.write(90);


  delay(1000);


  // ========================================
  // CRIAR REDE WI-FI
  // ========================================

  Serial.println();

  Serial.println("Iniciando Wi-Fi...");


  WiFi.softAP(
    ssid,
    senha
  );


  IPAddress IP = WiFi.softAPIP();


  Serial.println();

  Serial.println("==============================");

  Serial.println("     RADAR ESP32");

  Serial.println("==============================");

  Serial.print("Rede: ");

  Serial.println(ssid);

  Serial.print("Senha: ");

  Serial.println(senha);

  Serial.print("IP: ");

  Serial.println(IP);

  Serial.println("==============================");


  // ========================================
  // SERVIDOR
  // ========================================

  servidor.on(
    "/",
    paginaInicial
  );


  servidor.on(
    "/dados",
    enviarDados
  );


  servidor.begin();


  Serial.println("Servidor iniciado!");

  Serial.println();

}


// ==========================================
// LOOP
// ==========================================

void loop() {

  servidor.handleClient();


  // Reinicia a busca

  menorDistancia = 400;

  melhorAngulo = 90;


  // ========================================
  // VARREDURA 0 → 180
  // ========================================

  for (
    int angulo = 0;
    angulo <= 180;
    angulo++
  ) {

    anguloAtual = angulo;


    radarServo.write(
      angulo
    );


    delay(30);


    distanciaAtual =
      medirDistancia();


    Serial.print("Angulo: ");

    Serial.print(angulo);


    Serial.print("° | Distancia: ");

    Serial.print(distanciaAtual);


    Serial.println(" cm");


    // ======================================
    // VERIFICA OBJETO
    // ======================================

    if (
      distanciaAtual < menorDistancia &&
      distanciaAtual > 2
    ) {

      menorDistancia =
        distanciaAtual;


      melhorAngulo =
        angulo;

    }


    servidor.handleClient();

  }


  // ========================================
  // APONTA PARA O OBJETO
  // ========================================

  Serial.println();

  Serial.println("==============================");

  Serial.print("🎯 OBJETO EM ");

  Serial.print(melhorAngulo);

  Serial.print("° | ");

  Serial.print(menorDistancia);

  Serial.println(" cm");

  Serial.println("==============================");


  radarServo.write(
    melhorAngulo
  );


  anguloAtual =
    melhorAngulo;


  distanciaAtual =
    menorDistancia;


  // ========================================
  // FICA PARADO NO OBJETO
  // ========================================

  for (
    int i = 0;
    i < 100;
    i++
  ) {

    servidor.handleClient();

    delay(10);

  }

}
