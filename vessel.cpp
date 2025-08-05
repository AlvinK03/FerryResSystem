//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============================================================
//============================================================
/*
* Filename: vessel.cpp
*
* Revision History:
* Rev. 2 - 25/08/03 Modified by L. Xu
*        - Fixed eof handling
* Rev. 1 - 25/07/20 Original by L. Xu
*
* Description: Implementation file of the Vessel module of the Ferry
* Reservation System, being the only module able
* to modify the file i/o containing information
* about vessels
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

#include "vessel.hpp"
#include <fstream>
#include <stdexcept>
#include <cstring> 

//============================================================
// Module scope static variables
//------------------------------------------------------------
static std::fstream vesselFile; // file stream for the vessel data file
static const std::string VESSELFILENAME = "vessels.dat"; // name of the vessel file

//============================================================
// Function vesselOpen creates and opens the Vessel file for binary read/write
// Takes and returns nothing
// Throws an exception if the file cannot be opened
//------------------------------------------------------------
void vesselOpen()
{
    // Try to open the vessel file without overwriting the contents
    vesselFile.open(VESSELFILENAME, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    if (!vesselFile.is_open())
    {
        // Try to create a vessel file if it does not exist
        vesselFile.clear();
        vesselFile.open(VESSELFILENAME, std::ios::out | std::ios::binary);
        
        if (!vesselFile.is_open())
        {
            // Throw an exception if the file cannot be created
            throw std::runtime_error("Cannot create " + VESSELFILENAME + ".");
        }
        vesselFile.close();

        // Try to now re-open the file for reading and writing
        vesselFile.open(VESSELFILENAME, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
        if (!vesselFile.is_open())
        {
            // Throw an exception if the file cannot be opened
            throw std::runtime_error("Cannot open " + VESSELFILENAME + ".");
        } 
    }
}

// Function vesselReset seeks to the beginning of the Vessel file
// Takes and returns nothing
// Throws an exception if the file is not open
//------------------------------------------------------------
void vesselReset()
{
    if (!vesselFile.is_open())
    {
        // Throw an exception if the file could not be opened
        throw std::runtime_error("File " + VESSELFILENAME + " is not open.");
    }
    vesselFile.clear();
    vesselFile.seekg(0, std::ios::beg); // Set get position to the start of the file
    
}

// Function getNextVessel binary reads a line from the Vessel file
// Returns a boolean if retrieving all the data was successful
// Takes a Vessel object
// Throws an exception if the binary read operation fails
//------------------------------------------------------------
bool getNextVessel(Vessel& v)
{
    if (!vesselFile.is_open())
    {
        // Throw an exception if the file is not open
        throw std::runtime_error("File " + VESSELFILENAME + " is not open.");
    }

    vesselFile.clear();
    // Read information of the next vessel object in the file
    vesselFile.read(reinterpret_cast<char *>(&v), sizeof(Vessel));
    
    if (vesselFile.eof())
    {
        // Return false if there is no more data to read 
        return false;
    }

    vesselFile.flush();

    if (!vesselFile)
    {
        // Throw an exception if the file could not be read from
        throw std::runtime_error("Error reading from file " + VESSELFILENAME + ".");
    }

    return true;
}

// Function writeVessel binary writes to the Vessel file
// Returns nothing
// Takes a Vessel object
// Throws an exception if the binary write operation fails
//------------------------------------------------------------
void writeVessel(const Vessel& v)
{
    if (!vesselFile.is_open())
    {
        // Throw an exception if the file is not open
        throw std::runtime_error("File " + VESSELFILENAME + " is not open.");
    }

    // Write information of the vessel object at the end 
    vesselFile.clear();
    vesselFile.seekp(0, std::ios::end); // Move to the end of the file
    vesselFile.write(reinterpret_cast<const char *>(&v), sizeof(Vessel));
    
    if (!vesselFile)
    {
        // Throw an exception if the file could not be written to
        throw std::runtime_error("Error writing to file " + VESSELFILENAME + ".");
    }
}

// Function vesselClose closes the Vessel file
// Takes and returns nothing
// Throws an exception if the file was already closed
//------------------------------------------------------------
void vesselClose()
{
    if (vesselFile.is_open())
    {
        vesselFile.close();
    }
    else
    {
        // Throw an exception if the file was already closed
        throw std::runtime_error("File " + VESSELFILENAME + " was already closed.");
    }
}