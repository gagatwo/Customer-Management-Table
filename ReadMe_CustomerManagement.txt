Hash table을 이용한 고객 관리 API 구현

- 프로젝트의 주요 특징
고객의 이름과 아이디, 보유 자산을 등록하고 관리하는 알고리즘 구현
이름을 key로 하는 해시테이블과 아이디를 key로 하는 해시테이블을 만들어 각 해시테이블이 하나의 고객 정보에 개별로 연결되도록 설정
두개의 해시테이블을 이용한다는 특징을 이용해 이름 또는 아이디 중 하나의 정보만으로 고객의 자산 정보를 데이터 베이스에서 바로 찾을 수 있는 함수 개발
고객 정보를 등록 또는 제거할 때, 정확한 포인터 사용으로 연결된 고객정보들이 끊기지 않도록 함
고객 정보를 등록하고 제거할 때, 동적 메모리 할당을 이용하여 메모리 누수가 발생하지 않도록 함


- customer_manager.c
Hash table을 이용해 고객 관리 API를 구현한 코드
CreateCustomerDB : Customer DB를 만들 때 가장 처음으로 메모리를 할당 해주는 함수
DestroyCustomerDB : 모든 고객 정보와 DB를 삭제하고 할당된 메모리를 해제하는 함수
RegisterCustomer : 고객 정보를 DB에 등록해주는 함수
UnregisterCustomerByID : 해당 ID를 가진 고객을 DB에서 삭제하는 함수
UnregisterCustomerByName : 해당 이름을 가진 고객을 DB에서 삭제하는 함수
GetAssetByID : 해당 ID를 가진 고객의 자산을 찾는 함수
GetAssetByName :   해당 이름을 가진 고객의 자산을 찾는 함수


- Customer_managemente_DataStructure.pdf
구현한 고객 관리 자료구조를 간단히 시각화한 자료입니다

