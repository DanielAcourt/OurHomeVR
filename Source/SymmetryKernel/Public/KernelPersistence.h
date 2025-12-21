// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.
#pragma once

#include <string>
#include <unordered_map>
#include "ISymmetryKernel.h" // For ESymmetryState

// Forward-declarations to keep the header lean and free of specific JSON library includes.
namespace nlohmann
{
    template<typename, typename, typename, typename, typename, typename, typename>
    class basic_json;
    using json = basic_json<std::map, std::vector, std::string, bool, std::int64_t, std::uint64_t, float>;
}

/**
 * @class FKernelPersistence
 * @brief Handles saving and loading the Symmetry Kernel's state to/from a JSON file.
 */
class FKernelPersistence
{
public:
    /**
     * @brief Saves the current state of the Kernel's memory to a JSON file.
     */
    static bool SaveState(
        const std::string& FilePath,
        const std::unordered_map<uint64_t, ESymmetryState>& StateMatrix,
        const std::unordered_map<uint64_t, std::string>& ArchivalMemory,
        const std::unordered_map<uint64_t, float>& SignalTrust
    );

    /**
     * @brief Loads and populates the Kernel's memory from a JSON file.
     */
    static bool LoadState(
        const std::string& FilePath,
        std::unordered_map<uint64_t, ESymmetryState>& OutStateMatrix,
        std::unordered_map<uint64_t, std::string>& OutArchivalMemory,
        std::unordered_map<uint64_t, float>& OutSignalTrust
    );
};
