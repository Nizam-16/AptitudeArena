# AptitudeArena — C++ Mini Project

A console-based **aptitude test simulator** written in C++. It combines a fixed
question bank with randomly generated questions, so every run gives a fresh quiz
with instant feedback and a performance report.

## Features

- **15 handcrafted questions** across 6 topics:
  Percentages · Profit & Loss · Time-Speed-Distance · Ratio · Average · Time & Work
- **Auto-generated questions** — random values are plugged into aptitude formulas
  each round, so the quiz is never the same twice
- **Topic selection** — practise one topic or mix everything
- **Shuffled questions and answer options** every play-through
- **Instant feedback** with a step-by-step solution after each answer
- **Final report** — score, percentage, grade, and topic-wise accuracy
- **Input validation** — handles invalid entries gracefully
- **Play-again loop**

## Requirements

- A C++17 compiler (g++, MinGW, Clang, or MSVC)
- No external libraries needed

## Build & Run

```bash
# Compile
g++ -std=c++17 -Wall -Wextra -o aptitude_quiz aptitude_quiz.cpp

# Run (Linux / macOS)
./aptitude_quiz

# Run (Windows)
aptitude_quiz.exe
```

## Sample Output

```
Q1  [Percentages]
What is 20% of 150?
  1) 25
  2) 30
  3) 35
  4) 40
Your answer (1-4): 2
  + Correct!
  Why: 20/100 x 150 = 30

========================================
             FINAL REPORT
========================================
 Score      : 8 / 10  (80%)
 Grade      : Very Good
 Topic-wise accuracy:
   Percentages           : 3/4  (75%)
   Profit & Loss         : 2/2  (100%)
   Time Speed Distance   : 3/4  (75%)
========================================
```

## Concepts Demonstrated

| Concept | Where it's used |
|---|---|
| Structs + `vector<struct>` | `Question` model and question pool |
| `<random>` (`mt19937`) | Question generation, shuffling |
| STL algorithms | `shuffle`, `copy_if`, `find` |
| `map` + `array` | Topic-wise score statistics |
| Input validation | `readInt()` with `cin.clear()` recovery |
| Functions, loops, conditionals | Overall program flow |

## Project Structure

```
.
├── aptitude_quiz.cpp   # Source code (single file)
├── .gitignore
└── README.md
```

## Possible Extensions

- ⏱️ Per-question timer using `<chrono>`
- 💾 High-score persistence with `<fstream>`
- 📈 Adaptive difficulty based on performance
