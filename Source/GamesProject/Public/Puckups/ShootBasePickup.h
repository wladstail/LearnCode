
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootBasePickup.generated.h"

//��� �������� ������� ���������
class USphereComponent;

UCLASS()
class GAMESPROJECT_API AShootBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AShootBasePickup();

protected:
	
	//������� ����������� ��������
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
		USphereComponent* CollisionComponent;
		
	//����� ��������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
		float RespawnTime = 5.0f;

	virtual void BeginPlay() override;

	//�������������� ������� ���������������
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	
	virtual void Tick(float DeltaTime) override;

private:
	//���� �������� Pickup
	float RotationYaw = 0.0f;


	//������, ��� �� �������������� � Pickup �������� � �����������
	virtual bool GivePickupTo(APawn* PlayerPawn);	//���������� ������ ���� ������� �����. �������� ������� ����, ��� �� �������� � ��������� ��������� ������ � ������ � ������������ ������ ��������� �� ��������� Pickup

	//���������� ����� ����� Pickup
	void PickupWasTaken();

	//����������� �� ������������ ������� ��������
	void Respawn();

	//��������� ����������� ��� ����
	void GenerateRotationYaw();

};
