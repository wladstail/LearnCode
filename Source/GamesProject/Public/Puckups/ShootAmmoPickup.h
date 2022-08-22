// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "Puckups/ShootBasePickup.h"
#include "ShootAmmoPickup.generated.h"

class AShootBaseWeapon;

UCLASS()
class GAMESPROJECT_API AShootAmmoPickup : public AShootBasePickup
{
	GENERATED_BODY()

protected:

	//параметры оружия у Pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
		int32 ClipsAmount = 10;

	//тип оружия
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		TSubclassOf<AShootBaseWeapon> WeaponType;

private:
	//переопределять в Pickup боеприпасов
	virtual bool GivePickupTo(APawn* PlayerPawn) override;	//возвращает истину если удалось взять. Параметр сделали этот, что бы получать у персонажа компонент оружия и жизней и видоизменять данные персонажа от параметра Pickup
	
};
