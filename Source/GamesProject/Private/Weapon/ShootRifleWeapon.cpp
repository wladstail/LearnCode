// Shoot Game


#include "Weapon/ShootRifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"	//��������� debug ����������
#include "Weapon/Components/ShootWeaponFXComponent.h"	//��� ������� 


DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)


//����������� ����� ��������
AShootRifleWeapon::AShootRifleWeapon()
{	
	//��������� ������� ������� ��� ��������
	WeaponFXComponent = CreateDefaultSubobject<UShootWeaponFXComponent>("WeaponFXComponent");
}

//�������� BeginPlay, ��� �� ��������� ��� ������ ������� ��� �������� ���������
void AShootRifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	//�������� ��� ���������� ������� ������
	check(WeaponFXComponent);
}

//������ ��������
void AShootRifleWeapon::StartFire()
{

		//MakeShot();	//�������
		//������ ����������� � ���������� ������� ���������
		GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AShootRifleWeapon::MakeShot, TimeBetweenShots, true);	//������ ��������. 1 ������, 2 ���, 3 �������, 4 ������� ����������, 5 �������������
		MakeShot();
}

//����� ��������
void AShootRifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);	//������� ������� �������
}

//��������� ���������� ������
void AShootRifleWeapon::MakeShot()		//��������� �������
{
	//if (!GetWorld()) return;	//�������� ��� ��� �� �������
	if (!GetWorld() || IsAmmoEmpty()) //�������� ��� ��� �� ������� ��� ������� ������
	{
		//���� ���� � �������� ����� ����� ��������
		StopFire();
		return;
	}
/*
	const auto Player = Cast<ACharacter>(GetOwner());	//GetOwner - �������� ����������, ������ ����������
	if (!Player) return;	//�������� ���� �� ��������

	const auto Controller = Player->GetController<APlayerController>();	//�������� ���������� ���������
	if (!Controller) return;	//�������� �� ������� ����������
*/

	FVector TraceStart, TraceEnd;
	/*if (!GetTraceDate(TraceStart, TraceEnd)) return;*/
	if (!GetTraceDate(TraceStart, TraceEnd))
	{
		//���� ���� � �������� ����� ����� ��������
		StopFire();
		return;
	}
	/*
		FVector ViewLocation;	//��������� ���, �������� �� ���� ��������� ����. ��������� ������
		FRotator ViemRotation;	//����������� ������� ����� �� ����� ����� ��������

		GetPlayerViewPoint(ViewLocation, ViemRotation); //���������� ������� � ���������� � ����������� ������ � �� ���� ������ � PlayerCameraManager, ������� ���������� ����� � �� ���� ����� �������� ��������� �� ������ ���������. 1 ��������� ������ 2 �������� ������
		/*
		const auto Controller = GetPlayerController();	//��������� Controller �������� ��� ������� �� �������
		if (!Controller) return;	//���� ��������� ������� �� ������� �� �������


		FVector ViewLocation;	//��������� ���, �������� �� ���� ��������� ����. ��������� ������
		FRotator ViemRotation;	//����������� ������� ����� �� ����� ����� ��������

		Controller->GetPlayerViewPoint(ViewLocation, ViemRotation); //���������� ������� � ���������� � ����������� ������ � �� ���� ������ � PlayerCameraManager, ������� ���������� ����� � �� ���� ����� �������� ��������� �� ������ ���������. 1 ��������� ������ 2 �������� ������
		*/		/* ����������� ������� ��������

		//const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);//�������� ������������� ������ ����� �������� ��������� ����������. �������� ����� - GetSocketTransform(��� ������, ������� ���������)

		const FVector TraceStart = ViewLocation;//���������� ���������� ��������
		//const FVector TraceStart = SocketTransform.GetLocation();//���������� ���������� ��������. ���������� ��� � ������� �� ������ ��� � �� �� ����
		const FVector ShootDirector = ViemRotation.Vector();//������ ����������� Vector() - �������� ��� �� ��� GetRotation().GetForwardVector(). ����������� �����������
		//const FVector ShootDirector = SocketTransform.GetRotation().GetForwardVector();//������ ����������� ������� GetForwardVector - ���������� ������ �������� �� ��� �
		const FVector TraceEnd = TraceStart + ShootDirector * TraceMaxDistance;	//ShootDirector ��������� ����� ������� ���������. �������� ����� ����� �������

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);	//������ �����
	*/

	//����������� � �������� ����� �����
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	/*
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());	//�������� ����� �� ���������


		FHitResult HitResult;	//���������� ������ true ��� false
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);	//�������� ���������� �� ������ ������� � ������ ������������ ��������. ECollisionChannel - � ������� ���� ���������� �������� �� �����, �������� ����� ������ � ���� ��� ��������� ��� �����, ������ ������������
	*/

	//��������, ���� ������ �������� ����, ������ ����� �� ������ ������ � ����� �����������, ����� ����� ��������
	if (HitResult.bBlockingHit)	//�������� ���� �� �����������, ������� � FHitResult ���������� bBlockingHit. ���� �� ��� �� ������ true
	{
		//DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);	//������ �����, �� ������ ��� �� ��������� ������ ������ � �� ������

				//DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Blue, false, 3.0f, 0, 3.0f);	//������ �����, �� ������ ��� �� ��������� ������ ������ � �� ������

		MakeDamage(HitResult);	//��������� ����� ��� ����� ������� ���������
		/*������� �� - �� ������  DrawDebugLine(GetWorld(), GetMuzzleWorldlocation(), HitResult.ImpactPoint, FColor::Blue, false, 3.0f, 0, 3.0f);	//������ �����, �� ������ ��� �� ��������� ������ ������ � �� ������
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f, 0, 3.0f);	//������ ����� � ����� �������. HitResult.ImpactPoint - ����� �����������
		*/

		//��������� ������ ��� ����������� � ��������
		WeaponFXComponent->PlayImpactFX(HitResult);

		//UE_LOG(LogBaseWeapon, Display, TEXT("Bone %s"), *HitResult.BoneName.ToString());	//���������� ����� ���� � ������� ������
	}
	else
	{		//���� ������ �� ������
		/*
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
		*/
		DrawDebugLine(GetWorld(), GetMuzzleWorldlocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
	}

	//��������� ���������� �������� ��� ��������
	DecreaseAmmo();
}

//�������� ������ ���������� �����, ������ � �����
bool AShootRifleWeapon::GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const //������� ��� ��������� �������� ������ � �����
{
	FVector ViewLocation;	//��������� ���, �������� �� ���� ��������� ����. ��������� ������
	FRotator ViemRotation;	//����������� ������� ����� �� ����� ����� ��������

	if (!GetPlayerViewPoint(ViewLocation, ViemRotation)) return false; //���������� ������� � ���������� � ����������� ������ � �� ���� ������ � PlayerCameraManager, ������� ���������� ����� � �� ���� ����� �������� ��������� �� ������ ���������. 1 ��������� ������ 2 �������� ������

	//������ ���������� �����
	TraceStart = ViewLocation;//���������� ���������� ��������
	//const FVector TraceStart = SocketTransform.GetLocation();//���������� ���������� ��������. ���������� ��� � ������� �� ������ ��� � �� �� ����

	//������� ������
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);	//��������� � �������
	//FMath::VRandCone ����� � ���� ����� ������ ����������� �������, � ��� ����� �������������� ��� � ������������ � �������� ������ �������� ����

	//��������� ����������� ��������
	const FVector ShootDirector = FMath::VRandCone(ViemRotation.Vector(), HalfRad);//������ ����������� Vector() - �������� ��� �� ��� GetRotation().GetForwardVector(). ����������� �����������

	//����� ���������� �����
	//const FVector ShootDirector = SocketTransform.GetRotation().GetForwardVector();//������ ����������� ������� GetForwardVector - ���������� ������ �������� �� ��� �
	TraceEnd = TraceStart + ShootDirector * TraceMaxDistance;	//ShootDirector ��������� ������ ������� ���������. �������� ����� ����� �������
	return true;
}

void AShootRifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}
