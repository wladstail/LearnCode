// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootDevDamageActor.generated.h"

//создали что бы у него была трансформация на сцене
class USceneComponent;

UCLASS()
class GAMESPROJECT_API AShootDevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShootDevDamageActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) //настройки для трансформации
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Radios = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor SphereColor = FColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)		//распределение урона от центра
		bool DoFullDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)		//тип урона выбор
		TSubclassOf<UDamageType> DamageType;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
