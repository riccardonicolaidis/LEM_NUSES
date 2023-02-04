// GLOBAL VARIABLES
#ifndef MYGLOBALS_HH
#define MYGLOBALS_HH

// Set the number of sensors in the X-Y plane

#define N_SENSORS 8
#define IS_CONF_EPTAGONO true
#define IS_CONF_QUADRATO true




// Define the number of Plastic Scintillators
// The veto at the bottom is not considered
#define N_PL_SCINT_NO_VETO 0

// Set to 1 to enable Scintillation processes
// Scintillation is computationally expensive
#define OPTICAL_PROCESSES 0

// Generate the GDML file to export the geometry into a CAD
#define GENERATE_GDML 1

// Define the thickness of the Thin and the Thick detectors
// Thin: Delta E Detector
// Thick: E Detector
#define TK_THIN 100*um
#define TK_THICK 500*um
#define AREA_DETECTORS 150*mm2


#define WORLD_SIZE 3*m
#define CONTAINER_SIZE 2*m



#define RADIUS_EPTAGON 2.2*cm




// Per la configurazione a stella



// Select the geometry
// Set to 1 to view the new geometry
// Set to 0 to view the geometry from the Master thesis
//      Here, the geometry is implemented with Ametek PIPS
#define NEW_GEOMETRY 1

#define CZT_DETECTOR 0


#endif