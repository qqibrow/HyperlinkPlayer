#ifndef PLAYER_H
#define PLAYER_H

#include <QtGui/QMainWindow>
#include "ui_player.h"
#include "Section.h"
#include <list>
#include <vector>
#include <QEvent>
#include <QThread>
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>

class Player : public QMainWindow
{
	Q_OBJECT

public:
	Player(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Player();
	signals:
		void askedtostop(void);
		void seekTo(qint64);	
public slots:
	void handleMouseMoveEventFromLabel(QMouseEvent * event);
	 void sourceChanged(const Phonon::MediaSource &source);
	 void stateChangedd(Phonon::State,Phonon::State);
protected slots:
	void loadBnClicked();

	void loadHyperlinkVideo( QString videoName );

	void backBnClicked();

	void reloadAudioSource();

	void forwardBnClicked();
	void playBnClicked();
	void pauseBnClicked();
	void stopBnClicked();
	void DrawHyperlinkImage(QImage& back, int frame);
	void drawRectOnImage( QImage& backImage, QRect& rec, QColor& color );
	void drawFrame( int frame );
	void mousePressEvent ( QMouseEvent * event );


private slots:
	void tick(qint64);

private:
	void showQmessageBox(const QString info);
	void loadAudio(const QString fileName);
	void setAudioSlider();

	bool isEnd();
	Ui::PlayerClass ui;
	std::vector<Section> SectionList;
	std::vector<Section>::iterator cur;
	std::vector<QColor> colors;
	std::vector<QRect> areas;
	int freq;
	QCursor cursor;
	qint64 toFrame;
	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;
};


#endif // PLAYER_H
