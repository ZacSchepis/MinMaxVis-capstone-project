# Project Outline: Checkers and TicTacToe AI Games

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



# Using Custom Icons
I have (helpfully) provided some basic icons to be used and managed. However, if we decide to add a new/better looking icon for a piece, it can be done with the following:
1. Head over to https://www.piskelapp.com/p/create/sprite
2. Resize as needed with the Resize menu option on the side to a 32x32px grid
3. Create a new Sprite by using the drawing tools provided
4. When finished, hit `Export>PNG>Spritesheet file export: Download`
5. Move the file (`<FILE_NAME.png>`) into the folder in `app/res/<FILE_NAME.png>` and update the `resources.qrc` file with `<file>res/<FILE_NAME.png></file>`
6. Update the associated icon(s) in the code with `map_piece`. Usage can be seen in `app/sample_board.cpp`.
7. Now, whenever you call `DerivedBoard#place()` (where DerivedBoard implements/extends the Board base class) with that PieceType, it should use that new image when the board is drawn.