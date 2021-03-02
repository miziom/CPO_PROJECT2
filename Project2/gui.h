#pragma once

#include <stdlib.h>
#include "log.h"

//show input image in window
//cv::Mat - input image
//std::string winName - name of window
//bool wait - decision if after display image program should wait
bool draw(const cv::Mat img, std::string winName, bool wait = true);

//print first part of menu, user gets know wchich number means which decision
void printImgName();

//print line after which user types decision number
void printChoseLine();

//remove white spaces from input string starting from the end
//std::string& input - input string which is reference
void removeWhiteSpaces(std::string& inputString);

//check if input string size is 1
//std::string input - input string
bool checkInputSize(std::string inputString);

//check if input string is correct
//std::string input - input string
bool checkIsCorrectly(std::string inputString);

//check if input string is decisoin
//std::string decision - input string
bool checkDecision(std::string decision);

//check user decision up to decision number
//int decisionInt - input decision number,
//cv::Mat &img - input image which is reference,
//cv::Mat &imgCopy - input image which is reference
bool makeDecision(int decisionInt, cv::Mat& img, cv::Mat& imgCopy);

//method which run program
bool program();