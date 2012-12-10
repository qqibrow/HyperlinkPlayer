#include "Section.h"
#include <fstream>
#include <assert.h>

using namespace std;

Section::Section( std::string videoName, std::string metaName ):video(videoName)
{
	// may be not a goo idea to read file in the construction method
	ifstream fin(metaName);
	assert(fin.peek() != EOF && "file doesn't exist ");
	HyperLink link;
	while(fin>>link)
	{
	/*	fin>>link;*/
		hyperlinkVector.push_back(link);
	}
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
	return hyperlinkVector[i];
}
