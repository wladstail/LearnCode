
#include "Components/ShootHealtsComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"	//��� ����������
#include "GameFramework/Controller.h" //��� ����������
#include "Engine/World.h"
#include "TimerManager.h"	//������ ���������� �� ����������� ��
#include "Camera/CameraShakeBase.h"	//��� ����������


/*#include "DevContent/ShootFireDamageType.h"	//���������� �����
#include "DevContent/ShootIceDamageType.h"	//���
*/

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All)

UShootHealtsComponent::UShootHealtsComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UShootHealtsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//�������� ��� � �� ���� �� ���� �������
	check(MaxHealts > 0);

	//���������� �� � ������ ����
	SetHealts(MaxHealts);
			//Healts = MaxHealts;		//��� ������ ���� ��
			//OnHeatlsChanged.Broadcast(Healts);	//�������� ��� ��������� �����

	//�������� �������, ������� ��������� ����� � ��������� ����������
	AActor* ComponentOwner = GetOwner(); //GetOwner - ���������� ��������� �� ���������
	if (ComponentOwner)	//���� �� ������� �� ������������� �� ���������
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UShootHealtsComponent::OnTakeAnyDamageHandle);
	}
}

//���������� ��� ����� ���� �������
void UShootHealtsComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;//��� ������� ���� ������������� ���� �� ��� � �� ������ �������. ������ ���� �� ��� �� �������
	
	//�������� �� �� ����
	SetHealts(Healts - Damage);

	//Healts = FMath::Clamp(Healts - Damage, 0.0f, MaxHealts);//clamp 1 �������� ��� �������� ������� �� min �� max, ���� �� ������ � ��������, �� ������������ � ��������� �������
	//OnHeatlsChanged.Broadcast(Healts);	//������� ����� �������� �����
	
	//����������� �������� ��� ������
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);	//���������� ������ ��� ��������� � ���

	//������� ������ ��������� � ������� ���������
	if (IsDead())
	{
		OnDeath.Broadcast(); //���� ��������� ���� ����������� �� �������, ��� �������� �����
	}
	else if (AutoHill && GetWorld()) //���� ����, �� ��������� ��������� �� ��� ����, ������ ��� ������ � UWorld
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UShootHealtsComponent::HealUpdate, HealUpdateTime, true, HealDelay);//� ������� ������������� ��(������ - ����)
	}

	//������ ������ ��� ��������� ������ �����
	PlayCameraShake();
	//Healts -= Damage;


	// UE_LOG(BaseCharacterLog, Display, TEXT("Damage: %f"), Damage); 


/*
	if (DamageType)	//�������� ��� ���������� ����
	{
		//����� ���� �� ������� ������ ���� �� ��� �����, �.�. ��������� ��������� ������ � ������������� ���� ������ � ���� �������, �������� � ���
		if (DamageType->IsA<UShootFireDamageType>())	//IsA - ���������� ������ ���� ������ ��������� � ����� ����
		{
			UE_LOG(BaseCharacterLog, Display, TEXT("Fire Damage "));
		}
		else if (DamageType->IsA<UShootIceDamageType>())	//�������� �� ������� ����
		{
			UE_LOG(BaseCharacterLog, Display, TEXT("Ice Damage "));
		}
	}
	*/
}

//���� ��� ���������
void UShootHealtsComponent::HealUpdate()
{
	SetHealts(Healts + HealModifier);

	//Healts += FMath::Min(Healts + HealModifier, MaxHealts);// ���������� �� �� �������� � �� �������� ���� ��������
	//	OnHeatlsChanged.Broadcast(Healts); //�������� ��� ��������� �����
	
	//������� ����������� if (FMath::IsNearlyEqual(Healts, MaxHealts) && GetWorld()) //IsNearlyEqual ��������� �� ��������� 2 ����� ���� �����, � ��������� ������� ��� ����� �������� �������
	//if (Healts == MaxHealts && GetWorld())	//���� �� ��������� �������������� �������� �������������� ���� ������

	if (IsHealthFull() && GetWorld()) //IsNearlyEqual ��������� �� ��������� 2 ����� ���� �����, � ��������� ������� ��� ����� �������� �������
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);//�������� ������
	}
}

//�������������� �� (������� ������� ��-�� ������� ���������� � ����)
void UShootHealtsComponent::SetHealts(float NewHealts)
{
	//������� ��������� ���������� ��� ���� ���� �������� ������� ��������
	const auto NextHealth = FMath::Clamp(NewHealts, 0.0f, MaxHealts);	//����������� �� ������������ ���������
	const auto HealthDelta = NextHealth - Healts;//������� ����� ��������� ��������� �������� � �������

	//����������� ��-�� ���� ���� �������� ������ � �������
	//Healts = FMath::Clamp(NewHealts, 0.0f, MaxHealts);	//����������� �� ������������ ���������
	Healts = NextHealth;
	//OnHeatlsChanged.Broadcast(Healts); //�������� ��� ��������� �����
	OnHeatlsChanged.Broadcast(Healts, HealthDelta); //��������, ������ ��������, ��� �� ������� ��� ��������� �������� ������ ��� ��������, �������� ��� ��������� �����. HealthDelta - �������� ��������� ��������
}

//���������� ������, ���� ������� ��������� �� Pickup
bool UShootHealtsComponent::TryToAddHealth(float HealthAmount)
{
	//�������� ����� ����� false
	if (IsDead() || IsHealthFull()) return false;	//���� �������� �����, ���� ������ ����� ��������

	//�������������� �� �������� + ��� � Pickup
	SetHealts(Healts + HealthAmount);

	return true;	//������� ��������� ��
}

//������ �� ��
bool UShootHealtsComponent::IsHealthFull() const 
{
	//�������� �� ���������
	return FMath::IsNearlyEqual(Healts, MaxHealts);
}

//������������ ������� ������ ������
void UShootHealtsComponent::PlayCameraShake()
{
	//�������� ��� �� ��������
	if (IsDead()) return;

	//�������� ��������� �� Pawn
	const auto Player = Cast<APawn>(GetOwner());	// ��������� �� ���������� � APawn, ������� ��� ������ ������� � ACharacter

	//�������� ��������� �� ����
	if (!Player) return;

	//�������� ��������� �� ���������
	const auto Controller = Player->GetController<APlayerController>();

	//�������� ��������� �� ���� ����������� � ������ ���������
	if (!Controller ||!Controller->PlayerCameraManager) return;

	//�������� ����������
	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}