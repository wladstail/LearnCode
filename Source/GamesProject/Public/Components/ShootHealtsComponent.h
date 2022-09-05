#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootCoreTypes.h"
#include "ShootHealtsComponent.generated.h"


////������� ������������� ��� ������
//DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);	//delegate ���������� � F. ; ����� ��������� ���������� � ������ Signature
////���������� ��� ��������� ��
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);	//���������� ����� ����� �������� �����

class UCameraShakeBase; //��� ������ ����

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMESPROJECT_API UShootHealtsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShootHealtsComponent();

	//��������, ������� ��� ������
	FOnDeathSignature OnDeath; //������� ������� ����� �������� ����� ����� 0
	//������� ������������ ��� ��������� ��,
	FOnHealthChangedSignature OnHeatlsChanged;


	//������ ���������
	UFUNCTION(BlueprintCallable, Category = "Healts")	//������� ������
		bool IsDead() const { return FMath::IsNearlyZero(Healts); };	//�������� �� �� ����
																		//bool IsDead() const { return Healts <= 0.0f; };	// ������� ����� �� ������ ���� ����������

	//���������� ���������� �� ���������
		float GetHealts() const { return(Healts); };	//������� ���������� ��	

	//������� ������� ���������� ������� ��
		UFUNCTION(BlueprintCallable, Category = "Healts")
			float GetHealthPercent() const { return Healts / MaxHealts; };

		//���������� ������, ���� ������� ��������� �� Pickup
		UFUNCTION(BlueprintCallable, Category = "Healts")
			bool TryToAddHealth(float HealthAmount);

		//������ �� ��
		bool IsHealthFull() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Healts", meta = (ClampMin = "0.0", ClampMax = "500.0"))
		float MaxHealts = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
		bool AutoHill = true;

	//��������� ��������� ���� meta = (EditCondition = "AutoHill" - false
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",  meta = (EditCondition = "AutoHill")) //meta = (EditCondition = "AutoHill" ���� ��� ������� �� � ��� ���
		float HealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHill")) //meta = (EditCondition = "AutoHill" ���� ��� ������� �� � ��� ���
		float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHill")) //meta = (EditCondition = "AutoHill" ���� ��� ������� �� � ��� ���
		float HealModifier = 5.0f;

	//������ ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		TSubclassOf<UCameraShakeBase> CameraShake; //UCameraShakeBase - ������ ��� ��������� ������ ����

	virtual void BeginPlay() override;

private:

	//���������� �� ���������
	float Healts = 0.0f;

	//���������� ������� ���� ���� ���������
	FTimerHandle HealTimerHandle;	//���������� ������ ������� ����������� ��

	//���������� ��� ����� ���� �������
	UFUNCTION()
		void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);//���� ��� ����, ����, ��� �����, ��������� ���� ��� ����� ����, ����� ��� �����

	//���������� ������ ����� ����������� ������
	void HealUpdate();	//������������� ��

	void SetHealts(float NewHealts); //������� � �������� ��������, ������� ������ ������������ ����� �������� �����

	//������������ ������� ������ ������
	void PlayCameraShake();
};
