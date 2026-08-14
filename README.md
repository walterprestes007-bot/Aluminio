# 📡 Radar Inteligente com ESP32

> Sistema de detecção de objetos utilizando **ESP32 + sensor ultrassônico HC-SR04 + servo motor**, com interface web em tempo real para visualização do ângulo, distância e histórico das leituras.

---

## 📌 Sobre o Projeto

Este projeto implementa um sistema de **radar de proximidade baseado em ESP32**.

O dispositivo realiza uma varredura angular utilizando um servo motor, enquanto o sensor ultrassônico **HC-SR04** mede a distância dos objetos encontrados.

As informações coletadas são processadas pelo ESP32 e disponibilizadas através de um **servidor Web HTTP**, permitindo acessar uma interface gráfica diretamente pelo navegador.

### Principais funcionalidades

- 📡 Varredura de **0° a 180°**
- 📏 Medição da distância dos objetos
- 🎯 Identificação do ponto mais próximo
- 🔄 Controle automático do servo
- 🌐 Servidor Web integrado ao ESP32
- 📊 Histórico das medições
- 🟢🟡🟠🔴 Indicação visual baseada na distância
- 🚨 Sistema de alerta de proximidade
- 📈 Interface de radar animada
- 📱 Interface responsiva para computador e celular

---

## 🧠 Arquitetura do Projeto

```text
                   ┌─────────────────────┐
                   │       ESP32         │
                   │                     │
                   │  Processamento      │
                   │  Wi-Fi / WebServer  │
                   └──────────┬──────────┘
                              │
              ┌───────────────┴───────────────┐
              │                               │
              ▼                               ▼
      ┌───────────────┐                ┌───────────────┐
      │   HC-SR04     │                │ Servo Motor   │
      │               │                │               │
      │ Medição       │                │ 0° → 180°     │
      │ de distância  │                │               │
      └───────┬───────┘                └───────────────┘
              │
              ▼
      ┌───────────────────┐
      │ Processamento     │
      │ da distância      │
      └─────────┬─────────┘
                │
                ▼
      ┌───────────────────┐
      │ Servidor HTTP     │
      │ /                 │
      │ /dados            │
      └─────────┬─────────┘
                │
                ▼
      ┌───────────────────┐
      │ Interface Web     │
      │ HTML/CSS/JS       │
      └───────────────────┘
```

---

# 🛠️ Tecnologias Utilizadas

## Hardware

- ESP32 DevKit
- HC-SR04
- Servo motor SG90
- Protoboard
- Jumpers
- Resistor **1 kΩ**
- Resistor **2 kΩ**
- Cabo USB

## Software

- C++ / Arduino
- Arduino IDE
- HTML5
- CSS3
- JavaScript
- WebServer
- ESP32Servo
- Wi-Fi integrado do ESP32

---

# 📦 Bibliotecas

O projeto utiliza as seguintes bibliotecas:

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
```

### `WiFi.h`

Responsável pelo funcionamento da interface Wi-Fi do ESP32.

### `WebServer.h`

Permite criar um servidor HTTP diretamente no microcontrolador.

### `ESP32Servo.h`

Permite controlar o servo motor utilizando o ESP32.

---

# 🔌 Diagrama de Ligações

## HC-SR04

| HC-SR04 | ESP32 |
|---|---|
| VCC | VIN / 5V |
| GND | GND |
| TRIG | GPIO 12 |
| ECHO | GPIO 14 através de divisor de tensão |

## Servo Motor

| Servo | ESP32 |
|---|---|
| Vermelho | 5V |
| Marrom/Preto | GND |
| Laranja/Amarelo | GPIO 13 |

---

# ⚠️ Divisor de Tensão do ECHO

O HC-SR04 trabalha com sinal de **5 V no ECHO**, enquanto os GPIOs do ESP32 trabalham em nível lógico de **3,3 V**.

Por isso, o ECHO não deve ser conectado diretamente ao GPIO.

Utilize:

- Resistor de **1 kΩ**
- Resistor de **2 kΩ**

### Esquema

```text
                1 kΩ
