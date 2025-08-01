//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============================================================
//============================================================
/*
* Filename: reservationManager.cpp
*
* Revision History:
* Rev. 1 - 25/07/23 Original by A. Chung
*
* Description: Implementation file of the Reservation module of the Ferry
* Reservation System, being the module that manages sailing, vehicle, and
* reservation module functions
* 
* Design Issues: Using linear search for all traversal and deletions
*/
//================================================================
#include "reservationManager.hpp"
#include "vehicle.hpp"
#include "reservation.hpp"
#include "sailingManager.hpp"
#include <cstring>
#include <cctype>
#include <cstdio>
#include <vector>
#ifdef _WIN32
  #include <io.h>      
#else
  #include <unistd.h>  
  #include <fcntl.h>
#endif
//================================================================
// Function accessSailingManagerUpdate accesses the Sailing Manager module
// to update a sailing
//----------------------------------------------------------------
void accessSailingManagerUpdate(char sailingID[])
{
    try
    {
        // First verify the sailing exists
        if (sailingManagerExists(sailingID) != 1)
        {
            throw std::runtime_error("Sailing does not exist.");
        }
        
        // Get updated sailing information
        updateSailing(sailingID, 0);
        
        // Update reservation counts
        accessReservationManager(sailingID);
        
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error updating sailing: " << e.what() << std::endl;
        throw; // Re-throw for calling function to handle
    }
}
// Function accessSailingManager accesses the Sailing Manager module
// to query a sailing
//----------------------------------------------------------------
void accessSailingManagerQuery(char sailingID[])
{
    try
    {
        // Verify sailing exists
        if (sailingManagerExists(sailingID))
        {
            // Display sailing details
            char* selectedSailing = querySailing();
            
            // Show reservation count
            int reservations = viewReservations(sailingID);
            std::cout << "Total reservations: " << reservations << std::endl;
            
            // Show vessel capacity
            char* vessel = getVessel(); // Gets vessel for this sailing
            int capacity = getVesselLength(vessel);
            std::cout << "Vessel capacity: " << capacity << " meters" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error querying sailing: " << e.what() << std::endl;
        throw; // Re-throw for calling function to handle
    }
}
// Function vehicleCheck checks to see if the vehicle with provided licence plate exists.
// If vehicle does not exist in the system, create a record for vehicle.
//----------------------------------------------------------------
void vehicleCheck(char vehicleLicence[])
{
    //check if vehicle exists
    bool vehicleExists = false;
    Vehicle v;
    //resets vehicle to start of list
    vehicleReset();
    //find the vehicle
    while (getNextVehicle(v)) {
        if (strcmp(v.vehicleLicence, vehicleLicence) == 0) {
            vehicleExists = true;
            break;
        }
    }
    if (!vehicleExists) {
        // Vehicle doesn't exist - create new record
        Vehicle newVehicle;
        strncpy(newVehicle.vehicleLicence, vehicleLicence, sizeof(newVehicle.vehicleLicence) - 1);
        newVehicle.vehicleLicence[sizeof(newVehicle.vehicleLicence) - 1] = '\0';
        
        // Get additional vehicle details from user
        while(true)
        {
            std::cout << "Enter the customer phone number (Length: 14 char max.): ";
            std::cin >> newVehicle.phone;
            if (strlen(newVehicle.phone) > 14)
            {
                std::cout << "Error: Phone number too long (max 14 characters)\n";
            }
            else
            {
                break;
            }

        }
        while(true)
        {
            std::cout << "Enter the length of the vehicle in meters (Range: 7.1-99.9 max): ";
            std::cin >> newVehicle.vehicleLength;
            if (newVehicle.vehicleLength < 7.1 && newVehicle.vehicleLength > 99.9)
            {
                std::cout << "Error: vehicle length is invalid (Range: 7.1-99.9 max)\n";
            }
            else
            {
                break;
            }
        }
        while(true)
        {
            std::cout << "Enter the height of the vehicle in meters (Range: 2.1-9.9m max): ";
            std::cin >> newVehicle.vehicleHeight;
            if(newVehicle.vehicleHeight < 2.1 && newVehicle.vehicleHeight > 9.9)
            {
                std::cout << "Error: vehicle height is invalid (Range: 2.1-9.9m max)\n";
            }
            else
            {
                break;
            }
        }
        
        // Write the new vehicle to file
        writeVehicle(newVehicle);
        
        std::cout << "New vehicle record created." << std::endl;
    }
    else
    {
        std::cout << "Vehicle found in system." << std::endl;
    }

}
// Function createReservation creates a reservation for the vehicle
// with the corresponding licence plate on the specified sailing
//----------------------------------------------------------------
void createReservation(char sailingID[], char vehicleLicence[]){
    char phoneNumber[14];
    float vehicleLength, vehicleHeight;

    cout << "Enter 1 to create a reservation. 0 to go back to the main menu\n";
    while(true)
    {
    cout << "Enter the licence plate of the vehicle (Length: 10 char max.):\n";
    
         std::cin >> vehicleLicence;
         if (strlen(vehicleLicence) > 10)
        {
                std::cout << "Error: vehicle height is invalid (Length: 10 char max.)\n";
        }
        else
        {
            break;
        }
        
    }
    cout << "Vehicle verified\n";
    while(true)
    {
        cout << "Enter the customer phone number (Length: 14 char max.):\n";
        std::cin >> phoneNumber;
        if (strlen(vehicleLicence) > 14)
        {
            std::cout << "Error: phone number is invalid (Length: 14 char max.)\n";
        }
        else
        {
            break;
        }
    }
    cout << "Customer verified\n";
    while(true)
    {
        cout << "Enter the length of the vehicle in meters (Range: 7.1-99.9 max):\n";
        std::cin >> vehicleLength;
        if (vehicleLength < 7.1 && vehicleLength > 99.9)
        {
            std::cout << "Error: vehicle length is invalid (Range: 7.1-99.9 max)\n";
        }
        else
        {
            break;
        }
    }
    cout << "Valid length\n";
    while(true)
    {
    cout << "Enter the height of the vehicle in meters (Range: 2.1-9.9m max):\n";
    std::cin >> vehicleHeight;
        if (vehicleHeight < 2.1 && vehicleHeight > 9.9)
        {
            std::cout << "Error: vehicle height is invalid (Range: 2.1-9.9m max)\n";
        }
        else
        {
            break;
        }
    }
    cout << "Valid height\n";

    while(true)
    {
        cout << "Enter the Sailing ID (Format: ttt-dd-hh):\n";
        std::cin >> sailingID;
        
        // Check length first (should be exactly 9 characters: 3 + 1 + 2 + 1 + 2)
        if(strlen(sailingID) != 9)
        {
            cout << "Error: ID must be exactly 9 characters (Format: ttt-dd-hh)\n";
            continue;
        }
        
        // Check format ttt-dd-hh
        bool valid = true;
        
        // Check first 3 characters are letters
        for(int i = 0; i < 3; i++)
        {
            if(!isalpha(sailingID[i]))
            {
                valid = false;
                break;
            }
        }
        
        // Check 4th character is dash
        if(valid && sailingID[3] != '-')
        {
            valid = false;
        }
        
        // Check positions 5-6 are digits (index 4-5)
        if(valid)
        {
            for(int i = 4; i < 6; i++)
            {
                if(!isdigit(sailingID[i]))
                {
                    valid = false;
                    break;
                }
            }
        }
        
        // Check 7th character is dash
        if(valid && sailingID[6] != '-')
        {
            valid = false;
        }
        
        // Check positions 8-9 are digits (index 7-8)
        if(valid)
        {
            for(int i = 7; i < 9; i++)
            {
                if(!isdigit(sailingID[i]))
                {
                    valid = false;
                    break;
                }
            }
        }
        
        if(valid)
        {
            break; // Valid format
        } else
        {
            cout << "Error: Invalid format. Please use ttt-dd-hh (3 letters, 2 digits, 2 digits)\n";
        }
    }
    // Create new reservation 
    Reservation newRes;

    strncpy(newRes.sailingID, sailingID, sizeof(newRes.sailingID) - 1);
    newRes.sailingID[sizeof(newRes.sailingID) - 1] = '\0'; 

    strncpy(newRes.vehicleLicence, vehicleLicence, sizeof(newRes.vehicleLicence) - 1);
    newRes.vehicleLicence[sizeof(newRes.vehicleLicence) - 1] = '\0';  

    newRes.onBoard = false;
    newRes.isLRL = false;

    // Add to file
    writeReservation(newRes);  
}
// Function deleteReservations with parameters sailingID, vehicleLicence
// deletes a reservation on the specified sailing
// for the vehicle with the corresponding licence plate
//----------------------------------------------------------------
void deleteReservations(char sailingID[], char vehicleLicence[])
{
    deleteReservation(sailingID, vehicleLicence);
}
// Function deleteReservations with single parameter sailingID
// deletes all reservations on the specified sailing
//----------------------------------------------------------------
void deleteReservations(char sailingID[])
{
    std::vector<Reservation> all;
    reservationReset();
    Reservation r;
    
    // Read all reservations
    while (getNextReservation(r))
    {
        all.push_back(r);
    }
    reservationClose();

    bool found = false;
    std::vector<Reservation> remaining;  // Will store reservations to keep
    
    // Filter out reservations to delete
    for (auto& rec : all)
    {
        if (std::strcmp(rec.sailingID, sailingID) == 0)
        {
            found = true;  // Mark that we found at least one matching reservation
        } else
        {
            remaining.push_back(rec);  // Keep reservations that don't match
        }
    }

    if (!found)
    {
        throw std::runtime_error(std::string("Reservation: ") + sailingID + " not found.");
    }

    // Recreate the file with only non-matching reservations
    std::remove("reservation.dat");
    reservationOpen();
    for (auto& rec : remaining)
    {
        writeReservation(rec);
    }
    reservationClose();
}

int viewReservations(char sailingID[]) 
{
    reservationReset();
    Reservation r;
    int count = 0;
    while (getNextReservation(r)) 
    {
        if (strncmp(r.sailingID, sailingID, sizeof(r.sailingID)) == 0) 
        {
            count++;
        }
    }

    return count;
}

// Function checkIn() sets the status of specified reservation as checked in
//----------------------------------------------------------------
float checkIn(char sailingID[], char vehicleLicence[])
{
    float fare = 0;
    //loops through the file, if it matches, return true, else it will automatically be false
    Reservation r;
    reservationReset();
    while (getNextReservation(r))
    {
        if (strcmp(r.sailingID, sailingID) == 0 && strcmp(r.vehicleLicence, vehicleLicence) == 0)
        {
            r.onBoard = true;
            break;
        }
    }
    if(r.isLRL == true){
        fare = 14;
        return fare;
    }
    else
    {
        // Get vehicle dimensions for non-LRL vehicles
                float length, height;
                
                std::cout << "Enter the length of the vehicle in meters (Range: 7.1-99.9 max): ";
                std::cin >> length;
                while(length < 7.1 || length > 99.9)
                {
                    std::cout << "Invalid length. Please enter between 7.1 and 99.9 meters: ";
                    std::cin >> length;
                }
                
                std::cout << "Enter the height of the vehicle in meters (Range: 2.1-9.9m max): ";
                std::cin >> height;
                while(height < 2.1 || height > 9.9)
                {
                    std::cout << "Invalid height. Please enter between 2.1 and 9.9 meters: ";
                    std::cin >> height;
                }
                
                // Calculate fare
                fare = (length * 2) + (height * 3);
                return fare;
    }
    throw std::runtime_error("Reservation not found for check in.");
    return 0; //unreachable
}