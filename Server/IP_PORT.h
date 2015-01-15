#define NULL 0
#pragma once
#pragma pack(1)
struct IP_PORT {
	unsigned int IP; //4 ����� ��� IP
	unsigned short PORT;//2 ����� �� ����
	IP_PORT() {IP=0; PORT=0;}
	bool operator==(const IP_PORT&aa) {return ((IP == aa.IP) && (PORT == aa.PORT));}
	bool operator<(const IP_PORT&aa) {
		if (IP < aa.IP) return true;
		if (IP == aa.IP) return PORT<aa.PORT;
		return false;
	}
};
#pragma pack()
typedef struct IP_PORT IP_PORT;