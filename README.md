# Arduino CanBUS WIP
Contains all my experiments with Arduino and CANbus on my mk2.5 Focus.

---

I'm attempting to hack up my Focus so i can make it do cool things, through Canbus.

First job is to read messages and attempt to decipher their meaning.

This repo contains various sketches at different stages of this project.

After each major step forward a new sketch will be added with that change.

Current captured messages and their meanings are stored within the excel spreadsheet in the root.

# Sketches

## Print
Simply prints out all messages recieved.

## Filtered Listener
This provides the ability to filter out mesages ID's and only print ID's that have not already emitted.

## FilteresMessageData
Allows to listen to specific message ids. Will optionaly filter the first block of the message data to only list messages where that block is unique.

* Pressing up will put board into capture mode. Don't touch controls and wait untill no more messages IDs are indicated as captured. 
* Press up again to go out of capture mode.
* Press Center to enable fixed id mode, this will limit message printed to only ones that match the selected message id
* Press left and right to cycle through captured ids to select the one to filter on

## SendMessage1

Was the first attempt at emitting a message to the car. Message id 210 was emitted with a payload to indicate (what i believe is) locking the car.
However the car did not lock. The bus came alive, indicating the message was sent. When the bus was alive it emitted the 210 message back with the updated values which was odd. Maybe the car now though the doors were locked when they wasn't?