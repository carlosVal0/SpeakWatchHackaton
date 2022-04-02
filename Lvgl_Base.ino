#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <SPIFFS.h>

/*
 * En el futuro este proyecto podría incluir funciones de inteligencia
 * artificial implementadas mediante Redes Neuronales en Tensorflow
 * #include <TensorFlowLite_ESP32.h>
 * #include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
 * #include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
 * #include "tensorflow/lite/experimental/micro/micro_interpreter.h"
 * #include "tensorflow/lite/schema/schema_generated.h"
 * #include "tensorflow/lite/version.h"
*/

/*
 * Se importan los archivos de audio codificados en formato hexadecimal
*/
#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "audios/adios_hexa_audio.h"
#include "audios/bien_hexa_audio.h"
#include "audios/bienvenido_hexa_audio.h"
#include "audios/buenas_dias_hexa_audio.h"
#include "audios/buenas_noches_hexa_audio.h"
#include "audios/buenas_tardes_hexa_audio.h"
#include "audios/comos_estas_hexa_audio.h"
#include "audios/gracias_hexa_audio.h"
#include "audios/hola_hexa_audio.h"
#include "audios/lo_siento_hexa_audio.h"
#include "audios/mal_hexa_audio.h"
#include "audios/no_entiendo_hexa_audio.h"
#include "audios/por_favor_hexa_audio.h"

//#include "model.h"
#include "config.h"




const char* ssid = "Redmi 9";
const char* password = "redmi9CV";

//Peticiones HTTP para servidor
String httpGETRequest(const char* serverName);

String words[] = {"Adios","Bien","Bienvenido","Buenos dias","Buenas noches","Buenas tardes","Como estas","Gracias","Hola","Lo siento","Mal","No entiendo","Por favor"};

//Timestamps para la reproducción de audio
int tiempo_ant = 0,tiempo_act = 0;
/*
 * En el futuro este proyecto podría tener funciones de domótica
 * para permitirle al paciente interactuar con otros objetos
*/
//String words_domotica[] = {"1.Prender Bombillo","2.Apagar Bombillo","3.Apagar Televisor"};

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

String sensorReadings;
float sensorReadingsArr[3];

/*
int samplesRead = numSamples;

// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter;

static tflite::ops::micro::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

*/

void build_scr2();

//URL del servidor remoto, en este caso local
const char* serverName = "http://192.168.43.99:5000/domotica";



String txt; 
int w = 240;    // Screen width 
int h = 240;    // Screen height 
int16_t x, y;
char buf[128];


TTGOClass *ttgo;

//Objetos de Audio para separar memoria en el micrcocontrolador
AudioGeneratorMP3 *mp3;
AudioFileSourcePROGMEM *file;
AudioFileSourcePROGMEM *file2;
AudioFileSourcePROGMEM *file3;
AudioFileSourcePROGMEM *file4;
AudioFileSourcePROGMEM *file5;
AudioFileSourcePROGMEM *file6;
AudioFileSourcePROGMEM *file7;
AudioFileSourcePROGMEM *file8;
AudioFileSourcePROGMEM *file9;
AudioFileSourcePROGMEM *file10;
AudioFileSourcePROGMEM *file11;
AudioFileSourcePROGMEM *file12;
AudioFileSourcePROGMEM *file13;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

lv_obj_t *btn_img;

LV_IMG_DECLARE(adios_hexa);
LV_IMG_DECLARE(bien_hexa);
LV_IMG_DECLARE(bienvenido_hexa);
LV_IMG_DECLARE(buenas_noches_hexa);
LV_IMG_DECLARE(buenas_tardes_hexa);
LV_IMG_DECLARE(buenos_dias_hexa);
LV_IMG_DECLARE(como_estas_hexa);
LV_IMG_DECLARE(gracias_hexa);
LV_IMG_DECLARE(hola_hexa);
LV_IMG_DECLARE(lo_siento_hexa);
LV_IMG_DECLARE(mal_hexa);
LV_IMG_DECLARE(no_entiendo_hexa);
LV_IMG_DECLARE(por_favor_hexa);
LV_IMG_DECLARE(img_listen);


