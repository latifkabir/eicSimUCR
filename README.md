EIC Simulation Setup for UCR
=============================


1.Getting The Source Code
--------------------------
- Login to any RCAS interactive node of BNL RCF computer and Get the source code: 
```
git clone https://github.com/latifkabir/eicSimUCR.git
```

2.Setup / Sourcing
--------------------

The main purpose of this common setup is to have a single installation of all dependencies (Pythia or other generators, FastJet, Geant4 etc ..) on BNL or JLab machine which anyone can use without going thorugh the pain of installing them individually. The following sourcing achieves this goal. 

```
cd eicSimUCR/
source setup.csh
```
**Note:** You need to do `source setup.csh` for any new terminal session to comile or use it unless you add it to your `.cshrc` or `.bashrc` file.

Currently the following packages are included:

- ROOT 6
- Pythia 8
- Fastjet3

3.Compilation and Execution
------------------------
To compile use
```
make
```
- To run the example, Start `root` and call the function `ElecJetKin()` from the `root` prompt:
```
root
[0] ElecJetKin()
```
This should produce an output called `pytree.root` in the current directory. Make plots using the output root file.

4.Making Changes
--------------------
If you want to modify or write new functions to generate events that fit your requirements, you can follow the following recipe:

- Modify ElecJetKin.cxx or write a new one with `.cxx` extension and a corresponding header file `.h`.
- Add an entry to `LinkDef.h` if it's a new file. No chage to the makefile is necessary.
- Compile with: `make clean && make`
- Star `root` and call your function from the `root` prompt.


5.Updating The Package/Setup
--------------------------------
It is expected that the setup or packages will eveolve very rapidly as we work on it. You can update to the lastest version using the following commands from your `eicSimUCR/` directory:

```
git pull
make clean
make
```
