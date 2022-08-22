
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootBasePickup.generated.h"

//для коллизии форвард директива
class USphereComponent;

UCLASS()
class GAMESPROJECT_API AShootBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AShootBasePickup();

protected:
	
	//создаем сферическую коллизию
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
		USphereComponent* CollisionComponent;
		
	//время респауна
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
		float RespawnTime = 5.0f;

	virtual void BeginPlay() override;

	//переопределяем функцию соприкосновения
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	
	virtual void Tick(float DeltaTime) override;

private:
	//угол вращения Pickup
	float RotationYaw = 0.0f;


	//делаем, что бы переопределять в Pickup здоровья и боеприпасов
	virtual bool GivePickupTo(APawn* PlayerPawn);	//возвращает истину если удалось взять. Параметр сделали этот, что бы получать у персонажа компонент оружия и жизней и видоизменять данные персонажа от параметра Pickup

	//Вызывается когда взяли Pickup
	void PickupWasTaken();

	//срабатывает по срабатыванию таймера респавна
	void Respawn();

	//вычисляет необходимый нам угол
	void GenerateRotationYaw();

};
