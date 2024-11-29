#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 rtc;

class DS1307_RTC
{
  public:
    uint8_t hr = 0, min = 0, sec = 0, day = 0, month = 0;
    uint16_t year = 0;
    String hora = "", fecha = "", fecha2 = "";
      
  public:
    void RTC_init(const char*, const char*);
    //void RTC_init(void);
    void Time(void);
    void reloc(void);
    /*void GetDate(void);
    void GetTime(void);*/
};

void DS1307_RTC::RTC_init(const char* dat, const char* hou)
//void DS1307_RTC::RTC_init()
{
  while (!rtc.begin())
  {                  /* Mientras que el reloj no se ha iniciado. */
    Serial.println(F("Hay un error de reconocimiento del RTC, revisa conexiones y alimentación"));
    Serial.flush();                           /* Espera a que se transmita la información serial correctamente. */
    delay(3000);
    //abort();
  }
  rtc.adjust(DateTime(dat, hou));
  //rtc.adjust(DateTime(__DATE__, __TIME__));
}
void DS1307_RTC::Time(void)
{
  DateTime date = rtc.now();
  sec = date.second();
  min = date.minute();
  hr = date.hour();
  day = date.day();
  month = date.month();
  year = date.year();
  reloc();
}
void DS1307_RTC::reloc(void)
{
  hora = "";
  int horas2 = hr;
  if (horas2 == 0)
  {
    horas2 = 12;
  }
  else
  {
    if (horas2 > 12)
    {
      horas2 -= 12;
    }
  }

  if (horas2 < 10)
  {
    hora += '0';
  }
  hora += horas2;


  hora += ':';
  if (min < 10)
  {
    hora += '0';
  }
  hora += min;
  if (hr < 12)
  {
    hora += " AM";
  }
  else
  {
    hora += " PM";
  }
  
  fecha2 = "";
  if (day < 10)
  {
    fecha2 += '0';
  }
  fecha2 += day;
  fecha2 += "/";
  if (month < 10)
  {
    fecha2 += '0';
  }
  fecha2 += month;
  fecha2 += "/";
  fecha2 += year;
}
/*void DS1307_RTC::GetDate(void)
{
  sec = date.second();
  min = date.minute();
  hr = date.hour();
  day = date.day();
  month = date.month();
  year = date.year();
}
void DS1307_RTC::GetTime(void)
{
  DateTime date = rtc.now();
  sec = date.second();
  min = date.minute();
  hr = date.hour();
  day = date.day();
  month = date.month();
  year = date.year();
}*/