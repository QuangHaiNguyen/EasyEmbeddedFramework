# 2. Flash sim initialization

Date: 2022-04-30

## Status

Superceded by [3. Test](0003-test.md)

## Context

The issue motivating this decision, and any context that influences or constrains the decision.

when initialization the flash simulator, there are two flavors, which are:

* Let the users initialize the flash memory during run time
* Initializing the flash at compile time, and create a static array to store the the number of initialized flash memory.

## Decision

The second option is chosen to be implemented.

Rational - we are simulating the hardware flash memory, and in reaf life, flash memory is always available and do not need to be initialized by code. The code initialized only the communication interface to the flash and the configuration of the flash. 

## Consequences

The user must configure the flash, e.g. adding the flash name, size, into an array, which is implemented to manage the simulated flash.

