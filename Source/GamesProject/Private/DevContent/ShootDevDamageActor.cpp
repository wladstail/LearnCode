// Shoot Game


#include "DevContent/ShootDevDamageActor.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"		  //позволяет рисовать графические примитивы
#include "Kismet/GameplayStatics.h"		//получает всех акторов со сцены

// Sets default values
AShootDevDamageActor::AShootDevDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

}

// Called when the game starts or when spawned
void AShootDevDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShootDevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Сфера которая дамажит
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radios, 24, SphereColor);//рисует сферу

	//выбор урона сферы
	//UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radios, nullptr, {}, this, nullptr, DoFullDamage); //DoFullDamage - если false, интерполяция урона от центра к окраине. Последнее урон если есть предметы внутри, то урон меняется
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radios, DamageType, {}, this, nullptr, DoFullDamage); //DoFullDamage - если false, интерполяция урона от центра к окраине. Последнее урон если есть предметы внутри, то урон меняется

}

