//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//================================================================
//================================================================
/*
 * Filename: sailing.cpp
 * Revision History:
 * Rev. 1 - 25/07/23 Original by C. Wen
 * Rev. 2 - 25/08/04 Fixed coding convention by L. Xu
 *
 * Description: Implementation file of the sailing moduel of the Ferry
 * Reservation System, being the only module able
 * to modify the file i/o containing information
 * about sailing
 * Is a data storage only module using fixed-length binary records
 * All I/O operations allow for fast random access
 * Should close and open the file once
 * Should call the init() function before any
 * operations
 * Design Issues: Using linear locate sailing records
 * Must be on a system able to use fstream
 * Fixed-length records may waste space
 */

//================================================================
#include "sailing.hpp"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>
#ifdef _WIN32
	#include <io.h>      
#else
	#include <unistd.h>  
	#include <fcntl.h>
#endif

//============================================================
// Module scope static variables
//------------------------------------------------------------
static std::fstream sailingFile;
static const std::string SAILINGFILENAME = "sailings.dat";

//================================================================

// Function open creates and opens the Sailing file
// Throws an exception if the file cannot be opened
//----------------------------------------------------------------
void sailingOpen()
{
	// Try to open the sailing file without overwriting the contents
	sailingFile.open(SAILINGFILENAME, std::ios::in | std::ios::out | std::ios::binary);
	if (!sailingFile.is_open())
	{
		// Try to create a sailing file if it does not exist
		sailingFile.clear();
		sailingFile.open(SAILINGFILENAME, std::ios::out | std::ios::binary);
		
		if (!sailingFile.is_open())
		{
			// Throw an exception if the file cannot be created
			throw std::runtime_error("Cannot create " + SAILINGFILENAME);
		}
		sailingFile.close();

		// Try to now re-open the file for reading and writing
		sailingFile.open(SAILINGFILENAME, std::ios::in | std::ios::out | std::ios::binary);
		if (!sailingFile.is_open())
		{
			// Throw an exception if the file cannot be opened
			throw std::runtime_error("Cannot open " + SAILINGFILENAME);
		} 
	}
}

// Function close closes the Sailing file
//----------------------------------------------------------------
void sailingClose()
{
	if (sailingFile.is_open())
    {
        sailingFile.close();
    }
    else
    {
        // Throw an exception if the file was already closed
        throw std::runtime_error("File " + SAILINGFILENAME + " was already closed.");
    }
}

// Function reset seeks to the beginning of the Sailing file
// Throws an exception if the file is not open
//----------------------------------------------------------------
void sailingReset()
{
	if (!sailingFile.is_open())
	{
		throw std::runtime_error("Reset: " + SAILINGFILENAME + " File not open.");
	}
	sailingFile.clear();
	sailingFile.seekg(0, std::ios::beg); // Set get position to the start of the file
}

// Function getNextSailing obtains a line from the Sailing file
// Returns a boolean if retrieving all the data was successful
// Throws an exception if the read operation fails
//----------------------------------------------------------------
bool getNextSailing(Sailing& s)
{
	if (!sailingFile.is_open())
	{
		throw std::runtime_error("getNextSailing: File not open.");
	}

    sailingFile.clear();
    // Read information of the next sailing object in the file
	sailingFile.read(reinterpret_cast<char*>(&s), sizeof(Sailing));
	if (static_cast<std::size_t>(sailingFile.gcount()) < sizeof(Sailing))
	{
		// if it reaches end of file or partial, then reset the flags 
		sailingFile.clear();
		return false;
	}

    sailingFile.flush();

    if (!sailingFile)
    {
        // Throw an exception if the file could not be read from
        throw std::runtime_error("Error reading from file " + SAILINGFILENAME + ".");
    }
	return true;
}

// Function writeSailing writes a sailing record to the Sailing file
// Throws an exception if the write operation fails
//----------------------------------------------------------------
void writeSailing(const Sailing& s)
{
	if (!sailingFile.is_open())
	{
        // Throw an exception if the file is not open
		throw std::runtime_error("writeSailing: File not open.");
	}

    // Write information of the sailing object
    sailingFile.clear();
	sailingFile.write(reinterpret_cast<const char*>(&s), sizeof(Sailing));
	if (sailingFile.fail() || sailingFile.bad())
	{
		throw std::runtime_error("writeSailing: Failed to write record");
	}
	sailingFile.flush();
}

