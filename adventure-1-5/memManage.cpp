#include "memManage.h"

#define SORT_CONDITION(mem1,mem2,tag) (((tag) == B_FIT) ? \
							((mem1)->mem_left_size > (mem2)->mem_left_size) : \
							((mem2)->mem_left_size) > (mem1)->mem_left_size)
// 위의 전처리기는 다음 조건을 반환
// best fit일 경우 - (temp_1->mem_left_size > temp_2->mem_left_size)
// worst fit일 경우 - (temp_1->mem_left_size < temp_2->mem_left_size)

void setTag() {
	do {
		cout << "0 : first fit" << endl << "1 : best fit" << endl << "2 : worst fit" << endl << "입력 >> ";
		cin >> tag;
		if (!((tag == 0) || (tag == 1) || (tag == 2))) {
			system("cls");
			cout << "잘못된 입력입니다. 다시 입력해주세요." << endl;
		}
	} while ((tag != 0) && (tag != 1) && (tag != 2));

	system("PAUSE");
	system("cls");
}

void initMem() {
	available = new Chunk;
	available->link = NULL;
	available->mem_left_size = MAX_MEM; // 추후에 사용자가 최대 메모리 크기 설정
	available->mem_start = 0;
}

void releaseMem() { // 모든 메모리 할당 해제
	Chunk *temp = available;
	Chunk *del = temp;
	do {
		temp = temp->link;
		delete del;
		del = temp;
	} while (temp->link != NULL);
}

int myalloc(int req_size) {
	Chunk *current = available; // 삭제할 노드
	Chunk *temp2 = NULL; // 
	Chunk *del_link = NULL; // 삭제할 노드의 이전 노드를 검색
	int return_mem_start;
	while (current != NULL) { // 메모리 할당할 영역을 검색
		if (req_size <= current->mem_left_size) {
			// 조건 : 요청한 크기가 현재 노드의 남은 용량보다 작은 경우
			return_mem_start = current->mem_start;
			current->mem_start += req_size;
			current->mem_left_size -= req_size;
			// 현재 노드의 값을 변경
			printf("mem allocated succeced\t%3d, size %3d\n", current->mem_start - req_size, req_size);
			if (current->mem_left_size == 0) { // 남은 메모리 용량이 0인 경우 메모리 할당해제 필요
				del_link = available; // 헤드노드부터 검색
				if (current == available) { // 현재 노드가 헤드노드인 경우
					available = current->link;
				}
				else { // 현재 노드가 헤드 노드가 아닌 경우
					while (del_link->link != current) { // 삭제할 노드의 이전 노드를 검색
						del_link = del_link->link;
					}
					del_link->link = current->link;
				}
				delete current;
			}
			// 메모리 시작 주소 반환
			return return_mem_start;
			//return (available->mem_start - req_size);
		}
		current = current->link;
	}
	// error : 메모리 할당 실패
	printf("mem allocated falied\t%3d\n", -1);
	return -1;
}

// F_FIT B_FIT W_FIT

void myfree(int start_addr, int return_size) {
	Chunk *temp = available;

	while (temp->link != NULL) temp = temp->link;
	temp->link = new Chunk;
	temp = temp->link;
	setNode(temp, start_addr, return_size);

	switch (tag) {
	case F_FIT:
		memMerge();
		break;
	case B_FIT: // 정렬 없으면 오류 발생
	case W_FIT:
		bwfitSort();
		memMerge();
		bwfitSort();
		break;
	}

	printf("mem free\n");
}

void setNode(Chunk *temp, int start_addr, int return_size) { // 노드 설정
	temp->link = NULL;
	temp->mem_start = start_addr;
	temp->mem_left_size = return_size;
}

void memMerge() {	// best, worst fit 메모리 병합
	Chunk *current;				// 현재 노드
	Chunk *temp;				// 찾고있는 노드
	Chunk *head = available;	// 검색의 시작이 되는 노드
	Chunk *mid_node;			// 찾고있는 노드의 이전을 가리키는 노드

	while (head != NULL) {
		current = head; // head를 직접 건드리지 않겠다는 굳은 의지
		temp = current->link; // 찾고있는 노드 != 현재 노드

		while (temp != NULL) {
			if (current->mem_start + current->mem_left_size == temp->mem_start) {
				// 현재 노드의 메모리 시작주소 + 메모리 남은 용량 == 찾고있는 노드의 메모리 시작주소 ? 병합 : 다음 노드
				current->mem_left_size += temp->mem_left_size; // 현재 노드의 남은 메모리 용량에 찾고있는 노드의 남은 메모리 용량을 더함
				mid_node = current;	// 찾고있는 노드의 이전 노드를 찾는 과정
				while (mid_node->link != temp) {
					mid_node = mid_node->link;
				}
				mid_node->link = temp->link;	// 1→2→3→4 노드가 있을 때 temp node = 3일 경우, mid node = 2
												// mid node->link = temp node->link로 인해 1→2→4로 변경됨
				delete temp; // 찾고있는 노드 메모리 해제
				temp = current->link;
				continue;	// 찾고있는 노드를 현재 노드의 next값으로 변경, while문 계속 진행
			}
			else if (current->mem_start == temp->mem_start + temp->mem_left_size) {
				// 현재노드의 메모리 시작주소 == 찾고있는 노드의; 메모리 시작주소 + 남은 용량 ? 병합 : 다음 노드
				current->mem_start = temp->mem_start;
				current->mem_left_size += temp->mem_left_size;
				current->link = temp->link;
				// current 노드의 메모리 정보를 합병한 내용으로 변경
				delete temp;
				temp = current->link;
				continue;
			}
			temp = temp->link;
		}
		head = head->link;
	}

}

void bwfitSort() {
	Chunk *temp_1 = available, *temp_2 = NULL;

	for (; temp_1->link != NULL; temp_1 = temp_1->link) {
		for (temp_2 = temp_1->link; temp_2 != NULL; temp_2 = temp_2->link) {
			if (SORT_CONDITION(temp_1, temp_2, tag)) bwfitSwap(temp_1, temp_2);
		}
	}
}

void bwfitSwap(Chunk *temp_1, Chunk *temp_2) { // 데이터 swap
	int mem_left_size, mem_start;

	mem_start = temp_1->mem_start;
	temp_1->mem_start = temp_2->mem_start;
	temp_2->mem_start = mem_start;

	mem_left_size = temp_1->mem_left_size;
	temp_1->mem_left_size = temp_2->mem_left_size;
	temp_2->mem_left_size = mem_left_size;
}

void bwfitMerge() {
	Chunk *temp = available->link;
	Chunk *left = available;
	while (temp != NULL) {
		while (left->mem_left_size + left->mem_start == temp->mem_start) {
			//bwfitMergeData(left, temp);
			left->mem_left_size += temp->mem_left_size;
			temp = temp->link;
			delete left->link;
			left->link = temp;
		}
		left = left->link;
		temp = temp->link;
	}

}

void printMemStat() {
	Chunk *temp = available;

	for (int cnt = 0; temp != NULL; cnt++) {
		printf("(%3d) mem_start_addr : %3d, mem_left_size : %3d\n", cnt + 1, temp->mem_start, temp->mem_left_size);
		temp = temp->link;
	}
}