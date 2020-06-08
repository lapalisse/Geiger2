# Geiger2
Basic Arduino Geiger counter with a bit of a memory and a bit of intelligence.

Let's say, this is step 2 in the long line of your Arduino Geiger counter software: just a bit more elaborate than the very basic one that just counts clicks.

Provides the following functionalities:
  - displays number of clicks or radioactivity on past periods of time
  - cpm or uSv/h
  - has a memory of past values
  - if you ask for an average on the previous hour, and only 26 minutes are available, just base the calculation on this
  - fast computing...
  
Example: <br>
...<br>
3537 127 0 0 0 17 127 2.1544 0.0143 uSv/h<br>
3538 127 0 0 0 17 127 2.1538 0.0143 uSv/h<br>
3539 127 0 0 0 17 127 2.1532 0.0143 uSv/h<br>
3540 127 0 0 0 17 127 2.1525 0.0143 uSv/h<br>
3541 127 0 0 0 17 127 2.1519 0.0143 uSv/h<br>
3542 127 0 0 0 17 127 2.1513 0.0142 uSv/h<br>
3543 127 0 0 0 17 127 2.1507 0.0142 uSv/h<br>
3544 127 0 0 0 17 127 2.1501 0.0142 uSv/h<br>

In order:
1. number of seconds
2. clicks count
3. Clicks in past second
4. Clicks in past 10 seconds
5. Clicks in past 60 seconds
6. Clicks in past 10 minutes
7. Clicks in past 1 hour
8. Average clicks per second based on last hour data
9. Average radioactivity in uSv/h based on last hour data

PS: Feel free to contact me on this subject
You can also hire me if you want to do something elaborate, which requires special/original computing skills :-)

# How much memory do I need?

I've got a Mega2560, which has 8kbytes of SRAM, so I can store 30min of 16-bit values (every second), or 1 full hour of 32-bit values, but you may need to adapt this software to the limits of your card.

That said, another way to handle more data would be to store values not every second, but every say 10 seconds.

Talk to me if you want this functionality.
