// Shoot Game

#pragma once


#include "Weapon/ShootBaseWeapon.h"
#include "ShootLauncherBaseWeapon.generated.h"


class AShootProjectile;

UCLASS()
class GAMESPROJECT_API AShootLauncherBaseWeapon : public AShootBaseWeapon
{
	GENERATED_BODY()
	
public:
		//изменим функционал 
		virtual void StartFire() override;

protected:
	//класс снаряда что хотим заспаунить
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AShootProjectile> ProjectileClass;	//TSubclassOf - спавн, AShootProjectile - класс объекта, ProjectileClass - название

	//изменим функционал 
	virtual void MakeShot() override;
};
