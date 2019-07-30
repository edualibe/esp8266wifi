#include <ArduinoJson.h>

//#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

char auth[] = "Iw5eS_2dkuch2aKBXkKpT_zyrrbHJTF8";

#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

char ssid[] = "AA-2.4";
char pass[] = "Aaron202014";

unsigned long tiempo_inicial = millis();
int intervalo_lectura = 600000; //600000 = 10 minutos
//int busqueda_inicial = 0;
//int busqueda_final = 0;
//String valor_temperatura = "";
bool primera_lectura = false;

void setup(){

 delay(10);
  Serial.begin(115200);


  WiFi.begin(ssid, pass);

  //Serial.println("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    //Serial.print(".");
  }

  //Serial.println("Conectado con éxito, mi IP es: ");
  //Serial.println(WiFi.localIP());
  Blynk.begin(auth, ssid, pass);

}

void loop(){
  Blynk.run();
  if ((millis()>(tiempo_inicial + intervalo_lectura)) || (!primera_lectura) ){
    primera_lectura = true;
    tiempo_inicial = millis();
    if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

      HTTPClient http;
      //http.begin("http://www.devs.com.ar/smn/?ciudad=corrientes");        //Indicamos el destino
      http.begin("http://api.openweathermap.org/data/2.5/weather?q=corrientes,ar&APPID=d30ec5a270b36c0fb62dedc15fd1a199");        //Indicamos el destino
      //http.begin("http://jsonplaceholder.typicode.com/users/2");        //Indicamos el destino
      //http.addHeader("Content-Type", "plain-text"); //Preparamos el header text/plain si solo vamos a enviar texto plano sin un paradigma llave:valor.
  
      int codigo_respuesta = http.GET();   //Enviamos el post pasándole, los datos que queremos enviar. (esta función nos devuelve un código que guardamos en un int)
  
      if(codigo_respuesta>0){
        //Serial.println("Código HTTP ► " + String(codigo_respuesta));   //Print return code
        if(codigo_respuesta == 200){
          //String cuerpo_respuesta = http.getString();
          //delay(50);
          //Serial.println("El servidor respondió ▼ ");
          //Serial.println(cuerpo_respuesta);
          //valor_temperatura = "";
          //busqueda_inicial = cuerpo_respuesta.indexOf("TEMPERATURA:</span>");          
          //busqueda_inicial = cuerpo_respuesta.indexOf("TEMPERATURA:</span>");
          //delay(50);
          //busqueda_final = cuerpo_respuesta.indexOf("<br>",busqueda_inicial+1);
          //delay(50);
          //valor_temperatura = cuerpo_respuesta.substring(busqueda_final,busqueda_inicial+20);
          //valor_temperatura = cuerpo_respuesta.substring(busqueda_inicial);
          //Serial.println(valor_temperatura);
          //Serial.println(cuerpo_respuesta);

          // Parsing
          const size_t bufferSize = JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(8) + 370;
          DynamicJsonBuffer jsonBuffer(bufferSize);
          JsonObject& data = jsonBuffer.parseObject(http.getString());

          double temp = data["main"]["temp"];
          double hum = data["main"]["humidity"];
          double temp_min = data["main"]["temp_min"];
          double temp_max = data["main"]["temp_max"];
          temp = temp - 273.15; //convierte kelvin a centigrados
          temp_min = temp_min - 273.15; //convierte kelvin a centigrados
          temp_max = temp_max - 273.15; //convierte kelvin a centigrados
          String valor_temperatura = String(temp);
          String valor_humedad = String(hum);
          String valor_temperaturamin = String(temp_min);
          String valor_temperaturamax = String(temp_max);
          Serial.println("");
          Serial.print("T:");
          Serial.println(valor_temperatura);
          Serial.print("H:");
          Serial.println(valor_humedad);        
          Serial.print("TMn:");
          Serial.println(valor_temperaturamin);
          Serial.print("TMx:");
          Serial.println(valor_temperaturamax);
        
        }
      }
      http.end();  //libero recursos
    }else{
      Serial.println("Error en la conexión WIFI");
    }
  }
}
