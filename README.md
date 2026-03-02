# CS 210 Project Three – Corner Grocer Item Tracker

## Project Summary

This program was built for the Corner Grocer to analyze daily purchase records and help the store understand item purchase frequencies. The store needed a way to process a text-based log of items purchased throughout the day so they could rearrange their produce section based on customer buying patterns. The program reads the input file, builds a frequency map of all purchased items, automatically creates a backup data file (`frequency.dat`), and provides an interactive menu with four options: looking up a single item's frequency, displaying all item frequencies, displaying a text-based histogram, and exiting the program.

## What I Did Particularly Well

I focused on writing clean, well-organized code with a clear class structure that separates concerns effectively. The `GroceryTracker` class encapsulates all data processing logic with a well-defined public interface and private implementation details. I also implemented robust input validation that gracefully handles non-integer input, out-of-range selections, and empty or whitespace-only item lookups. The `normalise()` helper function ensures case-insensitive matching so that "apple", "Apple", and "APPLE" all resolve to the same key — a small detail that significantly improves the user experience. I also made sure to use `unsigned char` casts inside `toupper` and `tolower` to avoid undefined behavior with signed characters, which demonstrates attention to writing secure, standards-compliant code.

## Where I Could Enhance the Code

There are a few areas where I could improve the program:

- **Histogram alignment**: The padding in `printHistogram()` is hardcoded to 15 characters. A more robust approach would dynamically calculate the longest item name and pad accordingly, making the output adapt to any dataset.
- **Case handling at file read**: Currently, normalization happens both at load time and at query time. Centralizing this logic or documenting the design decision more explicitly would improve clarity.
- **Unit testing**: Adding unit tests for the `GroceryTracker` class methods would make the code more reliable and easier to maintain as features are added.
- **Configuration**: Hardcoded file names in `main()` could be replaced with command-line arguments or a configuration file, making the program more flexible for different deployments.

## Challenges and How I Overcame Them

The most challenging part was implementing reliable input validation in the menu system. Handling cases where users enter non-numeric input (like letters or special characters) required understanding how `std::cin` manages its error state and input buffer. I overcame this by researching `std::cin.fail()`, `std::cin.clear()`, and `std::cin.ignore()` through the zyBooks material and C++ reference documentation. Working through edge cases — such as a user typing "2abc" or just pressing Enter — helped me understand stream handling at a deeper level. I'm adding cppreference.com and zyBooks to my go-to support resources for future C++ work.

## Transferable Skills

Several skills from this project will carry forward into other coursework and professional work:

- **File I/O and data processing**: Reading, parsing, and writing structured data files is fundamental to almost every software project.
- **Object-oriented design**: Designing a class with clear public/private boundaries and encapsulated logic is a pattern I'll use across languages and projects.
- **Input validation and error handling**: Anticipating and handling bad user input is critical for building secure, production-quality software.
- **Using STL containers**: Working with `std::map` for efficient key-value storage is directly applicable to many data-driven applications.

## Maintainability, Readability, and Adaptability

I made the program maintainable and readable by using descriptive naming conventions (e.g., `m_frequencyMap`, `loadFromFile`, `writeBackupFile`), consistent formatting, and thorough inline comments that explain the *why* behind decisions rather than just restating the code. The class-based design makes the program adaptable — new menu options or output formats can be added without modifying the core data-processing logic. The use of `std::map` as the underlying data structure keeps the implementation straightforward while automatically providing sorted output, and swapping it for an `unordered_map` would be a simple change if performance with larger datasets became a concern.
