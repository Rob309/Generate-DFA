# Regex to NFA to DFA Converter

This project is a **C++ tool** that converts a regular expression into a **Nondeterministic Finite Automaton (NFA)** and then into a **Deterministic Finite Automaton (DFA)** using standard formal language theory algorithms.

It reads a regular expression from a text file, builds its corresponding NFA, and then determinizes it into a DFA. The goal of this project is to demonstrate an understanding of automata theory, regex parsing, and state machine generation.

---

## ✅ Features

- Support for basic regular expression syntax:
  - Concatenation
  - Alternation ( `|` )
  - Kleene Star ( `*` )
  - Grouping with parentheses `( )`
- Clear output of generated NFA and DFA structures

---

## 🛠️ Technologies Used

- **Language:** C++
- **File I/O:** For reading regex and writing output
- **Data Structures:** Custom state classes for NFA/DFA transitions and sets
- **Compiler:** Works with modern C++ compilers (C++17 and above)

---

## 📄 How to Use

1. Clone the repository
2. Write your regular expression in regex_input.txt, for example: (a|b)*abb
3. Compile and run the program
