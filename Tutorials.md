
EIC-Smear
=============

- Git Repository: https://gitlab.com/eic/eic-smear

- How to build and run:
   - https://wiki.bnl.gov/eic/index.php/Monte_Carlo_and_Smearing
   - https://wiki.bnl.gov/eic/index.php/Smearing

- Nice Presentation By Kolja: https://indico.bnl.gov/event/6336/
- Thomas Barton: https://wiki.bnl.gov/eic/upload/EicsmearOverview2014.pdf

**Workflow:**

- Run Pythia 6 or any other compatiple generator to make output text file to be smeared.
- Use `BuildTree("to_be_smeared.txt", ".", 10000)` to convert the output text file (to be smeared) to a root file that can be used as input file for SmearTree() 

- You need a detector description file that returns a Smear::Detector class and has the specification (acceptance, resolution etc) implemented. Say the function `BuildBeAST()` does it.

- Use `SmearTree(BuildBeAST(), "to_bo_smeared.root", "smeared_out.root")` to get smeared file. 

**Data by EIC Group**

`/gpfs/mnt/gpfs02/eic/DATA/PYTHIA/ep/TXTFILES/`



Fun4All
===========


Fun4All Resources / References
-----------------------------------

1. Tutorial presentation is [here](https://indico.bnl.gov/event/7281/sessions/3572/attachments/26409/39981/EIC-simulation-tutorial.pdf)

2. Commands for each tutorials [here](https://www.phenix.bnl.gov/WWW/publish/phnxbld/EIC/tutorial/)

3. Fun4All Github Repo [here](https://github.com/sPHENIX-Collaboration/coresoftware/tree/master/offline/framework)

4. sPhenix Tutorial Github Repo [here](https://github.com/sPHENIX-Collaboration/tutorials)

5. Working from RCF machine:

- Dedicated EIC cvmfs volume: `/cvmfs/eic.opensciencegrid.org`

Set up environment: `source /cvmfs/eic.opensciencegrid.org/x8664_sl7/opt/sphenix/core/bin/eic_setup.sh -n`

- Set up local install area:
`source /cvmfs/eic.opensciencegrid.org/x8664_sl7/opt/sphenix/core/bin/setup_local.sh <install area>`

6. Software Documentation [page](https://eic-detector.github.io)



UCR Setup Instruction
--------------------------

1. From the top level directory clone/download any of the following examples depending on your need:

- Jaroslav’s Luminosity Monitor:  https://github.com/EIC-Detector/Fun4All-lmon
- A simple Detector: https://github.com/EIC-Detector/g4exampledetector
- Fast Momentum Resolution Estimate : https://github.com/sPHENIX-Collaboration/tutorials/Momentum
- Adding new detector:  https://github.com/sPHENIX-Collaboration/macros/macros/g4simulations

For each of them, follow the insstruction in Ref: 1 and 2 from the above resources.

2. Steps
-------------

To Install:
```
source /cvmfs/eic.opensciencegrid.org/x8664_sl7/opt/sphenix/core/bin/eic_setup.sh -n
git clone <Example Repo>
cd ExampleDir
mkdir installDir
cd source
mkdir build
cd build
../autogen.sh -prefix=<path-to-install-dir>
make install
```

The shared libraries and include files will be copied to <installDir>.


To run:
```
source /cvmfs/eic.opensciencegrid.org/x8664_sl7/opt/sphenix/core/bin/eic_setup.sh -n #Unless it's same session
source /cvmfs/eic.opensciencegrid.org/x8664_sl7/opt/sphenix/core/bin/setup_local.sh <path-to-install-dir>
cd ExampleDir/macros
```
Now try running the macros following instructions from ref 1 and 2.
