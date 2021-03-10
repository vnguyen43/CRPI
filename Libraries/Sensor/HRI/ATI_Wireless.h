///////////////////////////////////////////////////////////////////////////////
//
//  Original System: CRPI
//  Subsystem:       Human-Robot Interaction
//  Workfile:        ATI_Wireless.h
//  Revision:        1.0 - 11 March, 2021
//  Author:          V. Nguyen
//
//  Description
//  ===========
//  Interface wrapper for the ATI Wireless F/T Interface
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <ulapi.h>
#include <crpi.h>


namespace Sensor
{
	class LIBRARY_API ATI_Wireless
	{
	public:
		robotPose curATIForces;
		robotPose Offset;
		ulapi_integer ATI_ID;

		//! @brief Default constructor. Note that the destination port is fixed to be 49152 and the local port
		ATI_Wireless(char *ipAddress, long LocalPort);

		//! @brief Default destructor
		~ATI_Wireless();

		//! @brief Get the measured Cartesian forces from the sensor by sampling from the UDP socket
		//! @param forces Cartesian force data structure to be populated by the method
		void GetATIForces(robotPose &forces);

		//! @brief Zero the force sensor by configuring an offset (alternate way would be to use Telnet, which is flimsy in c++)
		void ATI_Wireless::ZeroATI();

		//! @brief To configure the sensor properties (sampling rate, units, etc.), you will have to modify the sensor calibration file or
		// communicate with the sensor using Telnet protocol with the PUTTY software (manual is online)

	}; // ATIWireless
}