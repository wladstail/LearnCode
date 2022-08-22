#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootCoreTypes.h"
#include "ShootHealtsComponent.generated.h"


////событие срабатывающее при смерти
//DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);	//delegate начинаются с F. ; после делегатов объявлений и вконце Signature
////вызывается при изменении хп
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);	//выызвается тогда когда меняются жизни

class UCameraShakeBase; //для камеры шейк

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMESPROJECT_API UShootHealtsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShootHealtsComponent();

	//делегаты, события при смерти
	FOnDeathSignature OnDeath; //делегат который будем вызывать когда жизни 0
	//событие срабатывающе при изменении хп,
	FOnHealthChangedSignature OnHeatlsChanged;


	//смерть персонажа
	UFUNCTION(BlueprintCallable, Category = "Healts")	//функция смерти
		bool IsDead() const { return FMath::IsNearlyZero(Healts); };	//проверка хп на нуль
																		//bool IsDead() const { return Healts <= 0.0f; };	// условие когда хп меньше нуля вызывается

	//возвращает переменную хп персонажа
		float GetHealts() const { return(Healts); };	//функция возвращает хп	

	//функция которая возвращает процент хп
		UFUNCTION(BlueprintCallable, Category = "Healts")
			float GetHealthPercent() const { return Healts / MaxHealts; };

		//возвращает истину, если успешно дабавлено хп Pickup
		UFUNCTION(BlueprintCallable, Category = "Healts")
			bool TryToAddHealth(float HealthAmount);

		//полное ли хп
		bool IsHealthFull() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Healts", meta = (ClampMin = "0.0", ClampMax = "500.0"))
		float MaxHealts = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
		bool AutoHill = true;

	//выключает параметры если meta = (EditCondition = "AutoHill" - false
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",  meta = (EditCondition = "AutoHill")) //meta = (EditCondition = "AutoHill" если вкл автохил то и это вкл
		float HealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHill")) //meta = (EditCondition = "AutoHill" если вкл автохил то и это вкл
		float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHill")) //meta = (EditCondition = "AutoHill" если вкл автохил то и это вкл
		float HealModifier = 5.0f;

	//тряска камеры
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		TSubclassOf<UCameraShakeBase> CameraShake; //UCameraShakeBase - Объект где находится камера шейк

	virtual void BeginPlay() override;

private:

	//переменная хп персонажа
	float Healts = 0.0f;

	//переменная таймера авто хила персонажа
	FTimerHandle HealTimerHandle;	//переменная записи таймера восполнения хп

	//вызывается при любом типе события
	UFUNCTION()
		void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);//кому был урон, урон, тип урона, котроллер того кто нанес урон, актор что нанес

	//вызывается всегда когда срабатывает таймер
	void HealUpdate();	//востановление хп

	void SetHealts(float NewHealts); //функция с закрытым доступом, поэтому только хилкомпонент может изменять жизни

	//проигрывание эффекта тряски камеры
	void PlayCameraShake();
};
