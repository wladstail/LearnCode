
#include "Components/Shoot_CharacterMovementComponent.h"
#include "Player/ShootBaseCharacter.h"


float UShoot_CharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const AShootBaseCharacter* Player = Cast<AShootBaseCharacter>(GetPawnOwner());      //��������� ��������� ���� ���������� �� ����� Pawn. GetPawnOwner - ��������� �� ���� �����
    return Player && Player->isRuning() ? MaxSpeed * RunModifier : MaxSpeed;        //&& - ���� ������ ����, ������ �� �����������
}
