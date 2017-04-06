# TPK4128_project
NTNU project on the raspberry pi


Project description:

"Requirement for the project is that you can open a webpage from your phone and see "current data" from your Raspberry. This data can either be created (from internal sensor or connected device on raspberry) or be retreived from webpage (reading from XML/JSON APIs etc)."

I choose to implement the mifare nested attack (RFID crypto key crack) on a cheap RFID reader largely used by arduino enthusiasts, check out https://github.com/HakonHystad/MFRC522_nested_attack for more info. I also made a top level driver to utilize the HDMI port's I2C for an alpha-numerical 1602 LCD screen showing status messages, check out https://github.com/HakonHystad/pi_HDMI_to_LCD for more info.

To interface the web I wrote a class which inherits from a socket base class to build a POST request and send the retrived RFID data. This is all done in C++. The website is hosted on NTNU servers and consists of a couple of minimal pages in HTML to display the info. The POST requests are handled in PHP and uses MYSQL to store the information. To update the information I poll another PHP script (with AJAX) which gets the data from the SQL server.

Project run:

- Try default keys, send the data for each sector that open as well as fillers for the locked ones.
- Run recovery of the unknown cryptography keys and send each unlocked section, while moving on to the next.
- The sent data is inserted in a formated table in my SQL database, and retrived in the same order. 
- The retrival formats the data in a HTML table, this is updated every other second or so and displayed on my webpage.
