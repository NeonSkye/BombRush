# BombRush

## Description

This is the repository for Bomb Rush, a project made in Arduino on a LED Matrix, for the Intro to Robotics course (F.M.I Unibuc 2023-2024, Year III).

## Why this game?

I didn't have a specific source of inspiration (at least not consciously.). The only inspiration was the fact that this game would work on the limited hardware available.


## Components

Many wires, an 8x8 LED Matrix, Driver for said Matrix, an LCD Display 16x2, Joystick, Potentiometer, and ofcourse (the star of the show) an Arduino UNO board.

## Features
Immersive gameplay! You will feel how the bomb looks just by looking at the circuit itself!

Story-rich world! You are a pixel trapped in a 16x16 LED hell! With bombs!

Adrenaline inducing! The timer might get faster when you've got enough bombs...


## Lore
You find yourself in a weird house with 4 rooms, symbols in each room! You must defuse bombs for all eternity (or until you blow up.). Perhaps this is hell. 

One must imagine Sisyphus happy.

## How To Play
You will get a symbol and 2 coordinates. You must find the room with that symbol, go to said coordinates and defuse the bomb! 

You have until the time runs out to defuse as many bombs as possible. You **will** blow up eventually. Make the best of your time.

## Controls

Joystick for UP/DOWN/LEFT/RIGHT (no diagonal movement.).

Joystick Button for interact (either disarm the bomb, or select a menu option.).

## Game Map 
disclaimer: Artistic Representation. Does not match actual in-game colours.
![image](https://github.com/NeonSkye/BombRush/assets/92863316/24a225d8-ec51-4809-a134-92e48a494622)


## Gallery
### Photos
![image](https://github.com/NeonSkye/BombRush/assets/92863316/1410aec2-0e7f-42da-82e5-dfde25b285d5)
![image](https://github.com/NeonSkye/BombRush/assets/92863316/acd3ca55-459d-4c2f-9984-601b29fdf9e3)
![image](https://github.com/NeonSkye/BombRush/assets/92863316/b2b4d072-9341-4ebb-af73-b5f53f80fd54)



### Video Demonstration
https://www.youtube.com/watch?v=RnJOHuNkpkI

## Known Issues
### Software
- No highscore implementation
- Doesn't save settings to EEPROM. Nothing gets saved to EEPROM.
- W.I.P section in Settings
- No soundtrack or SFX (see Hardware Issues)
- No intro message, straight into the menu. (just like life, I think)
- (very specific) In Matrix Brightness settings, when you go from 10 to 9, the 0 lingers until you change to 8. I don't know why, I put lcd.clear() and it still doesn't clear.

### Hardware
- No buzzer :(
- Can't control LCD Brightness.
- The joystick cover sometimes gives up. Me too

### Other
- The video is slightly outdated. When you die it now shows score upon death. I changed the death screen after the video uploaded. (This shouldn't matter if the project works after being transported.).
