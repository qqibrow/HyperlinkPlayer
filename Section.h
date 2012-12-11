#ifndef Section_h__
#define Section_h__
#include <vector>
#include "../qtHelloWorld/HyperLink.h"
#include "../qtHelloWorld/Video.h"
#include <string>
#include <QRect>
#include <phonon/MediaSource>

class Section
{
public:	
	Section(std::string videoName, std::string metaName, std::string audioName);
	Video& getVideo()  { return video; };
	std::vector<QRect> getAllAreas( int frameNo );
	const HyperLink& getHyperLink(int i);
	void resume();
	Phonon::MediaSource& getAudioSource();
	~Section();
private:
	Section();	
	std::vector<HyperLink> hyperlinkVector;
	Video video;
	Phonon::MediaSource source;	

	//	int currentFrame;

};




#endif // Section_h__
