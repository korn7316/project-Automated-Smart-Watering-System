/**************************************
 * www.arduinona.com                  |
 * ตัวอย่างการดึงค่าสภาพอากาศจากอินเตอร์เน็ต  |
 **************************************/


/**************************************
 * ใช้สำหรับอ่านข้อมูลไฟล์ Json             |
 **************************************/
    #include <ArduinoJson.h>

/*********************|
 * ESP8266            |
 *********************/
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266WebServer.h>

/*********************|
 * ตั้งค่า WiFiที่ใช้       |
 *********************/
    const char* ssid = "LastlB";
    const char* password = "lonieusO12";

/*********************|
 * Token จากสถานี      |
 *********************/
    const char* weatherKey = "cdaba8bb1edb6e9077449a64cf23c48b";


/***********************************************************************************|
 * ตั้งค่า Server port (default คือ 80 ถ้าไม่ใช่ 80 เช่น 90 เวลาเข้าเว็ปต้องใส่ 192.168.xx.xx:90  |
 ***********************************************************************************/
    ESP8266WebServer server(80);

/************************|
 * ฟังก์ชันอ่านค่าสภาพอากาศ  |
 ************************/
    const char* getWeatherCondition(){

      const char* condition;
      if (WiFi.status() == WL_CONNECTED) {

  
          
    /*************************************************|
     * สร้าง client เพื่อดึงข้อมูลจาก URL ที่เราได้ลองในตอนที่ 1  |
     *************************************************/
 
        HTTPClient http;
        http.begin("http://api.openweathermap.org/data/2.5/weather?q=phuket&units=metric&appid="+String(weatherKey));
        int httpCode = http.GET();
     
        if (httpCode > 0) {
        /*****************************************************|
         * รับค่า json ที่ได้จากเว็ปข้างบน มาใส่ลงใน string ชื่อ payload |
         *****************************************************/
            String payload = http.getString();

        /*****************************************************|
         * จองพื้นที่หน่วยความจำ (200 ตัวอักษรก็น่าจะเพียงพอ)           |
         * แปลง payload ให้เป็น objectของ arduinoJson ชื่อว่า root  |
         * ซึ่ง root หมายถึงปีกกา วงนอกสุดของ json ที่เราเห็นในตอนที่ 1 |
         *****************************************************/
            DynamicJsonBuffer jsonBuffer(200);
            const char* json = payload.c_str();
            JsonObject& root = jsonBuffer.parseObject(json);


        /*******************************************************|
         * หลักๆเราต้องการ อุณหภูมิ ความชื้น และความกดอากาศ พบว่า      |
         * temp, pressure, humidity อยู่ใน object ชื่อ main ซึ่ง main |
         * จะอยู่ใน root เช่น temp = root["main"]["temp] แต่เพื่อประหยัด|
         * พื้นที่การเขียนโปรแกรม จะย่อตัวแปร root["main"] ให้เหลือแค่ main|
         *******************************************************/
            JsonArray& weather = root["weather"];
            JsonObject& main = root["main"];
            JsonObject& weather0 = weather[0];


        /**********************************************************************|
         * พิจารณาประเภทของตัวแปรก่อนเช่นเป็นทศนิยมหรือไม่ ก็แจกประเภทตามที่เห็นใน json เลย |
         **********************************************************************/
            condition       = weather0["main"];
            float temp      = main["temp"].as<float>();
            float pressure  = main["pressure"].as<float>();
            int humidity    = main["humidity"].as<int>();
            Serial.println("temp = "+String(temp)+" presure = " + String(pressure)+" humid = "+String(humidity));
        }else{
          Serial.println("Nothing");
        }
        http.end();   //Close connection
      }
      return condition;
    }



/************************************************************************************|
 * ฟังก์ชันใช้ส่งค่าออกไปยัง web browser ถูกเรียกเมื่อมีการเข้าไปยัง IP ของ ESP8266 ที่ได้จาก เราท์เตอร์  |
 ************************************************************************************/
    void handleRoot() {
      // Call weather API
      const char* condition_now = getWeatherCondition();
      server.send(200, "text/plain", condition_now);
    }

/********************************************************************************************|
 * ฟังก์ชันใช้ส่งค่าออกไปยัง web browser ถูกเรียกเมื่อมีการเข้าไปยัง IP ของ ESP8266 แต่เรียกไปยังหน้าที่ไม่ได้รองรับไว้ |
 ********************************************************************************************/
    void handleNotFound(){
      String message = "คำร้องขอผิดพลาด!\n\n";
      message += "URI: ";
      message += server.uri();
      message += "\nMethod: ";
      message += (server.method() == HTTP_GET)?"GET":"POST";
      message += "\nArguments: ";
      message += server.args();
      message += "\n";
      for (uint8_t i=0; i<server.args(); i++){
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      }
      server.send(404, "text/plain", message);
    }

void setup() {
  /***********************|
   * เริ่มการเชื่อมต่อ Serial  |
   ***********************/
        Serial.begin(9600);
  
  /****************************************|
   * พยายามเชื่อมต่อ WiFi เรื่อยๆจนกว่าจะได้รับ IP  |
   ****************************************/
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }


  /*******************|
   * แสดงค่า IP ที่ได้รับ  |
   *******************/
      Serial.println("\nConnected to " + String(ssid));
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());



  /*******************************************|
   * สร้าง Server สำหรับส่งค่าไปแสดงผลทาง browser  |
   *******************************************/
      server.on("/", handleRoot);
      server.onNotFound(handleNotFound);
      server.begin();
      Serial.println("HTTP server started");

  getWeatherCondition();
}

void loop(){

  

  /*********************************************************|
   * เช็คว่ามีการพยายามเข้าถึง ESP8266 (เช่นจากทาง browser) หรือไม่  |
   *********************************************************/
      server.handleClient();
}
