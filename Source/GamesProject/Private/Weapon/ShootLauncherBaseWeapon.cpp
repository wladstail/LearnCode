// Shoot Game


#include "Weapon/ShootLauncherBaseWeapon.h"
#include "Weapon/ShootProjectile.h"         
//#include "Kismet/GameplayStatics.h"


void AShootLauncherBaseWeapon::StartFire()
{
    MakeShot();
}

//������ ������ �������
void AShootLauncherBaseWeapon::MakeShot()
{
    /*
    //��������� ������������� �������
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldlocation());   //1 ���� 2 ��������� ���������
    auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);  //��������� �� ����� ������� � ������ ���������� ����� � ���� ������ ������ � �������������
    */
    // set projectile params

    //�������� �� �������������
    /*if (!GetWorld()) return;*/
    if (!GetWorld() || IsAmmoEmpty()) return;   //���� ��� �� ������� ��� ������� �� ������

    //��������� ������ � ����� ���������� �����
    FVector TraceStart, TraceEnd;
    if (!GetTraceDate(TraceStart, TraceEnd)) return;

    //����������� � �������� ����� �����
    FHitResult HitResult;//FHitResult - ���� �������� ��� ���� � ��� ������ �� ��� ���
    MakeHit(HitResult, TraceStart, TraceEnd);
  /*  UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);  //����������� ����� 1 ��������� 2 �������������
    */

    //������ ����� �������� ������� ������
    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd; //���� ������, �� EndPoint = ����� ���������, ����� ����� ������ ��������
    const FVector Direction = (EndPoint - GetMuzzleWorldlocation()).GetSafeNormal();    //��������� �����, ��� ����� ������ ��� �������. GetSafeNormal() - ������������� ������, � ���������, ��� �� �� ��������� �������

    //����� �������
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldlocation());
    AShootProjectile* Projectile = GetWorld()->SpawnActorDeferred<AShootProjectile>(ProjectileClass, SpawnTransform);//����� �������
    //�������� ��� ��������� �� ������� �� ������
    if (Projectile)
    {
        //������ ����� �������� ������� ������
        Projectile->SetShotDirection(Direction);

        //���������� ������� ������, �.�. ������ ������� ��� ��������
        Projectile->SetOwner(GetOwner());

        //�������� ��������� ������������� �������
        Projectile->FinishSpawning(SpawnTransform);
    }

    //���������� ��������
    DecreaseAmmo();

    //����� ������� ����
    SpawnMuzzleFX();
}
