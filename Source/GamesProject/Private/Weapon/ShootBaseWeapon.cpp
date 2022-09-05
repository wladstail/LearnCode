
#include "Weapon/ShootBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"	//���������� ��������� ����
#include "Engine/World.h"
#include "DrawDebugHelpers.h"	//��������� debug ����������
#include "GameFramework/Character.h"	//� ��� �� ���� ����������
#include "GameFramework/Controller.h" //�������� ������ � ������ ����� ����� �����������
#include "NiagaraFunctionLibrary.h"	//������� ��� �������
#include "NiagaraComponent.h"	//������� ��� �������



DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

AShootBaseWeapon::AShootBaseWeapon()
{
	//��������� ���� ����
	PrimaryActorTick.bCanEverTick = false;	
	
	//������� ��������� ������
	WeaponMesh = CreateDefaultSubobject <USkeletalMeshComponent>("WeaponMeshComponent");
	SetRootComponent(WeaponMesh);
}


void AShootBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));
	check(WeaponMesh)	//��������, ��� �� �� ������ � ��� if, ����� ��� ������������ ��� ��������� ������

	//����������� �������� ��������, ������� �� ���������
	CurrentAmmo = DefaultAmmo;
}

//������ �������� ��������� � �������� ������
void AShootBaseWeapon::StartFire()
{
	//MakeShot();	//������� //��������� � �������� 
	//GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AShootBaseWeapon::MakeShot, TimeBetweenShots, true);	//������ ��������. 1 ������, 2 ���, 3 �������, 4 ����� ������, 5 ������������� //��������� � �������� 
}

//����� �������� ��������� � �������� ������
void AShootBaseWeapon::StopFire()
{
	// GetWorldTimerManager().ClearTimer(ShotTimerHandle);	//������� ������� ������� ////��������� � �������� 
}

//������ �������� ��������� � �������� ������
void AShootBaseWeapon::MakeShot()		//��������� �������
{
//			if (!GetWorld()) return;	//�������� ��� ��� �� �������
///*
//	const auto Player = Cast<ACharacter>(GetOwner());	//GetOwner - �������� ����������, ������ ����������
//	if (!Player) return;	//�������� ���� �� ��������
//
//	const auto Controller = Player->GetController<APlayerController>();	//�������� ���������� ���������
//	if (!Controller) return;	//�������� �� ������� ����������
//*/
//
//			FVector TraceStart, TraceEnd;
//			if (!GetTraceDate(TraceStart, TraceEnd)) return;
//	/*
//		FVector ViewLocation;	//��������� ���, �������� �� ���� ��������� ����. ��������� ������
//		FRotator ViemRotation;	//����������� ������� ����� �� ����� ����� ��������
//
//		GetPlayerViewPoint(ViewLocation, ViemRotation); //���������� ������� � ���������� � ����������� ������ � �� ���� ������ � PlayerCameraManager, ������� ���������� ����� � �� ���� ����� �������� ��������� �� ������ ���������. 1 ��������� ������ 2 �������� ������
//		/*
//		const auto Controller = GetPlayerController();	//��������� Controller �������� ��� ������� �� �������
//		if (!Controller) return;	//���� ��������� ������� �� ������� �� �������
//
//
//		FVector ViewLocation;	//��������� ���, �������� �� ���� ��������� ����. ��������� ������
//		FRotator ViemRotation;	//����������� ������� ����� �� ����� ����� ��������
//
//		Controller->GetPlayerViewPoint(ViewLocation, ViemRotation); //���������� ������� � ���������� � ����������� ������ � �� ���� ������ � PlayerCameraManager, ������� ���������� ����� � �� ���� ����� �������� ��������� �� ������ ���������. 1 ��������� ������ 2 �������� ������
//		*/		/* ����������� ������� ��������
//
//		//const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);//�������� ������������� ������ ����� �������� ��������� ����������. �������� ����� - GetSocketTransform(��� ������, ������� ���������)
//
//		const FVector TraceStart = ViewLocation;//���������� ���������� ��������
//		//const FVector TraceStart = SocketTransform.GetLocation();//���������� ���������� ��������. ���������� ��� � ������� �� ������ ��� � �� �� ����
//		const FVector ShootDirector = ViemRotation.Vector();//������ ����������� Vector() - �������� ��� �� ��� GetRotation().GetForwardVector(). ����������� �����������
//		//const FVector ShootDirector = SocketTransform.GetRotation().GetForwardVector();//������ ����������� ������� GetForwardVector - ���������� ������ �������� �� ��� �
//		const FVector TraceEnd = TraceStart + ShootDirector * TraceMaxDistance;	//ShootDirector ��������� ����� ������� ���������. �������� ����� ����� �������
//
//		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);	//������ �����
//	*/
//
//			FHitResult HitResult;
//			MakeHit(HitResult, TraceStart, TraceEnd);
//	/*
//		FCollisionQueryParams CollisionParams;
//		CollisionParams.AddIgnoredActor(GetOwner());	//�������� ����� �� ���������
//
//
//		FHitResult HitResult;	//���������� ������ true ��� false
//		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);	//�������� ���������� �� ������ ������� � ������ ������������ ��������. ECollisionChannel - � ������� ���� ���������� �������� �� �����, �������� ����� ������ � ���� ��� ��������� ��� �����, ������ ������������
//	*/
//			if (HitResult.bBlockingHit)	//�������� ���� �� �����������, ������� � FHitResult ���������� bBlockingHit. ���� �� ��� �� ������ true
//	{
//		//DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);	//������ �����, �� ������ ��� �� ��������� ������ ������ � �� ������
//
//				//DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Blue, false, 3.0f, 0, 3.0f);	//������ �����, �� ������ ��� �� ��������� ������ ������ � �� ������
//
//		MakeDamage(HitResult);	//��������� ����� ��� ����� ������� ���������
//		DrawDebugLine(GetWorld(), GetMuzzleWorldlocation(), HitResult.ImpactPoint, FColor::Blue, false, 3.0f, 0, 3.0f);	//������ �����, �� ������ ��� �� ��������� ������ ������ � �� ������
//		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f, 0, 3.0f);	//������ ����� � ����� �������. HitResult.ImpactPoint - ����� �����������
//
//		UE_LOG(LogBaseWeapon, Display, TEXT("Bone %s"), *HitResult.BoneName.ToString());	//���������� ����� ���� � ������� ������
//	}
//			else
//	{		//���� ������ �� ������
//		/*
//		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
//		*/
//		DrawDebugLine(GetWorld(), GetMuzzleWorldlocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
//	}

}

