//joehannsenn 2025.3.13

//#include <QtObject>

//probably necessary:
#include "tictactoe.h"
#include "checkers.h"
#include "startmenu.h"
#include "TreeGraphics.h"

// IMPLEMENTATION
StartMenu::StartMenu(MainWindow* pw) : QWidget(pw), parent_window(pw) {
	main_vert_layout = new QVBoxLayout();
    mode_buttons_horiz_layout = new QHBoxLayout();
    launch_buttons_horiz_layout = new QHBoxLayout();
    mode_buttons = new QButtonGroup(this);
    pvp_button = new QRadioButton();
    pvc_button = new QRadioButton();
    checkers_launch_button = new QPushButton();
    ttt_launch_button = new QPushButton();
    create_layout();
	connect(checkers_launch_button, &QPushButton::clicked, this, &StartMenu::launch_checkers);
	connect(ttt_launch_button, &QPushButton::clicked, this, &StartMenu::launch_ttt);
}

void StartMenu::create_layout() {

	//fill sublayouts
	fill_mode_layout();
	fill_launch_layout();

	//add sublayouts to main layout
	main_vert_layout->addLayout(mode_buttons_horiz_layout);
	main_vert_layout->addLayout(launch_buttons_horiz_layout);

    //add main layout to main widget
	this->setLayout(main_vert_layout);
    this->show();
}

void StartMenu::fill_mode_layout() {
	//Entitle Mode Buttons
	pvp_button->setText("Player vs Player");
	pvc_button->setText("Player vs Computer");
	//Place in layout
	mode_buttons_horiz_layout->addWidget(pvp_button);
	mode_buttons_horiz_layout->addWidget(pvc_button);
	//add mutual exclusivity
	mode_buttons->addButton(pvp_button);
	mode_buttons->addButton(pvc_button);
	mode_buttons->setExclusive(true);

	//set default selection (PvP)
	pvp_button->setChecked(true);
}

void StartMenu::fill_launch_layout() {
	//Entitle Launch Buttons
	checkers_launch_button->setText("Launch Checkers");
    checkers_launch_button->setObjectName("menuButton");
	ttt_launch_button->setText("Launch Tic Tac Toe");
    ttt_launch_button->setObjectName("menuButton");
	//Place in layout
	launch_buttons_horiz_layout->addWidget(checkers_launch_button);
	launch_buttons_horiz_layout->addWidget(ttt_launch_button);
}

void StartMenu::launch_checkers() {
//    if(gameRef) {
//        this->
//    }
	auto checkers_game = new Checkers(this);

//	bool is_pvp = pvp_button.isChecked();
	checkers_game->populate_board(true);
	parent_window->setWindowTitle("MinMax Visualization - Checkers");
	parent_window->setCentralWidget(checkers_game);
    checkers_game->show();
}

void StartMenu::launch_ttt() {
	auto ttt_game = new TicTacToe(this);
    ttt_game->setStyleSheet(QString("background-color: #CCB5B6;"));
    parent_window->setWindowTitle("MinMax Visualization - Tic Tac Toe");
	parent_window->setCentralWidget(ttt_game);
    TreeGraphics* treeWidget = new TreeGraphics(nullptr,ttt_game);
    connect(ttt_game, &TicTacToe::update_tree_Visualization, treeWidget, &TreeGraphics::update_Tree);
    connect(ttt_game, &TicTacToe::move_Executed, treeWidget, &TreeGraphics::update_Tree);

    treeWidget->show();

    parent_window->setCentralWidget(ttt_game);
    ttt_game->show();
}

void StartMenu::launch_startmenu() {
	parent_window->setWindowTitle("MinMax Visualization - Start Menu");
	parent_window->setCentralWidget(this);
	parent_window->show();
}
