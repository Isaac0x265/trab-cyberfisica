# Guia de montagem: Detector de Barulho (ESP32)

## Peças

- 1 ESP32 DevKit
- 1 protoboard
- 3 LEDs: verde, amarelo e vermelho
- 3 resistores de 220 Ω (faixas vermelho, vermelho, marrom)
- 1 potenciômetro (3 pernas)
- 1 buzzer
- Jumpers

---

## Antes de tudo: o LED

```
        ___
       /   \
      |     |   <- lado da cabeça com a borda RETA = negativo
       \___/
        | |
        | |
        | |      PÉ CURTO = NEGATIVO (−) = vai no GND
        |
        |        PÉ LONGO = POSITIVO (+) = vai no resistor, que vai no pino do ESP32
```

**Regra: pé longo vai para o pino do ESP32, pé curto vai para o GND.**

Macete: o pé **longo** é o **mais** (+), porque "tem mais perna".

Se as pernas foram cortadas, olhe a borda da cabeça do LED: o lado **reto** é o pé curto (−).

LED ao contrário não queima, só não acende. Se não acender, inverta.

---

## O resistor

- Não tem lado certo, pode ligar de qualquer jeito.
- Cada LED precisa de um. Ele fica entre o pino do ESP32 e o pé longo do LED.

---

## Ligação de cada LED

Os três LEDs são ligados do mesmo jeito, só muda o pino:

```
 Pino do ESP32 ──── resistor 220Ω ──── PÉ LONGO do LED
                                       PÉ CURTO do LED ──── GND
```

| LED | Pino do ESP32 | Pé longo (+) | Pé curto (−) |
|---|---|---|---|
| Verde | **25** | resistor, depois pino 25 | GND |
| Amarelo | **26** | resistor, depois pino 26 | GND |
| Vermelho | **27** | resistor, depois pino 27 | GND |

> Na placa o pino pode estar escrito como `25`, `D25`, `G25` ou `IO25`. É tudo a mesma coisa.

---

## Potenciômetro

Ele tem 3 pernas. A **do meio** é a que importa.

```
     _______
    |       |
    |   O   |   <- botão que gira
    |_______|
     |  |  |
     |  |  |
    3V3 34 GND
```

| Perna | Liga em |
|---|---|
| Esquerda | **3V3** (nunca no 5V) |
| **Meio** | **pino 34** |
| Direita | **GND** |

> Se inverter as pernas das pontas não estraga nada, só muda o lado para onde você gira para aumentar.

---

## Buzzer

| Perna | Liga em |
|---|---|
| **+** (perna longa, ou marcada com "+") | **pino 14** |
| **−** (perna curta) | **GND** |

---

## GND

Tudo que diz "GND" acaba no mesmo lugar. Dica: ligue um jumper do GND do ESP32 na **linha azul (−)** da protoboard e ligue todos os GNDs nessa linha.

```
 GND do ESP32 ───┐
                 │
 ════════════════╧═════════════════════  linha azul (−) da protoboard
    │         │          │        │       │
 pé curto  pé curto  pé curto  buzzer   potenciômetro
  verde    amarelo   vermelho    (−)      (direita)
```

---

## Resumo: tudo numa tabela

| Componente | Pino do ESP32 | Observação |
|---|---|---|
| LED verde | 25 | pé longo no resistor, pé curto no GND |
| LED amarelo | 26 | pé longo no resistor, pé curto no GND |
| LED vermelho | 27 | pé longo no resistor, pé curto no GND |
| Buzzer | 14 | + no 14, − no GND |
| Potenciômetro (meio) | 34 | pontas no 3V3 e no GND |

---

## Checklist antes de ligar o USB

- [ ] Os 3 LEDs com o **pé longo** virado para o resistor
- [ ] Os 3 resistores no caminho (nenhum LED ligado direto no pino)
- [ ] Potenciômetro no **3V3**, e não no 5V
- [ ] Perna do **meio** do potenciômetro no pino 34
- [ ] Todos os GNDs ligados juntos

## Se não funcionar

| Problema | O que olhar |
|---|---|
| Um LED não acende | Está invertido: troque os pés de lado |
| Nenhum LED acende | O GND do ESP32 não está ligado na linha da protoboard |
| LED não muda ao girar | A perna do meio do potenciômetro não está no pino 34 |
| Buzzer não apita | Inverta + e −. Se for buzzer passivo, precisa de `tone()` no código |
