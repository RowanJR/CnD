# CnD 

This is a simple digital implementation of the combat system from Dungeons and Dragons in C.

This project is meant as a sandbox for programming in C; The data structure is intended to allow relatively easy implementation and modification of mechanics and abilities.

*This project is still in a non-working state, the following described features are not yet serviceable.

# Structure

outline of file structure and function in compilation order.

### map

A 2-dimensional map that providess logic for moving entities, line of sight, and collisions.

### entities

Definition of the entity struct and its member structs, and important entity functions (like healing, modifying abilities and taking damage).

### ability_system

Defines the ability struct and core ability system. "Abilities" are any excess data or external thing available to an entity. Abilities range from conditions, like poison, to actions, like attacking. The core ability system functions on an event system that allows for abilities to subscribe to and respond to only the events that they care about. 

Newly added events can be added byt editing the event enum and will automatically be accomodated for, and can then be subscribed to by abilities.

### turns

The time and turn system. Tracks time in traditional 6 second turns, and tracks initiative when combat is occuring.

### abilities

Long file defining every single ability in C code. Abilities are written using switch statements and based on which event the ability recieves. The earlier compiled files are meant to support a variety of possibilities in the abilities script without requiring modification of any other files.

*In the future, this file should be replaced with a folder that can have more C files added without conflicting with each other.
