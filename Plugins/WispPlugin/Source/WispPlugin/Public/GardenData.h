#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "GardenData.generated.h"

UENUM(BlueprintType)
enum class EUpdateFrequency : uint8
{
	High     UMETA(DisplayName = "High (Every Few Seconds)"),
	Medium   UMETA(DisplayName = "Medium (Every Minute)"),
	Low      UMETA(DisplayName = "Low (Every Hour)"),
	Never    UMETA(DisplayName = "Never (Only Updates on Action)")
};


UENUM(BlueprintType)
enum class ETrainingFocus : uint8
	//Every savable class has a state where depending on its tag or class it will be training in a certain way.
	// This system although is on the savable enity level it is kinda important to have across the board. As every item can grow over time and this system manages how they do that groweth
{
	TF_None				UMETA(DisplayName = "None"),
	TF_Qi				UMETA(DisplayName = "Qi"),
	TF_Strength			UMETA(DisplayName = "Strength"),
	TF_Dexterity		UMETA(DisplayName = "Dexterity"),
	TF_Constituion		UMETA(DisplayName = "Constiution"),
	TF_Wisdom			UMETA(DisplayName = "Wisdom"),
	TF_Intelligence		UMETA(DisplayName = "intelligence")
};
//Work in progress idea to create an enum for plants and there state stored as an int. 
//Current Groweth state of a plant (this my need to be rethought)
UENUM(BlueprintType)
enum class EGrowthState : uint8

	//SO in theory i can store a number and now know its tag. 
	//This tag is used later on when spawning in object and defining Gameplaytag for the general gameplay logic from the ground up. 
	// Is it worth me making more tags for aniomals etc or just lump it into 1 generic ENUM?

{
	PS_0		UMETA(DisplayName = "Seed"), // Awating germination
	PS_1		UMETA(DisplayName = "Sprout"), // Begins to germinate and has a blanced focus on root/leaf groweth
	PS_2		UMETA(DisplayName = "Seedling"), // Begin creating roots and true leaves
	PS_3		UMETA(DisplayName = "Vegetative"), // groweth in roots
	PS_4		UMETA(DisplayName = "Budding"), // creating a flower
	PS_5		UMETA(DisplayName = "Flowering"), // too atempot to fetilise a new seed
	PS_6		UMETA(DisplayName = "Ripening"), //to create a new seed
	PS_7		UMETA(DisplayName = "Maturing")// Will use this state to increase mass groweth
};

// This struct holds the data that is COMMON to EVERY saveable entity
USTRUCT(BlueprintType)
struct FEntitySaveData
{
	GENERATED_BODY()

	//dont forget i have a savable class do i need to declare this twice?

	//why wont this update accordiungly
	//Twitch GUUID Implementation
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//FGuid OwnerTwitchID; 
	//ideally i wanna replace this with state so every saveable item has a state it runs it core spawning and training logic


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag EntityTypeTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform WorldTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETrainingFocus CurrentFocus;

	// Current work in progress
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGrowthState CurrentState;



};

// This struct holds data ONLY for soil
USTRUCT(BlueprintType)
struct FSoilData
{
	GENERATED_BODY()

	// The last real-world time this soil cell was updated by the manager.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDateTime LastUpdateTime;

	//How Efficent this soil can upgrade as a percentage
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float GatherEfficency;


	// How fertile is the soil for plant growth etc, or later mineral quality
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Fertility;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FertilityMax;

	//How much this piece of soil is upgrading by
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float IncreaseFertilityBaseAmount;

	//How dense is the soil denser the level it begins to turn to rock
	// Think in nature rock with carbon from plants etc turn to oil, coal etc
	//A system for metals needs to be created
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Density;
	//How dense is the soil denser the level it begins to turn to rock
// Think in nature rock with carbon from plants etc turn to oil, coal etc
//A system for metals needs to be created

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DensityMax;

	//How much this piece of soil is upgrading by
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float IncreaseCompactionBaseAmount;


	// A tag to hold the current state of the soil
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag SoilStateTag;
	//In the editor, you would then define tags like
	// Soil.State.Normal, Soil.State.Fertile, and Soil.State.Rock.

};

// This struct holds data ONLY for Qi variables
USTRUCT(BlueprintType)
struct FQiData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentQi = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxQi = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BaseQiPerSecond = 1.f;
};

