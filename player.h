#ifndef PLAYER_H
#define PLAYER_H

#include <QtGui/QMainWindow>
#include "ui_player.h"
#include "Section.h"
#include <list>
#include <vector>
class Player : public QMainWindow
{
	Q_OBJECT

public:
	Player(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Player();
protected slots:
	void loadBnClicked();
	void backBnClicked();
	void forwardBnClicked();
	void playBnClicked();
	void pauseBnClicked();
	void stopBnClicked();
	void videoSliderClicked();
	void DrawHyperlinkImage(QImage& back, int frame);
	void drawRectOnImage( QImage& backImage, QRect& rec, QColor& color );
	void setSlider( int total );
	void drawFrame( int frame );
	void update();
private:
	Ui::PlayerClass ui;
	std::list<Section> SectionList;
	std::list<Section>::iterator cur;
	std::vector<QColor> colors;
	QTimer* timer;
	static const int freq = 1000/20;
};


#endif // PLAYER_H
