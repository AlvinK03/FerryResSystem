//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============================================================
//============================================================
/*
* Filename: testFileUnit2.cpp
*
* Revision History:
* Rev. 1 - 25/07/23 Original by A. Chung
*
* Unit Test: Delete functionality in Reservation
* Writes three Reservation records, then deletes the second Reservation record.
* if written correctly, calling a deleteReservation() again on the same record should result in an error
* an exception would be thrown producing a "PASS"
*
* Test Type: Bottom-up integration
* Preconditions:
* - The Reservation file is not used by another module
* - The file may or may not already exist
* - If the file alreaady exists, it must be empty
* - File must have at least 2 or more reservation records
* Test Steps:
* 1. Open file with reservationOpen()
* 2. Create 2 or more reservations
* 3. Write it to file using writeReservation()
* 4. Call deleteReservation() to attempt deletion of the 2nd reservation
* 5. Check again by pulling the same delete function on the same reservation.
* 6. Print "Pass" or "Fail"
*/
//============================================================

#include "reservation.hpp"
#include <iostream>
#include <cstring>

int main() {
//Creates 3 reservation records

    Reservation r, r2, r3;
    strncpy(r.sailingID, "123-03-45", sizeof(r.sailingID) - 1);
    r.sailingID[sizeof(r.sailingID) - 1] = '\0';
    strncpy(r.vehicleLicence, "123ASD", sizeof(r.vehicleLicence) - 1);
    r.vehicleLicence[sizeof(r.vehicleLicence) - 1] = '\0';
    r.onBoard = false;
    r.isLRL = true;

    strncpy(r2.sailingID, "987-63-22", sizeof(r2.sailingID) - 1);
    r2.sailingID[sizeof(r2.sailingID) - 1] = '\0';
    strncpy(r2.vehicleLicence, "232HHH", sizeof(r2.vehicleLicence) - 1);
    r2.vehicleLicence[sizeof(r2.vehicleLicence) - 1] = '\0';
    r2.onBoard = false;
    r2.isLRL = false;

    strncpy(r3.sailingID, "808-10-10", sizeof(r3.sailingID) - 1);
    r3.sailingID[sizeof(r3.sailingID) - 1] = '\0';
    strncpy(r3.vehicleLicence, "5PQ222", sizeof(r3.vehicleLicence) - 1);
    r3.vehicleLicence[sizeof(r3.vehicleLicence) - 1] = '\0';
    r3.onBoard = false;
    r3.isLRL = false;
    // Open reservation file
    reservationOpen();
    // Write test reservations to file
    writeReservation(r,false);
    writeReservation(r2,false);
    writeReservation(r3,false);

    // Test 1: Delete reservation r2
    std::cout << "\n=== Testing deletion of reservation 2 ===\n";
    deleteReservation(r2.sailingID, r2.vehicleLicence);
    std::cout << "Delete operation completed for reservation 2\n";
    // Test 2: Try to delete already deleted reservation (should throw)
    try
    {
        std::cout << "\n=== Test delete reservation 2 ===\n";
        deleteReservation(r2.sailingID, r2.vehicleLicence);
        std::cout << "FAIL " << endl;
    } 
    catch (const std::exception& e)
    {
        std::cout << "PASS " << endl;
    }
}

