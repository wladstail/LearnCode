// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootCoreTypes.h"
#include "ShootBaseWeapon.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

class USkeletalMeshComponent;
class UNiagaraSystem;		//������� ��� ������� �������
class UNiagaraComponent;	//������� ��� ������� �������

//USTRUCT(BlueprintType)
//struct FAmmoData
//{
//	GENERATED_USTRUCT_BODY()
//
//		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
//		int32 Bullets;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
//		int32 Clips;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
//		bool Infinite;
//};


UCLASS()
class GAMESPROJECT_API AShootBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AShootBaseWeapon();

	// ��������� ��� ����� �����������
	FOnClipEmptySignature OnClipEmpty;

	//�������� �� ������
	virtual void  StartFire();	//������� ����������� ������ ��� ����� ���������� BaseWeapon � ������� � ������ �������� ����� �� �������
	virtual void  StopFire();	//������� ����������� ������ ��� ����� ���������� BaseWeapon � ������� � ������ �������� ����� �� �������
	
	//������ ������ ������� ������ �� �����
	void ChangeClip();
	bool CanReload() const;
		
	//����� ��������� ������ ������� � ������
	FWeaponUIData GetUIData() const { return UIData; }

	//������� ��� ��������� ������� � �������� ��������, ��� ��������
	FAmmoData GetAmmoData() const { return CurrentAmmo; }

	//������ ���������� ��������� � ��������
	bool TryToAddAmmo(int32 ClipsAmount);

protected:



	//��� ������
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Compontnts")
	USkeletalMeshComponent* WeaponMesh;	//��������� �� ��� ������


	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)	//�������� ������
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket";

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)*/
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance = 1500.0f;			//���� ��������� ������, �.�. ���������� ��������

	//��������� � ��������, �.�. ��� �� �����
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//float DamageAmount = 10.0f;					//���� ������

	//��������� ��������� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FAmmoData DefaultAmmo { 15, 10, false };

	/* ���������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)		//������� ������������ ��� ���
	float TimeBetweenShots = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)		//���������� ��� ���
	float BulletSpread = 1.5f;							//� ��������
	*/


	//��������� ������ �������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")	//�.�. � ������ protected ������� � ������ ������ 
	FWeaponUIData UIData;

	//��������������� ������ ������� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UNiagaraSystem* MuzzleFX;	//��� ������ ������� �������, ��������� ������������ �� UNiagaraComponent


	virtual void BeginPlay() override;

	//������ �������� �� ������, ������� �������� ��-�� ���� ��� ������� ����� �� �������
	//����������� ������� ��-�� ���� ��� ����� ����� ��������������
	virtual void MakeShot();
	virtual bool GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const;

	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViemRotation) const;

	// bool GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const;	//������� �����������
	FVector GetMuzzleWorldlocation() const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, FVector& TraceEnd);
	/*void MakeDamage(const FHitResult& HitResult); ��������� � ��������, �.�. ��� ��� �� �����*/

	
	//���������
//private:
//	FTimerHandle ShotTimerHandle;

	//���������� ������ ��� ����� ��������, ��������� ���������� ��������
	void DecreaseAmmo();

	//���������� ������ ����� ������� ��������� ������(�������� � ����� ����)
	bool IsAmmoEmpty() const;

	//���������� ������ ����� ������� ������ ������
	bool IsClipEmpty() const;
	
	//������ ������ ������� ������ �� �����
	//void ChangeClip();
	
	//����� ���� � �������
	void LogAmmo();

	//��� ������ ���������� �������. ���� ����� ������� ����� ���������� ���
	bool IsAmmoFull() const;

	//������� ������ ������� �������. ���������� ��������� �� UNiagaraComponent
	UNiagaraComponent* SpawnMuzzleFX();
private:

	//������� ������� ������
	FAmmoData CurrentAmmo;
};