//Crear vistas de las imágenes
void lv_img_listen(void)
{
    
  
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &img_listen);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    //build_();

}

void lv_img_adios(void)
{
    
  
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &adios_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    //build_();

}
void lv_img_bien(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &bien_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    //lv_obj_del(img1);
    //build_scr2();

}
void lv_img_bienvenido(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &bienvenido_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}

void lv_img_buenas_noches(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &buenas_noches_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}

void lv_img_buenas_tardes(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &buenas_tardes_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}

void lv_img_buenos_dias(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &buenos_dias_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}

void lv_img_como_estas(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &como_estas_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}

void lv_img_gracias(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &gracias_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}

void lv_img_hola(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &hola_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}

void lv_img_lo_siento(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &lo_siento_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}

void lv_img_mal(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &mal_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}

void lv_img_no_entiendo(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &no_entiendo_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}
void lv_img_por_favor(void)
{
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &por_favor_hexa);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);
    
    //build_scr2();

}

void httpPOSTRequest(const char* serverName,String accion){
   if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      http.begin(client, serverName);

      http.addHeader("Content-Type", "text/plain");
      int httpResponseCode = http.POST(accion);
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  
  
  
  
  }




String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  
  http.begin(client, serverName);
  
  
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}

static void event_handler_btn1(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        Serial.println("Clicked");
        build_scr2();
    } else if (event == LV_EVENT_VALUE_CHANGED) {
        Serial.println("Toggled");
    }
}

static void event_handler_btn_lista_domotica(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        Serial.println("Clicked");
        Serial.println(lv_list_get_btn_text(obj));
        httpPOSTRequest(serverName,lv_list_get_btn_text(obj));
        

        
    } else if (event == LV_EVENT_VALUE_CHANGED) {
        Serial.println("Toggled");
    }
}



static void event_handler_btn_domotica(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        Serial.println("Clicked Domotica");
        build_scr3();
        //Serial.println(lv_list_get_btn_text(obj));
        
     
      }
}


static void event_handler_btn_img(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        Serial.println("Clicked");
        //lv_disp_load_scr(lv_disp_get_scr_prev(NULL));
        build_scr2();
    } else if (event == LV_EVENT_VALUE_CHANGED) {
        Serial.println("Toggled");
    }
}


static void event_handler_btn2(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
    
        Serial.println("Clicked");
        /*
        if(WiFi.status()== WL_CONNECTED){
              
      sensorReadings = httpGETRequest(serverName);
      Serial.println(sensorReadings);
      JSONVar myObject = JSON.parse(sensorReadings);
  
      
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
    
      
      JSONVar keys = myObject.keys();
    
      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        Serial.print(keys[i]);
        Serial.print(" = ");
        Serial.println(value);
//        tft->println(value);
        
        sensorReadingsArr[i] = double(value);
      }
      
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  */
    
        build_scr();
    } else if (event == LV_EVENT_VALUE_CHANGED) {
        Serial.println("Toggled");
    }
}


