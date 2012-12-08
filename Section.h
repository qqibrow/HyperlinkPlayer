#ifndef Section_h__
#define Section_h__
#include <vector>
#include "../qtHelloWorld/HyperLink.h"
#include "../qtHelloWorld/Video.h"
#include <string>
#include <QRect>
class Section
{
public:	
	Section(std::string videoName, std::string metaName);
	Video getVideo()  { return video; };
	std::vector<QRect> getAllAreas( int frameNo );
	const HyperLink& getHyperLink(int i);
	void resume();
	~Section();
protected:
private:
	Section();	
	std::vector<HyperLink> hyperlinkVector;
	Video video;
	
	//	int currentFrame;

};




#endif // Section_h__
