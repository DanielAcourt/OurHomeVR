// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "BoidsActor.h"
#include "BoidsManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

ABoidsActor::ABoidsActor() {
    PrimaryActorTick.bCanEverTick = false;
    EpistemicState = 0b11; // Start in Tangible Truth
    CurrentVelocity = FMath::VRand(); // Start with a random velocity
}

void ABoidsActor::BeginPlay() { Super::BeginPlay(); }

void ABoidsActor::UpdateBoid(float DeltaTime, const FVector& GroupVelocity, const FVector& GroupCenter) {
    if (EpistemicState < 0b11 || !Manager) {
        EnterSuccessionState(DeltaTime);
        return;
    }

    // --- Flocking Calculations ---

    // 1. Separation
    FVector SeparationVector = FVector::ZeroVector;
    int32 SeparationCount = 0;
    for (ABoidsActor* OtherBoid : Manager->ManagedFlock)
    {
        if (OtherBoid != this)
        {
            float Distance = FVector::Dist(GetActorLocation(), OtherBoid->GetActorLocation());
            if (Distance > 0 && Distance < PerceptionRadius / 2.0f) // Avoid division by zero and check separation radius
            {
                SeparationVector -= (OtherBoid->GetActorLocation() - GetActorLocation()).GetSafeNormal() / Distance;
                SeparationCount++;
            }
        }
    }
    if (SeparationCount > 0)
    {
        SeparationVector /= SeparationCount;
    }

    // 2. Cohesion
    FVector CohesionVector = (GroupCenter - GetActorLocation());

    // 3. Alignment
    FVector AlignmentVector = (GroupVelocity - CurrentVelocity);

    // 4. Obstacle Avoidance
    FVector AvoidanceVector = FVector::ZeroVector;
    if (!ValidatePhysicalSymmetry(GetActorLocation() + CurrentVelocity.GetSafeNormal() * AvoidanceDistance))
    {
        // Simple avoidance: find a clear path by turning
        AvoidanceVector = FVector::CrossProduct(CurrentVelocity, FVector::UpVector);
    }

    // 5. Bounds Checking
    FVector BoundsVector = FVector::ZeroVector;
    if (FVector::Dist(GetActorLocation(), Manager->GetActorLocation()) > Manager->FlockBounds)
    {
        BoundsVector = (Manager->GetActorLocation() - GetActorLocation());
    }

    // --- Combine Forces ---
    FVector Acceleration = FVector::ZeroVector;
    Acceleration += SeparationVector.GetSafeNormal() * SeparationWeight;
    Acceleration += CohesionVector.GetSafeNormal() * CohesionWeight;
    Acceleration += AlignmentVector.GetSafeNormal() * AlignmentWeight;
    Acceleration += AvoidanceVector.GetSafeNormal() * AvoidanceWeight; // Corrected avoidance logic
    Acceleration += BoundsVector.GetSafeNormal() * 1.5f; // Add a strong force to return to bounds

    // --- Update Velocity and Position ---
    CurrentVelocity += Acceleration * DeltaTime;
    CurrentVelocity = CurrentVelocity.GetClampedToMaxSize(MaxSpeed);

    SetActorLocation(GetActorLocation() + CurrentVelocity * DeltaTime);
    SetActorRotation(CurrentVelocity.Rotation());
}

bool ABoidsActor::ValidatePhysicalSymmetry(FVector TargetLocation) {
    FHitResult HitResult;
    FVector StartLocation = GetActorLocation();
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        TargetLocation,
        ECC_Visibility,
        CollisionParams
    );

    return !bHit;
}

void ABoidsActor::EnterSuccessionState(float DeltaTime)
{
    CurrentVelocity *= 0.95f; // Simple decay
	FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;
	SetActorLocation(NewLocation);
    SetActorRotation(CurrentVelocity.Rotation());
}
