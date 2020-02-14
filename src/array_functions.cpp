/*
 * functionstocomplete.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: keith
 */

//============================================================================
//	TODO add necessary includes here
//============================================================================
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sstream>

#include "array_functions.h"
#include "constants.h"
#include "utilities.h"

using namespace constants;
using namespace std;

const string DEFAULT_WORD_VALUE = "";
const int DEFAULT_OCCURANCES_VALUE = 0;

struct word_occurances{
	int occurances;
	string word;
};

//TODO add a global array of entry structs (global to this file)
word_occurances words[50] = {};

//TODO add variable to keep track of next available slot in array
int next_slot = 0;

//zero out array that tracks words and their occurrences
void clearArray(){
	for(int i = 0; i < getArraySize(); i++){
		words[i].occurances = DEFAULT_OCCURANCES_VALUE;
		words[i].word = DEFAULT_WORD_VALUE;
	}
	next_slot = 0;
}

//how many unique words are in array
int getArraySize(){
	return next_slot;
}

//get data at a particular location
string getArrayWordAt(int i){
	if(i < getArraySize()){
		return words[i].word;
	}
	else{
		return DEFAULT_WORD_VALUE;
	}
}

int getArrayWord_NumbOccur_At(int i){
	if(i < getArraySize()){
		return words[i].occurances;
	}
	else{
		return DEFAULT_OCCURANCES_VALUE;
	}
}

/*loop through whole file, one line at a time
 * call processLine on each line
 * returns false: myfstream is not open
 *         true: otherwise*/
bool processFile(fstream &myfstream){
	if(myfstream.is_open()){
		string line;
		while(!myfstream.eof()){
			getline(myfstream, line);
			processLine(line);
		}
		return true;
	}
	else{
		return false;
	}
}

/*take 1 line and extract all the tokens from it
feed each token to processToken for recording
Used the given code on github*/
void processLine(string &myString){
	stringstream ss(myString);
	string tempToken;

	while(getline(ss, tempToken, CHAR_TO_SEARCH_FOR)){
		processToken(tempToken);
	}
}

/*Keep track of how many times each token seen*/
//Input is a string
//The string may contain characters that need to be removed with strip_unwanted_chars()
//Add the manipulated string to the list
void processToken(string &token){
	string clone = token;
	toUpper(clone);
	if(strip_unwanted_chars(token)){  //If the method returns false, there won't be anything to add
		if(getArraySize()>0){
			for(int i = 0; i<getArraySize(); i++){
				string temp_val = getArrayWordAt(i);
				toUpper(temp_val);
				if(clone == temp_val){
					words[i].occurances++;
					return;  //Once it finds the place to add the word, it exits the method
				}
			}
		}
		//If word does not exist in the array, it is added to it.
		words[next_slot].word = token;
		words[next_slot].occurances = 1;
		next_slot += 1;
	}
}

/*if you are debugging the file must be in the project parent directory
  in this case Project2 with the .project and .cProject files*/
bool openFile(fstream& myfile, const string& myFileName, ios_base::openmode mode){
	if(myfile.is_open()){
		return false;
	}
	else{
		myfile.open(myFileName, mode);
		if(myfile.is_open()){
			return true;
		}
		else{
			return false;
		}
	}
}

/*if myfile is open then close it*/
void closeFile(fstream& myfile){
	if(myfile.is_open()){
		myfile.close();
	}
}

/* serializes all content in myEntryArray to file outputfilename
 * returns  FAIL_FILE_DID_NOT_OPEN if cannot open outputfilename
 * 			FAIL_NO_ARRAY_DATA if there are 0 entries in myEntryArray
 * 			SUCCESS if all data is written and outputfilename closes OK
 * */
int writeArraytoFile(const string &outputfilename){
	fstream myfile;
	if(getArraySize() == 0){
		return FAIL_NO_ARRAY_DATA;
	}
	else if(openFile(myfile, outputfilename, ios::out)){
		for(int i = 0; i<getArraySize(); i++){
			//cout << "Help?";
			//cout << getArrayWordAt(i) << " " << getArrayWord_NumbOccur_At(i) << endl;
			myfile << getArrayWordAt(i) << " " << getArrayWord_NumbOccur_At(i) << endl;
		}
		closeFile(myfile);
		return SUCCESS;
	}
	else{
		//cout << "Help";
		return FAIL_FILE_DID_NOT_OPEN;
	}
}

/*
 * Sort myEntryArray based on so enum value.
 * The presence of the enum implies a switch statement
 * Uses an insertion sort method for all cases
 */
void sortArray(constants::sortOrder so){
	switch(so){
	case NONE:
		break; //No sorting required
	default:  //Default sortArray will use an ascending sort
	case ASCENDING:
		for (int i = 1; i < getArraySize(); i++)
		{
			word_occurances temp_val = words[i];
			int j = i - 1;

			/* Move elements of arr[0..i-1], that are
			greater than key, to one position ahead
			of their current position */
			while (j >= 0 && words[j].word > temp_val.word)
			{
				words[j + 1] = words[j];
				j = j - 1;
			}
			words[j + 1] = temp_val;
		}
		break;
	case DESCENDING:
		for (int i = 1; i < getArraySize(); i++)
		{
			word_occurances temp_val = words[i];
			int j = i - 1;

			/* Move elements of arr[0..i-1], that are
			greater than key, to one position ahead
			of their current position */
			while (j >= 0 && words[j].word < temp_val.word)
			{
				words[j + 1] = words[j];
				j = j - 1;
			}
			words[j + 1] = temp_val;
		}
		break;
	case NUMBER_OCCURRENCES:
		for (int i = 1; i < getArraySize(); i++)
		{
			word_occurances temp_val = words[i];
			int j = i - 1;

			/* Move elements of arr[0..i-1], that are
			greater than key, to one position ahead
			of their current position */
			while (j >= 0 && words[j].occurances < temp_val.occurances)
			{
				words[j + 1] = words[j];
				j = j - 1;
			}
			words[j + 1] = temp_val;
		}
		break;
	}
}

