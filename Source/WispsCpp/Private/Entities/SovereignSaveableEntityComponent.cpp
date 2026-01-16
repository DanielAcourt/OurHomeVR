// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "Entities/SovereignSaveableEntityComponent.h"
#include "Entities/SovereignBaseEntity.h"
#include "SaveSystem/SovereignActorRegistry.h"
#include "Engine/World.h"

USovereignSaveableEntityComponent::USovereignSaveableEntityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Generate a unique ID for this entity upon creation.
	EntityID = FGuid::NewGuid();
}

void USovereignSaveableEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	// If for some reason the ID is invalid (e.g., from an old asset), generate a new one.
	if (!EntityID.IsValid())
	{
		EntityID = FGuid::NewGuid();
	}

	// Register this entity with the world's central registry.
	if (UWorld* World = GetWorld())
	{
		if (USovereignActorRegistry* Registry = World->GetSubsystem<USovereignActorRegistry>())
		{
			Registry->RegisterActor(EntityID, GetOwner());
		}
	}

	// Establish a baseline VSS score on spawn.
	RefreshVerifiedStatus();
}

void USovereignSaveableEntityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unregister this entity from the world's central registry upon its destruction.
	if (UWorld* World = GetWorld())
	{
		if (USovereignActorRegistry* Registry = World->GetSubsystem<USovereignActorRegistry>())
		{
			Registry->UnregisterActor(EntityID);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void USovereignSaveableEntityComponent::ReceiveElementalEnergy(ESovereignElement EnergyType, float RawAmount)
{
	// 1. AXIS LOGIC: Shift the correct influence based on the incoming energy type.
	if (EnergyType == ESovereignElement::Light || EnergyType == ESovereignElement::Dark)
	{
		if (EnergyType == AlignmentSocket)
		{
			AlignmentInfluence = FMath::Clamp(AlignmentInfluence + (RawAmount * 5.0f), 0.0f, 100.0f);
		}
		else
		{
			AlignmentInfluence -= (RawAmount * 2.0f);
			if (AlignmentInfluence <= 0.0f)
			{
				AlignmentSocket = EnergyType;
				AlignmentInfluence = 1.0f;
			}
		}
	}
	// Note: This logic assumes Body and Magic elements are mutually exclusive categories.
	else if (EnergyType == ESovereignElement::Fairy || EnergyType == ESovereignElement::Dragon || EnergyType == ESovereignElement::Electric)
	{
		// Logic for Magic socket would go here
	}
	else // Assumes any other valid type is a Body element
	{
		if (EnergyType == BodySocket)
		{
			BodyInfluence = FMath::Clamp(BodyInfluence + (RawAmount * 5.0f), 0.0f, 100.0f);
		}
		else
		{
			BodyInfluence -= (RawAmount * 2.0f);
			if (BodyInfluence <= 0.0f)
			{
				BodySocket = EnergyType;
				BodyInfluence = 1.0f;
			}
		}
	}

	// 2. YIELD LOGIC: Calculate growth based on elemental interactions.
	const float Modifier = GetElementalMultiplier(EnergyType);
	MaturityProgress += (RawAmount * Modifier * MaturityRate);

	UE_LOG(LogTemp, Log, TEXT("Sovereign: %s received %f %s energy. New Maturity: %f"),
		*GetOwner()->GetName(), RawAmount, *UEnum::GetValueAsString(EnergyType), MaturityProgress);

	// 3. EVOLUTION CHECK
	if (MaturityProgress >= 1.0f)
	{
		if (ASovereignBaseEntity* MyEntity = Cast<ASovereignBaseEntity>(GetOwner()))
		{
			MyEntity->Evolve();
		}
	}
}

float USovereignSaveableEntityComponent::GetElementalMultiplier(ESovereignElement IncomingType)
{
	float Multiplier = 1.0f;

	// --- 1. SPECIAL/MAGIC (Fairy/Dragon) ---
	if (IncomingType == ESovereignElement::Fairy || IncomingType == ESovereignElement::Dragon) return 1000.0f;

	// --- 2. THE 5-WAY BATTLE CYCLE (Body) ---
	if (BodySocket == ESovereignElement::Nature) {
		if (IncomingType == ESovereignElement::Water) Multiplier = 2.0f;
		if (IncomingType == ESovereignElement::Fire)  Multiplier = 0.5f;
	}
	else if (BodySocket == ESovereignElement::Earth) {
		if (IncomingType == ESovereignElement::Nature) Multiplier = 0.5f;
		if (IncomingType == ESovereignElement::Water)  Multiplier = 2.0f;
	}
	else if (BodySocket == ESovereignElement::Water) {
		if (IncomingType == ESovereignElement::Fire)   Multiplier = 2.0f;
		if (IncomingType == ESovereignElement::Nature) Multiplier = 0.5f;
	}
	else if (BodySocket == ESovereignElement::Fire) {
		if (IncomingType == ESovereignElement::Nature) Multiplier = 2.0f;
		if (IncomingType == ESovereignElement::Water)  Multiplier = 0.5f;
	}
	else if (BodySocket == ESovereignElement::Air) {
		if (IncomingType == ESovereignElement::Fire)   Multiplier = 2.0f;
		if (IncomingType == ESovereignElement::Earth)  Multiplier = 0.5f;
	}

	// --- 3. ALIGNMENT DUALITY ---
	if ((AlignmentSocket == ESovereignElement::Light && IncomingType == ESovereignElement::Dark) ||
		(AlignmentSocket == ESovereignElement::Dark && IncomingType == ESovereignElement::Light))
	{
		Multiplier *= 0.25f;
	}

	return Multiplier;
}

TMap<FString, FString> USovereignSaveableEntityComponent::GetActorMetaTags() const
{
	TMap<FString, FString> FoundTags;
	if (const AActor* Owner = GetOwner())
	{
		for (const FName& TagName : Owner->Tags)
		{
			FString TagString = TagName.ToString();
			FString Key, Value;
			if (TagString.Split(TEXT(":"), &Key, &Value))
			{
				FoundTags.Add(Key.TrimStartAndEnd(), Value.TrimStartAndEnd());
			}
		}
	}
	return FoundTags;
}

void USovereignSaveableEntityComponent::ApplyMetaTags(const TMap<FString, FString>& LoadedTags)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// This function now handles deserializing ALL data from the MetaTags map.

	// Clear any old, dynamically generated tags before applying the new ones.
	for (int32 i = Owner->Tags.Num() - 1; i >= 0; --i)
	{
		if (Owner->Tags[i].ToString().Contains(TEXT(":")))
		{
			Owner->Tags.RemoveAt(i);
		}
	}

	for (const auto& Elem : LoadedTags)
	{
		// Reconstruct the simple "Key:Value" actor tags.
		const FString ReconstructedTag = FString::Printf(TEXT("%s:%s"), *Elem.Key, *Elem.Value);
		Owner->Tags.AddUnique(FName(*ReconstructedTag));
	}

    // DESERIALIZE PSTAS SCORES
    if (LoadedTags.Contains(TEXT("PSTAS_P")))
    {
        PSTAS.P_Score = FCString::Atof(*LoadedTags.Find(TEXT("PSTAS_P")));
    }
    if (LoadedTags.Contains(TEXT("PSTAS_S")))
    {
        PSTAS.S_Score = FCString::Atof(*LoadedTags.Find(TEXT("PSTAS_S")));
    }
    if (LoadedTags.Contains(TEXT("PSTAS_T")))
    {
        PSTAS.T_Score = FCString::Atof(*LoadedTags.Find(TEXT("PSTAS_T")));
    }
    if (LoadedTags.Contains(TEXT("PSTAS_A")))
    {
        PSTAS.A_Score = FCString::Atof(*LoadedTags.Find(TEXT("PSTAS_A")));
    }

    // DESERIALIZE THIS COMPONENT'S OWN STATE
    UEnum* const ElementEnum = StaticEnum<ESovereignElement>();
    if (LoadedTags.Contains(TEXT("Genetics.AlignmentSocket")))
    {
        AlignmentSocket = (ESovereignElement)ElementEnum->GetValueByNameString(*LoadedTags.Find(TEXT("Genetics.AlignmentSocket")));
    }
    if (LoadedTags.Contains(TEXT("Genetics.BodySocket")))
    {
        BodySocket = (ESovereignElement)ElementEnum->GetValueByNameString(*LoadedTags.Find(TEXT("Genetics.BodySocket")));
    }
    if (LoadedTags.Contains(TEXT("Genetics.MagicSocket")))
    {
        MagicSocket = (ESovereignElement)ElementEnum->GetValueByNameString(*LoadedTags.Find(TEXT("Genetics.MagicSocket")));
    }
    if (LoadedTags.Contains(TEXT("Genetics.AlignmentInfluence")))
    {
        AlignmentInfluence = FCString::Atof(*LoadedTags.Find(TEXT("Genetics.AlignmentInfluence")));
    }
    if (LoadedTags.Contains(TEXT("Genetics.BodyInfluence")))
    {
        BodyInfluence = FCString::Atof(*LoadedTags.Find(TEXT("Genetics.BodyInfluence")));
    }
    if (LoadedTags.Contains(TEXT("Genetics.MagicInfluence")))
    {
        MagicInfluence = FCString::Atof(*LoadedTags.Find(TEXT("Genetics.MagicInfluence")));
    }
    if (LoadedTags.Contains(TEXT("Evolution.MaturityProgress")))
    {
        MaturityProgress = FCString::Atof(*LoadedTags.Find(TEXT("Evolution.MaturityProgress")));
    }
}

TMap<FString, FString> USovereignSaveableEntityComponent::CaptureFullEntityState()
{
	TMap<FString, FString> AggregatedData;

	// 1. SCRAPE META-TAGS from the actor level (e.g., "Role:Guardian").
	AggregatedData.Append(GetActorMetaTags());

	// 2. SCRAPE DATA from any other component on the actor that implements the save interface.
	// 2. SCRAPE DATA from any other component on the actor that implements the save interface.
	TArray<UActorComponent*> InterfaceComponents;
	GetOwner()->GetComponents(InterfaceComponents);

	for (UActorComponent* Component : InterfaceComponents)
	{
		if (Component && Component != this && Component->GetClass()->ImplementsInterface(USovereignSaveInterface::StaticClass()))
		{
			TMap<FName, FString> ComponentData = ISovereignSaveInterface::Execute_GetSaveData(Component);
			for (const auto& Pair : ComponentData)
			{
				AggregatedData.Add(Pair.Key.ToString(), Pair.Value);
			}
		}
	}

    // 3. SERIALIZE THIS COMPONENT'S OWN STATE
    UEnum* const ElementEnum = StaticEnum<ESovereignElement>();
    AggregatedData.Add(TEXT("Genetics.AlignmentSocket"), ElementEnum->GetNameStringByValue((int64)AlignmentSocket));
    AggregatedData.Add(TEXT("Genetics.BodySocket"), ElementEnum->GetNameStringByValue((int64)BodySocket));
    AggregatedData.Add(TEXT("Genetics.MagicSocket"), ElementEnum->GetNameStringByValue((int64)MagicSocket));
    AggregatedData.Add(TEXT("Genetics.AlignmentInfluence"), FString::SanitizeFloat(AlignmentInfluence));
    AggregatedData.Add(TEXT("Genetics.BodyInfluence"), FString::SanitizeFloat(BodyInfluence));
    AggregatedData.Add(TEXT("Genetics.MagicInfluence"), FString::SanitizeFloat(MagicInfluence));
    AggregatedData.Add(TEXT("Evolution.MaturityProgress"), FString::SanitizeFloat(MaturityProgress));

    // 4. SERIALIZE PSTAS SCORES
    AggregatedData.Add(TEXT("PSTAS_P"), FString::SanitizeFloat(PSTAS.P_Score));
    AggregatedData.Add(TEXT("PSTAS_S"), FString::SanitizeFloat(PSTAS.S_Score));
    AggregatedData.Add(TEXT("PSTAS_T"), FString::SanitizeFloat(PSTAS.T_Score));
    AggregatedData.Add(TEXT("PSTAS_A"), FString::SanitizeFloat(PSTAS.A_Score));

	return AggregatedData;
}

namespace PSTAS_Internal
{
    // Helper struct for parsing and holding the raw data points for a single dimension.
    struct FPSTASDatum
    {
        float Value;  // The x_ij value (0-100)
        float Weight; // The w_ij weight

        FPSTASDatum(float InValue, float InWeight) : Value(InValue), Weight(InWeight) {}
    };

    // Enum to strongly type our TMap keys.
    enum class EPSTASDimension : uint8
    {
        Psychological,
        Social,
        Technical,
        Administrative
    };
}

void USovereignSaveableEntityComponent::RefreshVerifiedStatus()
{
    // --- 1. AGGREGATION ---
    TMap<PSTAS_Internal::EPSTASDimension, TArray<PSTAS_Internal::FPSTASDatum>> AggregatedData;

    // 1a. Scrape raw actor tags for PSTAS data points.
    TMap<FString, FString> ActorTags = GetActorMetaTags();
    for (const auto& Elem : ActorTags)
    {
        if (Elem.Key.StartsWith(TEXT("PSTAS.")))
        {
            TArray<FString> Parts;
            Elem.Key.ParseIntoArray(Parts, TEXT("."), true);

            if (Parts.Num() >= 2)
            {
                PSTAS_Internal::EPSTASDimension Dimension;
                if (Parts[1].Equals(TEXT("P"))) Dimension = PSTAS_Internal::EPSTASDimension::Psychological;
                else if (Parts[1].Equals(TEXT("S"))) Dimension = PSTAS_Internal::EPSTASDimension::Social;
                else if (Parts[1].Equals(TEXT("T"))) Dimension = PSTAS_Internal::EPSTASDimension::Technical;
                else if (Parts[1].Equals(TEXT("A"))) Dimension = PSTAS_Internal::EPSTASDimension::Administrative;
                else continue;

                const float Value = FCString::Atof(*Elem.Value);
                // For now, we assume a default weight of 1.0 for all MetaTag entries.
                // This could be expanded later by parsing the key, e.g., "PSTAS.T.Pathfinding:0.8"
                const float Weight = 1.0f;

                AggregatedData.FindOrAdd(Dimension).Add(PSTAS_Internal::FPSTASDatum(Value, Weight));
            }
        }
    }

    // 1b. (Future) Check Binary Bitmask for hard failures.
    // For now, we can add a baseline "Administrative" score.
    const float AdminScore = EntityID.IsValid() ? 100.0f : 0.0f;
    AggregatedData.FindOrAdd(PSTAS_Internal::EPSTASDimension::Administrative).Add(PSTAS_Internal::FPSTASDatum(AdminScore, 1.0f));

    // --- 2. DIMENSIONAL HEALTH CALCULATION (D_i) ---
    TMap<PSTAS_Internal::EPSTASDimension, float> DimensionalHealthScores;
    float MinDimensionScore = 100.0f;

    for (auto const& Pair : AggregatedData)
    {
        float TotalWeightedValue = 0.0f;
        float TotalWeight = 0.0f;

        for (const auto& Datum : Pair.Value)
        {
            TotalWeightedValue += (Datum.Value * Datum.Weight);
            TotalWeight += Datum.Weight;
        }

        const float DimensionalHealth = (TotalWeight > 0) ? (TotalWeightedValue / TotalWeight) : 0.0f;
        DimensionalHealthScores.Add(Pair.Key, DimensionalHealth);

        if (DimensionalHealth < MinDimensionScore)
        {
            MinDimensionScore = DimensionalHealth;
        }
    }

    // Update the public-facing PSTAS struct
    PSTAS.P_Score = DimensionalHealthScores.FindRef(PSTAS_Internal::EPSTASDimension::Psychological);
    PSTAS.S_Score = DimensionalHealthScores.FindRef(PSTAS_Internal::EPSTASDimension::Social);
    PSTAS.T_Score = DimensionalHealthScores.FindRef(PSTAS_Internal::EPSTASDimension::Technical);
    PSTAS.A_Score = DimensionalHealthScores.FindRef(PSTAS_Internal::EPSTASDimension::Administrative);

    // --- 3. FINAL VSS CALCULATION ---
    // For now, we use simple, equal weighting (alpha = 0.25 for each).
    const float Alpha = 0.25f;
    float WeightedSum = 0.0f;
    for (auto const& Pair : DimensionalHealthScores)
    {
        WeightedSum += (Pair.Value * Alpha);
    }

    // --- 4. FINAL VSS CALCULATION & STATUS MAPPING ---
    const float VSS = MinDimensionScore * WeightedSum;

    if (VSS <= 0)
    {
        CurrentVSS = EVerifiedSafetyStatus::Critical;
    }
    else if (VSS <= 35)
    {
        CurrentVSS = EVerifiedSafetyStatus::Warning;
    }
    else if (VSS <= 70)
    {
        CurrentVSS = EVerifiedSafetyStatus::Caution;
    }
    else
    {
        CurrentVSS = EVerifiedSafetyStatus::Nominal;
    }

    UE_LOG(LogTemp, Log, TEXT("Refreshed VSS for %s. P:%.2f, S:%.2f, T:%.2f, A:%.2f. Final VSS: %.2f (%s)"),
        *GetOwner()->GetName(), PSTAS.P_Score, PSTAS.S_Score, PSTAS.T_Score, PSTAS.A_Score, VSS, *UEnum::GetValueAsString(CurrentVSS));
}

#if WITH_EDITOR
void USovereignSaveableEntityComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// This ensures that if you manually invalidate a GUID in the editor,
	// it is immediately regenerated to prevent saving invalid data.
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USovereignSaveableEntityComponent, EntityID))
	{
		if (!EntityID.IsValid())
		{
			EntityID = FGuid::NewGuid();
		}
	}
}

void USovereignSaveableEntityComponent::PostDuplicate(bool bDuplicateForPIE)
{
	Super::PostDuplicate(bDuplicateForPIE);

	// When an object is duplicated in the editor, it MUST get a new ID.
	// We ignore duplication for "Play in Editor" to maintain state between sessions.
	if (!bDuplicateForPIE)
	{
		EntityID = FGuid::NewGuid();
	}
}
#endif
