#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float compararValores(float valor[]);

int main(){
	int entrada;
	
	printf(">>> Insira o valor para analise.\n");
	scanf("%d",&entrada);
	(entrada%2==0)
		? printf("\nO numero %d e Par", entrada)
		: printf("\nO numero %d e Impar",entrada);
return 0;
}
/*
float compararValores(float valor[]){
	int i;
	float resultado;
	for(i=0;i<2;i++){
		if(resultado < valor[i]){
			resultado = valor[i];
		}
	}
	printf("O maior valor digitado foi %2.f",resultado);
}
*/
