# opiform sample

Lines below include a brief sample of how to use the project

## Instructions

1. Clone this repository with cmake:
``` git clone https://github.com/pixlifai/opiform.git ```
1. Go into the build folder:
``` cd opiform/build ```
1. Generate makefiles:
``` cmake ../src ```
1. Compile:
``` make ```

### Dependencies

* opiform doesn'r depend on any 3rd party library

### Running the opiform

* Use Main/main.cpp and adapt it as per requirements
* Adding stubborn agent:
``` Game/Game.cpp and modify function bool init(const double & adbThresholdLevel, int anSize) ```