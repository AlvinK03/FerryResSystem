//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//================================================================
//================================================================
/*
 * Filename: sailing.hpp
 *
 * Description: The header file for SailingManagement module of the Coastal
 *              Ferry Reservation System. This module is meant to keep information
 *              about ship sailings and it is the only moduel that can modify
 *              I/O file containing all the sailing data.
 *              init() should be called before any operations are performed.
 */
//================================================================
#pragma once 
#include <iostream>
#include <string>
using std::string;
//================================================================
// Struct: Sailing
// Purpose: Represents a vessel with its name, and capacity
//----------------------------------------------------------------
struct Sailing
{
  char sailingID[10]; // Sailing ID consisting of 9 characters
  char vesselName[26]; // Unique vessel name, consisting up to 25 characteres
  float lowRemainingLength; // Available low remaining length
  float highRemainingLength; // Available high remaining length
};
//================================================================
// Function open creates and opens the Sailing file
// Throws an exception if the file cannot be opened
//----------------------------------------------------------------
void sailingOpen();
// Function close closes the Sailing file
//----------------------------------------------------------------
void sailingClose();
// Function reset seeks to the beginning of the Sailing file
// Throws an exception if the file is not open
//----------------------------------------------------------------
void sailingReset();
// Function getNextSailing obtains a line from the Sailing file
// Returns a boolean if retrieving all the data was successful
// Throws an exception if the read operation fails
//----------------------------------------------------------------
bool getNextSailing(Sailing& s);
// Function writeSailing writes a sailing record to the Sailing file
// Throws an exception if the write operation fails
//----------------------------------------------------------------
void writeSailing(const Sailing& s);
// Function deleteSailing deletes a sailing record with the provided
// sailingID. Throws an exception if the record is not found.
//----------------------------------------------------------------
void deleteSailing(const char sailingID[]);
// Function checkSailingExists checks if a sailing with the provided
// sailingID exists. Returns sailingID, otherwise throws exception.
//----------------------------------------------------------------
int checkSailingExists(const char sailingID[]);