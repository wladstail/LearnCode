// Shoot Game


#include "DevContent/ShootDevDamageActor.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"		  //��������� �������� ����������� ���������
#include "Kismet/GameplayStatics.h"		//�������� ���� ������� �� �����

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

	//����� ������� �������
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radios, 24, SphereColor);//������ �����

	//����� ����� �����
	//UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radios, nullptr, {}, this, nullptr, DoFullDamage); //DoFullDamage - ���� false, ������������ ����� �� ������ � �������. ��������� ���� ���� ���� �������� ������, �� ���� ��������
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radios, DamageType, {}, this, nullptr, DoFullDamage); //DoFullDamage - ���� false, ������������ ����� �� ������ � �������. ��������� ���� ���� ���� �������� ������, �� ���� ��������

}