// Function checkSailingExists checks if a sailing with the provided
// sailingID exists. Returns sailingID, otherwise throws exception.
//----------------------------------------------------------------
int checkSailingExists(const char sailingID[])
{
	sailingReset();
	Sailing temp;
	int index = 0;
    // Check all the records and return the index of the sailing
	while (getNextSailing(temp))
	{
		if (std::strncmp(temp.sailingID, sailingID, sizeof(temp.sailingID)) == 0)
		{
			return index;
		}
	index++;
	}
	throw std::runtime_error("checkSailingExists: ID not found");
}

// Function deleteSailing deletes a sailing record with the provided
// sailingID. Throws an exception if the record is not found.
//----------------------------------------------------------------
void deleteSailing(const char sailingID[])
{
	if (!sailingFile.is_open())
	{
        // Throw an exception if the file is not open
		throw std::runtime_error("deleteSailing: File not open.");
	}
	//total record
	sailingFile.clear();
	sailingFile.seekg(0, std::ios::end);
	std::streampos size = sailingFile.tellg();
	int total = static_cast<int>(size / sizeof(Sailing));
	if (total == 0)
	{
        // Throw an exception if the file is empty
		throw std::runtime_error("deleteSailing: No records to delete");
	}

	// Find target index
	sailingFile.seekg(0, std::ios::beg);
	int target = -1;
	Sailing temp;
	Sailing lastRecord;
    // Find the sailing with the sailingID
	for (int i = 0; i < total; ++i)
	{
		sailingFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing));
		if (std::strncmp(temp.sailingID, sailingID, sizeof(temp.sailingID)) == 0)
		{
			target = i;
			break;
		}
	}
	sailingFile.seekg((total - 1) * sizeof(Sailing), std::ios::beg);
	sailingFile.read(reinterpret_cast<char*>(&lastRecord), sizeof(Sailing));

	if (target < 0)
	{
        // Throw an exception if the sailing was not found
		throw std::runtime_error(std::string("deleteSailing: '") + sailingID + "' not found");
	}

	// read record
	if (sailingFile.fail())
	{
		throw std::runtime_error("deleteSailing: Failed reading last record");
	}

	// overwirte target slot
	sailingFile.seekp(target * sizeof(Sailing), std::ios::beg);
	sailingFile.write(reinterpret_cast<const char*>(&lastRecord), sizeof(Sailing));
	if (sailingFile.fail())
	{
		throw std::runtime_error("deleteSailing: Overwrite failed");
	}

//Truncate 
#ifdef _WIN32
    {
        // flush and close c++ stream
        sailingFile.flush();
        sailingFile.close();

        // open FILE*
        FILE* f = std::fopen(SAILINGFILENAME.c_str(), "r+b");
        if (!f)
		{
            throw std::runtime_error("deleteSailing: fopen failed");
        }
        int fd = _fileno(f);

        // trunctate
        long newSize = static_cast<long>((total - 1) * sizeof(Sailing));
        if (_chsize_s(fd, newSize) != 0)
		{
            std::fclose(f);
            throw std::runtime_error("deleteSailing: truncate failed");
        }
        std::fclose(f);

        sailingFile.open(SAILINGFILENAME, std::ios::in | std::ios::out | std::ios::binary);
        if (!sailingFile.is_open()) 
		{
            throw std::runtime_error("deleteSailing: re-open failed");
        }
    }
#else
    {
        // close c++ stream
        sailingFile.close();

        // open file descriptor 
        int fd = ::open(SAILINGFILENAME.c_str(), O_RDWR);
        if (fd < 0)
		{
            throw std::runtime_error("deleteSailing: open failed");
        }

        // truncation
        off_t newSize = static_cast<off_t>((total - 1) * sizeof(Sailing));
        if (ftruncate(fd, newSize) != 0)
		{
            close(fd);
            throw std::runtime_error("deleteSailing: truncate failed");
        }
        close(fd);

        sailingFile.open(SAILINGFILENAME, std::ios::in | std::ios::out | std::ios::binary);
        if (!sailingFile.is_open())
		{
            throw std::runtime_error("deleteSailing: re-open failed");
        }
    }
#endif
}
