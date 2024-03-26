# cPong

A simple yet engaging pong game written in C using the `Raylib` library. This game brings the classic pong gameplay with a modern programming approach, offering a perfect blend of fun and coding elegance. Suitable for developers looking for a project to study or modify, as well as anyone who enjoys a good old game of pong.

## Note to the reader

As you explore the code and functionalities of this game, it's important to understand the context in which it was developed. This project was born out of a desire to delve deeper into the capabilities and uses of `Raylib`, a simple and easy-to-use library designed to make video games programming an enjoyable experience.

It's worth noting that optimization and formatting were not the primary focus during this project's development. The intention was never to create a piece of software for professional use or to meet the rigorous standards often required in a commercial or company software context. Instead, this game stands as a testament to the learning process, embodying the spirit of exploration and the joy of discovering new possibilities in the realm of game development.

## Features

- Classic pong gameplay with a competitive twist.
- Score tracking for intense 1v1 matches.
- Simple AI that controls the enemy paddle, making every game a challenge.
- Smooth animations and transitions for a polished experience.
- Customizable game settings including paddle speed, ball speed, and winning score. (if you can read code)

## Getting Started

To get started with cPong, you'll need to have the `Raylib` library installed on your system. `Raylib` is a simple and easy-to-use library to enjoy videogames programming. For more information on installing `Raylib`, visit the [official Raylib installation guide](https://github.com/raysan5/raylib/wiki#installing-raylib).

## Prerequisites

- A C compiler (GCC, Clang, etc.)
- `Raylib` library installed and configured
- Basic understanding of C programming

## Installation

```sh
   git clone https://github.com/yourusername/cPong.git
   cd cPong
   gcc -o cPong main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
   # Or, in case you have the library statically installed in your computer
   make build-linux
   ./build/cPong
```

## Controls

- Player 1 Movement: `W` to go UP, and `S` to go down.
- Player 2 Movement: Automatically controlled by simple AI.
- Restart Game: Press `R` to restart the game after a win.
- Quit Game: Press `Q` to quit the game after a win.

## Game Rules

- The game starts with a countdown. Once the countdown is over, the ball launches and the game begins.
- Each player controls a paddle. The goal is to prevent the ball from passing your paddle.
- When a player misses the ball, the other player scores a point.
- The first player to reach 10 points wins the game.
- After scoring, the game resets the ball and paddles, and a new round begins after a short countdown.

# License

Distributed under the MIT License. See LICENSE for more information.

So, if you read and understood the note in the start of this document, do whatever you want with this code.
