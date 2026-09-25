#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <math.h>

#define BLYNK_TEMPLATE_ID "TMPL3BPebRGLn"
#define BLYNK_TEMPLATE_NAME "MOTOR MONITORING"
#define BLYNK_AUTH_TOKEN "pscF2L_95SxZAQvN0qA03x5umZjnCBT_"

#include <BlynkSimpleEsp32.h>

/************** WIFI **************/
const char* ssid = "Mi 11X";
const char* password = "abcd1234";

/************** MQTT **************/
const char* mqtt_server = "10.246.24.151";
const int mqtt_port = 1883;

const char* topic_temp = "motor/temp";
const char* topic_vib = "motor/vibration";
const char* topic_status = "motor/status";

/************** NTC **************/
#define NTC_PIN 34
#define ADC_MAX 4095.0
#define R_FIXED 10000.0
#define R0 10000.0
#define BETA 3950.0
#define T0 298.15

/************** MPU **************/
#define MPU_ADDR 0x68

int16_t ax, ay, az, gx, gy, gz;
long ax_offset, ay_offset, az_offset;
long gx_offset, gy_offset, gz_offset;

/************** GLOBAL DATA **************/
volatile float g_temp = 0;
volatile float g_vibration = 0;
volatile int g_status = 0;

/************** NETWORK **************/
WiFiClient espClient;
PubSubClient client(espClient);

/************** WIFI **************/
void connectWiFi()
{
  Serial.print("Connecting WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
}

/************** MQTT **************/
void connectMQTT()
{
  while (!client.connected())
  {
    Serial.print("Connecting MQTT...");

    if (client.connect("ESP32_MOTOR"))
      Serial.println("connected");
    else
    {
      Serial.print("failed rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

/************** TEMP **************/
float readTemperature()
{
  int adc = analogRead(NTC_PIN);

  if (adc <= 0) adc = 1;
  if (adc >= 4095) adc = 4094;

  float r_ntc = R_FIXED * ((float)adc / (ADC_MAX - adc));
  float tempK = 1.0 / ((1.0 / T0) + (1.0 / BETA) * log(r_ntc / R0));

  return tempK - 273.15;
}

/************** MPU CALIBRATION **************/
void calibrateMPU()
{
  long ax_sum=0, ay_sum=0, az_sum=0;
  long gx_sum=0, gy_sum=0, gz_sum=0;

  for(int i=0;i<2000;i++)
  {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR,14,true);

    ax = Wire.read()<<8 | Wire.read();
    ay = Wire.read()<<8 | Wire.read();
    az = Wire.read()<<8 | Wire.read();

    Wire.read(); Wire.read();

    gx = Wire.read()<<8 | Wire.read();
    gy = Wire.read()<<8 | Wire.read();
    gz = Wire.read()<<8 | Wire.read();

    ax_sum += ax;
    ay_sum += ay;
    az_sum += az;

    gx_sum += gx;
    gy_sum += gy;
    gz_sum += gz;

    delay(2);
  }

  ax_offset = ax_sum/2000;
  ay_offset = ay_sum/2000;
  az_offset = (az_sum/2000) - 16384;

  gx_offset = gx_sum/2000;
  gy_offset = gy_sum/2000;
  gz_offset = gz_sum/2000;
}

/************** TASKS **************/

void TemperatureTask(void *pvParameters)
{
  while (1)
  {
    g_temp = readTemperature();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void MPUSensorTask(void *pvParameters)
{
  while (1)
  {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR,14,true);

    ax = Wire.read()<<8 | Wire.read();
    ay = Wire.read()<<8 | Wire.read();
    az = Wire.read()<<8 | Wire.read();

    Wire.read(); Wire.read();

    gx = Wire.read()<<8 | Wire.read();
    gy = Wire.read()<<8 | Wire.read();
    gz = Wire.read()<<8 | Wire.read();

    ax -= ax_offset;
    ay -= ay_offset;
    az -= az_offset;

    float ax_g = ax / 16384.0;
    float ay_g = ay / 16384.0;
    float az_g = az / 16384.0;

    float magnitude = sqrt(ax_g*ax_g + ay_g*ay_g + az_g*az_g);
    g_vibration = fabs(magnitude - 1.0);

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void FaultTask(void *pvParameters)
{
  while (1)
  {
    if (g_temp > 75)
      g_status = 2;
    else if (g_vibration > 0.08)
      g_status = 1;
    else
      g_status = 0;

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void MQTTTask(void *pvParameters)
{
  char msg[50];

  while (1)
  {
    if (!client.connected())
      connectMQTT();

    client.loop();

    dtostrf(g_temp,4,2,msg);
    client.publish(topic_temp,msg);

    dtostrf(g_vibration,4,4,msg);
    client.publish(topic_vib,msg);

    sprintf(msg,"%d",g_status);
    client.publish(topic_status,msg);

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

/************** BLYNK TASK **************/
void BlynkTask(void *pvParameters)
{
  static int prevStatus = -1;

  while (1)
  {
    Blynk.run();

    Blynk.virtualWrite(V0, String(g_temp,2));
    Blynk.virtualWrite(V1, String(g_vibration,4));
    Blynk.virtualWrite(V2, g_status);

    if(g_status != prevStatus)
    {
      if(g_status == 1)
      {
        Blynk.logEvent("high_vibration", "Status changed: HIGH VIBRATION");
      }
      else if(g_status == 2)
      {
        Blynk.logEvent("overheat_alert", "Status changed: OVERHEAT");
      }
      else if(g_status == 0)
      {
        Blynk.logEvent("system_normal", "Status changed: NORMAL");
      }

      prevStatus = g_status;
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

/************** SETUP **************/
void setup()
{
  Serial.begin(115200);

  analogReadResolution(12);

  Wire.begin(21,22);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  calibrateMPU();

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  xTaskCreate(TemperatureTask,"TempTask",4096,NULL,1,NULL);
  xTaskCreate(MPUSensorTask,"MPUTask",4096,NULL,1,NULL);
  xTaskCreate(FaultTask,"FaultTask",4096,NULL,1,NULL);
  xTaskCreate(MQTTTask,"MQTTTask",4096,NULL,1,NULL);
  xTaskCreate(BlynkTask,"BlynkTask",4096,NULL,1,NULL);
}

void loop(){}