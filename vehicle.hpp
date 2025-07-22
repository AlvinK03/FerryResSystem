//============================================================
//============================================================
/*
* Filename: vehicle.hpp
*
* Description: Header file of the Vehicle module of the Ferry
* Reservation System, being the only module able
* to modify the file i/o containing information
* about vehicles
* Is a data storage only module
* Should call the init() function before any
* operations
*
*/
//============================================================
#pragma once
#include <iostream>
#include <string>

//============================================================
// Struct: Vehicle
// Purpose: Represents a vehicle with its name, and associated
// phone number, and length/width measurements
//------------------------------------------------------------
struct Vehicle
{
    char vehicleLicence[10];
    // Unique vehicle licence, consisting of 6-10 characters
    char phone[14]; // Unique phone number
    float vehicleHeight; // Vehicle height (meters)
    float vehicleLength; // Vehicle length (meters)
};
//============================================================
// Function open creates and opens the Vehicle file
// Throws an exception if the file cannot be opened
//------------------------------------------------------------
void vehicleOpen();
// Function reset seeks to the beginning of the Vehicle file
// Throws an exception if the file is not open
//------------------------------------------------------------
void vehicleReset();
// Function getNextVehicle obtains a line from the Vehicle file
// Returns a boolean if retrieving all the data was successful
// Throws an exception if the read operation fails
//------------------------------------------------------------
bool getNextVehicle(Vehicle& v);
// Function writeVehicle writes to the Vehicle file
// Throws an exception if the write operation fails
//------------------------------------------------------------
void writeVehicle(const Vehicle& v);
// Function close closes the Vehicle file
//------------------------------------------------------------
void vehicleClose();