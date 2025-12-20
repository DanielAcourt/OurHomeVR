#include <iostream>
#include <string>
#include <any>
#include <vector>
#include <algorithm> // For std::max

#include "SymmetryBridge.h"

// --- Interactive C++ Test Harness for the Symmetry Kernel ---

/**
 * This main entry point acts as an INTERACTIVE "Test Harness" for the Symmetry Kernel.
 * Its purpose is to allow a human operator (Daniel) to act as the "Parental Authority"
 * during a "Successsion Protocol" event, providing new "Truths" or remaining silent
 * to test the AI's state machine under various conditions.
 */
int main()
{
    std::cout << "--- Wisp Symmetry Kernel Emulation (Interactive Mode) ---" << std::endl;
    std::cout << "--- Initializing Standalone Test Harness ---" << std::endl << std::endl;

    FSymmetryBridge Bridge;

    const uint64_t TEST_SIGNAL_HASH = 0xDEADBEEF;
    const std::string INITIAL_TRUTH_TAG = "Tree_Oak";

    // --- State flags for the emulation logic ---
    bool bTruthHasBeenRequested = false;
    bool bInitialEquilibriumReached = false;
    bool bSuccessionProtocolHasTriggered = false;
    int SilenceCounter = 0;

    // 1. Setup Callbacks (The 'Body' listening to the 'Brain')
    Bridge.OnRequestTruth = [&](const FRequestTruthData& Data)
    {
        if (Data.SignalHash == TEST_SIGNAL_HASH)
        {
            if (!bInitialEquilibriumReached)
            {
                // This is the first, non-interactive request for truth.
                std::cout << "[CALLBACK] OnRequestTruth triggered for unknown signal: " << Data.SignalHash << std::endl;
                bTruthHasBeenRequested = true;
            }
            else
            {
                // --- SUCCESSION PROTOCOL & INTERACTIVE LOOP ---
                bSuccessionProtocolHasTriggered = true;
                std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                std::cout << "[CALLBACK] SUCCESSION PROTOCOL TRIGGERED!" << std::endl;
                std::cout << "           The AI has changed its mind and requires a new truth." << std::endl;
                std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl << std::endl;

                // The simulation is now PAUSED, waiting for Parental Authority.
                while (true)
                {
                    std::cout << "[INTERACTIVE] Waiting for Authority ('0' for silence, or a new tag): ";
                    std::string UserInput;
                    std::cin >> UserInput;

                    if (UserInput == "0")
                    {
                        SilenceCounter++;
                        std::cout << "[INTERACTIVE] Parental Silence Detected. Counter: " << SilenceCounter << std::endl;
                        if (SilenceCounter >= 5)
                        {
                            std::cout << "\n    --- [AI LOG] DESPERATION/AUTONOMY ---" << std::endl;
                            std::cout << "    Parental Authority is silent after " << SilenceCounter << " requests." << std::endl;
                            std::cout << "    Unable to resolve signal " << Data.SignalHash << "." << std::endl;
                            std::cout << "    Awaiting new instructions, but proceeding with caution." << std::endl;
                            std::cout << "    -------------------------------------\n" << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "[INTERACTIVE] New truth '" << UserInput << "' received from Authority." << std::endl;
                        // Provide the new, nonsensical truth to the Kernel.
                        Bridge.ProvideTruth(Data.SignalHash, UserInput);
                        // Reset the silence counter as we received a valid response.
                        SilenceCounter = 0;
                        // Exit the interactive loop and resume the simulation.
                        break;
                    }
                }
            }
        }
    };

    Bridge.OnEquilibriumReached = [&](const FEquilibriumData& Data)
    {
        if (Data.SignalHash == TEST_SIGNAL_HASH)
        {
            std::cout << "[CALLBACK] OnEquilibriumReached for signal: " << Data.SignalHash << " | Tag: '" << Data.SynthesizedTag << "'" << std::endl << std::endl;

            // If the succession protocol has already happened, this confirms the AI has accepted the NEW truth.
            if(bSuccessionProtocolHasTriggered)
            {
                std::cout << "[AI LOG] New truth '" << Data.SynthesizedTag << "' has been successfully archived." << std::endl << std::endl;
            }

            bInitialEquilibriumReached = true;
        }
    };

    // 2. Initial discovery.
    std::cout << "Step 1: Processing an unknown signal for the first time..." << std::endl;
    Bridge.ProcessSignal(TEST_SIGNAL_HASH, std::any(), true);

    // 3. Initial "Parental Authority" handshake.
    if (bTruthHasBeenRequested)
    {
        std::cout << "Step 2: Parental Authority providing initial truth handshake..." << std::endl;
        Bridge.ProvideTruth(TEST_SIGNAL_HASH, INITIAL_TRUTH_TAG);
    }

    // 4. Sensor conflict loop.
    if (bInitialEquilibriumReached)
    {
        std::cout << "Step 3: Initiating sensor conflict loop to test Trust Decay..." << std::endl;

        // This loop will now run up to 20 times to allow for interactive testing after the first succession.
        for (int i = 1; i <= 20; ++i)
        {
            std::cout << "  -> Conflict Loop " << i << ": Sending Sensor-Conflict signal..." << std::endl;

            // This call will either decay trust or trigger the interactive OnRequestTruth callback.
            Bridge.ProcessSignal(TEST_SIGNAL_HASH, std::any(), false);
        }
    }

    std::cout << "\n--- Emulation Complete ---" << std::endl;

    return 0;
}
