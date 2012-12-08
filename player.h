#ifndef PLAYER_H
#define PLAYER_H

#include <QtGui/QMainWindow>
#include "ui_player.h"
#include "Section.h"
#include <list>
#include <vector>
#include "../src/gui/kernel/qevent.h"
class Player : public QMainWindow
{
	Q_OBJECT

public:
	Player(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Player();

	class myLabel: public QLabel
	{   

	public:    
		myLabel()
		{
			this->setAlignment(Qt::AlignCenter);

			//Default Label Value
			this->setText("No Value");

			//set MouseTracking true to capture mouse event even its key is not pressed
			this->setMouseTracking(true);
		};
		~ myLabel(){};

		void mouseMoveEvent ( QMouseEvent * event )
		{
			//Show x and y coordinate values of mouse cursor here
			this->setText("X:"+QString::number(event->x())+"-- Y:"+QString::number(event->y()));
		}; 

	};

protected slots:
	void loadBnClicked();

	void loadHyperlinkVideo( QString &videoName );

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
	void mousePressEvent ( QMouseEvent * event );
private:
	Ui::PlayerClass ui;
	std::list<Section> SectionList;
	std::list<Section>::iterator cur;
	std::vector<QColor> colors;
	QTimer* timer;
	std::vector<QRect> areas;
	static const int freq = 1000/20;
};


#endif // PLAYER_H