static void event_handler(lv_obj_t * obj, lv_event_t event)
{

    
    if(String(lv_list_get_btn_text(obj)) == "Adios"){
      Serial.printf("Entro a buenos\n");
      file = new AudioFileSourcePROGMEM(adios, sizeof(adios));  
      id3 = new AudioFileSourceID3(file);
    }
    else if(String(lv_list_get_btn_text(obj)) == "Bien"){
      Serial.printf("Angie fortachona\n");
      file2 = new AudioFileSourcePROGMEM(bien, sizeof(bien));  
      id3 = new AudioFileSourceID3(file2);
    }
    else if(String(lv_list_get_btn_text(obj)) == "Bienvenido"){
      Serial.printf("Buenas\n");
      file3 = new AudioFileSourcePROGMEM(bienvenido, sizeof(bienvenido));  
      id3 = new AudioFileSourceID3(file3);
    }
    else if(String(lv_list_get_btn_text(obj)) == "Buenos dias"){
      Serial.printf("Buenas\n");
      file4 = new AudioFileSourcePROGMEM(buenos_dias, sizeof(buenos_dias));  
      id3 = new AudioFileSourceID3(file4);
      //"Adios","Bien","Bienvenido","Buenos dias","Buenas noches","Buenas tardes","Como estas","Gracias","Hola","Lo siento","Mal","No entiendo","Por favor"
    }
    else if(String(lv_list_get_btn_text(obj)) == "Buenas noches"){
      Serial.printf("Buenas\n");
      file5 = new AudioFileSourcePROGMEM(buenas_noches, sizeof(buenas_noches));  
      id3 = new AudioFileSourceID3(file5);
    }
    else if(String(lv_list_get_btn_text(obj)) == "Buenas tardes"){
      Serial.printf("Buenas\n");
      file6 = new AudioFileSourcePROGMEM(buenas_tardes, sizeof(buenas_tardes));  
      id3 = new AudioFileSourceID3(file6);
    }
    else if(String(lv_list_get_btn_text(obj)) == "Como estas"){
      Serial.printf("Buenas\n");
      file7 = new AudioFileSourcePROGMEM(como_estas, sizeof(como_estas));  
      id3 = new AudioFileSourceID3(file7);
    }
    else if(String(lv_list_get_btn_text(obj)) == "Gracias"){
      Serial.printf("Buenas\n");
      file8 = new AudioFileSourcePROGMEM(gracias, sizeof(gracias));  
      id3 = new AudioFileSourceID3(file8);
    }
    else if(String(lv_list_get_btn_text(obj)) == "Hola"){
      Serial.printf("Buenas\n");
      file9 = new AudioFileSourcePROGMEM(hola, sizeof(hola));  
      id3 = new AudioFileSourceID3(file9);
    }
    else if(String(lv_list_get_btn_text(obj)) == "Lo siento"){
      Serial.printf("Buenas\n");
      file10 = new AudioFileSourcePROGMEM(lo_siento, sizeof(lo_siento));  
      id3 = new AudioFileSourceID3(file10);
    }
    else if(String(lv_list_get_btn_text(obj)) == "Mal"){
      Serial.printf("Buenas\n");
      file11 = new AudioFileSourcePROGMEM(mal, sizeof(mal));  
      id3 = new AudioFileSourceID3(file11);
    }
    else if(String(lv_list_get_btn_text(obj)) == "No entiendo"){
      Serial.printf("Buenas\n");
      file12 = new AudioFileSourcePROGMEM(no_entiendo, sizeof(no_entiendo));  
      id3 = new AudioFileSourceID3(file12);
    }
    else if(String(lv_list_get_btn_text(obj)) == "Por favor"){
      Serial.printf("Buenas\n");
      file13 = new AudioFileSourcePROGMEM(por_favor, sizeof(por_favor));  
      id3 = new AudioFileSourceID3(file13);
    }
    

    #if defined(STANDARD_BACKPLANE)
        out = new AudioOutputI2S(0, 1);
    #elif defined(EXTERNAL_DAC_BACKPLANE)
        out = new AudioOutputI2S();
        //External DAC decoding
        out->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
        
        
    #endif
        
        mp3 = new AudioGeneratorMP3();
        

    
    if(event == LV_EVENT_CLICKED) {
        
        printf("Palabra: %s\n", lv_list_get_btn_text(obj));
        tiempo_ant = millis();
        tiempo_act = millis();
        while(tiempo_act - tiempo_ant < 2500){
          sonido();  
          tiempo_act = millis();
          
        }
        tiempo_act = 0;
        tiempo_ant = 0;

        lv_obj_t *scr_img = lv_obj_create(NULL,NULL);
        lv_scr_load(scr_img);


        
      if(String(lv_list_get_btn_text(obj)) == "Adios"){
         Serial.print("Imagen adios");
         lv_img_adios();
        
      }
      else if(String(lv_list_get_btn_text(obj)) == "Bien"){
          lv_img_bien(); 
      }
      else if(String(lv_list_get_btn_text(obj)) == "Bienvenido"){
        lv_img_bienvenido();
      }
      else if(String(lv_list_get_btn_text(obj)) == "Buenos dias"){
        lv_img_buenos_dias();
      }
      else if(String(lv_list_get_btn_text(obj)) == "Buenas noches"){
        lv_img_buenas_noches();
      }
      else if(String(lv_list_get_btn_text(obj)) == "Buenas tardes"){
        lv_img_buenas_tardes();
      }
      else if(String(lv_list_get_btn_text(obj)) == "Como estas"){
        lv_img_como_estas();
      }
      else if(String(lv_list_get_btn_text(obj)) == "Gracias"){
        lv_img_gracias();
      }
      else if(String(lv_list_get_btn_text(obj)) == "Hola"){
        lv_img_hola();
      }
      else if(String(lv_list_get_btn_text(obj)) == "Lo siento"){
        lv_img_lo_siento();
      }
      else if(String(lv_list_get_btn_text(obj)) == "Mal"){
        lv_img_mal();
      }
      else if(String(lv_list_get_btn_text(obj)) == "No entiendo"){
        lv_img_no_entiendo();
      }
      else if(String(lv_list_get_btn_text(obj)) == "Por favor"){
        lv_img_por_favor();
      }

    lv_obj_t *label_img;

    lv_obj_t *btn_img = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn_img, event_handler_btn_img);
    lv_obj_align(btn_img, NULL, LV_ALIGN_CENTER, 0, 90);
    lv_btn_set_fit2(btn_img, LV_FIT_TIGHT,LV_FIT_TIGHT);
    label_img = lv_label_create(btn_img, NULL);
    lv_label_set_text(label_img, "Regresar");
    
         
    }
    
      
    
    
    
}

