#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){

	//App setup
		//Width & height
			ofSetWindowShape(Constants::APP_WINDOW_WIDTH, Constants::APP_WINDOW_HEIGHT);
		//Framerate
			ofSetFrameRate(Constants::APP_DESIRED_FRAMERATE);
		//Background color
			ofSetBackgroundColor(26, 27, 30);    ///Dark grey
		//Title for the window
			ofSetWindowTitle("Assignment3: Seeing in Color");

	//Video setup
		//Loads the video that will be displayed
			m_videoPlayer.load(Constants::VIDEO_PATH_VIDEO);
		//Makes the video play in a loop
			m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
			m_videoPlayer.play();
		//Initializes the videos that will be displayed + Allocates memory for CV images (So that when you try to set pixels it doesn't clash)
			//Video on top
				m_grayscaleDiff.allocate(Constants::VIDEO_WIDTH, Constants::VIDEO_HEIGHT); 
			//Video on bottom
				m_colorImage.allocate(Constants::VIDEO_WIDTH, Constants::VIDEO_HEIGHT); 

	//Loads in images that will be displayed for the Events
		//Detected: Event when objects are detected
			m_DetectedImage.load("Detected.png");
		//Undetected: Event when objects lose detection
			m_UndetectedImage.load("Undetected.png");
		//Near each other: Event when objects are near each other
			m_NearEachOther.load("NearEachOther.png");

	//Initialize Variables 
		//Threshold
			m_threshold = 128;
		//Contours
			m_numContoursConsidered = 2;         ///Colored object #1
			m_numContoursConsidered2 = 2;        ///Colored object #2
		//Area
			m_minArea = 10.0f;                   ///Colored object #1
			m_minArea2 = 10.0f;                  ///Colored object #2
			m_maxArea = 15000.0f;                ///Colored object #1
			m_maxArea2 = 15000.0f;               ///Colored object #2
		//Smooth factor
			m_smoothFactor = 1.0f;               ///Colored object #1
			m_smoothFactor2 = 1.0f;              ///Colored object #2
		//Movement
			m_movementTarget.set(0.0f, 0.0f);    ///Colored object #1
			m_movementTarget2.set(0.0f, 0.0f);   ///Colored object #2
			
	//GUI setup
		m_gui.setup();
}



//--------------------------------------------------------------
void ofApp::update(){

	//Avoids unnecessary performance for when the video runs at a different frequency than the app
		m_videoPlayer.update();
		if (m_videoPlayer.isFrameNew()) {
			//Specifies which video to look at
				m_colorImage.setFromPixels(m_videoPlayer.getPixels());
			//Finite state machine -- calls the Find Color function
				switch (m_appMode) {
					case Constants::APP_MODE::FIND_COLOR: {
							processColor();   ///Colored object #1
							processColor2();  ///Colored object #2
					}
					break;
				}
		}
}



