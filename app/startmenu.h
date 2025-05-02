//joehannsenn 2025.3.13

#ifndef STARTMENU_H
#define STARTMENU_H

#include <QLayout>
#include <QWidget>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QStackedWidget>
#include "QDockWidget"
#include "mainwindow.h"

//INTERFACE

/// <summary>
/// A Widget which displays a start menu with game options:
///		Gamemode: PvP or PvComputer
///		Game: Checkers or Tic-Tac-Toe
/// </summary>
class StartMenu : public QWidget
{
	Q_OBJECT
private:
	MainWindow* parent_window;
	QVBoxLayout* main_vert_layout;
	QHBoxLayout* mode_buttons_horiz_layout;
	QButtonGroup* mode_buttons; //provides mutual exclusivity
	QRadioButton* pvp_button;
	QRadioButton* pvc_button;
	QHBoxLayout* launch_buttons_horiz_layout;
	QPushButton* checkers_launch_button;
	QPushButton* ttt_launch_button;
	void launch_checkers();

	void launch_ttt();

	void create_layout();

	void fill_mode_layout();

	void fill_launch_layout();

public:
	//constructor
	explicit StartMenu(MainWindow* pw= nullptr);

	void launch_startmenu();
};
#endif // STARTMENU_H