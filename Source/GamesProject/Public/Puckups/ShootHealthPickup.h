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
	//���������� ������ ������� ����� ��������� ����� ������ Pickup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
		float HealthAmount = 100.0f;


private:

	//�������������� � Pickup ��������
	virtual bool GivePickupTo(APawn* PlayerPawn) override;	//���������� ������ ���� ������� �����. �������� ������� ����, ��� �� �������� � ��������� ��������� ������ � ������ � ������������ ������ ��������� �� ��������� Pickup

};