//--------------------------------------------------------------
void ofApp::draw(){
	
	//Videos
		//Sets background color of the videos 
			ofSetColor(255, 255, 255);	///White
		//Sets the spacing between the videos
			static int imageBorderSize = 10; 
		//Draws Top video
			m_grayscaleDiff.draw(imageBorderSize, imageBorderSize);	                                          ///Width & Height
		//Draws Bottom video
			m_colorImage.draw(Constants::VIDEO_WIDTH + imageBorderSize - 640.0f, imageBorderSize + 370.0f);   ///Width & Height
				
	//Contours
		//.....................................................................Colored Object #1.....................................................................
			//Colored Object#1 Variables
				static ofVec2f contourCenter;      ///Contour center variable 
				static float contourArea = 0.0f;   ///Area of the contour variable
			//Searches for contours from the Top video
				ofPushMatrix(); {
					//Identifies the position of the video to look at
						ofTranslate(Constants::VIDEO_BORDER_SIZE, Constants::VIDEO_BORDER_SIZE);   ///Position (Width), Position (Height)
					//Loops through the contours 
						for (int i = 0; i < m_contourFinder.nBlobs; i++) { 
							//Finds the Center & Area of the contour
								contourCenter.set(m_contourFinder.blobs[i].boundingRect.getCenter().x, m_contourFinder.blobs[i].boundingRect.getCenter().y);   ///Center
								contourArea = m_contourFinder.blobs[i].area;                                                                                   ///Area
							//Draws a contour around the object along with its movement 
								if (i == 0) {
									//Movement
										m_movementTarget.x = lerp(m_movementTarget.x, contourCenter.x, m_smoothFactor);   ///Movement in the x direction
										m_movementTarget.y = lerp(m_movementTarget.y, contourCenter.y, m_smoothFactor);   ///Movement in the y direction
									//Draws the contour of the object
										ofSetColor(163, 38, 65);                                ///Dark pink
										m_contourFinder.blobs[i].draw(0.0f, 0.0f);              ///Position (Width), Position (Height)
									//Draws the center of the contour (small circle in the middle)
										ofSetColor(242, 56, 96);                                ///Light pink
										ofDrawCircle(contourCenter.x, contourCenter.y, 5.0f);   ///Position (Width), Position (Height), size
									//Displays a text that tells the position of the object
										ofDrawBitmapString("Position: \nx= " + ofToString(m_movementTarget.x) + "\ny= " + ofToString(m_movementTarget.y), m_contourFinder.blobs[i].boundingRect.getMaxX() + Constants::VIDEO_BORDER_SIZE, contourCenter.y);
								}
							//Displays a text event that says that the object has been detected
								ofSetColor(163, 38, 65);                                       ///Dark pink
								ofDrawBitmapString("Object#1", 770.0f, 420.0f);                ///Name, Position (Width), Position (Height)
						}
				} ofPopMatrix();
			//Searches for contours from the Bottom video
				ofPushMatrix(); {
					//Identifies the position of the video to look at
						ofTranslate(Constants::VIDEO_WIDTH + imageBorderSize - 640.0f, imageBorderSize + 370.0f);   ///Position (Width), Position (Height)
					//Loops through the contours 
						for (int i = 0; i < m_contourFinder.nBlobs; i++) { 
							//Finds the Center & Area of the contour
								contourCenter.set(m_contourFinder.blobs[i].boundingRect.getCenter().x, m_contourFinder.blobs[i].boundingRect.getCenter().y);   ///Center
								contourArea = m_contourFinder.blobs[i].area;                                                                                   ///Area
							//Draws a contour around the object along with its movement 
								if (i == 0) {
									//Movement
										m_movementTarget.x = lerp(m_movementTarget.x, contourCenter.x, m_smoothFactor);   ///Movement in the x direction
										m_movementTarget.y = lerp(m_movementTarget.y, contourCenter.y, m_smoothFactor);   ///Movement in the y direction
									//Draws the contour of the object
										ofSetColor(163, 38, 65);                     ///Dark pink
										m_contourFinder.blobs[i].draw(0.0f, 0.0f);   ///Position (Width), Position (Height)
								}
						}
				} ofPopMatrix();
		//.....................................................................Colored Object #2.....................................................................
			//Colored Object#1 Variables
				static ofVec2f contourCenter2;      ///Contour center variable
				static float contourArea2 = 0.0f;   ///Area of the contour variable
			//Searches for contours from the Top video
				ofPushMatrix(); {
					//Identifies the position of the video to look at
						ofTranslate(Constants::VIDEO_BORDER_SIZE, Constants::VIDEO_BORDER_SIZE);   ///Position (Width), Position (Height)
					//Loops through the contours 
						for (int i = 0; i < m_contourFinder2.nBlobs; i++) { 
							//Finds the Center & Area of the contour
								contourCenter2.set(m_contourFinder2.blobs[i].boundingRect.getCenter().x, m_contourFinder2.blobs[i].boundingRect.getCenter().y);   ///Center
								contourArea2 = m_contourFinder2.blobs[i].area;                                                                                    ///Area
							//Draws a contour around the object along with its movement 
								if (i == 0) {
									//Movement
										m_movementTarget2.x = lerp(m_movementTarget2.x, contourCenter2.x, m_smoothFactor2);   ///Movement in the x direction
										m_movementTarget2.y = lerp(m_movementTarget2.y, contourCenter2.y, m_smoothFactor2);   ///Movement in the y direction
									//Draws the contour of the object
										ofSetColor(163, 38, 65);                                  ///Dark pink
										m_contourFinder2.blobs[i].draw(0.0f, 0.0f);               ///Position (Width), Position (Height)
									//Draws the center of the contour (small circle)
										ofSetColor(242, 56, 96);                                  ///Light pink
										ofDrawCircle(contourCenter2.x, contourCenter2.y, 5.0f);   ///Position (Width), Position (Height)
									//Displays position of the object
										ofDrawBitmapString("Position: \nx= " + ofToString(m_movementTarget2.x) + "\ny= " + ofToString(m_movementTarget2.y), m_contourFinder2.blobs[i].boundingRect.getMaxX() + Constants::VIDEO_BORDER_SIZE, contourCenter2.y);
								}
							//Displays a text event that says that the object has been detected
								ofSetColor(163, 38, 65);                                           ///Dark pink
								ofDrawBitmapString("Object#2", 770.0f, 435.0f);                    ///Name, Position (Width), Position (Height)
						}
				} ofPopMatrix();
			//Searches for contours from the Bottom video
				ofPushMatrix(); {
					//Identifies the position of the video to look at
						ofTranslate(Constants::VIDEO_WIDTH + imageBorderSize - 640.0f, imageBorderSize + 370.0f);
					//Loops through the contours
						for (int i = 0; i < m_contourFinder2.nBlobs; i++) { 
							//Finds the Center & Area of the contour
								contourCenter2.set(m_contourFinder2.blobs[i].boundingRect.getCenter().x, m_contourFinder2.blobs[i].boundingRect.getCenter().y);   ///Center
								contourArea2 = m_contourFinder2.blobs[i].area;                                                                                    ///Area
							//Draws a contour around the object along with its movement 
								if (i == 0) {
									//Movement
										m_movementTarget2.x = lerp(m_movementTarget2.x, contourCenter2.x, m_smoothFactor2);   ///Movement in the x direction
										m_movementTarget2.y = lerp(m_movementTarget2.y, contourCenter2.y, m_smoothFactor2);   ///Movement in the y direction
									//Draws the contour of the object
										ofSetColor(163, 38, 65);                      ///Dark pink
										m_contourFinder2.blobs[i].draw(0.0f, 0.0f);   ///Position (Width), Position (Height)
								}
						}
				} ofPopMatrix();

	//Events
		ofPushMatrix(); {
			//Two objects have been detected Near Each Other
				if (m_contourFinder.nBlobs && m_contourFinder2.nBlobs) {
					//Draws the image assosiated with the event
						ofSetColor(255, 255, 255);                                                ///White
						m_NearEachOther.draw(680.0f, 480.0f, (323.0f / 4.0f), (101.0f / 4.0f));   ///Position (Width), Position (Height), Size
					//Text
						ofSetColor(219, 237, 226);                                                ///Soft blue
						ofDrawBitmapString("Objects detected are ", 770.0f, 498.0f);              ///Name, Position (Width), Position (Height)
						ofSetColor(163, 38, 65);                                                  ///Dark pink
						ofDrawBitmapString("Near-Each-Other", 937.0f, 498.0f);                    ///Name, Position (Width), Position (Height)
				}
			//Either one of the objects have been detected
				if (m_contourFinder.nBlobs || m_contourFinder2.nBlobs) {
					//Draws the image assosiated with the event
						ofSetColor(255, 255, 255);                                                 ///White
						m_DetectedImage.draw(680.0f, 400.0f, (313.0f / 4.0f), (171.0f / 4.0f));    ///Position (Width), Position (Height), Size
					//Text
						ofSetColor(219, 237, 226);                                                 ///Soft blue
						ofDrawBitmapString("Object Detected: \n", 770.0f, 410.0f);                 ///Name, Position (Width), Position (Height)
				}
			//Objects lost detection / No object has been detected
				else {
					//Draws the image assosiated with the event
						ofSetColor(255, 255, 255);                                                  ///White
						m_UndetectedImage.draw(680.0f, 400.0f, (295.0f / 4.0f), (155.0f / 4.0f));   ///Position (Width), Position (Height), Size
					//Text
						ofSetColor(219, 237, 226);                                                  ///Soft blue
						ofDrawBitmapString("No objects detected", 770.0f, 425.0f);                  ///Name, Position (Width), Position (Height)
				}
		} ofPopMatrix();

	//GUI		
		m_gui.begin(); {
			//Instructions
				ImGui::Separator();                 ///Separator (Creates a line)
				ImGui::Text("Instructions:	\n");   ///Name
				ImGui::Separator();                 ///Separator (Creates a line)
				//Text instructions
					ImGui::Text("\n  -To pause the video -> SPACEBAR \n  -To detect Object#1 -> Right-Mouse-Click \n  -To detect Object#2 -> Middle-Mouse-Click \n\n\n\n");   ///Text
			//Color selector
				ImGui::Separator();              ///Separator (Creates a line)
				ImGui::Text("Object Tracking");  ///Name
				ImGui::Separator();              ///Separator (Creates a line)
				//RGB color detector
					switch (m_appMode) {
							case Constants::APP_MODE::FIND_COLOR: {
								//Object #1
									ImGui::Text("\nObject #1");                                                ///Name
									ImGui::ColorEdit3("  Selected Color", (float *)&m_trackedColor);           ///"3" refers to RGB array
								//Object #2
									ImGui::Text("Object #2");                                                  ///Name
									ImGui::ColorEdit3("  Selected Color \n\n\n", (float *)&m_trackedColor2);   ///"3" refers to RGB array
								break;
							}
					}
			//Adjust
				ImGui::Separator();      ///Separator (Creates a line)
				ImGui::Text("Adjust");   ///Name
				ImGui::Separator();      ///Separator (Creates a line)
				ImGui::Text("\n");
				//Sliders
					ImGui::SliderInt(" Threshold", &m_threshold, 0, 255);	                ///Name, Variable, Range of values
					ImGui::SliderFloat(" Smoothing Factor", &m_smoothFactor, 0.5f, 1.0f);   ///Name, Variable, Range of values
		}
		m_gui.end();
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	//Toggle to pause the video
		if (key == 32) {   ///32 refers to spacebar
			m_videoPlayer.setPaused(!m_videoPlayer.isPaused());
		}	
}



