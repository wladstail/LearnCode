// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootProjectile.generated.h"

class UShootWeaponFXComponent;	//������ ������, ������� ��� � �� � ������� �.�. �� ������������ �������� ������
class USphereComponent;
class UProjectileMovementComponent; //��������� ����� �������� ����� ���������� �� ���. ����� ������ ������������� ������ ��� ��� ���������� �������� ��� ������������ � ������� ����������

UCLASS()
class GAMESPROJECT_API AShootProjectile : public AActor
{
    GENERATED_BODY()

public:
    AShootProjectile();
    //������ ����� �������� ������� ������
    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }  //������� ��������� ���������� ShotDirection � � �� ���������� �������� ������� 

protected:
    //�������� �������
    /*UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")*/
        UPROPERTY(VisibleAnywhere, Category = "Weapon")
        USphereComponent* CollisionComponent;

    //�������� ��������� ������ �� ��� �� ����������� � ��� ����������
    /*UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")*/
        UPROPERTY(VisibleAnywhere, Category = "Weapon")
        UProjectileMovementComponent* MovementComponent;

    //������ ��������� �������
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
            float DamageRadius = 200.0f;

        //���� �������
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
            float DamageAmount = 50.0f;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
            bool DoFullDamage = false;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
            float LifeSeconds = 5.0f;

        //���������� ������
        UPROPERTY(VisibleAnywhere, Category = "VFX")
            UShootWeaponFXComponent* WeaponFXComponent;

    virtual void BeginPlay() override;

private:
    //��������� ���������� ����������� ���������� �� ���� �� �����-�� ������, �� ������� ����� MovementComponent �� unreal engine
    FVector ShotDirection;

    //F12 ��������� ���. ������������ �������� 
    UFUNCTION()
        void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
            const FHitResult& Hit);

    //���������� ������� ��� ��������� �����
    AController* GetController() const;
};