#include <iostream>
#include <string>
#include <any>
#include <vector>
#include <algorithm> // For std::max

#include "SymmetryBridge.h"

// --- Standalone C++ Test Harness for the Symmetry Kernel ---

/**
 * This main entry point acts as a "Test Harness" or "Emulator" for the Symmetry Kernel.
 * Its purpose is to simulate the Kernel's core logic in a controlled console environment,
 * completely independent of Unreal Engine. It specifically tests the "Succession Protocol"
 * by teaching the AI a "Truth" and then repeatedly contradicting it until the AI is forced
 * to "change its mind."
 */
int main()
{
    std::cout << "--- Wisp Symmetry Kernel Emulation ---" << std::endl;
    std::cout << "--- Initializing Standalone Test Harness ---" << std::endl << std::endl;

    // Instantiate the bridge, which in turn instantiates the Kernel.
    FSymmetryBridge Bridge;

    const uint64_t TEST_SIGNAL_HASH = 0xDEADBEEF;
    const std::string INITIAL_TRUTH_TAG = "Tree_Oak";

    // --- State flags for the emulation logic ---
    bool bTruthHasBeenRequested = false;
    bool bInitialEquilibriumReached = false;
    bool bSuccessionProtocolHasTriggered = false;

    // 1. Setup Callbacks (The 'Body' listening to the 'Brain')
    // These lambda functions will be triggered by the Kernel to report its state changes.
    Bridge.OnRequestTruth = [&](const FRequestTruthData& Data)
    {
        if (Data.SignalHash == TEST_SIGNAL_HASH)
        {
            if (!bInitialEquilibriumReached)
            {
                // This is the first, expected request for truth.
                std::cout << "[CALLBACK] OnRequestTruth triggered for unknown signal: " << Data.SignalHash << std::endl;
                bTruthHasBeenRequested = true;
            }
            else
            {
                // This is the CRITICAL event: the Kernel is re-requesting truth for a known signal.
                // This means the Succession Protocol has been triggered.
                std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                std::cout << "[CALLBACK] SUCCESSION PROTOCOL TRIGGERED!" << std::endl;
                std::cout << "[CALLBACK] OnRequestTruth RE-TRIGGERED for signal: " << Data.SignalHash << std::endl;
                std::cout << "           The AI has changed its mind and requires a new truth." << std::endl;
                std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl << std::endl;
                bSuccessionProtocolHasTriggered = true;
            }
        }
    };

    Bridge.OnEquilibriumReached = [&](const FEquilibriumData& Data)
    {
        if (Data.SignalHash == TEST_SIGNAL_HASH)
        {
            std::cout << "[CALLBACK] OnEquilibriumReached for signal: " << Data.SignalHash << " | Tag: '" << Data.SynthesizedTag << "'" << std::endl << std::endl;
            bInitialEquilibriumReached = true;
        }
    };

    // 2. Simulate the initial discovery of an unknown object.
    std::cout << "Step 1: Processing an unknown signal for the first time..." << std::endl;
    Bridge.ProcessSignal(TEST_SIGNAL_HASH, std::any(), true); // bSensorConfirmation is irrelevant here.

    // 3. The "Parental Authority" provides the "Truth" handshake, as requested by the callback.
    if (bTruthHasBeenRequested)
    {
        std::cout << "Step 2: Parental Authority providing initial truth handshake..." << std::endl;
        Bridge.ProvideTruth(TEST_SIGNAL_HASH, INITIAL_TRUTH_TAG);
    }

    // 4. Simulate a series of sensor conflicts to trigger Trust Decay.
    if (bInitialEquilibriumReached)
    {
        std::cout << "Step 3: Initiating sensor conflict loop to test Trust Decay..." << std::endl;
        float expectedTrust = 1.0f;
        const float parentalWeight = 1.0f; // Using the default weight.
        const float decayAmount = 0.2f * parentalWeight;

        for (int i = 1; i <= 10; ++i)
        {
            if (bSuccessionProtocolHasTriggered)
            {
                break; // Stop the loop once the AI has changed its mind.
            }

            expectedTrust = std::max(0.0f, expectedTrust - decayAmount);
            std::cout << "  -> Conflict Loop " << i << ": Sending Sensor-Conflict signal. Expected Trust should be ~" << expectedTrust << std::endl;

            // This is the core of the test: we repeatedly tell the Kernel its sensors are wrong.
            Bridge.ProcessSignal(TEST_SIGNAL_HASH, std::any(), false);
        }
    }

    std::cout << "--- Emulation Complete ---" << std::endl;

    return 0;
}
