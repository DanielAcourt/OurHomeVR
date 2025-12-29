#include "BeeActor.h"
#include "HiveManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Misc/Guid.h"
#include "Hash/CityHash.h"
#include "SaveableEntityComponent.h"
#include "QiComponent.h"
#include "GameplayTagContainer.h"
#include "EngineUtils.h"
#include "ActorRegistry.h"
#include "InteractionComponent.h"

ABeeActor::ABeeActor() {
    PrimaryActorTick.bCanEverTick = false;
    CurrentVelocity = FMath::VRand();

    SaveableEntityComponent = CreateDefaultSubobject<USaveableEntityComponent>(TEXT("SaveableEntityComponent"));
    QiComponent = CreateDefaultSubobject<UQiComponent>(TEXT("QiComponent"));
    InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void ABeeActor::BeginPlay()
{
    Super::BeginPlay();
    ApplyGeneticDNA();

    if (SaveableEntityComponent)
    {
        SaveableEntityComponent->EntityTypeTag = FGameplayTag::RequestGameplayTag(FName("Animal.Insect.Bee"));
    }

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UActorRegistry* Registry = GameInstance->GetSubsystem<UActorRegistry>();
        if (Registry)
        {
            Registry->RegisterActor(this);
        }
    }
}

void ABeeActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UActorRegistry* Registry = GameInstance->GetSubsystem<UActorRegistry>();
        if (Registry)
        {
            Registry->UnregisterActor(this);
        }
    }
}

void ABeeActor::InitializeIdentity(const FGuid& HiveID, int32 GenerationIndex, EBeeType InBeeType)
{
    GeneticProfile.HomeHiveID = HiveID;
    GeneticProfile.BeeType = InBeeType;

    FString HashedString = FString::Printf(TEXT("%s-%d-%d"), *HiveID.ToString(), GenerationIndex, static_cast<int32>(InBeeType));
    uint64 Hash1 = CityHash64(TCHAR_TO_ANSI(*HashedString), HashedString.Len());
    uint64 Hash2 = CityHash64(TCHAR_TO_ANSI(*HashedString), HashedString.Len()) + 1;
    uint32* Hash1AsInts = reinterpret_cast<uint32*>(&Hash1);
    uint32* Hash2AsInts = reinterpret_cast<uint32*>(&Hash2);
    GeneticProfile.LineageID = FGuid(Hash1AsInts[0], Hash1AsInts[1], Hash2AsInts[0], Hash2AsInts[1]);
    GeneticProfile.GeneticSeed.Initialize(GeneticProfile.LineageID.A);

    if (SaveableEntityComponent)
    {
        SaveableEntityComponent->InstanceID = GeneticProfile.LineageID;
    }
}

void ABeeActor::ApplyGeneticDNA()
{
    if (!GeneticProfile.GeneticSeed.IsInitialized()) return;

    MaxSpeed = GeneticProfile.GeneticSeed.FRandRange(400.0f, 600.0f);
    MaxSteeringForce = GeneticProfile.GeneticSeed.FRandRange(80.0f, 120.0f);
    float BaseQi = 100.0f;
    float MaxQiRange = 50.0f;

    switch (GeneticProfile.BeeType)
    {
        case EBeeType::Worker:
            break;
        case EBeeType::Drone:
            BaseQi = 150.0f;
            MaxQiRange = 75.0f;
            break;
        case EBeeType::Queen:
            BaseQi = 500.0f;
            MaxQiRange = 250.0f;
            break;
        case EBeeType::Scout:
            MaxSpeed = GeneticProfile.GeneticSeed.FRandRange(800.0f, 1000.0f);
            BaseQi = 1.0f;
            MaxQiRange = 0.0f;
            MaxPayload = 0.1f;
            break;
        case EBeeType::Gatherer:
            MaxSpeed = GeneticProfile.GeneticSeed.FRandRange(200.0f, 300.0f);
            BaseQi = 5.0f;
            MaxQiRange = 0.0f;
            MaxPayload = 1.0f;
            break;
        case EBeeType::Nursery:
            MaxSpeed = 0.0f;
            CohesionWeight = 0.0f;
            break;
    }

    QiCapacity = GeneticProfile.GeneticSeed.FRandRange(BaseQi, BaseQi + MaxQiRange);

    if (QiComponent)
    {
        QiComponent->QiData.MaxQi = QiCapacity;
        QiComponent->QiData.CurrentQi = QiCapacity;
    }
}

void ABeeActor::SynchronizeSymmetryWithComponents()
{
    if (QiComponent)
    {
        QiComponent->ConsumeQi(0.01f); // Consume a small amount of Qi each frame
        if (QiComponent->QiData.CurrentQi <= 0)
        {
            BeeState = EBeeState::Dead;
            return;
        }

        float QiPercentage = QiComponent->QiData.CurrentQi / QiComponent->QiData.MaxQi;
        if (QiPercentage < 0.2f && BeeState != EBeeState::ReturningToHive && BeeState != EBeeState::Resting)
        {
            BeeState = EBeeState::Resting;
            TargetActor = Manager;
        }

        float CurrentMaxSpeed = MaxSpeed * FMath::Clamp(QiPercentage, 0.1f, 1.0f);
        CurrentVelocity = CurrentVelocity.GetClampedToMaxSize(CurrentMaxSpeed);
    }
}

