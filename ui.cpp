//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//================================================================
//================================================================

/*
 * Filename: ui.cpp
 * 
 * Revision History: 
 * Rev. 1 - 25/07/21 Original by A. Kong
 *
 * Description: UI of the Ferry Reservation System,
 *              displays the menu to user, takes commands.
 *              
 */

//================================================================

#include <string>
#include <iostream>
#include "ui.hpp"

// different submenus user can be in, start at main menu
enum menu{mainMenu, sailingMenu, reservationMenu, exitProgram};
enum menu currentMenu = mainMenu;

void sailingManagement()
{
    currentMenu = sailingMenu;
    return;
}

void reservation()
{
    currentMenu = reservationMenu;
    return;
}

void createVessel()
{
    Vessel userVessel;
    std::cout << "Please enter a valid vessel name (max 20 char.)" << std::endl;
    cin >> userVessel.name;
    std::cout << "Please enter the low ceiling lane length of the vessel" << std::endl;
    cin >> userVessel.LCLL;
    std::cout << "Please enter the high ceiling lane length of the vessel" << std::endl;
    cin >> userVessel.HCLL;
    writeVessel(userVessel);
}

// Function takes user input and takes an action
// depending on which menu the user is in
void processInput()
{
    // variable initialization
    int userInput;
    char* printerName;
    char sailingID[10];
    char vehicleLicence[11];
    char vesselName[26];
    std::cout << "Enter choice: " << std::endl;
    std::cin >> userInput;

    switch(currentMenu)
    {
    case mainMenu:
        switch (userInput)
        {
        // access reservation submenu
        case 1:
            reservation();
            break;
        // access sailing submenu
        case 2:
            sailingManagement();
            break;
        // create a new vessel
        case 3:
            createVessel();
            break;
        // exitProgram program
        case 4:
            currentMenu = exitProgram;
        // invalid user input
        default:
            std::cout << "Please select a valid option" << std::endl;
            break;
        }
        break;
    case reservationMenu:
        switch (userInput)
        {
        // create a reservation
        case 1:
            std::cout << "Please enter a valid sailing ID" << std::endl;
            std::cin >> sailingID;
            std::cout << "Please enter the vehicle's licence plate" << std::endl;
            std::cin >> vehicleLicence;
            createReservation(sailingID, vehicleLicence);
            break;
        // delete a reservation
        case 2:
            std::cout << "Please enter a sailing ID" << std::endl;
            std::cin >> sailingID;
            std::cout << "Please enter the vehicle's licence plate" << std::endl;
            std::cin >> vehicleLicence;
            deleteReservations(sailingID, vehicleLicence);
            break;
        // return to main menu
        case 3:
            currentMenu = mainMenu;
        // invalid user input
        default:
            std::cout << "Please select a valid option" << std::endl;
            break;
        }
        break;
    case sailingMenu:
        switch (userInput)
        {
        // customer check in 
        case 1:
            std::cout << "Please enter a valid sailing ID" << std::endl;
            std::cin >> sailingID;
            std::cout << "Please enter the vehicle's licence plate" << std::endl;
            std::cin >> vehicleLicence;
            checkInReservation(sailingID, vehicleLicence);
            break;
        // create sailing
        case 2:
            std::cout << "Please enter a valid vessel name" << std::endl;
            std::cin >> vesselName;
            createSailing(vesselName);
            break;
        // query sailing
        case 3:
            querySailing();
            break;
        // delete sailing
        case 4:
            removeReservations(querySailing());
            break;
        // print sailing report
        case 5:
            cout << "Please enter the name"
                << " of the desired printing location." << std::endl;
            cin >> printerName;
            printSailingReport(printerName);
            break;
        // return to main menu
        case 6:
            currentMenu = mainMenu;
            break;
        // invalid user input
        default:
            std::cout << "Please select a valid option" << std::endl;
        }
        break;
    case exitProgram:
        break;
    }
    return;
}

// Function displays the appropriate menu to the user depending on currentMenu
void displayCurrentMenu()
{
    std::cout << "Welcome to the Ferry Reservation System!" << std::endl;
    // display menus and take input until user decides to exit
    while (currentMenu != exitProgram)
    {
        switch(currentMenu)
        {
        case mainMenu:
            std::cout << "\n=== Main Menu ===\n"
                << "1. Reservation Submenu\n"
                << "2. Sailing Submenu\n"
                << "3. Create Vessel\n"
                << "4. Exit" << std::endl;
            processInput();
            break;
        case reservationMenu:
            std::cout << "\n=== Reservation Menu ===\n"
                << "1. Create Reservation\n"
                << "2. Delete Reservation\n"
                << "3. Return to Main Menu" << std::endl;
            processInput();
            break;
        case sailingMenu:
            std::cout << "\n=== Sailing Menu ===\n"
                << "1. Check In\n"
                << "2. Create Sailing\n"
                << "3. Query Sailing\n"
                << "4. Delete Sailing\n"
                << "5. Print Sailing Report\n"
                << "6. Return to Main Menu" << std::endl;
            processInput();
            break;
        }
    }
    currentMenu = mainMenu;
    std::cout << "Exiting Ferry Reservation System. Goodbye!" << std::endl;
    return;
}