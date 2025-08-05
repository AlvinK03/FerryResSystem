//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============================================================
//============================================================
/*
* Filename: vessel.hpp
*
* Description: Header file of the Vessel module of the Ferry
* Reservation System, being the only module able
* to modify the file i/o containing information
* about vessels
* Is a data storage only module
* Should call the init() function before any
* operations
*/
//============================================================
#pragma once
#include <iostream>
#include <string>
//============================================================
// Struct: Vessel
// Purpose: Represents a vessel with its name, and capacity
// It is exported with public access to allow other modules
// to easily binary read/write fields in the binary data file
//------------------------------------------------------------
struct Vessel
{
    char name[26]; // Unique vessel name
    float HCLL; // High Ceiling Lane Length (meters)
    float LCLL; // Low Ceiling Lane Length (meters)
};
//============================================================
// Function vesselOpen creates and opens the Vessel file
// Throws an exception if the file cannot be opened
//------------------------------------------------------------
void vesselOpen();
// Function vesselReset seeks to the beginning of the Vessel file
// Throws an exception if the file is not open
//------------------------------------------------------------
void vesselReset();
// Function getNextVessel obtains a line from the Vessel file
// Returns a boolean if retrieving all the data was successful
// Throws an exception if the read operation fails
//------------------------------------------------------------
bool getNextVessel(Vessel& v);
// Function writeVessel writes to the Vessel file
// Throws an exception if the write operation fails
//------------------------------------------------------------
void writeVessel(const Vessel& v);
// Function vesselClose closes the Vessel file
// Throws an exception if the file was already closed
//------------------------------------------------------------
void vesselClose();