//�������� ��� ����� ���������� ��������� ��
APlayerController* AShootBaseWeapon::GetPlayerController() const //������� ������� ������� ����� ���������� ��������� �� PlayerController
{
	const auto Player = Cast<ACharacter>(GetOwner());	//GetOwner - �������� ����������, ������ ����������
	if (!Player) return nullptr;	//�������� ���� �� �����, ���� ���� ���������� �����������, �.�. ���������� �� ����������, � ���� ���������� ��

	return Player->GetController<APlayerController>();	//���������� ��������� �����������
}

//���������� ������� � ���������� � ������������ ������
bool AShootBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViemRotation) const
{
	const auto Controller = GetPlayerController();	//��������� Controller �������� ��� ������� �� �������
	if (!Controller) return false;	//���� ��������� ������� �� ������� ����� ����

	Controller->GetPlayerViewPoint(ViewLocation, ViemRotation); //���������� ������� � ���������� � ����������� ������ � �� ���� ������ � PlayerCameraManager, ������� ���������� ����� � �� ���� ����� �������� ��������� �� ������ ���������. 1 ��������� ������ 2 �������� ������
	return true;
}

//��������� ������� ������
FVector AShootBaseWeapon::GetMuzzleWorldlocation() const	//���������� ��������� ������
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);	//�������� ������� ������ ��� ������������� GetSocketTransform
}

//��������� �������� ������ � ����� ���������� �����
bool AShootBaseWeapon::GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const //������� ��� ��������� �������� ������ � �����
{
	FVector ViewLocation;	//��������� ���, �������� �� ���� ��������� ����. ��������� ������
	FRotator ViemRotation;	//����������� ������� ����� �� ����� ����� ��������

	if(!GetPlayerViewPoint(ViewLocation, ViemRotation)) return false; //���������� ������� � ���������� � ����������� ������ � �� ���� ������ � PlayerCameraManager, ������� ���������� ����� � �� ���� ����� �������� ��������� �� ������ ���������. 1 ��������� ������ 2 �������� ������
	
	TraceStart = ViewLocation;//���������� ���������� ��������
	//const FVector TraceStart = SocketTransform.GetLocation();//���������� ���������� ��������. ���������� ��� � ������� �� ������ ��� � �� �� ����

	//const auto HalfRad = FMath::DegreesToRadians(BulletSpread);	//��������� � �������
	//FMath::VRandCone ����� � ���� ����� ������ ����������� �������, � ��� ����� �������������� ��� � ������������ � �������� ������ �������� ����
	const FVector ShootDirector = ViemRotation.Vector();//������ ����������� Vector() - �������� ��� �� ��� GetRotation().GetForwardVector(). ����������� �����������

	//const FVector ShootDirector = SocketTransform.GetRotation().GetForwardVector();//������ ����������� ������� GetForwardVector - ���������� ������ �������� �� ��� �
	TraceEnd = TraceStart + ShootDirector * TraceMaxDistance;	//ShootDirector ��������� ����� ������� ���������. �������� ����� ����� �������
	return true;
}

//����������� � ���������� ������
void AShootBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, FVector& TraceEnd)	//������ trace
{
	if (!GetWorld()) return;	//�������� ������ ��� � ����� ������� �� ����������� ������, �.�. ��������� � ������ protected, � Private �������� �� ���� ���� �� ������
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());	//�������� ����� �� ���������

	//�� ��������� ���������� �������� �� ���������� � HitResult, ������� ��������� ��� ����. �� ���������� ��-�� �����������
	CollisionParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);	//�������� ���������� �� ������ ������� � ������ ������������ ��������. ECollisionChannel - � ������� ���� ���������� �������� �� �����, �������� ����� ������ � ���� ��� ��������� ��� �����, ������ ������������
}

