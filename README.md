[![PMP](http://img.youtube.com/vi/-JZhUTnfqzU/hqdefault.jpg)](https://youtu.be/-JZhUTnfqzU?t=0s) 

+ https://youtu.be/-JZhUTnfqzU

# ProjectMP


+ 간단한 RPG요소와 멀티플레이를 구현한 프로젝트입니다.

+ 애니메이션, UI, BT 등을 제외하고 대부분 C++로 구현하였습니다.

# 개발환경


+ Unreal Engine 5.3.2 

+ JetBrain Rider 2023.3


# 주요 구현 설명

+ Online Subsystem Steam 연결

  플러그인을 이용하여 스팀 로그인 상태에서 게임을 실행 후 Host 선택시 세션 호스팅 Join 선택시 세션에 입장

+ 캐릭터 기본 동작 구현

  1. 서버함수 호출, Authority가 없을경우 로컬 함수 호출

	2. 서버함수에서 멀티캐스트 호출

	3. 멀티캐스트에서 LocallyControlled이고 Authority가 없을경우 return, 아니면 로컬 함수 호출

	4. 서버 및 각 클라이언트에서 로컬 함수 호출(애니메이션 재생)

	5. 애니메이션 노티파이와 충돌 및 투사체 생성 함수 Delegate, 필요 시 변수값 Replicate

  [보스 랜덤위치 장판 스킬 동기화 코드]
  https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPBoss.cpp#L92
  
  https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPBoss.cpp#L331-L397
  
   6. 데미지는 서버에서만 적용 및 클라이언트로 동기화
  
  https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPCharacter.cpp#L236-L244

  https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPCharacter.cpp#L316-L343
 
+ EnumClassFlag와 비트마스킹을 이용한 상태 관리

	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPCharacter.h#L19-L30
	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPCharacter.cpp#L313
	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPCharacter.cpp#L327
	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPCharacter.cpp#L347

+ 버프 아이템 적용

	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPItem.cpp#L73-L75
	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPCharacter.cpp#L345-L359

+ 상태 이상 구현

	빙결, 슬로우 - 애니메이션 속도 조절, 현재 행동 가능여부, 머터리얼의 Emissive 파라미터를 변경하여 특정 색으로 빛나는 효과

	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPMonster.cpp#L112-L151
  
+  보스 AI - 랜덤 정수를 뽑고 확률에 따라 BehaviorTree에서 패턴 수행
	
	[랜덤 수를 뽑는 BTService]

	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/BTService_RandomNumber.cpp#L18-L45

+ 몬스터 락온 기능 - 마우스 휠버튼 클릭으로 사용 상태 토글, 원형 범위 탐색하여 적 존재시 시점 고정

	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPCharacter.cpp#L310-L314
	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPCharacter.cpp#L87-L135

+ 가독성 - 람다함수를 이용해 단순반복코드 간결화

	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPAnimInstance.cpp#L9-L44
	https://github.com/DeYeVe/PMP/blob/2162c845f0285d64028f898baa0266c54d78e98e/Source/PMP/PMPCharacterMuriel.cpp#L40-L51
	

# 문제 발생 및 해결

 + 플레이어 접속
	
	 처음에는 레벨에 미리 배치된 캐릭터들에 PlayerID 0, 1을 순서대로 접속하도록 구현하였는데 제대로 작동이 안돼서 디버깅해보니 PlayerID가 256부터 시작하는것을 발견.
	또한 에디터에서 플레이시 PlayerID가 256부터 누적되어 올라가서 테스트가 어려움이 생김

	-> 로그인 시 PlayerID에서 시작 번호인 256을 빼고 플레이어 수만큼 모듈러연산하여 0부터 차례대로 접속되도록 간접 구현

 + 애니메이션 동기화 이슈

	AnimInstance에서 관리하고 갱신하는 변수 Speed와 Movement Vector를 조건으로 이용하는 BlendSpace가 다른 클라이언트에서 제대로 작동이 안됨
	확인 결과 캐릭터의 속도는 자동으로 Replicate 되는 반면 Movement Vector는 replicate되지 않음
	
	-> 캐릭터 tick에서 서버로 Movement Vector 값을 전송하고 해당 변수를 Replicate 되도록 하여 해결

	
