# Hospital Appointment Management System

A console-based hospital appointment manager written in C++. Appointments are stored in a custom **Binary Search Tree (BST)** keyed by **priority level**, so the most urgent patients are always at the front of an in-order traversal. The program loads initial data from a text file on startup and then runs an interactive, menu-driven loop.

---

## Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Project Structure](#project-structure)
4. [How It Works](#how-it-works)
5. [Input File Format](#input-file-format)
6. [Getting Started](#getting-started)
7. [Using the Program](#using-the-program)
8. [Sample Data and Tree](#sample-data-and-tree)
9. [Example Session](#example-session)
10. [Implementation Details](#implementation-details)
11. [Time Complexity](#time-complexity)
12. [Input Validation](#input-validation)
13. [Known Limitations](#known-limitations)
14. [Authors](#authors)

---

## Overview

Hospitals need to know who should be seen first. This project models that problem with a BST where each node is an appointment holding:

| Field | Type | Description |
|-------|------|-------------|
| `patientName` | `string` | Full name of the patient |
| `priorityLevel` | `int` | Urgency of the case. **1 is the most urgent**; larger numbers are less urgent |
| `department` | `string` | Medical department (e.g. Cardiology, Emergency) |

The tree is ordered by `priorityLevel`, which makes in-order traversal return appointments from the most urgent to the least urgent, and makes priority-based operations (search, cancel, urgency filters) natural to express on a BST.

In the code, the "Appointment" concept is implemented by the `Node` class, and the tree itself is the `BST` class.

---

## Features

- **Schedule an appointment**: insert a new patient with a priority level and department.
- **Display all appointments**: print every appointment in order of urgency, preceded by a count.
- **Search by priority level**: list every appointment with exactly the given priority.
- **Cancel by priority level**: remove *all* appointments with the given priority, with the number of cancelled appointments reported.
- **Display more urgent**: list appointments at or more urgent than a given priority level.
- **Display less urgent**: list appointments at or less urgent than a given priority level.
- **File loading on startup**: initial appointments are read from `appointments.txt`. If the file is missing, the system starts empty instead of crashing.
- **Input validation**: priority levels below 1 are rejected, and invalid menu options are handled gracefully.

---

## Project Structure

```
.
├── BST.h               # Node and BST class definitions (all tree logic)
├── main.cpp            # Program entry point: file loading and menu loop
└── appointments.txt    # Sample data loaded when the program starts
```

| File | Purpose |
|------|---------|
| `BST.h` | Defines `Node` (one appointment) and `BST` (the tree and all operations). Implemented entirely in the header. |
| `main.cpp` | Reads `appointments.txt`, builds the tree, and runs the interactive menu. |
| `appointments.txt` | Seed data with 10 sample appointments. |

---

## How It Works

### Ordering rule

Every appointment is placed in the tree by comparing priority levels:

- If the new priority is **less than or equal to** the current node's priority, it goes to the **left** subtree.
- Otherwise it goes to the **right** subtree.

Because equal priorities go left, duplicate priorities are fully supported: several patients can share the same priority level. The order among patients with the same priority is not guaranteed to match the order in which they were added.

### Traversal

Displaying uses an **in-order traversal** (left, node, right), which visits nodes in ascending priority order. Priority 1 (most urgent) is printed first.

### Count-then-print pattern

Several operations first need to know *how many* matches exist so they can print a header such as `3 appointments found.` before listing them. To do this without duplicating code, the recursive helpers take a `print` flag:

1. The first pass runs with `print = false` and only counts matches.
2. If the count is greater than zero, the header is printed and a second pass runs with `print = true` to display them.

### Conditional traversal

`inOrderConditioned` is a single recursive helper shared by three features. A `condition` code selects the filter:

| Condition | Feature | Matches nodes where |
|-----------|---------|---------------------|
| `1` | Display more urgent | `priorityLevel <= priority` |
| `0` | Display less urgent | `priorityLevel >= priority` |
| `2` | Search | `priorityLevel == priority` |

### Cancelling appointments

`cancelAppointment` repeatedly searches the tree for a node with the target priority and deletes it, until no such node remains. Each deletion tracks the node's parent and handles the four standard BST deletion cases:

1. **Leaf node**: the parent's pointer is set to `nullptr` (or the root is cleared).
2. **Only a left child**: the parent is linked directly to that left child.
3. **Only a right child**: the parent is linked directly to that right child.
4. **Two children**: the **in-order successor** (the leftmost node in the right subtree) is located, its data is copied into the node being removed, and the successor node is then unlinked and deleted.

Since equal priorities can appear in more than one place, the search-and-delete step is repeated in a loop. The final message reports how many appointments were removed.

---

## Input File Format

`appointments.txt` must be in the same directory the program is run from. The format is:

- **Line 1**: the number of appointments, `N`.
- Then, for each appointment, **three lines**:
  1. Patient name
  2. Priority level (integer)
  3. Department

Example (two appointments):

```
2
Ahmed Hassan
3
Cardiology
Sara Mohamed
1
Emergency
```

Names and departments are read with `getline`, so they may contain spaces.

If the file cannot be opened, the program prints `File load failed. Starting system empty.` and continues with an empty tree.

---

## Getting Started

### Requirements

- A C++ compiler with C++11 support or newer (for example `g++` or `clang++`).

### Build

From the project folder:

```bash
g++ -std=c++11 main.cpp -o hospital
```

### Run

Make sure `appointments.txt` is in the folder you run the program from.

On Linux or macOS:

```bash
./hospital
```

On Windows:

```bash
hospital.exe
```

You can also open the project in any C++ IDE (Code::Blocks, CLion, Visual Studio, etc.) and run `main.cpp`. Just make sure the working directory is the one that contains `appointments.txt`.

---

## Using the Program

On startup the program loads the file and shows the menu:

```
System starting...
10 appointments loaded successfully.
	Hospital System Menu
	====================
[1] Schedule an appointment
[2] Display all appointments
[3] Search for an appointment
[4] Cancel an appointment
[5] Display more urgent than
[6] Display less urgent than
[7] Exit
----------------------------
Pick an option:
```

| Option | What it asks for | What it does |
|--------|------------------|--------------|
| **1** | Patient name, department, priority level | Inserts a new appointment into the tree |
| **2** | Nothing | Prints all appointments from most to least urgent |
| **3** | Priority level | Prints all appointments with exactly that priority |
| **4** | Priority level | Cancels all appointments with that priority |
| **5** | Priority level | Prints appointments with priority **less than or equal to** the value |
| **6** | Priority level | Prints appointments with priority **greater than or equal to** the value |
| **7** | Nothing | Exits the program |

> **Note:** "More urgent than" and "less urgent than" are inclusive. Searching with priority `3` in option 5 also returns appointments with priority exactly `3`.

---

## Sample Data and Tree

The included `appointments.txt` contains these 10 appointments, inserted in this order:

| # | Patient | Priority | Department |
|---|---------|----------|------------|
| 1 | Ahmed Hassan | 3 | Cardiology |
| 2 | Sara Mohamed | 1 | Emergency |
| 3 | Khaled Ali | 5 | Orthopedics |
| 4 | Mona Adel | 3 | Neurology |
| 5 | Omar Farouk | 2 | Emergency |
| 6 | Layla Ibrahim | 4 | Dermatology |
| 7 | Youssef Nasser | 1 | Emergency |
| 8 | Dina Mostafa | 6 | ENT |
| 9 | Tarek Samir | 2 | Cardiology |
| 10 | Hana Walid | 5 | Ophthalmology |

Following the insertion rule above, the resulting tree looks like this (name and priority shown for each node):

```
Ahmed Hassan (3)
├── L: Sara Mohamed (1)
│   ├── L: Youssef Nasser (1)
│   └── R: Mona Adel (3)
│       └── L: Omar Farouk (2)
│           └── L: Tarek Samir (2)
└── R: Khaled Ali (5)
    ├── L: Layla Ibrahim (4)
    │   └── R: Hana Walid (5)
    └── R: Dina Mostafa (6)
```

An in-order traversal of this tree visits the patients in this order: Youssef Nasser (1), Sara Mohamed (1), Tarek Samir (2), Omar Farouk (2), Mona Adel (3), Ahmed Hassan (3), Layla Ibrahim (4), Hana Walid (5), Khaled Ali (5), Dina Mostafa (6).

---

## Example Session

**Displaying all appointments (option 2):**

```
10 appointments found.
Patient Name: Youssef Nasser       | Department: Emergency       | Priority Level: 1
Patient Name: Sara Mohamed         | Department: Emergency       | Priority Level: 1
Patient Name: Tarek Samir          | Department: Cardiology      | Priority Level: 2
Patient Name: Omar Farouk          | Department: Emergency       | Priority Level: 2
Patient Name: Mona Adel            | Department: Neurology       | Priority Level: 3
Patient Name: Ahmed Hassan         | Department: Cardiology      | Priority Level: 3
Patient Name: Layla Ibrahim        | Department: Dermatology     | Priority Level: 4
Patient Name: Hana Walid           | Department: Ophthalmology   | Priority Level: 5
Patient Name: Khaled Ali           | Department: Orthopedics     | Priority Level: 5
Patient Name: Dina Mostafa         | Department: ENT             | Priority Level: 6
```

**Searching for priority 2 (option 3):**

```
Enter priority level to start search: 2
2 appointments found.
Patient Name: Tarek Samir          | Department: Cardiology      | Priority Level: 2
Patient Name: Omar Farouk          | Department: Emergency       | Priority Level: 2
```

**Cancelling priority 1 (option 4):**

```
Enter priority level to cancel: 1
2 appointments cancelled.
```

**Searching for a priority that does not exist:**

```
Enter priority level to start search: 9
No appointments with priority level 9 found.
```

---

## Implementation Details

### `Node` class

Represents one appointment and one tree node.

| Member | Description |
|--------|-------------|
| `patientName`, `priorityLevel`, `department` | The appointment data |
| `left`, `right` | Pointers to the left and right children, initialised to `nullptr` |
| `Node(name, priority, dept)` | Constructor that fills in the data and clears both child pointers |

### `BST` class

**Private members**

| Member | Description |
|--------|-------------|
| `Node* root` | Root of the tree (`nullptr` when empty) |
| `int inOrder(Node*, bool print)` | Recursive in-order traversal. Returns the number of nodes visited and prints them when `print` is true |
| `int inOrderConditioned(Node*, int priority, int condition, bool print)` | In-order traversal that only counts and prints nodes matching the selected condition (more urgent, less urgent, or exact match) |

**Public methods**

| Method | Description |
|--------|-------------|
| `BST()` | Creates an empty tree |
| `scheduleAppointment(name, priority, dep, showMessage = true)` | Iteratively inserts a new appointment. `showMessage` is set to `false` while loading the file so the confirmation is not printed once per appointment |
| `displayAppointments()` | Prints the count and all appointments in order of urgency |
| `searchForAppointment(priority)` | Prints all appointments whose priority equals the given value |
| `cancelAppointment(priority)` | Deletes every appointment with the given priority and reports how many were removed |
| `displayMoreUrgentThan(priority)` | Prints appointments with priority less than or equal to the value |
| `displayLessUrgentThan(priority)` | Prints appointments with priority greater than or equal to the value |

### Output formatting

Appointment rows are aligned using `<iomanip>`: the patient name is left-aligned in a 20-character column and the department in a 15-character column, so the output lines up as a readable table.

---

## Time Complexity

Let `n` be the number of appointments and `h` the height of the tree. For a reasonably balanced tree, `h` is about `log n`. In the worst case (for example, appointments inserted in already sorted priority order) the tree degenerates into a chain and `h` becomes `n`.

| Operation | Average case | Worst case |
|-----------|--------------|------------|
| Schedule appointment | O(log n) | O(n) |
| Display all appointments | O(n) | O(n) |
| Search by priority | O(n) | O(n) |
| Display more / less urgent | O(n) | O(n) |
| Cancel by priority | O(k · h) | O(k · n) |

Here `k` is the number of appointments that share the cancelled priority level. Search and the urgency filters traverse the whole tree twice (once to count and once to print), which is still O(n).

---

## Input Validation

- Priority levels of `0` or below are rejected with the message `Priority level can't be less than 1.` for scheduling, searching, cancelling, and both urgency filters.
- Any menu choice outside 1 to 7 prints `Invalid Option. Try again.` and shows the menu again.
- Operations on an empty tree print a clear message (for example `No appointments found.`) rather than failing.
- A missing `appointments.txt` does not stop the program; it starts with no appointments.

---

## Known Limitations

- **The tree is not self-balancing.** Inserting many appointments in sorted priority order produces a skewed tree and slows insertion to O(n).
- **No destructor.** Nodes still in the tree when the program exits are not explicitly freed (the operating system reclaims the memory on exit).
- **Only the priority level is validated.** Entering non-numeric text where a number is expected is not handled.
- **Data is not saved.** Changes made during a session (new or cancelled appointments) are not written back to `appointments.txt`.
- **Priority is the only key.** Search and cancel work by priority level, not by patient name.

---

## Authors

- Seif Hussein Mohamed
- Osama Ehab Mahmoud

Developed as a university coursework project.
