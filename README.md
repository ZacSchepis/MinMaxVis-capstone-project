# Project Outline: CHeckers and TicTacToe AI Games

## Project Goals
- Implement two games (TicTacToe and Checkers) with AI players using the MinMax algorithm and AlphaBeta pruning.
- Visualize algorithm decision-making.

### General
- [ ] Develop basic GUI displays to support both games.
- [ ] Set up a structure for unit testing and debugging.

### Checkers
#### Display
- [ ] Create a basic Checkers board display.


#### Game Mechanics
- [ ] Implement piece movement:
  - [ ] P1 moves one way (+).
  - [ ] P2 moves the other way (-).
- [ ] Add capture logic:
  - [ ] Allow capturing of n pieces for all n-valid capture moves 
  - [ ] Assign score for captured pieces

#### AI
- [ ] Implement MinMax algorithm
  - [ ] Visualize decision-making in tree format.
- [ ] Add AlphaBeta pruning
  - [ ] Highlight pruned branches in the visualization.
- [ ] Create an evaluation function for game states.


### TicTacToe
#### Display
- [ ] Create a basic TicTacToe board display.

#### Game Mechanics
- [ ] Implement piece placement rules.
- [ ] Add win/loss/tie evaluation logic.

#### AI
- [ ] Implement MinMax algorithm.
  - [ ] Visualize decision-making in tree format.
- [ ] Add AlphaBeta pruning.
  - [ ] Highlight pruned branches in the visualization.

## Testing
- [ ] Create test cases for both games:
  - [ ] Movement rules.
  - [ ] Capture rules.
  - [ ] AI decisions (correctness of MinMax and pruning).

## Stretch Goals (Optional)
- [ ] Add player vs. AI and AI vs. AI modes.
- [ ] Implement difficulty levels for the AI.
- [ ] Enhance visuals and animations for a polished UI.
