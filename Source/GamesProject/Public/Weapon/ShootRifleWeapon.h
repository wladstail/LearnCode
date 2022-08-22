// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShootBaseWeapon.h"
#include "ShootRifleWeapon.generated.h"

class UShootWeaponFXComponent;	//эффект ниагры, сделали тут т.к. в гранатомете логический хит и за столкновение отвечает снаряд

UCLASS()
class GAMESPROJECT_API AShootRifleWeapon : public AShootBaseWeapon
{
	GENERATED_BODY()
	
public:
	//конструктор класса
	AShootRifleWeapon();

	virtual void  StartFire() override;	//сделали вируатльной потому что будут наследники BaseWeapon в будущем и оружие стрелять будет по разному
	virtual void  StopFire() override;	//сделали вируатльной потому что будут наследники BaseWeapon в будущем и оружие стрелять будет по разному

protected:

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)*/		//частота срабатывания при ЛКМ
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float TimeBetweenShots = 0.1f;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)*/		//отклонение при ЛКМ
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float BulletSpread = 1.5f;							//в градусах

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float DamageAmount = 10.0f;

		//Визуальный эффект
		UPROPERTY(VisibleAnywhere, Category = "VFX")
		UShootWeaponFXComponent* WeaponFXComponent;

		//создали что бы проверить, что наш компонент визуальный существует
	virtual void BeginPlay() override;
	virtual void MakeShot() override;//логика выстрела из оружия
	virtual bool GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const override;


private:
	FTimerHandle ShotTimerHandle;

	void MakeDamage(const FHitResult& HitResult);

};