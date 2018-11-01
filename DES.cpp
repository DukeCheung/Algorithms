#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const static int IP_Table[64] = {//IP permutation table 
    58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4, 
    62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8, 
    57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3, 
    61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7 
};

const static int IPR_Table[64] = {//IP inverse permutation table
    40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31, 
    38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29, 
    36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27, 
    34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25 
};

const static int E_Table[48] = {// E expand table
    32,1,2,3,4,5,4,5,6,7,8,9, 
    8,9,10,11,12,13,12,13,14,15,16,17, 
    16,17,18,19,20,21,20,21,22,23,24,25, 
    24,25,26,27,28,29,28,29,30,31,32,1 
};

const static int S_Box[8][4][16] = {// S box
    // S1  
    14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7, 
    0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8, 
    4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0, 
    15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13, 
    //S2 
    15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10, 
    3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5, 
    0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15, 
    13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9, 
    //S3 
    10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8, 
    13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1, 
    13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7, 
    1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12, 
    //S4 
    7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15, 
    13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9, 
    10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4, 
    3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14, 
    //S5 
    2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9, 
    14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6, 
    4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14, 
    11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3, 
    //S6 
    12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11, 
    10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8, 
    9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6, 
    4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13, 
    //S7 
    4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1, 
    13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6, 
    1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2, 
    6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12, 
    //S8 
    13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7, 
    1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2, 
    7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8, 
    2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 
};

const static int P_Table[32] = {// P permutation table
    16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10, 
    2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25 
};

const static int PC1_Table[56] = {// PC1 permutation table
    57,49,41,33,25,17,9,1,58,50,42,34,26,18, 
    10,2,59,51,43,35,27,19,11,3,60,52,44,36, 
    63,55,47,39,31,23,15,7,62,54,46,38,30,22, 
    14,6,61,53,45,37,29,21,13,5,28,20,12,4 
};

const static int PC2_Table[48] = {// PC2 permutation table
    14,17,11,24,1,5,3,28,15,6,21,10, 
    23,19,12,4,26,8,16,7,27,20,13,2, 
    41,52,31,37,47,55,30,40,51,45,33,48, 
    44,49,39,56,34,53,46,42,50,36,29,32 
};

const static int LS_Table[16] = {//left shift table
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 
};

bool subKey[16][48];//subKey

void IP_Permutation(bool *l, bool *r, bool *m){
	bool temp[64];//store the result after permutating
	for(int i = 0;i < 64;i++){
		temp[i] = m[IP_Table[i]-1];
	}
	for(int i = 0;i < 64;i++){
		if(i<32){
			l[i] = temp[i];//store the left part
		}
		else{
			r[i-32] = temp[i];//store the right part
		}
	}
}

void IP_Inverse(bool *in){//IP inverse permutation
	bool temp[64];
	for(int i = 0; i < 64;i++){
		temp[i] = in[IPR_Table[i]-1];
	}
	for(int i = 0;i < 64;i++){
		in[i] = temp[i];//update the data
	}
}

void E_Permutation(bool *in, bool *out){
	for(int i = 0;i < 48;i++){
		out[i] = in[E_Table[i]-1];//put the result in out
	}
}

void P_Permutation(bool *in, bool *out){
	for(int i = 0;i < 32;i++){
		out[i] = in[P_Table[i]-1];//put the result in out
	}
}

void XOR(bool *r, bool *l, bool *f){//XOR operation
	for(int i = 0;i < 32;i++){
		r[i] = l[i]^f[i];//put the result in r
	}
}

void Feistel(bool *r, bool *k, bool *o){//feistel function
	bool out[48];
	E_Permutation(r, out);//expand to 48 bits
	for(int i = 0;i < 48;i++){
		out[i] ^= k[i];//XOR operation
	}
	
	bool s[32] = {0};
	
	for(int i = 0;i < 8;i++){
		int h = (out[i*6]<<1)+out[i*6+5];
		int l = (out[i*6+1]<<3)+(out[i*6+2]<<2)+(out[i*6+3]<<1)+out[i*6+4];
		int data = S_Box[i][h-1][l-1];//select the S Box data
		s[i*8] = (data&8)>>3;//convert decimal number to binary number
	    s[i*8+1] = (data&4)>>2;
	    s[i*8+2] = (data&2)>>1;
	    s[i*8+3] = data&1;
	}
	
	P_Permutation(s, o);//put the result in o
}

void Iteration_En(bool *l, bool *r, bool *out){//use the different sequences of subKey
	bool temp[32];

	bool fl[32] = {0};
	for(int i = 0;i < 32;i++){
		temp[i] = l[i];//temp is L(i-1)
	}
	for(int i = 1;i < 17;i++){
		
		//L(i) = R(i-1)
		for(int j = 0;j < 32;j++){
			l[j] = r[j];
		}
		//fl is the result
		Feistel(r, subKey[i-1], fl);//the order is from 0 to 15
		
		//r = L(i-1) XOR fl
		XOR(r, temp, fl);
		
		//L(i-1) = L(i)
		for(int j = 0;j < 32;j++){
			temp[j] = l[j];
		}
	}
	
	//out = R16L16
	for(int i = 0;i < 64;i++){
		if(i < 32){
			out[i] = r[i];
		}
		else{
			out[i] = l[i-32];
		}
	}
	
}

