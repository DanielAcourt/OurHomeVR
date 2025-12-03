#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "WispPawn.generated.h"

/**
 * @class AWispPawn
 * @brief The player's pawn in the game.
 *
 * This class represents the player's character in the world.
 */
UCLASS()
class WISPPLUGIN_API AWispPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	AWispPawn();
};