/*
//��������� �����
void AShootBaseWeapon::MakeDamage(const FHitResult& HitResult) 
{
	//���������� ��������� �� ������ ��� �������� ����������� � ��������
	const auto DamageActor = HitResult.GetActor();		//���������� ��������� �� ������ ��� �������� ����������� � ��������
	if (!DamageActor) return;	//���� ��� ������

	DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);	//��������� �����

}
*/

//���������� ������ ��� ����� ��������, ��������� ���������� ��������
void AShootBaseWeapon::DecreaseAmmo()
{
	//�������� ��� ���� ����
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty"));
		return;
	}

	//��������� ���������� ��������
	CurrentAmmo.Bullets--;
	
	//��� ��������
	//LogAmmo();

	//�������� ��� ����� �����������
	if (IsClipEmpty() && !IsAmmoEmpty())	//������ ������ ���� ����� � ��� ������� �� ������
	{
		//����� ������
		/*ChangeClip();*/
		//���� ���� �������� ����� �����������
		StopFire();
		OnClipEmpty.Broadcast(this/*���� ����. ��������� �� ������� ������*/);	//���������� ���� ����� �����������
	}
}

//���������� ������ ����� ������� ��������� ������(�������� � ����� ����)
bool AShootBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();	//������� ������ ���� ������� � ���������� ����� = 0 � ������� ������ ������
}

//���������� ������ ����� ������� ������ ������
bool AShootBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

//������ ������ ������� ������ �� �����
void AShootBaseWeapon::ChangeClip()
{
	//������������� ���������� ������� �������� �� �� ���������
	//CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	
	//�������� ��� �� ����������� ��������
	if (!CurrentAmmo.Infinite)
	{
		//�������� ��� ������ �����
		if (CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
			return;
		}

		//��������� ���������� �����
		CurrentAmmo.Clips--;
	}

	//������������� ���������� ������� �������� �� �� ���������
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	UE_LOG(LogBaseWeapon, Display, TEXT("------ Change Clip ------"));
}

void AShootBaseWeapon::LogAmmo()
{
	//������ � �������
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

bool AShootBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets&& CurrentAmmo.Clips > 0;
}

//������� ������� �����
bool AShootBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips &&
		CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

//������ ���������� ��������� � ��������
bool AShootBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	//������� ��� ������� �� ����� ��������
	if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0.0f) return false;	//����������� ����� ��� ������ ������� ��� ��������� ������ ����

	//������� ��� ������� �� ����� ��������, ���� ������ � ��������� 0
	if (IsAmmoEmpty())
	{
		//��������� ��������� �������� ���������
		CurrentAmmo.Clips = FMath::Clamp(CurrentAmmo.Clips + ClipsAmount, 0, DefaultAmmo.Clips + 1); //�.�. ��������� �������� ����� ���� ������, ��� ����� ���� � ��� �������, ������������� �������. ������������ ������� ClipsAmount, ��������� �� ��������� DefaultAmmo.Clips. 0 - ������ �������. �������� + 1, ������ ��� �������� �����������(��� ������� ������ � �� ��� ����)
		
		//�������� ������� �����������
		OnClipEmpty.Broadcast(this/*���� ����, ��������� �� ������� ������*/);
	}
	//���� ������� �������� ����� ������ ���� �� ���������
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
	//������� ��������� ���������� �.�.����� ��������� ������ �� ��������� � ������� � �������
		const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;//NextClipsAmount - ������� �������� ��������� � �������� ��������� ���������� � �������
		
	//�������� ���� �� ��������� � NextClipsAmount ������ ����, �� ����� ��������� ��������, �������� ����������  NextClipsAmount
		if (DefaultAmmo.Clips - NextClipsAmount >= 0)	//���������� ��������� �� ��������� �� ��������
		{
			CurrentAmmo.Clips = NextClipsAmount;
		}
		//�������� ���� �� ��������� � NextClipsAmount ������ ����, �� ���� 1 ��� ������ � �������, ����� ��������� ��������
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	//���� ������� ������ ����� �� ���������. �������� ���� �.�. ������ ��������� ���
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets; //����� �����, �� ������� ���, 
	}

	//����� Puckup
	return true;
}

//���������� ��������� �� ������� ���������
UNiagaraComponent* AShootBaseWeapon::SpawnMuzzleFX()
{
	//SpawnSystemAttached - niagara ����� ������������������ ������������ �� �������� � �������� ��������������. 
	return UNiagaraFunctionLibrary::SpawnSystemAttached
	(
		MuzzleFX,// 1. ��������� �� ������� �������
		WeaponMesh, //2 ��������� � �������� ���������.
		MuzzleSocketName,// 3.������� 
		FVector::ZeroVector,	//4,5 ��������� �������������
		FRotator::ZeroRotator,	//
		EAttachLocation::SnapToTarget,//������������� � ������
		true	//���������������
	);
}