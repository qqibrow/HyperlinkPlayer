#include "Section.h"
#include <fstream>
#include <assert.h>

using namespace std;

Section::Section( std::string videoName, std::string metaName, std::string audioName):video(videoName),source(audioName.c_str())
{
	// may be not a goo idea to read file in the construction method
	ifstream fin(metaName);
	if(fin.peek() != EOF)
	{
		HyperLink link;
		while(fin>>link)
		{			
			hyperlinkVector.push_back(link);

		}
	}
	//assert(fin.peek() != EOF && "file doesn't exist ");

}




Section::Section()
{

}

Section::~Section()
{

}

vector<QRect> Section::getAllAreas( int frameNo )
{

	vector<QRect> rectVector;
	if(hyperlinkVector.empty())
		return rectVector;

	for(int i = 0; i < hyperlinkVector.size(); i++)
	{
		//check every hyperlink
		rectVector.push_back(hyperlinkVector[i].interpolateFrame(frameNo));
	}
	return rectVector;
}

void Section::resume()
{
	// the video go back to the first frame
	this->video.goToframeNo(0);
	//throw std::exception("The method or operation is not implemented.");
}

const HyperLink& Section::getHyperLink( int i )
{
	assert( !hyperlinkVector.empty() && i >= 0 && i < hyperlinkVector.size());
	return hyperlinkVector[i];
}

Phonon::MediaSource& Section::getAudioSource()
{
	return source;
}
