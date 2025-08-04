//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============================================================
//============================================================
/*
* Filename: vehicle.cpp
*
* Revision History:
* Rev. 1 - 25/07/20 Original by L. Xu
* Rev. 2 - 25/08/03 Fixed eof handling by L. Xu
*
* Description: Implementation file of the Vehicle module of the Ferry
* Reservation System, being the only module able
* to modify the file i/o containing information
* about vehicles
* Is a data storage only module using fixed-length binary records
* All I/O operations allow for fast random access
* Should close and open the file once
* Should call the init() function before any
* operations
* 
* Design Issues: Using linear search for the data file
* Must be on a system able to use fstream
* Fixed-length records may waste space
*/
//============================================================

#include "vehicle.hpp"
#include <fstream>
#include <stdexcept>
#include <cstring> 

//============================================================
// Module scope static variables
//------------------------------------------------------------
static std::fstream vehicleFile; // file stream for the vehicle data file
static const std::string VEHICLEFILENAME = "vehicles.dat"; // name of the vessel file

//============================================================
// Function vehicleOpen creates and opens the Vehicle file for binary read/write
// Takes and returns nothing
// Throws an exception if the file cannot be opened
//------------------------------------------------------------
void vehicleOpen()
{
    // Try to open the vehicle file without overwriting the contents
    vehicleFile.open(VEHICLEFILENAME, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    if (!vehicleFile.is_open())
    {
        // Try to create a vehicle file if it does not exist
        vehicleFile.clear();
        vehicleFile.open(VEHICLEFILENAME, std::ios::out | std::ios::binary);
        
        if (!vehicleFile.is_open())
        {
            // Throw an exception if the file cannot be created
            throw std::runtime_error("Cannot create " + VEHICLEFILENAME + ".");
        }
        vehicleFile.close();

        // Try to now re-open the file for reading and writing
        vehicleFile.open(VEHICLEFILENAME, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
        if (!vehicleFile.is_open())
        {
            // Throw an exception if the file cannot be opened
            throw std::runtime_error("Cannot open " + VEHICLEFILENAME + ".");
        } 
    }
}

// Function vehicleReset seeks to the beginning of the Vehicle file
// Takes and returns nothing
// Throws an exception if the file is not open
//------------------------------------------------------------
void vehicleReset()
{
    if (!vehicleFile.is_open())
    {
        // Throw an exception if the file could not be opened
        throw std::runtime_error("File " + VEHICLEFILENAME + " is not open.");
    }
    vehicleFile.clear();
    vehicleFile.seekg(0, std::ios::beg); // Set get position to the start of the file
    
}

// Function getNextVehicle binary reads a line from the Vehicle file
// Returns a boolean if retrieving all the data was successful
// Takes a Vehicle object
// Throws an exception if the binary read operation fails
//------------------------------------------------------------
bool getNextVehicle(Vehicle& v)
{
    if (!vehicleFile.is_open())
    {
        // Throw an exception if the file is not open
        throw std::runtime_error("File " + VEHICLEFILENAME + " is not open.");
    }

    vehicleFile.clear();
    // Read information of the next vehicle object in the file
    vehicleFile.read(reinterpret_cast<char *>(&v), sizeof(Vehicle));

    if (vehicleFile.eof())
    {
        // Return false if there is no more data to read
        return false;
    }

    vehicleFile.flush();

    if (!vehicleFile)
    {
        // Throw an exception if the file could not be read from
        throw std::runtime_error("Error reading from file " + VEHICLEFILENAME + ".");
    }

    return true;
}

// Function writeVehicle binary writes to the Vehicle file
// Returns nothing
// Takes a Vehicle object
// Throws an exception if the binary write operation fails
//------------------------------------------------------------
void writeVehicle(const Vehicle& v)
{
    if (!vehicleFile.is_open())
    {
        // Throw an exception if the file is not open
        throw std::runtime_error("File " + VEHICLEFILENAME + " is not open.");
    }

    // Write information of the vehicle object at the end 
    vehicleFile.clear();
    vehicleFile.seekp(0, std::ios::end); // Move to the end of the file
    vehicleFile.write(reinterpret_cast<const char *>(&v), sizeof(Vehicle));
    
    if (!vehicleFile)
    {
        // Throw an exception if the file could not be written to
        throw std::runtime_error("Error writing to file " + VEHICLEFILENAME + ".");
    }
}

// Function close closes the Vehicle file
// Takes and returns nothing
// Throws an exception if the file was already closed
//------------------------------------------------------------
void vehicleClose()
{
    if (vehicleFile.is_open())
    {
        vehicleFile.close();
    }
    else
    {
        // Throw an exception if the file was already closed
        throw std::runtime_error("File " + VEHICLEFILENAME + " was already closed.");
    }
}