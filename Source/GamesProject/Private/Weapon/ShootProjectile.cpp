// Shoot Game

#include "Weapon/ShootProjectile.h"
#include "Components/SphereComponent.h"                 //��������� �����
#include "GameFramework/ProjectileMovementComponent.h"  //��������� ��������� �� ����
#include "DrawDebugHelpers.h"                              //��� ��������� ����� �����
#include "Kismet/GameplayStatics.h"                     //��� ��������� ������
#include "Weapon/Components/ShootWeaponFXComponent.h"	//��� ������� 


AShootProjectile::AShootProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
        
    //������� ����� � ������� � �������
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);                                         //������ ��������
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);              //����������� ������������ �� �����, ������ ���������� � ������������ ��������
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);   //������������ �������� �� ������ ������� ������� . ������������� �� ��� ������ �����

    //HitResult ������������  ��� ������������ ��������, ������� �������� ����������� ����������� ��������� ��� HitResult
    CollisionComponent->bReturnMaterialOnMove = true;

    SetRootComponent(CollisionComponent);

    //��������� ���������� ������� ����� �� ����, ��� �� ��� ���������� ����� �� ����
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;      //���� ���� ���������, ��� �������� ����� ���������� ����� ���������, � �� ��������
    MovementComponent->ProjectileGravityScale = 0.0f;   //���������� �������������

    //��������� ������� ������� ��� �����������
    WeaponFXComponent = CreateDefaultSubobject<UShootWeaponFXComponent>("WeaponFXComponent");
}

void AShootProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    check(MovementComponent);   
    check(CollisionComponent);
    //�������� ��� ���������� ������� ������
    check(WeaponFXComponent);

    //���������� ������������ �������������� ��������(� ���� � gravity Scale ����� ���������� ���� �������)
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;  //Velocity - �������� �� �������� � ����������� ��� = ��������� ������ ����������� ��������(ShotDirection) * �������� ��������(InitialSpeed) �� ������� �������� �.�. ��� ���������� ��� ������ ����
   /* SetLifeSpan(5.0f);
   */

    //������������� ������� �� ����� �������� �������
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);    //1 ��������� �� ������(�������� ��������) 2 ������������ ��� ��� ���

    //�������� �������, ��� �� �� ���� ������ ������ � ������������ �������
    CollisionComponent->OnComponentHit.AddDynamic(this, &AShootProjectile::OnProjectileHit);//OnComponentHit - ���������� ��� ������������, ������������ �������. 1 - ��������� �� ������� ������, 2 ������ �� �������.
    SetLifeSpan(LifeSeconds);
}

//������ ������������ � ��������� �
void AShootProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)    //F12 ��������� ���
{
    //�������� �� ����
    if (!GetWorld()) return;

    //������������� �������� �������
    MovementComponent->StopMovementImmediately();

    // ������� ���� ����������
    UGameplayStatics::ApplyRadialDamage(GetWorld(),  //���
        DamageAmount,                                //���������� �����
        GetActorLocation(),                          //���� �����
        DamageRadius,                                //������ �����
        UDamageType::StaticClass(),                  //��� �����
        { GetOwner() },                              //������ ������ ������� �������� �����
        this,                                        //��������� �� ������ ��� ����� ����
        GetController(),                             //��������� �� ���������� ���������, ����� ����� ��� �������� ��� ���� ���������� ����� nullptr
        DoFullDamage);                               //��������� �� ���� �� ��������� ���������� ��� �����


    //����������� ����� ��� ������������� �����
    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);

    //������ ������������ ������
    WeaponFXComponent->PlayImpactFX(Hit);   //�������� ���, ������� ������ ��� � CallBack

    Destroy();  //����������� ������
}

//�������� �������� �� �� pawn ��� ���, ���� ��, �� ���������� ���������� ���������. ����� ����� ��� �������� ��� ���� ����������
AController* AShootProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}