HC-SR04 ECHO ──/\/\/───┬──── GPIO 14
                       │
                      2 kΩ
                       │
                      GND
```

> ⚠️ **Nunca conecte diretamente o ECHO de um HC-SR04 de 5 V ao GPIO do ESP32.**

---

# 🔧 Montagem na Protoboard

## 1. Alimentação

Conecte:

```text
ESP32 VIN → trilho positivo
ESP32 GND → trilho negativo
```

O trilho positivo será utilizado para alimentação de 5 V.

---

## 2. HC-SR04

Conecte:

```text
VCC  → 5 V
GND  → GND
TRIG → GPIO 12
ECHO → divisor de tensão
```

---

## 3. Divisor de tensão

Monte:

```text
ECHO
 │
1 kΩ
 │
 ├──────── GPIO 14
 │
2 kΩ
 │
GND
```

---

## 4. Servo

Conecte:

```text
Servo VCC    → 5 V
Servo GND    → GND
Servo Sinal  → GPIO 13
```

### ⚠️ Alimentação do servo

Servos podem consumir uma corrente considerável durante o movimento.

Se o ESP32 reiniciar ou perder conexão quando o servo se movimentar, utilize uma **fonte de 5 V externa para o servo**, mantendo o GND da fonte conectado ao GND do ESP32.

---

# 💻 Configuração do Arduino IDE

## 1. Instalar o Arduino IDE

Instale o Arduino IDE e configure o suporte para placas ESP32.

---

## 2. Selecionar a placa

No Arduino IDE:

```text
Tools
 ↓
Board
 ↓
ESP32 Arduino
 ↓
ESP32 Dev Module
```

---

## 3. Selecionar a porta

Conecte o ESP32 através do USB e selecione a porta correspondente:

```text
Tools
 ↓
Port
 ↓
COMXX
```

---

# 📚 Instalação da Biblioteca do Servo

No Arduino IDE:

```text
Sketch
 ↓
Include Library
 ↓
Manage Libraries
```

Pesquise:

```text
ESP32Servo
```

Instale a biblioteca.

As bibliotecas `WiFi.h` e `WebServer.h` normalmente já acompanham o pacote do ESP32.

---

# ⚙️ Configuração do Wi-Fi

O ESP32 cria sua própria rede Wi-Fi utilizando:

```cpp
const char* ssid = "RADAR-ESP32";
const char* senha = "12345678";
```

Não é necessário possuir um roteador.

Quando o ESP32 iniciar, ele criará:

```text
Rede: RADAR-ESP32
Senha: 12345678
```

---

# 🌐 Servidor Web

O servidor é iniciado através de:

```cpp
WebServer servidor(80);
```

A porta utilizada é:

```text
80
```

A página principal está disponível em:

```text
/
```

Enquanto os dados são disponibilizados em:

```text
/dados
```

---

# 📡 API `/dados`

O ESP32 retorna os dados através de JSON.

Exemplo:

```json
{
  "angulo": 90,
  "distancia": 42,
  "status": "🎯 Objeto detectado"
}
```

A interface JavaScript consulta esse endpoint continuamente:

```javascript
fetch("/dados")
```

Isso permite atualizar a interface sem recarregar a página.

---

# 📏 Funcionamento do HC-SR04

O ESP32 envia um pulso de aproximadamente **10 μs** pelo TRIG:

```cpp
digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);

digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);

digitalWrite(TRIG_PIN, LOW);
```

O sensor retorna um pulso pelo ECHO proporcional ao tempo que o ultrassom levou para atingir o objeto e retornar.

O código calcula:

```cpp
distancia = duracao * 0.0343 / 2;
```

### Por que dividir por 2?

Porque o ultrassom percorre:

```text
Sensor → Objeto
       +
