// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "Puckups/ShootBasePickup.h"
#include "ShootHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class GAMESPROJECT_API AShootHealthPickup : public AShootBasePickup
{
	GENERATED_BODY()
	
protected:
	//поличество жизней которое хотим прибавить после взятия Pickup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
		float HealthAmount = 100.0f;


private:

	//переопределять в Pickup здоровья
	virtual bool GivePickupTo(APawn* PlayerPawn) override;	//возвращает истину если удалось взять. Параметр сделали этот, что бы получать у персонажа компонент оружия и жизней и видоизменять данные персонажа от параметра Pickup

};
