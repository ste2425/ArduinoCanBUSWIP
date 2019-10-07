# Arduino CanBUS WIP
Contains all my experiments with Arduino and CANbus on my mk2.5 Focus.

---

I'm attempting to hack up my Focus so i can make it do cool things, through Canbus.

First job is to read messages and attempt to decipher their meaning.

This repo contains various sketches at different stages of this project.

After each major step forward a new sketch will be added with that change.

# Sketches

## Print
Simply prints out all messages recieved.

## Filtered Listener
This provides the ability to filter out mesages ID's and only print ID's that have not already emitted.