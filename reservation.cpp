//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============================================================
//============================================================
/*
* Filename: reservation.cpp
*
* Revision History:
* Rev. 1 - 25/07/23 Original by A. Chung
* Rev. 2 - 25/08/04 Fixed deleteReservation to properly overwrite by L. Xu
*
* Description: Implementation module of the Ferry Reservation System,
* checks if information already exists for reservation and in some cases
* deletes. This is the only module able to modify thee file i/o containing
* information about reservations.
* Is a data storage only module using fixed-length binary records
* All I/O operations allow for fast random access
* Should close and open the file once
* Should call the init() function before any
* operations
* 
* Design Issues: Using linear search for all traversal and deletions
* Must be on a system able to use fstream
* Fixed-length records may waste space
*/
//================================================================

#include "reservation.hpp"
#include "sailing.hpp"
#include "vehicle.hpp"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cctype>
#include <cstdio>
#ifdef _WIN32
  #include <io.h>      
#else
  #include <unistd.h>  
  #include <fcntl.h>
#endif

//============================================================
// Module scope static variables
//------------------------------------------------------------v
static std::fstream reservationFile;
static const std::string RESERVATIONFILENAME = "reservations.dat";
//================================================================

// Function creates and opens reservation file.
// Throw an exception if it cannot be opened.
//----------------------------------------------------------------
void reservationOpen()
{

     // Try to open the reservation file without overwriting the contents
    reservationFile.open(RESERVATIONFILENAME, std::ios::in | std::ios::out | std::ios::binary);
    if (!reservationFile.is_open())
    {
        // Try to create a reservation file if it does not exist
        reservationFile.clear();
        reservationFile.open(RESERVATIONFILENAME, std::ios::out | std::ios::binary);
        
        if (!reservationFile.is_open())
        {
            // Throw an exception if the file cannot be created
            throw std::runtime_error("Cannot create " + RESERVATIONFILENAME + ".");
        }
        reservationFile.close();

        // Try to now re-open the file for reading and writing
        reservationFile.open(RESERVATIONFILENAME, std::ios::in | std::ios::out | std::ios::binary);
        if (!reservationFile.is_open())
        {
            // Throw an exception if the file cannot be opened
            throw std::runtime_error("Cannot open " + RESERVATIONFILENAME + ".");
        } 
    }
}

// Function resets to the beginning of the list.
// Throw an exception if it cannot be opened.
//----------------------------------------------------------------
void reservationReset()
{
    if (!reservationFile.is_open())
    {
        // Throw an exception if the file could not be opened
        throw std::runtime_error("File " + RESERVATIONFILENAME + " is not open.");
    }
    reservationFile.clear();
    reservationFile.seekg(0, std::ios::beg); // Set get position to the start of the file
}

// Function getNextReservation returns a line from the data
// Returns a boolean if the data is successfully read
// Throws an exception if there is an error with reading the files
//----------------------------------------------------------------
bool getNextReservation(Reservation& r)
{
    if (!reservationFile.is_open())
    {
        // Throw an exception if the file is not open
        throw std::runtime_error("File " + RESERVATIONFILENAME + " is not open.");
    }

    reservationFile.clear();
    // Read information of the next reservation object in the file
    reservationFile.read(reinterpret_cast<char *>(&r), sizeof(Reservation));

    if (reservationFile.eof())
    {
        // Return false if there is no more data to read
        return false;
    }

    reservationFile.flush();

    if (!reservationFile)
    {
        // Throw an exception if the file could not be read from
        throw std::runtime_error("Error reading from file " + RESERVATIONFILENAME + ".");
    }

    return true;

}
// Function writeReservation writes to reservation file
// Throws an exception if it fails
// Returns true if more reservations exist, false if we've shown all
//----------------------------------------------------------------
void writeReservation(const Reservation& r, bool overWrite)
{
    //throw exception if file not opened
   if (!reservationFile.is_open())
    {
        // Throw an exception if the file is not open
        throw std::runtime_error("File " + RESERVATIONFILENAME + " is not open.");
    }
    // Write information of the reservation object 
    reservationFile.clear();

    // Write to the end if not overwriting, skip if overwriting
    if (!overWrite)
    {
        
        reservationFile.seekp(0, std::ios::end); // Move to the end of the file
    }
    reservationFile.write(reinterpret_cast<const char *>(&r), sizeof(Reservation));
    
    if (!reservationFile)
    {
        // Throw an exception if the file could not be written to
        throw std::runtime_error("Error writing to file " + RESERVATIONFILENAME + ".");
    }
    reservationFile.flush();
}