Objeto → Sensor
```

O tempo medido corresponde ao percurso completo.

Portanto:

```text
distância = tempo × velocidade / 2
```

---

# 🔄 Sistema de Varredura

O servo percorre:

```text
0°
 ↓
1°
 ↓
2°
 ↓
...
180°
```

Em cada posição:

1. O servo é movimentado.
2. O ESP32 aguarda o posicionamento.
3. O HC-SR04 realiza uma medição.
4. A distância é armazenada.
5. O ângulo é associado à distância.
6. O servidor disponibiliza os dados.

Depois da varredura:

```text
180°
 ↓
179°
 ↓
...
0°
```

O processo continua indefinidamente.

---

# 🎯 Identificação do Objeto Mais Próximo

Durante a varredura, o sistema compara as distâncias:

```cpp
if (
  distanciaAtual < menorDistancia &&
  distanciaAtual > 2
) {
  menorDistancia = distanciaAtual;
  melhorAngulo = angulo;
}
```

Assim, o sistema identifica:

```text
Menor distância
       +
Ângulo correspondente
```

Por exemplo:

```text
Objeto detectado

Ângulo: 117°
Distância: 32 cm
```

Após terminar a varredura, o servo é direcionado para:

```text
117°
```

---

# 🖥️ Interface Web

A interface foi desenvolvida utilizando:

- HTML5
- CSS3
- JavaScript

Não é necessário instalar nenhum framework.

A interface apresenta:

### Radar

- Círculos de distância
- Linha de varredura
- Centro do radar
- Posição do objeto
- Animação do alvo

### Informações

```text
Ângulo atual
Distância atual
Status
```

### Histórico

O sistema mantém as últimas leituras recebidas pelo navegador.

Exemplo:

```text
21:42:15    90°    32 cm
21:42:15    91°    31 cm
21:42:16    92°    30 cm
```

---

# 🌈 Sistema de Cores

A interface muda a cor do objeto de acordo com a distância.

| Distância | Cor | Significado |
|---:|---|---|
| 0–20 cm | 🔴 Vermelho | Perigo |
| 20–50 cm | 🟠 Laranja | Atenção |
| 50–100 cm | 🟡 Amarelo | Médio |
| 100–200 cm | 🟢 Verde | Seguro |

---

# 🚨 Sistema de Status

### Objeto muito próximo

```text
🚨 PERIGO — OBJETO MUITO PRÓXIMO
```

### Objeto próximo

```text
⚠️ ATENÇÃO — OBJETO PRÓXIMO
```

### Área segura

```text
🟢 ÁREA SEGURA
```

---

# 📱 Acesso pelo Celular ou Computador

## 1. Abra o Monitor Serial

Utilize:

```text
115200 baud
```

Você deverá encontrar algo semelhante a:

```text
==============================
     RADAR ESP32
==============================
Rede: RADAR-ESP32
Senha: 12345678
IP: 192.168.4.1
==============================
Servidor iniciado!
```

---

## 2. Conecte seu dispositivo à rede

No celular ou computador:

```text
Wi-Fi
 ↓
