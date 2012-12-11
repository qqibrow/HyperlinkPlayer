#include "player.h"
#include "../qtHelloWorld/Color.h"
#include <string>
#include <QPainter>
#include <QImage>
#include <assert.h>
#include "../src/gui/dialogs/qfiledialog.h"
#include <QMouseEvent>
#include "qmessagebox.h"
using namespace std;

Player::Player(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	freq = 24;
	// disable the backbutton and forwardbutton
	this->ui.backBn->setDisabled(true);
	this->ui.forwardBn->setDisabled(true);
	this->ui.pauseBn->setDisabled(true);
	ui.playBn->setDisabled(true);
	ui.stopBn->setDisabled(true);

	//init media object
	mediaObject = new Phonon::MediaObject(this);
	audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	mediaObject->setTickInterval(1000/freq);

	//
	connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
	connect(this, SIGNAL(seekTo(qint64)), mediaObject, SLOT(seek(qint64)));

	//init colors
	colors = ColorFactory::getListofColors();

	this->connect(this->ui.label, SIGNAL(emitMouseSignal(QMouseEvent*)), SLOT(handleMouseMoveEventFromLabel(QMouseEvent *)));


	connect(ui.loadBn, SIGNAL(clicked()), this, SLOT(loadBnClicked()));
	connect(ui.backBn, SIGNAL(clicked()), this, SLOT(backBnClicked()));
	connect(ui.forwardBn, SIGNAL(clicked()), this, SLOT(forwardBnClicked()));
	connect(ui.playBn, SIGNAL(clicked()), mediaObject, SLOT(play()));
	connect(ui.pauseBn, SIGNAL(clicked()), mediaObject, SLOT(pause()));
	connect(ui.stopBn, SIGNAL(clicked()), mediaObject, SLOT(stop()));
	connect(ui.seekBn, SIGNAL(clicked()), this, SLOT(playBnClicked()));
	connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
		this, SLOT(sourceChanged(Phonon::MediaSource)));

	connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
		this, SLOT(stateChangedd(Phonon::State,Phonon::State)));

//	timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(update()));

	//setup media stuff
	ui.seekSlider->setMediaObject(mediaObject);
	ui.volumeSlider->setAudioOutput(audioOutput);
	ui.volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	Phonon::createPath(mediaObject, audioOutput);
	toFrame = -1;
	//create a consumer thread, which is a 
}

Player::~Player()
{
	if (mediaObject)
	{
		delete mediaObject;
		mediaObject = NULL;
	}

	if(audioOutput)
	{
		delete audioOutput;
		audioOutput = NULL;
	}
}

void Player::loadBnClicked()
{
	QString videoName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"/",tr("Images (*.rgb)"));

	if(videoName != "")
	{
		loadHyperlinkVideo(videoName);
		showQmessageBox("load successfully");
		ui.pauseBn->setDisabled(false);
		ui.playBn->setDisabled(false);
		ui.stopBn->setDisabled(false);

	}

}


void Player::backBnClicked()
{
//	emit askedtostop();
//	mediaObject->stop();
	//should deactivate the backBn if there is no back way
	assert(cur != SectionList.begin());
	--cur;

	if(cur == SectionList.begin())
		ui.backBn->setDisabled(true);

	//play the back video
	reloadAudioSource();

	toFrame = 1000*cur->getVideo().getCurrentFrame()/freq;
	ui.forwardBn->setDisabled(false);

	//this->pauseBnClicked();
}

void Player::forwardBnClicked()
{
	//should deactivate the forwardBn if there is no forward way
	++cur;
	//assert(cur != SectionList.end());
	
	if(isEnd())
		this->ui.forwardBn->setDisabled(true);

	
	//play the forward video
	reloadAudioSource();
	ui.backBn->setDisabled(false);
	toFrame = 1000*cur->getVideo().getCurrentFrame()/freq;
}

void Player::playBnClicked()
{
	emit seekTo(7000);
}

void Player::pauseBnClicked()
{
	mediaObject->pause();
}

void Player::stopBnClicked()
{
	mediaObject->stop();
}


void Player::DrawHyperlinkImage( QImage& back, int frame )
{
	//areas' size should be the size of the hyperlinks
	areas.clear();
	areas = cur->getAllAreas(frame);
	for(int i = 0; i < areas.size(); i++)
	{
		if(!areas[i].isNull())
		{
			drawRectOnImage(back, areas[i], colors[i]);
		}
	}
}

