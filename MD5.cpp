#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const unsigned int loop[64] = {
	 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
	 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
	 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 
 };

const unsigned int S[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

const unsigned int  T[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

unsigned int *MD = new unsigned int[4]{0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476};

int F(int b, int c, int d){
	return (b&c)|((~b)&d);
} 
int G(int b, int c, int d){
	return (b&d)|(c&(~d));
}
int H(int b, int c, int d){
	return b^c^d;
}
int I(int b, int c, int d){
	return c^(b|(~d));
}

unsigned int LS(unsigned int t, unsigned short s){
	unsigned int temp;
	s = s%32;
	temp = t >> (32-s);
	t = t << s;
	t = t|temp;
	return t;
}

void HMD5(void *buffer){
	
	unsigned int A, B, C, D, Temp;
	const unsigned int *X = static_cast<unsigned int*>(buffer);

	A = MD[0];
	B = MD[1];
	C = MD[2];
	D = MD[3];
	
	for(int i = 0;i < 64;i++){

		switch(i/16){ // a = a + g(b,c,d)
			case 0:{
				Temp = F(B, C, D);
				break;
			}	
			case 1:{
				Temp = G(B, C, D);
				break;
			}
			case 2:{
				Temp = H(B, C, D);
				break;
			}
			case 3:{
				Temp = I(B, C, D);
				break;
			}
			default:
				break;
		}
		Temp += X[loop[i]] + T[i] + A;//a = a + g(b,c,d) + X[k] + T[i];
		A = LS(Temp,S[i]) + B;// a = b + ((a + g(b,c,d) + X[k] + T[i]) <<< s);
		
		Temp = D;
		D = C;
		C = B;
		B = A;
		A = Temp;
	}
	MD[0] = A;
	MD[1] = B;
	MD[2] = C;
	MD[3] = D;
}
void Init(const char *msg){
	unsigned int K = strlen(msg);
	unsigned int P = 0;
	if(K*8%512==448){ //char is 8 bits
		P = 512;
	} else{
		P = 448 - (K*8)%512;
	}
	P /= 8; //transform bits to byte
	unsigned int msgLen = K + P + 8;
	unsigned char* buffer = new unsigned char[msgLen];
	for(int i = 0;i < K;i++){
		buffer[i] = msg[i];
	}
	for(int i = K+1;i < msgLen;i++){
		buffer[i] = 0x00;
	}
	buffer[K] = 0x80;//10000000
	buffer[msgLen-5] = (K*8&0xFF000000)>>24;
	buffer[msgLen-6] = (K*8&0x00FF0000)>>16;
	buffer[msgLen-7] = (K*8&0x0000FF00)>>8;
	buffer[msgLen-8] = K*8&0x000000FF;
	
	unsigned int L = msgLen/64;//The length of buffer is msgLen*8 bits, length/512 = msgLen/64
	unsigned int A, B, C, D;
	for (int i = 0;i < L;i++){
		A = MD[0];//CVq
		B = MD[1];
		C = MD[2];
		D = MD[3];
		
		HMD5(&buffer[i*64]);
		
		MD[0] += A;//CVq+1
		MD[1] += B;
		MD[2] += C;
		MD[3] += D;
	}
		
}

int main(){
	char msg[56];//you can allocate more space 
	printf("Please enter the message: ");
	scanf("%s", msg); 
	Init(msg);
	
	printf("MD5: ");
	unsigned char a, b, c, d;
	for(int i = 0;i < 4;i++){
		a = (MD[i]&0xFF000000)>>24;
		b = (MD[i]&0x00FF0000)>>16;
		c = (MD[i]&0x0000FF00)>>8;
		d = MD[i]&0x000000FF;
		printf("%x%x%x%x", d,c,b,a);
	}
	printf("\n");
	return 0;
}
