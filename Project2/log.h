#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/world.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>

//searching cards contours and get theirs positions
//std::vector<std::vector<cv::Point>> contours - input vector of contours of cards
//std::vector<cv::Vec4i> hierarchy - input vector of hierarchy of contours of cards
//std::vector<int>& cardContoursPos - vector of card contours position in contours vector, which is reference
//int& noParent - reference veriable of amount of contours, which have not parents
bool getCards(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Vec4i> hierarchy, std::vector<int>& cardContoursPos, int& noParent);

//find children for card and count theirs 
//std::vector<std::vector<cv::Point>> contours - input vector of contours
//std::vector<cv::Vec4i> hierarchy - input vector of contours hierarchy
//std::vector<int> cardContoursPos - input vector of posiotion of card without parents in contours/hierarchy vectors
//std::vector<int>& cardFirstChildrenNumber - reference vector of amount of children for contours without parents
bool getCardsFirstChildren(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Vec4i> hierarchy, std::vector<int> cardContoursPos, std::vector<int>& cardFirstChildrenNumber);

//find center of cards
//std::vector<std::vector<cv::Point>> contours - input vector of contours
//std::vector<int> cardContoursPos - input vector of positions from contours vector for that which have not got parents
//std::vector<cv::Point2d>& centerOfCards - reference vector of center of card (main contours, which have not got parents)
bool getCardsCenters(std::vector<std::vector<cv::Point>> contours, std::vector<int> cardContoursPos, std::vector<cv::Point2d>& centerOfCards);

//get highest Card
//std::vector<int> cardFirstChildrenNumber - input vector of amount of children of first parent contours(cards)
//int& hightsCard - reference variable of highest card value
//int& hightsCardPos - reference variable of posiotion of highest card
bool getHighestCard(std::vector<int> cardFirstChildrenNumber, int& hightsCard, int& hightsCardPos);

//mark image with card and text
//cv::Mat& img - reference image, which will be marked
//std::vector<cv::Point2d> centerOfCards - input vector of center of cards
//int hightsCardPos - input value of position of highest card
//int hightsCard - input value of highest card
bool markImage(cv::Mat& img, std::vector<cv::Point2d> centerOfCards, int hightsCardPos, int hightsCard);

//check is input image with salt pepper noise
//cv::Mat input - input image
bool checkIsSaltPepper(cv::Mat input);

//check is input image with blur noise
//cv::Mat input - input image
bool checkIsBlur(cv::Mat input);

//check is input image with gradient noise
//cv::Mat input - input image
bool checkIsGradient(cv::Mat input);

//filter image with salt pepper noise
//cv::Mat& img - reference input image
//int threshold = 160 - treshold value, default 160
//int medianBlurSize = 5 - size of kernel of median blur filter, default 5
bool saltPepperFilter(cv::Mat& img, int threshold = 160, int medianBlurSize = 5);

//filter image with blur noise
//cv::Mat& img - reference input image
//int threshold = 160 - treshold value, default 160
bool blurFilter(cv::Mat& img, int threshold = 160);


//filter image with gradient noise
//cv::Mat& img - reference input image
//int correction = 60 - variable of correction value, default 60
//int threshold = 160 - treshold value, default 160
//int medianBlurSize = 5 - size of kernel of median blur filter, default 5
bool gradientFilter(cv::Mat& img, int correction = 60, int threshold = 160, int medianBlurSize = 5);