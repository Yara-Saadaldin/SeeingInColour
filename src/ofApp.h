#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxImGui.h"
#include "Constants.h"



class ofApp : public ofBaseApp{

	public:

	//---------------------------------------------------------------------Variables---------------------------------------------------------------------
		//Video
			ofVideoPlayer	      m_videoPlayer;

		//Countour Finder
			ofxCvContourFinder    m_contourFinder;	  ///Object #1
			ofxCvContourFinder    m_contourFinder2;	  ///Object #2

		//Images (for processing) -- OF images converted to CV images
			ofxCvColorImage       m_colorImage;       ///CV image that holds raw current video image (captures every frame of the image and converts it)
			ofxCvGrayscaleImage   m_grayscaleImage;   ///Grayscale version of the colored image ^ 
			ofxCvGrayscaleImage   m_grayscaleBg;      ///Captured background of our video feed
			ofxCvGrayscaleImage   m_grayscaleDiff;    ///Holds the difference information (grayscaleDiff= grayscaleImage - grayscaleBg / whatever is left)

		//Movement
			ofVec2f               m_movementTarget;	  ///Object #1
			ofVec2f               m_movementTarget2;  ///Object #2

		//Event Images 
			ofImage               m_DetectedImage;    ///Detected
			ofImage               m_UndetectedImage;  ///Undetected				
			ofImage               m_NearEachOther;    ///Near Each Other

		//GUI
			ofxImGui::Gui         m_gui;
			//Adjust
				//Threshold: Allows us to adjust how much color or highlight and contour we want 
						int     m_threshold;	            ///Object #1
						int	    m_threshold2;               ///Object #2
				//Contours Considered: Allows us to specify how many contours we want to find/ are looking for
						int     m_numContoursConsidered;	///Object #1
						int	    m_numContoursConsidered2;   ///Object #2
				//Min & Max area: Filters that help us reduce the amount of information to make things go faster + to have a clearer image
					//High pass filter that allows us to ignore contours found that are too small
						float   m_minArea;                  ///Object #1
						float   m_minArea2;                 ///Object #2
					//Low passs filter that allows us to ignore contours found that are too big
						float   m_maxArea;	                ///Object #1
						float   m_maxArea2;                 ///Object #2
				//Smooth Factor
						float   m_smoothFactor;	            ///Object #1
						float   m_smoothFactor2;	        ///Object #2
			//Color Tracking
				Constants::APP_MODE        m_appMode;
				std::vector<std::string>   m_appModes;
				float                      m_trackedColor[4]=  { 0.0f, 0.0f, 0.0f, 0.0f };	  ///Object #1
				float                      m_trackedColor2[4]= { 0.0f, 0.0f, 0.0f, 0.0f };	  ///Object #2


		//---------------------------------------------------------------------Functions---------------------------------------------------------------------
			void  setup();
			void  update();
			void  draw();
			void  keyPressed(int key);
			void  mousePressed(int x, int y, int button);	
			void  processColor();
			void  processColor2();
			float lerp(float start, float end, float percent);
};