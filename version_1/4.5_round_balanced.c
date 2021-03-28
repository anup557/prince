#include<stdio.h>
#include<stdlib.h>
#include "prince_version_1_oracle.h"
#include "prince_v_1_necessary_functions.h"

#define nibble_one 0
#define nibble_two 1
#define nibble_three 2

long four_point_five_round_distinguisher(long msg){
	long k0 = 0x0123456789abcdef, k1 = 0xfedcba9876543210,
	rc[12] = {0x0000000000000000,0x13198a2e03707344,0xa4093822299f31d0,0x082efa98ec4e6c89,0x452821e638d01377,0xbe5466cf34e90c6c,0x7ef84f78fd955cb1,0x85840851f1ac43aa,0xc882d32f25323c54,0x64a51195e0e3610d,0xd3b5a399ca0c2399,0xc0ac29b7c97c50dd};  

//	msg = msg^k0;	//

	msg = SR(MC(SB(rc[3]^k1^msg)));

	msg = SR(MC(SB(rc[4]^k1^msg)));					// 2 forward rounds

	msg = inv_SB(MC(SB(rc[5]^k1^msg)));					// middle round

	msg = MC(inv_SR(k1^rc[6]^msg));					// 1 incomplete backward round i.e. without inv_SB

	msg = msg^k1^rc[7];		//
//	msg = msg^circ_shift(k0,1)^(k0>>63);	//

	return msg;		
	}

	
long* msg_list(int nibble_1, int nibble_2, int nibble_3){
	long *msg_list, limit = (1<<12);
	msg_list = (long*)malloc(limit*sizeof(long));
	
	for(long i=0; i<limit; i++)
		msg_list[i] = ((i&0xf)<<(60 - 4*nibble_1))|(((i>>4)&0xf)<<(60 - 4*nibble_2))|(((i>>8)&0xf)<<(60 - 4*nibble_3));

	
	return msg_list;

	}
	
int main(){
	long *cipher_list, *message_list;
	long limit = (1<<12);
	cipher_list = (long*)malloc(limit*sizeof(long));
	message_list = msg_list(nibble_one, nibble_two, nibble_three);

	for(long i=0;i<limit;i++){
		cipher_list[i] = four_point_five_round_distinguisher(message_list[i]);

		}
		
	check_balanced(cipher_list, limit);		// checking whether the stored 2^12 cipher texts balanced or not

//check_all_property_in_64_bit(cipher_list, limit, 0xff00000000000000);		


	}
	
	