TArray<ABeeActor*> ABeeActor::GetNearbySisters()
{
    TArray<ABeeActor*> NearbySisters;
    if (!Manager) return NearbySisters;

    TArray<ABeeActor*> NearbyBees = Manager->GetNearbyBees(GetActorLocation(), PerceptionRadius);
    for (ABeeActor* OtherBee : NearbyBees)
    {
        if (OtherBee != this && OtherBee->GeneticProfile.HomeHiveID == this->GeneticProfile.HomeHiveID)
        {
            NearbySisters.Add(OtherBee);
        }
    }
    return NearbySisters;
}

void ABeeActor::FindNewTarget()
{
    UActorRegistry* Registry = GetGameInstance()->GetSubsystem<UActorRegistry>();
    if (!Registry || !Manager) return;

    if (GeneticProfile.BeeType == EBeeType::Scout)
    {
        FGuid TargetID;
        if (Manager->RequestTarget(TargetID))
        {
            TargetActor = Registry->FindActor(TargetID);
            if(TargetActor)
            {
                Manager->RegisterProbableTarget(TargetID);
            }
        }
    }
    else if (GeneticProfile.BeeType == EBeeType::Gatherer)
    {
        for (auto const& [Guid, State] : Manager->DiscoveryRegistry)
        {
            if (State == ETruthState::Tangible)
            {
                TargetActor = Registry->FindActor(Guid);
                if (TargetActor) break;
            }
        }
    }
}

