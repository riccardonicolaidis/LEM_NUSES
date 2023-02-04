# LOW ENERGY MODULE: Geant4 simulation


![Logo](data:image/jpeg;base64,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)


- For the official page of **Geant4 toolkit** visit [https://geant4.web.cern.ch/](https://geant4.web.cern.ch/)

------------------------------------

## The content of the software

In this simulation I am going to build the geometry of the Low Energy Module which is an instrument for the measurement of the fluxes of particles in the sub-MeV region up to some tenths of MeVs for electrons. 

![LEM](./docs/assets/NewGeometry/newGeometryFront._0000.png)


The detector is designed in the following way. From the external to the internal part there are:
- An aluminium mask formin a collimator. The Aluminium layer is 0.8 cm thick
- An active Veto made of plastic scintillator EJ-200 1 cm thick
- A first Silicon layer $100 \ \mu m$ thick. This is the $\Delta E$ layer in which only a small fraction of the energy is deposited. This amount of energy is sometimes called Linear Energy Transfer (LET). This because  
$$
\Delta E \approx \biggl| - \frac{1}{\rho} \frac{dE}{dx} \biggr| \rho_{Si} \times \Delta x
$$
- Then, a second layer made of Silicon with larger thickness is the layer in which the particle has to stop. The simulation has the setting to change this thickness and also to select another option of the material which is the CZT, a denser material.
- An active veto, made of plastic scintillator Ej-200 is then positioned on the back. If a particle is too energetic, it reaches the veto which wiull trigger ignoring that event.
- Additional plastic scintillator layers are added to extend te observed energy window to higher energies

The detector is based on the $\Delta E - E$ technique. This simulation is aimed at characterising the energy deposition inside the materials using the correct geometry of the instrument. 

Below are reported the front and the lateral view of the instrument.


![LEM](./docs/assets/NewGeometry/newGeometry._0000._0000.png)


This geometry has been implemented within the DetectorConstruction class.

**The number of holes is parametrised in the myglobals.cc**
--------------------

- All the parameters are described in the first part of the **Construct()** method

- **All the variables used have been declared in the preamble of the class declaration (file .hh) in the private section**

- **All the energy measurements are performed invoking the Sensitive Detector class**


- In the ConstructSDandFields() methods are defined all the sensitive detectors.

**PROBLEM**:
------------------------

Since the sensitive detector class has been used, it is not possible to reinitialize the geometry in order to change some parameters and materials because there is a problem with the sensitive detector construction. **To be solved!**

![LEM](./docs/assets/NewGeometry/debuggingMode._0000.png)
![LEM](./docs/assets/NewGeometry/muon._0000.png)


## Particle gun and GPS

There are two possible way to generate particles:
- The **General Particle Source** generates an isotropic flux of particles from a generation plane. To set the required features one needs to write a macros as in the following case. As you can see, the **Debugging Mode** has to be switched off. By default it is switched off but it is always a good practise to initialise in the macro the value of the boolean variable.

```
/particleEnergyRandom/DebbuggingModeIsOn false

/gps/verbose 0
/gps/pos/type Plane
/gps/pos/shape Square
/gps/pos/centre 0 0 -2.4 cm
/gps/pos/halfx 6.1 cm
/gps/pos/halfy 6.1 cm
/gps/ang/type iso
/gps/ene/type Lin
/gps/ene/gradient 0
/gps/ene/intercept 1
/gps/ang/maxtheta 90 deg
/gps/ang/mintheta 0 deg

 # -------------------------------------------------------- #
 #                         ELECTRONS                        #
 # -------------------------------------------------------- #

/gps/particle e-
/gps/ene/min 0.001 MeV
/gps/ene/max 1 MeV

/NameOfFile/NameOfFile 01_08_2022_ELECTRON
/run/beamOn 10000000

```

- The **Debugging Mode** is extremely useful when studying the energy deposition inside the materials involved in the detector. With this mode, particles are shooted exactly along the axes of the collimators as it is possible to see from the pictures







-----------------------
## Brief description of the directory structure

This is a **CMake** project. Therefore, a CMake file is required. In this case, the file is named **CMakeLists.txt**. The subdirectory **build/** is needed to compile the project.

Files are divided into two cathegories:
- **src** : Sources file with the implementation of all the classes
- **include** : Classes declaration and all the libraries to be included in the project
- **macros** : Here there are all the macros used to generate the data for the simulation. To run these macros you need to start the simulation in batch mode
```
./Name_of_the_executable_file Name_of_the_macro.mac
```
- **ROOT_macros** : Here there are all the .root macros to process data and also used to generate all the plots. To propely run these macros you need to follow these steps:
  - Run the sumulation in batch mode using one of the macros .mac in the ./macros folder. For this example take  the following (you need to be in the build directory)
```
./Simulation run_01_08_2022.mac
```
  - Create a new directory in the LEM_SIMULATION/ folder named DST_01_08_2022
```
mkdir ${LOCATION_REPOSITORY}/LEM_SIMULATION/DST_01_08_2022
```
  - Move all the .root files in this directory 
```
cd build
mv *.root ../DST_01_08_2022/.
```
  - Create a .txt file named FileNames.txt and write the names of the files in this .txt 
```
01_08_2022_ELECTRON.root
01_08_2022_PROTON.root
01_08_2022_ALPHA.root
```
  - Now, from this directory you can run a root macro
```
cd ${LOCATION_REPOSITORY}/LEM_SIMULATION/DST_01_08_2022
root -l ../ROOT_macros/AngleResolution.C
```



---------------------------------------


## Build the project
To build the project go to the /build directory and run the command

```
cmake ..
```

Then, you can run the command

```
make
```

Then, an executable file will be created in the /build directory. To run the executable file, simply run the command
```
./Name_of_the_executable_file
```
The name of the executable file, in this case, is simply "simulation", which is exactly the name of the project.

--------------------------------------------

## Useful resources 
- Physics Matters Geant4 tutorial : [Link](https://www.youtube.com/playlist?list=PLLybgCU6QCGWgzNYOV0SKen9vqg4KXeVL)
- Geant4 page : [Link](https://geant4.web.cern.ch/)
- Geant4 User guide : [Link](https://geant4.web.cern.ch/support/user_documentation)
- ROOT Cern C++ : [Link](https://root.cern/)
