#include "KernelPersistence.h"
#include "SymmetryBridge.h"
#include "SymmetryKernel.h"
#include "json.hpp" // nlohmann/json single-header library
#include <fstream>
#include <iomanip>

// --- Bridge Accessor Implementation ---

FSymmetryKernel* FSymmetryBridge::GetKernel()
{
    // Safely downcast the interface pointer to the concrete implementation pointer.
    return static_cast<FSymmetryKernel*>(KernelInstance.get());
}


// --- Persistence Implementation ---

// Use the nlohmann namespace for convenience.
using json = nlohmann::json;

// Helper function to convert ESymmetryState to string for JSON serialization.
inline std::string StateToString(ESymmetryState state)
{
    switch (state)
    {
        case ESymmetryState::Null: return "Null";
        case ESymmetryState::Protocol: return "Protocol";
        case ESymmetryState::Latency: return "Latency";
        case ESymmetryState::Equilibrium: return "Equilibrium";
        default: return "Unknown";
    }
}

// Helper function to convert string back to ESymmetryState during JSON deserialization.
inline ESymmetryState StringToState(const std::string& str)
{
    if (str == "Protocol") return ESymmetryState::Protocol;
    if (str == "Latency") return ESymmetryState::Latency;
    if (str == "Equilibrium") return ESymmetryState::Equilibrium;
    return ESymmetryState::Null;
}

bool FKernelPersistence::SaveState(
    const std::string& FilePath,
    const std::unordered_map<uint64_t, ESymmetryState>& StateMatrix,
    const std::unordered_map<uint64_t, std::string>& ArchivalMemory,
    const std::unordered_map<uint64_t, float>& SignalTrust)
{
    json root;
    json signals = json::object();

    for (const auto& pair : StateMatrix)
    {
        uint64_t hash = pair.first;
        json signal_data;

        signal_data["state"] = StateToString(pair.second);

        if (ArchivalMemory.count(hash))
        {
            signal_data["tag"] = ArchivalMemory.at(hash);
        }

        if (SignalTrust.count(hash))
        {
            signal_data["trust"] = SignalTrust.at(hash);
        }

        signals[std::to_string(hash)] = signal_data;
    }

    root["signals"] = signals;

    try
    {
        std::ofstream file(FilePath);
        if (!file.is_open()) return false;
        file << std::setw(4) << root << std::endl;
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool FKernelPersistence::LoadState(
    const std::string& FilePath,
    std::unordered_map<uint64_t, ESymmetryState>& OutStateMatrix,
    std::unordered_map<uint64_t, std::string>& OutArchivalMemory,
    std::unordered_map<uint64_t, float>& OutSignalTrust)
{
    try
    {
        std::ifstream file(FilePath);
        if (!file.is_open()) return false;

        json root;
        file >> root;

        if (!root.contains("signals")) return false;

        const json& signals = root["signals"];
        for (auto it = signals.begin(); it != signals.end(); ++it)
        {
            uint64_t hash = std::stoull(it.key());
            const json& signal_data = it.value();

            if (signal_data.contains("state"))
            {
                OutStateMatrix[hash] = StringToState(signal_data["state"]);
            }

            if (signal_data.contains("tag"))
            {
                OutArchivalMemory[hash] = signal_data["tag"];
            }

            if (signal_data.contains("trust"))
            {
                OutSignalTrust[hash] = signal_data["trust"];
            }
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}
