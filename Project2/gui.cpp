#include "gui.h"

bool draw(const cv::Mat img, std::string winName, bool wait) {
	//if image is empty return false
	if (img.empty()) {
		std::cout << "Wrong input" << std::endl;
		return false;
	}
	cv::namedWindow(winName, cv::WND_PROP_FULLSCREEN);
	cv::imshow(winName, img);
	if (wait) {
		cv::waitKey(0);
	}
	return true;
}

void printImgName() {
	std::cout << "Created by Mateusz Mizio" << std::endl;
	std::cout << "1. Original Image." << std::endl;
	std::cout << "2. Blur Noise Image." << std::endl;
	std::cout << "3. Gradient Nosie Image." << std::endl;
	std::cout << "4. Salt Pepper Noise Image." << std::endl;
	std::cout << "5. Close." << std::endl;
}

void printChoseLine() {
	std::cout << "Choose image to detect (1-4) or close (5): ";
}

void removeWhiteSpaces(std::string& inputString) {
	for (int i = inputString.size() - 1; i >= 0; i--) {
		if (inputString[i] == ' ') {
			inputString.erase(i, 1);
		}
	}
}

bool checkInputSize(std::string inputString) {
	//if input string size is not 1 return false
	if (inputString.size() != 1) {
		return false;
	}
	else {
		return true;
	}
}

bool checkIsCorrectly(std::string inputString) {
	for (int i = 0; i < inputString.size(); i++) {
		//for each part of string check is equal 1, 2, 3, 4 or 5 and if not return false
		if (!((inputString[i] == '1') || (inputString[i] == '2') || (inputString[i] == '3')
			|| (inputString[i] == '4') || (inputString[i] == '5'))) {
			return false;
		}
	}
	return true;
}

bool checkDecision(std::string decision) {
	//remove white spaces in input string
	removeWhiteSpaces(decision);
	//check input size, if equals not 1 return false
	if (!checkInputSize) {
		return false;
	}
	//check if input is 1, 2, 3, 4 or 5, if not return false
	if (!checkIsCorrectly(decision)) {
		return false;
	}
	return true;
}

bool makeDecision(int decisionInt, cv::Mat &img, cv::Mat &imgCopy) {
	switch (decisionInt) {
		//load image without noise
		case 1: {
			img = cv::imread("image.bmp");
			imgCopy = img;
			return true;
		}
		//load image with blur noise
		case 2: {
			img = cv::imread("image_blur.bmp");
			imgCopy = img;
			return true;
		}
		//load image with gradient noise
		case 3: {
			img = cv::imread("image_gradient.bmp");
			imgCopy = img;
			return true;
		}
		//load image with salt pepper noise
		case 4: {
			img = cv::imread("image_salt_pepper.bmp");
			imgCopy = img;
			return true;
		}
		//exit program
		case 5: {
			exit(1);
		}
	}
	return false;
}

bool program() {
	//print menu
	printImgName();
	printChoseLine();
	//get user decision
	std::string decision;
	std::getline(std::cin, decision);
	if (!checkDecision(decision)) {
		return false;
	}
	//convert decision from string to int
	int decisionInt = std::stoi(decision);
	//initialize place for input image
	cv::Mat img;
	cv::Mat imgCopy;
	//load image up to user decision
	if (!makeDecision(decisionInt, img, imgCopy)) {
		return false;
	}
	//convert image to grayscale
	cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
	//check if image is with salt pepper noise 
	if (checkIsSaltPepper(img)) {
		//filter image with salt pepper noise
		saltPepperFilter(img, 160, 5);
	}
	//check if image is with blur noise
	else if (checkIsBlur(img)) {
		//filter image with blur noise
		blurFilter(img, 160);
	}
	//check if image is with gradient noise 
	else if (checkIsGradient(img)) {
		//filter image with gradient noise
		gradientFilter(img, 60, 160, 5);
	}
	else
	{
		//convert image to binary image 
		cv::threshold(img, img, 160, 255, 0);
	}
	//initialize vectors for contours and hierarchy
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	//find contours of cards
	cv::findContours(img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	//initialize vector for card contours posiotions and integer amount of first parents
	std::vector<int> cardContoursPos;
	int noParent = 0;
	//find card
	getCards(contours, hierarchy, cardContoursPos, noParent);
	//initialize vector for number of first child for first parent cards
	std::vector<int> cardFirstChildrenNumber;
	//get number of first child for first parent cards
	getCardsFirstChildren(contours, hierarchy, cardContoursPos, cardFirstChildrenNumber);
	//initialize vector for center of cards
	std::vector<cv::Point2d> centerOfCards;
	getCardsCenters(contours, cardContoursPos, centerOfCards);
	///initialize integers for highest card positions and highest card
	int hightsCardPos = -1;
	int hightsCard = 0;
	//get know about highest card
	getHighestCard(cardFirstChildrenNumber, hightsCard, hightsCardPos);
	//mark image with highest card
	markImage(imgCopy, centerOfCards, hightsCardPos, hightsCard);
	//display image result
	draw(imgCopy, "Result");
	return false;
}