void Iteration_De(bool *l, bool *r, bool *out){//use the different sequences of subKey
	bool temp[32];

	bool fl[32] = {0};
	for(int i = 0;i < 32;i++){
		temp[i] = l[i];//temp is L(i-1)
	}
	for(int i = 1;i < 17;i++){
		//L(i) = R(i-1)
		for(int j = 0;j < 32;j++){
			l[j] = r[j];
		}
		//fl is the result
		Feistel(r, subKey[16-i], fl);//the order is from 15 to 0
		
		//r = L(i-1) XOR fl
		XOR(r, temp, fl);
		
		//L(i-1) = L(i)
		for(int j = 0;j < 32;j++){
			temp[j] = l[j];
		}
	}
	
	//out = R16L16
	for(int i = 0;i < 64;i++){
		if(i < 32){
			out[i] = r[i];
		}
		else{
			out[i] = l[i-32];
		}
	}
	
}

void PC1_Permutation(bool *key, bool *c, bool *d){
	bool temp[56];
	
	for(int i = 0;i < 56;i++){
		temp[i] = key[PC1_Table[i]-1];//put the result in temp
	}
	
	//c is the left part, r is the right part
	for(int i = 0;i < 56;i++){
		if(i < 28){
			c[i] = temp[i];
		}
		else{
			d[i] = temp[i-28];
		}
	}
}

void LS(bool *s, int times){//left shift times bits

	for(int i = 0;i < times;i++){
		for(int j = 0;j < 27;j++){
			s[j] = s[j+1];
		}
		//set the last bit to 0
		s[27] = false;
	}
}

void PC2_Permutation(bool *c, bool *d, int times){
	bool temp[56];
	for(int i = 0;i < 56;i++){
		if(i < 28){
			temp[i] = c[i];
		}
		else{
			temp[i] = d[i-28];
		}
	}
	
	for(int i = 0;i < 48;i++){
		subKey[times][i] = temp[PC2_Table[i]-1];//put the subKey in the array
	}
}

void Output(bool *s){//convert the binary number to hexadecimal number
	for(int i = 0;i < 16;i++){
		int data = s[i*4];
		data = data*2+s[i*4+1];
		data = data*2+s[i*4+2];
		data = data*2+s[i*4+3];
		printf("%X",data);
	}
}

void setSubkey(bool *key){//set the subKey
	bool k[56], c[28], d[28];
	int j = 0;
	
	for(int i = 0;i < 64;i++){
		if((i+1)%8!=0){
			k[j] = key[i];
			j++;
		}
	}
	
	PC1_Permutation(k,c,d);//get C0 and R0
	
	for(int i = 0;i < 16;i++){
		LS(c, LS_Table[i-1]);//C(i) = LS(i)(C(i-1))
		LS(d, LS_Table[i-1]);
		PC2_Permutation(c, d, i);//set the subKey
	}
}

void Encrypt(bool *m, bool *key, bool *L, bool *R){//Encrypt operation

	bool out[64];//result
	setSubkey(key);
	IP_Permutation(L, R, m);//get L0 and R0
	Iteration_En(L, R, out);
	IP_Inverse(out);//get ciphertext
	printf("The ciphertext is: ");
	Output(out);//output the result
	printf("\n");	
}

void Decrypt(bool *m, bool *key, bool *L, bool *R){//Decrypt operation
	
	bool out[64];
	setSubkey(key);
	IP_Permutation(L, R, m);//get L0 and R0
	Iteration_De(L, R, out);
	IP_Inverse(out);//get plaintext
	printf("The plaintext is: ");
	Output(out);//output the result
	printf("\n");
}

void Input(bool *m, char *s){//convert the hexadecimal number to binary number
	for(int i = 0;i < 16;i++){
		int data = 0;
		if(s[i]>='0'&&s[i]<='9'){
			data = s[i]-'0';
		    m[i*4] = (data&8)>>3;
	        m[i*4+1] = (data&4)>>2;
		    m[i*4+2] = (data&2)>>1;
		    m[i*4+3] = data&1;
		}
		else if(s[i]>='a'&&s[i]<='f'){
			data = s[i]-'a'+10;
		    m[i*4] = (data&8)>>3;
	        m[i*4+1] = (data&4)>>2;
		    m[i*4+2] = (data&2)>>1;
		    m[i*4+3] = data&1;
		}
		else if(s[i]>='A'&&s[i]<='F'){
			data = s[i]-'A'+10;
		    m[i*4] = (data&8)>>3;
	        m[i*4+1] = (data&4)>>2;
		    m[i*4+2] = (data&2)>>1;
		    m[i*4+3] = data&1;
		}
	}
}

int main(){
	bool M[64];//ciphertext or plaintext
	bool L[32];//Left part of M
	bool R[32];//Right part of M
	bool K[64];//Key
	
	char flag[10] = {0};
	while(strlen(flag)!=1||flag[0]!='1'&&flag[0]!='2'){
		printf("[1] Encrypt\n"
	           "[2] Decrypt\n"
		       "Please enter: ");
	    scanf("%s", flag);
	}
	
	if(flag[0]=='1'){
		printf("Please enter the plaintext: ");
		char str[17] = {0};
	    scanf("%s", str);
	    Input(M, str);
	    printf("Please enter the Key: ");
	    scanf("%s", str);
	    Input(K, str);
	    Encrypt(M, K, L, R);
	}
	else{
		printf("Please enter the ciphertext: ");
		char str[17] = {0};
	    scanf("%s", str);
	    Input(M, str);
	    printf("Please enter the Key: ");
	    scanf("%s", str);
	    Input(K, str);
	    Decrypt(M, K, L, R);
	}
	system("pause");
	return 0;
}
