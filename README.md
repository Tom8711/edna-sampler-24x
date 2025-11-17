# eDNA Sampler 24x

Dit project bevat de volledige broncode, firmware, hardware-documentatie en backend-services voor de **eDNA Sampler 24x**.  
Het doel is een modulair, schaalbaar systeem dat 24 afzonderlijke pompkanalen kan aansturen, data kan loggen en via 4G of WiFi kan communiceren.

---

## Projectstructuur

.
├── backend/ # Backend/API, dataprocessing, dashboards
├── data/ # Testdata, kalibratiegegevens, voorbeeldmetingen
├── docs/ # Architectuur, schema’s, flow-diagrammen
├── firmware/ # Microcontrollercode (ESP32/SIM7670G)
├── hardware/ # Schema’s, PCB, pinouts, aansluitdiagrammen
├── tests/ # Unit/integration testing (optioneel)
└── tools/ # Hulpscripts voor build, deployment, logs


---

## Getting Started

### 1. Repository clonen

```bash
git clone https://github.com/Tom8711/edna-sampler-24x.git
cd edna-sampler-24x

2. Vereisten

Afhankelijk van welke module je gebruikt:

Firmware: PlatformIO / Arduino IDE / ESP-IDF

Backend: Python of Node.js (volgt in docs)

Hardware: KiCad of Fritzing (volgt in docs)

3. Project bouwen

Uitleg volgt zodra de eerste code is toegevoegd.

### Architectuur

Het systeem bestaat uit:

Hardware
ESP32-S3 + SIM7670G 4G module, MOSFET-drivers, 24-pompsysteem.

Firmware
Besturing pompkanalen, scheduler, telemetrie, foutafhandeling.

Backend (optioneel)
Opslag van metingen, identiteit pompmodules, dashboarding.

Lees meer in:
docs/architecture.md

-Contributie

Workflow:

Nieuwe feature → nieuwe branch feature/...

Commit-conventie:

feat: nieuwe functionaliteit

fix: bugfix

docs: documentatie

chore: niet-functionele wijzigingen

-Pull request naar main

Licentie

De licentie volgt later.


---

# **2. docs/architecture.md – Architectuurskelet**

```markdown
# Architectuur – eDNA Sampler 24x

Dit document beschrijft de high-level architectuur van het eDNA Sampler 24x systeem: hardware, firmware en backend.

---

## 1. Overzicht

Het systeem stuurt 24 pompkanalen individueel aan en logt/rapporteert sensordata.
Het is ontworpen voor gebruik in het veld: compact, modulair en betrouwbaar.

Hoofdonderdelen:

- Hardware (ESP32-S3, SIM7670G, MOSFET-board, voeding)
- Firmware (pompbesturing, timing, telemetrie)
- Backend (optioneel: datastorage & dashboard)
- Tools (scripts voor calibratie, logging, testen)

---

## 2. Hardware Architectuur

### 2.1 Componenten

- **ESP32-S3**
  Hoofdcontroller, WiFi/BLE/4G via SIM7670G.

- **SIM7670G 4G module**
  Voor mobiele datacommunicatie.

- **MOSFET-driverboard**
  Schakelt 24 pompen (aan/uit, eventueel polariteit).

- **Voeding**
  5V → conversie naar 3.3V voor ESP32, 12V/24V voor pompen.

### 2.2 Blokschema (tekstueel)

             ↓
       [SIM7670G 4G]
             ↓
         [Backend]


---

## 3. Firmware Architectuur

### 3.1 Modules

- **PumpController**
  Aansturing van alle 24 kanalen.

- **Scheduler**
  Taken (sampling, intervals) op tijd laten lopen.

- **Telemetry**
  Versturen van data via 4G/WiFi.

- **ErrorManager**
  Foutdetectie, beveiliging, watchdog.

### 3.2 Datastromen

1. Scheduler → bepaalt welke pomp actief wordt
2. PumpController → voert aan/uit uit
3. Telemetry → stuurt status + sensorinformatie naar backend
4. Backend → slaat data op en toont deze in dashboard

---

## 4. Backend Architectuur (optioneel)

### 4.1 Componenten

- API (Python/Node)
- Database (SQLite/PostgreSQL)
- Dashboard (web-based)

### 4.2 Functies

- Ontvangen van metingen via HTTP/MQTT
- Opslag en tijdreeksanalyse
- Weergave systeemstatus / foutmeldingen

---

## 5. Tools

`/tools` bevat scripts voor:

- uitlezen seriële logs
- initialisatie hardware
- testen I/O-kanalen
- firmware uploaden (automatisering)

---

## 6. Toekomstige uitbreidingen

- Over-the-air firmware updates (OTA)
- Machine learning op sensordata
- Extra modules voor temperatuur/drukmetingen
- Configuratie via webinterface

---

Dit document wordt verder uitgebreid zodra de eerste hardware- en firmwaredefinities concreet zijn.

