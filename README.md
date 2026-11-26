# 🎮 2D Platformer Jump System — C++ & Raylib

This project is a small, modular jumping system for 2D platformers, developed using **C++ and Raylib**.

I created this project to explore how common platformer mechanics can work together to make character movement feel more responsive, forgiving, and customizable.

I wanted to improve the overall feel of the character's movement, so I experimented with different approaches to make jumping more natural and responsive. I treated it as a personal challenge, focusing on integrating each mechanic in a way that felt right to me. The result includes **Coyote Time, Jump Buffering, and Variable Jump Height**.

## ✨ Features

The system currently implements three movement mechanics:

- **Coyote Time:** Allows the player to jump for a short period after leaving a platform.
- **Jump Buffering:** Registers jump inputs shortly before landing, allowing the player to jump as soon as they touch the ground.
- **Variable Jump Height:** Allows the player to influence the height of their jump by holding the jump button.

### 🐺 Coyote Time

Have you ever walked off a platform and pressed jump just a little too late?

Coyote Time makes that situation more forgiving by providing a small window in which the player can still jump after leaving the ground.

The duration is customizable through `coyoteDuration`.

### ⏳ Jump Buffering

Sometimes you press jump just before your character touches the ground.

Without jump buffering, that input might be ignored, forcing you to press the button again.

This implementation stores the input for a short period, allowing the jump to execute when the player becomes grounded.

You can adjust this behavior using `bufferDuration`.

### 🦘 Variable Jump Height

Not every jump needs to reach the same height.

By holding the jump button, the system maintains upward velocity while the player is below a predefined jump target.

This creates a simple way to control jump height and gives the player more influence over their movement.

## &#x20;Configuration

The main movement parameters are defined inside the `Player` and `JumpState` structures.

| Parameter        | Description                                                |
| ---------------- | ---------------------------------------------------------- |
| `moveSpeed`      | Horizontal movement speed.                                 |
| `jumpForce`      | Initial upward jump velocity.                              |
| `bufferDuration` | Maximum time a jump input remains buffered.                |
| `coyoteDuration` | Maximum time the player can jump after leaving the ground. |
| `targetY`        | Vertical position used to control the jump height.         |

You can experiment with these values to find the movement style that works best for your game.

## &#x20;Requirements

- C++
- [Raylib](https://www.raylib.com/)

## &#x20;Getting Started

1. Clone or download the repository.
2. Make sure Raylib is installed and configured.
3. Compile the source code with your preferred C++ compiler.
4. Run the executable and experiment with the movement mechanics.

## &#x20;Controls

| Input                | Action                                                    |
| -------------------- | --------------------------------------------------------- |
| A / Left Arrow       | Move left                                                 |
| D / Right Arrow      | Move right                                                |
| Space / W / Up Arrow | Jump                                                      |
| Hold Jump            | Maintain upward movement until the jump target is reached |

## 🤝 Feedback and Contributions

Feel free to explore the implementation, experiment with the parameters, and adapt the mechanics to your own projects.

Suggestions, improvements, and different approaches are always welcome!

## ❤️ Thanks for Checking Out This Project!

I hope this implementation gives you a useful starting point for experimenting with 2D platformer movement.

Have fun building your games! 🚀
