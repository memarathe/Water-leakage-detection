# Water-leakage-detection
On detection of leakage in the pipeline, the location and sms alert of the leakage is sent to the water management team.

About :
Making use of water flow sensors and NodeMCU, we can detect the leakages in the pipelines and avoid the wastage of water by 
sending an alert notification immediately to the management team looking after the supply chain of water. Using easily accessible resources such as third party applications, network access, smart objects and highly efficient computing power has made it possible for smart methods like these to make things more reliable.

Pre requisites:
1. Open a account on Thingsspeak : https://w4krl.com/iot-kits/open-a-thingspeak-account/#:~:text=Open%20thingspeak.com%20in%20your,a%20channel%20name%20and%20description.
2. Download and setup Arduino IDE in your laptop : https://docs.arduino.cc/software/ide-v1/tutorials/Windows
3. Download and install a Android app called push bullet.
4. Setup a account on PushingBox.
5. Hardware required : 
   Water flow sensors.
   Mobile.
   Node MCU esp8266.
   Power Supply


Steps:
1. write the code in the arduino ide
2. Perform connections as showed in the image uploaded (ESP8266-PINOUT.PNG)
3. setup the channels on thingspeak and add the required fields(data from water sensors)
4. Create a notification trigger on pushing box.
5. link it to the pushbullet app on your android using the device id.
6. run the code and observe the test case.
