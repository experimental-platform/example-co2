## Project goal

Indoor Air Quality (IAQ) is an important factor for feeling well while being at home, work or school. One way to measure air quality is to sense the current CO2 (Carbon dioxide) value which is a good indicator for overall air quality.
Our project was aimed at measuring the CO2-value on a wireless module which sends the data over WLAN to the Experimental Platform and then shows the data in a dynamically generated graph.

![image](https://41.media.tumblr.com/9b974a338c61f6cd284e4334f6cb0202/tumblr_inline_nwzfkxNfHC1tzfota_540.jpg)

On the software side the main task was how to get and process the incoming sensor data and show it on a dynamically generated webpage. Hardware wise the challenge was a bit more ambitious, because we had to decide which precise but still affordable sensor could be used and what microcontroller/ WLAN combination was the ideal couple for this project. Reading data and powering the sensor were also two important issues.

## Overview

**Hardware**

Measuring environment data is one of the most basic implementations for sensors. Why not doing a project everybody has a direct connection to? Air is all around us and especially the air quality in indoor rooms like offices, schools or even at home should be of particular importance to us. Carbon dioxide (CO2) is quite a good IAQ indicator beside sensors that measure VOCs (Volatile Organic Compounds). CO2 could be gauged by electrochemical reaction or by spectroscopical measurements. As the first needs more energy, is less exact and has drift over time we went for the latter which is known as NDIR (Non Dispersive InfraRed) technology. Basically it measures the absorption of infrared light of specific wavelengths - if you want to know more about it you might read the page on wikipedia ([https://en.wikipedia.org/wiki/Nondispersive_infrared_sensor](https://en.wikipedia.org/wiki/Nondispersive_infrared_sensor)).

Good NDIR CO2-sensors are quite expensive - for our use case the Senseair S8 ([http://www.senseair.com/products/oem-modules/senseair-s8/](http://www.senseair.com/products/oem-modules/senseair-s8/)) which is sold for about 70 Euros per piece is one of the most affordable and also with a very small form factor - so an ideal combination for our Experimental Platform modules we plan.

A very cheap microcontroller with WLAN already implemented is the chinese ESP8266 which could easily programmed with the Arduino IDE. We went for an inexpensive development board which has the ESP8266 module, USB to serial and also a Lithium (LiPo) charging IC on board. It's called "ESPtoy" and you find more information here: [http://rayshobby.net/esptoy/](http://rayshobby.net/esptoy/) .

As the Senseair S8 needs 5 volts we did some dirty hack to get this voltage. More on that in the step-by-step instructions.

**Software - Experimental Platform**

The software side was quite easy to handle. We used the JavaScript/ Node.js combination delivering a static HTML-page and showing a graph. You can find the source code - which is open source as all our projects are - on the Experimental Platform page on github: [https://github.com/experimental-platform](https://github.com/experimental-platform)/.

![image](https://41.media.tumblr.com/37aee3c46eafc6111e116393d1ffce76/tumblr_inline_nwzfxzS2eI1tzfota_540.png)

**Firmware - Microcontroller**

For the firmware we have essentially two parts. The one thing we have to handle is the WiFi connection to our network which is done by the ESP8266. The other task is to read the values out of our CO2 sensor, where we were fortunate to have the suitable functions already at hand. It's written for another Senseair sensor, but works as well. Everything is community-friendly programmable with the Arduino IDE, you just have to copy the CO2-sensor code into your Arduino-documents folder. See the step-by-step instructions for more info.

## Step-by-step instructions

We start with the software.

If you haven't done it yet, you first have to generate a SSH public key for the communication with your platform (log in to your Experimental Platform and go to "Configuration"-page "SSH Public Keys", copy your generated public key into the field provided and add it to the existing keys).

Then go to the "My Apps" page.

![image](https://40.media.tumblr.com/d821a4336b1c36c17bea2a141a3108eb/tumblr_inline_nwzg0hMILl1tzfota_540.png)

Klick on the orange button "New App" on the upper right corner and the page with detailed instructions comes up. Later you don't need to go to the page to deploy an app - this is just for the introduction on how to deploy apps now or for later if you forgot the git-command chain syntax.

![image](https://40.media.tumblr.com/136a2a2ef7e372b6928202bea0cb9a22/tumblr_inline_nwzg4hV2GW1tzfota_540.png)

If you click for example "Node.js" on the next page you will see the instructions how to get your apps deployed.

![image](https://41.media.tumblr.com/0fa613bc9214059a66e3e3686bdf11af/tumblr_inline_nwzg5ofOtY1tzfota_540.png)

Now open a terminal or command line on your preferred operating system. You will of cause have an installation with at least a commandline git environment to get all the git-commands executed before doing anything on the console. A search for "git for Windows" or a packetmanager installation like "apt-get install git" on a Ubuntu will get you there.

Our app for the CO2-sensor is like all other available apps hosted on our github page [https://github.com/experimental-platform](https://github.com/experimental-platform) . It's called "example-co2".

![image](https://40.media.tumblr.com/0df1e8f3bd60094dc882448f34c946e2/tumblr_inline_nwzg71os991tzfota_540.png)

Go to your command line and type the following:

```
git clone https://github.com/experimental-platform/example-co2
```

For all the non-gitters among us - this is the instruction to make an exact copy of the git repository named "example-co2".

Next change to the directory "example-co2".

Now add the remote platform (your Experimental Platform) to the repository by typing:

```
git remote add platform ssh://dokku@yourIP:8022/co2
```

This command might need a bit of explaining. The term "platform" is decided by you. You can name it like you want, but you have to use the name later for pushing the repository to your Experimental Platform. "yourIP" of cause is the IP of your Experimental Platform, substitute it with the raw IP or something like "myMachine.local" (remember you initially named your platform). Last "co2" is the path where the app looks for data, so do not change that.

You are almost there - you just have to push the repository to your Experimental Platform with

```
git push platform master
```

![image](https://36.media.tumblr.com/f6989435803cfc5051859e8da1c2fdc6/tumblr_inline_nwzg88rQx81tzfota_540.png)

A bit of script working in the background will work and after some time the app should be deployed.

![image](https://40.media.tumblr.com/7dc40d7419b71ec741664d0e597f02f2/tumblr_inline_nwzg8nHPnd1tzfota_540.png)

That's all, at least software wise. Your app is deployed and ready to start. But first we have to get our module hardware ready.

**Hardware**

The hardware part this time is a bit more tricky, but we will explain everything in detail right now. There are some electronics and hacks involved but you won't need much electronics or soldering knowledge to get the thing working.

The parts we need:

1.  ESPtoy development board [http://rayshobby.net/esptoy/](http://rayshobby.net/esptoy/) . Add a Lithium Polymer rechargeable battery.
2.  CO2-sensor Senseair S8 for example in Germany from [http://www.driesen-kern.de/ ](http://www.driesen-kern.de/). For distributors worldwide look at [http://www.senseair.com/contact-support/distributors/](http://www.senseair.com/contact-support/distributors/)
3.  Cheap step-up board e.g. from a power bank, like this one we actually used for our project: [http://www.ebay.de/itm/Universal-LED-2xAA-Akku-Ladegerat-Emergency-USB-Power-Bank-Charger-For-iPhone-/291589822510](http://www.ebay.de/itm/Universal-LED-2xAA-Akku-Ladegerat-Emergency-USB-Power-Bank-Charger-For-iPhone-/291589822510) . If you take a different device from the one we used be sure it steps-up the voltage from at least 3 volts to 5.
![image](https://36.media.tumblr.com/0fbbc6314f7cd0e68c1eca4b8874819d/tumblr_inline_nwzgc6mB8a1tzfota_540.jpg)

Apart from that you should have a soldering iron and some solder at hand, you need six short wires (preferably two red, two black and two different colors) and some insulating tape and/ or heat shrink tube. If you want to mount it in a kind of box you also might need some double sided tape. And if you want it safe and perfect and maybe want to remove the sensor for testing purposes, you could add a 4pin 2.54mm pin header and connector like we did for our prototype (e.g.:[ http://www.ebay.de/itm/80Stk-XH2-54-4P-Connector-Kits-2-54mm-Female-Pin-20-4P-Terminal-20-Housing-/111536359862](http://www.ebay.de/itm/80Stk-XH2-54-4P-Connector-Kits-2-54mm-Female-Pin-20-4P-Terminal-20-Housing-/111536359862) ).

We start with soldering the wires to the CO2-sensor. We need two for the UART signal-lines and two for the power supply. See photo for exact wiring. The length of the wires should be according to your installation. You may have a look at the datasheet for the exact pinout of the module ( [http://www.senseair.com/wp-content/uploads/2012/06/SenseAir-S8-Residential-PSP107-Rev14.pdf](http://www.senseair.com/wp-content/uploads/2012/06/SenseAir-S8-Residential-PSP107-Rev14.pdf) , page 4).

![image](https://36.media.tumblr.com/6a8f49efbf35ef19531b730ac0acc5fe/tumblr_inline_nwzgh0aN4T1tzfota_540.jpg)![image](https://36.media.tumblr.com/fc2491e65be105ae744b0b6f941f03ee/tumblr_inline_nwzgh0aXL61tzfota_540.png)

Let's come to the dirty hack of this project now! In order to get 5 volts out of the Lithium Polymer battery we need a step-up converter. This is because we only get a maximum of 4.2 volts (minimum about 3 volts) out of the LiPo. You have to disassemble your power bank or whatever cheap version for a step-up you are using. According to the part you use you have to desolder the USB connector and connectors for the supplying side (that's where in our case the batteries were connected).

Then cut the wires of the LiPo battery in half (be careful to do no short-circuit) put two extra wires to the USB step-up input (attention again to reverse-polarity) and solder everything together and wrap it in insulating tape or heat shrinking tube. The last step is to solder the UART wires of the sensors to the ESPtoy board. As a picture says more than words have a detailed look at the photo below.

_**IMPORTANT DISCLAIMER
TAKE ATTENTION TO THE POLARITY OF THE SENSOR!**_

As the sensor has no reverse-polarity protection, you could cause massive damage to it and render it useless. Use a multimeter to get the polarity of the USB output of the step-up converter, if you are not sure about it.

This warning said connect the power wires of the sensor to the output (USB socket pads) of the step-up converter.

![image](https://36.media.tumblr.com/f6a52a5b3772af420a3f8bb607ecd5c7/tumblr_inline_nwzgjcVPgf1tzfota_540.jpg)

The yellow wire (RxD) of the sensor goes to pin 8 on the ESPtoy, the green wire (TxD) to pin 7. If you later get no data you might have those accidentally switched. As said you don't need to have a connector to the USB output of the power bank. In the photo you can also see how our prototype fits in our hexagon shaped modules.

If you connect the ESPtoy board to a USB power adapter the lights on the board should illuminate and the sensor should blink. The LiPo battery gets charged by the board and the hardware part is done.

**Firmware**

Locate the Arduino-Sketch which is on our github page in the example-co2 repository (folder/ file "CO2_Sensor_ESP8266"). If you haven't done so install an Arduino IDE from &nbsp;[https://www.arduino.cc/en/Main/Software](https://www.arduino.cc/en/Main/Software) .

Copy the folder "CO2_Sensor_ESP8266" into your Arduino document-folder&nbsp; (on Windows e.g. C:\Users\yourNameHere\Documents\Arduino).

The ESP8266 is not integrated into the Arduino IDE by default. Follow the instructions at [https://github.com/esp8266/Arduino](https://github.com/esp8266/Arduino) to get the IDE ESP8266-ready.

Then start the Arduino IDE and open the Arduino sketch "CO2_Sensor_ESP8266". Under "Tools" choose first the correct board which in our case is "Generic ESP8266 module". Then activate the correct serial port. Cut the board from both the USB and LiPo spower supply and press the button while reconnecting power. This will set the board in programming mode. After that you can upload the sketch to the board. Repower the unit and you are ready!

![image](https://41.media.tumblr.com/cd4a39b59a3cdb2e6807046e4c14f144/tumblr_inline_nwzgkgx8IR1tzfota_540.png)

You are almost done! Connect the board with a USB cable to the Experimental platform and start the app by again going to the "My Apps" page (which should now display an app like "co2"). The app should already have been started after deploying, so you can klick on the left symbol to open the webpage of the app.

![image](https://40.media.tumblr.com/53349cf4c68a9b47a618f30fd36d189f/tumblr_inline_nwzgl1bnaT1tzfota_540.png)

You can add any feature to the code you want, fork it, report issues or even do a pull request to add new features into the master branch. Enjoy!

Still questions? Feel free to send any questions or suggestions to markus.ulsass@protonet.info