void sonido(){

  if (mp3->isRunning()) {
        if (!mp3->loop()) mp3->stop();
        Serial.printf("En Running\n");
    } else {
        mp3->begin(id3, out);
        Serial.printf("MP3 done\n");
        delay(1000);
    }
  
  
  }


void build_scr2(){

    lv_obj_del(lv_scr_act());
    lv_obj_t *scr2 = lv_obj_create(NULL,NULL);
    lv_scr_load(scr2);

    lv_obj_t * list1 = lv_list_create(lv_scr_act(), NULL);
    lv_obj_set_size(list1, 160, 160);
    lv_obj_align(list1, NULL, LV_ALIGN_CENTER, 0, 30);

    /*Add buttons to the list*/
    lv_obj_t * list_btn;

   
    for(int i = 0; i < 13; i++){

      list_btn = lv_list_add_btn(list1, LV_SYMBOL_RIGHT, words[i].c_str());
      lv_obj_set_event_cb(list_btn, event_handler);
      
    }
    
 

    lv_obj_t *label;

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn1, event_handler_btn2);
    lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, -90);

    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Regresar");
    
}

static void event_handler_btn_listen(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        Serial.println("Clicked");
        //lv_disp_load_scr(lv_disp_get_scr_prev(NULL));
        build_scr4();
    } else if (event == LV_EVENT_VALUE_CHANGED) {
        Serial.println("Toggled");
    }
}

void build_scr4(){

  lv_obj_del(lv_scr_act());
  lv_obj_t *scr4= lv_obj_create(NULL,NULL);
  lv_scr_load(scr4);

  lv_img_buenos_dias();
  
    lv_obj_t *label_img;

    lv_obj_t *btn_img = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn_img, event_handler_btn_img);
    lv_obj_align(btn_img, NULL, LV_ALIGN_CENTER, 0, 90);
    lv_btn_set_fit2(btn_img, LV_FIT_TIGHT,LV_FIT_TIGHT);
    label_img = lv_label_create(btn_img, NULL);
    lv_label_set_text(label_img, "Hola, Buenos dias");
  
  
  
  
}


void build_scr3(){

    //This code is just for demo purposes, is not the functionality itself
    lv_obj_del(lv_scr_act());
    lv_obj_t *scr3 = lv_obj_create(NULL,NULL);
    lv_scr_load(scr3);

    lv_img_listen();
    
    

    lv_obj_t *label_img;

    lv_obj_t *btn_img = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn_img, event_handler_btn_listen);
    lv_obj_align(btn_img, NULL, LV_ALIGN_CENTER, 0, 90);
    lv_btn_set_fit2(btn_img, LV_FIT_TIGHT,LV_FIT_TIGHT);
    label_img = lv_label_create(btn_img, NULL);
    lv_label_set_text(label_img, "Interpretar");
  
}

