//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============================================================
//============================================================
/*
* Filename: reservationManager.hpp
*
* Revision History:
* Rev. 1 - 25/07/23 Original by A. Chung
*
* Description: Header file for the reservationManager moduleof the Reservation
* module of the FerryReservation System, being the module that
* manages sailing, vehicle, and reservation module functions
* 
* Design Issues: Using linear search for all traversal and deletions
*/
//================================================================
#pragma once
#include <iostream>
#include <string>
using std::endl;
using std::cout;
using std::string;
//================================================================
// Function accessSailingManagerUpdate accesses the Sailing Manager module
// to update a sailing
//----------------------------------------------------------------
void accessSailingManagerUpdate(char sailingID[]);
// Function accessSailingManager accesses the Sailing Manager module
// to query a sailing
//----------------------------------------------------------------
void accessSailingManagerQuery(char sailingID[]);
// Function vehicleCheck checks to see if the vehicle with provided licence plate exists.
// If vehicle does not exist in the system, create a record for vehicle.
//----------------------------------------------------------------
void vehicleCheck(char vehicleLicence[]);
// Function createReservation creates a reservation for the vehicle
// with the corresponding licence plate on the specified sailing
//----------------------------------------------------------------
void createReservation(char sailingID[], char vehicleLicence[]);
// Function deleteReservations with parameters sailingID, vehicleLicence
// deletes a reservation on the specified sailing
// for the vehicle with the corresponding licence plate
//----------------------------------------------------------------
void createReservationRepeat(char input);
void deleteReservations(char sailingID[], char vehicleLicence[]);
// Function deleteReservations with single parameter sailingID
// deletes all reservations on the specified sailing
//----------------------------------------------------------------
void deleteReservations(char sailingID[]);
// Function viewReservations returns the number of reservations for a sailing
int viewReservations(char sailingID[]);
// Function checkIn() sets the status of specified reservation as checked in
//----------------------------------------------------------------
float checkIn(char sailingID[], char vehicleLicence[]);