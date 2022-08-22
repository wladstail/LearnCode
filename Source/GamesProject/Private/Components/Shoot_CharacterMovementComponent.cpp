
#include "Components/Shoot_CharacterMovementComponent.h"
#include "Player/ShootBaseCharacter.h"


float UShoot_CharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const AShootBaseCharacter* Player = Cast<AShootBaseCharacter>(GetPawnOwner());      //указатель компонент лишь указаывает на класс Pawn. GetPawnOwner - указатель на свой класс
    return Player && Player->isRuning() ? MaxSpeed * RunModifier : MaxSpeed;        //&& - если первое ложь, другое не вычисляется
}
