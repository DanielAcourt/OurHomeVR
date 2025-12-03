# Wisps C++ Project

## Overview

This project is a C++ based game developed in Unreal Engine. From the codebase, it appears to be a game with gardening or farming mechanics, built on a grid system. Key features include a player character, interaction systems, and a save/load system. The project is structured as a plugin named `WispPlugin`.

## Features

*   **Player Character:** The game features a player character, likely represented by `WispPawn` and `AWispPlayerController`.
*   **Gardening System:** A core feature of the game appears to be a gardening or farming system, managed by the `GardenGridManager`. This system seems to use components like `PlantDataComponent` and `SoilDataComponent`.
*   **Grid System:** The game world is likely organized on a grid, with helper functions in `BFL_GridHelpers`.
*   **Interaction System:** The `Interactable` and `InteractionComponent` classes suggest a system for players to interact with objects in the world.
*   **Save/Load System:** The project has a robust save/load system, with classes like `BFL_UniversalSaveLoad`, `BFL_WispSaveManager`, and `SaveableEntityComponent`.
*   **Data Management:** The project includes tools for importing and exporting data tables, such as `FileImportLibrary` and `ImportExportDataTable`.

## Getting Started

### Prerequisites

*   Unreal Engine 5
*   A C++ compiler compatible with Unreal Engine (e.g., Visual Studio 2022)

### Setup

1.  Clone the repository.
2.  Right-click on the `WispsCpp.uproject` file and select "Generate Visual Studio project files".
3.  Open the generated `.sln` file in Visual Studio.
4.  Build the project in Visual Studio.
5.  Open the `WispsCpp.uproject` file in the Unreal Editor.

## Usage

Once the project is open in the Unreal Editor, you can start exploring the features. The core logic is contained within the `WispPlugin`. You can find the C++ classes in the `Plugins/WispPlugin` folder in the Solution Explorer in Visual Studio, or in the Content Browser in the Unreal Editor (under `WispPlugin C++ Classes`).

## Project Structure

The project's C++ code is organized as a plugin.

*   `Plugins/WispPlugin/Source/WispPlugin/Public`: Contains the public header files for the plugin's classes.
*   `Plugins/WispPlugin/Source/WispPlugin/Private`: Contains the private implementation files for the plugin's classes.
