// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootProjectile.generated.h"

class UShootWeaponFXComponent;	//эффект ниагры, сделали тут а не в базовом т.к. за столкновение отвечает снаряд
class USphereComponent;
class UProjectileMovementComponent; //компонент вдоль которого актор изменяется на тик. Можно сделаь самонаводящую ракету тут или количество отскоков при столкновении с другими предметами

UCLASS()
class GAMESPROJECT_API AShootProjectile : public AActor
{
    GENERATED_BODY()

public:
    AShootProjectile();
    //вектор вдоль которого полетит снаряд
    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }  //создали локальную переменную ShotDirection и в неё записываем параметр функции 

protected:
    //коллизия снаряда
    /*UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")*/
        UPROPERTY(VisibleAnywhere, Category = "Weapon")
        USphereComponent* CollisionComponent;

    //изменяет положения актора на тик от настроенных в нем параметров
    /*UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")*/
        UPROPERTY(VisibleAnywhere, Category = "Weapon")
        UProjectileMovementComponent* MovementComponent;

    //радиус поражения снаряда
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
            float DamageRadius = 200.0f;

        //урон снаряда
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
            float DamageAmount = 50.0f;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
            bool DoFullDamage = false;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
            float LifeSeconds = 5.0f;

        //Визуальный эффект
        UPROPERTY(VisibleAnywhere, Category = "VFX")
            UShootWeaponFXComponent* WeaponFXComponent;

    virtual void BeginPlay() override;

private:
    //локальная переменная направления изменяется по тику на какую-то дельту, но сделаем через MovementComponent из unreal engine
    FVector ShotDirection;

    //F12 сигнатура там. Столкновение коллизии 
    UFUNCTION()
        void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
            const FHitResult& Hit);

    //контроллер создали для нанесения урона
    AController* GetController() const;
};