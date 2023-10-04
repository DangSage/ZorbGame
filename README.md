# ZorbGame
This is a C++ game using the basic principles of the language including Polymorphism, Object-Oriented Programming and Inheritance using the Standard Template Library, as well as Classes and Objects to create a Alien Army Rogue-Like.

Here are some of the implementations in this file so far:

    zUtility.hpp
        zUtility.hpp provides utility functions and macros for various purposes. It includes functions for handling random values, string manipulation, text formatting, and memory object counting. This header is essential for general utility functions used throughout the Zorb project.
        
        Functions:
            RandomValue(min, max): Generates a random value within a specified range.
            SplitMultilineString(multilineString): Splits a multi-line string into individual lines.
            CenterAlignString(input, width): Center-aligns a string within a given width.
            PrintFormattedText(text, color): Prints text with optional color formatting.
            FormattedText(text, color): Returns text with optional color formatting.
            CountGameObjectsInMemory(): Prints counts of Zorb and ZorbAppearance objects in memory.
            SpaceToPrint(spaces): Returns a string with a specified number of spaces.
            GetLengthWithoutEscapeCodes(input): Calculates the length of a string without ANSI escape codes.

    Zorb.hpp
        Zorb.hpp defines the Zorb class, which represents a game object in the Zorb game. It includes constructors for creating Zorb objects with different attributes, accessors and mutators for Zorb properties, and overloads operators for various operations. It also includes friend functions for custom output formatting and comparisons.

        Functions:
            Constructors for Zorb objects with various parameters.
            Copy and move constructors for Zorb objects.
            Accessor and mutator functions for Zorb properties.
            Overloaded operators for output, comparison, and addition of Zorb objects.

    ZorbAppearance.hpp:
        ZorbAppearance.hpp defines the ZorbAppearance class, which represents the appearance of a Zorb game object. It includes constructors for creating ZorbAppearance objects, a function to set the appearance based on an enum, and accessor functions for appearance and color. The header also defines an enum for appearance options and a map for appearance names.

        Functions:
            Constructors for ZorbAppearance objects with various parameters.
            SetAppearance function to set the appearance based on an enum.
            Accessor functions for appearance and color.

    Z_UI.hpp:
        Z_UI.hpp defines the UI class, responsible for displaying Zorb game information. It includes constructors, functions for displaying Zorbs in various formats (table, ASCII art, simple text, color), and functions for creating styled text boxes, dividers, and clearing the screen. The header also provides functions for displaying title and debug screens, enhancing the user interface of the game.

        Functions:
            Constructors for the UI class with default display format.
            Functions for displaying Zorbs in different formats.
            Functions for creating styled text boxes, dividers, and clearing the screen.
            Functions for displaying title and debug screens.
