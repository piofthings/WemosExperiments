# Wemos Experiments

Having deleted this repository once already, I would like this to be a safe starting point for beginners like me to get
started with Wemos D1 Minis.

The Wemos D1 mini powered by the ESP8266 is a powerful little microcontroller platform that you can use for any IoT endpoint.

Unfortunately it is very easy to start off on the wrong foot. Most examples on the Web start with hardcoding your WiFi username and password into 
the source code! brrrrrrpppppp! Big no no if like me you like to use public repositories like Github (now you know why this repo was deleted once already).

So these are a set of experiments that I am doing to learn my way towards best practices for using ESP8266 responsibly... these little devices are very powerful

> With great power comes great responsibility - Uncle Ben (Spiderman)


## Experiment 1:
-------------
**Using a cheap I2C display with the Wemos D1**
I bought a cheap SSD1306 based 0.96 inch single colour OLED display. I found the article in references below, muddled around with the I2C address. Apparently the address they screen printed on the PCB is wrong (yay cheap)! Anyway, it was super simple to start using it.

In the reference below you'll also find code that sets up a Web Server on the little Wemos D1 and responds with updates to the OLED screen if you send it over the browser.

## Experiment 2:
---------------
**Using a BME280 sensor and an I2C Diplay with the Wemos D1**
In this evolution of the code, we loose the ability to send messages to the HTTP server we had in the first experiment, but gain the ability to read from a BME280
sensor module and display Temperature/Humidity/Pressure and Altitude on the display as well as return these values as HTML to be viewd on a browser that can access
the Wemos.
Continues to use poor security practices. But hey I am still learning.

# References
[I2C Display and Web Server](https://blog.hau.me/2018/12/26/i2c-oled-display-on-a-wemos-d1-mini/)
[BME280 and NodeMCU Weather station](https://lastminuteengineers.com/bme280-esp8266-weather-station/)
