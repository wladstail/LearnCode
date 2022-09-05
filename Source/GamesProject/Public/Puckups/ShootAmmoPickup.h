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

	//��������� ������ � Pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
		int32 ClipsAmount = 10;

	//��� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		TSubclassOf<AShootBaseWeapon> WeaponType;

private:
	//�������������� � Pickup �����������
	virtual bool GivePickupTo(APawn* PlayerPawn) override;	//���������� ������ ���� ������� �����. �������� ������� ����, ��� �� �������� � ��������� ��������� ������ � ������ � ������������ ������ ��������� �� ��������� Pickup
	
};
