// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShootBaseWeapon.h"
#include "ShootRifleWeapon.generated.h"

class UShootWeaponFXComponent;	//������ ������, ������� ��� �.�. � ����������� ���������� ��� � �� ������������ �������� ������

UCLASS()
class GAMESPROJECT_API AShootRifleWeapon : public AShootBaseWeapon
{
	GENERATED_BODY()
	
public:
	//����������� ������
	AShootRifleWeapon();

	virtual void  StartFire() override;	//������� ����������� ������ ��� ����� ���������� BaseWeapon � ������� � ������ �������� ����� �� �������
	virtual void  StopFire() override;	//������� ����������� ������ ��� ����� ���������� BaseWeapon � ������� � ������ �������� ����� �� �������

protected:

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)*/		//������� ������������ ��� ���
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float TimeBetweenShots = 0.1f;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)*/		//���������� ��� ���
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float BulletSpread = 1.5f;							//� ��������

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float DamageAmount = 10.0f;

		//���������� ������
		UPROPERTY(VisibleAnywhere, Category = "VFX")
		UShootWeaponFXComponent* WeaponFXComponent;

		//������� ��� �� ���������, ��� ��� ��������� ���������� ����������
	virtual void BeginPlay() override;
	virtual void MakeShot() override;//������ �������� �� ������
	virtual bool GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const override;


private:
	FTimerHandle ShotTimerHandle;

	void MakeDamage(const FHitResult& HitResult);

};