// To hold key attributes
//Can be assigned as a compnent to anything these Variables can be redifined here, this is an example
USTRUCT(BlueprintType)
struct FAtrributesData
{

	GENERATED_BODY()

	// CoreStats Like in DND

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int AStrength; //Used for leveling logic

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int ADexterity; //User to represrnt the current amount of qi one has

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int AConsitution; // the maximium amount of qi one can hold

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int AWisdom;// to to calculate fration of qi so if it hit  1 it can be added this is added to current Qi, can serve as a base so any world modifiers are added to this in gametime.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int AIntelligence;// to to calculate fration of qi so if it hit  1 it can be added this is added to current Qi, can serve as a base so any world modifiers are added to this in gametime.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int ALuck;// to to calculate fration of qi so if it hit  1 it can be added this is added to current Qi, can serve as a base so any world modifiers are added to this in gametime.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int ACharisma;// to to calculate fration of qi so if it hit  1 it can be added this is added to current Qi, can serve as a base so any world modifiers are added to this in gametime.


	//Ressilence Based Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int AResilience;// to to calculate fration of qi so if it hit  1 it can be added this is added to current Qi, can serve as a base so any world modifiers are added to this in gametime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int AFortitude;// to to calculate fration of qi so if it hit  1 it can be added this is added to current Qi, can serve as a base so any world modifiers are added to this in gametime.


};

// This struct holds data ONLY for plants and their parents/class. 
USTRUCT(BlueprintType)
struct FPlantData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Growth;

	// this should let the plant know what its x,y and therefor work out its soil.
	// if the plants moves this would need to be updated and a special function would have to be called that superceeds the main frequency thing. If important. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntPoint MySoilIntPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSoilData MySoilData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float GrowthMax;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int GrowthStage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Maturity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Scale;//might need to be a transform of x y and z

	//Work In progress
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag PlantStateTag;//Seedling // Sprout // Sapling // x , y ,z 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag PlantSpeciesTag; // Oak // Willow // Flower no clear speration at this stage for like family branches tree, conifers, to a small grass or flower this can add rendering and living simulation fidility in the future?etc//
};

// This struct holds data ONLY for animals (insects, mammanls etc.)
USTRUCT(BlueprintType)
struct FAnimalData
{
	GENERATED_BODY()


	//just a place holder for now i know i want butterflies that help plants, and worms that help soil

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag AnimalSpeciesTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentEnergy;
};

// This struct holds the rich data ONLY for the main Dragon/Player entity
// NOTE: We have removed the inheritance from FTableRowBase as it's not needed here.
USTRUCT(BlueprintType)
struct FFairyData
{
	GENERATED_BODY()

public:

	// this eventually becomes an ai assistant in the garden that does taks to make the garden more fetile, dense or magical
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name = "Default Fairy";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Gold = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int FairyPower = 1;

};

// This struct holds the rich data ONLY for the main Dragon/Player entity
// NOTE: We have removed the inheritance from FTableRowBase as it's not needed here.

USTRUCT(BlueprintType)
struct FDragonData
{
	GENERATED_BODY()

public:

	// every garden starts with a dragon egg.
	// dragons are elemental by nature so they need at least 1 affintity. (Null/ Light or dark, and an element Fire, water, earth, and you can get a gem stone Ruby, Shappire and Emerald, And a metalic form Iron, Copper, SIlver, Gold,  )
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name = "Default Dragon";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int WispsPower = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Gold = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Silver = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Copper = 1;

	// Health/Stamina are handled by components, but we might have special max values here
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HealthMax = 100.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StaminaMax = 100.f;
};


// The FINAL master save struct that will be written to the .json file
USTRUCT(BlueprintType)
struct FSaveGameData
{
	GENERATED_BODY()

public:

	// This is the master list of every saveable object in the world.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGuid, FEntitySaveData> AllEntities;

	// These maps hold the unique data for specific types of entities.
	// An entity will only have an entry in the maps that are relevant to it.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGuid, FSoilData> AllSoilData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGuid, FPlantData> AllPlantData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGuid, FAnimalData> AllAnimalData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGuid, FFairyData> AllFairyData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGuid, FDragonData> AllDragonData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGuid, FQiData> AllQiData;

	// The new grid that holds all soil data for the entire map , thi slogic is more scalable as it does not reuire the soil to be an actor but can be referenced in a tMap by looking up an X and Y value.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FIntPoint, FSoilData> SoilDataGrid;

};