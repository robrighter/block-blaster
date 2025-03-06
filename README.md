# Block Blast - A Raylib Puzzle Game

Block Blast is a simple but addictive puzzle game built using the [Raylib](https://www.raylib.com/) library in C++. It's inspired by games like 1010! and Tetris, combining block placement with line-clearing mechanics.

## Features

*   **Drag-and-Drop Gameplay:**  Intuitively drag blocks from the selection area onto the grid.
*   **Rotation:** Rotate blocks before placing them using the 'Q' and 'E' keys (or 'Z' and 'X' in some layouts).
*   **Line Clearing:**  Complete horizontal or vertical lines to clear them and earn points.
*   **Gravity:** After clearing lines, blocks above will fall down to fill the empty spaces.
*   **Scoring System:**
    *   Points awarded for each block segment placed.
    *   Points awarded for clearing lines (bonus for multiple lines).
*   **Game Over:** The game ends when no more blocks can be placed.
*   **Restart:** Press 'R' to restart the game after it ends.
*   **Gradient Blocks:** Blocks are drawn with a visually appealing gradient.
*   **Ghost Placement:** A semi-transparent preview shows where the block will be placed, turning red if the placement is invalid.
*   **Clean UI:**  Includes a score display, game title, and instructions.

## Building and Running

### Prerequisites

*   **Raylib:** You need to have the Raylib library installed.  Follow the instructions on the [Raylib website](https://www.raylib.com/)'s installation page or their GitHub repository.  The specific installation steps vary depending on your operating system (Windows, macOS, Linux) and development environment.  Common methods include:
    *   **Using a package manager:** (e.g., `apt-get` on Debian/Ubuntu, `brew` on macOS, `pacman` on Arch Linux)
    *   **Building from source:** Download the source code from GitHub and compile it.
    *   **Using a pre-built library:** Download pre-compiled binaries for your system.
*   **C++ Compiler:** You'll need a C++ compiler (like g++, clang++, or MinGW).

### Compilation (Linux/macOS - using g++)

1.  **Navigate:** Open a terminal and navigate to the directory containing the `block_blast.cpp` file (or whatever you name the source file).
2.  **Compile:** Use a command similar to the following to compile the game:

    ```bash
    g++ block_blast.cpp -o block_blast -lraylib -lGL -lm -lpthread -ldl -lX11
    ```

    *   `block_blast.cpp`:  The name of your C++ source file.
    *   `-o block_blast`:  Specifies the name of the output executable file (`block_blast` in this case).
    *   `-lraylib`: Links the Raylib library.
    *   `-lGL -lm -lpthread -ldl -lX11`: These are additional libraries that Raylib often depends on (OpenGL, math, pthreads, dl, X11).  You might need to adjust these slightly depending on your specific Raylib installation and OS.

### Compilation (Windows - using MinGW and g++)

1.  **Install MinGW:** Make sure you have MinGW (Minimalist GNU for Windows) installed and set up correctly.  This provides the `g++` compiler.
2.  **Raylib with MinGW:**  Ensure you have the correct version of Raylib compiled for MinGW.
3.  **Compile:**  Use a command like this in the MinGW terminal:

    ```bash
    g++ block_blast.cpp -o block_blast.exe -I"path/to/raylib/include" -L"path/to/raylib/lib" -lraylib -lopengl32 -lgdi32 -lwinmm
    ```
    *   Replace `"path/to/raylib/include"` and `"path/to/raylib/lib"` with the actual paths to your Raylib include and lib directories.
    *   `-lraylib -lopengl32 -lgdi32 -lwinmm`: These are the necessary libraries on Windows.

### Compilation (Using an IDE - e.g., Code::Blocks, Visual Studio)

1.  **Create a Project:** Create a new C++ project in your IDE.
2.  **Add Source File:** Add the `block_blast.cpp` file to your project.
3.  **Configure Raylib:**
    *   **Include Directories:** Add the Raylib `include` directory to your project's include paths.
    *   **Library Directories:** Add the Raylib `lib` directory to your project's library paths.
    *   **Linker Settings:** Add the necessary Raylib libraries (e.g., `raylib`, `opengl32`, `gdi32`, `winmm` on Windows) to your linker settings. The specific libraries will depend on your OS and Raylib configuration.
4.  **Build:** Build (compile and link) your project.

### Running

1.  **Open a Terminal:** Open a terminal or command prompt.
2.  **Navigate:** Navigate to the directory where the compiled executable (`block_blast` or `block_blast.exe`) is located.
3.  **Run:** Execute the game:

    ```bash
    ./block_blast  # On Linux/macOS
    block_blast.exe  # On Windows
    ```

## Gameplay

1.  **Drag and Drop:** Click and hold the left mouse button on one of the three available blocks at the bottom of the screen. Drag the block onto the grid.
2.  **Release to Place:** Release the mouse button to place the block.  The block will only be placed if it's a valid move.
3.  **Rotate Blocks:** Use the 'Q' and 'E' keys (or 'Z' and 'X') to rotate the currently dragged block clockwise and counter-clockwise, respectively.
4.  **Clear Lines:**  Fill an entire horizontal or vertical line with blocks to clear it and earn points.
5.  **Game Over:** The game ends when you can no longer place any of the three available blocks on the grid.
6. **Restart** Press 'R' to start a new game.

## Future Improvements (Possible Enhancements)

*   **Level System:** Introduce levels with increasing difficulty (e.g., faster gravity, more complex shapes).
*   **High Score Tracking:** Save and display high scores.
*   **Sound Effects:** Add sound effects for block placement, line clears, and game over.
*   **Music:** Include background music.
*   **Power-Ups:**  Introduce occasional power-ups (e.g., a bomb to clear a small area, a "shuffle" to get new blocks).
*   **Different Game Modes:**  Add alternative game modes (e.g., a timed mode).
*   **Undo:** Allow the player to undo their last move.

## Contributing

Feel free to fork this repository and submit pull requests with improvements or new features.

## License

This project is released under the [MIT License](LICENSE) (you'll need to create a `LICENSE` file with the MIT License text).  This is a very permissive open-source license.