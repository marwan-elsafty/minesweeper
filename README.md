# Minesweeper

Minesweeper is a single-player puzzle video game. It has its origins in the earliest mainframe games of the 1960s and 1970s.The objective of the game is to clear a rectangular board con-taining hidden mines without detonating any of them, with help from clues about the number
of neighboring mines in each eld. The game originates from the 1960s, and has been written for many computing platforms in use today. In this project you will make your own version of the game. You can read more about Minesweeper on wikipedia.

## Overview

The goal of Minesweeper is to mark locations of mines with flags and to uncover all other locations that don't contais mines. Score is calculated based on time, number of operations performed and size of map.
The map is a two grid where each location of the grid has either a:
*  Mine
*  Number of adjacent cells that contains mines.
*  If no adjacent cell has mine, then this cell is empty

The first cell the player opens shouldn't contain a mine. Also, player is free to start in any cell.

When an empty cell (cell with no adjacent mines) is opened, then all empty cells that are reacheable from this cell are opened
