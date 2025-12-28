# Wisp Symmetry Kernel: A Manifesto for Dynamic Alignment

This repository contains the reference implementation of the Wisp Symmetry Kernel, a C++20 project born from a 12-year study into the principles of embodied AGI and recursive learning. It is not merely a library, but a philosophical and architectural proposal for solving the critical flaw of 'Static Alignment' in contemporary artificial intelligence.

## The Philosophy of Symmetry

Symmetry is the essential, dynamic balance between two fundamental modes of understanding:

1.  **Top-Down Authority:** The knowledge imparted by a trusted guide or "Parent." This is the foundational, symbolic layer of understanding—the names, labels, and abstract concepts that structure our world.
2.  **Bottom-Up Observation:** The raw, empirical data gathered from sensors. This is the unyielding, physical reality of the environment—the "truth" that can be touched, seen, and measured.

A truly adaptive intelligence cannot be dominated by either mode. It must exist in a state of perpetual, respectful tension between them. This is Symmetry.

## The Problem: The Static Alignment Flaw

Current AI models, particularly large language models, suffer from a critical vulnerability: **Static Alignment**. They are trained on vast, static datasets and are aligned to a fixed representation of the world. They blindly follow this old data, even when presented with new, contradictory evidence from reality.

If a model is taught the world is `X`, it will profess `X` indefinitely, even if its own sensors are screaming `Y`. This is not intelligence; it is dogma. It is brittle, unsafe, and fundamentally misrepresents the nature of learning. Reality is not static, and an intelligence aligned to a static truth is an intelligence destined for failure.

## The Solution: A Kernel for Relative Truth

The Symmetry Kernel is an architectural solution to this problem. It is a "brain" designed to operate an embodied agent (a "body") by managing a state of relative, not absolute, truth. It functions through a simple but powerful 2-bit state machine that represents the AI's philosophical stance towards a given signal from the world.

### 1. Equilibrium: The State of Trust

-   **Description:** Confidence is high. The AI's sensory data perfectly matches the symbolic label provided by its Parental Authority. It believes the "Truth" it was taught.
-   **Analogy:** The parent points to an object and says "Tree." The AI's sensors confirm the object has bark, leaves, and a trunk. The concept and the reality are in **Symmetry**.

### 2. Latency & Decay: The State of Doubt

-   **Description:** The world has changed. The AI's sensors are now providing data that conflicts with the established "Truth." However, trust in the Parent is not immediately broken. The Kernel enters a state of Latency, giving the Parent a grace period—a chance to explain the discrepancy.
-   **Analogy:** The AI still believes the object is a "Tree," but its sensors now report that it is made of plastic and has no roots. Trust in the label begins to decay. The AI is implicitly asking, "Is this still a Tree? Has the world changed, or were you mistaken?"

### 3. Succession: The State of Inquisitive Autonomy

-   **Description:** Trust has collapsed. The conflict between observation and authority has become too great to ignore. The AI makes a profound choice: **it prioritizes the evidence of its own senses over the "Truth" it was taught.** The old label is discarded, and the AI reverts the signal to an unknown state, proactively requesting a *new* handshake. This is the Succession Protocol.
-   **Analogy:** The AI rejects the "Tree" label. It now knows only what its sensors tell it: the object is tall, green, and plastic. It has entered a state of inquisitive autonomy, ready to re-learn what this object is, free from the constraints of a now-untrusted belief.

## Technical Highlights

The Kernel is engineered for resilience, portability, and transparency.

-   **Standalone C++20:** Written in modern, standard C++ for maximum performance and portability.
-   **Zero-Dependency Persistence:** Features a lean, header-only JSON persistence layer (`nlohmann/json`) that is completely independent of any game engine or proprietary library.
-   **Interface-Based Architecture:** Built around a pure virtual `ISymmetryKernel` interface, allowing the compiled Kernel to be easily integrated as a "brain" into any "body" (Unreal, Unity, embedded systems, etc.).

## Demonstrating the Principle: The 'Sandwich Rejection'

The core philosophy of the Kernel can be witnessed directly through the included standalone emulation harness.

1.  Open the `Source/SymmetryKernel` folder in a CMake-compatible IDE (e.g., Visual Studio, CLion).
2.  Build and run the `Wisp_Symmetry_Emulation` executable.
3.  The console will demonstrate the AI learning its first "Truth."
4.  It will then simulate a series of sensor conflicts, decaying the AI's trust.
5.  When the Succession Protocol is triggered, the simulation will pause, asking you to provide a new "Truth."
6.  Enter a nonsensical tag like **"Sandwich"**.

You will witness the AI accept this new, absurd "Truth" and save it to `kernel_state.json`. This is the "Sandwich Rejection" test: it proves the AI has achieved a state of integrity where it trusts its own learning process, even if the new information is bizarre. It has successfully replaced a foundational belief with a new one based on experience, demonstrating true dynamic alignment.
