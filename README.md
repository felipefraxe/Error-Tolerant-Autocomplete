# Autocomplete with Error-Tolerant Query Processing

This project implements an efficient error-tolerant autocomplete system inspired by the research paper [BEVA: An Efficient Query Processing Algorithm for Error-Tolerant Autocompletion](https://www.researchgate.net/publication/303789335_BEVA_An_Efficient_Query_Processing_Algorithm_for_Error-Tolerant_Autocompletion).

## Features

- **Trie-based Data Structure**: Efficient storage and retrieval of strings.
- **Edit Distance Automata**: Supports error-tolerant query matching using an edit distance threshold.
- **Bitmap Encoding**: Optimized bitmask representation for character matching.
- **Vector Trie**: A specialized data structure for mapping edit vectors to states efficiently.
- **Normalization**: Handles case folding and diacritic removal for robust matching.

## Project Structure

- `src/`: Contains the main implementation files.
  - `trie.cpp`: Implements the Trie data structure for storing and traversing words.
  - `eva.cpp`: Implements the Edit Vector Automata for error-tolerant matching.
  - `bitmap.cpp`: Implements the Bitmap for efficient character encoding and bitmask extraction.
  - `vecTrie.cpp`: Implements the Vector Trie, a compact and efficient data structure for mapping edit vectors to states.
  - `utils.cpp`: Utility functions for string manipulation and other helpers.
- `include/`: Header files for the project.
- `tests/`: Unit tests for various components of the system.
- `data/`: Sample data files for testing.

## Build Instructions

To build the project, use the provided `Makefile`:
```sh
make
```

The compiled binary will be located in the `bin/` directory.

## Usage

Run the program with a file containing suggestions:

```sh
./bin/autocomplete <suggestions_file>
```

You can then input queries interactively, and the program will return error-tolerant autocomplete suggestions.
