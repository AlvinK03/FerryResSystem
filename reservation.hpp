//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============================================================
//============================================================
/*
* Filename: reservation.hpp
*
* Description: Header file of the Reservation module of the Ferry Reservation System,
* checks if information already exists for reservation and in some cases
* deletes. This is the only module able to modify thee file i/o containing
* information about reservations.
* Is a data storage only module
* Should call the init() function before any
* operations
* 
* Design Issues: Using linear search for all traversal and deletions
* Must be on a system able to use fstream
* Fixed-length records may waste space
*/
//================================================================

#pragma once
#include <iostream>
#include <string>
using std::endl; 
using std::cout;
using std::string;

//================================================================
// Struct: Reservation
// Purpose: Represents a vessel with its name, and capacity
//----------------------------------------------------------------
struct Reservation
{
    char sailingID[10]; // 9 digit sailing id
    char vehicleLicence[11]; // Unique vehicle licence, consisting of 6-10 characters
    bool onBoard; // Specifies if a reservation has checked in
    bool isLRL; // Specifies which section of the sailing the vehicle is to be parked
};

//================================================================

// Function creates and opens reservation file.
// Throw an exception if it cannot be opened.
//----------------------------------------------------------------
void reservationOpen();

// Function resets to the beginning of the list.
// Throw an exception if it cannot be opened.
//----------------------------------------------------------------
void reservationReset();

// Function getNextReservation returns a line from the data
// Returns a boolean if the data is successfully read
// Throws an exception if there is an error with reading the files
//----------------------------------------------------------------
bool getNextReservation(Reservation& r);

// Function writeReservation writes to reservation file
// Throws an exception if it fails
//----------------------------------------------------------------
void writeReservation(const Reservation& r, bool overWrite);


// Function closes reservation file
//----------------------------------------------------------------
void reservationClose();

// Function deleteReservation deletes a reservation with the provided
// sailingID and vehicleLicence. Throws an exception if the record is not found.
//----------------------------------------------------------------
void deleteReservation(char sailingID[], char vehicleLicence[]);