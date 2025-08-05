//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============================================================
//============================================================
/*
 * Filename: sailingManager.cpp
 *
 * Revision History:
 * Rev. 1 - 25/07/23 Original by C. Wen
 *
 * Description: sailingManager.cpp is the control module for
 * managing sailing componets within Coastal Ferry Reservation
 * System. It provides the interface logic for users to create,
 * update, query and delete for sailings.
 * This module performs high level validation and user input
 * handling, while delegating low level operation to other modules
 * Desgin considerations:
 * Use fixed length binary storage in sailing.cpp
 * Use Char[] for vessel and sailing ID for compatibility
 * Must have fixed length binary format
 * Advoid direct file manipulation
*/
//============================================================
#include "sailingManager.hpp"
#include "vessel.hpp"            
#include "sailing.hpp"
#include "vehicle.hpp"
#include "reservation.hpp"
#include "reservationManager.hpp"
#include <vector>
#include <string>
#include <cstring>              
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdio> 
#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <iomanip>

//================================================================

// Function getVessel displays all available vessels for sailings
// and prompts the user to select a vessel
// Returns a string containing the user selected vessel's name
// Throws an exception if no vessels exist
//----------------------------------------------------------------
char* getVessel()
{
    vesselReset();
    Vessel vessel;
    std::vector<std::string> names;
    std::cout << "\nVessels:\n";
    while (getNextVessel(vessel))
    {
        names.emplace_back(vessel.name);
    }
    if (names.empty())
    {
        throw std::runtime_error("getVessel: No avaliable vessels.");
    }
    // display information
    static char vesselName[26]; 
    // vessel name 25 characters
    int pageSize = 5;
    int page = 0;
    int total = static_cast<int>(names.size());
    std::string input;

    while (true)
    {
        int start = page * pageSize;
        int end = std::min(start + pageSize, total);
        int count = end - start;

        // display header
        std::cout << "\nList of Vessels:\n";
        std::cout << "===============\n";
        //pages 
        for (int i = 0; i < count; ++i)
        {
            std::cout << std::setw(2) << (i + 1) << ") "<<names[start + i]<<"\n";
        }
        // if there is more informaiton, can dispaly another page
        if (end < total)
        {
            std::cout << std::setw(2) << (pageSize + 1) << ") Display More\n";
        }
        std::cout << std::setw(2) << 0 << ") Quit\n";
        std::cout << "Select an option [0-" << (end<total ? pageSize : count) << "] or enter vessel name: ";
        std::cin >> input;

        // if user selects numeric choice
        bool allDigits = !input.empty() && std::all_of(input.begin(), input.end(), [](unsigned char c) 
        {
            return std::isdigit(c) != 0;
        });
        if (allDigits)
        {
            int choice = std::stoi(input);
            if (choice == 0)
            {
                throw std::runtime_error("getVessel: User cancelled.");
            }
            if (choice == pageSize && end < total)
            {
                ++page;
                std::cin.clear();
                continue;
            }
            if (choice >= 1 && choice <= count)
            {
                std::strncpy(vesselName, names[start + choice - 1].c_str(),sizeof(vesselName) - 1);
                vesselName[sizeof(vesselName) - 1] = '\0';
                return vesselName;
            }
        }
        for (auto &nm : names)
        {
            if (nm == input)
            {
                std::strncpy(vesselName, nm.c_str(),sizeof(vesselName) - 1);
                vesselName[sizeof(vesselName) - 1] = '\0';
                return vesselName;
            }
        }
        std::cout << "Error: Invalid vessel name or number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}


// Function getVesselLength
// Returns the total lane length of the specified vessel (irrespective of high/low) 
// as an int value
// Throws an exception if vessel does not exist
//----------------------------------------------------------------
int getVesselLength(char vesselName[])
{
    vesselReset();
    Vessel vessel;
    while (getNextVessel(vessel))
    {
        if (std::strcmp(vesselName, vessel.name) == 0)
        {
            return static_cast<int>(vessel.HCLL + vessel.LCLL);
        }
    }
    throw std::runtime_error(std::string("getVesselLength: ") + vesselName + " not found.");
}

// Function checkSailingExists checks if a sailing with the provided 
// sailing ID exists.
// Returns 1 if sailing exists, otherwise throw exception
//----------------------------------------------------------------
int sailingManagerExists(char sailingID[])
{
    checkSailingExists(sailingID);
    return 1;
}

// Function accessSailingManagerUpdate accesses the Reservation Manager module
// to view the total number reservations on
// prints total reservations on sailing
//----------------------------------------------------------------
void accessReservationManager(char sailingID[])
{
    int count = viewReservations(sailingID);
    std::cout << "Total reservations on " << sailingID << ": " << count << "\n";
} 

// Function createSailing creates a sailing on a vessel
// Throws an exception if a vessel with the corresponding name already exists
//----------------------------------------------------------------
void createSailing(char vesselName[])
{
    // total capacity and if vessel exists and ask user for id
    
    
    char sailingID[10];

    while(true)
    {
        std::cout << "Enter the Sailing ID (Format: ttt-dd-hh):\n";
        cout << "If you like to exit, press 0\n";
        std::cin >> sailingID;
        if(strcmp(sailingID, "0") == 0)
        {
            return;
        }
        // Check length first (should be exactly 9 characters: 3 + 1 + 2 + 1 + 2)
        if(strlen(sailingID) != 9)
        {
            cout << "Error: ID must be exactly 9 characters (Format: ttt-dd-hh)\n";

        }
        
        // Check format ttt-dd-hh
        bool valid = true;
        
        // Check first 3 characters are letters
        for(int i = 0; i < 3; i++)
        {
            if(!isalpha(sailingID[i]))
            {
                valid = false;
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

    Vessel temp;
    vesselReset();
    // Find the correct vessel record
    while (true)
    {
        getNextVessel(temp);
        if (std::strcmp(temp.name, vesselName) == 0)
        {
            break;
        }
    }
    //check uniqueness
    try
    {
        checkSailingExists(sailingID);
        throw std::runtime_error("createSailing: ID already exists.");
    }
    catch (const std::runtime_error&)
    {                                         
    }

    // build record name length lrl hrl
    Sailing s;
    std::strncpy(s.vesselName, vesselName, sizeof(s.vesselName) - 1);
    std::strncpy(s.sailingID, sailingID, sizeof(s.sailingID) - 1);
    s.lowRemainingLength = temp.LCLL;
    s.highRemainingLength = temp.HCLL;

    //call write sailing
    writeSailing(s);
    std::cout << "Created sailing " << sailingID << " on vessel " << vesselName << ".\n";
}

// Function updateSailing updates the total space available on a sailing
// vehicleLen is the length of the vehicle being added/removed, measured in meters
// Throws an exception if space on sailing cannot be added to/subtracted from,
// in the case that the sailing is full or empty respectively
//----------------------------------------------------------------
void updateSailing(char sailingID[], int vehicleLen)
{
    std::vector<Sailing> all;
    sailingReset();
    Sailing s;
    while (getNextSailing(s)) all.push_back(s);

    bool found = false;
    for (auto& rec : all)
    {
        if (std::strcmp(rec.sailingID, sailingID)==0)
        {
            if (rec.lowRemainingLength < vehicleLen)
            {
                throw std::runtime_error("updateSailing: Not enough low lane space.");
            }
            rec.lowRemainingLength -= vehicleLen;
            rec.highRemainingLength += vehicleLen;
            found = true;
            break;
        }
    }
    if (!found)
    {
        throw std::runtime_error(std::string("updateSailing: ") + sailingID + " not found.");
    }
    sailingClose();
    std::remove("sailings.dat");
    sailingOpen();
    for (auto & rec : all)
    {
        writeSailing(rec);
    }
    std::cout << "Updated sailing " << sailingID << ".\n";
}

// Function checkInReservation calculates and prompts user to collect the appropriate
// fare from the customer, then calls the appropriate functions in the 
// ReservationManager module to register the reservation as checked in
// Throw an exception if vehicleLicence is invalid
//----------------------------------------------------------------
void checkInReservation(char sailingID[], char vehicleLicence[])
{
    float fare = checkIn(sailingID, vehicleLicence);
    std::cout<<"Collect fare: $"<< fare << "\nConfirm payment [Y/N]: ";
    char c; std::cin>> c;
    if (std::toupper(c) != 'Y')
    {
        throw std::runtime_error("checkInReservation: Payment not confirmed.");
    }
    std::cout<<"Reservation checked in.\n";
}

void printSailingInfo(char sailingID[])
{
    Sailing tempSailing;
    Reservation tempRes;
    Vehicle tempVehicle;
    sailingReset();
    reservationReset();
    vehicleReset();
    int numEntries = 5; // default number of reservations to display
    int i = 1;
    char isSpecial;
    char isOnboard;
    int userInput;

    // search for provided sailing
    while (getNextSailing(tempSailing))
    {
        if (strncmp(tempSailing.sailingID, sailingID, sizeof(tempSailing.sailingID)) == 0)
        {
            break;
        }
    }
    cout << "Information about the sailing: " << endl;
    cout << "\tSailing ID: " << sailingID << endl;
    cout << "\tLow Remaining Length (LRL): " << tempSailing.lowRemainingLength << "m" << endl;
    cout << "\tHigh Remaining Length (HRL): " << tempSailing.highRemainingLength << "m" << endl;
    cout << "\tDay of Departure: " << sailingID[4] << sailingID[5] << endl;
    cout << "\tHour of Departure: " << sailingID[7] << sailingID[8] << endl;
    cout << "\tDeparture Terminal: " << sailingID[0] << sailingID[1] << sailingID[2] << endl;
    cout << "\tVessel Name: " << tempSailing.vesselName << endl << endl;
    cout << "List of Reservations" << endl;
    cout << "================" << endl;
    cout << std::left
         << std::setw(16) << "  Licence #"
         << std::setw(18) << "Phone #"
         << std::setw(12) << "Length(m)"
         << std::setw(12) << "Special?"
         << std::setw(12) << "Onboard?" << endl;
    // keep printing out entries until there are enough    
    while (i <= numEntries)
    {
        // found a reservation with matching sailingID
        if (strncmp(tempRes.sailingID, sailingID, sizeof(tempRes.sailingID)) == 0)
        {
            // search vehicles.dat for relevant vehicle
            while (getNextVehicle(tempVehicle))
            {
                if (strncmp(tempVehicle.vehicleLicence, tempRes.vehicleLicence, sizeof(tempVehicle)) == 0)
                {
                    break;
                }
            }
            // check if onboard
            if (tempRes.isLRL == true)
            {
                isSpecial = 'N';
            }
            else
            {
                isSpecial = 'Y';
            }
            // check if special vehicle
            if (tempRes.onBoard == true)
            {
                isOnboard = (char) 'Y';
            }
            else
            {
                isOnboard = (char) 'N';
            }

            cout << std::left
                 << i << ") " 
                 << std::setw(13) << tempRes.vehicleLicence
                 << std::setw(18) << tempVehicle.phone
                 << std::setw(12) << tempVehicle.vehicleLength
                 << std::setw(12) << isSpecial
                 << std::setw(12) << isOnboard << endl;
            i++;
        }
        // if there are no more reservations to show for said sailing
        if (!getNextReservation(tempRes))
        {
            cout << "No more reservations to display" << endl;
            break;
        }
        // upon printing numEntries amt of reservations, prompt user to either
        // print more or quit
        if (i > numEntries)
        {
            cout << std::setw(12) << i << ") Display More" << endl;
            cout << std::setw(12) << "0) Quit" << endl;
            cout << "Select an option [0/" << i << "] and press ENTER:" << endl;
            std::cin >> userInput;
            if (userInput == i)
            {
                numEntries += 5;
            }
            else if (userInput == 0)
            {
                break;
            }
            else
            {
                cout << "Please select a valid option" << endl;
            }

        }
    }
    

}

// Function querySailing displays all available sailings,
// and prompts the user to select a sailing
// Displays information on the sailing and 
// returns a string containing the user selected sailingID
//----------------------------------------------------------------
char* querySailing()
{
    sailingReset();
    Sailing s;
    std::vector<std::string> ids;
    static char sailingID[10]; //9 characters for id, 1 buffer
    std::cout<<"\nAvailable sailings:\n";

    while(getNextSailing(s))
    {
        ids.emplace_back(s.sailingID);
        std::cout << ids.size() << ") "
                        << s.sailingID << " on " << s.vesselName
                        << "  LRL=" << s.lowRemainingLength
                        << "  HRL=" << s.highRemainingLength << "\n";
    }
    if (ids.empty())
    {
        throw std::runtime_error("querySailing: No available sailings.");
    }
    int userInput = 0;
    while (true)
    {
        std::cout<< "Select sailing [1-"<<ids.size() << "]" << std::endl;
        if (std::cin >> userInput && userInput>=1 && userInput <= (int)ids.size())
        {
            strncpy(sailingID, ids[userInput - 1].c_str(), sizeof(sailingID) - 1);
            sailingID[sizeof(sailingID) - 1] = '\0';
            printSailingInfo(sailingID);
            break;
        }
        std::cout << "Invalid. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return sailingID;
}

// Function removeReservations calls the appropriate functions in the 
// ReservationManager module to remove all reservations from a sailing
//----------------------------------------------------------------
void removeReservations(char sailingID[])
{
    deleteReservations(sailingID);
    deleteSailing(sailingID);
    std::cout<<"Removed all reservations on "<< sailingID <<".\n";
} 

// Function printSailingReport sends a sailing report to a printer to be printed
void printSailingReport(char printerName[])
{
    Sailing tempSailing;
    Vessel tempVessel;
    sailingReset();
    vesselReset();
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);
    char date_str[9];
    std::strftime(date_str, sizeof(date_str), "%y/%m/%d", local_time);
    std::cout << "Printing report to " << printerName <<"...\n" << endl;
    std::cout << "Date of Sailing Report Request: " << date_str << std::endl;
    std::cout << std::left 
              << std::setw(12) << "Sailing ID"
              << std::setw(28) << "Vessel Name"
              << std::setw(10) << "LRL(m)"
              << std::setw(10) << "HRL(m)"
              << std::setw(12) << "#Vehicles"
              << std::setw(12) << "LenFull(%)" << std::endl;
    // print a line for every sailing
    while (getNextSailing(tempSailing))
    {
        // calculate the percent of the total lane length full
        float vesselTtlLen = (float)getVesselLength(tempSailing.vesselName);
        float vesselTtlRmngLen = tempSailing.lowRemainingLength + tempSailing.highRemainingLength;
        float percentLenFull = (vesselTtlRmngLen/vesselTtlLen) * 100;
        std::cout << std::left << std::fixed << std::setprecision(1)
            << std::setw(12) << tempSailing.sailingID 
            << std::setw(28) << tempSailing.vesselName
            << std::setw(10) << tempSailing.lowRemainingLength 
            << std::setw(10) << tempSailing.highRemainingLength
            << std::setw(12) << viewReservations(tempSailing.sailingID)
            << std::setw(12) << percentLenFull;
    }
}