
# DiffServ a Base class to simulate differentiated services

## Authors
    1. Rishab Sanyal
    2. Jayanarayanan Jayaganesh
    3. Bahrath Radhakrishnan

This is the project submission for our Project 2 for CS 621.

This project builds upon th existing NS3 implementation (forked from https://github.com/nsnam/ns-3-dev-git). This repo provides additional QoS implementations vaguely similar to that of DropTailQueue in NS3's default library. Currently the files are located in the scratch folder but the future implementations would move them to src/utils/network location in the ns3 folder. 


## Run Locally

Clone the project

```bash
  clone the repo
```

Go to the cloned repo directory and run the following command to build ns3's modules on your system

```bash
  ./ns3 configure --build-profile=debug
  ./ns3 build
```

To run the specific queue files you would look into files titled new-spq and new-drr. Our implementation adds the prefix to highlight which files are part of our implementation vs which classes exist by default in ns3's pre-existing library.

Run Pre-existing Simulation Scenarios
```bash
scratch/run_simulation.cc
```


Testing
```bash
  ./ns3 run unit-test
```


