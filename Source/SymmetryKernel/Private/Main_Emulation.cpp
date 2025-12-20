#include <iostream>
#include <string>
#include <any>
#include <vector>
#include <algorithm>

#include "SymmetryBridge.h"
#include "KernelPersistence.h" // Include the persistence layer
#include "SymmetryKernel.h"    // Include the concrete kernel for friend access

// --- Unified C++ Test Harness with Persistence and Interactive Input ---

int main()
{
    std::cout << "--- Wisp Symmetry Kernel Emulation (Unified Build) ---" << std::endl;

    FSymmetryBridge Bridge;
    // We need a pointer to the concrete kernel to pass its data maps to the persistence layer.
    FSymmetryKernel* Kernel = Bridge.GetKernel();

    const std::string SaveFilePath = "kernel_state.json";

    // --- 1. Load State at Startup ---
    std::cout << "--- Attempting to load state from '" << SaveFilePath << "'..." << std::endl;
    if (Kernel && FKernelPersistence::LoadState(SaveFilePath, Kernel->StateMatrix, Kernel->ArchivalMemory, Kernel->SignalTrust))
    {
        std::cout << "--- State successfully loaded. ---" << std::endl << std::endl;
    }
    else
    {
        std::cout << "--- No previous state found or failed to load. Starting fresh. ---" << std::endl << std::endl;
    }

    const uint64_t TEST_SIGNAL_HASH = 0xDEADBEEF;
    const std::string INITIAL_TRUTH_TAG = "Tree_Oak";

    bool bTruthHasBeenRequested = false;
    bool bInitialEquilibriumReached = false;
    bool bSuccessionProtocolHasTriggered = false;
    int SilenceCounter = 0;

    // --- 2. Setup Callbacks with Interactive Loop ---
    Bridge.OnRequestTruth = [&](const FRequestTruthData& Data) {
        if (Data.SignalHash == TEST_SIGNAL_HASH) {
            if (!bInitialEquilibriumReached) {
                std::cout << "[CALLBACK] OnRequestTruth triggered for unknown signal: " << Data.SignalHash << std::endl;
                bTruthHasBeenRequested = true;
            } else {
                bSuccessionProtocolHasTriggered = true;
                std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                std::cout << "[CALLBACK] SUCCESSION PROTOCOL TRIGGERED!" << std::endl;
                std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl << std::endl;

                while (true) {
                    std::cout << "[INTERACTIVE] Waiting for Authority ('0' for silence, or a new tag): ";
                    std::string UserInput;
                    std::cin >> UserInput;

                    if (UserInput == "0") {
                        SilenceCounter++;
                        std::cout << "[INTERACTIVE] Parental Silence Detected. Counter: " << SilenceCounter << std::endl;
                        if (SilenceCounter >= 5) {
                            std::cout << "\n    --- [AI LOG] DESPERATION/AUTONOMY ---" << std::endl;
                        }
                    } else {
                        std::cout << "[INTERACTIVE] New truth '" << UserInput << "' received from Authority." << std::endl;
                        Bridge.ProvideTruth(Data.SignalHash, UserInput);
                        SilenceCounter = 0;
                        break;
                    }
                }
            }
        }
    };

    Bridge.OnEquilibriumReached = [&](const FEquilibriumData& Data) {
        if (Data.SignalHash == TEST_SIGNAL_HASH) {
            std::cout << "[CALLBACK] OnEquilibriumReached for signal: " << Data.SignalHash << " | Tag: '" << Data.SynthesizedTag << "'" << std::endl << std::endl;
            if(bSuccessionProtocolHasTriggered) {
                std::cout << "[AI LOG] New truth '" << Data.SynthesizedTag << "' has been successfully archived." << std::endl << std::endl;
            }
            bInitialEquilibriumReached = true;
        }
    };

    // --- 3. Run Simulation Logic ---
    std::cout << "Step 1: Processing signal to check current state..." << std::endl;
    Bridge.ProcessSignal(TEST_SIGNAL_HASH, std::any(), true);

    if (bTruthHasBeenRequested && !bInitialEquilibriumReached) {
        std::cout << "Step 2: Parental Authority providing initial truth handshake..." << std::endl;
        Bridge.ProvideTruth(TEST_SIGNAL_HASH, INITIAL_TRUTH_TAG);
    }

    if (bInitialEquilibriumReached) {
        std::cout << "Step 3: Initiating sensor conflict loop to test Trust Decay..." << std::endl;
        for (int i = 1; i <= 10; ++i) { // Reduced loop for clarity
            if(bSuccessionProtocolHasTriggered) break;
            std::cout << "  -> Conflict Loop " << i << ": Sending Sensor-Conflict signal..." << std::endl;
            Bridge.ProcessSignal(TEST_SIGNAL_HASH, std::any(), false);
        }
    }

    // --- 4. Save State at Shutdown ---
    std::cout << "\n--- Emulation Complete. Saving final state to '" << SaveFilePath << "'..." << std::endl;
    if (Kernel && FKernelPersistence::SaveState(SaveFilePath, Kernel->StateMatrix, Kernel->ArchivalMemory, Kernel->SignalTrust)) {
        std::cout << "--- State successfully saved. ---" << std::endl;
    } else {
        std::cout << "--- Failed to save state. ---" << std::endl;
    }

    return 0;
}
