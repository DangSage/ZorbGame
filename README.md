# Zorb Game Header Files

This repository contains several header files that are essential for the Zorb game project. Each header file serves a specific purpose and provides various functions and classes. Here's an overview of each header file and its functions:

## `zUtility.hpp`

### Functions
- `RandomValue(min, max)`: Generates a random value within a specified range.
- `SplitMultilineString(multilineString)`: Splits a multi-line string into individual lines.
- `CenterAlignString(input, width)`: Center-aligns a string within a given width.
- `PrintFormattedText(text, color)`: Prints text with optional color formatting.
- `FormattedText(text, color)`: Returns text with optional color formatting.
- `CountGameObjectsInMemory()`: Prints counts of Zorb and ZorbAppearance objects in memory.
- `SpaceToPrint(spaces)`: Returns a string with a specified number of spaces.
- `GetLengthWithoutEscapeCodes(input)`: Calculates the length of a string without ANSI escape codes.

### Summary
`zUtility.hpp` provides utility functions and macros for various purposes. This header is essential for general utility functions used throughout the Zorb project.

---

## `Zorb.hpp`

### Functions
- Constructors for Zorb objects with various parameters.
- Copy and move constructors for Zorb objects.
- Accessor and mutator functions for Zorb properties.
- Overloaded operators for output, comparison, and addition of Zorb objects.

### Summary
`Zorb.hpp` defines the Zorb class, which represents a game object in the Zorb game. This header is responsible for creating, managing, and displaying Zorb objects.

---

## `ZorbAppearance.hpp`

### Functions
- Constructors for ZorbAppearance objects with various parameters.
- `SetAppearance` function to set the appearance based on an enum.
- Accessor functions for appearance and color.

### Summary
`ZorbAppearance.hpp` defines the ZorbAppearance class, responsible for managing the appearance of Zorb game objects. It provides functions to set and retrieve appearance information.

---

## `Z_UI.hpp`

### Functions
- Constructors for the UI class with default display format.
- Functions for displaying Zorbs in different formats.
- Functions for creating styled text boxes, dividers, and clearing the screen.
- Functions for displaying title and debug screens.

### Summary
`Z_UI.hpp` defines the UI class, responsible for displaying Zorb game information in a user-friendly way. It enhances the user interface of the game by providing functions for displaying Zorbs and other UI elements.

These header files collectively provide the essential components for the Zorb game, including utility functions, the Zorb and ZorbAppearance classes, and the user interface through the UI class. They work together to create and display Zorb objects, manage their appearances, and provide a user-friendly interface for the game.

For detailed usage and implementation, please refer to the individual header files and their associated source code in this repository.
