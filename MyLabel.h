#ifndef MyLabel_h__
#define MyLabel_h__




#include <QLabel>
#include <QMouseEvent>
#include <QtCore>

class myLabel: public QLabel
{   
	Q_OBJECT
public:    
	myLabel()
	{
		this->setAlignment(Qt::AlignCenter);

		//Default Label Value
		this->setText("No Value");

		//set MouseTracking true to capture mouse event even its key is not pressed
	//	this->setMouseTracking(true);
	};

	myLabel( QWidget * parent = 0, Qt::WindowFlags f = 0 ):QLabel(parent,f)
	{
		this->setAlignment(Qt::AlignCenter);

		//Default Label Value
		this->setText("ddddddddddddddd Value");

		//set MouseTracking true to capture mouse event even its key is not pressed
		this->setMouseTracking(true);
	}

	~ myLabel(){};

	void mouseMoveEvent ( QMouseEvent * event )
	{
		//Show x and y coordinate values of mouse cursor here
	//	this->setText("X:"+QString::number(event->x())+"-- Y:"+QString::number(event->y()));
		emit emitMouseSignal(event);
	}; 
public:
signals:
	void emitMouseSignal(QMouseEvent * event);
};

#endif // MyLabel_h__