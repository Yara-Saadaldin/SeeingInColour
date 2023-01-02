#pragma once
#include "ofMain.h"



namespace Constants {

	//App
		//Sets the app's width & height
			static const int APP_WINDOW_WIDTH		= 1066;
			static const int APP_WINDOW_HEIGHT		= 750;
		//Sets the app's framerate
			static const int APP_DESIRED_FRAMERATE	= 60; 

	//Video
		//Loads the video that will be displayed
			static const std::string VIDEO_PATH_VIDEO	= "VideoToTrack.mov";
		//Sets the video's width & height
			static const int VIDEO_WIDTH		        = 640;
			static const int VIDEO_HEIGHT		        = 362;
		//Sets the video's border size
			static const int VIDEO_BORDER_SIZE	        = 10;

	//Functions
		enum APP_MODE {
			//Lets you pick 2 colors that you would wish to detect
				FIND_COLOR
		};
}
