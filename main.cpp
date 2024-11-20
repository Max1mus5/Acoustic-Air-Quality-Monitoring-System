/*
 * Ondas Acústicas y su Aplicación en la Medición de Calidad del Aire
 * Acoustic Waves and their Application in Air Quality Measurement
 * 
 * Universidad Tecnológica de Pereira
 * 
 * Componentes utilizados:
 * - ESP32
 * - Micrófono MAX9814
 * - Sensor MQ-135 (CO2)
 * - Altavoz 8Ω
 * 
 * Este sistema implementa un método de detección de contaminantes
 * basado en la propagación de ondas acústicas y su interacción
 * con el medio, complementado con sensores de gas.
 */

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Definición de pines y constantes del sistema
#define SPEAKER_PIN 25     // Pin del altavoz
#define GAS_SENSOR 34      // Pin del sensor de gas
#define MIC_PIN 32         // Pin del micrófono MAX9814

// Constantes para el análisis acústico
const int BUFFER_SIZE = 1024;
const int SAMPLE_RATE = 8000;
const int AVERAGE_WINDOW_SIZE = 5;  // Ventana de promedio móvil

// Variables para el muestreo acústico
int16_t audioBuffer[BUFFER_SIZE];
float averageValues[AVERAGE_WINDOW_SIZE];
int averageIndex = 0;
float totalAverage = 0;

// Variables para el control del sistema
BlynkTimer timer;
String inputString = "";
boolean newData = false;
unsigned long toneStartTime = 0;
boolean isTonePlaying = false;
int frequency = 1000;

// Credenciales de conexión
char auth[] = "7WyWzD2ASiKPP1FnOz8VS3W8pnG6HSml";
char ssid[] = "RIVEROS";
char pass[] = "1088239515";

// Función para reproducir tonos de prueba
void playTone(int freq) {
  toneStartTime = millis();
  isTonePlaying = true;
  frequency = freq;
}

void setup() {
  Serial.begin(115200);
  
  // Configuración de pines
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(GAS_SENSOR, INPUT);
  
  // Configuración del ADC para el micrófono
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  // Inicialización del buffer de promedios
  for (int i = 0; i < AVERAGE_WINDOW_SIZE; i++) {
    averageValues[i] = 0;
  }
  
  // Periodo de calentamiento para sensores
  Serial.println("Periodo de calentamiento (60s)...");
  delay(60000);

  // Inicialización de Blynk
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  Serial.println("Sistema iniciado");
  Serial.println("Control manual de frecuencia: Ingrese un número (20-20000 Hz)");
  Serial.println("Alarma automática: 18000Hz cuando nivel de gas > 50");
}

// Análisis de niveles de gas
void GASLevel() {
  int value = analogRead(GAS_SENSOR);
  value = map(value, 0, 4095, 0, 100);

  if (value >= 50) {
    playTone(18000);
    WidgetLED LED(V1);
    LED.on();
  } else {
    WidgetLED LED(V1);
    LED.off();
  }

  Blynk.virtualWrite(V0, value);
  Serial.print("Nivel de gas: ");
  Serial.println(value);
}

// Análisis acústico del medio
void analyzeAcoustics() {
  float currentAverage = 0;
  
  // Muestreo y promedio
  for(int i = 0; i < BUFFER_SIZE; i++) {
    audioBuffer[i] = analogRead(MIC_PIN);
    currentAverage += audioBuffer[i];
    delayMicroseconds(125);
  }
  
  currentAverage = currentAverage / BUFFER_SIZE;
  
  // Actualización del promedio móvil
  totalAverage -= averageValues[averageIndex];
  averageValues[averageIndex] = currentAverage;
  totalAverage += currentAverage;
  averageIndex = (averageIndex + 1) % AVERAGE_WINDOW_SIZE;
  
  float last5Average = totalAverage / AVERAGE_WINDOW_SIZE;
  
  // Análisis de amplitud pico a pico
  int16_t maxSample = -32768;
  int16_t minSample = 32767;
  
  for(int i = 0; i < BUFFER_SIZE; i++) {
    if(audioBuffer[i] > maxSample) maxSample = audioBuffer[i];
    if(audioBuffer[i] < minSample) minSample = audioBuffer[i];
  }
  
  float peakToPeak = maxSample - minSample;
  
  Serial.print("Promedio móvil: ");
  Serial.print(last5Average);
  Serial.print(" | Amplitud P-P: ");
  Serial.println(peakToPeak);
}

void loop() {
  // Control manual de frecuencia
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    if (isDigit(inChar)) {
      inputString += inChar;
    }
    if (inChar == '\n') {
      newData = true;
    }
  }

  if (newData) {
    if (inputString.length() > 0) {
      int newFreq = inputString.toInt();
      if (newFreq >= 20 && newFreq <= 20000) {
        playTone(newFreq);
        Serial.print("Nueva frecuencia: ");
        Serial.println(newFreq);
      }
    }
    inputString = "";
    newData = false;
  }

  // Generación de tono
  if (isTonePlaying) {
    if (millis() - toneStartTime < 1000) {
      digitalWrite(SPEAKER_PIN, HIGH);
      delayMicroseconds(500000/frequency);
      digitalWrite(SPEAKER_PIN, LOW);
      delayMicroseconds(500000/frequency);
    } else {
      digitalWrite(SPEAKER_PIN, LOW);
      isTonePlaying = false;
    }
  }

  // Ejecución de análisis
  GASLevel();
  analyzeAcoustics();
  //Blynk.run();
  
  // Delay solo si no hay tono activo
  if (!isTonePlaying) {
    delay(200);
  }
}