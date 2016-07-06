#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MenuSystem.h>
#include <FlashStorage.h>
 
Adafruit_SSD1306 display = Adafruit_SSD1306();
 
#if defined(ESP8266)
  #define BUTTON_A 0
  #define BUTTON_B 16
  #define BUTTON_C 2
  #define LED      0
#elif defined(ARDUINO_STM32F2_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
  #define LED PB5
#elif defined(TEENSYDUINO)
  #define BUTTON_A 4
  #define BUTTON_B 3
  #define BUTTON_C 8
  #define LED 13
#else 
  #define BUTTON_A 9
  #define BUTTON_B 6
  #define BUTTON_C 5
  #define LED      13
#endif
 
#if (SSD1306_LCDHEIGHT != 32)
 #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif



FlashStorage(my_flash_store, int);

extern "C" char *sbrk(int i);
 
int FreeRam () {
  char stack_dummy = 0;
  return &stack_dummy - sbrk(0);
}


class MyRenderer : public MenuComponentRenderer
{
public:
    virtual void render(Menu const& menu) const
    {
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(menu.get_name());
        display.setCursor(0,0);        
        menu.get_current_component()->render(*this);
        display.display(); // actually display all of the above
    }

    virtual void render_menu_item(MenuItem const& menu_item) const
    {
        display.clearDisplay();
        display.print(menu_item.get_name());
        display.display(); 
    }

    virtual void render_back_menu_item(BackMenuItem const& menu_item) const
    {
        display.clearDisplay();
        display.print(menu_item.get_name());
        display.display(); 
    }

    virtual void render_numeric_menu_item(NumericMenuItem const& menu_item) const
    {
        display.clearDisplay();
        display.print(menu_item.get_name());
        display.display(); 
    }

    virtual void render_menu(Menu const& menu) const
    {
        display.clearDisplay();
        display.print(menu.get_name());
        display.display(); 
    }
};
MyRenderer my_renderer;
// Forward declarations

void on_item1_selected(MenuItem* p_menu_item);
void on_item2_selected(MenuItem* p_menu_item);
void on_item3_selected(MenuItem* p_menu_item);

// Menu variables

MenuSystem ms(my_renderer);
MenuItem mm_mi1("Login", &on_item1_selected);
MenuItem mm_mi2("Settings", &on_item2_selected);
Menu mu1("Test");
MenuItem mu1_mi1("Test3", on_item3_selected);

// Menu callback function

void on_item1_selected(MenuItem* p_menu_item)
{
  display.clearDisplay();
  display.setCursor(0,1);
  display.print("Password:");
  display.display();
  delay(1500); // so we can look the result on the LCD
}

void on_item2_selected(MenuItem* p_menu_item)
{
  display.clearDisplay();
  display.setCursor(0,1);
  display.println(FreeRam ());
  int number;

  // Read the content of "my_flash_store" and assign it to "number"
  number = my_flash_store.read();
  display.println(number);
  //display.print("test:");
  display.display();
  delay(1500); // so we can look the result on the LCD
}

void on_item3_selected(MenuItem* p_menu_item)
{
  display.clearDisplay();
  display.setCursor(0,1);
  display.print("Huh?");
  display.display();
  delay(1500); // so we can look the result on the LCD
}


void setup() {  
  delay(500);

  Serial.begin(115200);
  
  Serial.println("OLED FeatherWing test");
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  Serial.println("OLED begun");
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);
 
  // Clear the buffer.
  display.clearDisplay();
  display.display();

  ms.get_root_menu().add_item(&mm_mi1);
  ms.get_root_menu().add_item(&mm_mi2);
  ms.get_root_menu().add_menu(&mu1);
  mu1.add_item(&mu1_mi1);

  Serial.println("IO test");
 
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
 
  // text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  ms.display();
  display.display(); // actually display all of the above

    int number;

  // Read the content of "my_flash_store" and assign it to "number"
  number = my_flash_store.read();

  // Print the current number on the serial monitor
  Serial.println(number);

  // Save into "my_flash_store" the number increased by 1 for the
  // next run of the sketch
  my_flash_store.write(number + 1);
}

void loop() {
  if (! digitalRead(BUTTON_A))
  {
      ms.prev();
      ms.display();
  }
  if (! digitalRead(BUTTON_B)) {
          ms.select();
      ms.display();
  }
  if (! digitalRead(BUTTON_C)) {
          ms.next();
      ms.display();
  }
  delay(10);
  yield();
  display.display();
}
