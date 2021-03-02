#include "log.h"

bool getCards(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Vec4i> hierarchy, std::vector<int>& cardContoursPos, int& noParent) {
	//go throw hierarchy and contours vectors
	for (int i = 0; i < hierarchy.size(); i++) {
		//if contours have not parent and points in contours is more then 50, it save as card
		if (hierarchy[i][3] == -1 && contours[i].size() > 50) {
			//save position of contour and increment amount of cards without parents
			cardContoursPos.push_back(i);
			noParent++;
		}
	}
	return true;
}

bool getCardsFirstChildren(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Vec4i> hierarchy, std::vector<int> cardContoursPos, std::vector<int>& cardFirstChildrenNumber) {
	//go throw card contours positions vector
	for (int cardContPos = 0; cardContPos < cardContoursPos.size(); cardContPos++) {
		//for each posiotion
		//start -4, because 4 children are - 2 from numbers, 2 from type under numbers
		cardFirstChildrenNumber.push_back(-4);
		//go throw hierarchy vector
		for (int i = 0; i < hierarchy.size(); i++) {
			//id contours have found parent and their contour is longer then 5, increment amount of children for that contour
			if (hierarchy[i][3] == cardContoursPos[cardContPos] && contours[i].size() > 5) {
				cardFirstChildrenNumber[cardContPos]++;
			}
		}
	}
	return true;
}

bool getCardsCenters(std::vector<std::vector<cv::Point>> contours, std::vector<int> cardContoursPos, std::vector<cv::Point2d>& centerOfCards) {
	//go throw card contours posiotion vector
	for (int a : cardContoursPos) {
		//count moments for contour
		auto hu = cv::moments(contours[a]);
		//count center up to counted moments
		cv::Point2d center = cv::Point2d(hu.m10 / hu.m00, hu.m01 / hu.m00);
		//save counted center to vector
		centerOfCards.push_back(center);
	}
	return true;
}

bool getHighestCard(std::vector<int> cardFirstChildrenNumber, int& hightsCard, int& hightsCardPos) {
	//go throw vector of amount of children of main contours
	for (int i = 0; i < cardFirstChildrenNumber.size(); i++) {
		//if amount of children is greater than highest card, then save highest card and their position
		if (cardFirstChildrenNumber[i] > hightsCard) {
			hightsCard = cardFirstChildrenNumber[i];
			hightsCardPos = i;
		}
	}
	return true;
}

bool markImage(cv::Mat& img, std::vector<cv::Point2d> centerOfCards, int hightsCardPos, int hightsCard) {
	//create text for display
	std::string text = "The Highest card is: " + std::to_string(hightsCard);
	//put text in image
	cv::putText(img, text, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
	//draw circle in image in posiotion of highest card
	cv::circle(img, centerOfCards[hightsCardPos], 50, cv::Scalar(0, 255, 0), -1);
	return true;
}

bool checkIsSaltPepper(cv::Mat input) {
	//initialize variable for 255 and 0 values pixels
	int whiteNumber = 0;
	int blackNumber = 0;
	//go throw image
	for (int row = 0; row < input.rows; row++) {
		for (int col = 0; col < input.cols; col++) {
			//if pixel value equals 255 increment whiteNumber variable
			if (input.at<uchar>(row, col) == 255) {
				whiteNumber++;
			}
			//else if pixel value equals 0 increment blackNumber variable
			else if (input.at<uchar>(row, col) == 0) {
				blackNumber++;
			}
		}
	}
	//if amount of black and also white pixel is more then 10000, it can be image with salt pepper noise
	if (whiteNumber > 10000 && blackNumber > 10000) {
		return true;
	}
	else
	{
		return false;
	}
}

bool checkIsBlur(cv::Mat input) {
	//initialize image as copy of input image
	cv::Mat imgLaplacian = input;
	//calculate laplacian of image
	cv::Laplacian(input, imgLaplacian, CV_64F);
	//initialize variables for standard deviation and mean value
	cv::Scalar mean, stddev;
	//calulate standard deviation and mean value
	cv::meanStdDev(imgLaplacian, mean, stddev, cv::Mat());
	//calculate variance of standard deviation
	double variance = stddev.val[0] * stddev.val[0];
	//if variance is from 0 to 1 image can be with blur nosie
	if (0 <= variance && variance < 1) {
		return true;
	}
	else {
		return false;
	}
}

bool checkIsGradient(cv::Mat input) {
	//initialize image as copy of input image
	cv::Mat imgLaplacian = input;
	//calculate laplacian of image
	cv::Laplacian(input, imgLaplacian, CV_64F);
	//initialize variables for standard deviation and mean value
	cv::Scalar mean, stddev;
	//calulate standard deviation and mean value
	cv::meanStdDev(imgLaplacian, mean, stddev, cv::Mat());
	//if mean value is smaller than 0 , image can be with gradient noise or blur, so it's important to check first blur
	if (mean[0] < 0) {
		return true;
	}
	else {
		return false;
	}
}

bool saltPepperFilter(cv::Mat& img, int threshold, int medianBlurSize) {
	//use median filter
	cv::medianBlur(img, img, medianBlurSize);
	//convert image to binary image 
	cv::threshold(img, img, threshold, 255, 0);
	return true;
}

bool blurFilter(cv::Mat& img, int threshold) {
	//initialize variables for gaussianBlur results
	cv::Mat gaussianResult, unsharpMask;
	//blur image with gaussian blur method
	cv::GaussianBlur(img, gaussianResult, cv::Size(501, 501), 2.0, 2.0);
	//calculate image up to result from gaussian blur
	cv::addWeighted(img, 1.8, gaussianResult, -1, 0, unsharpMask);
	//change input image to result of addWeighted method
	img = unsharpMask;
	//convert image to binary image 
	cv::threshold(img, img, threshold, 255, 0);
	return true;
}

bool gradientFilter(cv::Mat& img, int correction, int threshold, int medianBlurSize) {
	//go throw second part of image
	for (int row = 0; row < img.rows; row++) {
		for (int col = img.cols / 2; col < img.cols; col++) {
			//make pixel lighter
			if ((img.at<uchar>(row, col) < threshold) && (img.at<uchar>(row, col) > threshold - correction)) {
				img.at<uchar>(row, col) = threshold + 5;
			}
		}
	}
	//convert image to binary image 
	cv::threshold(img, img, threshold - 1, 255, 0);
	//use median filter
	cv::medianBlur(img, img, medianBlurSize);
	return true;
}