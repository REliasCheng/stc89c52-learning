#include <STC89C5xRC.H>

void Delay100ms(void)	//@11.0592MHz
{
	unsigned char data i, j;

	i = 180;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}


void main(){
	unsigned char tmp = 0x01;
	while(1){
		P0=~tmp;
		Delay100ms();
		tmp <<=1;
		
		if(tmp == 0x00){
			tmp = 0x01;
		}
	}
}