//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	//Detecting the color of Object#1
		//When you right mouse click on object#1 the color of it will be detected
			if (button == OF_MOUSE_BUTTON_RIGHT && m_appMode == Constants::APP_MODE::FIND_COLOR) { 
				//Convert from screen-space coordinates to "video-based" coordinates so we don't click and set an invalid pixel coordinate when outside image
					//Specifies the position of the video we are trying to interact with
						ofRectangle videoRect(Constants::VIDEO_WIDTH + Constants::VIDEO_BORDER_SIZE - 640.0f, Constants::VIDEO_BORDER_SIZE + 370.0f, Constants::VIDEO_WIDTH, Constants::VIDEO_HEIGHT);
					//Clamp (So when we click anywhere else from ^ it loses detection)
						int convertScreenToImageSpace_X = ofClamp(x, videoRect.getMinX(), videoRect.getMaxX());
						int convertScreenToImageSpace_Y = ofClamp(y, videoRect.getMinY(), videoRect.getMaxY());
					//Shifts everything into the image (gives us the correct pixel of the image that we want)
						convertScreenToImageSpace_X -= videoRect.getMinX();
						convertScreenToImageSpace_Y -= videoRect.getMinY();
					//Finds the pixels (index= x + y * width)
						const int index = (convertScreenToImageSpace_X + convertScreenToImageSpace_Y * m_colorImage.getWidth()) * m_colorImage.getPixels().getBytesPerPixel();
							m_trackedColor[0] = m_colorImage.getPixels()[index + 0] / 255.0f;   ///Red
							m_trackedColor[1] = m_colorImage.getPixels()[index + 1] / 255.0f;   ///Green
							m_trackedColor[2] = m_colorImage.getPixels()[index + 2] / 255.0f;   ///Blue
			}

	//Detecting the color of Object#2
		//When you middle mouse click on object#2 the color of it will be detected
		if (button == OF_MOUSE_BUTTON_MIDDLE && m_appMode == Constants::APP_MODE::FIND_COLOR) { 
			//Convert from screen-space coordinates to "video-based" coordinates so we don't click and set an invalid pixel coordinate when outside image
				//Specifies the position of the video we are trying to interact with
					ofRectangle videoRect = ofRectangle(Constants::VIDEO_WIDTH + Constants::VIDEO_BORDER_SIZE - 640.0f, Constants::VIDEO_BORDER_SIZE + 370.0f, Constants::VIDEO_WIDTH, Constants::VIDEO_HEIGHT);
				//Clamp (So when we click anywhere else from ^ it loses detection)
					int convertScreenToImageSpace_X = ofClamp(x, videoRect.getMinX(), videoRect.getMaxX());
					int convertScreenToImageSpace_Y = ofClamp(y, videoRect.getMinY(), videoRect.getMaxY());
				//Shifts everything into the image (gives us the correct pixel of the image that we want)
					convertScreenToImageSpace_X -= videoRect.getMinX();
					convertScreenToImageSpace_Y -= videoRect.getMinY();
				//Finds the pixels (index= x + y * width)
					const int index = (convertScreenToImageSpace_X + convertScreenToImageSpace_Y * m_colorImage.getWidth()) * m_colorImage.getPixels().getBytesPerPixel();
						m_trackedColor2[0] = m_colorImage.getPixels()[index + 0] / 255.0f;   ///Red
						m_trackedColor2[1] = m_colorImage.getPixels()[index + 1] / 255.0f;   ///Green
						m_trackedColor2[2] = m_colorImage.getPixels()[index + 2] / 255.0f;   ///Blue
		}
}



