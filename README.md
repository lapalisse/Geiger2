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
1. Number of seconds
2. Clicks since boot
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

I've got a Mega2560, which has 8kbytes of SRAM, so I can store 30min of 32-bit values (every second), or 1 full hour of 16-bit values, but you may need to adapt this software to the limits of your card.

That said, another way to handle more data would be to store values not every second, but every say 10 seconds.

You'll probably want to modify the MEMORY_SIZE constant, which contains the max number of seconds you'll be storing click data. (don't forget to add 1, because if you need n deltas, that means you'll need n+1 values!)

Using 16-bit values is a way to have more values, but you'll probably overflow after around 26 seconds of high radiation. So it's your choice, either to have a lot of values in memory, or to be able to handle high radiation doses...

Talk to me if you want this functionality. :-)

# Ideas for improvement

Thinking of another version of this software, how about the following ideas:
1. Storing how much time your Geiger-Müller tube has been working for in the EEPROM (they don't last forever, and I think it makes sense to just know how much time they've been working)
2. Storing data n seconds at a time, instead of for every second...
3. Of course putting everything in a lovely portable box :-)

# Simple design

I think I like simple designs, and I want to keep this Arduino software simple, so that people can understand how it works. I'll probably create a Geiger3 which will be more sophisticated.
