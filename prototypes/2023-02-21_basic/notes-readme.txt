This prototype runs on Arduino.

Before you can start, configure your IDE.


Board select
============

Inside the Arduino IDE:
Tools -> Boards -> ESP32 -> ESP32-WROOM-DA Modulo



USB connection
==============

Inside the Arduino IDE:
Tools -> Port -> (choose port)

When there is no port listed or you fail to upload the code because there is no connection, try to change the USB cable. Some cables are not appropriate for firmware uploads.



Dependencies to install
=======================

Open Tools -> Library Manager

Install:
+ TFT_eSPI by Bodmer
+ LovyanGFX by lovyan03
+ NTPClient by Fabrice Weinberg

 