//--------------------------------------------------------------
void ofApp::processColor() {  

	/*	For color tracking we need to create a new image that is a black and white to represent the found color. Once we have that image, we can pass that 
		image to the color tracker																															*/

	//Finds color for Object #1
		//Create our black and white image representing the color
			const int numPixels = Constants::VIDEO_WIDTH * Constants::VIDEO_HEIGHT * 3;
			//Array of RGB pixels
				float pixel[3] = { 0.0f, 0.0f, 0.0f };
			//Loops through the pixels
				for (int i = 0; i < numPixels; i += 3) {
						pixel[0] = m_colorImage.getPixels()[i + 0];   ///Red
						pixel[1] = m_colorImage.getPixels()[i + 1];   ///Green
						pixel[2] = m_colorImage.getPixels()[i + 2];   ///Blue
					//If/else statment
						if ((abs(pixel[0] - m_trackedColor[0] * 255.0f) < m_threshold) &&
							(abs(pixel[1] - m_trackedColor[1] * 255.0f) < m_threshold) &&
							(abs(pixel[2] - m_trackedColor[2] * 255.0f) < m_threshold)) {
							m_grayscaleDiff.getPixels()[i / 3] = 255;
						}
						else {
							m_grayscaleDiff.getPixels()[i / 3] = 0;
						}
				}
				//Updates the image
					m_grayscaleDiff.flagImageChanged(); 
				//Finds contour from the updated image
					m_contourFinder.findContours(m_grayscaleDiff, m_minArea, m_maxArea, m_numContoursConsidered, false, true);
}



