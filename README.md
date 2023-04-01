# Arcade

Arduino Arcade Emulator

## Games

- Pong
- Snake
- Tetris
- Space Invaders

## Components

All components is listed below:

```cpp
• [1] Arduino Uno
• [1] Display MAX7219
• [1] Display TM1637
• [4] Potentiometer
• [4] Push Button
• [4] Resistor 1k ohms
• [-] Jumpers
```

## Requirements

Sample

## Compiling

Sample

## Controls

Each control is solely associated a pin in Arduino and each game have itself functions to controls.

### General

Here is the general controls, shared by all games:

```cpp
Potenciometers:
	• A0: Control game level
	• A1: Control displays bright
```

### Selector

Here is the selector controls:

```cpp
Potenciometers:
	• A2: Change game

Buttons:
	• 10: Select game
	• 11: Select game
	• 12: Select game
	• 13: Select game
```

### Pong

Here is the Pong controls:

```cpp
Potenciometers:
	• A2: Control left player
	• A3: Control right player
```

### Snake

Here is the Snake controls:

```cpp
Buttons:
	• 10: Move snake to left
	• 11: Move snake to down
	• 12: Move snake to up
	• 13: Move snake to right
```

### Tetris

Here is the Tetris controls:

```cpp
Potenciometers:
	• A2: Move piece
	• A3: Rotate piece

Buttons:
	• 10: Drop piece in floor
	• 11: Drop piece in floor
	• 12: Drop piece in floor
	• 13: Drop piece in floor
```

### Space Invaders

Here is the Space Invaders controls:

```vim
Potenciometers:
	• A2: Move ship

Buttons:
	• 10: Shot a bullet
	• 11: Shot a bullet
	• 12: Shot a bullet
	• 13: Shot a bullet
```