// Function closes reservation file
//----------------------------------------------------------------
void reservationClose()
{

     if (reservationFile.is_open())
    {
        reservationFile.close();
    }
    else
    {
        // Throw an exception if the file was already closed
        throw std::runtime_error("File " + RESERVATIONFILENAME + " was already closed.");
    }
}

// Function deleteReservation deletes a reservation with the provided
// sailingID and vehicleLicence. Throws an exception if the record is not found.
//----------------------------------------------------------------
void deleteReservation(char sailingID[], char vehicleLicence[])
{

    // Throw an exception if the file is not open
    if (!reservationFile.is_open()) 
    {
        throw std::runtime_error("deleteReservation: File not open.");
    }
    
    // Get total records
    reservationFile.clear();
    reservationFile.seekg(0, std::ios::end);
    std::streampos size = reservationFile.tellg();
    int total = static_cast<int>(size / sizeof(Reservation));
    // Throw an exception if the file is empty
    if (total == 0)
    {
        throw std::runtime_error("deleteReservation: No records to delete");
    }

    // Find target index (checking BOTH sailingID AND vehicleLicence)
    reservationFile.seekg(0, std::ios::beg);
    int target = -1;
    Reservation temp;
    Reservation lastRecord;
    
    // Find the reservation with the correct sailingID and vehicleLicence
    for (int i = 0; i < total; ++i) 
    {
        
        getNextReservation(temp);
        if (std::strncmp(temp.sailingID, sailingID, sizeof(temp.sailingID)) == 0 &&
            std::strncmp(temp.vehicleLicence, vehicleLicence, sizeof(temp.vehicleLicence)) == 0) 
        {
            target = i;
            break;
        }
    }
    
    
    // If the reservation was not found throw an exception
    if (target < 0) 
    {
        throw std::runtime_error(std::string("deleteReservation: Reservation with sailingID '") + 
                               sailingID + "' and vehicleLicence '" + vehicleLicence + "' not found");
    }

    // Get last record
    reservationFile.seekg((total - 1) * sizeof(Reservation), std::ios::beg);
    getNextReservation(lastRecord);
    if (reservationFile.fail()) 
    {
        throw std::runtime_error("deleteReservation: Failed reading last record");
    }
    
    // Overwrite target slot with last record
    reservationFile.seekp(target * sizeof(Reservation), std::ios::beg);
    writeReservation(lastRecord, true);
    // Throw an exception if the overwriting failed
    if (reservationFile.fail()) 
    {
        throw std::runtime_error("deleteReservation: Overwrite failed");
    }

    Sailing s;
    Vehicle v;
    sailingReset();
    vehicleReset();
    while (true){
        if (!getNextSailing(s))
        {
            throw std::runtime_error("Failed getting sailing");
        }
        if (std::strcmp(s.sailingID, sailingID) == 0)
        {
            while (true)
            {
                if (!getNextVehicle(v))
                {
                    throw std::runtime_error("Failed getting vehicle information for cancellation");
                }
                if (std::strcmp(v.vehicleLicence, vehicleLicence) == 0)
                {
                    break;
                }
            }
            
            if (v.vehicleHeight <= 2)
            {
                deleteSailing(s.sailingID);
                s.lowRemainingLength += v.vehicleLength;
                writeSailing(s);
            }
            else
            {
                deleteSailing(s.sailingID);
                s.highRemainingLength += v.vehicleLength;
                writeSailing(s);
            }
            
            break;
        }

    }
    


    // Truncate file (platform-specific)
#ifdef _WIN32
    {
        reservationFile.flush();
        reservationFile.close();
        FILE* f = std::fopen(RESERVATIONFILENAME.c_str(), "r+b");
        if (!f) 
        {
            throw std::runtime_error("deleteReservation: file open failed");
        }
        int fd = _fileno(f);
        long newSize = static_cast<long>((total - 1) * sizeof(Reservation));
        if (_chsize_s(fd, newSize) != 0) 
        {
            std::fclose(f);
            throw std::runtime_error("deleteReservation: truncate failed");
        }
        std::fclose(f);
    }
#else
    {
        reservationFile.close();
        int fd = ::open(RESERVATIONFILENAME.c_str(), O_RDWR);
        if (fd < 0) 
        {
            throw std::runtime_error("deleteReservation: open failed");
        }
        off_t newSize = static_cast<off_t>((total - 1) * sizeof(Reservation));
        if (ftruncate(fd, newSize) != 0) 
        {
            ::close(fd);
            throw std::runtime_error("deleteReservation: truncate failed");
        }
        ::close(fd);
    }
#endif

    // Reopen file
    reservationFile.open(RESERVATIONFILENAME,
                       std::ios::in | std::ios::out | std::ios::binary);
    if (!reservationFile.is_open()) {
        throw std::runtime_error("deleteReservation: re-open failed");
    }
}