void build_scr(){

     
    //lv_obj_del(lv_scr_act());
    lv_obj_t * scr1 = lv_obj_create(NULL,NULL);
    lv_scr_load(scr1);

    lv_obj_t *label, *label_2;

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn1, event_handler_btn1);
    lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Hablar");

    lv_obj_t *btn_domotica = lv_btn_create(lv_scr_act(),NULL);
    lv_obj_set_event_cb(btn_domotica, event_handler_btn_domotica);
    lv_obj_align(btn_domotica, NULL, LV_ALIGN_CENTER,0,40);
    

    label_2 = lv_label_create(btn_domotica, NULL);
    lv_label_set_text(label_2, "Escuchar");
  
  
  }


void setup()
{
    //WiFi.mode(WIFI_OFF);
    delay(500);
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->lvgl_begin();

   //tft = watch->tft;
//    sensor = watch->bma;
    
    SPIFFS.begin();
    //ttgo->enableLDO3();
    ttgo->enableAudio();

    Acfg cfg;
    /*!
        Output data rate in Hz, Optional parameters:
            - BMA4_OUTPUT_DATA_RATE_0_78HZ
            - BMA4_OUTPUT_DATA_RATE_1_56HZ
            - BMA4_OUTPUT_DATA_RATE_3_12HZ
            - BMA4_OUTPUT_DATA_RATE_6_25HZ
            - BMA4_OUTPUT_DATA_RATE_12_5HZ
            - BMA4_OUTPUT_DATA_RATE_25HZ
            - BMA4_OUTPUT_DATA_RATE_50HZ
            - BMA4_OUTPUT_DATA_RATE_100HZ
            - BMA4_OUTPUT_DATA_RATE_200HZ
            - BMA4_OUTPUT_DATA_RATE_400HZ
            - BMA4_OUTPUT_DATA_RATE_800HZ
            - BMA4_OUTPUT_DATA_RATE_1600HZ
    */
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    /*!
        G-range, Optional parameters:
            - BMA4_ACCEL_RANGE_2G
            - BMA4_ACCEL_RANGE_4G
            - BMA4_ACCEL_RANGE_8G
            - BMA4_ACCEL_RANGE_16G
    */
    cfg.range = BMA4_ACCEL_RANGE_2G;
    /*!
        Bandwidth parameter, determines filter configuration, Optional parameters:
            - BMA4_ACCEL_OSR4_AVG1
            - BMA4_ACCEL_OSR2_AVG2
            - BMA4_ACCEL_NORMAL_AVG4
            - BMA4_ACCEL_CIC_AVG8
            - BMA4_ACCEL_RES_AVG16
            - BMA4_ACCEL_RES_AVG32
            - BMA4_ACCEL_RES_AVG64
            - BMA4_ACCEL_RES_AVG128
    */
    //cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

    /*! Filter performance mode , Optional parameters:
        - BMA4_CIC_AVG_MODE
        - BMA4_CONTINUOUS_MODE
    */
  //  cfg.perf_mode = BMA4_CONTINUOUS_MODE;

    // Configure the BMA423 accelerometer
//    sensor->accelConfig(cfg);

    // Enable BMA423 accelerometer
//    sensor->enableAccel();

    /*
    tflModel = tflite::GetModel(model);
    if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
      Serial.println("Model schema mismatch!");
      while (1);
    }

    tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);

    // Allocate memory for the model's input and output tensors
    tflInterpreter->AllocateTensors();
  
    // Get pointers for the model's input and output tensors
    tflInputTensor = tflInterpreter->input(0);
    tflOutputTensor = tflInterpreter->output(0);
    */

/*
   tft->setTextColor(random(0xFFFF));
    tft->drawString("BMA423 accel",  25, 50, 4);
    tft->setTextFont(4);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
  */  
    
/*
    lv_obj_t * bar1 = lv_bar_create(lv_scr_act(), NULL);
    lv_obj_set_size(bar1, 200, 20);
    lv_obj_align(bar1, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_bar_set_anim_time(bar1, 2000);
    lv_bar_set_value(bar1, 100, LV_ANIM_ON);
*/
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  
    build_scr();

 

  
}

void loop()
{
    lv_task_handler();
    delay(5);
}
