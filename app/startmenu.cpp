//joehannsenn 2025.3.13

//probably redundant (included in header):
#include <QButtonGroup>
#include <QPushButton>
#include <QRadioButton>
#include "mainwindow.h"

//#include <QtObject>

//probably necessary:
#include "tictactoe.h"
#include "checkers.h"
#include "startmenu.h"


// IMPLEMENTATION
StartMenu::StartMenu(MainWindow* pw = nullptr) {
	MainWindow parent_window = pw;
	create_layout();

	//Button-Function Bindings
	connect(&checkers_launch_button, &QPushButton::clicked, this, &StartMenu::launch_checkers);
	connect(&ttt_launch_button, &QPushButton::clicked, this, &StartMenu::launch_ttt);
}

void StartMenu::create_layout() {

	//fill sublayouts
	fill_mode_layout();
	fill_launch_layout();

	//add sublayouts to main layout
	main_vert_layout.addLayout(&mode_buttons_horiz_layout);
	main_vert_layout.addLayout(&launch_buttons_horiz_layout);

	//add main layout to main widget
	main_widget.setLayout(&main_vert_layout);
}

void StartMenu::fill_mode_layout() {
	//Entitle Mode Buttons
	pvp_button.setText("Player vs Player");
	pvc_button.setText("Player vs Computer");

	//Place in layout
	mode_buttons_horiz_layout.addWidget(&pvp_button);
	mode_buttons_horiz_layout.addWidget(&pvc_button);

	//add mutual exclusivity
	mode_buttons.addButton(&pvp_button);
	mode_buttons.addButton(&pvc_button);
	mode_buttons.setExclusive(true);

	//set default selection (PvP)
	pvp_button.setChecked(true); 
}

void StartMenu::fill_launch_layout() {
	//Entitle Launch Buttons
	checkers_launch_button.setText("Launch Checkers");
	ttt_launch_button.setText("Launch Tic Tac Toe");

	//Place in layout
	launch_buttons_horiz_layout.addWidget(&checkers_launch_button);
	launch_buttons_horiz_layout.addWidget(&ttt_launch_button);
}

void StartMenu::launch_checkers() {
	Checkers checkers_game;
	bool is_pvp = pvp_button.isChecked();
	checkers_game.populate_board(is_pvp);
	parent_window.setWindowTitle("MinMax Visualization - Checkers");
	parent_window.setCentralWidget(&checkers_game);
	parent_window.show();
}

void StartMenu::launch_ttt() {
	TicTacToe ttt_game;
	parent_window.setWindowTitle("MinMax Visualization - Tic Tac Toe");
	parent_window.setCentralWidget(&ttt_game);
	parent_window.show();
}

void StartMenu::launch_startmenu() {
	parent_window.setWindowTitle("MinMax Visualization - Start Menu");
	parent_window.setCentralWidget(&main_widget);
	parent_window.show();
}