#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Shoot_CharacterMovementComponent.generated.h"


UCLASS()
class GAMESPROJECT_API UShoot_CharacterMovementComponent : public UCharacterMovementComponent		//потом сделали так, что бы вместо базового компонента, был наш
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.5", ClampMax = "4.0"))
		float RunModifier = 2.0f;

		virtual float GetMaxSpeed() const override;

};
