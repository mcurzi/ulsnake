# ulsnake (Unix-like Snake)

A classic Snake game clone written in **pure C** using the `ncurses` library. Originally developed for Windows around **2004** and later ported to Unix-like systems, this project is a "time capsule" of mid-2000s terminal programming.

## 🕹️ Overview

`ulsnake` is a lightweight, terminal-based game where you control a snake, collect numbers to grow, and avoid crashing into walls or your own tail. It was built during an era where documentation was scarce, requiring manual implementation of game logic and terminal management.

## 🛠️ Technical Features

*   **NCurses Interface:** Uses the `ncurses` library for window management, color pairs, and non-blocking keyboard input (`nodelay`).
*   **Manual Memory Management:** Implements a circular-buffer-style logic to manage the snake's body coordinates, reusing array space to maintain a minimal memory footprint.
*   **Collision Engine:** Uses a coordinate matrix to handle real-time collision detection for walls, the snake's body, and items.
*   **Legacy Portability:** Originally written in the Turbo C++/Borland era and later migrated to ANSI C for modern Unix/Linux distributions.

## 🚀 Installation & Run

To compile and run `ulsnake`, you need the `ncurses` development libraries installed.

1. **Clone the repository:**
   ```bash
   git clone https://github.com/mcurzi/ulsnake.git
   cd ulsnake
   ```

2. **Compile using the provided Makefile:**
   ```bash
   make
   ```

3. **Run the game:**
   ```bash
   ./ulsnake

   ```

## 📜 Controls

*   **Arrow Keys / WASD:** Move the snake.
*   **Space / P:** Pause the game.
*   **Enter:** Confirm selection.
*   **Q:** Quit.

---

### 🕹️ Historical Note

This is one of my first projects in C, dating back to 2004. Originally written for Windows, I later ported it to Linux using `ncurses` to keep it functional.

The code is far from modern standards, but I’m keeping it here as a reference of how I first approached collision logic and memory management.

## 💾 Legacy Version (MS-DOS)
Inside the `dos-legacy` folder, you can find the original 2004 version of the game.
It was written for Borland Turbo C++ and uses DOS-specific headers like `conio.h` and `dos.h`.
It features:
* 80x50 text mode support.
* Hardcoded cheat codes (`1357`, `2468`).
* Classic PC-Speaker style timing with `delay()`.

