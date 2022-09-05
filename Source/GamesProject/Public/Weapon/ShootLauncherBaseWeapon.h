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
		//������� ���������� 
		virtual void StartFire() override;

protected:
	//����� ������� ��� ����� ����������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AShootProjectile> ProjectileClass;	//TSubclassOf - �����, AShootProjectile - ����� �������, ProjectileClass - ��������

	//������� ���������� 
	virtual void MakeShot() override;
};