RADAR-ESP32
```

Senha:

```text
12345678
```

---

## 3. Abra o navegador

Digite o IP mostrado no Monitor Serial.

Normalmente:

```text
http://192.168.4.1
```

---

# 🧪 Teste do Sistema

Faça os testes nesta ordem:

### Teste 1 — ESP32

Verifique se o código é carregado sem erros.

### Teste 2 — Wi-Fi

Verifique se a rede:

```text
RADAR-ESP32
```

aparece.

### Teste 3 — Servo

O servo deve realizar a varredura:

```text
0° → 180°
```

### Teste 4 — HC-SR04

Abra o Monitor Serial e verifique:

```text
Angulo: 45° | Distancia: 63 cm
Angulo: 46° | Distancia: 61 cm
Angulo: 47° | Distancia: 59 cm
```

### Teste 5 — Interface

Acesse o IP do ESP32 e confirme:

- Ângulo atualizando
- Distância atualizando
- Radar movimentando
- Ponto aparecendo
- Histórico sendo preenchido
- Cores mudando

---

# 🐛 Solução de Problemas

## Sensor mostra `400 cm`

Verifique:

- VCC do HC-SR04
- GND
- TRIG
- ECHO
- Resistores
- GPIO configurado no código

Principalmente:

```text
TRIG → GPIO 12
ECHO → divisor → GPIO 14
```

---

## Servo não movimenta

Verifique:

```text
Sinal → GPIO 13
VCC → 5 V
GND → GND
```

Também confirme se a biblioteca:

```text
ESP32Servo
```

está instalada.

---

## ESP32 reinicia quando o servo movimenta

Isso normalmente indica problema de alimentação do servo.

Utilize uma fonte de 5 V adequada para o servo e conecte:

```text
Fonte GND ───── ESP32 GND
```

---

## Site não abre

Verifique:

1. Se o ESP32 está ligado.
2. Se o celular/computador está conectado à rede `RADAR-ESP32`.
3. O IP mostrado no Monitor Serial.
4. Se o servidor foi iniciado.

---

# 📁 Estrutura Recomendada do Repositório

```text
radar-esp32/
│
├── radar-esp32.ino
│
├── README.md
│
├── docs/
│   ├── circuito.png
│   ├── protoboard.png
│   └── arquitetura.png
│
└── images/
    ├── radar-interface.png
    └── projeto-final.jpg
```

---

# 🔮 Possíveis Melhorias

- [ ] Histórico persistente
- [ ] Gráfico de distância
- [ ] Detecção de múltiplos objetos
- [ ] Filtro de ruído
- [ ] Limite de distância configurável
- [ ] Buzzer de alerta
- [ ] LEDs indicadores
- [ ] Sensor de temperatura para compensação da velocidade do som
- [ ] Controle manual do servo
- [ ] Modo automático/manual
- [ ] Registro das leituras
- [ ] Interface mobile aprimorada
- [ ] MQTT
- [ ] Banco de dados
- [ ] Dashboard remoto
- [ ] Integração com outros sensores

---

# 📊 Especificações

| Característica | Valor |
|---|---|
| Microcontrolador | ESP32 |
| Sensor | HC-SR04 |
| Atuador | Servo SG90 |
| Varredura | 0°–180° |
| Interface | Web |
| Comunicação | Wi-Fi |
| Protocolo | HTTP |
| Formato dos dados | JSON |
| Front-end | HTML/CSS/JavaScript |
| Backend | ESP32 WebServer |
| Alimentação HC-SR04 | 5 V |
| Divisor ECHO | 1 kΩ + 2 kΩ |

---

# 👨‍💻 Autor

**Walter Prestes**

Projeto desenvolvido como estudo prático de:

> **ESP32 + IoT + Sistemas Embarcados + Desenvolvimento Web**

---

# 📜 Licença

Este projeto pode ser utilizado para fins educacionais e de estudo.

Sinta-se livre para modificar, melhorar e expandir o sistema.

---

## ⭐ Resultado Esperado

Ao final, o projeto deverá funcionar como um radar físico conectado a uma interface web:

```text
                📡 RADAR ESP32

                    90°
                     │
              ╭──────●──────╮
           ╭──╯      │      ╰──╮
          │          │          │
          │      🟢  │          │
          │          │          │
           ╰─────────┼─────────╯
                     │

        ┌─────────────────────────┐
        │ ÂNGULO      90°         │
        │ DISTÂNCIA  64 cm        │
        │                         │
        │ 🟢 ÁREA SEGURA         │
        └─────────────────────────┘

        HISTÓRICO
        90°  → 64 cm
        91°  → 63 cm
        92°  → 61 cm
```