//--------------------------------------------------------------
void ofApp::processColor2() {

	/*	For color tracking we need to create a new image that is a black and white to represent the found color. Once we have that image, we can pass that
		image to the color tracker																															*/

		//Finds color for Object #1
			//Create our black and white image representing the color
				const int numPixels = Constants::VIDEO_WIDTH * Constants::VIDEO_HEIGHT * 3;
			//Array of RGB pixels
				float pixel[3] = { 0.0f, 0.0f, 0.0f };
			//Loops through the pixels
				for (int i = 0; i < numPixels; i += 3) {
						pixel[0] = m_colorImage.getPixels()[i + 0];   ///Red
						pixel[1] = m_colorImage.getPixels()[i + 1];   ///Green
						pixel[2] = m_colorImage.getPixels()[i + 2];   ///Blue
					//If/else statment
						if ((abs(pixel[0] - m_trackedColor2[0] * 255.0f) < m_threshold) &&
							(abs(pixel[1] - m_trackedColor2[1] * 255.0f) < m_threshold) &&
							(abs(pixel[2] - m_trackedColor2[2] * 255.0f) < m_threshold)) {
							m_grayscaleDiff.getPixels()[i / 3] = 255;
						}
						else {
							m_grayscaleDiff.getPixels()[i / 3] = 0;
						}
				}
				//Updates the image
					m_grayscaleDiff.flagImageChanged(); 
				//Finds contour from the updated image
					m_contourFinder2.findContours(m_grayscaleDiff, m_minArea2, m_maxArea2, m_numContoursConsidered2, false, true);
}



//--------------------------------------------------------------
float ofApp::lerp(float start, float end, float percent) {
	//Linear Interpolation formula for "easing" between points (or smoothing data)
		return (start + percent * (end - start));
}
