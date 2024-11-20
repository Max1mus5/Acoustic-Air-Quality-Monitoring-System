# Acoustic Air Quality Monitoring System

## Descripción
Este proyecto implementa un sistema innovador para la detección y monitoreo de contaminantes en el aire utilizando ondas acústicas, basado en la investigación "Ondas Acústicas y su Aplicación en la Medición de Calidad del Aire" de la Universidad Tecnológica de Pereira. El sistema combina análisis acústico con sensores tradicionales para proporcionar una evaluación más completa de la calidad del aire.

## Características Principales
- Detección dual: análisis acústico + sensor de gas
- Monitoreo en tiempo real
- Sistema de alarma inteligente
- Interfaz IoT mediante Blynk
- Control manual de frecuencias acústicas
- Análisis de atenuación sonora

## Componentes de Hardware
- ESP32 (Microcontrolador principal)
- Micrófono MAX9814 (Análisis acústico)
- Sensor MQ-135 (Detección de CO2)
- Altavoz 8Ω (Generación de ondas acústicas)

## Diagrama de Conexiones
```
ESP32           Componentes
GPIO25    -->   Altavoz
GPIO34    -->   MQ-135 (CO2)
GPIO32    -->   MAX9814 (Mic)
5V      -->   VCC (todos los componentes)
GND       -->   GND (todos los componentes)
```

## Fundamento Científico
El sistema se basa en tres principios fundamentales:

1. **Impedancia Acústica (Z = ρc)**
   - Medición de cambios en la densidad del medio
   - Análisis de propagación de ondas sonoras

2. **Atenuación Acústica**
   - Monitoreo de pérdida de energía sonora
   - Detección de partículas suspendidas

3. **Análisis de Gas**
   - Medición directa de niveles de CO2
   - Correlación con datos acústicos

## Características del Software
- Frecuencia de muestreo: 8000 Hz
- Buffer de audio: 1024 muestras
- Ventana de promedio móvil: 5 muestras
- Resolución ADC: 12 bits
- Rango de frecuencias: 20-20000 Hz

## Instalación

### Requisitos Previos
1. Arduino IDE
2. Biblioteca Blynk
3. Biblioteca ESP32 para Arduino

### Configuración
1. Clonar el repositorio:
```bash
git clone https://github.com/yourusername/acoustic-air-quality
```

2. Abrir el archivo `.ino` en Arduino IDE

3. Configurar credenciales:
```cpp
char auth[] = "TU_TOKEN_BLYNK";
char ssid[] = "TU_WIFI_SSID";
char pass[] = "TU_WIFI_PASSWORD";
```

4. Cargar el código al ESP32

## Uso
1. Al iniciar, el sistema requiere 60 segundos de calentamiento
2. El monitoreo comienza automáticamente después del calentamiento
3. Controles disponibles:
   - Monitor serial: entrada manual de frecuencias (20-20000 Hz)
   - App Blynk: visualización de datos en tiempo real
   - LED indicador: se activa cuando los niveles de gas superan el umbral

## Dashboard Blynk
- V0: Nivel de gas (0-100%)
- V1: LED de alarma

## Calibración
1. Colocar el sistema en un ambiente limpio
2. Esperar el periodo de calentamiento completo
3. Los primeros valores servirán como referencia base

## Contribución
Las contribuciones son bienvenidas. Por favor, seguir estos pasos:
1. Fork del repositorio
2. Crear una rama para features (`git checkout -b feature/AmazingFeature`)
3. Commit de cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abrir un Pull Request

## Referencias
- Pierce, A. D. (2019). Acoustics: An Introduction to Its Physical Principles and Applications
- Morse, P. M., & Ingard, K. U. (1986). Theoretical Acoustics

## Licencia
Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE.md](LICENSE.md) para detalles.

## Autores
- Jeronimo Riveros, Esteban Sanchez
- Universidad Tecnológica de Pereira

## Agradecimientos
- Universidad Tecnológica de Pereira
- Phd. Sebastian Velasquez Bonilla - Por su guía y enseñanzas