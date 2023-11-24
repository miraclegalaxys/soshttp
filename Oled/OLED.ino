#include <Arduino.h>
#include <U8g2lib.h>

// กำหนดคอนสตรักเตอร์สำหรับ OLED display
// สำหรับ 1.3" SH1106 128x64, ใช้งานผ่าน I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.clearBuffer(); // ล้างบัฟเฟอร์
  u8g2.setFont(u8g2_font_ncenB08_tr); // เลือกฟอนต์
  u8g2.drawStr(0,10,"Hello OLED!");  // เขียนข้อความ
  u8g2.sendBuffer(); // ส่งบัฟเฟอร์ไปยังหน้าจอ
  delay(1000);
}
