// Shoot Game


#include "Weapon/ShootLauncherBaseWeapon.h"
#include "Weapon/ShootProjectile.h"         
//#include "Kismet/GameplayStatics.h"


void AShootLauncherBaseWeapon::StartFire()
{
    MakeShot();
}

//логика спауна снаряда
void AShootLauncherBaseWeapon::MakeShot()
{
    /*
    //начальная трансформация снаряда
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldlocation());   //1 угол 2 начальная положение
    auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);  //указатель на актор снаряда и делаем отложенный спавн в мире нашего класса и трансформацию
    */
    // set projectile params

    //проверка на существование
    /*if (!GetWorld()) return;*/
    if (!GetWorld() || IsAmmoEmpty()) return;   //если мир не нулевой или арсенал не пустой

    //получение начала и конца логической линии
    FVector TraceStart, TraceEnd;
    if (!GetTraceDate(TraceStart, TraceEnd)) return;

    //пересечение с объектом нашей линии
    FHitResult HitResult;//FHitResult - сюда попадает вся инфа о том попали мы или нет
    MakeHit(HitResult, TraceStart, TraceEnd);
  /*  UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);  //заканчиваем спавн 1 указатель 2 трансформация
    */

    //вектор вдоль которого полетит снаряд
    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd; //если попали, то EndPoint = точке попадания, иначе длине трейса конечной
    const FVector Direction = (EndPoint - GetMuzzleWorldlocation()).GetSafeNormal();    //начальная точка, это сокет откуда идёт выстрел. GetSafeNormal() - нормализовали вектор, в единичный, что бы не нагружать систему

    //спавн снаряда
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldlocation());
    AShootProjectile* Projectile = GetWorld()->SpawnActorDeferred<AShootProjectile>(ProjectileClass, SpawnTransform);//спавн снаряда
    //проверка что указатель не нулевой на снаряд
    if (Projectile)
    {
        //вектор вдоль которого полетит снаряд
        Projectile->SetShotDirection(Direction);

        //определяет хозяина актора, т.е. хозяин снаряда это персонаж
        Projectile->SetOwner(GetOwner());

        //передаем параметры трансформации снаряда
        Projectile->FinishSpawning(SpawnTransform);
    }

    //уменьшение патронов
    DecreaseAmmo();

    //вызов вспышки дула
    SpawnMuzzleFX();
}