FVector ABeeActor::CalculateFlockingForces(const FVector& GroupVelocity, const FVector& GroupCenter)
{
    FVector Acceleration = FVector::ZeroVector;

    TArray<ABeeActor*> NearbySisters = GetNearbySisters();

    FVector SeparationVector = FVector::ZeroVector;
    if (NearbySisters.Num() > 0)
    {
        for (ABeeActor* Sister : NearbySisters)
        {
            if (FVector::DistSquared(GetActorLocation(), Sister->GetActorLocation()) < FMath::Square(PerceptionRadius / 2.0f))
            {
                SeparationVector -= (Sister->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            }
        }
        SeparationVector /= NearbySisters.Num();
    }

    FVector CohesionVector = (GroupCenter - GetActorLocation());
    FVector AlignmentVector = (GroupVelocity - CurrentVelocity);
    FVector AvoidanceVector = FVector::ZeroVector;
    if (!ValidatePhysicalSymmetry(GetActorLocation() + CurrentVelocity.GetSafeNormal() * AvoidanceDistance))
    {
        AvoidanceVector = FVector::CrossProduct(CurrentVelocity, FVector::UpVector);
    }

    FVector BoundsVector = FVector::ZeroVector;
    if (FVector::DistSquared(GetActorLocation(), Manager->GetActorLocation()) > FMath::Square(Manager->SwarmBounds))
    {
        BoundsVector = (Manager->GetActorLocation() - GetActorLocation());
    }

    FVector TargetVector = FVector::ZeroVector;
    if(TargetActor)
    {
        TargetVector = (TargetActor->GetActorLocation() - GetActorLocation());
    }

    Acceleration += SeparationVector.GetSafeNormal() * SeparationWeight;
    Acceleration += CohesionVector.GetSafeNormal() * CohesionWeight;
    Acceleration += AlignmentVector.GetSafeNormal() * AlignmentWeight;
    Acceleration += AvoidanceVector.GetSafeNormal() * AvoidanceWeight;
    Acceleration += BoundsVector.GetSafeNormal() * BoundsWeight;
    Acceleration += TargetVector.GetSafeNormal() * TargetWeight;

    return Acceleration;
}

void ABeeActor::UpdateMovement(const FVector& Steering, float DeltaTime)
{
    CurrentVelocity += Steering * DeltaTime;
    CurrentVelocity = CurrentVelocity.GetClampedToMaxSize(MaxSpeed);

    SetActorLocation(GetActorLocation() + CurrentVelocity * DeltaTime);
    SetActorRotation(CurrentVelocity.Rotation());
}


void ABeeActor::UpdateBee(float DeltaTime, const FVector& GroupVelocity, const FVector& GroupCenter) {
    if (GeneticProfile.BeeType == EBeeType::Nursery) return;

    // State Machine
    switch (BeeState)
    {
        case EBeeState::Idle:
            HandleIdleState();
            break;
        case EBeeState::Scouting:
            HandleScoutingState();
            break;
        case EBeeState::MovingToTarget:
            HandleMovingToTargetState();
            break;
        case EBeeState::ReturningToHive:
            HandleReturningToHiveState();
            break;
        case EBeeState::Interacting:
            HandleInteractingState();
            break;
        case EBeeState::Resting:
            HandleRestingState();
            break;
        case EBeeState::Dead:
            EnterSuccessionState();
            break;
    }

    SynchronizeSymmetryWithComponents();

    if (!Manager) {
        EnterSuccessionState();
        return;
    }

    FVector Steering = CalculateFlockingForces(GroupVelocity, GroupCenter).GetClampedToMaxSize(MaxSteeringForce);
    UpdateMovement(Steering, DeltaTime);
}

void ABeeActor::HandleIdleState()
{
    if (GeneticProfile.BeeType == EBeeType::Scout)
    {
        BeeState = EBeeState::Scouting;
    }
    else if (GeneticProfile.BeeType == EBeeType::Gatherer)
    {
        FindNewTarget();
        if (TargetActor)
        {
            BeeState = EBeeState::MovingToTarget;
        }
    }
}

void ABeeActor::HandleScoutingState()
{
    FindNewTarget();
    if (TargetActor)
    {
        BeeState = EBeeState::MovingToTarget;
    }
}

void ABeeActor::HandleMovingToTargetState()
{
    if (!TargetActor)
    {
        BeeState = EBeeState::Idle;
        return;
    }

    if (FVector::DistSquared(GetActorLocation(), TargetActor->GetActorLocation()) < FMath::Square(InteractionDistance))
    {
        BeeState = EBeeState::Interacting;
    }
}

void ABeeActor::HandleReturningToHiveState()
{
    if (FVector::DistSquared(GetActorLocation(), Manager->GetActorLocation()) < FMath::Square(InteractionDistance))
    {
        if (DiscoveredTargetID.IsValid())
        {
            Manager->PromoteToTangible(DiscoveredTargetID);
            DiscoveredTargetID.Invalidate();
        }

        if (CurrentPayload > 0)
        {
            Manager->DepositPollen(CurrentPayload);
            CurrentPayload = 0;
        }
        BeeState = EBeeState::Idle;
    }
}

void ABeeActor::HandleInteractingState()
{
    if (!TargetActor)
    {
        BeeState = EBeeState::Idle;
        return;
    }

    USaveableEntityComponent* TargetSaveComponent = TargetActor->FindComponentByClass<USaveableEntityComponent>();
    if(TargetSaveComponent)
    {
        if (GeneticProfile.BeeType == EBeeType::Scout)
        {
            float Extracted = InteractionComponent->TryAIInteract(TargetActor, 0.1f, GeneticProfile.LineageID);
            if (Extracted > 0)
            {
                CurrentPayload += Extracted;
                DiscoveredTargetID = TargetSaveComponent->InstanceID;
            }
            else
            {
                Manager->DemoteToUnknown(TargetSaveComponent->InstanceID);
            }
            TargetActor = Manager;
            BeeState = EBeeState::ReturningToHive;
        }
        else if (GeneticProfile.BeeType == EBeeType::Gatherer && TargetActor != Manager)
        {
            if (Manager->GetTruthState(TargetSaveComponent->InstanceID) == ETruthState::Tangible)
            {
                float Extracted = InteractionComponent->TryAIInteract(TargetActor, MaxPayload - CurrentPayload, GeneticProfile.LineageID);
                if (Extracted > 0)
                {
                    CurrentPayload += Extracted;
                    TargetActor = Manager;
                    BeeState = EBeeState::ReturningToHive;
                }
                else
                {
                    Manager->DemoteToUnknown(TargetSaveComponent->InstanceID);
                    TargetActor = nullptr;
                    BeeState = EBeeState::Idle;
                }
            }
        }
    }
    else
    {
        BeeState = EBeeState::Idle;
    }
}


void ABeeActor::HandleRestingState()
{
    if (!Manager)
    {
        BeeState = EBeeState::Idle;
        return;
    }

    if (FVector::DistSquared(GetActorLocation(), Manager->GetActorLocation()) < FMath::Square(InteractionDistance))
    {
        if (QiComponent->QiData.CurrentQi < QiComponent->QiData.MaxQi)
        {
            float HoneyNeeded = (QiComponent->QiData.MaxQi - QiComponent->QiData.CurrentQi) * 0.5f; // Conversion rate
            float HoneyReceived = Manager->RequestHoney(HoneyNeeded);
            QiComponent->QiData.CurrentQi += HoneyReceived * 2.0f; // Inverse conversion
        }
        else
        {
            BeeState = EBeeState::Idle;
            TargetActor = nullptr;
        }
    }
}

bool ABeeActor::ValidatePhysicalSymmetry(FVector TargetLocation) {
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    return !GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), TargetLocation, ECC_Visibility, CollisionParams);
}

void ABeeActor::EnterSuccessionState()
{
    if (Manager)
    {
        Manager->UnregisterBee(this);
    }
    Destroy();
}
