#include "player.h"
#include "../qtHelloWorld/Color.h"
#include <string>
#include <QPainter>
#include <QImage>
#include <assert.h>

using namespace std;

Player::Player(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	//init colors
	colors = ColorFactory::getListofColors();
	
	connect(ui.loadBn, SIGNAL(clicked()), this, SLOT(loadBnClicked()));
	connect(ui.backBn, SIGNAL(clicked()), this, SLOT(backBnClicked()));
	connect(ui.forwardBn, SIGNAL(clicked()), this, SLOT(forwardBnClicked()));
	connect(ui.playBn, SIGNAL(clicked()), this, SLOT(playBnClicked()));
	connect(ui.pauseBn, SIGNAL(clicked()), this, SLOT(pauseBnClicked()));
	connect(ui.stopBn, SIGNAL(clicked()), this, SLOT(stopBnClicked()));
	connect(ui.videoSlider, SIGNAL(sliderReleased()), this, SLOT(videoSliderClicked()));

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));

}

Player::~Player()
{
	delete timer;
}

void Player::loadBnClicked()
{
	string videoName = "prison.rgb";
	string metaName = "a.txt";

	SectionList.push_back(Section(videoName, metaName));
	cur = SectionList.end();
	--cur;
	//draw the frame
	//drawFrame(40);
	// do I need to deractivate  the loadBn?
}

void Player::backBnClicked()
{
	//should deactivate the backBn if there is no back way
	assert(cur != SectionList.begin());
	--cur;

	//set slider
	setSlider(cur->getVideo().getTotalFrames());

	//play the back video
	int frame = cur->getVideo().getCurrentFrame();
	drawFrame(frame);

}

void Player::forwardBnClicked()
{
	//should deactivate the forwardBn if there is no forward way
	++cur;
	assert(cur != SectionList.end());
	//set slider
	setSlider(cur->getVideo().getTotalFrames());

	//play the forward video
	int frame = cur->getVideo().getCurrentFrame();
	drawFrame(0);

}

void Player::playBnClicked()
{
	//start timer, play the video, each time set the video frame in the Section
	timer->start(freq);
}

void Player::pauseBnClicked()
{
	//pause the timer
	timer->stop();

}

void Player::stopBnClicked()
{
	// set the slider to zero
	timer->stop();
	//cur->resume();
	int firstFrame = 0;
	ui.videoSlider->setValue(firstFrame);
	drawFrame(firstFrame);
}

void Player::videoSliderClicked()
{
	int curframe = ui.videoSlider->value();
	drawFrame(curframe);
}

void Player::DrawHyperlinkImage( QImage& back, int frame )
{
	//areas's size should be the size of the hyperlinks
	vector<QRect> areas = cur->getAllAreas(frame);
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

void Player::setSlider( int total )
{
	ui.videoSlider->setRange(1, total);
}

void Player::drawFrame( int frame )
{
	Video& video = cur->getVideo();
	video.goToframeNo(frame);
	QImage background = video.getQimage();
	DrawHyperlinkImage(background, frame);
	ui.label->setPixmap(QPixmap::fromImage(background));

}

void Player::update()
{
	if (ui.videoSlider->value() == cur->getVideo().getTotalFrames())
	{
		this->stopBnClicked();
		return;
	}
	//update slider
	int value = ui.videoSlider->value() + 1;
	ui.videoSlider->setValue(value);	
	drawFrame(value);

}