void Player::drawRectOnImage( QImage& image, QRect& rec, QColor& color )
{
	QPainter* painter = new QPainter(&image); 
	painter->setPen(color);
	painter->drawRect(rec);
	delete painter;
}


void Player::drawFrame( int frame )
{
	Video& video = cur->getVideo();
	video.goToframeNo(frame);
	QImage background = video.getQimage();
	DrawHyperlinkImage(background, frame);
	ui.label->setPixmap(QPixmap::fromImage(background));

}

void Player::mousePressEvent( QMouseEvent * event )
{
	QPoint p = event->pos() - ui.label->pos();
// 	vector<QRect> areas = cur->getAllAreas(frame);
	for(int i = 0; i < areas.size(); i++)
	{
		if(!areas[i].isNull() && areas[i].contains(p))
		{
			mediaObject->stop();
				//clear the sections behind
			while(cur + 1 != SectionList.end())
			{
				SectionList.pop_back();
			}

// 			if(cur->getVideo().getVideoName() != cur->getHyperLink(i).getSecondaryVideoName())
// 			{
				loadHyperlinkVideo(QString(cur->getHyperLink(i).getSecondaryVideoName().c_str()));
				int secondaryVideoStartFrame = cur->getHyperLink(i).getSecondaryVideoStartFrame();
				toFrame = secondaryVideoStartFrame*1000/freq;
			//	mediaObject->play();
				//emit seekTo(toFrame);
				//mediaObject->seek(7000);
				//mediaObject->pause();
				ui.backBn->setDisabled(false);
				ui.forwardBn->setDisabled(true);
			//	playBnClicked();
// 			}
// 			else
// 			{
// 				forwardBnClicked();
// 			}
			return;
		}
	}
	
}


// load hyperlink video to tail
void Player::loadHyperlinkVideo( QString videoName )
{
	std::string utf8_text = videoName.toUtf8().constData();
	string metaName = utf8_text + ".meta";

	int index = videoName.indexOf(".");
	QString audioName = videoName.left(index) + ".wav";

	SectionList.push_back(Section(utf8_text, metaName, audioName.toUtf8().constData()));

	cur = SectionList.end() - 1;
	
	mediaObject->stop();
	mediaObject->clearQueue();

	mediaObject->setCurrentSource(cur->getAudioSource());
}

void Player::showQmessageBox( const QString info )
{
	QMessageBox::information(NULL, "Title",info, QMessageBox::Yes, QMessageBox::Yes);

}


void Player::handleMouseMoveEventFromLabel( QMouseEvent * event )
{
	QPoint start = ui.label->pos();
	QPoint p = event->pos() - ui.label->pos();
	for( int i = 0; i < areas.size(); i++)
	{
		if(!areas[i].isNull() && areas[i].contains(p))
		{
			cursor.setShape(Qt::PointingHandCursor);
			this->setCursor(cursor);
			return;
		}
	}
	cursor.setShape(Qt::ArrowCursor);
	this->setCursor(cursor);
}

bool Player::isEnd()
{
	std::vector<Section>::iterator it = cur;
	return ++it == SectionList.end();
}


void Player::tick( qint64 time)
{
// 	QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);
// 	//ui.label->setText(displayTime.toString("mm:ss"));
// 	//update();
// 	static int i = 0;
// 	ui.label->setText(displayTime.toString("mm:ss") + QString(" %1,%2").arg(time).arg(i));
// 	i++;
	int frame = (time*24)/1000;
	drawFrame(frame);
}

void Player::reloadAudioSource()
{
	int frame = cur->getVideo().getCurrentFrame();

	mediaObject->stop();
	mediaObject->clearQueue();
	mediaObject->setCurrentSource(cur->getAudioSource());

	//mediaObject->play();
	//qint64 time = 1000*frame/freq;
	//mediaObject->seek(time);
	//mediaObject->pause();
}

void Player::sourceChanged( const Phonon::MediaSource &source )
{
	drawFrame(cur->getVideo().getCurrentFrame());
	mediaObject->play();
}

void Player::stateChangedd( Phonon::State newState,Phonon::State )
{
	if ((newState == Phonon::PlayingState || newState == Phonon::PausedState) && toFrame > -1)
	{
		mediaObject->seek(toFrame);
		toFrame = -1;
	}

}


